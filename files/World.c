#include "World.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

// -- SHARED FUNCTIONS ----------------------------------------------------- //

World World_Init(Engine *_engine, const char *_worldFileName)
{
	World world = { 0 };
	ExploDir directoryWorld = { 0 };
	int nbLevels = 0;
	FILE *worldFile = NULL;

	// Access file
	char worldFilepath[BUFFER_LARGE] = WORLD_DIR_PATH;
	strcat(worldFilepath, "/");
	strcat(worldFilepath, _worldFileName);

	// Read file
	worldFile = fopen(worldFilepath, "rt");
	if (worldFile != NULL)
	{
		// Counts how many levels the world is composed of.
		while (!feof(worldFile))
		{
			char tempString[BUFFER_SMALL] = { 0 };
			nbLevels++;
			fscanf(worldFile, "%[^\n]\n", tempString);
		}

		rewind(worldFile);

		// Get level names
		world.levelNames.number = nbLevels;
		world.levelNames.array = calloc(nbLevels, sizeof(char*));

		if (world.levelNames.array != NULL)
		{
			for (int i = 0; i < nbLevels; i++)
			{
				char tempString[BUFFER_SMALL] = { 0 };
				fscanf(worldFile, "%s\n", tempString);
				printf("%s\n", tempString);
				world.levelNames.array[i] = _strdup(tempString);
			}
		}
		else
		{
			printf("Error : World building process failure...");
		}
		fclose(worldFile);
	}

	return world;
}

void World_Create(const char *_levelName, const char *_worldName, int _position)
{
	FILE *worldFile = NULL;
	FILE *tempWorldFile = NULL;
	char tempWorldFilePath[BUFFER_LARGE] = { 0 };

	// Access file
	char worldFilepath[BUFFER_LARGE] = WORLD_DIR_PATH;
	strcat(worldFilepath, "/");
	strcpy(tempWorldFilePath, worldFilepath);
	strcat(worldFilepath, _worldName);
	strcat(tempWorldFilePath, "temp.tmp");

	// Read file
	worldFile = fopen(worldFilepath, "rt");
	// Create temporary file
	tempWorldFile = fopen(tempWorldFilePath, "wt");

	if (worldFile != NULL && tempWorldFile != NULL)
	{
		char tmpString[BUFFER_SMALL] = { 0 };
		int line = 0;

		// Copy every line to the temporary file...
		while (!feof(worldFile))
		{
			line++;
			fscanf(worldFile, "%s\n", tmpString);
			if (line != _position)
			{
				fputs(tmpString, tempWorldFile);
			}
			// ...except for the line chosen to be replaced.
			else
			{
				fputs(_levelName, tempWorldFile);
			}
			if (line < NB_LEVELS)
			{
				fputs("\n", tempWorldFile);
			}
		}

		// Close and save
		fclose(worldFile);
		fclose(tempWorldFile);

		// Delete old data
		remove(worldFilepath);
		// rename temp file to correct name
		rename(tempWorldFilePath, worldFilepath);
	}
	else
	{
		printf("%s doesn't exist...", _worldName);
	}
}

void World_Free(World *_world)
{
	if (_world->levelNames.array != NULL)
	{
		for (int i = 0; i < _world->levelNames.number; i++)
		{
			if (_world->levelNames.array[i] != NULL)
			{
				free(_world->levelNames.array[i]);
				_world->levelNames.array[i] = NULL;
				_world->levelNames.number--;
			}
		}
		free(_world->levelNames.array);
		_world->levelNames.array = NULL;
	}
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //