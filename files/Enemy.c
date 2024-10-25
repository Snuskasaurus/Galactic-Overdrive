#include "Enemy.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

int Get_NumberOfType(Enemies *_enemies, EnemyType _type);

void CreateDefaultEnemy(Engine *_engine, Enemy *_enemy, Waypoint *_spawn);
void CreateFollowerEnemy(Engine *_engine, Enemy *_enemy, Waypoint *_spawn, Player *_player);
void CreateArtilleryEnemy(Engine *_engine, Enemy *_enemy, Waypoint *_spawn, Player *_player);
void CreateThiefEnemy(Engine* _engine, Enemy* _enemy);

Enemy *GetEnemyBlueprintFromName(BP_Enemies *_EnemyBlueprints, const char *_enemyName);

void UpdateDefaultEnemy(Engine *_engine, Enemy *_enemy, float _dt, List *_projectiles);
void UpdateFollowerEnemy(Engine *_engine, Enemy *_enemy, float _dt, List *_projectiles);
void UpdateArtilleryEnemy(Engine *_engine, Enemy *_enemy, float _dt, List *_projectiles, Lasers *_lasers);
void UpdateThiefEnemy(Engine* _engine, Enemy* _enemy, float _dt, Money** _List);

float Enemy_CaculateDamageFromWeapon(Engine *_engine, Enemy *enemy, Weapon *_weaponUsed);

void AdjustDifficultyToEnemy(Enemy *_enemy, Difficulty *_currentDif);

// -- SHARED FUNCTIONS ----------------------------------------------------- //

Enemies *Enemy_Initialize()
{
	return calloc(1, sizeof(Enemies));
}

//Must init weapon blueprint before this ...
BP_Enemies EnemyLoader(Engine *_engine)
{
	BP_Weapons *weaponsBP = &_engine->blueprints.weapons;
	ExploDir directory = ExploreDirectory(ENEMY_SHIPS_DIR);
	BP_Enemies blueprints;
	blueprints.array = calloc(directory.nbrFiles, sizeof(Enemy*));
	blueprints.number = directory.nbrFiles;

	printf("Initialization of enemies bluprints : \n");
	for (int i = 0; i < blueprints.number; i++)
	{
		blueprints.array[i] = calloc(1, sizeof(Enemy));
		if (blueprints.array[i] != NULL)
		{
			FILE *shipFile = NULL;
			char filepath[BUFFER_LARGE] = ENEMY_SHIPS_DIR;
			strcat(filepath, "/");
			strcat(filepath, directory.fileNames[i]);
			shipFile = fopen(filepath, "rt");

			if (shipFile != NULL)
			{
				Enemy *enemy = blueprints.array[i];
				char spriteName[BUFFER_SMALL] = { 0 };
				char weaponName[BUFFER_SMALL] = { 0 };
				float timeInvulnerability = 0.f;

				//Default values
				enemy->health = 50.f;
				enemy->behaviour = ENEMY_TYPE_DEFAULT;
				enemy->type = TYPE_DEFAULT;
				enemy->invulnerability = Timer_Create(1.f);
				strcpy(enemy->name, "NONE");

				//Get the info for the blueprints
				while (!feof(shipFile))
				{
					char keyWord[BUFFER_SMALL] = { 0 };
					sfBool didntFoundKeyWord = sfTrue;
					fscanf(shipFile, "%s", keyWord);

					if (strcmp(keyWord, "NAME") == 0)
					{
						fscanf(shipFile, "%s\n", enemy->name);
						didntFoundKeyWord = sfFalse;
					}

					if (strcmp(keyWord, "HEALTH") == 0)
					{
						fscanf(shipFile, "%f\n", &enemy->health);
						didntFoundKeyWord = sfFalse;
					}

					if (strcmp(keyWord, "SPRITE_NAME") == 0)
					{
						fscanf(shipFile, "%s\n", spriteName);
						enemy->sprite = Sprite_CreateFromFile(spriteName, 0.5f, 0.5f);
						didntFoundKeyWord = sfFalse;
					}

					if (strcmp(keyWord, "WEAPON_NAME") == 0)
					{
						fscanf(shipFile, "%s\n", weaponName);
						enemy->haveWeapon = TRUE;
						enemy->weapon = GetWeapon(weaponName, weaponsBP, WEAPON_OWNER_ENEMY, NULL);
						didntFoundKeyWord = sfFalse;
					}

					if (strcmp(keyWord, "BEHAVIOUR") == 0)
					{
						fscanf(shipFile, "%i\n", &enemy->behaviour);
						didntFoundKeyWord = sfFalse;
					}

					if (strcmp(keyWord, "TYPE") == 0)
					{
						fscanf(shipFile, "%i\n", &enemy->type);
						didntFoundKeyWord = sfFalse;
					}

					if (strcmp(keyWord, "COLLIDER_SMALL") == 0)
					{
						fscanf(shipFile, " TYPE %d", &enemy->smallCollider.type);
						Collider_CreateFromFile(&enemy->smallCollider, shipFile);
						didntFoundKeyWord = sfFalse;
					}

					if (strcmp(keyWord, "COLLIDER_LARGE") == 0)
					{
						fscanf(shipFile, " TYPE %d", &enemy->largeCollider.type);
						Collider_CreateFromFile(&enemy->largeCollider, shipFile);
						didntFoundKeyWord = sfFalse;
					}

					if (strcmp(keyWord, "TIME_INVULNERABILITY") == 0)
					{
						fscanf(shipFile, "%f\n", &enemy->invulnerability.maxTime);
						didntFoundKeyWord = sfFalse;
					}

					if (strcmp(keyWord, "EXPLOSION_SIZE") == 0)
					{
						fscanf(shipFile, "%d\n", &enemy->explosionsSize);
						didntFoundKeyWord = sfFalse;
					}

					if (strcmp(keyWord, "SLOW_DOWN_WHILE_SHOOTING") == 0)
					{
						int value;
						fscanf(shipFile, "%d\n", &value);
						enemy->canSlowDown = value;
						didntFoundKeyWord = sfFalse;
					}

					if (strcmp(keyWord, "TIME_BEFORE_SHOOTING") == 0)
					{
						float time;
						fscanf(shipFile, "%f\n", &time);
						enemy->timerBeforeShooting = Timer_Create(time);
						didntFoundKeyWord = sfFalse;
					}

					if (strcmp(keyWord, "TIME_WHILE_SHOOTING") == 0)
					{
						float time;
						fscanf(shipFile, "%f\n", &time);
						enemy->timerWhileShooting = Timer_Create(time);
						didntFoundKeyWord = sfFalse;
					}

					if (strcmp(keyWord, "SPEED") == 0)
					{
						fscanf(shipFile, "%f\n", &enemy->speed);
						didntFoundKeyWord = sfFalse;
					}

					if (strcmp(keyWord, "SCORE_POOL") == 0)
					{
						fscanf(shipFile, "%d\n", &enemy->totalScorePool);
						didntFoundKeyWord = sfFalse;
					}

					if (strcmp(keyWord, "TURRET") == 0)
					{
						char name[BUFFER_SMALL];
						sfVector2f position;
						fscanf(shipFile, "%s POSITION %f %f\n", &name, &position.x, &position.y);
						enemy->turret = Turret_Create(_engine, name, WEAPON_OWNER_ENEMY, NULL, position);
						enemy->haveTurretLive = TRUE;
						didntFoundKeyWord = sfFalse;
					}

					if (strcmp(keyWord, "TURRET_POSITION") == 0)
					{
						fscanf(shipFile, "%f %f\n", &enemy->turretOffset.x, &enemy->turretOffset.y);
						didntFoundKeyWord = sfFalse;
					}

					if (didntFoundKeyWord == sfTrue)
					{
						fscanf(shipFile, "%[^\n]\n", keyWord);
					}
				}
				printf("(%d/%d)\t%s\n", i + 1, directory.nbrFiles, enemy->name);
				fclose(shipFile);
			}
		}
	}
	printf("\n");
	return blueprints;
}

Enemy *GetCopyOfEnemyBlueprint(Blueprint *_blueprints, const char *_enemyName)
{
	Enemy *enemyBlueprint = GetEnemyBlueprintFromName(&_blueprints->enemies, _enemyName);
	Enemy *enemy = calloc(1, sizeof(Enemy));
	if (enemy && enemyBlueprint)
	{
		enemy->health = enemyBlueprint->health;
		enemy->type = enemyBlueprint->type;
		enemy->invulnerability = enemyBlueprint->invulnerability;
		enemy->isAlive = sfTrue;
		enemy->sprite = enemyBlueprint->sprite;
		enemy->behaviour = enemyBlueprint->behaviour;
		// Colliders
		enemy->largeCollider = Collider_Copy(&enemyBlueprint->largeCollider);
		enemy->smallCollider = Collider_Copy(&enemyBlueprint->smallCollider);
		// Weapons
		enemy->weapon = enemyBlueprint->weapon;
		enemy->haveWeapon = enemyBlueprint->haveWeapon;
		// Turrets
		enemy->haveTurretLive = enemyBlueprint->haveTurretLive;
		enemy->turretOffset = enemyBlueprint->turretOffset;
		enemy->turret = enemyBlueprint->turret;
		// Timers
		enemy->timerBeforeShooting = enemyBlueprint->timerBeforeShooting;
		enemy->timerWhileShooting = enemyBlueprint->timerWhileShooting;
		// Others
		enemy->speed = enemyBlueprint->speed;
		enemy->totalScorePool = enemyBlueprint->totalScorePool;
		enemy->activeScorePool = enemy->totalScorePool;
		enemy->timerToDisplayWhite = Timer_Create(FEEDBACK_TIME_WHITE);
		enemy->timerWaitingTime = Timer_Create(FEEDBACK_TIME_WAIT);
		enemy->timerToDisplayColor = Timer_Create(FEEDBACK_TIME_COLORED);
		enemy->canSlowDown = enemyBlueprint->canSlowDown;
		enemy->explosionsSize = enemyBlueprint->explosionsSize;
	}
	else
	{
		printf("Failed to create enemy copy from blueprint...\n");
	}
	return enemy;
}

Enemy *Enemy_Create(Engine *_engine, Enemies *_enemies, Waypoint *_spawn, Player *_target, Difficulty *_currentDif)
{
	Enemy *enemy = NULL;
	Waypoint_Spawn *spawnData = NULL;
	if (_enemies->number < MAX_ENEMY_NUMBER)
	{
		enemy = calloc(1, sizeof(Enemy));
		if (enemy)
		{
			spawnData = _spawn->content;
			enemy = GetCopyOfEnemyBlueprint(&_engine->blueprints, spawnData->enemyName);
			enemy->entity.position = _spawn->position;
			enemy->invulnerability = Timer_Create(ENEMY_INVULNERABILITY_TIME);
			enemy->isAlive = sfTrue;
			// Initialization of specific variables
			switch (enemy->behaviour)
			{
			default:
				CreateDefaultEnemy(_engine, enemy, _spawn);
				break;
			case ENEMY_TYPE_ARTILLERY:
				CreateArtilleryEnemy(_engine, enemy, _spawn, _target);
				break;
			case ENEMY_TYPE_FOLLOWER:
				CreateFollowerEnemy(_engine, enemy, _spawn, _target);
				break;
			case ENEMY_TYPE_THIEF:
				CreateThiefEnemy(_engine, enemy);
			}

			AdjustDifficultyToEnemy(enemy, _currentDif);

			// Add the enemy to the main enemies structure
			_enemies->array[_enemies->number] = enemy;
			_enemies->number++;
			enemy->isSlowDown = TRUE;
		}
	}
	return enemy;
}

void Enemy_TakeDamage(Engine *_engine, MainData *_data, Weapon *_weaponUsed, int _index, sfVector2f* _position)
{
	GameData* state = _data->stateData[GAME];
	Enemy *enemy = state->enemies->array[_index];
	if (_weaponUsed)
	{
		Player* tempPlayer = _weaponUsed->player;
		unsigned long tempScore = state->squadScore;
		float damage = Enemy_CaculateDamageFromWeapon(_engine, enemy, _weaponUsed);
		int index = -1;
		for (int i = 0; i < state->players.number; i++)
		{
			if (state->players.array[i]->joystick == tempPlayer->joystick)
			{
				index = i;
			}
		}
		Score_GainByWeaponDamage(&state->squadScore, enemy, damage, &state->players, &_engine->currentDifficulty);
		// Remove the health to the turret
		if (enemy->haveTurretLive)
		{
			enemy->turret.health -= damage;
		}
		// Else, remove the health to the enemy
		else
		{
			enemy->health -= damage;
		}
		enemy->isHit = TRUE;
		Timer_Reset(&enemy->timerToDisplayWhite);
		Timer_Reset(&enemy->timerWaitingTime);
		Timer_Reset(&enemy->timerToDisplayColor);
		Entity_CreateShaking(&enemy->entity.shake, SHAKE_POWER, SHAKE_DURATION);
		if (enemy->health <= 0.0f)
		{
			Score_GainByDeath(_data, &state->squadScore, enemy, &state->players, &state->explosions, &_engine->currentDifficulty);
			if (_position)
			{
				*_position = enemy->entity.position;
			}
			enemy->health = 0.0f;
			enemy->isAlive = sfFalse;
		}
		state->players.array[index]->score += state->squadScore - tempScore;
	}
	else
	{
		enemy->health = 0.0f;
		enemy->isAlive = sfFalse;
	}
}

void Enemy_Update(Engine *_engine, Enemies *_enemies, float _dt, List *_projectiles, List* _explosions, Lasers *_lasers, Money** _money)
{
	for (int i = _enemies->number - 1; i >= 0; i--)
	{
		Enemy *enemy = _enemies->array[i];

		if (enemy->isAlive)
		{
			// Update specific mechanics
			switch (enemy->behaviour)
			{
			default:
				UpdateDefaultEnemy(_engine, enemy, _dt, _projectiles);
				break;
			case ENEMY_TYPE_FOLLOWER:
				UpdateFollowerEnemy(_engine, enemy, _dt, _projectiles);
				break;
			case ENEMY_TYPE_ARTILLERY:
				UpdateArtilleryEnemy(_engine, enemy, _dt, _projectiles, _lasers);
				break;
			case ENEMY_TYPE_THIEF:
				UpdateThiefEnemy(_engine, enemy, _dt, _money);
				break;
			}
			// Update common mechanics
			Timer_Update(&enemy->invulnerability, _dt);
			if (!enemy->isSlowDown && enemy->canSlowDown)
			{
				enemy->entity.velocity = V2F_MUL_VALUE(enemy->entity.velocity, 0.3f);
			}
			Entity_UpdatePosition(&enemy->entity, _dt);
			Weapon_UpdatePosition(&enemy->weapon, enemy->entity.position, V2F(0.f, 0.f));
			Entity_DoShake(&enemy->entity.shake, _dt);
			// Update colliders positions
			Collider_UpdatePosition(&enemy->largeCollider, enemy->entity.position);
			Collider_UpdatePosition(&enemy->smallCollider, enemy->entity.position);
			// Update the turret
			if (enemy->haveTurretLive)
			{
				sfVector2f turretPosition = enemy->entity.position;
				sfVector2f targetPosition;
				// Seet the turret position
				turretPosition.x += enemy->turretOffset.x;
				turretPosition.y += enemy->turretOffset.y;
				// Set the target position
				if (enemy->target)
				{
					targetPosition = enemy->target->entity.position;
				}
				else
				{
					targetPosition = turretPosition;
					turretPosition.y -= 100.0f;
				}
				Turret_Update(_engine, &enemy->turret, _projectiles, targetPosition, turretPosition);
				// Destroy the turret and create an explosion
				if (enemy->turret.health <= 0.0f)
				{
					Explosion_Create(_engine, _explosions, turretPosition, enemy->explosionsSize, enemy->explosionsType);
					enemy->haveTurretLive = FALSE;
				}
			}
			// Update the weapon and shoot
			else if (enemy->haveWeapon)
			{
				Timer_Update(&enemy->timerBeforeShooting, _engine->dt);
				if (Timer_Check(&enemy->timerBeforeShooting))
				{
					Timer_Update(&enemy->timerWhileShooting, _engine->dt);
					if (enemy->timerWhileShooting.currentTime <= enemy->timerWhileShooting.maxTime)
					{
						enemy->isSlowDown = FALSE;
						Weapon_Update(_engine, &enemy->weapon, _projectiles, &enemy->entity, &enemy->target->entity);
					}
					else
					{
						enemy->isSlowDown = TRUE;
						Timer_Reset(&enemy->timerBeforeShooting);
						Timer_Reset(&enemy->timerWhileShooting);
					}
				}
			}
			// Update the feedbacks
			if (enemy->isHit)
			{
				Timer_Update(&enemy->timerToDisplayWhite, _dt);
				if (Timer_Check(&enemy->timerToDisplayWhite))
				{
					Timer_Update(&enemy->timerWaitingTime, _dt);
					if (Timer_Check(&enemy->timerWaitingTime))
					{
						Timer_Update(&enemy->timerToDisplayColor, _dt);
						if (Timer_Check(&enemy->timerToDisplayColor))
						{
							enemy->isHit = FALSE;
						}
					}
				}
			}
		}
		// Kill the enemy
		if (enemy->health <= 0.0f || enemy->isAlive == sfFalse)
		{
			Explosion_Create(_engine, _explosions, enemy->entity.position, enemy->explosionsSize, enemy->type);
			Enemy_Destroy(_enemies, i, _lasers);
		}
	}
}

void Enemy_Display(Engine *_engine, Enemies *_enemies)
{
	sfVector2f scale = _engine->gameScale;
	for (int i = 0; i < _enemies->number; i++)
	{
		Enemy *enemy = _enemies->array[i];
		float angleEnemy = RAD_TO_DEG(enemy->entity.angle) - 90.f;
		float angleTurret = RAD_TO_DEG(enemy->turret.entity.angle) - 90.f;
		sfVector2f positionEnemy = V2F_ADD_V2F(enemy->entity.position, enemy->entity.shake.shakingOffset);
		sfVector2f positionTurret = V2F_ADD_V2F(positionEnemy, enemy->turretOffset);
		// Display feedback
		if (enemy->isHit)
		{
			if (!Timer_Check(&enemy->timerToDisplayWhite))
			{
				sfRenderStates *renderState = Shader_TakingDamage(enemy->hitType);
				if (enemy->haveTurretLive)
				{
					Sprite_Display(_engine->canvas, enemy->sprite, enemy->entity.position, scale, angleEnemy, NULL);
					Sprite_Display(_engine->canvas, enemy->turret.sprite, positionTurret, scale, angleTurret, renderState);
				}
				else
				{
					Sprite_Display(_engine->canvas, enemy->sprite, positionEnemy, scale, angleEnemy, renderState);
				}
			}
			else
			{
				if (!Timer_Check(&enemy->timerWaitingTime))
				{
					Sprite_Display(_engine->canvas, enemy->sprite, positionEnemy, scale, angleEnemy, NULL);
					if (enemy->haveTurretLive)
					{
						Sprite_Display(_engine->canvas, enemy->turret.sprite, positionTurret, scale, angleTurret, NULL);
					}
				}
				else
				{
					sfRenderStates *renderState = Shader_TakingDamage(enemy->hitType);
					if (enemy->haveTurretLive)
					{
						Sprite_Display(_engine->canvas, enemy->sprite, enemy->entity.position, scale, angleEnemy, NULL);
						Sprite_Display(_engine->canvas, enemy->turret.sprite, positionTurret, scale, angleTurret, renderState);
					}
					else
					{
						Sprite_Display(_engine->canvas, enemy->sprite, positionEnemy, scale, angleEnemy, renderState);
					}
				}
			}
		}
		// Else display default sprite
		else
		{
			Sprite_Display(_engine->canvas, enemy->sprite, positionEnemy, scale, angleEnemy, NULL);
			if (enemy->haveTurretLive)
			{
				Sprite_Display(_engine->canvas, enemy->turret.sprite, positionTurret, scale, angleTurret, NULL);
			}
		}
#ifdef DEBUG
		Collider_Display(_engine, &enemy->largeCollider);
		Collider_Display(_engine, &enemy->smallCollider);
#endif // DEBUG
	}
}

void Enemy_Destroy(Enemies *_enemies, int _index, Lasers *_lasers)
{
	if (_enemies->number >= 1)
	{
		_enemies->number--;
		Laser_StopShooting(_lasers, _enemies->array[_index], &_enemies->array[_index]->weapon);
		free(_enemies->array[_index]);
		for (int i = _index; i < _enemies->number; i++)
		{
			_enemies->array[i] = _enemies->array[i + 1];
		}
	}
}

void Enemy_DestroyAll(Enemies *_enemies, Lasers *_lasers)
{
	for (int i = _enemies->number - 1; i >= 0; i--)
	{
		Laser_StopShooting(_lasers, _enemies->array[i], &_enemies->array[i]->weapon);
		free(_enemies->array[i]);
		_enemies->array[i] = NULL;
	}
	_enemies->number = 0;
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //

int Get_NumberOfType(Enemies *_enemies, EnemyType _type)
{
	int number = 0;
	for (int i = 0; i < _enemies->number; i++)
	{
		Enemy *enemy = _enemies->array[i];
		if (enemy->behaviour == _type)
		{
			number++;
		}
	}
	return number;
}

void CreateDefaultEnemy(Engine *_engine, Enemy *_enemy, Waypoint *_spawn)
{
	_enemy->currentWaypoint = _spawn->next;
}

void CreateFollowerEnemy(Engine *_engine, Enemy *_enemy, Waypoint *_spawn, Player *_player)
{
	_enemy->target = _player;
}

void CreateArtilleryEnemy(Engine *_engine, Enemy *_enemy, Waypoint *_spawn, Player *_player)
{
	_enemy->target = _player;
}

void CreateThiefEnemy(Engine* _engine, Enemy* _enemy)
{
	_enemy->target = NULL;
}

Enemy *GetEnemyBlueprintFromName(BP_Enemies *_EnemyBlueprints, const char *_enemyName)
{
	Enemy *enemyBlueprint = NULL;
	for (int i = 0; i < _EnemyBlueprints->number; i++)
	{
		if (strcmp(_EnemyBlueprints->array[i]->name, _enemyName) == 0)
		{
			enemyBlueprint = _EnemyBlueprints->array[i];
			return enemyBlueprint;
		}
	}
	return enemyBlueprint;
}

void UpdateDefaultEnemy(Engine *_engine, Enemy *_enemy, float _dt, List *_projectiles)
{
	if (_enemy->currentWaypoint)
	{
		(_enemy->currentWaypoint->position, 1.0f);
		if (Collider_CheckCollisionWithMouse(&_enemy->smallCollider, _enemy->currentWaypoint->position))
		{
			_enemy->currentWaypoint = _enemy->currentWaypoint->next;
		}
		if (_enemy->currentWaypoint)
		{
			_enemy->entity.angle = Angle_RadGetAngle(_enemy->entity.position, _enemy->currentWaypoint->position);
			_enemy->entity.velocity.x = cosf(_enemy->entity.angle) * _enemy->speed;
			_enemy->entity.velocity.y = sinf(_enemy->entity.angle) * _enemy->speed;
		}
	}
}

void UpdateFollowerEnemy(Engine *_engine, Enemy *_enemy, float _dt, List *_projectiles)
{
	if (_enemy->target)
	{
		_enemy->entity.angle = Angle_RadGetAngle(_enemy->entity.position, _enemy->target->entity.position);
	}
	else
	{
		_enemy->entity.angle = DEG_TO_RAD(90.0f);
	}
	_enemy->entity.velocity.x = cosf(_enemy->entity.angle) * _enemy->speed;
	_enemy->entity.velocity.y = sinf(_enemy->entity.angle) * _enemy->speed;
}

void UpdateArtilleryEnemy(Engine *_engine, Enemy *_enemy, float _dt, List *_projectiles, Lasers *_lasers)
{
	if (_enemy->target)
	{
		_enemy->entity.angle = Angle_RadGetAngle(_enemy->entity.position, _enemy->target->entity.position);
	}
	else
	{
		_enemy->entity.angle = DEG_TO_RAD(90.0f);
	}
	_enemy->entity.velocity.x = cosf(_enemy->entity.angle) * _enemy->speed;
	if (_enemy->entity.position.y < 150.0f)
	{
		_enemy->entity.velocity.y = sinf(_enemy->entity.angle) * _enemy->speed;
	}
	else
	{
		_enemy->entity.velocity.y = 0.0f;
	}
	_enemy->weapon.angle = _enemy->entity.angle;
	_enemy->weapon.position.x = _enemy->entity.position.x;
	_enemy->weapon.position.y = _enemy->entity.position.y;

	if (_enemy->weapon.behaviour != WEAPON_STRAIGHTED_LASER_ORIENTED)
	{
		Timer_Update(&_enemy->weapon.rate, _dt);
		if (Timer_Check(&_enemy->weapon.rate))
		{
			switch (RAND(1, 3))
			{
			default:
				Sound_PlayFromName(_engine->sound, "ProjectileEnemy1.wav");
				break;
			case 1:
				Sound_PlayFromName(_engine->sound, "ProjectileEnemy1.wav");
				break;
			case 2:
				Sound_PlayFromName(_engine->sound, "ProjectileEnemy2.wav");
				break;
			case 3:
				Sound_PlayFromName(_engine->sound, "ProjectileEnemy3.wav");
				break;
			}
			Projectile_Create(_engine, _projectiles, &_enemy->weapon);
			Timer_Reset(&_enemy->weapon.rate);
		}
	}
	else
	{
		if (Laser_IsAlreadyShooting(_lasers, _enemy, &_enemy->weapon) == sfFalse)
		{
			Laser_Create(_engine, _lasers, &_enemy->weapon, _enemy);
		}
	}
}

void UpdateThiefEnemy(Engine* _engine, Enemy* _enemy, float _dt, Money** _List)
{
	Entity* entity = &_enemy->entity;
	Money* TempMoney = *_List;
	Money* TempTarget = NULL;
	sfVector2i tempResolution = _engine->configuration->resolution;
	sfVector2f posEnemy = _enemy->entity.position;
	sfVector2f posTarget = { (float)(tempResolution.x / 2), (float)(-tempResolution.y / 2) };
	sfVector2f u = { cosf((entity->angle - 90.0f) * PI / 180.0f), sinf((entity->angle - 90.0f) * PI / 180.0f) };
	sfVector2f v = { 0, 0 };
	float distance = 5000.0f;
	float crossProduct = 0;
	float rotationSpeed = 180.0f;
	while (TempMoney)
	{
		sfVector2f posMoney = TempMoney->entity.position;
		float tempDist = (float)sqrt(pow(posMoney.x - posEnemy.x, 2) + pow(posMoney.y - posEnemy.y, 2));
		TempTarget = tempDist < distance ? TempMoney : TempTarget;
		distance = tempDist < distance ? tempDist : distance;
		TempMoney = TempMoney->Next;
	}
	posTarget = TempTarget ? TempTarget->entity.position : posTarget;
	v.x = posTarget.x - posEnemy.x;
	v.y = posTarget.y - posEnemy.y;
	crossProduct = u.x * v.y - u.y * v.x;
	entity->angle += crossProduct >= 0 ? rotationSpeed * _dt : -rotationSpeed * _dt;
	entity->position.x += _enemy->speed * cosf((entity->angle - 90.0f) * PI / 180.0f) * _dt;
	entity->position.y += _enemy->speed * sinf((entity->angle - 90.0f) * PI / 180.0f) * _dt;
	if (!TempTarget)
	{
		distance = (float)sqrt(pow(posEnemy.x - posTarget.x, 2) + pow(posEnemy.y - posTarget.y, 2));
		_enemy->health = distance <= 100 ? 0.0f : _enemy->health;
	}
}

float Enemy_CaculateDamageFromWeapon(Engine *_engine, Enemy *_enemy, Weapon *_weapon)
{
	float damage = 0.0f;

	// If the weapon is a laser, the damge are reduce by delta time
	if (_weapon->behaviour == WEAPON_STRAIGHTED_LASER_ORIENTED)
	{
		damage = _weapon->damage * _engine->dt;
	}
	else
	{
		damage = _weapon->damage;
	}
	// Calcualte if the weapon is the good type
	_enemy->hitType = HIT_DEFAULT;
	if (_weapon->type == _enemy->type)
	{
		_enemy->hitType = HIT_GOOD;
		_enemy->goodHitAmount++;
		damage *= _engine->currentDifficulty.weaponMultiplierGood;
	}
	else
	{
		_enemy->hitType = HIT_BAD;
		damage *= _engine->currentDifficulty.weaponMultiplierBad;
	}
	_enemy->hitAmount++;
	return damage;
}

void AdjustDifficultyToEnemy(Enemy *_enemy, Difficulty *_currentDif)
{
	if (_currentDif != NULL)
	{
		_enemy->health *= _currentDif->enemiesHealthMultiplier;
		_enemy->speed *= _currentDif->enemiesSpeedMultiplier;
		_enemy->weapon.precision *= _currentDif->enemiesPrecisionMultiplier;
		_enemy->weapon.baseDamage *= _currentDif->enemiesDamageMultiplier;
		_enemy->weapon.damage *= _currentDif->enemiesDamageMultiplier;
		_enemy->weapon.speed *= _currentDif->enemiesProjSpeedMultiplier;
		_enemy->weapon.rate.maxTime *= _currentDif->enemiesFiringRateMultiplier;
	}
}