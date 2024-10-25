#include "Level.h"
#include "List.h"
#include "Editor.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //


void Level_SavePaths(FILE *_file, Level *_level);
void Level_SaveMap(FILE *_file, Level *_level, List* _listParallax);
void Level_LoadPath(FILE *_file, Level *_level, sfBool _isInEditor);
void Level_LoadMap(FILE *_file, Level *_level, sfBool _isInEditor, MainData* _data);

// -- SHARED FUNCTIONS ----------------------------------------------------- //

void Level_Save(Engine *_engine, const char *_filename, Level *_level, List _listParallax)
{
	FILE *file = NULL;
	char filepath[BUFFER_LARGE] = LEVEL_FOLDER;
	int sizeString = 0;

	strcat(filepath, _filename);
	strcat(filepath, ".bin");
	file = fopen(filepath, "wb+");
	// Right the file
	if (file)
	{
		fwrite(_level, 1, sizeof(Level), file);
		Level_SavePaths(file, _level);
		Level_SaveMap(file, _level, &_listParallax);
		// write boss name
		sizeString = (int)(strlen(_level->bossName) + 1);
		fwrite(&sizeString, 1, sizeof(int), file);
		fwrite(_level->bossName, sizeString, sizeof(char), file);
		fwrite(&_level->bossTime, sizeof(float), 1, file);
		fclose(file);
	}
}

Level *Level_Init(int _nbLevels)
{
	Level *levels = NULL;

	levels = calloc(_nbLevels, sizeof(Level));
	if (!levels)
	{
		printf("Error : level init\n");
	}

	return levels;
}

Level Level_Load(const char *_filename, sfBool _isInEditor, MainData* _data)
{
	Level level;
	FILE *file = NULL;
	char filepath[BUFFER_LARGE] = LEVEL_FOLDER;
	int sizeString = 0;
	char temp[BUFFER_LARGE] = { 0 };

	strcat(filepath, _filename);
	strcat(filepath, ".bin");
	// Read the file
	file = fopen(filepath, "rb");
	if (file)
	{
		fread(&level, sizeof(Level), 1, file);
		level.paths = Path_Initialize();
		Level_LoadPath(file, &level, _isInEditor);
		Level_LoadMap(file, &level, _isInEditor, _data);
		// Read boss name
		fread(&sizeString, 1, sizeof(int), file);
		fread(temp, sizeof(char), sizeString, file);
		strcpy(level.bossName, temp);
		///level.bossName = _strdup(temp);
		fread(&level.bossTime, sizeof(float), 1, file);
		fclose(file);
	}

	// Adjust for difficulty if not in editor.
	if (_isInEditor == sfFalse)
	{
		Path_SetDifficulty(&level.paths, _data->engine.currentDifficulty.enemiesSpawnMultiplier);
	}

	return level;
}

LevelNames *Level_GetNamesInFolder()
{
	LevelNames *levels = calloc(1, sizeof(LevelNames));
	ExploDir dirrectory = ExploreDirectory(LEVEL_FOLDER);
	if (levels)
	{
		levels->number = dirrectory.nbrFiles;
		levels->array = calloc(levels->number, sizeof(char*));
		for (int i = 0; i < levels->number; i++)
		{
			levels->array[i] = _strdup(dirrectory.fileNames[i]);
		}
	}
	return levels;
}

void Level_DestroyLevelNames(LevelNames *_levelNames)
{
	for (int i = 0; i < _levelNames->number; i++)
	{
		free(_levelNames->array[i]);
	}
	free(_levelNames);
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //
void Level_SavePaths(FILE *_file, Level *_level)
{
	fwrite(&_level->paths.number, sizeof(int), 1, _file);
	for (int i = 0; i < _level->paths.number; i++)
	{
		fwrite(&_level->paths.array[i]->number, sizeof(int), 1, _file);
		Waypoint *current = _level->paths.array[i]->first;
		while (current)
		{
			sfVector2f position;
			position.x = current->position.x - EDITOR_SCREEN_POSITION.x;
			position.y = current->position.y - EDITOR_SCREEN_POSITION.y;
			fwrite(&position, sizeof(sfVector2f), 1, _file);
			fwrite(&current->type, sizeof(WaypointType), 1, _file);
			switch (current->type)
			{
			default:
				break;
			case WAYPOINT_SPAWN:
			{
				Waypoint_Spawn *spawn = current->content;
				int sizeName = 0;
				fwrite(current->content, sizeof(Waypoint_Spawn), 1, _file);
				// Write the enemy name
				sizeName = (int)(strlen(spawn->enemyName) + 1);
				fwrite(&sizeName, 1, sizeof(int), _file);
				fwrite(&spawn->enemyName, sizeName, sizeof(char), _file);
				break;
			}
			}
			current = current->next;
		}
	}
}

void Level_SaveMap(FILE *_file, Level *_level, List* _listParallax)
{
	// -- Get nb Background && Foreground
	ListElement *current = _listParallax->first;
	_level->map.nbOfBckgrndParallax = 0;
	_level->map.nbOfForgrndParallax = 0;
	printf("List Total : %d\n", _listParallax->number);
	while (current)
	{
		MapParallax *currentParallax = current->data;
		if (currentParallax->depth < 0)
		{
			_level->map.nbOfBckgrndParallax++;
			printf("nbOfBckgrndParallax++	Total : %d\n", _level->map.nbOfBckgrndParallax);
		}
		else
		{
			_level->map.nbOfForgrndParallax++;
			printf("nbOfForgrndParallax++	Total : %d\n", _level->map.nbOfForgrndParallax);
		}
		// Asign the next element
		current = current->next;
	}

	printf("nb of parallax %d\n", (int)(_level->map.nbOfForgrndParallax + _level->map.nbOfBckgrndParallax));

	// Save the level without his pointers
	fwrite(&_level->map, sizeof(Map), 1, _file);
	// Save each terrain
	for (int i = 0; i < _level->map.nbTerrains; i++)
	{
		fwrite(&_level->map.terrains[i], sizeof(Terrain), 1, _file);
	}
	// -- Save parallaxes ------------------------
	// Background parallax
	current = _listParallax->first;
	while (current)
	{
		MapParallax *currentParallax = current->data;
		if (currentParallax->depth < 0)
		{
			fwrite(currentParallax, sizeof(MapParallax), 1, _file);
		}
		current = current->next;
	}
	// Foreground parallax 
	current = _listParallax->first;
	while (current)
	{
		MapParallax *currentParallax = current->data;
		if (currentParallax->depth >= 0)
		{
			fwrite(currentParallax, sizeof(MapParallax), 1, _file);
			printf("Save at offset %f\n", currentParallax->startingPosition.x);
		}
		current = current->next;
	}
}

void Level_LoadPath(FILE *_file, Level *_level, sfBool _isInEditor)
{
	int numberPaths;
	int numberWaypoints;
	fread(&numberPaths, sizeof(int), 1, _file);
	for (int i = 0; i < numberPaths; i++)
	{
		fread(&numberWaypoints, sizeof(int), 1, _file);
		for (int n = 0; n < numberWaypoints; n++)
		{
			sfVector2f position;
			WaypointType type;
			Waypoint *waypoint = NULL;
			fread(&position, sizeof(sfVector2f), 1, _file);
			fread(&type, sizeof(WaypointType), 1, _file);
			if (_isInEditor)
			{
				position.x += EDITOR_SCREEN_POSITION.x;
				position.y += EDITOR_SCREEN_POSITION.y;
			}
			else
			{
				position.x *= EDITOR_MAP_ZOOM;
				position.y *= EDITOR_MAP_ZOOM;
			}
			waypoint = _Waypoint_Create(&_level->paths, type, position);
			switch (waypoint->type)
			{
			default:
				break;
			case WAYPOINT_SPAWN:
			{
				Waypoint_Spawn *spawn = NULL;
				int sizeName = 0;
				waypoint->content = calloc(1, sizeof(Waypoint_Spawn));
				fread(waypoint->content, sizeof(Waypoint_Spawn), 1, _file);
				spawn = waypoint->content;
				fread(&sizeName, 1, sizeof(int), _file);
				fread(spawn->enemyName, sizeName, sizeof(char), _file);
				break;
			}
			}
		}
	}
	Path_RecalculateData(&_level->paths);
}

void Level_TryToLoadAnimation(Animations* _anims, MapType _mapType, void* _mapElement)
{
	char tempChar[BUFFER_SMALL] = { 0 };
	if (_mapType == MAP_BACKGROUND)
	{
		Map* map = _mapElement;

		map->animatedBackground = GetAnim(_anims, Editor_removeDotPNG(ExtractFilenameFromFilepath(map->backgroundFilepath), tempChar), "loop");
		map->entityBackground.animationTimer.currentTime = 0;
		if (map->animatedBackground != NULL)
		{
			map->entityBackground.animationTimer.maxTime = map->animatedBackground->maxTime;
			map->entityBackground.animationTimer.isPlaying = sfTrue;
			map->entityBackground.angle = 0;
			for (int i = 0; i < map->animatedBackground->nbrFrames; i++)
			{
				sfSprite_setOrigin(map->animatedBackground->sprites[i], V2F(0, 0));
			}
			printf("Animation loaded\n");
		}
		else
		{
			printf("Error while trying to load animation, animation not found\n");
		}
	}
	else if (_mapType == MAP_TERRAIN)
	{
		Terrain* currentTerrain = _mapElement;

		currentTerrain->animation = NULL;
		currentTerrain->animation = GetAnim(_anims, Editor_removeDotPNG(ExtractFilenameFromFilepath(currentTerrain->filePath), tempChar), "loop");
		if (currentTerrain->animation != NULL)
		{
			currentTerrain->entity.animationTimer.maxTime = currentTerrain->animation->maxTime;
			currentTerrain->entity.animationTimer.currentTime = 0;
			currentTerrain->entity.animationTimer.isPlaying = sfTrue;
			currentTerrain->entity.angle = 0;
			currentTerrain->isSpriteLoaded = 1;
			for (int i = 0; i < currentTerrain->animation->nbrFrames; i++)
			{
				sfSprite_setOrigin(currentTerrain->animation->sprites[i], V2F(0, 0));
			}
			printf("Animation loaded\n");
		}
		else
		{
			currentTerrain->sprite = Sprite_CreateFromFile(currentTerrain->filePath, 0, 0);
			currentTerrain->isSpriteLoaded = 1;
			printf("Sprite loaded\n");
		}
	}
	else if (_mapType == MAP_PARALLAX)
	{
		MapParallax* tempPara = _mapElement;

		tempPara->animation = GetAnim(_anims, Editor_removeDotPNG(ExtractFilenameFromFilepath(tempPara->filePath), tempChar), "loop");
		if (tempPara->animation != NULL)
		{
			// Size unscaled
			tempPara->size = Sprite_GetSize(tempPara->animation->sprites[0]);
			// Set TimeMax
			tempPara->entity.animationTimer.maxTime = tempPara->animation->maxTime;
			tempPara->entity.animationTimer.isPlaying = sfTrue;
			for (int i = 0; i < tempPara->animation->nbrFrames; i++)
			{
				sfSprite_setOrigin(tempPara->animation->sprites[i], V2F(0, 0));
			}
			printf("Animation loaded\n");
		}
		// Sprite
		else if (tempPara->animation == NULL)
		{
			tempPara->sprite = Sprite_CreateFromFile(tempPara->filePath, 0, 0);
			// Size unscaled
			tempPara->size = Sprite_GetSize(tempPara->sprite);
			printf("Sprite loaded\n");
		}
	}
}

void Level_LoadMap(FILE *_file, Level *_level, sfBool _isInEditor, MainData* _data)
{
	if (_isInEditor)
	{
		EditorData* state = _data->stateData[EDITOR];
		List_Destroy(&state->listParallax);
	}
	// Load the entire map struct
	fread(&_level->map, sizeof(Map), 1, _file);

	// Init our dynamic tables
	_level->map.time = 0.0f;
	_level->map.terrains = (Terrain*)calloc(_level->map.nbTerrains, sizeof(Terrain));
	_level->map.tabParallaxBckgrnd = (MapParallax*)calloc(_level->map.nbOfBckgrndParallax, sizeof(MapParallax));
	_level->map.tabParallaxForgrnd = (MapParallax*)calloc(_level->map.nbOfForgrndParallax, sizeof(MapParallax));

	// Load background sprite
	printf("\n-------------------------------------\nLoading background...\n");
	Level_TryToLoadAnimation(_data->engine.anims, MAP_BACKGROUND, &_level->map);
	_level->map.entityBackground.position.x = 0;
	_level->map.entityBackground.position.y = 0;

	if (_isInEditor == sfTrue)
	{
		EditorData* state = _data->stateData[EDITOR];
		Carousel_SetCurrentName(state->parameters[TOOLMODE_LEVEL].carousels.array[0], ExtractFilenameFromFilepath(_level->map.backgroundFilepath));
	}

	// Load each terrain
	printf("\nLoading terrain... (%d)\n", _level->map.nbTerrains);
	for (int i = 0; i < _level->map.nbTerrains; i++)
	{
		fread(&_level->map.terrains[i], sizeof(Terrain), 1, _file);
		Terrain* currentTerrain = &_level->map.terrains[i];
		// -- In Editor ----------------------------------------------- //
		if (_isInEditor == sfTrue)
		{
			Level_TryToLoadAnimation(_data->engine.anims, MAP_TERRAIN, currentTerrain);
		}
		// -- In game ------------------------------------------------- //
		else
		{
			if (i < 2)
			{
				Level_TryToLoadAnimation(_data->engine.anims, MAP_TERRAIN, currentTerrain);
			}
			else
			{
				_level->map.terrains[i].isSpriteLoaded = 0;
				printf("%d - Sprite filepath registered but kept unloaded\n", i);
			}
			if (i == 0)
			{
				// Set Y
				_level->map.terrains[i].position.x = 0;
				_level->map.terrains[i].position.y = _data->engine.configuration->resolution.y - _level->map.terrains[i].size.y*MAP_SCALE;
			}
			else
			{
				// Set Y
				_level->map.terrains[i].position.y = _level->map.terrains[i - 1].position.y - _level->map.terrains[i].size.y*MAP_SCALE;
			}
		}
	}

	// -- Load parallaxes ----------------------------------------------- //
	printf("\nLoading parallaxes...(%d)\n", _level->map.nbOfBckgrndParallax + _level->map.nbOfForgrndParallax);
	if (_isInEditor == sfTrue)
	{
		EditorData* state = _data->stateData[EDITOR];
		for (int i = 0; i < _level->map.nbOfBckgrndParallax + _level->map.nbOfForgrndParallax; i++)
		{
			MapParallax* tempPara = calloc(1, sizeof(MapParallax));
			fread(tempPara, sizeof(MapParallax), 1, _file);

			Level_TryToLoadAnimation(_data->engine.anims, MAP_PARALLAX, tempPara);
			List_InsertElement(&state->listParallax, i, (MapParallax*)tempPara);
		}
	}
	else
	{
		// Load each background parallax
		printf("Loading background parallaxes...(%d)\n", _level->map.nbOfBckgrndParallax);
		for (int i = 0; i < _level->map.nbOfBckgrndParallax; i++)
		{
			fread(&_level->map.tabParallaxBckgrnd[i], sizeof(MapParallax), 1, _file);
			_level->map.tabParallaxBckgrnd[i].isSpriteLoaded = 0;
			_level->map.tabParallaxBckgrnd[i].startingPosition.x *= EDITOR_MAP_ZOOM;
			_level->map.tabParallaxBckgrnd[i].position.x = 0;
			printf("%d - Loaded\n", i);
		}
		// Load each foreground parallax
		printf("Loading foreground parallaxes...(%d)\n", _level->map.nbOfForgrndParallax);
		for (int i = 0; i < _level->map.nbOfForgrndParallax; i++)
		{
			fread(&_level->map.tabParallaxForgrnd[i], sizeof(MapParallax), 1, _file);
			_level->map.tabParallaxForgrnd[i].isSpriteLoaded = 0;
			_level->map.tabParallaxForgrnd[i].startingPosition.x *= EDITOR_MAP_ZOOM;
			_level->map.tabParallaxForgrnd[i].position.x = 0;
			printf("%d - Loaded\n", i);
		}
	}

	printf("-------------------------------------\n");
}
