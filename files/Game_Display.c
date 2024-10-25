#include "Game.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

void Game_HUDDisplay(MainData* _data);
void Game_DisplayMapTimer(MainData *_data);
void Game_DisplayPlayerID(MainData *_data);
void GameOutroDisplay(MainData* _data);
void Game_DisplayScoreboard(MainData* _data);

// -- MAIN FUNCTION -------------------------------------------------------- //

void Game_Display(MainData *_data)
{

	// Create a shortcut to the state data
	GameData *state = _data->stateData[GAME];
	sfVector2f tempScale = V2F_MUL_VALUE(_data->engine.configuration->scale, 4.0f);
	sfBool isInDebug = FALSE;
#ifdef DEBUG
	isInDebug = TRUE;
#endif // DEBUG
	if (!state->isDefeated)
	{
		Map_Display(&_data->engine, &state->starBackground, &state->level);
		Map_DisplayForeground(&_data->engine, state->level.map);
		Shadow_DisplayPlayersShadow(&_data->engine, &state->players, !state->level.isInSpace);
		Shadow_DisplayEnemiesShadow(&_data->engine, state->enemies, !state->level.isInSpace);
		Shadow_DisplayBossShadow(&_data->engine, &state->boss, !state->level.isInSpace);
		if (state->level.map.time < 0.0f)
		{
			Sprite_Display(_data->engine.canvas, state->intro, state->introSpawn, tempScale, -90.0f, NULL);
		}
		Money_Display(&state->moneyBills, state->bill, &_data->engine);
		Boss_Display(&_data->engine, &state->boss);
		Laser_Display(&_data->engine, state->lasers);
		Enemy_Display(&_data->engine, state->enemies);
		GameOutroDisplay(_data);
		for (int projectileType = 0; projectileType < WEAPON_OWNER_SIZE; projectileType++)
		{
			Projectile_Display(&_data->engine, &state->projectiles[projectileType]);
		}
		Skill_Display(&_data->engine, state);
		Player_Display(&_data->engine, &state->players);
		Explosion_Display(&_data->engine, &state->explosions);
		if (state->boss.isAlive && state->boss.isActive)
		{
			if (state->boss.isInIntroduction)
			{
				DisplayAnimation(_data->engine.canvas, state->boss.intro, &state->boss.introEntity, _data->engine.gameScale, NULL);
			}
			else
			{
				if (state->hasInterfaceDisplayed)
				{
					Game_HUDDisplay(_data);
				}
			}
		}
		else
		{
			if (state->hasInterfaceDisplayed)
			{
				Game_HUDDisplay(_data);
			}
		}
		if (state->hasInterfaceDisplayed)
		{
			Game_DisplayScoreboard(_data);
		}
#ifndef DEBUG
		Shader_ApplyBloomToCanvas(_data->engine.canvas);
#endif // !DEBUG
		if (isInDebug || _data->isLaunchedFromEditor)
		{
			Game_DisplayMapTimer(_data);
		}
		///printf("Score : %d\n", state->players.array[0]->score);
		///Path_Display(&_data->engine, &state->level.paths);
	}
	else
	{
		printf("ELSE \n");
		Map_Display(&_data->engine, &state->starBackground, &state->level);
		Map_DisplayForeground(&_data->engine, state->level.map);
		Shader_ApplyBloomToCanvas(_data->engine.canvas);
		DisplayAnimation(_data->engine.canvas, state->defeatScreen, &state->defeatSreenEntity, tempScale, NULL);
	}
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //

void Game_DisplayPlayerSkillAmount(MainData *_data, sfVector2f _position, int _player, int _isTransparent)
{
	GameData *state = _data->stateData[GAME];// Create a shortcut to the state data
	char amount[BUFFER_SMALL];
	sprintf(amount, "%d/%d", state->players.array[_player]->skillInfo.nbSkillInStorage, state->players.array[_player]->skillInfo.skillStorageCap);
	state->parametersText.color = (sfColor) { 0, 255, 155, _isTransparent ? 51 : 255 };
	Text_Display(_data->engine.canvas, &state->parametersText, amount, _position, (sfVector2f) { 0.5f, 0.5f });
	state->parametersText.color = sfWhite;
}

void Game_HUDDisplay(MainData* _data)
{
	GameData *state = _data->stateData[GAME];
	sfVector2i tempResolution = _data->engine.configuration->resolution;
	sfVector2f tempScale = _data->engine.configuration->scale;
	float tempX[MAX_PLAYER_NUMBER] = { (float)(tempResolution.x / 5), (float)(2 * tempResolution.x / 5), (float)(3 * tempResolution.x / 5), (float)(4 * tempResolution.x / 5) };
	float tempY[MAX_PLAYER_NUMBER] = { (float)(10 * tempResolution.y / 11), (float)(10 * tempResolution.y / 11), (float)(10 * tempResolution.y / 11), (float)(10 * tempResolution.y / 11) };
	float scale = 3.f * tempScale.x;
#ifdef DEBUG
	Game_DisplayPlayerID(_data);
#endif
	Score_Display(state->squadScore, &_data->engine, &state->parametersText);
	MoneyAmount_Display(&_data->engine, state->collectedBills, state);
	for (int i = 0; i < state->players.number; i++)
	{
		int isTransparent = sfFalse;
		char alpha = 51;
		int index = 0;
		for (int j = 0; j < state->players.number; j++)
		{
			sfVector2f tempPos = state->players.array[j]->entity.position;
			float distance = (float)sqrt(pow(tempPos.x - tempX[i], 2) + pow(tempPos.y - tempY[i], 2));
			isTransparent = distance <= tempResolution.y / 5 ? sfTrue : isTransparent;
		}
		for (int j = 0; j < _data->engine.skills.nbr; j++)
		{
			index = _data->engine.skills.array[j]->type == state->players.array[i]->skillInfo.type ? j : index;
		}
		if (isTransparent)
		{
			sfSprite_setColor(state->HUD.HUD, (sfColor) { 255, 255, 255, alpha });
			sfSprite_setColor(_data->engine.skills.array[index]->icon, (sfColor) { 255, 255, 255, alpha });
			sfSprite_setColor(state->HUD.shield, (sfColor) { 255, 255, 255, alpha });
			sfSprite_setColor(state->HUD.armorBegin, (sfColor) { 255, 255, 255, alpha });
			sfSprite_setColor(state->HUD.armorBody, (sfColor) { 255, 255, 255, alpha });
			sfSprite_setColor(state->HUD.armorEnd, (sfColor) { 255, 255, 255, alpha });
			sfSprite_setColor(state->HUD.energyBegin, (sfColor) { 255, 255, 255, alpha });
			sfSprite_setColor(state->HUD.energyBody, (sfColor) { 255, 255, 255, alpha });
			sfSprite_setColor(state->HUD.energyEnd, (sfColor) { 255, 255, 255, alpha });
		}
		else
		{
			sfSprite_setColor(state->HUD.HUD, sfWhite);
			sfSprite_setColor(_data->engine.skills.array[index]->icon, sfWhite);
			sfSprite_setColor(state->HUD.shield, sfWhite);
			sfSprite_setColor(state->HUD.armorBegin, sfWhite);
			sfSprite_setColor(state->HUD.armorBody, sfWhite);
			sfSprite_setColor(state->HUD.armorEnd, sfWhite);
			sfSprite_setColor(state->HUD.energyBegin, sfWhite);
			sfSprite_setColor(state->HUD.energyBody, sfWhite);
			sfSprite_setColor(state->HUD.energyEnd, sfWhite);
		}
		Sprite_Display(_data->engine.canvas, state->HUD.HUD, (sfVector2f) { tempX[i], tempY[i] }, (sfVector2f) {
			scale, scale
		}, 0, NULL);
		Sprite_Display(_data->engine.canvas, _data->engine.skills.array[index]->icon, (sfVector2f) { tempX[i] - 29 * scale, tempY[i] - 6.5f * scale }, (sfVector2f) { scale * 1.5, scale * 1.5 }, 0, NULL);
		Game_DisplayPlayerSkillAmount(_data, (sfVector2f) { tempX[i] - 15.0f * scale/*- 29 * scale*/, tempY[i] + 14.0f * scale }, i, isTransparent);
		if (state->players.array[i]->protectionShield > 0)
		{
			int j = 0;
			int toDraw = (int)(state->players.array[i]->shield * 3 / state->players.array[i]->maxShield);
			for (j = 0; j < state->players.array[i]->protectionShield; j++)
			{
				Sprite_Display(_data->engine.canvas, state->HUD.shield, (sfVector2f) { tempX[i] - 2 * scale + j * 17 * scale, tempY[i] - 14.5f * scale }, (sfVector2f) { scale, scale }, 0, NULL);
			}
		}
		if (state->players.array[i]->armor > 0)
		{
			int j = 0;
			int toDraw = (int)(state->players.array[i]->armor * 51 / state->players.array[i]->maxArmor);
			Sprite_Display(_data->engine.canvas, state->HUD.armorBegin, (sfVector2f) { tempX[i] - 11 * scale, tempY[i] - 3.5f * scale }, (sfVector2f) { scale, scale }, 0, NULL);
			for (j = 0; j < toDraw - 1; j++)
			{
				Sprite_Display(_data->engine.canvas, state->HUD.armorBody, (sfVector2f) { tempX[i] - 9 * scale + j * scale, tempY[i] - 3.5f * scale }, (sfVector2f) { scale, scale }, 0, NULL);
			}
			Sprite_Display(_data->engine.canvas, state->HUD.armorEnd, (sfVector2f) { tempX[i] - 9 * scale + j * scale, tempY[i] -3.5f * scale }, (sfVector2f) { scale, scale }, 0, NULL);
		}
		if (state->players.array[i]->skillInfo.currentScore > 0)
		{
			int j = 0;
			int toDraw = (int)(state->players.array[i]->skillInfo.currentScore * 56 / state->players.array[i]->skillInfo.scoreToUnlock);
			Sprite_Display(_data->engine.canvas, state->HUD.energyBegin, (sfVector2f) { tempX[i] - 16 * scale, tempY[i] + 5.5 * scale }, (sfVector2f) { scale, scale }, 0, NULL);
			for (j = 0; j < toDraw - 1; j++)
			{
				Sprite_Display(_data->engine.canvas, state->HUD.energyBody, (sfVector2f) { tempX[i] - 14 * scale + j * scale, tempY[i] + 5.0f * scale }, (sfVector2f) { scale, scale }, 0, NULL);
			}
			Sprite_Display(_data->engine.canvas, state->HUD.energyEnd, (sfVector2f) { tempX[i] - 14 * scale + j * scale, tempY[i] + 5.0f * scale }, (sfVector2f) { scale, scale }, 0, NULL);
		}
	}
	if (state->boss.isActive && state->boss.isAlive && !state->boss.isInIntroduction)
	{
		int index = 0;
		for (int i = 0; i < _data->engine.blueprints.boss.number; i++)
		{
			index = _data->engine.blueprints.boss.array[i]->name == state->boss.name ? i : index;
		}
		int toDraw = _data->engine.blueprints.boss.array[index]->phases[state->boss.currentPhase].health == 0 ? (int)(state->boss.phases[state->boss.currentPhase].health * 309 / state->boss.phases[state->boss.currentPhase + 1].health) : (int)(state->boss.phases[state->boss.currentPhase].health * 309 / (_data->engine.blueprints.boss.array[index]->phases[state->boss.currentPhase].health * _data->engine.currentDifficulty.enemiesHealthMultiplier));
		Sprite_Display(_data->engine.canvas, state->HUD.bossHUD, (sfVector2f) { (float)(tempResolution.x / 2), (float)(tempResolution.y / 11) }, (sfVector2f) { 4 * tempScale.x, 4 * tempScale.x }, 0, NULL);
		for (int i = 0; i < toDraw; i++)
		{
			Sprite_Display(_data->engine.canvas, state->HUD.bossArmor, (sfVector2f) { (float)(tempResolution.x / 2 - 616 * tempScale.x + i * 4 * tempScale.x), (float)(tempResolution.y / 11) }, (sfVector2f) { 4 * tempScale.x, 4 * tempScale.x }, 0, NULL);
		}
	}
}

void Game_DisplayMapTimer(MainData *_data)
{
	GameData *state = _data->stateData[GAME];// Create a shortcut to the state data
	sfVector2f tempScale = _data->engine.configuration->scale;
	char second[BUFFER_SMALL];
	sprintf(second, "map timer : %.1f sec", state->level.map.time);
	Text_Display(_data->engine.canvas, &state->parametersText, second, V2F(0.f, 60.0f * tempScale.y), V2F_NULL);
}

void Game_DisplayPlayerID(MainData *_data)
{
	GameData *state = _data->stateData[GAME];// Create a shortcut to the state data
	char* name[MAX_PLAYER_NUMBER] = { "j1", "j2", "j3", "j4" };
	sfColor color[MAX_PLAYER_NUMBER] = { sfBlue, sfRed, sfYellow, sfGreen };
	for (int i = 0; i < state->players.number; i++)
	{
		state->parametersText.color = color[i];
		Text_Display(_data->engine.canvas, &state->parametersText, name[i], state->players.array[i]->entity.position, (sfVector2f) { 0.5f, 0.5f });
	}
	state->parametersText.color = sfWhite;
}

void GameOutroDisplay(MainData* _data)
{
	GameData *state = _data->stateData[GAME];
	if (!state->boss.isAlive && _strcmpi("NULL", state->boss.name))
	{
		state->boss.entity.shake.isShaking = sfTrue;
		state->boss.entity.shake.shakingOffset.x = rand() % 20 - 10;
		state->boss.entity.shake.shakingOffset.y = rand() % 20 - 10;
		sfVector2f tempEntity = { state->boss.entity.position.x + state->boss.entity.shake.shakingOffset.x, state->boss.entity.position.y + state->boss.entity.shake.shakingOffset.y };
		Sprite_Display(_data->engine.canvas, state->boss.phases[state->boss.nbPhases - 1].animation->sprites[0], tempEntity, _data->engine.gameScale, state->boss.entity.angle, NULL);
	}
}

void Game_DisplayIndividualScore(MainData* _data)
{
	GameData *state = _data->stateData[GAME];
	sfVector2i tempResolution = _data->engine.configuration->resolution;
	char score[BUFFER_SMALL];
	for (int i = 0; i < state->players.number; i++)
	{
		sprintf(score, "j%d - %u", i + 1, state->players.array[i]->score);
		Text_Display(_data->engine.canvas, &state->parametersText, score, (sfVector2f) { state->scoreboardPos.x, state->scoreboardPos.y - tempResolution.y / 6 + i * tempResolution.y / 11 }, (sfVector2f) { 0.5f, 0.5f });
	}
	sprintf(score, "total - %u", state->squadScore);
	Text_Display(_data->engine.canvas, &state->parametersText, score, (sfVector2f) { state->scoreboardPos.x, state->scoreboardPos.y - tempResolution.y / 6 + 4 * tempResolution.y / 11 }, (sfVector2f) { 0.5f, 0.5f });
}

void Game_DisplayScoreboard(MainData* _data)
{
	GameData *state = _data->stateData[GAME];
	sfVector2f tempScale = _data->engine.configuration->scale;
	if (!state->boss.isAlive)
	{
		Sprite_Display(_data->engine.canvas, state->scoreboard, state->scoreboardPos, (sfVector2f) { 3 * tempScale.x, 3 * tempScale.y }, 0, NULL);
		Game_DisplayIndividualScore(_data);
	}
}