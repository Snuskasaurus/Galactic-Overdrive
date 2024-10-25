#include "Difficulty.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

void LoadDifficultyNbPlayerRelated(Difficulties *_gameDifficulties);
void LoadGameModeDifficulty(Difficulties *_gameDifficulties);
void LoadStageDifficulty(Difficulties *_gameDifficulties);


// -- SHARED FUNCTIONS ----------------------------------------------------- //

Difficulties DifficultyData_Load()
{
	Difficulties gameDifficulties = { 0 };

	LoadDifficultyNbPlayerRelated(&gameDifficulties);
	LoadGameModeDifficulty(&gameDifficulties);
	LoadStageDifficulty(&gameDifficulties);

	return gameDifficulties;
}


// -- LOCAL FUNCTIONS ------------------------------------------------------ //

void LoadDifficultyNbPlayerRelated(Difficulties *_gameDifficulties)
{
	ExploDir directoryNbPlayers = { 0 };
	char dirPath[BUFFER_LARGE] = DIFFICULTY_DIR;
	strcat(dirPath, "/");
	strcat(dirPath, DIF_PLAYER_FOLDER_NAME);
	directoryNbPlayers = ExploreDirectory(dirPath);

	for (int j = 0; j < directoryNbPlayers.nbrFiles; j++)
	{
		FILE *difficultyFile = NULL;
		char filepath[BUFFER_LARGE] = DIFFICULTY_DIR;
		strcat(filepath, "/");
		strcat(filepath, DIF_PLAYER_FOLDER_NAME);
		strcat(filepath, "/");
		strcat(filepath, directoryNbPlayers.fileNames[j]);
		difficultyFile = fopen(filepath, "rt");

		if (difficultyFile != NULL)
		{
			while (!feof(difficultyFile))
			{
				char keyWord[BUFFER_SMALL] = { 0 };
				sfBool didntFoundKeyWord = sfTrue;
				fscanf(difficultyFile, "%s", keyWord);

				// Read the file
				if (strcmp(keyWord, "SCORE_MULTIPLIER") == 0)
				{
					fscanf(difficultyFile, "%f\n", &_gameDifficulties->playerNumberDif[j].scoreMultiplier);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "ENEMIES_HEALTH") == 0)
				{
					fscanf(difficultyFile, "%f\n", &_gameDifficulties->playerNumberDif[j].enemiesHealthMultiplier);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "ENEMIES_DAMAGE") == 0)
				{
					fscanf(difficultyFile, "%f\n", &_gameDifficulties->playerNumberDif[j].enemiesDamageMultiplier);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "ENEMIES_SPAWN_NUMBER") == 0)
				{
					fscanf(difficultyFile, "%f\n", &_gameDifficulties->playerNumberDif[j].enemiesSpawnMultiplier);
					didntFoundKeyWord = sfFalse;
				}

				if (didntFoundKeyWord == sfTrue)
				{
					fscanf(difficultyFile, "%[^\n]\n", keyWord);
				}
			}
			fclose(difficultyFile);
		}

		// Exit loop if nbr of files is superior to the max nbr players.
		if ((j == MAX_PLAYER_NUMBER - 1) && (directoryNbPlayers.nbrFiles > MAX_PLAYER_NUMBER))
		{
			j = directoryNbPlayers.nbrFiles;
		}
	}
}

void LoadGameModeDifficulty(Difficulties *_gameDifficulties)
{
	ExploDir directoryGMode = { 0 };
	char dirPath[BUFFER_LARGE] = DIFFICULTY_DIR;
	strcat(dirPath, "/");
	strcat(dirPath, DIF_GMODE_FOLDER_NAME);
	directoryGMode = ExploreDirectory(dirPath);

	for (int j = 0; j < directoryGMode.nbrFiles; j++)
	{
		FILE *difficultyFile = NULL;
		char filepath[BUFFER_LARGE] = DIFFICULTY_DIR;
		strcat(filepath, "/");
		strcat(filepath, DIF_GMODE_FOLDER_NAME);
		strcat(filepath, "/");
		strcat(filepath, directoryGMode.fileNames[j]);
		difficultyFile = fopen(filepath, "rt");

		if (difficultyFile != NULL)
		{
			while (!feof(difficultyFile))
			{
				char keyWord[BUFFER_SMALL] = { 0 };
				sfBool didntFoundKeyWord = sfTrue;
				fscanf(difficultyFile, "%s", keyWord);
				
				// Read the file

				if (strcmp(keyWord, "WEAPON_MULTIPLIER_GOOD")==0)
				{
					fscanf(difficultyFile, "%f\n", &_gameDifficulties->gamemodeDif[j].weaponMultiplierGood);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "WEAPON_MULTIPLIER_BAD")==0)
				{
					fscanf(difficultyFile, "%f\n", &_gameDifficulties->gamemodeDif[j].weaponMultiplierBad);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "ENEMIES_HEALTH") == 0)
				{
					fscanf(difficultyFile, "%f\n", &_gameDifficulties->gamemodeDif[j].enemiesHealthMultiplier);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "ENEMIES_PRECISION") == 0)
				{
					fscanf(difficultyFile, "%f\n", &_gameDifficulties->gamemodeDif[j].enemiesPrecisionMultiplier);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "ENEMIES_DAMAGE") == 0)
				{
					fscanf(difficultyFile, "%f\n", &_gameDifficulties->gamemodeDif[j].enemiesDamageMultiplier);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "OBJECT_COST") == 0)
				{
					fscanf(difficultyFile, "%f\n", &_gameDifficulties->gamemodeDif[j].objectCostMultplier);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "ENEMIES_SPEED") == 0)
				{
					fscanf(difficultyFile, "%f\n", &_gameDifficulties->gamemodeDif[j].enemiesSpeedMultiplier);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "ENEMIES_PROJECTILES_SPEED") == 0)
				{
					fscanf(difficultyFile, "%f\n", &_gameDifficulties->gamemodeDif[j].enemiesProjSpeedMultiplier);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "ENEMIES_FIRING_RATE") == 0)
				{
					fscanf(difficultyFile, "%f\n", &_gameDifficulties->gamemodeDif[j].enemiesFiringRateMultiplier);
					didntFoundKeyWord = sfFalse;
				}

				if (didntFoundKeyWord == sfTrue)
				{
					fscanf(difficultyFile, "%[^\n]\n", keyWord);
				}
			}
			fclose(difficultyFile);
		}

		// Exit loop if nbr of files is superior to the nbr of difficulties allowed.
		if (j == NB_DIF_MODE)
		{
			j = directoryGMode.nbrFiles;
		}
	}
}

void LoadStageDifficulty(Difficulties *_gameDifficulties)
{
	ExploDir directoryStage = { 0 };
	char dirPath[BUFFER_LARGE] = DIFFICULTY_DIR;
	strcat(dirPath, "/");
	strcat(dirPath, DIF_STAGE_FOLDER_NAME);
	directoryStage = ExploreDirectory(dirPath);

	for (int j = 0; j < directoryStage.nbrFiles; j++)
	{
		FILE *difficultyFile = NULL;
		char filepath[BUFFER_LARGE] = DIFFICULTY_DIR;
		strcat(filepath, "/");
		strcat(filepath, DIF_STAGE_FOLDER_NAME);
		strcat(filepath, "/");
		strcat(filepath, directoryStage.fileNames[j]);
		difficultyFile = fopen(filepath, "rt");

		if (difficultyFile != NULL)
		{
			while (!feof(difficultyFile))
			{
				char keyWord[BUFFER_SMALL] = { 0 };
				sfBool didntFoundKeyWord = sfTrue;
				fscanf(difficultyFile, "%s", keyWord);

				// Read the file
				if (strcmp(keyWord, "ENEMIES_HEALTH") == 0)
				{
					fscanf(difficultyFile, "%f\n", &_gameDifficulties->stageDif[j].enemiesHealthMultiplier);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "ENEMIES_DAMAGE") == 0)
				{
					fscanf(difficultyFile, "%f\n", &_gameDifficulties->stageDif[j].enemiesDamageMultiplier);
					didntFoundKeyWord = sfFalse;
				}

				if (didntFoundKeyWord == sfTrue)
				{
					fscanf(difficultyFile, "%[^\n]\n", keyWord);
				}
			}
			fclose(difficultyFile);
		}

		// Exit loop if nbr of files is superior to the nbr of stages allowed.
		if (j == DIF_MAX_NB_STAGES)
		{
			j = directoryStage.nbrFiles;
		}
	}
}

Difficulty SetDifficulty(Difficulties _difficultyData, int _numberPlayers, eDifficultyMode _gameModeDif, int _stage)
{
	Difficulty tempDifficulty = {0};

	// Settings
	tempDifficulty.mode = _gameModeDif;
	tempDifficulty.nbPlayer = _numberPlayers;
	tempDifficulty.stage = _stage;

	// Players number
	tempDifficulty.scoreMultiplier				= 1.0f * _difficultyData.playerNumberDif[_numberPlayers - 1].scoreMultiplier;
	tempDifficulty.enemiesHealthMultiplier		= 1.0f * _difficultyData.playerNumberDif[_numberPlayers - 1].enemiesHealthMultiplier;
	tempDifficulty.enemiesDamageMultiplier		= 1.0f * _difficultyData.playerNumberDif[_numberPlayers - 1].enemiesDamageMultiplier;
	tempDifficulty.enemiesSpawnMultiplier		= 1.0f * _difficultyData.playerNumberDif[_numberPlayers - 1].enemiesSpawnMultiplier;

	// GameMode Difficulty
	tempDifficulty.weaponMultiplierGood	    	 = 1.0f * _difficultyData.gamemodeDif[_gameModeDif].weaponMultiplierGood;
	tempDifficulty.weaponMultiplierBad			 = 1.0f * _difficultyData.gamemodeDif[_gameModeDif].weaponMultiplierBad;
	tempDifficulty.enemiesHealthMultiplier	     = 1.0f * _difficultyData.gamemodeDif[_gameModeDif].enemiesHealthMultiplier;
	tempDifficulty.enemiesPrecisionMultiplier	 = 1.0f * _difficultyData.gamemodeDif[_gameModeDif].enemiesPrecisionMultiplier;
	tempDifficulty.enemiesDamageMultiplier		 = 1.0f * _difficultyData.gamemodeDif[_gameModeDif].enemiesDamageMultiplier;
	tempDifficulty.objectCostMultplier			 = 1.0f * _difficultyData.gamemodeDif[_gameModeDif].objectCostMultplier;
	tempDifficulty.enemiesSpeedMultiplier		 = 1.0f * _difficultyData.gamemodeDif[_gameModeDif].enemiesSpeedMultiplier;
	tempDifficulty.enemiesProjSpeedMultiplier	 = 1.0f * _difficultyData.gamemodeDif[_gameModeDif].enemiesProjSpeedMultiplier;
	tempDifficulty.enemiesFiringRateMultiplier	 = 1.0f * _difficultyData.gamemodeDif[_gameModeDif].enemiesFiringRateMultiplier;
	
	// Stage
	tempDifficulty.enemiesHealthMultiplier		 = 1.0f * _difficultyData.stageDif[_stage - 1].enemiesHealthMultiplier;
	tempDifficulty.enemiesDamageMultiplier		 = 1.0f * _difficultyData.stageDif[_stage - 1].enemiesDamageMultiplier;

	return tempDifficulty;
}