#include "Turret.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

// -- SHARED FUNCTIONS ----------------------------------------------------- //

BP_Turrets Turret_LoadBlueprints()
{
	printf("Initialization of turrets blueprints : \n");
	BP_Turrets blueprints;
	ExploDir directory = ExploreDirectory(TURRETS_DIR);
	// Allocate the array
	blueprints.number = directory.nbrFiles;
	blueprints.array = calloc(blueprints.number, sizeof(Turret*));
	// Read all weapons file exept the fist one (its the _weapon_readme file)
	for (int i = 0; i < directory.nbrFiles; i++)
	{
		char filepath[BUFFER_LARGE] = TURRETS_DIR;
		strcat(filepath, "/");
		strcat(filepath, directory.fileNames[i]);
		FILE *file = fopen(filepath, "rt");
		Turret *turret = calloc(1, sizeof(Turret));
		if (file && turret)
		{
			char buffer[BUFFER_MEDIUM] = { 0 };

			while (!feof(file))
			{
				char keyWord[BUFFER_SMALL] = { 0 };
				sfBool didntFoundKeyWord = sfTrue;
				fscanf(file, "%s", keyWord);

				// Initialize the name
				if (strcmp(keyWord, "NAME") == 0)
				{
					fscanf(file, "%s\n", &turret->name);
					didntFoundKeyWord = sfFalse;
				}

				// Initialize the behaviour
				if (strcmp(keyWord, "BEHAVIOUR") == 0)
				{
					fscanf(file, "%d\n", &turret->behaviour);
					didntFoundKeyWord = sfFalse;
				}

				// Initialize the turn rate
				if (strcmp(keyWord, "TURN_RATE") == 0)
				{
					fscanf(file, "%f\n", &turret->turnRate);
					didntFoundKeyWord = sfFalse;
				}

				// Initialize the damage type
				if (strcmp(keyWord, "DAMAGE_TYPE") == 0)
				{
					fscanf(file, "%d\n", &turret->type);
					didntFoundKeyWord = sfFalse;
				}

				// Initialize the weapon
				if (strcmp(keyWord, "WEAPON_NAME") == 0)
				{
					fscanf(file, "%s\n", turret->weaponName);
					didntFoundKeyWord = sfFalse;
				}

				// Initialize the offset of the weapon
				if (strcmp(keyWord, "WEAPON_OFFSET") == 0)
				{
					fscanf(file, "%f %f\n", &turret->weaponOffset.x, &turret->weaponOffset.y);
					didntFoundKeyWord = sfFalse;
				}

				// Initialize the maximum health
				if (strcmp(keyWord, "HEALTH") == 0)
				{
					fscanf(file, "%f\n", &turret->maximumHealth);
					turret->health = turret->maximumHealth;
					didntFoundKeyWord = sfFalse;
				}

				// Initialize the sprite
				if (strcmp(keyWord, "SPRITE_NAME") == 0)
				{
					fscanf(file, "%s\n", buffer);
					turret->sprite = Sprite_CreateFromFile(buffer, 0.5f, 0.5f);
					didntFoundKeyWord = sfFalse;
				}

				// Initialize the collider
				if (strcmp(keyWord, "COLLIDER") == 0)
				{
					fscanf(file, " TYPE %d", &turret->collider.type);
					Collider_CreateFromFile(&turret->collider, file);
					didntFoundKeyWord = sfFalse;
				}

				// Initialize the reload timer
				if (strcmp(keyWord, "RELOAD_TIME") == 0)
				{
					fscanf(file, "%f\n", &turret->reloadTimer.maxTime);
					turret->reloadTimer.isPlaying = TRUE;
					didntFoundKeyWord = sfFalse;
				}

				// Initialize the firing timer
				if (strcmp(keyWord, "FIRING_TIME") == 0)
				{
					fscanf(file, "%f\n", &turret->firingTimer.maxTime);
					turret->firingTimer.isPlaying = TRUE;
					didntFoundKeyWord = sfFalse;
				}

				// Initialize the default angle if the turret dont find target
				if (strcmp(keyWord, "DEFAULT_ANGLE") == 0)
				{
					float tempAngle;
					fscanf(file, "%f\n", &tempAngle);
					turret->defaultAngle = DEG_TO_RAD(tempAngle);
					didntFoundKeyWord = sfFalse;
				}

				// No key word has been found
				if (didntFoundKeyWord == sfTrue)
				{
					fscanf(file, "%[^\n]\n", keyWord);
				}
			}

			// Add the weapon to the blueprints
			printf("(%d/%d)\t%s\n", i + 1, directory.nbrFiles, turret->name);
			blueprints.array[i] = turret;
			fclose(file);
		}
		else
		{
			printf("Problem while trying to create the turret %s\n", directory.fileNames[i]);
		}
	}
	printf("\n");
	return blueprints;
}

Turret Turret_Create(Engine *_engine, const char *_name, WeaponOwner _ownerType, void *_ownerPointer, sfVector2f _offset)
{
	BP_Turrets *turretsBP = &_engine->blueprints.turrets;
	BP_Weapons *weaponsBP = &_engine->blueprints.weapons;
	Turret *blueprint = NULL;
	Turret turret;
	// Find the blueprint
	for (int i = 0; i < turretsBP->number; i++)
	{
		if (_stricmp(turretsBP->array[i]->name, _name) == 0)
		{
			blueprint = turretsBP->array[i];
			i = turretsBP->number;
		}
	}
	// Create a copy of the blueprint
	if (blueprint)
	{
		turret = *blueprint;
		turret.weapon = GetWeapon(blueprint->weaponName, weaponsBP, _ownerType, _ownerPointer);
		turret.offset = V2F_MUL_V2F(_offset, _engine->gameScale);
		turret.collider = Collider_Copy(&blueprint->collider);
	}
	else
	{
		printf("Failed to find the turret blueprint : %s\n", _name);
	}
	return turret;
}

void Turret_Update(Engine *_engine, Turret *_turret, List *_projectiles, sfVector2f _aimingPosition, sfVector2f _ownerPosition)
{
	float newAngle;
	if (_turret->health > 0.0f)
	{
		// Update position
		_turret->entity.position = V2F_ADD_V2F(_ownerPosition, _turret->offset);
		_turret->weapon.position = _turret->entity.position;
		Collider_UpdatePosition(&_turret->collider, _turret->entity.position);
		// Update angle
		switch (_turret->behaviour)
		{
		default:
			newAngle = _turret->defaultAngle;
			break;
		case TURRET_BEHAVIOUR_CLOSEST:
			newAngle = Angle_RadGetAngle(_turret->entity.position, _aimingPosition);
			break;
		case TURRET_BEHAVIOUR_ROTATE:
			newAngle = _turret->entity.angle + _turret->turnRate * _engine->dt;
			break;
		}
		_turret->entity.angle = newAngle;
		_turret->weapon.angle = _turret->entity.angle;
		// Update feedbacks 
		Timer_Update(&_turret->timerFeedback, _engine->dt);
		// Update shooting
		Weapon_UpdateFiringRate(_engine, &_turret->weapon);
		Timer_Update(&_turret->reloadTimer, _engine->dt);
		if (Timer_Check(&_turret->reloadTimer))
		{
			Timer_Update(&_turret->firingTimer, _engine->dt);
			if (Timer_Check(&_turret->firingTimer))
			{
				Timer_Reset(&_turret->reloadTimer);
				Timer_Reset(&_turret->firingTimer);
			}
			else
			{
				Weapon_ShootProjectile(_engine, _projectiles, &_turret->weapon);
			}
		}
	}
}

void Turret_Display(Engine *_engine, Turret *_turret, sfRenderStates *_renderState)
{
	float angle = RAD_TO_DEG(_turret->entity.angle) - 90.f;
	sfVector2f scale = _engine->gameScale;
	if (_turret->health > 0.0f)
	{
		Sprite_Display(_engine->canvas, _turret->sprite, _turret->entity.position, scale, angle, _renderState);
	}
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //