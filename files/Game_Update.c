#include "Game.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

void Game_ChooseClosestPlayerFromEnemy(MainData *_data);
void GameMoneyDrop(Engine *_engine, Money** _List, sfVector2f _tempPos);
void GameIntro(MainData* _data, float _dt);
void GameOutro(MainData* _data, float _dt);
void Game_CheckIfDefeated(Engine *_engine, GameData *_state);
void Game_UpdateIndestructibleMode(Engine *_engine, MainData *_data);
void DestroyEverythingWhenTheBossShowsUp(GameData *_state);

// -- MAIN FUNCTION -------------------------------------------------------- //

void Game_Update(MainData *_data, float _dt)
{
	// Create a shortcut to the state data
	GameData *state = _data->stateData[GAME];
	sfVector2f tempPos = { 0, 0 };

	if (!state->isDefeated)
	{
		_data->engine.time += _dt;
		Mechanic_SpawnEnemies(_data, &state->level, state->enemies, &state->players, _dt, &state->boss, &_data->engine.currentDifficulty);
		Map_Update(&_data->engine, &state->level.map, _dt);
		Money_Update(&state->moneyBills, _dt, _data->engine.configuration->resolution.y);
		if (state->level.map.time >= 0 && state->boss.isAlive)
		{
			Player_HandleMovementInput(&state->players);
			HandleShootInput(&_data->engine, &state->players, &state->projectiles[WEAPON_OWNER_PLAYER], state->lasers);
		}
		Skill_Update(_dt, state);
		if (state->level.map.time >= 0 && state->boss.isAlive)
		{
			Player_Update(&_data->engine, &state->players, _dt, state->lasers);
		}
		Enemy_Update(&_data->engine, state->enemies, _dt, &state->projectiles[WEAPON_OWNER_ENEMY], &state->explosions, state->lasers, &state->moneyBills);
		for (int projectileType = 0; projectileType < WEAPON_OWNER_SIZE; projectileType++)
		{
			Projectile_Update(&_data->engine, _dt, &state->projectiles[projectileType], state, _data->currentState);
		}
		Boss_Update(&_data->engine, &state->boss, &state->projectiles[WEAPON_OWNER_ENEMY], state->lasers, &state->music, state->enemies, &state->level.paths);
		Boss_UpdateTurret(&_data->engine, &state->boss, &state->players, &state->projectiles[WEAPON_OWNER_ENEMY]);
		Laser_Update(state->lasers, &_data->engine);
		StarsBackground_Update(&state->starBackground, _dt, &_data->engine);

		// Handle collisions
		Game_HandleBossCollisions(_data);
		if (state->level.map.time >= 0 && state->boss.isAlive)
		{
			Game_HandlePlayersCollisions(_data);
		}
		Game_HandleProjectilesCollisions(_data);
		Game_HandleEnemiesCollisions(_data, &tempPos);
		Game_HandleLasersCollisions(_data, _dt, &tempPos);

		Achievement_UpdateStageAchievement(_data);
		Explosion_Update(&_data->engine, &state->explosions, _dt);
		Game_ChooseClosestPlayerFromEnemy(_data);
		Game_UpdateIndestructibleMode(&_data->engine, _data);
		GameMoneyDrop(&_data->engine, &state->moneyBills, tempPos);

		DestroyEverythingWhenTheBossShowsUp(state);

		GameIntro(_data, _dt);
		GameOutro(_data, _dt);

		Game_CheckIfDefeated(&_data->engine, state);

		// Return to the editor when tlevel is in testing mode
		if (state->level.map.time >= state->level.bossTime && _data->isLaunchedFromEditor)
		{
			_data->currentState = EDITOR;
		}

		// Clean the lists
		for (int projectileType = 0; projectileType < WEAPON_OWNER_SIZE; projectileType++)
		{
			List_Clear(&state->projectiles[projectileType]);
		}
		List_Clear(&state->explosions);
	}
	else
	{
		StarsBackground_Update(&state->starBackground, _dt, &_data->engine);
		Map_Update(&_data->engine, &state->level.map, _dt);
		UpdateAnimation(_data->engine.dt, state->defeatScreen, &state->defeatSreenEntity);
	}
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //

void Game_ReturnToOverworld(MainData* _data)
{
	GameData *state = _data->stateData[GAME];
	SaveFinalScore(_data, state->squadScore, _data->levelName);
	_data->currentState = OVERWORLD;
	sfMusic_stop(state->music);
}

void Game_ChooseClosestPlayerFromEnemy(MainData *_data)
{
	GameData *state = _data->stateData[GAME]; // Create a shortcut to the state data
	for (size_t iEnemy = 0; iEnemy < state->enemies->number; iEnemy++)
	{
		Enemy *currentEnemy = state->enemies->array[iEnemy];
		Player *playerToAim = NULL;
		float minimumDistance = 10000000000000.0f;
		for (size_t iPlayer = 0; iPlayer < state->players.number; iPlayer++)
		{
			float distance = 0.0f;
			Player *currentPlayer = state->players.array[iPlayer];
			distance = Angle_CalculateDistance(currentEnemy->entity.position, currentPlayer->entity.position);
			distance = fabsf(distance);
			if (distance < minimumDistance && currentPlayer->isAlive && currentPlayer->canBeTargeted)
			{
				minimumDistance = distance;
				playerToAim = state->players.array[iPlayer];
			}
		}
		currentEnemy->target = playerToAim;
	}
}

void GameMoneyDrop(Engine *_engine, Money** _List, sfVector2f _tempPos)
{
	if (_tempPos.x || _tempPos.y)
	{
		if (100 * (float)rand() / RAND_MAX <= GAME_DROP_RATE)
		{
			float random = 100 * (float)rand() / RAND_MAX;
			if (random <= 10)
			{
				Money_Create(_engine, _List, _tempPos, MONEY_1000);
			}
			else if (random <= 40)
			{
				Money_Create(_engine, _List, _tempPos, MONEY_500);
			}
			else
			{
				Money_Create(_engine, _List, _tempPos, MONEY_100);
			}
		}
	}
}

void GameIntro(MainData* _data, float _dt)
{
	GameData *state = _data->stateData[GAME];
	sfVector2i tempResolution = _data->engine.configuration->resolution;

	if (state->level.map.time < 0)
	{
		// Update y position of players
		state->introSpawn.y += PLAYER_SPEED * _dt;
		// Separation of players
		if (state->introSpawn.y > tempResolution.y)
		{
			for (int i = 0; i < state->players.number; i++)
			{
				sfVector2f* tempPos = &state->players.array[i]->entity.position;
				state->players.array[i]->currentAnim = state->players.array[i]->idle;
				if (tempPos->x > (i + 1) * tempResolution.x / (state->players.number + 1))
				{
					state->players.array[i]->currentAnim = state->players.array[i]->leftAnim;
					tempPos->x -= PLAYER_SPEED * PLAYER_X_VELOCITY_MULTIPLIER * _dt;
					if (tempPos->x < (i + 1) * tempResolution.x / (state->players.number + 1))
					{
						tempPos->x = (float)((i + 1) * tempResolution.x / (state->players.number + 1));
					}
				}
				if (tempPos->x < (i + 1) * tempResolution.x / (state->players.number + 1))
				{
					state->players.array[i]->currentAnim = state->players.array[i]->rightAnim;
					tempPos->x += PLAYER_SPEED * PLAYER_X_VELOCITY_MULTIPLIER * _dt;
					if (tempPos->x > (i + 1) * tempResolution.x / (state->players.number + 1))
					{
						tempPos->x = (float)((i + 1) * tempResolution.x / (state->players.number + 1));
					}
				}
			}
		}
	}
}

void GameToShop(MainData* _data)
{
	_data->currentState = SHOP;
}

void GameOutro(MainData* _data, float _dt)
{
	GameData *state = _data->stateData[GAME];
	sfBool isBossNull = FALSE;
	float baseMusicVolume = _data->engine.configuration->musicVolume;

	if (_strcmpi("NULL", state->boss.name) == 0)
	{
		isBossNull = TRUE;
	}

	// With boss
	if (!state->boss.isAlive && state->level.map.time > state->level.bossTime && isBossNull == FALSE)
	{
		sfVector2i tempResolution = _data->engine.configuration->resolution;
		static float timerScoreboard = 0;

		timerScoreboard += _dt;

		// Slowly lower music volume
		sfMusic_setVolume(state->music, _data->engine.configuration->musicVolume - (timerScoreboard / GAME_SCOREBOARD_TIMER_MAX) * (baseMusicVolume / 2));
		// Clear projectiles
		for (int projectileType = 0; projectileType < WEAPON_OWNER_SIZE; projectileType++)
		{
			List_Destroy(&state->projectiles[projectileType]);
		}
		///int noPlayerInScreen = sfTrue;
		state->boss.entity.position.y += MAP_SCROLLING_SPEED * _dt;
		state->outroCooldown += _dt;
		if (state->outroCooldown >= 0.1f)
		{
			sfVector2f tempPos = state->boss.entity.position;
			sfFloatRect tempBounds = sfSprite_getGlobalBounds(state->boss.phases[state->boss.nbPhases - 1].animation->sprites[0]);
			sfVector2f tempSize = { tempBounds.width, tempBounds.height };

			///RectangleCollider* tempCollider = state->boss.phases[state->boss.nbPhases - 1].impactCollider.pointer;
			//sfVector2f tempSize = tempCollider->size;
			Explosion_Create(&_data->engine, &state->explosions, (sfVector2f) { tempPos.x + rand() % (int)tempSize.x - tempSize.x / 2, tempPos.y + rand() % (int)tempSize.y - tempSize.y / 2 }, EXPLOSION_SIZE_DEFAULT, EXPLOSION_TYPE_DEFAULT);
			state->outroCooldown = 0;
		}
		for (int i = 0; i < state->players.number; i++)
		{
			state->players.array[i]->entity.position.y -= PLAYER_SPEED * _dt;
			state->players.array[i]->currentAnim = state->players.array[i]->idle;
			///noPlayerInScreen = state->players.array[i]->entity.position.y > -300 ? sfFalse : noPlayerInScreen;
		}
		if (state->boss.entity.position.y >= tempResolution.y + 300)
		{
			if ((_data->engine.world.levelNames.number - 1) > state->levelIndex)
			{
				_data->engine.world.currentLevel = state->levelIndex + 1;
			}
			else
			{
				/*SaveFinalScore(_data, state->squadScore, _data->levelName);
				_data->currentState = OVERWORLD;*/
				Fade_CreateTransition(&_data->transitionFade, &Game_ReturnToOverworld);
			}
			timerScoreboard = 0;
		}
		if (state->scoreboardPos.x < tempResolution.x / 2)
		{
			state->scoreboardPos.x += 1000.0f * _dt;
			if (state->scoreboardPos.x > tempResolution.x / 2.f)
			{
				state->scoreboardPos.x = tempResolution.x / 2.f;
			}
		}
	}
	// Without boss
	else if (!state->boss.isAlive && state->level.map.time > state->level.bossTime && isBossNull == TRUE)
	{
		sfVector2i tempResolution = _data->engine.configuration->resolution;
		static float timerScoreboard = 0;

		timerScoreboard += _dt;

		// Slowly lower music volume
		sfMusic_setVolume(state->music, _data->engine.configuration->musicVolume - (timerScoreboard/ GAME_SCOREBOARD_TIMER_MAX) * (baseMusicVolume/2));
		// Clear projectiles
		for (int projectileType = 0; projectileType < WEAPON_OWNER_SIZE; projectileType++)
		{
			List_Destroy(&state->projectiles[projectileType]);
		}
		// Move players up
		for (int i = 0; i < state->players.number; i++)
		{
			state->players.array[i]->entity.position.y -= PLAYER_SPEED * _dt;
			state->players.array[i]->currentAnim = state->players.array[i]->idle;
		}
		// Update scoreboard position
		if (state->scoreboardPos.x < tempResolution.x / 2)
		{
			state->scoreboardPos.x += 1000.0f * _dt;
			if (state->scoreboardPos.x > tempResolution.x / 2.f)
			{
				state->scoreboardPos.x = tempResolution.x / 2.f;
			}
		}
		// Next level / Return to lobby
		if (timerScoreboard > GAME_SCOREBOARD_TIMER_MAX)
		{
			timerScoreboard = 0;
			if ((_data->engine.world.levelNames.number - 1) > state->levelIndex)
			{
				_data->engine.world.currentLevel++;
			}
			else
			{
				_data->engine.world.currentLevel = 0;
				Fade_CreateTransition(&_data->transitionFade, &Game_ReturnToOverworld);
			}
		}
	}

	if (state->levelIndex != _data->engine.world.currentLevel && !_data->isLaunchedFromEditor)
	{
		Fade_CreateTransition(&_data->transitionFade, GameToShop);
	}
}

void Game_CheckIfDefeated(Engine *_engine, GameData *_state)
{
	_state->isDefeated = TRUE;
	for (int iPlayer = 0; iPlayer < _state->players.number; iPlayer++)
	{
		if (_state->players.array[iPlayer]->isAlive)
		{
			_state->isDefeated = FALSE;
		}
	}
}

void Game_UpdateIndestructibleMode(Engine *_engine, MainData *_data)
{
	GameData *state = _data->stateData[GAME];
	if (state->isIndestructible || _data->isLaunchedFromEditor)
	{
		for (int i = 0; i < state->players.number; i++)
		{
			Player *player = state->players.array[i];
			player->armor = player->maxArmor;
		}
	}
}

void DestroyEverythingWhenTheBossShowsUp(GameData *_state)
{
	if (_state->boss.isInIntroduction && _state->boss.isActive && _state->hasClearedEverything == sfFalse)
	{
		for (WeaponOwner projectileType = WEAPON_OWNER_ENEMY; projectileType < WEAPON_OWNER_SIZE; projectileType++)
		{
			for (ListElement *tempElement = _state->projectiles[projectileType].first; tempElement != NULL; tempElement = _state->projectiles[projectileType].first)
			{
				List_DestroyElementFromPointer(&_state->projectiles[projectileType], tempElement, 1);
			}
		}

		for (int i = 0; i < _state->enemies->number; i++)
		{
			_state->enemies->array[i]->health = 0.f;
		}

		_state->hasClearedEverything = sfTrue;
	}
}