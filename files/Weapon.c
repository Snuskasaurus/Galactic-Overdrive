#include "Weapon.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

Weapon *Weapon_FindBlueprint(BP_Weapons *_blueprints, const char *_name);
Weapon Weapon_CreateFromBlueprint(Weapon *_blueprint, WeaponOwner _owner, void *_pointer);

// -- SHARED FUNCTIONS ----------------------------------------------------- //

BP_Weapons Weapon_LoadBlueprints(Animations *_anims)
{
	printf("Initialization of weapons blueprints : \n");
	BP_Weapons blueprints;
	ExploDir directory = ExploreDirectory(WEAPONS_DIR);
	// Allocate the array
	blueprints.number = directory.nbrFiles;
	blueprints.array = calloc(blueprints.number, sizeof(Weapon*));
	// Read all weapons file exept the fist one (its the _weapon_readme file)
	for (int i = 0; i < directory.nbrFiles; i++)
	{
		char filepath[BUFFER_LARGE] = WEAPONS_DIR;
		strcat(filepath, "/");
		strcat(filepath, directory.fileNames[i]);
		FILE *file = fopen(filepath, "rt");
		Weapon *weapon = calloc(1, sizeof(Weapon));
		weapon->nbUpgardes = 0;
		if (file && weapon)
		{
			char iconSpritePath[BUFFER_MEDIUM] = { 0 };
			char projectileSpritePath[BUFFER_MEDIUM] = { 0 };
			char animsName[BUFFER_MEDIUM] = { 0 };
			char animStateProj[BUFFER_MEDIUM] = "NONE";
			char animStateBody[BUFFER_MEDIUM] = "NONE";
			char animStateBase[BUFFER_MEDIUM] = "NONE";
			char animWeapon[BUFFER_MEDIUM] = "NONE";
			float angle = 0;
			float dispersionShotAngle = 0;
			float precisionAngle = 0;

			while (!feof(file))
			{
				char keyWord[BUFFER_SMALL] = { 0 };
				sfBool didntFoundKeyWord = sfTrue;
				fscanf(file, "%s", keyWord);

				// Read the file
				if (strcmp(keyWord, "NAME") == 0)
				{
					fscanf(file, "%s\n", &weapon->name);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "SPRITE_ICON") == 0)
				{
					fscanf(file, "%s\n", &iconSpritePath);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "SPRITE_PROJECTILE") == 0)
				{
					fscanf(file, "%s\n", &projectileSpritePath);
					didntFoundKeyWord = sfFalse;
					weapon->spriteProjectile = Sprite_CreateFromFile(projectileSpritePath, 0.5f, 0.5f);
				}

				if (strcmp(keyWord, "ANIMATIONS_NAME") == 0)
				{
					fscanf(file, "%s\n", &animsName);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "ANIMATION_PROJ") == 0)
				{
					fscanf(file, "%s\n", &animStateProj);
					weapon->animProj = GetAnim(_anims, animsName, animStateProj);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "ANIMATION_BODY") == 0)
				{
					fscanf(file, "%s\n", &animStateBody);
					weapon->animBody = GetAnim(_anims, animsName, animStateBody);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "ANIMATION_BASE") == 0)
				{
					fscanf(file, "%s\n", &animStateBase);
					weapon->animBase = GetAnim(_anims, animsName, animStateBase);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "ANIMATION_LOOP") == 0)
				{
					fscanf(file, "%s\n", &animWeapon);
					weapon->weaponAnim = GetAnim(_anims, animsName, animWeapon);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "TYPE") == 0)
				{
					fscanf(file, "%i\n", &weapon->type);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "BEHAVIOUR") == 0)
				{
					fscanf(file, "%i\n", &weapon->behaviour);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "DAMAGE") == 0)
				{
					fscanf(file, "%f\n", &weapon->damage);
					weapon->baseDamage = weapon->damage;
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "RATE") == 0)
				{
					float rate = 0.0f;
					fscanf(file, "%f\n", &rate);
					weapon->rate = Timer_Create(rate);
					weapon->baseFireRateMaxTime = weapon->rate.maxTime;
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "SOUNDS_SHOOTING") == 0)
				{
					fscanf(file, "%d", &weapon->nbSoundsShooting);
					weapon->soundsShooting = calloc(weapon->nbSoundsShooting, sizeof(char*));
					for (size_t i = 0; i < weapon->nbSoundsShooting; i++)
					{
						char buffer[BUFFER_SMALL];
						fscanf(file, " %s\n", buffer);
						weapon->soundsShooting[i] = _strdup(buffer);
					}
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "SHOOTS") == 0)
				{
					fscanf(file, "%i\n", &weapon->nbShot);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "DISPERSION_ANGLE") == 0)
				{
					fscanf(file, "%f\n", &dispersionShotAngle);
					weapon->dispersionShotAngle = DEG_TO_RAD(dispersionShotAngle);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "SHOT_OFFSET") == 0)
				{
					fscanf(file, "%f\n", &weapon->shotOffset);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "ANGLE") == 0)
				{
					fscanf(file, "%f\n", &angle);
					weapon->defaultAngle = DEG_TO_RAD(angle);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "PRECISION") == 0)
				{
					fscanf(file, "%f\n", &weapon->precision);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "PRECISION_ANGLE") == 0)
				{
					fscanf(file, "%f\n", &precisionAngle);
					weapon->precisionAngle = DEG_TO_RAD(precisionAngle);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "SPEED") == 0)
				{
					fscanf(file, "%f\n", &weapon->speed);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "AMPLITUDE") == 0)
				{
					fscanf(file, "%f\n", &weapon->amplitude);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "PERIOD") == 0)
				{
					fscanf(file, "%f\n", &weapon->period);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "RANGE") == 0)
				{
					fscanf(file, "%f\n", &weapon->range);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "INVERTED_DIRECTION") == 0)
				{
					fscanf(file, "%d\n", &weapon->isInvertedCurve);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "AIMING_TYPE") == 0)
				{
					fscanf(file, "%d\n", &weapon->aimType);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "AIMING_ROTATION_SPEED") == 0)
				{
					fscanf(file, "%f\n", &weapon->aimingRotationSpeed);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "ROTATION_SPEED") == 0)
				{
					fscanf(file, "%f\n", &weapon->rotationSpeed);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "FUEL_DURATION") == 0)
				{
					fscanf(file, "%f\n", &weapon->fuel);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "SHIELD_ANGLE_COVERED") == 0)
				{
					fscanf(file, "%d\n", &weapon->shieldAngleCovered);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "SHIELD_LIFE_MAX") == 0)
				{
					fscanf(file, "%f\n", &weapon->lifeMax);
					weapon->life = weapon->lifeMax;
					printf("Life = %f\n", weapon->life);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "SHIELD_REGEN_RATE") == 0)
				{
					fscanf(file, "%f\n", &weapon->lifeRegenPerSecond);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "SHIELD_ATTACK_CHARGE_MAX") == 0)
				{
					fscanf(file, "%d\n", &weapon->attackChargesMax);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "SHIELD_ATTACK_ANGLE") == 0)
				{
					fscanf(file, "%f\n", &weapon->attackRadius);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "UPGRADE") == 0)
				{
					fscanf(file, "%s\n", weapon->upgrades[weapon->nbUpgardes]);
					weapon->nbUpgardes++;
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "TIER") == 0)
				{
					int tier;
					fscanf(file, "%d\n", &tier);
					weapon->upgradeTier = (eWeaponTier)tier;
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "PRICE") == 0)
				{
					fscanf(file, "%d\n", &weapon->price);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "VISIBLE_IN_SHOP") == 0)
				{
					fscanf(file, "%d\n", &weapon->isVisibleInShop);
					didntFoundKeyWord = sfFalse;
				}

				if (didntFoundKeyWord == sfTrue)
				{
					fscanf(file, "%[^\n]\n", keyWord);
				}
			}
			// Load the sprites & animations
			weapon->spriteW = Sprite_CreateFromFile(iconSpritePath, 0.5f, 0.5f);

			// Add the weapon to the blueprints
			printf("(%d/%d)\t%s\n", i + 1, directory.nbrFiles, weapon->name);
			blueprints.array[i] = weapon;
			fclose(file);
		}
		else
		{
			printf("Problem while trying to create the weapon %s\n", directory.fileNames[i]);
		}
	}
	printf("\n");
	return blueprints;
}

Weapon GetWeapon(const char _weaponName[], BP_Weapons *_blueprints, WeaponOwner _owner, void *_pointer)
{
	Weapon *weaponBlueprint = Weapon_FindBlueprint(_blueprints, _weaponName);
	Weapon weaponToReturn;
	if (!weaponBlueprint)
	{
		printf("Impossible to find the blueprint of the weapon : %s\n", _weaponName);
	}
	else
	{
		weaponToReturn = Weapon_CreateFromBlueprint(weaponBlueprint, _owner, _pointer);
	}
	return weaponToReturn;
}

Weapon* Weapon_GetPointer(const char _weaponName[], BP_Weapons *_blueprints)
{
	return Weapon_FindBlueprint(_blueprints, _weaponName);
}

void Weapon_UpdatePosition(Weapon *_weapon, sfVector2f _entityPosition, sfVector2f _offset)
{
	_weapon->position.x = _entityPosition.x + _offset.x;
	_weapon->position.y = _entityPosition.y + _offset.y;
}

void Weapon_SetPosition(Weapon *_weapon, sfVector2f _entityPosition, sfVector2f _offset)
{
	Weapon_UpdatePosition(_weapon, _entityPosition, _offset);
	// Set the symetry and the real value of the weapon angle
	if (_weapon->aimType == WEAPON_AIMING_STATIC)
	{
		if (_offset.x < 0.0f && _weapon->defaultAngle != 0.0f)
		{
			_weapon->angle = -_weapon->defaultAngle - PI / 2.0f;
		}
		else
		{
			_weapon->angle = _weapon->defaultAngle - PI / 2.0f;
		}
	}
	if (_weapon->isInvertedCurve)
	{
		_weapon->curveDirection = (float)GET_SIGN((_weapon->position.x - _entityPosition.x));
	}
	else
	{
		_weapon->curveDirection = (float)GET_SIGN((_entityPosition.x - _weapon->position.x));
	}
}

void Weapon_UpdateFiringRate(Engine *_engine, Weapon *_weapon)
{
	if (_weapon->rate.currentTime > 0.0f)
	{
		Timer_Update(&_weapon->rate, _engine->dt);
		if (Timer_Check(&_weapon->rate))
		{
			Timer_Reset(&_weapon->rate);
		}
	}
}

void Weapon_Update(Engine *_engine, Weapon *_weapon, List *_projectiles, Entity *_owner, Entity *_target)
{
	// Update firerate
	if (_weapon->rate.currentTime > 0.0f)
	{
		Timer_Update(&_weapon->rate, _engine->dt);
		if (Timer_Check(&_weapon->rate))
		{
			Timer_Reset(&_weapon->rate);
		}
	}
	// Update angle
	if (_weapon->aimType == WEAPON_AIMING_ROTATION)
	{
		_weapon->angle += _weapon->aimingRotationSpeed * _engine->dt;
	}
	else if (_weapon->aimType == WEAPON_AIMING_PLAYER && _target != NULL)
	{
		_weapon->angle = Angle_RadGetAngle(_owner->position, _target->position);
	}
	else
	{
		_weapon->angle = _owner->angle;
	}
	///printf("angle : %f\n", _weapon->angle);
	// Update position
	_weapon->entity.position = _owner->position;
	_weapon->position = _owner->position;
	// Shoot
	// Check that the owner is not an enemy below the last quarter of the screen height
	if ((_weapon->owner == WEAPON_OWNER_ENEMY && _owner->position.y > _engine->configuration->resolution.y * 0.75f) == 0)
	{
		Weapon_ShootProjectile(_engine, _projectiles, _weapon);
	}
}

void Weapon_ShootProjectile(Engine *_engine, List *_projectiles, Weapon *_weapon)
{
	int idSound = 0;
	if (_weapon->rate.currentTime == 0.0f)
	{
		// Play a random sound
		if (_weapon->nbSoundsShooting >= 1)
		{
			idSound = RAND(0, _weapon->nbSoundsShooting - 1);
			Sound_PlayFromName(_engine->sound, _weapon->soundsShooting[idSound]);
		}
		#ifdef DEBUG
		else
		{
			printf("Sound is missing from the weapon : %s\n", _weapon->name);
		}
		#endif // DEBUG
		// Create the projectile
		Projectile_Create(_engine, _projectiles, _weapon);
		_weapon->rate.currentTime += 0.00001f;
	}
}

void Weapon_UpdateOrientation(Weapon *_weapon, joystickId _joystick)
{
	sfVector2f axisPosition;
	sfVector2f trigoCircle;

	// X
	// Get the position of the directional pad
	axisPosition.x = sfJoystick_getAxisPosition(_joystick, sfJoystickV);
	// If the directional pad have a default position, use the position of the left stick
	if (axisPosition.x == 0.0f)
	{
		axisPosition.x = sfJoystick_getAxisPosition(_joystick, sfJoystickV);
	}
	// Change x position to 0 if the input is lower than the joystick sensitivity
	if (axisPosition.x >= -JOYSTICK_MINIMUM_SENSITIVITY && axisPosition.x <= JOYSTICK_MINIMUM_SENSITIVITY)
	{
		axisPosition.x = 0.0f;
	}
	// Y
	// Get the position of the directional pad
	axisPosition.y = -sfJoystick_getAxisPosition(_joystick, sfJoystickU);
	// If the directional pad have a default position, use the position of the left stick
	if (axisPosition.y == 0.0f)
	{
		axisPosition.y = sfJoystick_getAxisPosition(_joystick, sfJoystickU);
	}
	// Change y position to 0 if the input is lower than the joystick sensitivity
	if (axisPosition.y >= -JOYSTICK_MINIMUM_SENSITIVITY && axisPosition.y <= JOYSTICK_MINIMUM_SENSITIVITY)
	{
		axisPosition.y = 0.0f;
	}

	// Calculate and set trigonometric circle
	trigoCircle.x = axisPosition.x / 100.0f;
	trigoCircle.y = axisPosition.y / 100.0f;

	// Set weapon angle
	if (axisPosition.x == 0 && axisPosition.y == 0)
	{
		_weapon->entity.angle = Angle_DegToRad(-180);
	}
	else
	{
		_weapon->entity.angle = atan2f(trigoCircle.y, trigoCircle.x);
	}
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //

Weapon *Weapon_FindBlueprint(BP_Weapons *_blueprints, const char *_name)
{
	Weapon *blueprint = NULL;
	for (int i = 0; i < _blueprints->number; i++)
	{
		if (_stricmp(_name, _blueprints->array[i]->name) == 0)
		{
			return _blueprints->array[i];
		}
	}
	return blueprint;
}

Weapon Weapon_CreateFromBlueprint(Weapon *_blueprint, WeaponOwner _owner, void *_pointer)
{
	Weapon weapon = *_blueprint;
	weapon.Next = NULL;
	weapon.player = _pointer;
	weapon.owner = _owner;
	weapon.spriteProjectile = _blueprint->spriteProjectile;
	return weapon;
}