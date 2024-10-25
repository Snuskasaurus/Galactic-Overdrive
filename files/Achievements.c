#include "Achievements.h"

BP_Achievements Achievements_LoadBlueprints(MainData* _data)
{
	printf("Initialization of achievements blueprints : \n");
	BP_Achievements blueprints;
	ExploDir directory = ExploreDirectory(ACHIEVEMENTS_DIR);
	// Allocate the array
	blueprints.number = directory.nbrFiles;
	blueprints.array = calloc(blueprints.number, sizeof(BP_Achievements*));
	// Read all achievements file
	for (int i = 0; i < directory.nbrFiles; i++)
	{
		char filepath[BUFFER_LARGE] = ACHIEVEMENTS_DIR;
		strcat(filepath, "/");
		strcat(filepath, directory.fileNames[i]);
		FILE *file = fopen(filepath, "rt");
		Achievement *achievement = calloc(1, sizeof(Achievement));
		if (file && achievement)
		{
			char iconSpritePath[BUFFER_MEDIUM] = ACHIEVEMENTS_SPRITES_DIR;

			while (!feof(file))
			{
				char keyWord[BUFFER_SMALL] = { 0 };
				sfBool didntFoundKeyWord = sfTrue;
				fscanf(file, "%s", keyWord);

				// Read the file
				if (strcmp(keyWord, "ID_NAME") == 0)
				{
					fscanf(file, "%s\n", &achievement->id_Name);
					///printf("ID_NAME : %s\n", achievement->id_Name);

					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "ICON") == 0)
				{
					fscanf(file, "%s\n", &achievement->icon_Name);
					///printf("ICON : %s\n", achievement->icon_Name);

					// -- Load Sprite
					strcat(iconSpritePath, "/");
					strcat(iconSpritePath, achievement->icon_Name);
					achievement->icon_Sprite = Sprite_CreateFromFile(iconSpritePath, 0, 0);
					if (achievement->icon_Sprite != NULL)
					{
						achievement->icon_Size = sfTexture_getSize(sfSprite_getTexture(achievement->icon_Sprite));
					}

					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "TITLE") == 0)
				{
					fscanf(file, "%[^\n]\n", &achievement->title);
					///printf("TITLE : %s\n", achievement->title);
					achievement->parametersTextTitle.font = Text_GetFont(_data->engine.text, "AXIS Extra Bold 800.otf");
					achievement->parametersTextTitle.color = sfWhite;

					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "DESC") == 0)
				{
					fscanf(file, "%[^\n]\n", &achievement->description);
					///printf("DESC : %s\n", achievement->description);
					achievement->parametersTextDesc.font = Text_GetFont(_data->engine.text, "Arial.ttf");
					achievement->parametersTextDesc.color = sfWhite;
					didntFoundKeyWord = sfFalse;
				}

				if (didntFoundKeyWord == sfTrue)
				{
					fscanf(file, "%[^\n]\n", keyWord);
				}
			}

			// Add the achievement to the blueprints
			printf("(%d/%d)\t%s\n", i + 1, directory.nbrFiles, achievement->id_Name);
			blueprints.array[i] = achievement;
			fclose(file);
		}
		else
		{
			printf("Problem while trying to create the achievement %s\n", directory.fileNames[i]);
		}
	}
	printf("\n");
	return blueprints;
}

void Achievement_Save(Engine* _engine, const char* _filename)
{
	FILE *file = NULL;
	BP_Achievements* achievementsBP = &_engine->blueprints.achievements;
	char filepath[BUFFER_LARGE] = { 0 };
	int sizeString = 0;

	strcpy(filepath, _engine->configuration->gameFilePath);
	strcat(filepath, "/");
	strcat(filepath, _filename);
	strcat(filepath, ".data");
	file = fopen(filepath, "wt+");
	// Right the file
	if (file)
	{
		// -- Total number of achievements
		fprintf(file, "%d \n", achievementsBP->number);
		// -- Loop every achievement ------------------- //
		for (int i = 0; i < achievementsBP->number; i++)
		{
			Achievement* currentAchievement = achievementsBP->array[i];

			// -- Achivement ID
			fprintf(file,"%s ", currentAchievement->id_Name);
			// -- Achievement unlock state
			fprintf(file,"%d ", currentAchievement->isUnlocked);
			fputs("\n", file);
		}
		fclose(file);
	}
}

void Achievement_Load(Engine* _engine, const char* _filename)
{
	FILE *file = NULL;
	BP_Achievements* achievementsBP = &_engine->blueprints.achievements;
	char filepath[BUFFER_LARGE] = { 0 };
	int sizeString = 0;

	strcpy(filepath, _engine->configuration->gameFilePath);
	strcat(filepath, "/");
	strcat(filepath, _filename);
	strcat(filepath, ".data");
	file = fopen(filepath, "r");
	// Right the file
	if (file)
	{
		int nbSavedAchievements;
		fscanf(file, "%d",&nbSavedAchievements);
		// -- Loop every achievement saved ------------------- //
		for (int i = 0; i < nbSavedAchievements; i++)
		{
			// -- Get the ID_NAME
			char ID_NAME[BUFFER_SMALL];
			fscanf(file, "%s\n", ID_NAME);
			// -- Search for the corresponding achievement
			for (int j = 0; j < achievementsBP->number; j++)
			{
				if (strcmp(achievementsBP->array[j]->id_Name, ID_NAME) == 0)
				{
					// -- Get unlock state
					fscanf(file, "%d\n", &achievementsBP->array[j]->isUnlocked);
					j = achievementsBP->number;
				}
				if (j == achievementsBP->number - 1)
				{
					printf("Load from save file : Can't load %s, couldn't found in BP_achivements\n", ID_NAME);
				}
			}
		}
		fclose(file);
	}
}

void Achievement_Unclock(Notifications* _notifications, Engine* _engine, const char* _ID_Name)
{
	BP_Achievements* achievements = &_engine->blueprints.achievements;

	// Search for the right achievement data
	for (int i = 0; i < achievements->number; i++)
	{
		// Unlock and notify when achievement found
		if (strcmp(achievements->array[i]->id_Name, _ID_Name) == 0)
		{
			if (achievements->array[i]->isUnlocked == sfFalse)
			{
				achievements->array[i]->isUnlocked = sfTrue;
				Notification_Create(_notifications, _engine, achievements->array[i], NOTIFY_ACHIEVEMENT);
				Achievement_Save(_engine, ACHIEVEMENTS_FILENAME);
				break;
			}
		}
	}
}

void Achievement_UpdateStageAchievement(MainData* _data)
{
	GameData* state = _data->stateData[GAME];

	// -- Boss is dead --------- //
	if (state->boss.isAlive == 0)
	{
		// -- Game difficulties ------------------------------- //
		if (_data->engine.currentDifficulty.mode == EASY)
		{
			// -- Current World
			/// Terra
			if (strcmp(_data->engine.world.levelNames.array[_data->engine.world.currentLevel], "HumanPlanet3") == 0)
			{
				Achievement_Unclock(&_data->notifications, &_data->engine, "TerraWorld_1");
			}
			/// Space
			else if (strcmp(_data->engine.world.levelNames.array[_data->engine.world.currentLevel], "Space3") == 0)
			{
				Achievement_Unclock(&_data->notifications, &_data->engine, "SpaceWorld_1");
			}
			/// Alien
			else if (strcmp(_data->engine.world.levelNames.array[_data->engine.world.currentLevel], "AlienPlanet3") == 0)
			{
				Achievement_Unclock(&_data->notifications, &_data->engine, "AlienWorld_1");
			}
		}
		else if (_data->engine.currentDifficulty.mode == NORMAL)
		{
			// -- Current World
			/// Terra
			if (strcmp(_data->engine.world.levelNames.array[_data->engine.world.currentLevel], "HumanPlanet3") == 0)
			{
				Achievement_Unclock(&_data->notifications, &_data->engine, "TerraWorld_2");
			}
			/// Space
			else if (strcmp(_data->engine.world.levelNames.array[_data->engine.world.currentLevel], "Space3") == 0)
			{
				Achievement_Unclock(&_data->notifications, &_data->engine, "SpaceWorld_2");
			}
			/// Alien
			else if (strcmp(_data->engine.world.levelNames.array[_data->engine.world.currentLevel], "AlienPlanet3") == 0)
			{
				Achievement_Unclock(&_data->notifications, &_data->engine, "AlienWorld_2");
			}
		}
		else if (_data->engine.currentDifficulty.mode == HARD)
		{
			// -- Current World
			/// Terra
			if (strcmp(_data->engine.world.levelNames.array[_data->engine.world.currentLevel], "HumanPlanet3") == 0)
			{
				Achievement_Unclock(&_data->notifications, &_data->engine, "TerraWorld_3");
			}
			/// Space
			else if (strcmp(_data->engine.world.levelNames.array[_data->engine.world.currentLevel], "Space3") == 0)
			{
				Achievement_Unclock(&_data->notifications, &_data->engine, "SpaceWorld_3");
			}
			/// Alien
			else if (strcmp(_data->engine.world.levelNames.array[_data->engine.world.currentLevel], "AlienPlanet3") == 0)
			{
				Achievement_Unclock(&_data->notifications, &_data->engine, "AlienWorld_3");
			}
		}
	}
}

void Achievement_UpdateToUnlock(MainData* _data)
{
	// -- Normal or superior game difficulty ----------- //
	if (_data->engine.currentDifficulty.mode >= NORMAL)
	{
		// -- Into GAME -------------------- /
		if (_data->currentState == GAME)
		{
			GameData* gameState = _data->stateData[GAME];
			// -- Boss is dead ---------------------- //
			if (gameState != NULL && gameState->boss.isAlive == sfFalse && _strcmpi(_data->engine.world.levelNames.array[_data->engine.world.currentLevel], "Tutorial") != 0)
			{
				if (_data->achievementsUnlockVar.doPlayerTookDamage == sfFalse)
				{
					Achievement_Unclock(&_data->notifications, &_data->engine, "Untouchable");
				}
				if (_data->achievementsUnlockVar.hasBoughtWeaponInAlien == 0 &&
					_data->achievementsUnlockVar.hasBoughtWeaponInSpace == 0 &&
					_data->achievementsUnlockVar.hasBoughtWeaponInTerra == 0)
				{
					Achievement_Unclock(&_data->notifications, &_data->engine, "NoWeapon");

				}
				if (_data->achievementsUnlockVar.nbEnemiesKilled == _data->achievementsUnlockVar.nbEnemiesSpawned)
				{
					Achievement_Unclock(&_data->notifications, &_data->engine, "Cleared");
				}
			}
		}
	}
}

void Achievement_GameInit(MainData* _data)
{
	if (strcmp(_data->engine.world.levelNames.array[_data->engine.world.currentLevel], "HumanPlanet") == 0)
	{
		if (_data->achievementsUnlockVar.hasBoughtWeaponInTerra == 1)
		{
			_data->achievementsUnlockVar.hasBoughtWeaponInTerra = 0;
		}
	}
	if (strcmp(_data->engine.world.levelNames.array[_data->engine.world.currentLevel], "AlienPlanet") == 0)
	{
		if (_data->achievementsUnlockVar.hasBoughtWeaponInAlien == 1)
		{
			_data->achievementsUnlockVar.hasBoughtWeaponInAlien = 0;
		}
	}
	if (strcmp(_data->engine.world.levelNames.array[_data->engine.world.currentLevel], "Space") == 0)
	{
		if (_data->achievementsUnlockVar.hasBoughtWeaponInSpace == 1)
		{
			_data->achievementsUnlockVar.hasBoughtWeaponInSpace = 0;
		}
	}

	_data->achievementsUnlockVar.nbEnemiesKilled = 0;
	_data->achievementsUnlockVar.nbEnemiesSpawned = 0;
}