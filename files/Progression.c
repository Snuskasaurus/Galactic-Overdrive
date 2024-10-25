#include "Progression.h"

void LoadProgression(GOProgression* _Progression, MainData* _data)
{
	FILE* usedFile = NULL;
	char tempString[BUFFER_SMALL] = { 0 };
	char filePath[BUFFER_SMALL] = { 0 };
	sprintf(filePath, "%s%s", _data->engine.configuration->gameFilePath, "/Progression.data");
	usedFile = fopen(filePath, "rt");
	if (!usedFile)
	{
		usedFile = fopen(filePath, "wt");
		if (!usedFile)
		{
			printf("Error while loading and creating progression\n");
			return;
		}
		fprintf(usedFile, "PROGRESS 0\n");
		fprintf(usedFile, "LEVEL_1_EASY 0 0 0 0 0 0\n");
		fprintf(usedFile, "LEVEL_1_NORMAL 0 0 0 0 0 0\n");
		fprintf(usedFile, "LEVEL_1_HARD 0 0 0 0 0 0\n");
		fprintf(usedFile, "LEVEL_2_EASY 0 0 0 0 0 0\n");
		fprintf(usedFile, "LEVEL_2_NORMAL 0 0 0 0 0 0\n");
		fprintf(usedFile, "LEVEL_2_HARD 0 0 0 0 0 0\n");
		fprintf(usedFile, "LEVEL_3_EASY 0 0 0 0 0 0\n");
		fprintf(usedFile, "LEVEL_3_NORMAL 0 0 0 0 0 0\n");
		fprintf(usedFile, "LEVEL_3_HARD 0 0 0 0 0 0");
		fclose(usedFile);
		_Progression->progress = 0;
		for (int i = 0; i < NB_DIF_MODE; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				_Progression->Level1Highscores[i][j] = 0;
				_Progression->Level2Highscores[i][j] = 0;
				_Progression->Level3Highscores[i][j] = 0;
			}
		}
		return;
	}
	while (fscanf(usedFile, "%s", tempString) != EOF)
	{
		if (!_stricmp(tempString, "PROGRESS"))
		{
			fscanf(usedFile, "%d", &_Progression->progress);
		}

		if (!_stricmp(tempString, "LEVEL_1_EASY"))
		{
			for (int i = 0; i < 6; i++)
			{
				fscanf(usedFile, "%d", &_Progression->Level1Highscores[EASY][i]);
			}
		}
		if (!_stricmp(tempString, "LEVEL_1_NORMAL"))
		{
			for (int i = 0; i < 6; i++)
			{
				fscanf(usedFile, "%d", &_Progression->Level1Highscores[NORMAL][i]);
			}
		}
		if (!_stricmp(tempString, "LEVEL_1_HARD"))
		{
			for (int i = 0; i < 6; i++)
			{
				fscanf(usedFile, "%d", &_Progression->Level1Highscores[HARD][i]);
			}
		}

		if (!_stricmp(tempString, "LEVEL_2_EASY"))
		{
			for (int i = 0; i < 6; i++)
			{
				fscanf(usedFile, "%d", &_Progression->Level2Highscores[EASY][i]);
			}
		}
		if (!_stricmp(tempString, "LEVEL_2_NORMAL"))
		{
			for (int i = 0; i < 6; i++)
			{
				fscanf(usedFile, "%d", &_Progression->Level2Highscores[NORMAL][i]);
			}
		}
		if (!_stricmp(tempString, "LEVEL_2_HARD"))
		{
			for (int i = 0; i < 6; i++)
			{
				fscanf(usedFile, "%d", &_Progression->Level2Highscores[HARD][i]);
			}
		}

		if (!_stricmp(tempString, "LEVEL_3_EASY"))
		{
			for (int i = 0; i < 6; i++)
			{
				fscanf(usedFile, "%d", &_Progression->Level3Highscores[EASY][i]);
			}
		}
		if (!_stricmp(tempString, "LEVEL_3_NORMAL"))
		{
			for (int i = 0; i < 6; i++)
			{
				fscanf(usedFile, "%d", &_Progression->Level3Highscores[NORMAL][i]);
			}
		}
		if (!_stricmp(tempString, "LEVEL_3_HARD"))
		{
			for (int i = 0; i < 6; i++)
			{
				fscanf(usedFile, "%d", &_Progression->Level3Highscores[HARD][i]);
			}
		}
	}
	fclose(usedFile);
}

void SaveProgression(GOProgression* _Progression, MainData* _data)
{
	FILE* usedFile = NULL;
	char tempString[BUFFER_SMALL] = { 0 };
	char filePath[BUFFER_SMALL] = { 0 };
	sprintf(filePath, "%s%s", _data->engine.configuration->gameFilePath, "/Progression.data");
	usedFile = fopen(filePath, "wt");
	if (!usedFile)
	{
		printf("Error while saving progression\n");
		return;
	}
	fprintf(usedFile, "PROGRESS %d", _Progression->progress);
	for (int i = 0; i < NB_DIF_MODE; i++)
	{
		fprintf(usedFile, "\nLEVEL_%d_EASY", (i + 1));
		for (int j = 0; j < 6; j++)
		{
			switch (i)
			{
			case 0:
				fprintf(usedFile, " %d", _Progression->Level1Highscores[EASY][j]);
				break;
			case 1:
				fprintf(usedFile, " %d", _Progression->Level2Highscores[EASY][j]);
				break;
			case 2:
				fprintf(usedFile, " %d", _Progression->Level3Highscores[EASY][j]);
				break;
			default:
				break;
			}
		}

		fprintf(usedFile, "\nLEVEL_%d_NORMAL", (i + 1));
		for (int j = 0; j < 6; j++)
		{
			switch (i)
			{
			case 0:
				fprintf(usedFile, " %d", _Progression->Level1Highscores[NORMAL][j]);
				break;
			case 1:
				fprintf(usedFile, " %d", _Progression->Level2Highscores[NORMAL][j]);
				break;
			case 2:
				fprintf(usedFile, " %d", _Progression->Level3Highscores[NORMAL][j]);
				break;
			default:
				break;
			}
		}

		fprintf(usedFile, "\nLEVEL_%d_HARD", (i + 1));
		for (int j = 0; j < 6; j++)
		{
			switch (i)
			{
			case 0:
				fprintf(usedFile, " %d", _Progression->Level1Highscores[HARD][j]);
				break;
			case 1:
				fprintf(usedFile, " %d", _Progression->Level2Highscores[HARD][j]);
				break;
			case 2:
				fprintf(usedFile, " %d", _Progression->Level3Highscores[HARD][j]);
				break;
			default:
				break;
			}
		}
	}
	fclose(usedFile);
}

int SortHighscores(const void* _a, const void* _b)
{
	return (*(unsigned long*)_b - *(unsigned long*)_a);
}

void SaveFinalScore(MainData* _data, unsigned long _finalScore, char* _levelName)
{
	GOProgression tempProgression;
	LoadProgression(&tempProgression, _data);
	if (!_stricmp(_levelName, "Tutorial.dat"))
	{
		if (tempProgression.progress < 1)
		{
			tempProgression.progress = 1;
		}
	}
	else if (!_stricmp(_levelName, "HumanPlanet.dat"))
	{
		if (tempProgression.progress < 2)
		{
			tempProgression.progress = 2;
		}
		tempProgression.Level1Highscores[_data->engine.currentDifficulty.mode][5] = _finalScore;
		qsort(tempProgression.Level1Highscores[_data->engine.currentDifficulty.mode], 6, sizeof(unsigned long), SortHighscores);
	}
	else if (!_stricmp(_levelName, "Space.dat"))
	{
		if (tempProgression.progress < 3)
		{
			tempProgression.progress = 3;
		}
		tempProgression.Level2Highscores[_data->engine.currentDifficulty.mode][5] = _finalScore;
		qsort(tempProgression.Level2Highscores[_data->engine.currentDifficulty.mode], 6, sizeof(unsigned long), SortHighscores);
	}
	else if (!_stricmp(_levelName, "AlienPlanet.dat"))
	{
		tempProgression.Level3Highscores[_data->engine.currentDifficulty.mode][5] = _finalScore;
		qsort(tempProgression.Level3Highscores[_data->engine.currentDifficulty.mode], 6, sizeof(unsigned long), SortHighscores);
	}
	else
	{
		printf("error while saving end level score\n");
		return;
	}
	SaveProgression(&tempProgression, _data);
}