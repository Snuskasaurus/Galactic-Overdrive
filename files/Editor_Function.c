#include "Editor.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

void ResetSpawnData(MainData *_data);
void Editor_StartLevelTesting(MainData *_data);
void Editor_EndLevelTesting(MainData *_data);
void Editor_DeleteTerrain(MainData* _data);
void Editor_DeleteParallax(MainData* _data);
void Editor_PrintParallaxList(MainData* _data);
void SetSlidersValuesAtLoad(MainData *_data);
char* Editor_removeDotPNG(const char* _imgName, char* _tempChar);
char* ExtractFilenameFromFilepath(char* _filePath);

// -- MAIN FUNCTIONS ------------------------------------------------------- //

void Editor_ChangeWaypointValues(MainData *_data, EditorData *_state)
{
	if (_state->level.paths.selected)
	{
		if (_state->level.paths.selected->type == WAYPOINT_SPAWN)
		{
			Waypoint_Spawn *spawn = _state->level.paths.selected->content;
			// Enemy to spawn
			strcpy(spawn->enemyName, Carousel_GetCurrentName(_state->parameters[TOOLMODE_PATH].carousels.array[0]));
			// Number of enemies to spawn
			spawn->maximum = Slider_GetIntValue(_state->parameters[TOOLMODE_PATH].sliders.array[0]);
			// Starting time
			spawn->timeStart = (float)Slider_GetFloatValue(_state->parameters[TOOLMODE_PATH].sliders.array[1]);
			// Duration
			spawn->duration = (float)Slider_GetFloatValue(_state->parameters[TOOLMODE_PATH].sliders.array[2]);
			// Difficulty
			spawn->difficulty = (int)Slider_GetIntValue(_state->parameters[TOOLMODE_PATH].sliders.array[3]);
		}
	}
}

void Editor_RenameLevel(MainData *_data)
{
	EditorData *state = _data->stateData[EDITOR]; // Create a shortcut to the state data
	state->isEditingLevelName = TRUE;
}

void Editor_NewLevel(MainData *_data)
{
	EditorData *state = _data->stateData[EDITOR]; // Create a shortcut to the state data
}

void Editor_SaveLevel(MainData *_data)
{
	EditorData *state = _data->stateData[EDITOR]; // Create a shortcut to the state data
	ResetSpawnData(_data);
	Path_RecalculateData(&state->level.paths);
	Level_Save(&_data->engine, state->level.name, &state->level, state->listParallax);
	Level_DestroyLevelNames(state->levelList);
	state->levelList = Level_GetNamesInFolder();
	World_Create(state->level.name, state->worldName, state->worldPosition);
	///printf("name at save : %s\n", state->level.bossName);
}

void Editor_LoadLevel(MainData *_data)
{
	EditorData *state = _data->stateData[EDITOR]; // Create a shortcut to the state data
	state->level = Level_Load(state->level.name, sfTrue, _data);
	ResetSpawnData(_data);
	SetSlidersValuesAtLoad(_data);
}

void Editor_DeleteLevel(MainData *_data)
{
	EditorData *state = _data->stateData[EDITOR]; // Create a shortcut to the state data
}

void Editor_TestLevel(MainData *_data)
{
	EditorData *state = _data->stateData[EDITOR]; // Create a shortcut to the state data
	if (state->isTesting == sfTrue)
	{
		Editor_EndLevelTesting(_data);
	}
	else
	{
		Editor_StartLevelTesting(_data);
	}
}

void Editor_PlayLevel(MainData *_data)
{
	EditorData *state = _data->stateData[EDITOR]; // Create a shortcut to the state data
	_data->isLaunchedFromEditor = TRUE;

	Editor_SaveLevel(_data);
	_data->engine.currentDifficulty = SetDifficulty(_data->engine.blueprints.difficultyBlueprint, 1, NORMAL, 1);
	strcpy(_data->levelName, state->level.name);
	_data->currentState = GAME;
}

void Editor_SelectLevelMode(MainData *_data)
{
	EditorData *state = _data->stateData[EDITOR]; // Create a shortcut to the state data
	state->toolModeSelected = TOOLMODE_LEVEL;
}

// MODES

void Editor_SelectPathMode(MainData *_data)
{
	EditorData *state = _data->stateData[EDITOR]; // Create a shortcut to the state data
	state->toolModeSelected = TOOLMODE_PATH;
	state->level.paths.selected = NULL;
}

void Editor_SelectTerrainMode(MainData *_data)
{
	EditorData *state = _data->stateData[EDITOR]; // Create a shortcut to the state data
	state->toolModeSelected = TOOLMODE_TERRAIN;
}

void Editor_SelectParallaxMode(MainData *_data)
{
	EditorData *state = _data->stateData[EDITOR]; // Create a shortcut to the state data
	state->toolModeSelected = TOOLMODE_PARALLAX;
	// Set the default speed at the map scrolling speed value
	Slider_SetFloatValue(state->parameters[TOOLMODE_PARALLAX].sliders.array[PARALLAX_SPEED], state->level.map.scrollingSpeed);
}

// TOOLS

void Editor_SelectSelectTool(MainData *_data)
{
	EditorData *state = _data->stateData[EDITOR]; // Create a shortcut to the state data
	if (state->toolSelected != TOOL_SELECTION && state->toolModeSelected != TOOLMODE_PARALLAX)
	{
		state->toolSelected = TOOL_SELECTION;
	}
	// Deselect the selected Parallax
	else if(state->toolModeSelected == TOOLMODE_PARALLAX && state->selectedParallax != NULL)
	{
		state->toolSelected = TOOL_CREATION;
		state->selectedParallax = NULL;
		Slider_SetFloatValue(state->parameters[TOOLMODE_PARALLAX].sliders.array[PARALLAX_TIME_TO_UPDATE], state->timerTopScreen);
		Slider_SetFloatValue(state->parameters[TOOLMODE_PARALLAX].sliders.array[PARALLAX_SPEED], state->level.map.scrollingSpeed + (state->level.map.scrollingSpeed/3));
		Slider_SetIntValue(state->parameters[TOOLMODE_PARALLAX].sliders.array[PARALLAX_DEPTH], 0);
		Slider_SetIntValue(state->parameters[TOOLMODE_PARALLAX].sliders.array[PARALLAX_OFFSET_X], 0);
	}
}

void Editor_SelectMoveTool(MainData *_data)
{
	EditorData *state = _data->stateData[EDITOR]; // Create a shortcut to the state data
	state->toolSelected = TOOL_MOVE;
}

void Editor_SelectCreateTool(MainData *_data)
{
	EditorData *state = _data->stateData[EDITOR]; // Create a shortcut to the state data
	if (state->toolModeSelected == TOOLMODE_TERRAIN)
	{
		Editor_AddTerrain(_data, Carousel_GetCurrentName(state->parameters[TOOLMODE_TERRAIN].carousels.array[0]));
	}
	else if (state->toolModeSelected == TOOLMODE_PARALLAX)
	{
		Editor_AddParallax(_data);
	}
	else
	{
		state->toolSelected = TOOL_CREATION;
	}
}

void Editor_SelectDeleteTool(MainData *_data)
{
	EditorData *state = _data->stateData[EDITOR]; // Create a shortcut to the state data
	state->toolSelected = TOOL_DELETION;

	if (state->toolModeSelected == TOOLMODE_TERRAIN)
	{
		Editor_DeleteTerrain(_data);
	}
	else if (state->toolModeSelected == TOOLMODE_PARALLAX)
	{
		Editor_DeleteParallax(_data);
	}
}

void Editor_ReturnToMenu(MainData *_data)
{
	EditorData *state = _data->stateData[EDITOR]; // Create a shortcut to the state data
	_data->isLaunchedFromEditor = FALSE;
	_data->currentState = MENU;
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //

void Editor_StartLevelTesting(MainData *_data)
{
	EditorData *state = _data->stateData[EDITOR]; // Create a shortcut to the state data
	ResetSpawnData(_data);
	state->offsetMapY = 0.0f;
	state->level.map.time = 0.0f;
	Path_RecalculateData(&state->level.paths);
	// Start the test
	state->isTesting = sfTrue;
}

void Editor_EndLevelTesting(MainData *_data)
{
	EditorData *state = _data->stateData[EDITOR]; // Create a shortcut to the state data
	// Delete all enemies
	Enemy_DestroyAll(state->enemies, state->lasers);
	// Delete all projectiles
	for (int projectileType = 0; projectileType < WEAPON_OWNER_SIZE; projectileType++)
	{
		List_Destroy(&state->projectiles[projectileType]);
	}
	// End the test
	state->isTesting = sfFalse;
}

void ResetSpawnData(MainData *_data)
{
	EditorData *state = _data->stateData[EDITOR]; // Create a shortcut to the state data
	state->level.paths.selected = NULL;
	for (int i = 0; i < state->level.paths.number; i++)
	{
		Waypoint_Spawn *spawn = state->level.paths.array[i]->first->content;
		spawn->since = 0.0f;
		spawn->spawned = 0;
	}
}

void InitParallaxList(MainData* _data)
{
	EditorData *state = _data->stateData[EDITOR]; // Create a shortcut to the state data
	state->listParallax.first = NULL;
	state->listParallax.last = NULL;
	state->listParallax.number = 0;
}

void Editor_SetBackground(MainData* _data, const char *_name)
{
	EditorData *state = _data->stateData[EDITOR]; // Create a shortcut to the state data
	char filePath[256] = "Data/Sprites/Backgrounds/";

	strcat(filePath, _name);
	if (strcmp(filePath, state->level.map.backgroundFilepath) != 0 || state->level.map.animatedBackground == NULL)
	{
		// Clean up containers
		if (state->level.map.animatedBackground != NULL)
		{
			state->level.map.animatedBackground = NULL;
		}

		// Try to access the animation
		char tempChar[BUFFER_SMALL] = { 0 };
		state->level.map.animatedBackground = GetAnim(_data->engine.anims, Editor_removeDotPNG(_name, tempChar), "loop");
		if (state->level.map.animatedBackground != NULL)
		{
			state->level.map.entityBackground.animationTimer.maxTime = state->level.map.animatedBackground->maxTime;
			state->level.map.entityBackground.animationTimer.isPlaying = sfTrue;
			state->level.map.entityBackground.animationTimer.currentTime = 0;
			state->level.map.entityBackground.currentFrame = 0;
			printf("Animated background selected\n");
			for (int i = 0; i < state->level.map.animatedBackground->nbrFrames; i++)
			{
				sfSprite_setOrigin(state->level.map.animatedBackground->sprites[i], V2F(0, 0));
			}
		}
		else
		{
			printf("Problem while trying to load the animation, animation not found/n");
			printf("Have you made the animation txt file ?/n");
		}

		// Register the filepath for futur use
		strcpy(state->level.map.backgroundFilepath, filePath);
	}
}

void Editor_SetBoss(MainData* _data, const char *_name)
{
	EditorData *state = _data->stateData[EDITOR]; // Create a shortcut to the state data
	if (state->level.bossName)
	{
		free(state->level.bossName);
		strcpy(state->level.bossName, _name);
	}
}

void Editor_AddTerrain(MainData* _data, const char *_name)
{
	EditorData *state = _data->stateData[EDITOR]; // Create a shortcut to the state data
	char filePath[256] = "Data/Sprites/Terrains/";
	// Getting the filePath of the sprite
	strcat(filePath, _name);
	// Alloc
	state->level.map.nbTerrains++;
	state->level.map.terrains = realloc(state->level.map.terrains, sizeof(Terrain)*state->level.map.nbTerrains);

	// Register the filePath
	strcpy(state->level.map.terrains[state->level.map.nbTerrains - 1].filePath, filePath);

	// -- Assign animation or sprite ---------------- //
	char tempChar[BUFFER_SMALL] = { 0 };
	state->level.map.terrains[state->level.map.nbTerrains - 1].animation = GetAnim(_data->engine.anims, Editor_removeDotPNG(_name, tempChar), "loop");
	if (state->level.map.terrains[state->level.map.nbTerrains - 1].animation != NULL)
	{
		// Get size unscaled
		state->level.map.terrains[state->level.map.nbTerrains - 1].size = Sprite_GetSize(state->level.map.terrains[state->level.map.nbTerrains - 1].animation->sprites[0]);
		// Init variables
		state->level.map.terrains[state->level.map.nbTerrains - 1].entity.animationTimer.maxTime = state->level.map.terrains[state->level.map.nbTerrains - 1].animation->maxTime;
		state->level.map.terrains[state->level.map.nbTerrains - 1].entity.animationTimer.currentTime = 0;
		state->level.map.terrains[state->level.map.nbTerrains - 1].entity.animationTimer.isPlaying = sfTrue;
		state->level.map.terrains[state->level.map.nbTerrains - 1].entity.angle = 0;
		for (int i = 0; i < state->level.map.terrains[state->level.map.nbTerrains - 1].animation->nbrFrames; i++)
		{
			sfSprite_setOrigin(state->level.map.terrains[state->level.map.nbTerrains - 1].animation->sprites[i], V2F(0, 0));
		}
	}
	else if (state->level.map.terrains[state->level.map.nbTerrains - 1].animation == NULL)
	{
		// Create the Sprite
		state->level.map.terrains[state->level.map.nbTerrains - 1].sprite = Sprite_CreateFromFile(filePath, 0, 0);
		// Register the size unscaled
		state->level.map.terrains[state->level.map.nbTerrains - 1].size = Sprite_GetSize(state->level.map.terrains[state->level.map.nbTerrains - 1].sprite);
	}

	// Set position Y
	if (state->level.map.nbTerrains > 1)
	{
		state->level.map.terrains[state->level.map.nbTerrains - 1].position.x = 0;
		state->level.map.terrains[state->level.map.nbTerrains - 1].position.y = state->level.map.terrains[state->level.map.nbTerrains - 2].position.y -
			(state->level.map.terrains[state->level.map.nbTerrains - 1].size.y * EDITOR_MAP_SCALE);
	}
	else
	{
		state->level.map.terrains[state->level.map.nbTerrains - 1].position.x = 0;
		state->level.map.terrains[state->level.map.nbTerrains - 1].position.y = (EDITOR_SCREEN_POSITION.y + EDITOR_SCREEN_HEIGHT) - (state->level.map.terrains[state->level.map.nbTerrains - 1].size.y * EDITOR_MAP_SCALE);
	}

	// Reset terrain repetitions
	state->nbTerrainRepetition = 0;

}

void Editor_AddParallaxInList(MainData* _data, MapParallax* _newParallax)
{
	EditorData *state = _data->stateData[EDITOR]; // Create a shortcut to the state data
	if (state->listParallax.number == 0)
	{
		List_InsertElement(&state->listParallax, 0, _newParallax);
	}
	else
	{
		ListElement *current = state->listParallax.first;
		int index = 0;
		while (current)
		{
			MapParallax *currentParallax = current->data;
			if (_newParallax->depth < 0 && _newParallax->depth >= currentParallax->depth || _newParallax->depth >= 0 && _newParallax->depth <= currentParallax->depth)
			{
				if (index == 0)
				{
					List_InsertElement(&state->listParallax, index + 1, _newParallax);
				}
				else
				{
					List_InsertElement(&state->listParallax, index, _newParallax);
				}
				break;
			}
			else if (current->next == NULL && _newParallax->depth >= 0)
			{
				List_InsertElement(&state->listParallax, index + 1, _newParallax);
				break;
			}
			else if (current->next == NULL && _newParallax->depth < 0)
			{
				List_InsertElement(&state->listParallax, 0, _newParallax);
				break;
			}
			index++;
			// Asign the next element
			current = current->next;
		}
	}
}

void Editor_AddParallax(MainData* _data)
{
	EditorData *state = _data->stateData[EDITOR]; // Create a shortcut to the state data
	MapParallax* newParallax = calloc(1, sizeof(MapParallax));
	char filePath[256] = "Data/Sprites/Parallaxes/";
	float speed = Slider_GetFloatValue(state->parameters[TOOLMODE_PARALLAX].sliders.array[1]);
	float depth = (float)Slider_GetIntValue(state->parameters[TOOLMODE_PARALLAX].sliders.array[2]);
	float angle = Slider_GetFloatValue(state->parameters[TOOLMODE_PARALLAX].sliders.array[PARALLAX_ANGLE]);
	int offsetX = Slider_GetIntValue(state->parameters[TOOLMODE_PARALLAX].sliders.array[PARALLAX_OFFSET_X]);

	// -- Create new parallax ---------
	// Getting the filePath
	strcat(filePath, Carousel_GetCurrentName(state->parameters[TOOLMODE_PARALLAX].carousels.array[0]));

	// Animation 
	char tempChar[BUFFER_SMALL] = { 0 };
	newParallax->animation = GetAnim(_data->engine.anims, Editor_removeDotPNG(Carousel_GetCurrentName(state->parameters[TOOLMODE_PARALLAX].carousels.array[0]), tempChar), "loop");
	if (newParallax->animation != NULL)
	{
		// Size unscaled
		newParallax->size = Sprite_GetSize(newParallax->animation->sprites[0]);
		// Set TimeMax
		newParallax->entity.animationTimer.maxTime = newParallax->animation->maxTime;
		newParallax->entity.animationTimer.isPlaying = sfTrue;
		for (int i = 0; i < newParallax->animation->nbrFrames; i++)
		{
			sfSprite_setOrigin(newParallax->animation->sprites[i], V2F(0, 0));
		}
	}
	// Sprite
	else if (newParallax->animation == NULL)
	{
		newParallax->sprite = Sprite_CreateFromFile(filePath, 0, 0);
		// Size unscaled
		newParallax->size = Sprite_GetSize(newParallax->sprite);
	}
	// Filepath
	strcpy(newParallax->filePath, filePath);
	// Update start condition timer
	newParallax->timerToUpdate = state->timerTopScreen;
	// Starting position
	newParallax->startingPosition.x = (float)offsetX;
	newParallax->startingPosition.y =
		EDITOR_SCREEN_POSITION.y -
		newParallax->timerToUpdate * (state->level.map.scrollingSpeed / EDITOR_MAP_ZOOM) -
		newParallax->size.y * EDITOR_MAP_SCALE;
	// Speed
	newParallax->speed = speed;
	// Depth
	newParallax->depth = (int)depth;
	// Angle 
	newParallax->entity.angle = angle;

	// -- Add newParallax to the list -------
	Editor_AddParallaxInList(_data, newParallax);
}

void Editor_UpdateTimerTopScreen(MainData* _data)
{
	/// Converting position into time : posY/speed = Time
	EditorData *state = _data->stateData[EDITOR]; // Create a shortcut to the state data
	state->timerTopScreen = state->offsetMapY / (state->level.map.scrollingSpeed / EDITOR_MAP_ZOOM);
	state->level.map.time = state->timerTopScreen;
}

void Editor_SelectParallax(MainData *_data)
{
	EditorData *state = _data->stateData[EDITOR]; // Create a shortcut to the state data
	MapParallax* selectedParallax = state->selectedParallax->data;
	Slider_SetFloatValue(state->parameters[TOOLMODE_PARALLAX].sliders.array[PARALLAX_TIME_TO_UPDATE], selectedParallax->timerToUpdate);
	Slider_SetFloatValue(state->parameters[TOOLMODE_PARALLAX].sliders.array[PARALLAX_SPEED], selectedParallax->speed);
	Slider_SetIntValue(state->parameters[TOOLMODE_PARALLAX].sliders.array[PARALLAX_DEPTH], selectedParallax->depth);
	Slider_SetIntValue(state->parameters[TOOLMODE_PARALLAX].sliders.array[PARALLAX_OFFSET_X], (int)selectedParallax->startingPosition.x);
	Slider_SetFloatValue(state->parameters[TOOLMODE_PARALLAX].sliders.array[PARALLAX_ANGLE], (int)selectedParallax->entity.angle);
}

void Editor_SelectParallaxOrTerrain(MainData* _data, sfVector2f _mousePos)
{
	EditorData *state = _data->stateData[EDITOR]; // Create a shortcut to the state data

	// -- PARALLAX SELECTORS ----------------------------------------
	// -- Loop over every Parallax
	ListElement *current = state->listParallax.first;
	while (current)
	{
		MapParallax *currentParallax = current->data;

		sfVector2f selectorPos;
		selectorPos.x = EDITOR_MAP_BACKGROUND_POSITION + EDITOR_SCREEN_WIDTH;
		selectorPos.y = currentParallax->position.y + currentParallax->size.y*EDITOR_MAP_SCALE - EDITOR_SELECTOR_HEIGHT;

		RectangleCollider tempRectangle;
		tempRectangle.centered = sfFalse;
		tempRectangle.position.x = selectorPos.x;
		tempRectangle.position.y = selectorPos.y;
		tempRectangle.size.x = EDITOR_SELECTOR_WIDTH;
		tempRectangle.size.y = EDITOR_SELECTOR_HEIGHT;

		if (IsColliding_Point_Rectangle(_mousePos, tempRectangle))
		{
			// -- Point to the Parallax struct ------------------
			state->selectedParallax = current;
			Editor_SelectParallax(_data);

			// -- Create a selectionMask ------------------------
			MapParallax* selectedParallax = state->selectedParallax->data;
			sfColor color = { 200,200,200,125 };
			// Destroy Image
			if (state->selectionMaskImage != NULL)
			{
				sfImage_destroy(state->selectionMaskImage);
			}
			// Destroy texture
			sfTexture_destroy(sfSprite_getTexture(state->selectionMask));
			// Create Image
			state->selectionMaskImage = sfImage_createFromColor((int)selectedParallax->size.x, (int)selectedParallax->size.y, color);
			// Create Texture
			sfTexture* tempTexture = sfTexture_createFromImage(state->selectionMaskImage, NULL);
			sfSprite_setTexture(state->selectionMask, tempTexture, sfTrue);
		}

		// Asign the next element
		current = current->next;
	}

	// -- TERRAIN SELECTORS ------------------------------------------
	// -- Loop over every Terrain
	for (int i = 0; i < state->level.map.nbTerrains; i++)
	{
		sfVector2f selectorPos;
		RectangleCollider tempRectangle;
		
		selectorPos.x = EDITOR_MAP_BACKGROUND_POSITION - EDITOR_SELECTOR_WIDTH;
		selectorPos.y = state->level.map.terrains[i].position.y + state->level.map.terrains[i].size.y*EDITOR_MAP_SCALE - EDITOR_SELECTOR_HEIGHT + state->offsetMapY;
		tempRectangle.centered = sfFalse;
		tempRectangle.position.x = selectorPos.x;
		tempRectangle.position.y = selectorPos.y;
		tempRectangle.size.x = EDITOR_SELECTOR_WIDTH;
		tempRectangle.size.y = EDITOR_SELECTOR_HEIGHT;

		if (IsColliding_Point_Rectangle(_mousePos, tempRectangle))
		{
			state->selectedTerrain = &state->level.map.terrains[i];
			Editor_SelectSelectTool(_data);
		}
	}
}

void Editor_DeleteTerrain(MainData* _data)
{
	EditorData *state = _data->stateData[EDITOR]; // Create a shortcut to the state data

	if (state->selectedTerrain != NULL)
	{
		int heightMapToDelete = (int)(state->selectedTerrain->size.y * EDITOR_MAP_SCALE);
		// Looking for the selected terrain adress
		for (int i = 0; i < state->level.map.nbTerrains; i++)
		{
			if (&state->level.map.terrains[i] == state->selectedTerrain)
			{
				// Adress found !
				// From there, move every map one index lower
				for (int j = i; j < state->level.map.nbTerrains - 1; j++)
				{
					state->level.map.terrains[j] = state->level.map.terrains[j + 1];
					// Lower the Y position
					state->level.map.terrains[j].position.y += heightMapToDelete;
				}

				// -- Soft break;
				i = state->level.map.nbTerrains;
				state->level.map.nbTerrains--;
				state->selectedTerrain = NULL;
			}
		}
	}
	else
	{
		///printf("First, select a terrain to delete\n");
	}
}

void Editor_DeleteParallax(MainData* _data)
{
	EditorData *state = _data->stateData[EDITOR]; // Create a shortcut to the state data
	if (state->selectedParallax != NULL)
	{
		List_DestroyElementFromPointer(&state->listParallax, state->selectedParallax, sfTrue);
		state->selectedParallax = NULL;
	}
	else
	{
		///printf("First, select a parallax to delete\n");
	}
}

void Editor_PrintParallaxList(MainData* _data)
{
	EditorData *state = _data->stateData[EDITOR]; // Create a shortcut to the state data
	List* list = &state->listParallax;
	ListElement *current = list->first;
	int index = 0;
	///printf("----------------------------- LINKED LIST ------------------------------\n");
	///printf("%d ELEMENTS\n", list->number);
	if (list->first)
	{
		///printf("ID\tTIME_TO_UPDATE\tSPEED\tDEPTH\n");
		while (current)
		{
			MapParallax* parallax = current->data;
			///printf("%d\t%f\t%f\t%d\n",index, parallax->timerToUpdate, parallax->speed, parallax->depth);
			current = current->next;
			index++;
		}
	}
	///printf("------------------------------------------------------------------------\n");
}

void SetSlidersValuesAtLoad(MainData *_data)
{
	EditorData *state = _data->stateData[EDITOR]; // Create a shortcut to the state data
	Carousel_SetCurrentName(state->parameters[TOOLMODE_LEVEL].carousels.array[0], state->worldName);
	Carousel_SetCurrentName(state->parameters[TOOLMODE_LEVEL].carousels.array[1], ExtractFilenameFromFilepath(state->level.map.backgroundFilepath));
	Carousel_SetCurrentName(state->parameters[TOOLMODE_LEVEL].carousels.array[2], state->level.bossName);
	Carousel_SetCurrentName(state->parameters[TOOLMODE_LEVEL].carousels.array[3], state->level.musicName);
	Slider_SetIntValue(state->parameters[TOOLMODE_LEVEL].sliders.array[0], state->worldPosition);
	Slider_SetFloatValue(state->parameters[TOOLMODE_LEVEL].sliders.array[1], state->level.bossTime);
	Slider_SetIntValue(state->parameters[TOOLMODE_LEVEL].sliders.array[2], state->level.isInSpace);
	Slider_SetIntValue(state->parameters[TOOLMODE_LEVEL].sliders.array[3], state->level.terrainsAreRepeated);
	state->level.map.animatedBackground = NULL;
	///printf("name at load : %s\n", state->level.bossName);
}

// Simply remove the ".png" at the end of the file name
// NOTE : To not destroy the char* we made, you have to bring a _tempChar to store it
// BE AWARE : There is no security to check if _imgName is a ".png"
char* Editor_removeDotPNG(const char* _imgName, char* _tempChar)
{
	int size = (int)strlen(_imgName);
	strncpy(_tempChar, _imgName, size - 4);
	return _tempChar;
}

// Extract the fileName with his format (".png" for exemple)
// In this path : "directory/subDirectory/FILENAME.png" --> "FILENAME.png" will be extracted
char* ExtractFilenameFromFilepath(char* _filePath)
{
	char* fileName = NULL;
	int sizeFilePath = (int)strlen(_filePath);
	int sizeFileName = 0;
	int indexFileName = sizeFilePath;
	while (_filePath[indexFileName] != '/')
	{
		indexFileName--;
		sizeFileName++;
	}
	fileName = &_filePath[indexFileName + 1];
	return fileName;
}

// -- Buttons Change state Functions -----------------------

void EditorFade_EditorToMenu(MainData *_data)
{
	Fade_CreateTransition(&_data->transitionFade, &Editor_ReturnToMenu);
}

void EditorFade_EditorToGame(MainData *_data)
{
	Fade_CreateTransition(&_data->transitionFade, &Editor_PlayLevel);
}