#include "Boss.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

void L_Boss_GoTowardsPosition(Engine * _engine, Boss *_boss);
void L_Boss_GetPhaseFromFolder(Engine * _engine, const char *_folder, Boss *_boss);
void L_Boss_GetWeaponSlot(Engine * _engine, BossPhase *_phase, int _weaponSlot, FILE *_file);
void L_Boss_LoadPatternInfo(Engine * _engine, BossPhase *_phase, int _indexPattern, FILE *_file);
void L_Boss_LoadPatternWeapons(Engine * _engine, BossPhase *_phase, BossPattern *_pattern, int _indexWeapon, FILE *_file);
void L_Boss_ChangePhase(Engine * _engine, Boss *_boss);
void L_Boss_StartPhase(Engine * _engine, Boss *_boss);
void L_Boss_UpdatePattern(Engine * _engine, Boss *_boss, List *_projectiles, Lasers *_lasers, Enemies *_enemies, Paths *_paths);
void L_Boss_DisplayTurrets(Engine *_engine, Boss *_boss);

// -- SHARED FUNCTIONS ----------------------------------------------------- //

BP_Boss Boss_InitializeBlueprints(Engine * _engine)
{
	BP_Boss blueprints;
	ExploDir directory = ExploreDirectory(BOSS_DIRECTORY);

	printf("Initialization of boss : \n");
	blueprints.number = directory.nbrFiles;
	blueprints.array = calloc(directory.nbrFiles, sizeof(Boss*));

	for (int i = 0; i < directory.nbrFiles; i++)
	{
		char filepath[BUFFER_LARGE] = BOSS_DIRECTORY;
		strcat(filepath, "/");
		strcat(filepath, directory.fileNames[i]);
		FILE *file = fopen(filepath, "rt");
		Boss *boss = calloc(1, sizeof(Boss));
		if (file && boss)
		{
			char bossFolder[BUFFER_LARGE] = { 0 };
			char bossName[BUFFER_LARGE] = { 0 };

			while (!feof(file))
			{
				char keyWord[BUFFER_SMALL] = { 0 };
				sfBool didntFoundKeyWord = sfTrue;
				fscanf(file, "%s", keyWord);

				if (strcmp(keyWord, "NAME") == 0)
				{
					fscanf(file, "%s\n", &bossName);
					boss->name = _strdup(bossName);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "FOLDER") == 0)
				{
					fscanf(file, "%s\n", &bossFolder);
					L_Boss_GetPhaseFromFolder(_engine, bossFolder, boss);
					didntFoundKeyWord = sfFalse;
				}

				if (didntFoundKeyWord == sfTrue)
				{
					fscanf(file, "%[^\n]\n", keyWord);
				}
			}
			fclose(file);
			// Add the boss to the blueprints
			printf("(%d/%d)\t%s\n", i + 1, directory.nbrFiles, boss->name);
			blueprints.array[i] = boss;
		}
		else
		{
			printf("Problem while trying to create the boss blueprint %s\n", directory.fileNames[i]);
		}
	}
	printf("\n");
	return blueprints;
}

Boss Boss_Create(Engine *_engine, const char *_name)
{
	Boss boss;
	Boss *blueprint = NULL;
	sfVector2i tempResolution = _engine->configuration->resolution;
	// Find the blueprint

	boss.name = _strdup(_name);
	if (_strcmpi("Null", _name))
	{
		for (int i = 0; i < _engine->blueprints.boss.number; i++)
		{
			if (strcmp(_name, _engine->blueprints.boss.array[i]->name) == 0)
			{
				blueprint = _engine->blueprints.boss.array[i];
			}
		}
		boss.isActive = FALSE;
		if (blueprint)
		{
			// Copy the content of the blueprint
			boss = *blueprint;
			// Copy the content of the phases
			if (blueprint->nbPhases)
			{
				boss.phases = calloc(blueprint->nbPhases, sizeof(BossPhase));
				memcpy(boss.phases, blueprint->phases, blueprint->nbPhases * sizeof(BossPhase));
				///printf("create %d BossPhase\n", boss.nbPhases);
				///printf("create %d BossPhase\n", blueprint->nbPhases);
			}
			for (int iPhase = 0; iPhase < boss.nbPhases; iPhase++)
			{
				// Copy the colliders
				for (int iCollider = 0; iCollider < boss.phases->nbColliders; iCollider++)
				{
					boss.phases[iPhase].colliders[iCollider] = Collider_Copy(&blueprint->phases[iPhase].colliders[iCollider]);
				}
				// Copy the weakpoints
				boss.phases[iPhase].weakpoints = calloc(boss.phases[iPhase].nbWeakpoints, sizeof(BossWeakpoint));
				memcpy(boss.phases[iPhase].weakpoints, blueprint->phases[iPhase].weakpoints, boss.phases[iPhase].nbWeakpoints * sizeof(BossWeakpoint));
				for (int iWeakpoint = 0; iWeakpoint < boss.phases->nbWeakpoints; iWeakpoint++)
				{
					boss.phases[iPhase].weakpoints[iWeakpoint].collider = Collider_Copy(&blueprint->phases[iPhase].weakpoints[iWeakpoint].collider);
				}
				// Copy the content of the turrets
				if (blueprint->phases[iPhase].nbTurrets)
				{
					boss.phases[iPhase].turrets = calloc(blueprint->phases[iPhase].nbTurrets, sizeof(Turret));
					memcpy(boss.phases[iPhase].turrets, blueprint->phases[iPhase].turrets, blueprint->phases[iPhase].nbTurrets * sizeof(Turret));
					///printf("Create %d turret\n", boss.phases[iPhase].nbTurrets);
				}
				// Copy the content of the weapons
				if (blueprint->phases[iPhase].nbWeapons)
				{
					boss.phases[iPhase].weapons = calloc(blueprint->phases[iPhase].nbWeapons, sizeof(FiringMode));
					memcpy(boss.phases[iPhase].weapons, blueprint->phases[iPhase].weapons, blueprint->phases[iPhase].nbWeapons * sizeof(FiringMode));
					///printf("create %d FiringMode\n", boss.phases[iPhase].nbWeapons);
					///printf("create %d FiringMode\n", blueprint->phases[iPhase].nbWeapons);
				}
				// Copy the content of the patterns
				if (blueprint->phases[iPhase].nbPatterns)
				{
					boss.phases[iPhase].patterns = calloc(blueprint->phases[iPhase].nbPatterns, sizeof(BossPattern));
					memcpy(boss.phases[iPhase].patterns, blueprint->phases[iPhase].patterns, blueprint->phases[iPhase].nbPatterns * sizeof(BossPattern));
					///printf("create %d BossPattern\n", boss.phases[iPhase].nbPatterns);
					///printf("create %d BossPattern\n", blueprint->phases[iPhase].nbPatterns);
				}
				for (int iPatern = 0; iPatern < boss.phases[iPhase].nbPatterns; iPatern++)
				{
					BossPattern *localPattern = &boss.phases[iPhase].patterns[iPatern];
					BossPattern *blueprintPattern = &blueprint->phases[iPhase].patterns[iPatern];
					Timer_Reset(&localPattern->duration);
					Timer_Reset(&localPattern->wait);
					// Copy the spawns
					if (localPattern->nbEnemySpawn)
					{
						memcpy(localPattern->enemySpawn, blueprintPattern->enemySpawn, localPattern->nbEnemySpawn * sizeof(BossEnemySpawn));
					}
					// Copy the weapons
					if (localPattern->nbWeaponSlot)
					{
						localPattern->weaponSlot = calloc(localPattern->nbWeaponSlot, sizeof(BossPatternWeapon));
						memcpy(localPattern->weaponSlot, blueprintPattern->weaponSlot, localPattern->nbWeaponSlot * sizeof(BossPatternWeapon));
						///printf("create %d BossPatternWeapon\n", localPattern->nbWeaponSlot);
						///printf("create %d BossPatternWeapon\n", blueprintPattern->nbWeaponSlot);
					}
					for (int iWeaponSlot = 0; iWeaponSlot < localPattern->nbWeaponSlot; iWeaponSlot++)
					{
						///localPattern->weaponSlot[iWeaponSlot].pointer = blueprintPattern->weaponSlot[iWeaponSlot].pointer;
						Timer_Reset(&localPattern->weaponSlot[iWeaponSlot].duration);
						Timer_Reset(&localPattern->weaponSlot[iWeaponSlot].wait);
					}
				}
				// Create the animation for the introduction fo the boss
				boss.intro = GetAnim(_engine->anims, "Boss_Intro", "Boss_Introduction");
				boss.introEntity.position.x = (float)(tempResolution.x / 2);
				boss.introEntity.position.y = (float)(tempResolution.y / 2);
				InitEntityAnim(&boss.introEntity, boss.intro->maxTime);
				boss.isInIntroduction = TRUE;
			}
			// Set position of the boss at the start of the first phase
			switch (boss.phases[0].introduction)
			{
			case BOSS_INTRODUCTION_FROM_TOP:
				boss.entity.position.x = _engine->configuration->resolution.x / 2.0f;
				boss.entity.position.y = -100.0f * _engine->gameScale.y;
				break;
			case BOSS_INTRODUCTION_FROM_BOTTOM:
				boss.entity.position.x = _engine->configuration->resolution.x / 2.0f;
				boss.entity.position.y = _engine->configuration->resolution.y + 100.0f * _engine->gameScale.y;
				break;
			}
			// Create the logical collider
			boss.logicalCollider = Collider_CreateCircleCollider(boss.entity.position, 10.0f);
			// Various initializations
			boss.isAlive = TRUE;
			boss.invulnerability = Timer_Create(ENEMY_INVULNERABILITY_TIME);
			InitEntityAnim(&boss.entity, boss.phases[0].animation->maxTime);
			if (boss.phases[0].introductionAnimation)
			{
				InitEntityAnim(&boss.introductionAnimationEntity, boss.phases[0].introductionAnimation->maxTime);
			}
			// Difficulty
			for (int iPhase = 0; iPhase < boss.nbPhases; iPhase++)
			{
				///boss.phases[iPhase].maximumHealth *= _engine->currentDifficulty.enemiesHealthMultiplier;
				boss.phases[iPhase].health *= _engine->currentDifficulty.enemiesHealthMultiplier;
				///printf("health : %f\n", boss.phases[iPhase].health);
				for (int iTurret = 0; iTurret < blueprint->phases[iPhase].nbTurrets; iTurret++)
				{
					boss.phases[iPhase].turrets[iTurret].maximumHealth = boss.phases[iPhase].health / (float)blueprint->phases[iPhase].nbTurrets;
					boss.phases[iPhase].turrets[iTurret].health = boss.phases[iPhase].turrets[iTurret].maximumHealth;
					///printf("  - turret : %f\n", boss.phases[iPhase].turrets[iTurret].health);
				}
			}
		}
		else
		{
			printf("BOSS:001:ERROR : Failed to find the blueprint of %s\n", _name);
		}
	}
	else
	{
		boss.isAlive = TRUE;
		boss.isActive = FALSE;
	}
	return boss;
}

void Boss_Update(Engine* _engine, Boss *_boss, List *_projectiles, Lasers *_lasers, sfMusic ** _levelMusic, Enemies *_enemies, Paths *_paths)
{
	if (_strcmpi("Null", _boss->name))
	{
		BossPhase *currentPhase = &_boss->phases[_boss->currentPhase];
		static float timeWithIntro = 0.0f;
		static sfBool warningIsPlaying = sfFalse;
		float musicVolume = sfMusic_getVolume(*_levelMusic);
		if (_boss->isAlive && _boss->isActive)
		{
			if (_boss->isInIntroduction)
			{
				if (warningIsPlaying == sfFalse)
				{
					Sound_PlayFromName(_engine->sound, "BossWarning.wav");
					warningIsPlaying = sfTrue;
				}

				if (musicVolume <= 0)
				{
					sfMusic_stop(*_levelMusic);
				}
				else
				{
					musicVolume -= MUSIC_FADE_RATIO;
					sfMusic_setVolume(*_levelMusic, musicVolume);
				}

				timeWithIntro += _engine->dt;
				UpdateAnimation(_engine->dt, _boss->intro, &_boss->introEntity);
				if (timeWithIntro >= BOSS_TIME_INTRODUCTION)
				{
					_boss->isInIntroduction = FALSE;
					timeWithIntro = 0.0f;
					sfMusic_stop(*_levelMusic);
					*_levelMusic = sfMusic_createFromFile("Data/Musics/InGame/Boss.wav");
					sfMusic_play(*_levelMusic);
					sfMusic_setLoop(*_levelMusic, sfTrue);
					sfMusic_setVolume(*_levelMusic, (float)_engine->configuration->musicVolume);
					warningIsPlaying = sfFalse;
					///printf("end of intro\n");
				}
			}
			else
			{
				int index = 0;
				for (int i = 0; i < _engine->blueprints.boss.number; i++)
				{
					index = _engine->blueprints.boss.array[i]->name == _boss->name ? i : index;
				}
				if (currentPhase->introduction != BOSS_INTRODUCTION_NONE)
				{
					L_Boss_GoTowardsPosition(_engine, _boss);
				}
				if (_engine->blueprints.boss.array[index]->phases[_boss->currentPhase].health == 0)
				{
					_boss->phases[_boss->currentPhase].health += _engine->dt * _boss->phases[_boss->currentPhase + 1].health / (_boss->currentPhase > 0 ? 0.5f : 3);
					_boss->phases[_boss->currentPhase].health = _boss->phases[_boss->currentPhase].health > _boss->phases[_boss->currentPhase + 1].health ? _boss->phases[_boss->currentPhase + 1].health : _boss->phases[_boss->currentPhase].health;
				}
				// Update the animations
				_boss->introductionAnimationEntity.position = _boss->entity.position;
				UpdateAnimation(_engine->dt, currentPhase->animation, &_boss->entity);
				UpdateAnimation(_engine->dt, currentPhase->introductionAnimation, &_boss->introductionAnimationEntity);
				Timer_Update(&_boss->invulnerability, _engine->dt);
				// Update feedbacks
				for (int i = 0; i < currentPhase->nbWeakpoints; i++)
				{
					Timer_Update(&currentPhase->weakpoints[i].timerFeedback, _engine->dt);
				}
				// Update collider positions
				Collider_UpdatePosition(&_boss->logicalCollider, _boss->entity.position);
				for (int i = 0; i < currentPhase->nbColliders; i++)
				{
					Collider_UpdatePosition(&currentPhase->colliders[i], _boss->entity.position);
				}
				for (int i = 0; i < currentPhase->nbWeakpoints; i++)
				{
					Collider_UpdatePosition(&currentPhase->weakpoints[i].collider, _boss->entity.position);
				}
				L_Boss_UpdatePattern(_engine, _boss, _projectiles, _lasers, _enemies, _paths);
				L_Boss_ChangePhase(_engine, _boss);
				// Get the current sprite displayed
				_boss->entity.angle = 0.0f;
				if (currentPhase->animation && _boss->entity.currentFrame >= 0)
				{
					_boss->currentSprite = currentPhase->animation->sprites[_boss->entity.currentFrame];
				}
				else
				{
					_boss->currentSprite = NULL;
				}
			}
		}
	}
}

sfRenderStates *L_Boss_GetRenderState(Engine *_engine, Boss *_boss)
{
	BossPhase *currentPhase = &_boss->phases[_boss->currentPhase];
	sfRenderStates *renderstate;
	sfColor colors[4];
	for (int i = 0; i < currentPhase->nbWeakpoints; i++)
	{
		if (!Timer_Check(&currentPhase->weakpoints[i].timerFeedback))
		{
			colors[i] = currentPhase->weakpoints[i].color;
		}
	}
	renderstate = Shader_Boss(colors, currentPhase->nbWeakpoints, currentPhase->maskWeakpoints);
	return renderstate;
}

void Boss_Display(Engine *_engine, Boss *_boss)
{
	if (_strcmpi("NULL", _boss->name))
	{
		BossPhase *currentPhase = &_boss->phases[_boss->currentPhase];
		if (_boss->isAlive && _boss->isActive)
		{
			if (!_boss->isInIntroduction)
			{
				sfRenderStates *renderState = L_Boss_GetRenderState(_engine, _boss);
				_boss->entity.angle = 0.0f;
				if (!currentPhase->turretsAreBehind)
				{
					L_Boss_DisplayTurrets(_engine, _boss);
				}
				DisplayAnimation(_engine->canvas, currentPhase->animation, &_boss->entity, _engine->gameScale, renderState);
				DisplayAnimation(_engine->canvas, currentPhase->introductionAnimation, &_boss->introductionAnimationEntity, _engine->gameScale, NULL);
				if (currentPhase->turretsAreBehind)
				{
					L_Boss_DisplayTurrets(_engine, _boss);
				}
#ifdef DEBUG
				// Display the colliders
				///Collider_Display(_engine, &_boss->logicalCollider);
				for (int i = 0; i < currentPhase->nbColliders; i++)
				{
					Collider_Display(_engine, &currentPhase->colliders[i]);
				}
				for (int i = 0; i < currentPhase->nbWeakpoints; i++)
				{
					Collider_Display(_engine, &currentPhase->weakpoints[i].collider);
				}
#endif // DEBUG
			}
		}
	}
}

float Boss_CalculateDamage(Engine * _engine, Weapon * _weaponUsed)
{
	float damage = 0.0f;
	if (_weaponUsed)
	{
		damage = _weaponUsed->damage;
		if (_weaponUsed->behaviour == WEAPON_STRAIGHTED_LASER_ORIENTED)
		{
			damage *= _engine->dt;
		}
	}
	return damage;
}

void Boss_TakeDamageFromWeakpoint(Engine * _engine, Boss *_boss, int _weakpointID, float _damage)
{
	BossPhase *currentPhase = &_boss->phases[_boss->currentPhase];
	BossWeakpoint *weakpoint = &currentPhase->weakpoints[_weakpointID];
	if (_damage)
	{
		weakpoint->isHit = TRUE;
		Timer_Reset(&weakpoint->timerFeedback);
		currentPhase->health -= _damage;
		if (currentPhase->health < 0.0f)
		{
			currentPhase->health = 0.0f;
		}
	}
}

void Boss_TakeDamageFromTurret(Engine * _engine, Boss *_boss, int _turretID, float _damage)
{
	if (_strcmpi("Null", _boss->name))
	{
		BossPhase *currentPhase = &_boss->phases[_boss->currentPhase];
		Turret *turret = &currentPhase->turrets[_turretID];
		if (_damage)
		{
			if (turret->health > 0)
			{
				turret->health -= _damage;
				currentPhase->health -= _damage;
				if (currentPhase->health < 0.0f)
				{
					currentPhase->health = 0.0f;
				}
			}
			Timer_Reset(&turret->timerFeedback);
		}
	}
}

void Boss_TakeDamage(Engine * _engine, Boss *_boss, Weapon * _weaponUsed)
{
	BossPhase *currentPhase = &_boss->phases[_boss->currentPhase];
	if (Timer_Check(&currentPhase->timerTurretActivation))
	{
		float damage = Boss_CalculateDamage(_engine, _weaponUsed);
		currentPhase->health -= damage;
		// Avoid the health to be negative
		if (currentPhase->health < 0.0f)
		{
			currentPhase->health = 0.0f;
		}
	}
}

void Boss_UpdateTurret(Engine *_engine, Boss *_boss, Players *_players, List *_projectiles)
{
	if (_strcmpi("NULL", _boss->name) != 0 && _boss->isAlive)
	{
		BossPhase *currentPhase = &_boss->phases[_boss->currentPhase];
		if (Timer_Check(&currentPhase->timerTurretActivation))
		{
			for (size_t iTurret = 0; iTurret < currentPhase->nbTurrets; iTurret++)
			{
				Turret *turret = &currentPhase->turrets[iTurret];
				sfVector2f positionToAim;
				float minimumDistance = 10000000000000.0f;
				for (size_t iPlayer = 0; iPlayer < _players->number; iPlayer++)
				{
					float distance = 0.0f;
					Player *currentPlayer = _players->array[iPlayer];
					distance = Angle_CalculateDistance(turret->entity.position, currentPlayer->entity.position);
					distance = fabsf(distance);
					if (distance < minimumDistance && currentPlayer->isAlive && currentPlayer->canBeTargeted)
					{
						minimumDistance = distance;
						positionToAim = currentPlayer->entity.position;
					}
				}
				Turret_Update(_engine, turret, _projectiles, positionToAim, _boss->entity.position);
			}
		}
		else
		{
			Timer_Update(&currentPhase->timerTurretActivation, _engine->dt);
		}
	}
}

void Boss_Destroy(Engine *_engine, Boss *_boss)
{

}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //

void L_Boss_GoTowardsPosition(Engine * _engine, Boss *_boss)
{
	BossPhase *currentPhase = &_boss->phases[_boss->currentPhase];
	if (Collider_CheckCollisionWithMouse(&_boss->logicalCollider, currentPhase->goalPosition))
	{
		_boss->entity.position = currentPhase->goalPosition;
	}
	else
	{
		_boss->entity.angle = Angle_RadGetAngle(_boss->entity.position, currentPhase->goalPosition);
		_boss->entity.position.x += cosf(_boss->entity.angle) * currentPhase->velocity * _engine->dt;
		_boss->entity.position.y += sinf(_boss->entity.angle) * currentPhase->velocity * _engine->dt;
	}
}

void L_Boss_GetPhaseFromFolder(Engine * _engine, const char *_folder, Boss *_boss)
{
	Animations *animations = _engine->anims;
	BP_Weapons *weaponsBlueprints = &_engine->blueprints.weapons;
	BP_Turrets *turretsBlueprints = &_engine->blueprints.turrets;

	ExploDir directory = ExploreDirectory(_folder);

	_boss->nbPhases = directory.nbrFiles;
	///printf("nb of phases : %d\n", _boss->nbPhases);
	_boss->phases = calloc(_boss->nbPhases, sizeof(BossPhase));

	for (int iPhase = 0; iPhase < _boss->nbPhases; iPhase++)
	{
		char filepath[BUFFER_LARGE] = { 0 };
		strcat(filepath, _folder);
		strcat(filepath, "/");
		strcat(filepath, directory.fileNames[iPhase]);
		FILE *file = fopen(filepath, "rt");
		if (file)
		{
			int currentSlot = 0;
			int currentWeaponInPattern = 0;

			while (!feof(file))
			{
				char keyWord[BUFFER_SMALL] = { 0 };
				sfBool didntFoundKeyWord = sfTrue;
				fscanf(file, "%s", keyWord);

				if (strcmp(keyWord, "INTRODUCTION_TYPE") == 0)
				{
					fscanf(file, "%d\n", &_boss->phases[iPhase].introduction);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "PHASE_ENDING_TYPE") == 0)
				{
					fscanf(file, "%d\n", &_boss->phases[iPhase].ending);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "PHASE_ENDING_TIMER") == 0)
				{
					float time;
					fscanf(file, "%f\n", &time);
					_boss->phases[iPhase].endingTimer = Timer_Create(time);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "DAMAGED_FROM") == 0)
				{
					fscanf(file, "%d\n", &_boss->phases[iPhase].damagedFrom);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "NB_TURRETS") == 0)
				{
					fscanf(file, "%d\n", &_boss->phases[iPhase].nbTurrets);
					_boss->phases[iPhase].turrets = calloc(_boss->phases[iPhase].nbTurrets, sizeof(Turret));
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "TURRET") == 0)
				{
					int id;
					char name[BUFFER_SMALL];
					int hiding;
					sfVector2f position;
					fscanf(file, "%d NAME %s POSITION %f %f %d\n", &id, &name, &position.x, &position.y, &hiding);
					_boss->phases[iPhase].turrets[id] = Turret_Create(_engine, name, WEAPON_OWNER_BOSS, _boss, position);
					_boss->phases[iPhase].turretsAreBehind = hiding;
					_boss->phases[iPhase].turrets[id].timerFeedback = Timer_Create(FEEDBACK_TIME_COLORED);
					_boss->phases[iPhase].turrets[id].timerFeedback.currentTime = FEEDBACK_TIME_COLORED;
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "TURRET_WAITING_TIME") == 0)
				{
					float time;
					fscanf(file, "%f\n", &time);
					_boss->phases[iPhase].timerTurretActivation = Timer_Create(time);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "HEALTH") == 0)
				{
					fscanf(file, "%f\n", &_boss->phases[iPhase].health);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "GOAL_POSITION") == 0)
				{
					fscanf(file, "%f %f\n", &_boss->phases[iPhase].goalPosition.x, &_boss->phases[iPhase].goalPosition.y);
					_boss->phases[iPhase].goalPosition.x *= _engine->configuration->resolution.x;
					_boss->phases[iPhase].goalPosition.y *= _engine->configuration->resolution.y;
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "INVULNERABILITY") == 0)
				{
					fscanf(file, "%d\n", &_boss->phases[iPhase].isInvulnerable);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "ANIMATION") == 0)
				{
					char animation[BUFFER_LARGE] = "NONE";
					fscanf(file, "%s\n", &animation);
					_boss->phases[iPhase].animation = GetAnim(animations, _boss->name, animation);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "INTRODUCTION_ANIMATION") == 0)
				{
					char animation[BUFFER_LARGE] = "NONE";
					fscanf(file, "%s", &animation);
					_boss->phases[iPhase].introductionAnimation = GetAnim(animations, _boss->name, animation);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "WEAKPOINT_MASK") == 0)
				{
					char buffer[BUFFER_LARGE] = { 0 };
					fscanf(file, "%s", buffer);
					_boss->phases[iPhase].maskWeakpoints = sfTexture_createFromFile(buffer, NULL);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "NB_WEAKPOINT") == 0)
				{
					fscanf(file, "%d", &_boss->phases[iPhase].nbWeakpoints);
					_boss->phases[iPhase].weakpoints = calloc(_boss->phases[iPhase].nbWeakpoints, sizeof(BossWeakpoint));
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "WEAKPOINT_COLLIDER") == 0)
				{
					int index = 0;
					int type = 0;
					fscanf(file, "%d TYPE %d", &index, &type);
					_boss->phases[iPhase].weakpoints[index].collider.type = type;
					Collider_CreateFromFile(&_boss->phases[iPhase].weakpoints[index].collider, file);
					didntFoundKeyWord = sfFalse;
					_boss->phases[iPhase].weakpoints[index].timerFeedback = Timer_Create(0.05f);
				}

				if (strcmp(keyWord, "WEAKPOINT_MASK_COLOR") == 0)
				{
					int index = 0;
					int r = 255;
					int g = 255;
					int b = 255;
					fscanf(file, "%d %d %d %d", &index, &r, &g, &b);
					_boss->phases[iPhase].weakpoints[index].color = sfColor_fromRGB(r, g, b);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "VELOCITY") == 0)
				{
					fscanf(file, "%f\n", &_boss->phases[iPhase].velocity);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "NB_WEAPONS") == 0)
				{
					fscanf(file, "%d\n", &_boss->phases[iPhase].nbWeapons);
					///printf("nb of weapons in the phase n%d : %d\n", iPhase, _boss->phases[iPhase].nbWeapons);
					_boss->phases[iPhase].weapons = calloc(_boss->phases[iPhase].nbWeapons, sizeof(FiringMode));
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "WEAPON_SLOT") == 0)
				{
					L_Boss_GetWeaponSlot(_engine, &_boss->phases[iPhase], currentSlot, file);
					currentSlot++;
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "NB_PATTERNS") == 0)
				{
					fscanf(file, "%d\n", &_boss->phases[iPhase].nbPatterns);
					///printf("nb of patterns in phase n%d : %d\n", iPhase, _boss->phases[iPhase].nbPatterns);
					_boss->phases[iPhase].patterns = calloc(_boss->phases[iPhase].nbPatterns, sizeof(BossPattern));
					///printf("%p\tAT INITIALIZATION\n", _boss->phases[iPhase].patterns);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "PATTERN_INFO") == 0)
				{
					int indexPattern = 0;
					fscanf(file, "%d\n", &indexPattern);
					L_Boss_LoadPatternInfo(_engine, &_boss->phases[iPhase], indexPattern, file);
					currentWeaponInPattern = 0;
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "PATTERN_MOVEMENT") == 0)
				{
					int indexPattern = 0;
					Boss_MovementAxis axis;
					float min = 0.0f;
					float max = 0.0f;
					float speed = 1.0f;
					fscanf(file, "%d %d %f %f %f\n", &indexPattern, &axis, &speed, &min, &max);

					if (axis == BOSS_MOVEMENT_AXIS_X)
					{
						_boss->phases[iPhase].patterns[indexPattern].haveXMovement = TRUE;
						_boss->phases[iPhase].patterns[indexPattern].isMovingRight = RAND(FALSE, TRUE);
						_boss->phases[iPhase].patterns[indexPattern].speedXMovement = speed;
						_boss->phases[iPhase].patterns[indexPattern].minXMovement = min;
						_boss->phases[iPhase].patterns[indexPattern].maxXMovement = max;
					}
					else if (axis == BOSS_MOVEMENT_AXIS_Y)
					{
						_boss->phases[iPhase].patterns[indexPattern].haveYMovement = TRUE;
						_boss->phases[iPhase].patterns[indexPattern].isMovingBottom = RAND(FALSE, TRUE);
						_boss->phases[iPhase].patterns[indexPattern].speedYMovement = speed;
						_boss->phases[iPhase].patterns[indexPattern].minYMovement = min;
						_boss->phases[iPhase].patterns[indexPattern].maxYMovement = max;
					}
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "PATTERN_SPAWN_NUMBER") == 0)
				{
					int indexPattern = 0;
					int nbSpawn = 0;
					fscanf(file, "%d %d\n", &indexPattern, &nbSpawn);
					BossPattern *currentPattern = &_boss->phases[iPhase].patterns[indexPattern];
					currentPattern->nbEnemySpawn = nbSpawn;
					currentPattern->enemySpawn = calloc(nbSpawn, sizeof(BossEnemySpawn));
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "PATTERN_SPAWN") == 0)
				{
					int indexPattern = 0;
					int indexSpawn = 0;
					char enemyName[BUFFER_LARGE];
					int direction = 0;
					int number = 0;
					float position = 0.0f;
					float offset = 0.0f;
					float time = 0.0f;

					fscanf(file,
						"%d ID_SPAWN %d NAME %s DIRECTION %d NB %d POSITION %f OFFSET %f RELOAD_TIME %f\n",
						&indexPattern, &indexSpawn, enemyName, &direction, &number, &position, &offset, &time);
					BossPattern *currentPattern = &_boss->phases[iPhase].patterns[indexPattern];
					currentPattern->enemySpawn[indexSpawn].direction = direction;
					strcpy(currentPattern->enemySpawn[indexSpawn].enemyName, enemyName);

					sfVector2f startingPosition;
					if (currentPattern->enemySpawn[indexSpawn].direction == BOSS_ENEMY_SPAWN_TOP)
					{
						startingPosition = V2F(position * _engine->configuration->resolution.x, -100.0f);
					}
					else if (currentPattern->enemySpawn[indexSpawn].direction == BOSS_ENEMY_SPAWN_BOTTOM)
					{
						startingPosition = V2F(position * _engine->configuration->resolution.x, _engine->configuration->resolution.y + 100.0f);
					}
					else if (currentPattern->enemySpawn[indexSpawn].direction == BOSS_ENEMY_SPAWN_LEFT)
					{
						startingPosition = V2F(-100.0f, position * _engine->configuration->resolution.y);
					}
					else if (currentPattern->enemySpawn[indexSpawn].direction == BOSS_ENEMY_SPAWN_RIGHT)
					{
						startingPosition = V2F(_engine->configuration->resolution.x + 100.0f, position * _engine->configuration->resolution.y);
					}
					currentPattern->enemySpawn[indexSpawn].startingPosition = startingPosition;
					currentPattern->enemySpawn[indexSpawn].timer = Timer_Create(time);
					currentPattern->enemySpawn[indexSpawn].offset = offset;
					currentPattern->enemySpawn[indexSpawn].number = number;
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "PATTERN_WEAPON") == 0)
				{
					int indexPattern = 0;
					fscanf(file, "%d\n", &indexPattern);
					///printf("%p\tBEFORE FUNCTION at %d\n", &_boss->phases[iPhase].patterns[indexPattern], indexPattern);
					L_Boss_LoadPatternWeapons(_engine, &_boss->phases[iPhase], &_boss->phases[iPhase].patterns[indexPattern], currentWeaponInPattern, file);
					currentWeaponInPattern++;
					didntFoundKeyWord = sfFalse;
				}

				if (didntFoundKeyWord == sfTrue)
				{
					fscanf(file, "%[^\n]\n", keyWord);
				}
			}
			fclose(file);
		}
		else
		{
			printf("Failed to open boss phase file : %s\n", filepath);
		}
	}
}

void L_Boss_GetWeaponSlot(Engine * _engine, BossPhase *_phase, int _weaponSlot, FILE *_file)
{
	char temp_string[BUFFER_EXTRA_LARGE] = { 0 };

	fscanf(_file, " NAME %s", temp_string);
	_phase->weapons[_weaponSlot].name = _strdup(temp_string);
	fscanf(_file, " WEAPON_NAME %s", temp_string);
	_phase->weapons[_weaponSlot].weapon = GetWeapon(temp_string, &_engine->blueprints.weapons, WEAPON_OWNER_ENEMY, NULL);

	fscanf(_file, " NB_TURRETS %d", &_phase->weapons[_weaponSlot].number);
	_phase->weapons[_weaponSlot].offsets = calloc(_phase->weapons[_weaponSlot].number, sizeof(sfVector2f));

	_phase->weapons[_weaponSlot].rate = Timer_Create(_phase->weapons[_weaponSlot].weapon.rate.maxTime);
	_phase->weapons[_weaponSlot].angle = _phase->weapons[_weaponSlot].weapon.angle;
	for (int i = 0; i < _phase->weapons[_weaponSlot].number; i++)
	{
		sfVector2f posTurret;
		fscanf(_file, " POSITION %f %f", &posTurret.x, &posTurret.y);
		posTurret.x *= SCALE_SPRITE;
		posTurret.y *= SCALE_SPRITE;
		_phase->weapons[_weaponSlot].offsets[i] = posTurret;
	}
	fscanf(_file, "%[^\n]\n", temp_string);
}

void L_Boss_LoadPatternInfo(Engine * _engine, BossPhase *_phase, int _indexPattern, FILE *_file)
{
	fscanf(_file, " NB_WEAPONS %d", &_phase->patterns[_indexPattern].nbWeaponSlot);
	fscanf(_file, " WAIT %f", &_phase->patterns[_indexPattern].wait.maxTime);
	fscanf(_file, " DURATION %f", &_phase->patterns[_indexPattern].duration.maxTime);
	///printf("nb of weapons in the pattern n%d : %d\n", _indexPattern, _phase->patterns[_indexPattern].nbWeaponSlot);
	_phase->patterns[_indexPattern].weaponSlot = calloc(_phase->patterns[_indexPattern].nbWeaponSlot, sizeof(BossPatternWeapon));
}

FiringMode *L_Boss_LookForWeaponSlot(BossPhase *_phase, const char *_name)
{
	for (int i = 0; i < _phase->nbWeapons; i++)
	{
		if (strcmp(_name, _phase->weapons[i].name) == 0)
		{
			return &_phase->weapons[i];
		}
	}
	printf("Failed to find the %s weapon for the boss pattern.\n", _name);
	return NULL;
}

void L_Boss_LoadPatternWeapons(Engine * _engine, BossPhase *_phase, BossPattern *_pattern, int _indexWeapon, FILE *_file)
{
	char temp_string[BUFFER_EXTRA_LARGE];
	float wait = 0.0f;
	float duration = 0.0f;
	///printf("%p\tIN FUNCTION\n", _pattern);
	fscanf(_file, " SLOT_NAME %s", temp_string);
	_pattern->weaponSlot[_indexWeapon].pointer = L_Boss_LookForWeaponSlot(_phase, temp_string);
	fscanf(_file, " WAIT %f", &wait);
	fscanf(_file, " DURATION %f", &duration);
	fscanf(_file, " REPEAT %d", &_pattern->weaponSlot[_indexWeapon].isRepeated);

	_pattern->weaponSlot[_indexWeapon].wait = Timer_Create(wait);
	_pattern->weaponSlot[_indexWeapon].duration = Timer_Create(duration);
}

void L_Boss_ChangePhase(Engine * _engine, Boss *_boss)
{
	BossPhase *currentPhase = &_boss->phases[_boss->currentPhase];
	switch (currentPhase->ending)
	{
		default:
		{
			if (currentPhase->health <= 0.0f)
			{
				L_Boss_StartPhase(_engine, _boss);
			}
			break;
		}
		case BOSS_PHASE_ENDING_POSITION:
		{
			if (V2F_COMPARE(_boss->entity.position, currentPhase->goalPosition))
			{
				L_Boss_StartPhase(_engine, _boss);
			}
			break;
		}
		case BOSS_PHASE_ENDING_ANIMATION:
		{
			if (Timer_Check(&_boss->entity.animationTimer))
			{
				L_Boss_StartPhase(_engine, _boss);
			}
			break;
		}
		case BOSS_PHASE_ENDING_TIME:
		{
			Timer_Update(&currentPhase->endingTimer, _engine->dt);
			if (Timer_Check(&currentPhase->endingTimer))
			{
				L_Boss_StartPhase(_engine, _boss);
			}
			break;
		}
	}
}

void L_Boss_StartPhase(Engine * _engine, Boss *_boss)
{
	BossPhase *currentPhase = NULL;
	_boss->currentPhase++;
	currentPhase = &_boss->phases[_boss->currentPhase];
	// Kill the boss if all phases has been completed
	if (_boss->currentPhase >= _boss->nbPhases)
	{
		_boss->currentPhase = 0;
		_boss->isAlive = sfFalse;
	}
	else
	{
		InitEntityAnim(&_boss->entity, currentPhase->animation->maxTime);
		if (currentPhase->introductionAnimation)
		{
			InitEntityAnim(&_boss->introductionAnimationEntity, currentPhase->introductionAnimation->maxTime);
		}
	}
}

void L_Boss_UpdatePattern(Engine * _engine, Boss *_boss, List *_projectiles, Lasers *_lasers, Enemies *_enemies, Paths *_paths)
{
	static Waypoint *spawn = NULL;
	BossPhase *currentPhase = &_boss->phases[_boss->currentPhase];
	BossPattern *currentPatern = &currentPhase->patterns[currentPhase->currentPattern];
	if (currentPatern)
	{
		Timer_Update(&currentPatern->wait, _engine->dt);
		if (Timer_Check(&currentPatern->wait))
		{
			Timer_Update(&currentPatern->duration, _engine->dt);
			// Change the current pattern
			if (Timer_Check(&currentPatern->duration))
			{
				// Set the new pattern
				currentPhase->currentPattern++;
				if (currentPhase->currentPattern >= currentPhase->nbPatterns)
				{
					currentPhase->currentPattern = 0;
				}
				currentPatern = &currentPhase->patterns[currentPhase->currentPattern];
				// Reset timers for the pattern
				Timer_Reset(&currentPatern->duration);
				Timer_Reset(&currentPatern->wait);
				for (int iWeaponSlot = 0; iWeaponSlot < currentPatern->nbWeaponSlot; iWeaponSlot++)
				{
					Timer_Reset(&currentPatern->weaponSlot[iWeaponSlot].duration);
					Timer_Reset(&currentPatern->weaponSlot[iWeaponSlot].wait);
				}
			}
			else
			{
				// Update X movement
				if (currentPatern->haveXMovement)
				{
					if (currentPatern->isMovingRight)
					{
						float max = _engine->configuration->resolution.x / 2.0f + currentPatern->maxXMovement;
						_boss->entity.position.x += currentPatern->speedXMovement * _engine->dt;
						if (_boss->entity.position.x >= max)
						{
							_boss->entity.position.x = max;
							currentPatern->isMovingRight = FALSE;
						}
					}
					else
					{
						float min = _engine->configuration->resolution.x / 2.0f - currentPatern->minXMovement;
						_boss->entity.position.x -= currentPatern->speedXMovement * _engine->dt;
						if (_boss->entity.position.x <= min)
						{
							_boss->entity.position.x = min;
							currentPatern->isMovingRight = TRUE;
						}
					}
				}
				// Update Y movement
				if (currentPatern->haveYMovement)
				{
					if (currentPatern->isMovingBottom)
					{
						float max = _engine->configuration->resolution.y / 2.0f + currentPatern->maxYMovement;
						_boss->entity.position.y += currentPatern->speedYMovement * _engine->dt;
						if (_boss->entity.position.y >= max)
						{
							_boss->entity.position.y = max;
							currentPatern->isMovingBottom = FALSE;
						}
					}
					else
					{
						float min = _engine->configuration->resolution.y / 2.0f - currentPatern->minYMovement;
						_boss->entity.position.y -= currentPatern->speedYMovement * _engine->dt;
						if (_boss->entity.position.y <= min)
						{
							_boss->entity.position.y = min;
							currentPatern->isMovingBottom = TRUE;
						}
					}
				}
				// Update enemy spawn
				if (currentPatern->nbEnemySpawn > 0)
				{
					if (!spawn || !spawn->content)
					{
						spawn = _Waypoint_Create(_paths, WAYPOINT_SPAWN, V2F_NULL);
					}
					for (int spawnIndex = 0; spawnIndex < currentPatern->nbEnemySpawn; spawnIndex++)
					{
						Enemy *enemy;
						Waypoint_Spawn *spawnContent = spawn->content;
						BossEnemySpawn *spawnPattern = &currentPatern->enemySpawn[spawnIndex];
						Timer_Update(&spawnPattern->timer, _engine->dt);
						if (Timer_Check(&spawnPattern->timer))
						{
							///printf("spawn %p\n", spawn);
							for (int iEnemy = 0; iEnemy < spawnPattern->number; iEnemy++)
							{
								sfVector2f spawnPosition = spawnPattern->startingPosition;
								sfVector2f goalPosition = spawnPattern->startingPosition;
								strcpy(spawnContent->enemyName, spawnPattern->enemyName);
								///printf("spawnPosition %f %f\n", spawnPosition.x, spawnPosition.y);
								switch (spawnPattern->direction)
								{
									case BOSS_ENEMY_SPAWN_TOP:
										spawnPosition.x += spawnPattern->offset * (float)iEnemy;
										spawnPosition.y = -100.0f;
										goalPosition.x = spawnPosition.x;
										goalPosition.y = _engine->configuration->resolution.y + 100.0f;
										break;
									case BOSS_ENEMY_SPAWN_RIGHT:
										spawnPosition.x = _engine->configuration->resolution.x + 100.0f;
										spawnPosition.y += spawnPattern->offset * (float)iEnemy;

										goalPosition.x = -100.0f;
										goalPosition.y = spawnPosition.y;
										break;
									case BOSS_ENEMY_SPAWN_BOTTOM:
										spawnPosition.x += spawnPattern->offset * (float)iEnemy;
										spawnPosition.y = _engine->configuration->resolution.y + 100.0f;

										goalPosition.x = spawnPosition.x;
										goalPosition.y = -100.0f;
										break;
									case BOSS_ENEMY_SPAWN_LEFT:
										spawnPosition.x = -100.0f;
										spawnPosition.y += spawnPattern->offset * (float)iEnemy;

										goalPosition.x = _engine->configuration->resolution.x + 100.0f;
										goalPosition.y = spawnPosition.y;

										break;
								}
								spawn->position = spawnPosition;
								enemy = Enemy_Create(_engine, _enemies, spawn, NULL, &_engine->currentDifficulty);
								if (enemy)
								{
									enemy->entity.angle = Angle_RadGetAngle(spawnPosition, goalPosition);
									enemy->entity.velocity.x = cosf(enemy->entity.angle) * enemy->speed;
									enemy->entity.velocity.y = sinf(enemy->entity.angle) * enemy->speed;
								}
								Timer_Reset(&spawnPattern->timer);
							}
						}
					}
				}
				// Update the weapons
				for (int iWeaponSlot = 0; iWeaponSlot < currentPatern->nbWeaponSlot; iWeaponSlot++)
				{
					FiringMode *currentFiringMode = currentPatern->weaponSlot[iWeaponSlot].pointer;

					Timer_Update(&currentPatern->weaponSlot[iWeaponSlot].wait, _engine->dt);
					if (Timer_Check(&currentPatern->weaponSlot[iWeaponSlot].wait))
					{
						Timer_Update(&currentPatern->weaponSlot[iWeaponSlot].duration, _engine->dt);
						if (!Timer_Check(&currentPatern->weaponSlot[iWeaponSlot].duration))
						{
							if (currentFiringMode)
							{
								// Shoot with conventional weapons
								Timer_Update(&currentFiringMode->rate, _engine->dt);
								if (Timer_Check(&currentFiringMode->rate))
								{
									if (currentFiringMode->weapon.behaviour != WEAPON_STRAIGHTED_LASER_ORIENTED)
									{
										if (currentFiringMode->weapon.aimType == WEAPON_AIMING_ROTATION)
										{
											currentFiringMode->weapon.angle += currentFiringMode->weapon.aimingRotationSpeed * _engine->dt;
										}
										for (int iWeapon = 0; iWeapon < currentFiringMode->number; iWeapon++)
										{
											sfVector2f offset = currentFiringMode->offsets[iWeapon];
											Weapon_SetPosition(&currentFiringMode->weapon, _boss->entity.position, offset);
											Projectile_Create(_engine, _projectiles, &currentFiringMode->weapon);
										}
										Timer_Reset(&currentFiringMode->rate);
									}
									// Shoot with lasers
									else
									{
										for (int iWeapon = 0; iWeapon < currentFiringMode->number; iWeapon++)
										{
											sfVector2f offset = currentFiringMode->offsets[iWeapon];
											Weapon_SetPosition(&currentFiringMode->weapon, _boss->entity.position, offset);
											currentFiringMode->weapon.owner = WEAPON_OWNER_BOSS;
											if (Laser_IsAlreadyShooting(_lasers, _boss, &currentFiringMode->weapon) == sfFalse)
											{
												Laser_Create(_engine, _lasers, &currentFiringMode->weapon, _boss);
											}
										}
									}
								}
							}
						}
						else
						{
							if (currentFiringMode)
							{
								// Stop the laser from shooting
								if (currentFiringMode->weapon.behaviour == WEAPON_STRAIGHTED_LASER_ORIENTED)
								{
									Laser_StopShooting(_lasers, _boss, &currentFiringMode->weapon);
								}
								Timer_Reset(&currentPatern->weaponSlot[iWeaponSlot].wait);
								Timer_Reset(&currentPatern->weaponSlot[iWeaponSlot].duration);
							}
						}
					}
				}
			}
		}
	}
}

void L_Boss_DisplayTurrets(Engine *_engine, Boss *_boss)
{
	if (_strcmpi("Null", _boss->name))
	{
		BossPhase *currentPhase = &_boss->phases[_boss->currentPhase];
		if (Timer_Check(&currentPhase->timerTurretActivation))
		{
			for (size_t iTurret = 0; iTurret < currentPhase->nbTurrets; iTurret++)
			{
				Turret *turret = &currentPhase->turrets[iTurret];
				if (Timer_Check(&turret->timerFeedback) || currentPhase->damagedFrom == BOSS_DAMAGED_FROM_WEAKPOINT)
				{
					Turret_Display(_engine, turret, NULL);
				}
				else
				{
					Turret_Display(_engine, turret, Shader_TakingDamage(HIT_GOOD));
				}
			}
		}
	}
}