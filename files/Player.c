#include "Player.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

Player *Player_GetBlueprintFromName(BP_Players *_blueprints, const char *_name);
//void LoadPlayerWeapons(Engine *_engine, Player *_player);
void LoadPlayerWeaponSlot(Engine *_engine, Player *_player, FILE *_file, int _weaponSlot);
void Player_UpdateWeapons(Player* _player, float _dt, Engine* _engine);
sfBool Keyboard_isActive();

// -- SHARED FUNCTIONS ----------------------------------------------------- //

BP_Players Player_LoadBlueprints(Engine *_engine)
{
	printf("Initialization of players blueprints : \n");
	ExploDir directory = ExploreDirectory(PLAYER_SHIPS_DIR);
	BP_Players blueprints;
	blueprints.array = calloc(directory.nbrFiles, sizeof(Enemy*));
	blueprints.number = directory.nbrFiles;
	for (int i = 0; i < blueprints.number; i++)
	{
		blueprints.array[i] = calloc(1, sizeof(Enemy));
		if (blueprints.array[i] != NULL)
		{
			FILE *file = NULL;
			char filepath[BUFFER_LARGE] = PLAYER_SHIPS_DIR;
			strcat(filepath, "/");
			strcat(filepath, directory.fileNames[i]);
			file = fopen(filepath, "rt");
			if (file != NULL)
			{
				Player *player = calloc(1, sizeof(Player));
				if (player)
				{
					while (!feof(file))
					{
						char temp_string[BUFFER_EXTRA_LARGE] = { 0 };
						char keyWord[BUFFER_SMALL] = { 0 };
						sfBool didntFoundKeyWord = sfTrue;
						blueprints.array[i] = player;
						fscanf(file, "%s", keyWord);

						// Initialize various variables
						if (strcmp(keyWord, "NAME") == 0)
						{
							fscanf(file, "%s\n", temp_string);
							player->name = _strdup(temp_string);
							didntFoundKeyWord = sfFalse;
						}

						if (strcmp(keyWord, "ARMOR") == 0)
						{
							fscanf(file, "%f\n", &player->armor);
							didntFoundKeyWord = sfFalse;
						}

						if (strcmp(keyWord, "SHIELD_AMOUNT") == 0)
						{
							fscanf(file, "%d\n", &player->shield);
							didntFoundKeyWord = sfFalse;
						}

						if (strcmp(keyWord, "SHIELD_REGEN_SPEED") == 0)
						{
							fscanf(file, "%f\n", &player->shieldRegenSpeed);
							didntFoundKeyWord = sfFalse;
						}

						if (strcmp(keyWord, "SHIELD_COOLDOWN") == 0)
						{
							fscanf(file, "%f\n", &player->shieldCooldown.maxTime);
							didntFoundKeyWord = sfFalse;
						}

						// Initialize animations
						if (strcmp(keyWord, "ANIMATIONS_IDLE") == 0)
						{
							fscanf(file, "%s\n", temp_string);
							player->idle = GetAnim(_engine->anims, player->name, temp_string);
							player->currentAnim = player->idle;
							didntFoundKeyWord = sfFalse;
						}

						if (strcmp(keyWord, "ANIMATIONS_LEFT") == 0)
						{
							fscanf(file, "%s\n", temp_string);
							player->leftAnim = GetAnim(_engine->anims, player->name, temp_string);
							didntFoundKeyWord = sfFalse;
						}

						if (strcmp(keyWord, "ANIMATIONS_RIGHT") == 0)
						{
							fscanf(file, "%s\n", temp_string);
							player->rightAnim = GetAnim(_engine->anims, player->name, temp_string);
							didntFoundKeyWord = sfFalse;
						}

						// Initialize colliders
						if (strcmp(keyWord, "COLLIDER_COCKPIT") == 0)
						{
							fscanf(file, " TYPE %d", &player->cockpitCollider.type);
							Collider_CreateFromFile(&player->cockpitCollider, file);
							didntFoundKeyWord = sfFalse;
						}

						if (strcmp(keyWord, "COLLIDER_SPACESHIP") == 0)
						{
							fscanf(file, " TYPE %d", &player->spaceshipCollider.type);
							Collider_CreateFromFile(&player->spaceshipCollider, file);
							didntFoundKeyWord = sfFalse;
						}

						if (strcmp(keyWord, "WEAPON_SLOT_0") == 0)
						{
							LoadPlayerWeaponSlot(_engine, player, file, 0);
							didntFoundKeyWord = sfFalse;
						}

						if (strcmp(keyWord, "WEAPON_SLOT_1") == 0)
						{
							LoadPlayerWeaponSlot(_engine, player, file, 1);
							didntFoundKeyWord = sfFalse;
						}

						if (strcmp(keyWord, "WEAPON_SLOT_2") == 0)
						{
							LoadPlayerWeaponSlot(_engine, player, file, 2);
							didntFoundKeyWord = sfFalse;
						}

						if (strcmp(keyWord, "SKILL_TYPE") == 0)
						{
							int tempNbr = 0;
							fscanf(file, "%i\n", &tempNbr);
							player->skillInfo = Skill_Get(tempNbr, &_engine->skills);
							didntFoundKeyWord = sfFalse;
						}

						if (didntFoundKeyWord == sfTrue)
						{
							fscanf(file, "%[^\n]\n", keyWord);
						}
					}
				}
				printf("(%d/%d)\t%s\n", i + 1, directory.nbrFiles, player->name);
				fclose(file);
			}
		}
	}
	printf("\n");
	return blueprints;
}

Player *Player_Create(Engine *_engine, Players *_players, sfVector2f _position, joystickId _joystick, const char *_blueprintName)
{
	Player *player;
	Player *blueprint = Player_GetBlueprintFromName(&_engine->blueprints.players, _blueprintName);
	player = calloc(1, sizeof(Player));
	if (player && blueprint)
	{
		player->name = _strdup(blueprint->name);
		player->entity.position = _position;
		player->internalDisplay = sfTrue;
		player->leftAnim = blueprint->leftAnim;
		player->rightAnim = blueprint->rightAnim;
		player->idle = blueprint->idle;
		player->currentAnim = player->idle;
		player->entity.currentFrame = 0;
		player->entity.animationTimer = Timer_Create(player->currentAnim->maxTime);
		player->joystick = _joystick;
		player->armor = blueprint->armor;
		player->maxArmor = player->armor;
		player->shield = blueprint->shield;
		player->maxShield = player->shield;
		player->shieldCooldown = blueprint->shieldCooldown;
		player->shieldRegenSpeed = blueprint->shieldRegenSpeed;
		player->changeWeapon = 0.2f;
		player->nbSlotsMax = 3;
		player->cockpitCollider = Collider_Copy(&blueprint->cockpitCollider);
		player->spaceshipCollider = Collider_Copy(&blueprint->spaceshipCollider);
		player->entity.size = Sprite_GetSize(player->currentAnim->sprites[player->entity.currentFrame]);
		player->isAlive = sfTrue;
		player->canBeTargeted = sfTrue;
		player->invulnerability.maxTime = PLAYER_INVULNERABILITY_TIME;
		player->invulnerability.isPlaying = sfTrue;
		player->skillInfo = blueprint->skillInfo;
		player->LaserBoosted = Timer_Create(0.2f);
		player->LaserBoosted.isPlaying = sfFalse;
		//Give weapons & slots
		player->fireRateMultiplier = 1.f;
		player->dmgMultiplier = 1.f;
		player->weapons[0] = blueprint->weapons[0];
		player->weapons[1] = blueprint->weapons[1];
		player->weapons[2] = blueprint->weapons[2];
		player->weaponSlots[0] = blueprint->weaponSlots[0];
		player->weaponSlots[1] = blueprint->weaponSlots[1];
		player->weaponSlots[2] = blueprint->weaponSlots[2];
		player->score = 0;
		player->inventory = NULL;
		player->inventorySize = 0;
		player->money = 0;
		for (int i = 0; i < player->nbSlotsMax; i++)
		{
			player->weapons[i].weapon.player = player;
			player->weaponSlots[i].weapon.player = player;
		}
		// Add the player
		_players->array[_players->number] = player;
		_players->number++;
		return player;
	}
	else
	{
		printf("Error creating the player...\n");
		return NULL;
	}
}

void Player_JoystickButtonPressed(joystickId _joystick, joystickButton _button, GameData *_state, GameState _currentState, Engine *_engine)
{
	for (int i = 0; i < _state->players.number; i++)
	{
		Player *player = _state->players.array[i];

		// Stops laser if changing weapon...
		if (player->joystick == _joystick && (_button == XBOX_RB || _button == XBOX_LB))
		{
			Laser_StopShooting(_state->lasers, player, &player->weapons[player->currentFireMode].weapon);
		}
	}
	if (_currentState != SHOP)
	{
		Skill_EventButtonPressed(_joystick, _button, _state, _engine);
	}
}

void Player_JoystickMoved(MainData *_data, joystickId _joystick, sfJoystickAxis _axis, float _position)
{
	GameData *state = _data->stateData[GAME];

	/*for (int i = 0; i < state->players.number; i++)
	{
		Player *player = state->players.array[i];

	}*/
}

void Player_JoystickButtonReleased(joystickId _joystick, joystickButton _button, GameData *_state)
{
	Skill_EventButtonReleased(_joystick, _button, _state);
}

void Player_HandleMovementInput(Players *_players)
{
	for (int i = 0; i < _players->number; i++)
	{
		Player *player = _players->array[i];
		if (player->skillInfo.isDashing == sfFalse)
		{
			if (player->joystick == 4)
			{
				if (sfKeyboard_isKeyPressed(sfKeyZ))
				{
					player->entity.velocity.y = -PLAYER_SPEED;
				}
				if (sfKeyboard_isKeyPressed(sfKeyQ))
				{
					player->entity.velocity.x = -PLAYER_SPEED * PLAYER_X_VELOCITY_MULTIPLIER;
				}
				if (sfKeyboard_isKeyPressed(sfKeyS))
				{
					player->entity.velocity.y = PLAYER_SPEED;
				}
				if (sfKeyboard_isKeyPressed(sfKeyD))
				{
					player->entity.velocity.x = PLAYER_SPEED * PLAYER_X_VELOCITY_MULTIPLIER;
				}
			}
			else
			{
				sfVector2f axisPosition;
				if (player->isAlive)
				{
					// Change x velocity
					// Get the position of the directional pad
					axisPosition.x = axisPosition.x = sfJoystick_getAxisPosition(player->joystick, sfJoystickPovX);
					// If the directional pad have a default position, use the position of the left stick
					if (axisPosition.x == 0.0f)
					{
						axisPosition.x = sfJoystick_getAxisPosition(player->joystick, sfJoystickX);
					}
					// Change x velocity to 0 if the input is lower than the joystick sensitivity
					if (axisPosition.x >= -JOYSTICK_MINIMUM_SENSITIVITY && axisPosition.x <= JOYSTICK_MINIMUM_SENSITIVITY)
					{
						axisPosition.x = 0.0f;
					}
					// Change y velocity
					// Get the position of the directional pad
					axisPosition.y = -sfJoystick_getAxisPosition(player->joystick, sfJoystickPovY);
					// If the directional pad have a default position, use the position of the left stick
					if (axisPosition.y == 0.0f)
					{
						axisPosition.y = sfJoystick_getAxisPosition(player->joystick, sfJoystickY);
					}
					// Change y velocity to 0 if the input is lower than the joystick sensitivity
					if (axisPosition.y >= -JOYSTICK_MINIMUM_SENSITIVITY && axisPosition.y <= JOYSTICK_MINIMUM_SENSITIVITY)
					{
						axisPosition.y = 0.0f;
					}
					// Calculate and set the velocities
					///angle = atan2f(axisPosition.y, axisPosition.x);
					///printf("angle %f\n", angle);
					///player->entity.velocity.x = cos(angle) * PLAYER_SPEED;
					///player->entity.velocity.y = sin(angle) * PLAYER_SPEED;

					player->entity.velocity.x = axisPosition.x / 100.0f * PLAYER_SPEED * PLAYER_X_VELOCITY_MULTIPLIER;
					player->entity.velocity.y = axisPosition.y / 100.0f * PLAYER_SPEED;
				}
			}
		}
	}
}

void Player_HandleShopMovementInput(Players *_players)
{
	for (int i = 0; i < _players->number; i++)
	{
		Player *player = _players->array[i];
		if (player->joystick == 4)
		{
			if (sfKeyboard_isKeyPressed(sfKeyZ))
			{
				player->entity.velocity.y = -PLAYER_SPEED;
			}
			if (sfKeyboard_isKeyPressed(sfKeyQ))
			{
				player->entity.velocity.x = -PLAYER_SPEED * PLAYER_X_VELOCITY_MULTIPLIER;
			}
			if (sfKeyboard_isKeyPressed(sfKeyS))
			{
				player->entity.velocity.y = PLAYER_SPEED;
			}
			if (sfKeyboard_isKeyPressed(sfKeyD))
			{
				player->entity.velocity.x = PLAYER_SPEED * PLAYER_X_VELOCITY_MULTIPLIER;
			}
		}
		else
		{
			sfVector2f axisPosition;
			if (player->isAlive)
			{
				// Change x velocity
				// Get the position of the left stick
				axisPosition.x = sfJoystick_getAxisPosition(player->joystick, sfJoystickX);
				// Change x velocity to 0 if the input is lower than the joystick sensitivity
				if (axisPosition.x >= -JOYSTICK_MINIMUM_SENSITIVITY && axisPosition.x <= JOYSTICK_MINIMUM_SENSITIVITY)
				{
					axisPosition.x = 0.0f;
				}
				// Change y velocity
				// Get the position of the left stick
				axisPosition.y = sfJoystick_getAxisPosition(player->joystick, sfJoystickY);
				// Change y velocity to 0 if the input is lower than the joystick sensitivity
				if (axisPosition.y >= -JOYSTICK_MINIMUM_SENSITIVITY && axisPosition.y <= JOYSTICK_MINIMUM_SENSITIVITY)
				{
					axisPosition.y = 0.0f;
				}
				// Calculate and set the velocities
				///angle = atan2f(axisPosition.y, axisPosition.x);
				///printf("angle %f\n", angle);
				///player->entity.velocity.x = cos(angle) * PLAYER_SPEED;
				///player->entity.velocity.y = sin(angle) * PLAYER_SPEED;

				player->entity.velocity.x = axisPosition.x / 100.0f * PLAYER_SPEED * PLAYER_X_VELOCITY_MULTIPLIER;
				player->entity.velocity.y = axisPosition.y / 100.0f * PLAYER_SPEED;
			}
		}
	}
}

void HandleShootInput(Engine *_engine, Players *_players, List *_projectiles, Lasers *_lasers)
{
	for (int i = 0; i < _players->number; i++)
	{
		Player *player = _players->array[i];
		if (player->isAlive)
		{
			// Change to the next weapon
			if (sfJoystick_isButtonPressed(player->joystick, XBOX_RB) && player->changeWeapon <= 0.0f || sfKeyboard_isKeyPressed(sfKeyRShift) && player->changeWeapon <= 0.0f && player->joystick == 4)
			{
				Laser_StopShooting(_lasers, player, &player->weapons[player->currentFireMode].weapon);

				player->currentFireMode++;
				if (player->currentFireMode >= 3)
				{
					player->currentFireMode = 0;
				}
				player->changeWeapon = 0.2f;
			}
			if (sfJoystick_isButtonPressed(player->joystick, XBOX_LB) && player->changeWeapon <= 0.0f || sfKeyboard_isKeyPressed(sfKeyLShift) && player->changeWeapon <= 0.0f && player->joystick == 4)
			{
				Laser_StopShooting(_lasers, player, &player->weapons[player->currentFireMode].weapon);

				player->currentFireMode--;
				if (player->currentFireMode < 0)
				{
					player->currentFireMode = 2;
				}
				player->changeWeapon = 0.2f;
			}
			if (sfJoystick_isButtonPressed(player->joystick, XBOX_X) && player->changeWeapon <= 0.0f || sfKeyboard_isKeyPressed(sfKeyNumpad1) && player->changeWeapon <= 0.0f && player->joystick == 4)
			{
				Laser_StopShooting(_lasers, player, &player->weapons[player->currentFireMode].weapon);

				player->currentFireMode = 0;
				player->changeWeapon = 0.2f;
			}
			if (sfJoystick_isButtonPressed(player->joystick, XBOX_Y) && player->changeWeapon <= 0.0f || sfKeyboard_isKeyPressed(sfKeyNumpad2) && player->changeWeapon <= 0.0f && player->joystick == 4)
			{
				Laser_StopShooting(_lasers, player, &player->weapons[player->currentFireMode].weapon);

				player->currentFireMode = 1;
				player->changeWeapon = 0.2f;
			}
			if (sfJoystick_isButtonPressed(player->joystick, XBOX_B) && player->changeWeapon <= 0.0f || sfKeyboard_isKeyPressed(sfKeyNumpad3) && player->changeWeapon <= 0.0f && player->joystick == 4)
			{
				Laser_StopShooting(_lasers, player, &player->weapons[player->currentFireMode].weapon);

				player->currentFireMode = 2;
				player->changeWeapon = 0.2f;
			}
			// Can shoot if he's not teleporting
			if (player->skillInfo.hasDisappeared == sfFalse)
			{
				// Fire with the current fire
				if ((player->joystick != 4 && sfJoystick_getAxisPosition(player->joystick, sfJoystickZ) <= -JOYSTICK_MINIMUM_SENSITIVITY) || (player->joystick == 4 && sfKeyboard_isKeyPressed(sfKeySpace)))
				{
					// Fire with the shield
					if (player->weapons[player->currentFireMode].weapon.behaviour == WEAPON_SHIELD)
					{
						if (player->weapons[player->currentFireMode].weapon.canBeActivated)
						{
							DirectionalShield_ActivationOperation(&player->weapons[player->currentFireMode].weapon, player->joystick, _engine);
						}
						else
						{
							//// Feedback that the shield cant be activated
							//Sound_PlayFromName(_engine->sound, "DirectionalShield_ShieldDisactivate.wav");
						}
					}
					// Fire with the laser
					else if (player->weapons[player->currentFireMode].weapon.behaviour == WEAPON_STRAIGHTED_LASER_ORIENTED)
					{
						if (Laser_IsAlreadyShooting(_lasers, player, &player->weapons[player->currentFireMode].weapon) == sfFalse)
						{
							Laser_Create(_engine, _lasers, &player->weapons[player->currentFireMode].weapon, player);
						}
					}
					// Fire with standard weapons
					else
					{
						if (Timer_Check(&player->weapons[player->currentFireMode].weapon.rate))
						{
							for (int i = 0; i < player->weapons[player->currentFireMode].number; i++)
							{
								sfVector2f offset = player->weapons[player->currentFireMode].offsets[i];
								Weapon_SetPosition(&player->weapons[player->currentFireMode].weapon, player->entity.position, offset);
								Projectile_Create(_engine, _projectiles, &player->weapons[player->currentFireMode].weapon);
							}
							Timer_Reset(&player->weapons[player->currentFireMode].weapon.rate);
							Weapon *weaponUsed = &player->weapons[player->currentFireMode].weapon;
							Sound_PlayFromName(_engine->sound, weaponUsed->soundsShooting[RAND(0, weaponUsed->nbSoundsShooting - 1)]);
						}
					}
				}
				// Not pressing "attack"
				else if (!((player->joystick != 4 && sfJoystick_getAxisPosition(player->joystick, sfJoystickZ) <= -JOYSTICK_MINIMUM_SENSITIVITY) || (player->joystick == 4 && sfKeyboard_isKeyPressed(sfKeySpace))))
				{
					if (player->weapons[player->currentFireMode].weapon.behaviour == WEAPON_SHIELD)
					{
						DirectionalShield_Attack(_engine, _projectiles, &player->weapons[player->currentFireMode].weapon);
						player->weapons[player->currentFireMode].weapon.isActive = FALSE;
					}
					if (!(sfJoystick_getAxisPosition(player->joystick, sfJoystickZ) <= -JOYSTICK_MINIMUM_SENSITIVITY))
					{
						Laser_StopShooting(_lasers, player, &player->weapons[player->currentFireMode].weapon);
					}
				}
			}
		}
	}
}

void Player_ShopTestWeapon(Engine *_engine, Players *_players, List *_projectiles, Lasers *_lasers, Weapon* _testWeapon[MAX_PLAYER_NUMBER])
{
	// Fire with the shield
	for (int i = 0; i < _players->number; i++)
	{
		Player* player = _players->array[i];
		Weapon* testedWeapon = _testWeapon[i];
		Weapon_SetPosition(testedWeapon, player->entity.position, (sfVector2f) { 0, 0 });
		if (testedWeapon->behaviour == WEAPON_SHIELD)
		{
			if (testedWeapon->canBeActivated)
			{
				DirectionalShield_ActivationOperation(testedWeapon, player->joystick, _engine);
			}
			else
			{
				// Feedback that the shield cant be activated
				Sound_PlayFromName(_engine->sound, "DirectionalShield_ShieldDisactivate.wav");
			}
		}
		// Fire with the laser
		else if (testedWeapon->behaviour == WEAPON_STRAIGHTED_LASER_ORIENTED)
		{
			if (Laser_IsAlreadyShooting(_lasers, player, testedWeapon) == sfFalse)
			{
				Laser_Create(_engine, _lasers, testedWeapon, player);
			}
		}
		// Fire with standard weapons
		else
		{
			Timer_Update(&testedWeapon->rate, _engine->dt);
			if (Timer_Check(&testedWeapon->rate))
			{
				Projectile_Create(_engine, _projectiles, testedWeapon);
				Timer_Reset(&testedWeapon->rate);
				Sound_PlayFromName(_engine->sound, "Projectile.wav");
			}
		}
	}
}

void Player_TakeDamage(MainData*_data, Player *_player, Weapon *_weaponUsed)
{
	if (Timer_Check(&_player->invulnerability))
	{
		// Protection of the shield
		if (_player->protectionShield > 0)
		{
			// Play sound for bullet absorbed by shield
			// ...
			_player->protectionShield--;
		}
		else
		{
			if (_weaponUsed)
			{
				float damage = _weaponUsed->damage;
				if (_weaponUsed->behaviour != WEAPON_STRAIGHTED_LASER_ORIENTED)
				{
					Timer_Reset(&_player->invulnerability);
					Sound_PlayFromName(_data->engine.sound, "HitMarker.wav");
				}
				else
				{
					damage *= _data->engine.dt;
				}
				_player->armor -= damage;
				}
			else
			{
				_player->armor -= PLAYER_DEFAULT_DAMAGE;
			}
			_data->achievementsUnlockVar.doPlayerTookDamage = sfTrue;
			}
		}
	// Kill the player
	if (_player->armor <= 0)
	{
		_player->armor = 0;
#ifndef DEBUG
		_player->isAlive = sfFalse;
		_player->canBeTargeted = sfFalse;
#endif // !DEBUG
	}
	}

void Player_Heal(Engine *_engine, Player *_player, float _quantity)
{
	//Sound_PlayFromName(_engine->sound, "TakingDamage.wav");
	_player->armor += _quantity;
	if (_player->armor >= _player->maxArmor)
	{
		_player->armor = _player->maxArmor;
	}
}

void Player_UpdateDamageAndFireRate(Player *_player)
{
	Weapon *playerWeapon = &_player->weapons[_player->currentFireMode].weapon;

	// Update damage
	playerWeapon->damage = playerWeapon->baseDamage * _player->dmgMultiplier;
	// Update fire rate
	playerWeapon->rate.maxTime = playerWeapon->baseFireRateMaxTime * _player->fireRateMultiplier;
}

void Player_Update(Engine *_engine, Players *_players, float _dt, Lasers *_lasers)
{
	for (int i = 0; i < _players->number; i++)
	{
		Player *player = _players->array[i];
		if (player->isAlive)
		{

			UpdateAnimation(_dt, player->currentAnim, &player->entity);

			Entity_UpdatePosition(&player->entity, _dt);
			// Get collisions with screen borders
			if (player->entity.position.x < 0.0f)
			{
				player->entity.position.x = 0.0f;
				player->entity.velocity.x = 0.0f;
			}
			else if (player->entity.position.x > _engine->configuration->resolution.x)
			{
				player->entity.position.x = (float)_engine->configuration->resolution.x;
				player->entity.velocity.x = 0.0f;
			}
			// Get collisions with screen borders
			if (player->entity.position.y < 0.0f)
			{
				player->entity.position.y = 0.0f;
				player->entity.velocity.y = 0.0f;

			}
			else if (player->entity.position.y > _engine->configuration->resolution.y)
			{
				player->entity.position.y = (float)_engine->configuration->resolution.y;
				player->entity.velocity.y = 0.0f;
			}

			// Update Weapon position
			for (int i = 0; i < player->weapons[player->currentFireMode].number; i++)
			{
				Weapon_UpdatePosition(&player->weapons[player->currentFireMode].weapon, player->entity.position, player->weapons[player->currentFireMode].offsets[i]);
			}

			// Update weapon damage
			Player_UpdateDamageAndFireRate(player);

			// Choose which sprite to display
			if (player->entity.velocity.x > PLAYER_SPEED / 3.f)
			{
				if (player->currentAnim != player->rightAnim && player->rightAnim != NULL)
				{
					ResetAnim(&player->entity);
					player->currentAnim = player->rightAnim;
				}
			}
			else if (player->entity.velocity.x < -PLAYER_SPEED / 3.f)
			{
				if (player->currentAnim != player->leftAnim && player->leftAnim != NULL)
				{
					ResetAnim(&player->entity);
					player->currentAnim = player->leftAnim;
				}
			}
			else
			{
				if (player->currentAnim != player->idle && player->idle != NULL)
				{
					ResetAnim(&player->entity);
					player->currentAnim = player->idle;
				}
			}
			// Update weapon
			Player_UpdateWeapons(player, _dt, _engine);

			player->changeWeapon -= _dt;
			// Udpate Colliders
			Collider_UpdatePosition(&player->cockpitCollider, player->entity.position);
			Collider_UpdatePosition(&player->spaceshipCollider, player->entity.position);
			Timer_Update(&player->weapons[player->currentFireMode].weapon.rate, _dt);
			Timer_Update(&player->invulnerability, _dt);

			// Updates, resets and stops the laserboost timer
			Timer_Update(&player->LaserBoosted, _dt);
			if (Timer_Check(&player->LaserBoosted))
			{
				Timer_Reset(&player->LaserBoosted);
				player->LaserBoosted.isPlaying = sfFalse;
				player->fireRateMultiplier += player->laserBoostReceived;
			}

			if (player->skillInfo.isDashing == sfFalse)
			{
				player->entity.velocity.x = 0;
				player->entity.velocity.y = 0;
			}
		}
		else
		{
			for (int i = 0; i < player->nbSlotsMax; i++)
			{
				Laser_StopShooting(_lasers, player, &player->weapons[i].weapon);
			}
		}
	}
}

void Player_DisplayDrawableWeapon(Engine *_engine, Player *_player)
{
	for (int i = 0; i < _player->nbSlotsMax; i++)
	{
		if (_player->weapons)
		{
			sfRenderStates *state = NULL;// Every Weapon that can be drawn
			switch (_player->weapons[i].weapon.behaviour)
			{
			case WEAPON_SHIELD:
				if ((_player->joystick != 4 && sfJoystick_getAxisPosition(_player->joystick, sfJoystickZ) <= -JOYSTICK_MINIMUM_SENSITIVITY) || (_player->joystick == 4 && sfKeyboard_isKeyPressed(sfKeySpace)))
				{
					if (_player->weapons[i].weapon.isActive)
					{
						// Set position to the player
						_player->weapons[i].weapon.entity.position = _player->entity.position;
						// Convert rad to deg
						_player->weapons[i].weapon.entity.angle = RAD_TO_DEG(_player->weapons[i].weapon.entity.angle);
						// Change alpha
						sfSprite_setColor(_player->weapons[i].weapon.weaponAnim->sprites[_player->weapons[i].weapon.entity.currentFrame], (sfColor) { 255, 255, 255, (char)((_player->weapons[i].weapon.life / _player->weapons[i].weapon.lifeMax) * 255) });
						// -- Display
						DisplayAnimation(_engine->canvas, _player->weapons[i].weapon.weaponAnim, &_player->weapons[i].weapon.entity, _engine->gameScale, state);
					}
				}
				break;
			default:
				break;
			}
		}
	}
}

void Player_Display(Engine *_engine, Players *_players)
{
	for (int i = 0; i < _players->number; i++)
	{
		Player *player = _players->array[i];
		sfRenderStates *playerState = NULL;
		if (player->isAlive && player->internalDisplay)
		{
			if (!Timer_Check(&player->invulnerability))
			{
				playerState = Shader_ToWhite();
			}
			else
			{
				playerState = Shader_PlayerSpaceship((int)player->armor, (int)player->maxArmor, player->color, sfFalse, _engine->time);
			}
			DisplayAnimation(_engine->canvas, player->currentAnim, &player->entity, _engine->gameScale, playerState);

			Player_DisplayDrawableWeapon(_engine, player);
#ifdef DEBUG
			Collider_Display(_engine, &player->cockpitCollider);
			Collider_Display(_engine, &player->spaceshipCollider);
#endif // DEBUG
		}
	}
}

void Player_DestroyAll(Players *_players)
{
	for (int i = 0; i < _players->number; i++)
	{
		free(_players->array[i]);
		_players->array[i] = NULL;
	}
	_players->number = 0;
}

void Player_UpdateWeapons(Player* _player, float _dt, Engine* _engine)
{
	// For each weapon
	for (int i = 0; i < _player->nbSlotsMax; i++)
	{
		//// Rate of fire depending of "isDamageBoosted" state
		//if (_player->isDamageBoosted == sfFalse)
		//{
		//	Timer_Update(&_player->weaponSlots[i].weapon.rate, _dt);
		//}
		//else if (_player->isDamageBoosted == sfTrue)
		//{
		//	Timer_Update(&_player->weaponSlots[i].weapon.rate, _dt * 5); // <-- Aya le code du "5" en dur
		//}
		Timer_Update(&_player->weaponSlots[i].weapon.rate, _dt);

		// Update if behaviour meet case
		switch (_player->weapons[i].weapon.behaviour)
		{
		case WEAPON_SHIELD:
			DirectionalShield_Regenlife(&_player->weapons[i].weapon, _dt, _engine);
			break;
		default:
			break;
		}
	}
}
// -- LOCAL FUNCTIONS ------------------------------------------------------ //

Player *Player_GetBlueprintFromName(BP_Players *_blueprints, const char *_name)
{
	for (int i = 0; i < _blueprints->number; i++)
	{
		if (_stricmp(_name, _blueprints->array[i]->name) == 0)
		{
			return _blueprints->array[i];
		}
	}
	return NULL;
}

void LoadPlayerWeaponSlot(Engine *_engine, Player *_player, FILE *_file, int _weaponSlot)
{
	char temp_string[BUFFER_EXTRA_LARGE] = { 0 };

	fscanf(_file, " WEAPON_NAME %s", temp_string);
	_player->weapons[_weaponSlot].weapon = GetWeapon(temp_string, &_engine->blueprints.weapons, WEAPON_OWNER_PLAYER, _player);

	fscanf(_file, " NB_TURRETS %d", &_player->weapons[_weaponSlot].number);
	_player->weapons[_weaponSlot].offsets = calloc(_player->weapons[_weaponSlot].number, sizeof(sfVector2f));

	for (int i = 0; i < _player->weapons[_weaponSlot].number; i++)
	{
		sfVector2f posTurret;
		fscanf(_file, " POSITION %f %f", &posTurret.x, &posTurret.y);
		posTurret.x *= SCALE_SPRITE;
		posTurret.y *= SCALE_SPRITE;
		_player->weapons[_weaponSlot].offsets[i] = posTurret;
	}
	fscanf(_file, "%[^\n]\n", temp_string);
}

sfBool Keyboard_isActive()
{
	if (sfKeyboard_isKeyPressed(sfKeyZ))
	{
		return sfTrue;
	}
	else if (sfKeyboard_isKeyPressed(sfKeyQ))
	{
		return sfTrue;
	}
	else if (sfKeyboard_isKeyPressed(sfKeyS))
	{
		return sfTrue;
	}
	else if (sfKeyboard_isKeyPressed(sfKeyD))
	{
		return sfTrue;
	}
	else
	{
		return sfFalse;
	}
}