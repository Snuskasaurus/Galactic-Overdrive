#include "Skill.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

void RegenUpdate(Player *_player, float _dt, Players *_players);
void RegenDisplay(Engine *_engine, Player *_player, Players *_players);

void DmgBoostUpdate(Player *_player, float _dt, Players *_players);
void DmgBoostDisplay(Engine *_engine, Players *_players, Player *_playerUsingSkill);

void EnemyClearEventButtonPressed(Enemies* _enemies, joystickButton _button, Player* _player);
void EnemyClearUpdate(Player *_player, float _dt);

void ClearProjEventButtonPressed(Player *_player, joystickButton _button, List *_enemyProjList);
void ClearProjUpdate(Player *_player, float _dt);

void ProtectionShieldButtonPressed(Player *_player, Players *_players, joystickButton _button);
void ProtectionShieldUpdate(Player *_player, float _dt, Players *_players);
void ProtectionShieldDisplay(Engine *_engine, Skill *_skill, Player* _player, Players* _players);

void RezEventButtonPressed(Player *_player, joystickButton _button, Players *_players);
void RezUpdate(Player *_player, float _dt);
void RezDisplay(Engine *_engine, Skill *_skill, Players *_players);

void TeleportationEventButtonPressed(Player *_player, joystickButton _button, Lasers *_lasers);
void TeleportationEventButtonReleased(Player *_player, joystickButton _button, Enemies* _enemies);
void TeleportationUpdate(Player *_player, float _dt, Enemies* _enemies);
void TeleportationDisplay(Engine *_engine, Player *_player);

void DashEventButtonPressed(Player *_player);
void DashUpdate(Player *_player, float _dt);

// -- SHARED FUNCTIONS ----------------------------------------------------- //

Skills Skill_Load(Engine *_engine)
{
	Skills skills;
	ExploDir directory = ExploreDirectory(SKILL_DIR);
	skills.array = calloc(directory.nbrFiles, sizeof(Skill*));
	skills.nbr = directory.nbrFiles;

	if (skills.array != NULL)
	{
		for (int i = 0; i < skills.nbr; i++)
		{
			FILE *skillFile = NULL;
			char filepath[BUFFER_LARGE] = SKILL_DIR;
			strcat(filepath, "/");
			strcat(filepath, directory.fileNames[i]);
			skillFile = fopen(filepath, "rt");
			skills.array[i] = calloc(1, sizeof(Skill));

			if (skillFile != NULL && skills.array[i] != NULL)
			{
				Skill *skill = skills.array[i];
				skill->autoDisplay = sfTrue;

				while (!feof(skillFile))
				{
					char keyWord[BUFFER_SMALL] = { 0 };
					char tempString[BUFFER_SMALL] = { 0 };
					sfBool didntFoundKeyWord = sfTrue;
					fscanf(skillFile, "%s", keyWord);

					if (strcmp(keyWord, "NAME") == 0)
					{
						fscanf(skillFile, " %[^\n]\n", skill->name);
						didntFoundKeyWord = sfFalse;
					}

					if (strcmp(keyWord, "TYPE") == 0)
					{
						fscanf(skillFile, "%i\n", &skill->type);
						didntFoundKeyWord = sfFalse;
					}

					if (strcmp(keyWord, "PATH_ICON") == 0)
					{
						fscanf(skillFile, "%s\n", tempString);
						skill->icon = Sprite_CreateFromFile(tempString, 0.5f, 0.5f);
						didntFoundKeyWord = sfFalse;
					}

					if (strcmp(keyWord, "SCORE_TO_UNLOCK") == 0)
					{
						fscanf(skillFile, "%ld\n", &skill->scoreToUnlock);
						didntFoundKeyWord = sfFalse;
					}

					if (strcmp(keyWord, "STORAGE_CAP") == 0)
					{
						fscanf(skillFile, "%d\n", &skill->skillStorageCap);
						didntFoundKeyWord = sfFalse;
					}

					if (strcmp(keyWord, "BASE_NB_SKILLS_IN_STORAGE") == 0)
					{
						fscanf(skillFile, "%d\n", &skill->nbSkillInStorage);
						didntFoundKeyWord = sfFalse;
					}

					if (strcmp(keyWord, "DURATION") == 0)
					{
						fscanf(skillFile, "%f\n", &skill->duration.maxTime);
						skill->duration = Timer_Create(skill->duration.maxTime);
						didntFoundKeyWord = sfFalse;
					}

					if (strcmp(keyWord, "ANIMATION") == 0)
					{
						fscanf(skillFile, "%s\n", tempString);
						skill->anim = GetAnim(_engine->anims, SKILL_ANIM_CLASS, tempString);
						InitEntityAnim(&skill->entity, skill->anim->maxTime);
						didntFoundKeyWord = sfFalse;
					}

					if (strcmp(keyWord, "REGEN_SPEED") == 0)
					{
						fscanf(skillFile, "%f\n", &skill->regenSpeed);
						didntFoundKeyWord = sfFalse;
					}

					if (strcmp(keyWord, "BOOST_PER_CENT_DMG") == 0)
					{
						fscanf(skillFile, "%f\n", &skill->boostPerCent_Dmg);
						didntFoundKeyWord = sfFalse;
					}

					if (strcmp(keyWord, "BOOST_PER_CENT_FR") == 0)
					{
						fscanf(skillFile, "%f\n", &skill->boostPerCent_FireRate);
						didntFoundKeyWord = sfFalse;
					}

					if (strcmp(keyWord, "RADIUS") == 0)
					{
						fscanf(skillFile, "%f\n", &skill->effectRadius);
						skill->effectRadius *= _engine->gameScale.x;
						didntFoundKeyWord = sfFalse;
					}

					if (strcmp(keyWord, "SHIELD_PROTECTION_QUANTITY") == 0)
					{
						fscanf(skillFile, "%d\n", &skill->nbProtectionShield);
						didntFoundKeyWord = sfFalse;
					}

					if (strcmp(keyWord, "ANIMATION_REZ") == 0)
					{
						fscanf(skillFile, "%s\n", tempString);
						skill->animRez = GetAnim(_engine->anims, SKILL_ANIM_CLASS, tempString);
						InitEntityAnim(&skill->entityRez, skill->animRez->maxTime);
						didntFoundKeyWord = sfFalse;
					}

					if (strcmp(keyWord, "ANIMATION_DISAPPEAR") == 0)
					{
						fscanf(skillFile, "%s\n", tempString);
						skill->animDisappear = GetAnim(_engine->anims, SKILL_ANIM_CLASS, tempString);
						InitEntityAnim(&skill->entityDisappearTP, skill->animDisappear->maxTime);
						skill->entityDisappearTP.animationTimer.isPlaying = sfFalse;
						didntFoundKeyWord = sfFalse;
					}

					if (strcmp(keyWord, "ANIMATION_BLAST") == 0)
					{
						fscanf(skillFile, "%s\n", tempString);
						skill->animBlast = GetAnim(_engine->anims, SKILL_ANIM_CLASS, tempString);
						InitEntityAnim(&skill->entityBlastTP, skill->animBlast->maxTime);
						skill->entityBlastTP.animationTimer.isPlaying = sfFalse;
						didntFoundKeyWord = sfFalse;
					}

					if (strcmp(keyWord, "RANGE") == 0)
					{
						fscanf(skillFile, "%d\n", &skill->dashRange);
						didntFoundKeyWord = sfFalse;
					}

					if (strcmp(keyWord, "BASE_SOUND") == 0)
					{
						fscanf(skillFile, "%s\n", skill->soundName);
						didntFoundKeyWord = sfFalse;
					}

					// Keep last
					if (didntFoundKeyWord == sfTrue)
					{
						fscanf(skillFile, "%[^\n]\n", keyWord);
					}
				}
				fclose(skillFile);
			}
			else
			{
				perror("Skills loading failure...");
			}
		}
	}

	return skills;
}

Skill Skill_Get(eSkill _skillType, Skills *_skills)
{
	Skill skill = { 0 };

	for (int i = 0; i < _skills->nbr; i++)
	{
		if (_skills->array[i]->type == _skillType)
		{
			return *(_skills->array[i]);
		}
	}

	return skill;
}

void Skill_EventButtonPressed(joystickId _joystick, joystickButton _button, GameData *_state, Engine *_engine)
{
	for (int i = 0; i < _state->players.number; i++)
	{
		Player *player = _state->players.array[i];

		if (player->joystick == _joystick)
		{
			if ((_button == SKILL_BUTTON) && (player->skillInfo.nbSkillInStorage > 0) && (player->skillInfo.isActive == sfFalse))
			{
				player->skillInfo.isActive = sfTrue;
				player->skillInfo.nbSkillInStorage--;
				if (player->skillInfo.soundName[0] != 0)
				{
					Sound_PlayFromName(_engine->sound, player->skillInfo.soundName);
				}

				switch (player->skillInfo.type)
				{
				case ENEMY_CLEAR:
				{
					EnemyClearEventButtonPressed(_state->enemies, _button, player);
					break;
				}
				case CLEAR_PROJ:
				{
					ClearProjEventButtonPressed(player, _button, &_state->projectiles[WEAPON_OWNER_ENEMY]);
					break;
				}
				case PROTECTION_SHIELD:
				{
					ProtectionShieldButtonPressed(player, &_state->players, _button);
					break;
				}
				case REZ:
				{
					RezEventButtonPressed(player, _button, &_state->players);
					break;
				}
				case TP:
				{
					TeleportationEventButtonPressed(player, _button, _state->lasers);
					break;
				}
				case DASH:
				{
					DashEventButtonPressed(player);
					break;
				}
				default:
					break;
				}
			}
		}
	}
}

void Skill_EventButtonReleased(joystickId _joystick, joystickButton _button, GameData *_state)
{
	for (int i = 0; i < _state->players.number; i++)
	{
		Player *player = _state->players.array[i];

		if (player->joystick == _joystick)
		{
			if ((_button == SKILL_BUTTON) && (player->skillInfo.isActive == sfTrue))
			{
				switch (player->skillInfo.type)
				{
				case TP:
				{
					TeleportationEventButtonReleased(player, _button, _state->enemies);
					break;
				}
				default:
					break;
				}
			}
		}
	}
}

void Skill_AddSkillScore(Players *_players, unsigned long _scoreToAdd, Difficulty *_currentDif)
{
	for (int i = 0; i < _players->number; i++)
	{
		Player *player = _players->array[i];
		sfBool someoneHasActivatedSkill = sfFalse;

		// Check if someone has used a skill
		for (int j = 0; j < _players->number; j++)
		{
			if (_players->array[j]->skillInfo.isActive)
			{
				someoneHasActivatedSkill = sfTrue;
			}
		}

		// Can't earn score to unlock skills if someone is using a skill
		if (someoneHasActivatedSkill == sfFalse)
		{
			if (player->skillInfo.currentScore < player->skillInfo.scoreToUnlock)
			{
				player->skillInfo.currentScore += (unsigned long)((_scoreToAdd / (unsigned long)_players->number) * _currentDif->scoreMultiplier);
			}
		}

		if ((player->skillInfo.currentScore >= player->skillInfo.scoreToUnlock) && player->skillInfo.nbSkillInStorage < player->skillInfo.skillStorageCap)
		{
			player->skillInfo.currentScore -= player->skillInfo.scoreToUnlock;
			player->skillInfo.nbSkillInStorage++;
		}

		if (player->skillInfo.nbSkillInStorage == player->skillInfo.skillStorageCap)
		{
			player->skillInfo.currentScore = (unsigned long)0;
		}
	}
}

void Skill_Update(float _dt, GameData *_state)
{
	for (int i = 0; i < _state->players.number; i++)
	{
		Player *player = _state->players.array[i];

		if (player->skillInfo.isActive == sfTrue)
		{
			// Checks if the skill uses a timer
			if (player->skillInfo.duration.maxTime > 0.f)
			{
				// Updates the timer and skill
				Timer_Update(&player->skillInfo.duration, _dt);
			}

			switch (player->skillInfo.type)
			{
			case REGEN:
			{
				RegenUpdate(player, _dt, &_state->players);
				break;
			}
			case ATK_BOOST:
			{
				DmgBoostUpdate(player, _dt, &_state->players);
				break;
			}
			case ENEMY_CLEAR:
			{
				EnemyClearUpdate(player, _dt);
				break;
			}
			case CLEAR_PROJ:
			{
				ClearProjUpdate(player, _dt);
				break;
			}
			case PROTECTION_SHIELD:
			{
				ProtectionShieldUpdate(player, _dt, &_state->players);
				break;
			}
			case REZ:
			{
				RezUpdate(player, _dt);
				break;
			}
			case TP:
			{
				TeleportationUpdate(player, _dt, _state->enemies);
				break;
			}
			case DASH:
			{
				DashUpdate(player, _dt);
				break;
			}
			default:
				break;
			}

			// Checks if the skill uses a timer
			if (player->skillInfo.duration.maxTime > 0.f)
			{
				// Resets and stop the timer and skill
				if (Timer_Check(&player->skillInfo.duration) && player->skillInfo.bypass == sfFalse)
				{
					player->skillInfo.isActive = sfFalse;
					Timer_Reset(&player->skillInfo.duration);
				}
			}
		}
	}
}

void Skill_Display(Engine *_engine, GameData *_state)
{
	for (int i = 0; i < _state->players.number; i++)
	{
		Player *player = _state->players.array[i];

		if (player->skillInfo.isActive)
		{
			// Display animation of the skill
			if (player->skillInfo.autoDisplay)
			{
				DisplayAnimation(_engine->canvas, player->skillInfo.anim, &player->skillInfo.entity, _engine->gameScale, NULL);
			}

			// Display special animations
			switch (player->skillInfo.type)
			{
			case REGEN:
			{
				RegenDisplay(_engine, player, &_state->players);
				break;
			}
			case ATK_BOOST:
			{
				DmgBoostDisplay(_engine, &_state->players, player);
				break;
			}
			case PROTECTION_SHIELD:
			{
				ProtectionShieldDisplay(_engine, &player->skillInfo, player, &_state->players);
				break;
			}
			case REZ:
			{
				RezDisplay(_engine, &player->skillInfo, &_state->players);
				break;
			}
			case TP:
			{
				TeleportationDisplay(_engine, player);
				break;
			}

			default:
				break;
			}
		}
	}
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //

// -- REGEN ------------------------------------------------------ //

void RegenUpdate(Player *_player, float _dt, Players *_players)
{
	_player->skillInfo.entity.position = _player->entity.position;
	UpdateAnimation(_dt, _player->skillInfo.anim, &_player->skillInfo.entity);

	for (int i = 0; i < _players->number; i++)
	{
		if (_players->array[i]->isAlive && _players->array[i]->armor < _players->array[i]->maxArmor)
		{
			_players->array[i]->armor += _player->skillInfo.regenSpeed * _dt;
			if (_players->array[i]->armor > _players->array[i]->maxArmor)
			{
				_players->array[i]->armor = _players->array[i]->maxArmor;
			}
		}
		else if (_players->array[i]->armor > _players->array[i]->maxArmor)
		{
			_players->array[i]->armor = _players->array[i]->maxArmor;
		}
	}
}

void RegenDisplay(Engine *_engine, Player *_player, Players *_players)
{
	for (int i = 0; i < _players->number; i++)
	{
		if (_players->array[i] != _player && _players->array[i]->isAlive)
		{
			Entity entity = _player->skillInfo.entity;
			entity.position = _players->array[i]->entity.position;
			DisplayAnimation(_engine->canvas, _player->skillInfo.anim, &entity, _engine->gameScale, NULL);
		}
	}
}

// -- ATK BOOST ------------------------------------------------------ //

void DmgBoostUpdate(Player *_player, float _dt, Players *_players)
{
	_player->skillInfo.entity.position = _player->entity.position;
	UpdateAnimation(_dt, _player->skillInfo.anim, &_player->skillInfo.entity);

	for (int i = 0; i < _players->number; i++)
	{
		Player *player = _players->array[i];

		if (!Timer_Check(&_player->skillInfo.duration) && (_player->skillInfo.hasBeenActivated_Boost == sfFalse))
		{
			// Add to multipliers
			player->dmgMultiplier += 1.f + _player->skillInfo.boostPerCent_Dmg;
			player->fireRateMultiplier -= _player->skillInfo.boostPerCent_FireRate;
		}
		else if (Timer_Check(&_player->skillInfo.duration))
		{
			// Sub dmg multi
			player->dmgMultiplier -= 1.f + _player->skillInfo.boostPerCent_Dmg;
			player->fireRateMultiplier += _player->skillInfo.boostPerCent_FireRate;
		}
	}

	_player->skillInfo.hasBeenActivated_Boost = sfTrue;
	_player->skillInfo.bypass = sfTrue;

	if (Timer_Check(&_player->skillInfo.duration))
	{
		_player->skillInfo.bypass = sfFalse;
		_player->skillInfo.hasBeenActivated_Boost = sfFalse;
		ResetAnim(&_player->skillInfo.entity);
	}
}

void DmgBoostDisplay(Engine *_engine, Players *_players, Player *_playerUsingSkill)
{
	for (int i = 0; i < _players->number; i++)
	{
		Player *player = _players->array[i];
		if (player != _playerUsingSkill && player->isAlive)
		{
			Entity entity = _playerUsingSkill->skillInfo.entity;
			entity.position = player->entity.position;
			DisplayAnimation(_engine->canvas, _playerUsingSkill->skillInfo.anim, &entity, _engine->gameScale, NULL);
		}
	}
}

// -- ENEMY_CLEAR ------------------------------------------------------ //

void EnemyClearEventButtonPressed(Enemies* _enemies, joystickButton _button, Player* _player)
{
	for (int i = 0; i < _enemies->number; i++)
	{
		_enemies->array[i]->health = 0;
	}
}

void EnemyClearUpdate(Player *_player, float _dt)
{
	_player->skillInfo.entity.position = _player->entity.position;
	UpdateAnimation(_dt, _player->skillInfo.anim, &_player->skillInfo.entity);

	if (Timer_Check(&_player->skillInfo.entity.animationTimer))
	{
		_player->skillInfo.isActive = sfFalse;
		ResetAnim(&_player->skillInfo.entity);
	}
}

// -- CLEAR PROJ ------------------------------------------------------ //

void ClearProjEventButtonPressed(Player *_player, joystickButton _button, List *_enemyProjList)
{
	for (ListElement *tempElement = _enemyProjList->first; tempElement != NULL; tempElement = _enemyProjList->first)
	{
		List_DestroyElementFromPointer(_enemyProjList, tempElement, 1);
	}
}

void ClearProjUpdate(Player *_player, float _dt)
{
	_player->skillInfo.entity.position = _player->entity.position;
	UpdateAnimation(_dt, _player->skillInfo.anim, &_player->skillInfo.entity);

	if (Timer_Check(&_player->skillInfo.entity.animationTimer))
	{
		_player->skillInfo.isActive = sfFalse;
		ResetAnim(&_player->skillInfo.entity);
	}
}

// -- PROTECTION SHIELD ---------------------------------------------- //

void ProtectionShieldButtonPressed(Player *_player, Players *_players, joystickButton _button)
{
	for (int i = 0; i < _players->number; i++)
	{
		_players->array[i]->protectionShield = _player->skillInfo.nbProtectionShield;
		_player->skillInfo.isActive = sfTrue;
	}
}

void ProtectionShieldUpdate(Player *_player, float _dt, Players *_players)
{
	if (_player->protectionShield > 0)
	{
		_player->skillInfo.entity.position = _player->entity.position;
		UpdateAnimation(_dt, _player->skillInfo.anim, &_player->skillInfo.entity);
	}
	else
	{
		ResetAnim(&_player->skillInfo.entity);
		_player->skillInfo.isActive = sfFalse;
	}
}

// Use the same animation for every players
// Only the position change
void ProtectionShieldDisplay(Engine *_engine, Skill *_skill, Player* _player, Players* _players)
{
	for (int i = 0; i < _players->number; i++)
	{
		if ((_players->array[i]->protectionShield > 0) && (_players->array[i]->isAlive) && (_players->array[i] != _player))
		{
			_player->skillInfo.entity.position = _players->array[i]->entity.position;
			DisplayAnimation(_engine->canvas, _skill->anim, &_skill->entity, _engine->gameScale, NULL);
		}
	}

	// Reset position to original owner of the skill
	_player->skillInfo.entity.position = _player->entity.position;
}


// -- REZ --------------------------------------------------------- //

void RezEventButtonPressed(Player *_player, joystickButton _button, Players *_players)
{
	// Inspect
	int nbDeadPlayers = 0;
	for (int i = 0; i < _players->number; i++)
	{
		if (_players->array[i] != _player && _players->array[i]->isAlive == sfFalse)
		{
			nbDeadPlayers++;
		}
	}

	if (nbDeadPlayers > 0)
	{
		_player->skillInfo.nbIndex = 0;

		_player->skillInfo.indexOfPlayerRez = calloc(nbDeadPlayers, sizeof(int));

		if (_player->skillInfo.indexOfPlayerRez != NULL)
		{
			//Rez
			for (int i = 0; i < _players->number; i++)
			{
				if (_players->array[i] != _player && _players->array[i]->isAlive == sfFalse)
				{
					_players->array[i]->isAlive = sfTrue;
					// Revive with half armor
					_players->array[i]->armor = _players->array[i]->maxArmor / 2.f;
					// Set invulnerability for a short time
					Timer_Reset(&_players->array[i]->invulnerability);
					_player->skillInfo.indexOfPlayerRez[_player->skillInfo.nbIndex] = i;
					_player->skillInfo.nbIndex++;
				}
			}
		}
	}
	else if (nbDeadPlayers == 0)
	{
		_player->skillInfo.isActive = sfFalse;
		_player->skillInfo.nbSkillInStorage++;
	}
}

void RezUpdate(Player *_player, float _dt)
{
	_player->skillInfo.entity.position = _player->entity.position;
	UpdateAnimation(_dt, _player->skillInfo.anim, &_player->skillInfo.entity);
	UpdateAnimation(_dt, _player->skillInfo.animRez, &_player->skillInfo.entityRez);

	if (Timer_Check(&_player->skillInfo.entity.animationTimer) && Timer_Check(&_player->skillInfo.entityRez.animationTimer))
	{
		if (_player->skillInfo.indexOfPlayerRez != NULL)
		{
			free(_player->skillInfo.indexOfPlayerRez);
			_player->skillInfo.indexOfPlayerRez = NULL;
		}
		_player->skillInfo.isActive = sfFalse;
		ResetAnim(&_player->skillInfo.entity);
		ResetAnim(&_player->skillInfo.entityRez);
	}
}

void RezDisplay(Engine *_engine, Skill *_skill, Players *_players)
{
	for (int i = 0; i < _skill->nbIndex; i++)
	{
		if (_players->array[_skill->indexOfPlayerRez[i]]->isAlive)
		{
			_skill->entityRez.position = _players->array[_skill->indexOfPlayerRez[i]]->entity.position;
			DisplayAnimation(_engine->canvas, _skill->animRez, &_skill->entityRez, _engine->gameScale, NULL);
		}
	}
}

// -- TP ------------------------------------------------------ //

void TeleportationEventButtonPressed(Player *_player, joystickButton _button, Lasers *_lasers)
{
	_player->internalDisplay = sfFalse;
	_player->skillInfo.hasDisappeared = sfTrue;
	_player->skillInfo.autoDisplay = sfFalse;
	_player->skillInfo.entityDisappearTP.position = _player->entity.position;
	_player->skillInfo.entityDisappearTP.animationTimer.isPlaying = sfTrue;
	_player->skillInfo.bypass = sfTrue;
	_player->skillInfo.entity.animationTimer.isPlaying = sfTrue;
	// Bigger collider during Teleportation
	_player->skillInfo.cockpitColliderSaved = _player->cockpitCollider;
	_player->cockpitCollider = _player->spaceshipCollider;
	// So that enemies can't attack him during the teleportation
	_player->canBeTargeted = sfFalse;

	Laser_StopShooting(_lasers, _player, &_player->weapons[_player->currentFireMode].weapon);
}

void TeleporationEnemyDestruction(Player *_player, Enemies *_enemies)
{
	Collider zoneCol = Collider_CreateCircleCollider(_player->entity.position, _player->skillInfo.effectRadius);

	for (int i = _enemies->number - 1; i >= 0; i--)
	{
		Enemy *enemy = _enemies->array[i];

		if (Collider_CheckCollision(&zoneCol, &enemy->largeCollider))
		{
			enemy->health = 0.f;
		}
	}
}

void TeleportationEventButtonReleased(Player *_player, joystickButton _button, Enemies *_enemies)
{
	_player->skillInfo.hasDisappeared = sfFalse;
	_player->canBeTargeted = sfTrue;
	_player->internalDisplay = sfTrue;
	_player->skillInfo.entityBlastTP.position = _player->entity.position;
	_player->skillInfo.duration.currentTime = _player->skillInfo.duration.maxTime;
	Timer_Reset(&_player->invulnerability);
	// Give back small collider
	_player->cockpitCollider = _player->skillInfo.cockpitColliderSaved;
	// Stop animations
	ResetAnim(&_player->skillInfo.entity);
	ResetAnim(&_player->skillInfo.entityDisappearTP);
	_player->skillInfo.entityDisappearTP.animationTimer.isPlaying = sfFalse; // Hard stop if animation is not finished when keyreleased
	_player->skillInfo.entity.animationTimer.isPlaying = sfFalse;
	// Begin blast animation
	_player->skillInfo.entityBlastTP.animationTimer.isPlaying = sfTrue;
	// Need to check collision to blast enemies away...
	TeleporationEnemyDestruction(_player, _enemies);
}

void TeleportationUpdate(Player *_player, float _dt, Enemies *_enemies)
{
	if ((sfJoystick_isButtonPressed(_player->joystick, SKILL_BUTTON) && (!Timer_Check(&_player->skillInfo.duration))) || (sfKeyboard_isKeyPressed(sfKeyA) && (!Timer_Check(&_player->skillInfo.duration))))
	{
		Timer_Reset(&_player->invulnerability);
		_player->skillInfo.entity.position = _player->entity.position;
		UpdateAnimation(_dt, _player->skillInfo.anim, &_player->skillInfo.entity);
		UpdateAnimation(_dt, _player->skillInfo.animDisappear, &_player->skillInfo.entityDisappearTP);
		if (Timer_Check(&_player->skillInfo.entityDisappearTP.animationTimer))
		{
			ResetAnim(&_player->skillInfo.entityDisappearTP);
			_player->skillInfo.entityDisappearTP.animationTimer.isPlaying = sfFalse;
		}
	}
	else
	{
		if (_player->skillInfo.entityDisappearTP.animationTimer.isPlaying || _player->skillInfo.entity.animationTimer.isPlaying)
		{
			_player->skillInfo.hasDisappeared = sfFalse;
			_player->canBeTargeted = sfTrue;
			_player->internalDisplay = sfTrue;
			ResetAnim(&_player->skillInfo.entity);
			ResetAnim(&_player->skillInfo.entityDisappearTP);
			_player->skillInfo.entityDisappearTP.animationTimer.isPlaying = sfFalse;
			_player->skillInfo.entity.animationTimer.isPlaying = sfFalse;
			// Give back small collider
			_player->cockpitCollider = _player->skillInfo.cockpitColliderSaved;
			// Begin blast animation
			_player->skillInfo.entityBlastTP.position = _player->entity.position;
			_player->skillInfo.entityBlastTP.animationTimer.isPlaying = sfTrue;
			// Need to check collision to blast enemies away...
			TeleporationEnemyDestruction(_player, _enemies);
		}
		UpdateAnimation(_dt, _player->skillInfo.animBlast, &_player->skillInfo.entityBlastTP);
	}

	if (Timer_Check(&_player->skillInfo.entityBlastTP.animationTimer))
	{
		ResetAnim(&_player->skillInfo.entityBlastTP);
		_player->skillInfo.entityBlastTP.animationTimer.isPlaying = sfFalse;
		_player->skillInfo.bypass = sfFalse;
	}
}

void TeleportationDisplay(Engine *_engine, Player *_player)
{
	if (_player->skillInfo.entity.animationTimer.isPlaying)
	{
		DisplayAnimation(_engine->canvas, _player->skillInfo.anim, &_player->skillInfo.entity, _engine->gameScale, NULL);
	}
	if (_player->internalDisplay == sfFalse)
	{
		// Draw player in see through
		sfRenderStates *state = NULL;
		state = Shader_PlayerSpaceship((int)_player->armor, (int)_player->maxArmor, _player->color, sfTrue, _engine->time);
		DisplayAnimation(_engine->canvas, _player->currentAnim, &_player->entity, _engine->gameScale, state);
	}
	if (_player->skillInfo.entityDisappearTP.animationTimer.isPlaying)
	{
		DisplayAnimation(_engine->canvas, _player->skillInfo.animDisappear, &_player->skillInfo.entityDisappearTP, _engine->gameScale, NULL);
	}
	if (_player->skillInfo.entityBlastTP.animationTimer.isPlaying)
	{
		DisplayAnimation(_engine->canvas, _player->skillInfo.animBlast, &_player->skillInfo.entityBlastTP, _engine->gameScale, NULL);
	}
}

// -- Dash ------------------------------------- //

void DashEventButtonPressed(Player* _player)
{
	_player->skillInfo.isDashing = sfTrue;

	// Bigger collider during Teleportation
	_player->skillInfo.cockpitColliderSaved = _player->cockpitCollider;
	_player->cockpitCollider = _player->spaceshipCollider;

	if (_player->joystick == 4)
	{
		if (sfKeyboard_isKeyPressed(sfKeyZ))
		{
			_player->entity.velocity.y = -PLAYER_SPEED;
		}
		if (sfKeyboard_isKeyPressed(sfKeyQ))
		{
			_player->entity.velocity.x = -PLAYER_SPEED * PLAYER_X_VELOCITY_MULTIPLIER;
		}
		if (sfKeyboard_isKeyPressed(sfKeyS))
		{
			_player->entity.velocity.y = PLAYER_SPEED;
		}
		if (sfKeyboard_isKeyPressed(sfKeyD))
		{
			_player->entity.velocity.x = PLAYER_SPEED * PLAYER_X_VELOCITY_MULTIPLIER;
		}
	}
	else
	{
		sfVector2f axisPosition;
		if (_player->isAlive)
		{
			// Change x velocity
			// Get the position of the directional pad
			axisPosition.x = axisPosition.x = sfJoystick_getAxisPosition(_player->joystick, sfJoystickPovX);
			// If the directional pad have a default position, use the position of the left stick
			if (axisPosition.x == 0.0f)
			{
				axisPosition.x = sfJoystick_getAxisPosition(_player->joystick, sfJoystickX);
			}
			// Change x velocity to 0 if the input is lower than the joystick sensitivity
			if (axisPosition.x >= -JOYSTICK_MINIMUM_SENSITIVITY && axisPosition.x <= JOYSTICK_MINIMUM_SENSITIVITY)
			{
				axisPosition.x = 0.0f;
			}
			// Change y velocity
			// Get the position of the directional pad
			axisPosition.y = -sfJoystick_getAxisPosition(_player->joystick, sfJoystickPovY);
			// If the directional pad have a default position, use the position of the left stick
			if (axisPosition.y == 0.0f)
			{
				axisPosition.y = sfJoystick_getAxisPosition(_player->joystick, sfJoystickY);
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

			_player->entity.velocity.x = axisPosition.x / 100.0f * PLAYER_SPEED * PLAYER_X_VELOCITY_MULTIPLIER;
			_player->entity.velocity.y = axisPosition.y / 100.0f * PLAYER_SPEED;
		}
	}

	if (_player->entity.velocity.x == 0 && _player->entity.velocity.y == 0)
	{
		_player->entity.velocity.y = -100 / 100.0f * PLAYER_SPEED;
	}
	_player->entity.velocity.x *= _player->skillInfo.dashRange / _player->skillInfo.duration.maxTime;
	_player->entity.velocity.y *= _player->skillInfo.dashRange / _player->skillInfo.duration.maxTime;
}

void DashUpdate(Player * _player, float _dt)
{
	Timer_Reset(&_player->invulnerability);

	// Set animation on player and update
	_player->skillInfo.entity.position = _player->entity.position;
	UpdateAnimation(_dt, _player->skillInfo.anim, &_player->skillInfo.entity);

	if (Timer_Check(&_player->skillInfo.duration))
	{
		_player->skillInfo.isDashing = sfFalse;

		// Give back small collider
		_player->cockpitCollider = _player->skillInfo.cockpitColliderSaved;
	}
}