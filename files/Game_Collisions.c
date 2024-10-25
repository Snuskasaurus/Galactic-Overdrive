#include "Game.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

float BossCollisionWithProjectiles(MainData *_data, Collider *_collider);
sfBool EnemyCollisionWithScreen(MainData *_data, int iEnemy);
sfBool EnemyCollisionWithProjectiles(MainData *_data, int iEnemy, sfVector2f* _position);
sfBool EnemyCollisionWithPlayers(MainData *_data, int iEnemy);
void EnemyCollisionWithMoney(MainData* _data, int _iEnemy);
void HandlePlayerLaserCollisions(MainData* _data, float _dt, sfVector2f *_position);
void HandleEnemyLaserCollisions(MainData* _data, float _dt, sfVector2f *_position);
void BossColliderCheck(float *_tempEntityPosY, Collider *_collider);
void BossLaserCol(Collider *_bossCollider, Collider *_laserCollider, float _laserHeadSizeY, float _playerWeaponPosY, float *_entityPosY, float *_entitySizeY, sfBool *_bossTouched, float *_bossDmg, MainData *_data, int *_bossVulnerabilityIndex, int _i, float _laserDmg);

// -- MAIN FUNCTIONS ------------------------------------------------------- //

void Game_HandleBossCollisions(MainData *_data)
{
	GameData *state = _data->stateData[_data->currentState]; // Create a shortcut to the state data
	BossPhase *currentBossPhase = &state->boss.phases[state->boss.currentPhase];

	// Collision with the players
	if (_strcmpi("NULL", state->boss.name) != 0 && state->boss.isActive)
	{
		for (int i = 0; i < currentBossPhase->nbColliders; i++)
		{
			for (int iPlayer = state->players.number - 1; iPlayer >= 0; iPlayer--)
			{
				Collider *collider = &currentBossPhase->colliders[i];
				Player *player = state->players.array[iPlayer];
				if (player->isAlive)
				{
					if (Collider_CheckCollision(&player->spaceshipCollider, collider))
					{
						///printf("Boss collision with a player\n");
						Player_TakeDamage(_data, player, NULL);
					}
				}
			}
		}
		// Collision with the player projectiles
		switch (currentBossPhase->damagedFrom)
		{
		case BOSS_DAMAGED_FROM_WEAKPOINT:
		{
			for (int i = 0; i < currentBossPhase->nbWeakpoints; i++)
			{
				Collider *collider = &currentBossPhase->weakpoints[i].collider;
				float damage = 0.0f;
				damage = BossCollisionWithProjectiles(_data, collider);
				Boss_TakeDamageFromWeakpoint(&_data->engine, &state->boss, i, damage);
			}
			break;
		}
		case BOSS_DAMAGED_FROM_TURRET:
		{
			for (int i = 0; i < currentBossPhase->nbTurrets; i++)
			{
				Collider *collider = &currentBossPhase->turrets[i].collider;
				float damage = 0.0f;
				damage = BossCollisionWithProjectiles(_data, collider);
				Boss_TakeDamageFromTurret(&_data->engine, &state->boss, i, damage);
			}
			break;
		}
		}
	}
}

void Game_HandlePlayersCollisions(MainData *_data)
{
	GameData *state = _data->stateData[_data->currentState]; // Create a shortcut to the state data
	for (int iPlayer = state->players.number - 1; iPlayer >= 0; iPlayer--)
	{
		Player *player = state->players.array[iPlayer];
		if (player->isAlive)
		{
			// Collisions with enemy projectiles
			ListElement *current = state->projectiles[WEAPON_OWNER_ENEMY].first;
			Money* TempMoney = state->moneyBills;
			while (current)
			{
				if (!current->mustBeCleared)
				{
					ProjectileData *projectile = current->data;
					// Collision with DirectionalShield
					if (player->weapons[player->currentFireMode].weapon.behaviour == WEAPON_SHIELD &&
						player->weapons[player->currentFireMode].weapon.isActive == TRUE &&
						DirectionalShield_isColliding(player, projectile))
					{
						DirectionalShield_AbsorbBullet(projectile, &player->weapons[player->currentFireMode].weapon, &_data->engine);
						List_DestroyElementFromPointer(&state->projectiles[WEAPON_OWNER_ENEMY], current, FALSE);
					}
					else if (Collider_CheckCollision(&player->cockpitCollider, &projectile->collider))
					{
						///printf("Player collision with projectile\n");
						Player_TakeDamage(_data, player, &projectile->weapon);
						List_DestroyElementFromPointer(&state->projectiles[WEAPON_OWNER_ENEMY], current, FALSE);
						if (!player->isAlive)
						{
							Explosion_Create(&_data->engine, &state->explosions, player->entity.position, EXPLOSION_SIZE_DEFAULT, EXPLOSION_TYPE_DEFAULT);
						}
					}
				}
				// Asign the next element
				current = current->next;
			}
			while (TempMoney)
			{
				if (Collider_CheckCollision(&player->spaceshipCollider, &TempMoney->collider))
				{
					int tempAmount[3] = { 100, 500, 1000 };
					Money* TempErase = TempMoney;
					state->collectedBills += tempAmount[TempMoney->Amount];
					TempMoney = TempMoney->Next;
					Money_Destroy(&_data->engine, &state->moneyBills, TempErase, &state->explosions);
				}
				else
				{
					TempMoney = TempMoney->Next;
				}
			}
		}
	}
}

void Game_HandleEnemiesCollisions(MainData *_data, sfVector2f* _position)
{
	// Create a shortcut to the state data
	GameData *state = _data->stateData[_data->currentState];
	for (int iEnemy = state->enemies->number - 1; iEnemy >= 0; iEnemy--)
	{
		Enemy *enemy = state->enemies->array[iEnemy];
		if (EnemyCollisionWithScreen(_data, iEnemy));
		else if (EnemyCollisionWithProjectiles(_data, iEnemy, _position));
		else if (EnemyCollisionWithPlayers(_data, iEnemy));
		EnemyCollisionWithMoney(_data, iEnemy);
	}
}

void Game_HandleProjectilesCollisions(MainData *_data)
{
	// Create a shortcut to the state data
	GameData *state = _data->stateData[_data->currentState];
	for (int projectileType = 0; projectileType < WEAPON_OWNER_SIZE; projectileType++)
	{
		ListElement *current = state->projectiles[projectileType].first;
		while (current)
		{
			if (!current->mustBeCleared)
			{
				ProjectileData *projectile = current->data;
				if (!Collider_CheckCollision(&projectile->collider, &state->screenCollider))
				{
					///printf("projectile collision with screen\n");
					List_DestroyElementFromPointer(&state->projectiles[projectileType], current, FALSE);
				}
			}
			// Asign the next element
			current = current->next;
		}
	}
}

void Game_HandleLasersCollisions(MainData *_data, float _dt, sfVector2f* _position)
{
	GameData *state = _data->stateData[_data->currentState];

	if (state->lasers != NULL)
	{
		HandlePlayerLaserCollisions(_data, _dt, _position);
		HandleEnemyLaserCollisions(_data, _dt, _position);
	}
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //

float BossCollisionWithProjectiles(MainData *_data, Collider *_collider)
{
	GameData *state = _data->stateData[_data->currentState]; // Create a shortcut to the state data
	ListElement *currentProjectile = state->projectiles[WEAPON_OWNER_PLAYER].first;
	float damage = 0.0f;
	while (currentProjectile)
	{
		if (!currentProjectile->mustBeCleared)
		{
			ProjectileData *projectile = currentProjectile->data;
			if (Collider_CheckCollision(_collider, &projectile->collider))
			{
				///printf("Boss collision with a projectile\n");
				damage += Boss_CalculateDamage(&_data->engine, &projectile->weapon);
				// Create the impact explosion of the projectile
				Explosion_Create(&_data->engine, &state->explosions, projectile->entity.position, EXPLOSION_SIZE_IMPACT, projectile->weapon.type);
				List_DestroyElementFromPointer(&state->projectiles[WEAPON_OWNER_PLAYER], currentProjectile, FALSE);
			}
		}
		// Asign the next element
		currentProjectile = currentProjectile->next;
	}
	return damage;
}

sfBool EnemyCollisionWithScreen(MainData *_data, int iEnemy)
{
	GameData *state = _data->stateData[_data->currentState];
	Engine *engine = &_data->engine;
	Enemy *enemy = state->enemies->array[iEnemy];
	if (enemy->entity.position.y >= _data->engine.configuration->resolution.y + GAME_MAXIMUM_DISTANCE_FROM_BORDERS && enemy->entity.velocity.y > 0.0f)
	{
		Enemy_Destroy(state->enemies, iEnemy, state->lasers);
		///printf("Enemy collision with screen\n");
		return sfTrue;
	}
	else if (enemy->entity.position.y <= -GAME_MAXIMUM_DISTANCE_FROM_BORDERS && enemy->entity.velocity.y < 0.0f)
	{
		Enemy_Destroy(state->enemies, iEnemy, state->lasers);
		///printf("Enemy collision with screen\n");
		return sfTrue;
	}
	else if (enemy->entity.position.x >= _data->engine.configuration->resolution.x + GAME_MAXIMUM_DISTANCE_FROM_BORDERS && enemy->entity.velocity.x > 0.0f)
	{
		Enemy_Destroy(state->enemies, iEnemy, state->lasers);
		///printf("Enemy collision with screen\n");
		return sfTrue;
	}
	else if (enemy->entity.position.x <= -GAME_MAXIMUM_DISTANCE_FROM_BORDERS && enemy->entity.velocity.x < 0.0f)
	{
		Enemy_Destroy(state->enemies, iEnemy, state->lasers);
		///printf("Enemy collision with screen\n");
		return sfTrue;
	}
	else
	{
		return sfFalse;
	}
}

sfBool EnemyCollisionWithProjectiles(MainData *_data, int iEnemy, sfVector2f* _position)
{
	GameData *state = _data->stateData[_data->currentState];
	Enemy *enemy = state->enemies->array[iEnemy];
	ListElement *current = state->projectiles[WEAPON_OWNER_PLAYER].first;
	while (current)
	{
		if (!current->mustBeCleared)
		{
			ProjectileData *projectile = current->data;
			if (Collider_CheckCollision(&enemy->largeCollider, &projectile->collider))
			{
				float currentLife = state->enemies->array[iEnemy]->health;
				///printf("Enemy collision with projectile\n");
				Enemy_TakeDamage(&_data->engine, _data, &projectile->weapon, iEnemy, _position);
				// Create the impact explosion of the projectile
				Explosion_Create(&_data->engine, &state->explosions, projectile->entity.position, EXPLOSION_SIZE_IMPACT, projectile->weapon.type);
				List_DestroyElementFromPointer(&state->projectiles[WEAPON_OWNER_PLAYER], current, FALSE);
				return sfTrue;
			}
		}
		// Asign the next element
		current = current->next;
	}
	return sfFalse;
}


sfBool EnemyCollisionWithPlayers(MainData *_data, int iEnemy)
{
	GameData *state = _data->stateData[_data->currentState];
	Enemy *enemy = state->enemies->array[iEnemy];
	for (int iPlayer = state->players.number - 1; iPlayer >= 0; iPlayer--)
	{
		Player *player = state->players.array[iPlayer];
		if (player->isAlive)
		{
			if (Collider_CheckCollision(&enemy->smallCollider, &player->cockpitCollider))
			{
				///printf("Enemy collision with player\n");
				Player_TakeDamage(_data, player, NULL);
				Enemy_TakeDamage(&_data->engine, _data, NULL, iEnemy, NULL);
				return sfTrue;
			}
		}
	}
	return sfFalse;
}

void EnemyCollisionWithMoney(MainData* _data, int _iEnemy)
{
	GameData *state = _data->stateData[_data->currentState];
	Money* TempMoney = state->moneyBills;
	Enemy *enemy = state->enemies->array[_iEnemy];
	if (enemy->behaviour == ENEMY_TYPE_THIEF)
	{
		while (TempMoney)
		{
			if (Collider_CheckCollision(&enemy->smallCollider, &TempMoney->collider))
			{
				Money* TempErase = TempMoney;
				TempMoney = TempMoney->Next;
				Money_Destroy(&_data->engine, &state->moneyBills, TempErase, &state->explosions);
			}
			else
			{
				TempMoney = TempMoney->Next;
			}
		}
	}
}

void HandlePlayerLaserCollisions(MainData *_data, float _dt, sfVector2f *_position)
{
	GameData *state = _data->stateData[_data->currentState];
	Engine *engine = &_data->engine;
	for (Laser *tempLaser = state->lasers->firstElement[WEAPON_OWNER_PLAYER]; tempLaser != NULL; tempLaser = tempLaser->nextPtr)
	{
		int enemyTouchedIndex = -1;
		int playerTouchedIndex = 0;
		RectangleCollider *colliderLaser = (RectangleCollider*)tempLaser->collider.pointer;
		Player *player = (Player*)tempLaser->entityPtr;
		float playerWeaponPosY = player->weapons[player->currentFireMode].weapon.position.y;
		float entitySizeY = playerWeaponPosY - colliderLaser->position.y;
		sfBool bossTouched = sfFalse;
		sfVector2f headSpriteSize = Sprite_GetSize(tempLaser->origin->animProj->sprites[0]);
		float entityPosY = 0.f;
		float bossDmg = 0.f;
		Boss_DamagedFrom bossDmgFrom = 0;
		int bossVulnerabilityIndex = -1;

		headSpriteSize = V2F(headSpriteSize.x * engine->gameScale.x, headSpriteSize.y * engine->gameScale.y);
		entityPosY = headSpriteSize.y / 2.f;

		if (state->players.number > 1 && (tempLaser->origin->type == WEAPON_HEAL || tempLaser->origin->type == WEAPON_DMG_BOOST))
		{
			// Check collision player laser to other players
			for (int iPlayer = state->players.number - 1; iPlayer >= 0; iPlayer--)
			{
				Player *player2 = state->players.array[iPlayer];
				if (player != player2)
				{
					if (Collider_CheckCollision(&player2->spaceshipCollider, &tempLaser->collider))
					{
						float tempEntityPosY = 0.f;
						float tempEntitySizeY = 0.f;

						switch (player2->spaceshipCollider.type)
						{
						case COLLIDER_CIRCLE:
						{
							CircleCollider *player2CicleCol = (CircleCollider*)player2->spaceshipCollider.pointer;
							tempEntityPosY = player2->entity.position.y + player2CicleCol->radius;
							break;
						}
						case COLLIDER_RECTANGLE:
						{
							RectangleCollider *player2RecCol = (RectangleCollider*)player2->spaceshipCollider.pointer;
							tempEntityPosY = player2->entity.position.y + (player2RecCol->size.y / 2.f);
							break;
						}
						case COLLIDER_SHAPE:
						{
							// TODO
							//tempEntityPosY = player2->entity.position.y + ??;
							break;
						}
						default:
						{
							tempEntityPosY = player2->entity.position.y;
							break;
						}
						}

						tempEntitySizeY = playerWeaponPosY - tempEntityPosY + (headSpriteSize.y / 2.f);
						if (tempEntityPosY > entityPosY)
						{
							entityPosY = tempEntityPosY;
							entitySizeY = tempEntitySizeY;
							playerTouchedIndex = iPlayer;
						}
					}
				}
			}
		}
		else if (tempLaser->origin->type != WEAPON_HEAL && tempLaser->origin->type != WEAPON_DMG_BOOST)
		{
			// Check laser collision with the boss
			Boss *boss = &state->boss;
			if (boss != NULL && state->boss.isAlive && state->boss.isActive)
			{
				BossPhase *currentBossPhase = &boss->phases[boss->currentPhase];
				bossDmgFrom = currentBossPhase->damagedFrom;
				switch (currentBossPhase->damagedFrom)
				{
					// Check collision with the turrets
				case BOSS_DAMAGED_FROM_TURRET:
				{
					for (int i = 0; i < currentBossPhase->nbTurrets; i++)
					{
						if (currentBossPhase->turrets[i].health > 0.f)
						{
							Collider *collider = &currentBossPhase->turrets[i].collider;
							BossLaserCol(collider, &tempLaser->collider, headSpriteSize.y, playerWeaponPosY, &entityPosY, &entitySizeY, &bossTouched, &bossDmg, _data, &bossVulnerabilityIndex, i, tempLaser->origin->damage);
						}
					}
					break;
				}
				// Check collision with the weakpoints
				case BOSS_DAMAGED_FROM_WEAKPOINT:
				{
					for (int i = 0; i < currentBossPhase->nbWeakpoints; i++)
					{
						Collider *collider = &currentBossPhase->weakpoints[i].collider;
						BossLaserCol(collider, &tempLaser->collider, headSpriteSize.y, playerWeaponPosY, &entityPosY, &entitySizeY, &bossTouched, &bossDmg, _data, &bossVulnerabilityIndex, i, tempLaser->origin->damage);
					}
					break;
				}
				}
				// Check collision with all the boss
				/*for (int i = 0; i < currentBossPhase->nbColliders; i++)
				{
					Collider *bossCollider = &boss->phases[state->boss.currentPhase].colliders[i];
					if (Collider_CheckCollision(bossCollider, &tempLaser->collider))
					{
						RectangleCollider *colliderBoss = (RectangleCollider*)bossCollider->pointer;
						float tempEntityPosY = boss->entity.position.y + (colliderBoss->size.y / 2.f);
						float tempEntitySizeY = playerWeaponPosY - tempEntityPosY + (headSpriteSize.y / 2.f);
						if (tempEntityPosY > entityPosY)
						{
							entityPosY = tempEntityPosY;
							entitySizeY = tempEntitySizeY;
							bossTouched = sfTrue;
						}
					}
				}*/
			}
			// Check laser collision with enemies
			for (int iEnemy = state->enemies->number - 1; iEnemy >= 0; iEnemy--)
			{
				Enemy *enemy = state->enemies->array[iEnemy];

				if (Collider_CheckCollision(&enemy->largeCollider, &tempLaser->collider))
				{
					float tempEntityPosY = 0.f;
					float tempEntitySizeY = 0.f;

					switch (enemy->largeCollider.type)
					{
					case COLLIDER_CIRCLE:
					{
						CircleCollider *enemyCicleCol = (CircleCollider*)enemy->largeCollider.pointer;
						tempEntityPosY = enemy->entity.position.y + enemyCicleCol->radius;
						break;
					}
					case COLLIDER_RECTANGLE:
					{
						RectangleCollider *enemyRecCol = (RectangleCollider*)enemy->largeCollider.pointer;
						tempEntityPosY = enemy->entity.position.y + (enemyRecCol->size.y / 2.f);
						break;
					}
					case COLLIDER_SHAPE:
					{
						// TODO
						//tempEntityPosY = enemy->entity.position.y + ??;
						break;
					}
					default:
					{
						tempEntityPosY = enemy->entity.position.y;
						break;
					}
					}

					tempEntitySizeY = playerWeaponPosY - tempEntityPosY + (headSpriteSize.y / 2.f);
					if (tempEntityPosY > entityPosY)
					{
						//printf("enemy index : %i, posY : %f\n", iEnemy, tempEntityPosY);
						entityPosY = tempEntityPosY;
						entitySizeY = tempEntitySizeY;
						enemyTouchedIndex = iEnemy;
						bossTouched = sfFalse;
					}
				}
			}
		}
		//printf("2 : %f\n", entityPosY);
		tempLaser->laserEntityHead.position.y = entityPosY;
		colliderLaser->size.y = entitySizeY;
		if (entityPosY != (headSpriteSize.y / 2.f))
		{
			if (tempLaser->origin->type == WEAPON_HEAL)
			{
				Player_Heal(engine, state->players.array[playerTouchedIndex], tempLaser->origin->damage * _dt);
			}
			else if ((tempLaser->origin->type == WEAPON_DMG_BOOST) && (state->players.array[playerTouchedIndex]->LaserBoosted.isPlaying == sfFalse))
			{
				state->players.array[playerTouchedIndex]->laserBoostReceived = tempLaser->origin->baseDamage;
				state->players.array[playerTouchedIndex]->fireRateMultiplier -= tempLaser->origin->baseDamage;
				state->players.array[playerTouchedIndex]->LaserBoosted.isPlaying = sfTrue;
			}
			else if (bossTouched == sfTrue)
			{
				switch (bossDmgFrom)
				{
				case BOSS_DAMAGED_FROM_TURRET:
				{
					Boss_TakeDamageFromTurret(&_data->engine, &state->boss, bossVulnerabilityIndex, bossDmg);
					break;
				}
				case BOSS_DAMAGED_FROM_WEAKPOINT:
				{
					Boss_TakeDamageFromWeakpoint(&_data->engine, &state->boss, bossVulnerabilityIndex, bossDmg);
					break;
				}
				default:
					break;
				}
			}
			else if (enemyTouchedIndex > -1)
			{
				Enemy_TakeDamage(engine, _data, tempLaser->origin, enemyTouchedIndex, NULL);
			}
		}
	}
}

void HandleEnemyLaserCollisions(MainData* _data, float _dt, sfVector2f *_position)
{
	GameData *state = _data->stateData[_data->currentState];
	Engine *engine = &_data->engine;
	for (WeaponOwner iOwner = WEAPON_OWNER_ENEMY; iOwner < WEAPON_OWNER_SIZE; iOwner++)
	{
		for (Laser *tempLaser = state->lasers->firstElement[iOwner]; tempLaser != NULL; tempLaser = tempLaser->nextPtr)
		{
			int playerTouchedIndex = 0;
			RectangleCollider *colliderLaser = (RectangleCollider*)tempLaser->collider.pointer;
			Enemy *enemy = NULL;
			Boss *boss = NULL;
			float entityWeaponPosY = 0;
			float entitySizeY = 0;
			sfVector2f headSpriteSize = Sprite_GetSize(tempLaser->origin->animProj->sprites[0]);
			float entityPosY = 0.f;
			headSpriteSize = V2F(headSpriteSize.x * engine->gameScale.x, headSpriteSize.y * engine->gameScale.y);
			entityPosY = engine->configuration->resolution.y - (headSpriteSize.y / 2.f);

			switch (iOwner)
			{
			case WEAPON_OWNER_ENEMY:
			{
				enemy = (Enemy*)tempLaser->entityPtr;
				entityWeaponPosY = enemy->entity.position.y + 0;  // 0 = weapon Offset Y, not yet implemeted
				break;
			}
			case WEAPON_OWNER_BOSS:
			{
				boss = (Boss*)tempLaser->entityPtr;
				entityWeaponPosY = tempLaser->origin->position.y;
				break;
			}
			default:
				break;
			}

			entitySizeY = entityPosY - entityWeaponPosY;

			for (int iPlayer = state->players.number - 1; iPlayer >= 0; iPlayer--)
			{
				Player *player = state->players.array[iPlayer];
				if (Collider_CheckCollision(&player->cockpitCollider, &tempLaser->collider) && player->isAlive)
				{
					float tempEntityPosY = 0.f;
					float tempEntitySizeY = 0.f;

					switch (player->spaceshipCollider.type)
					{
					case COLLIDER_CIRCLE:
					{
						CircleCollider *playerCicleCol = (CircleCollider*)player->spaceshipCollider.pointer;
						tempEntityPosY = player->entity.position.y + playerCicleCol->radius;
						break;
					}
					case COLLIDER_RECTANGLE:
					{
						RectangleCollider *playerRecCol = (RectangleCollider*)player->spaceshipCollider.pointer;
						tempEntityPosY = player->entity.position.y + (playerRecCol->size.y / 2.f);
						break;
					}
					case COLLIDER_SHAPE:
					{
						// TODO
						//tempEntityPosY = player->entity.position.y + ??;
						break;
					}
					default:
					{
						tempEntityPosY = player->entity.position.y;
						break;
					}
					}

					tempEntitySizeY = tempEntityPosY - entityWeaponPosY;
					if (tempEntityPosY < entityPosY)
					{
						entityPosY = tempEntityPosY;
						entitySizeY = tempEntitySizeY;
						playerTouchedIndex = iPlayer;
					}
				}
			}

			tempLaser->laserEntityHead.position.y = entityPosY;
			colliderLaser->size.y = entitySizeY;
			if (entityPosY != engine->configuration->resolution.y - (headSpriteSize.y / 2.f))
			{
				Player_TakeDamage(_data, state->players.array[playerTouchedIndex], tempLaser->origin);
			}
		}
	}
}

void BossColliderCheck(float *_tempEntityPosY, Collider *_collider)
{
	switch (_collider->type)
	{
	case COLLIDER_CIRCLE:
	{
		CircleCollider *bossCicleCol = (CircleCollider*)_collider->pointer;
		*_tempEntityPosY = bossCicleCol->position.y + bossCicleCol->radius;
		break;
	}
	case COLLIDER_RECTANGLE:
	{
		RectangleCollider *bossRecCol = (RectangleCollider*)_collider->pointer;
		*_tempEntityPosY = bossRecCol->position.y + (bossRecCol->size.y / 2.f);
		break;
	}
	case COLLIDER_SHAPE:
	{
		// TODO
		//*_tempEntityPosY = enemy->entity.position.y + ??;
		break;
	}
	default:
	{
		break;
	}
	}
}

void BossLaserCol(Collider *_bossCollider, Collider *_laserCollider, float _laserHeadSizeY, float _playerWeaponPosY, float *_entityPosY, float *_entitySizeY, sfBool *_bossTouched, float *_bossDmg, MainData *_data, int *_bossVulnerabilityIndex, int _i, float _laserDmg)
{
	if (Collider_CheckCollision(_bossCollider, _laserCollider))
	{
		float tempEntityPosY = 0.f;
		float tempEntitySizeY = 0.f;

		BossColliderCheck(&tempEntityPosY, _bossCollider);

		tempEntitySizeY = _playerWeaponPosY - tempEntityPosY + (_laserHeadSizeY / 2.f);
		if (tempEntityPosY > *_entityPosY)
		{
			*_entityPosY = tempEntityPosY;
			*_entitySizeY = tempEntitySizeY;
			*_bossTouched = sfTrue;
			*_bossDmg = _laserDmg * _data->engine.dt;
			*_bossVulnerabilityIndex = _i;
		}
	}
}