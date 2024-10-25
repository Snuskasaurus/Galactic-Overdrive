#include "Editor.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

void DisplayParameters(MainData *_data, EditorData *_state);
void DisplayTextLevels(MainData *_data);
void DisplayLevelMaps(MainData *_data);
void DisplayTextTimeline(MainData *_data);
void DisplayLevelParallaxForeground(MainData* _data);
void DisplaySelectors(MainData* _data);

// -- MAIN FUNCTION -------------------------------------------------------- //

void Editor_Display(MainData *_data)
{
	EditorData *state = _data->stateData[EDITOR]; // Create a shortcut to the state data
	DisplayLevelMaps(_data);
	Projectile_Display(&_data->engine, state->projectiles);
	Enemy_Display(&_data->engine, state->enemies);
	DisplayLevelParallaxForeground(_data);
	Sprite_Display(_data->engine.canvas, state->background, V2F_FROM_VALUE(0.0f), V2F_FROM_VALUE(1.0f), 0, NULL);
	DisplaySelectors(_data);
	Button_Display(&_data->engine, &state->buttons);
	Path_Display(&_data->engine, &state->level.paths, state->timerTopScreen);
	DisplayTextLevels(_data);
	DisplayTextTimeline(_data);
	DisplayParameters(_data, state);
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //

void DisplayParameters(MainData *_data, EditorData *_state)
{
	Carousel_Display(&_data->engine, &_state->parameters[_state->toolModeSelected].carousels);
	Slider_Display(&_data->engine, &_state->parameters[_state->toolModeSelected].sliders);
}

void DisplayTextLevels(MainData *_data)
{
	EditorData *state = _data->stateData[EDITOR]; // Create a shortcut to the state data
	for (int i = 0; i < state->levelList->number; i++)
	{
		Text_Display(_data->engine.canvas, &state->parametersText, state->levelList->array[i], V2F(50.0f, 175.0f + i * 35.0f), V2F_NULL);
	}
}

// Display the position in time of the top player screen
void DisplayTextTimeline(MainData *_data)
{
	EditorData *state = _data->stateData[EDITOR]; // Create a shortcut to the state data
	char second[256];
	sprintf(second, "%.1f sec", state->timerTopScreen);
	Text_Display(_data->engine.canvas, &state->parametersText, second, V2F(EDITOR_SCREEN_POSITION.x - 60, EDITOR_SCREEN_POSITION.y), V2F(0.5, 0.5));
}

void DisplaySelectionMask(MapParallax* _currentParallax, sfSprite* _sprite, float _dt, sfRenderTexture* _canvas)
{
	static float time = 0;
	static int alpha = 0;
	static int doIncrease = 1;
	time += _dt;

	if (doIncrease == 1)
	{
		alpha = (int)(EDITOR_SLECTION_MASK_ALPHA_MIN + (time * 50));
		if (alpha >= EDITOR_SLECTION_MASK_ALPHA_MAX)
		{
			alpha = EDITOR_SLECTION_MASK_ALPHA_MAX;
			time = 0;
			doIncrease = 0;
		}
	}
	else
	{
		alpha = (int)(EDITOR_SLECTION_MASK_ALPHA_MAX - (time * 50));
		if (alpha <= EDITOR_SLECTION_MASK_ALPHA_MIN)
		{
			alpha = EDITOR_SLECTION_MASK_ALPHA_MIN;
			doIncrease = 1;
			time = 0;
		}
	}
	sfColor tempColor = { 255,255,255, alpha };
	sfSprite_setColor(_sprite, tempColor);
	Sprite_Display(_canvas, _sprite, V2F(EDITOR_MAP_BACKGROUND_POSITION + _currentParallax->startingPosition.x, _currentParallax->position.y), V2F_FROM_VALUE(EDITOR_MAP_SCALE), _currentParallax->entity.angle, NULL);
}

void DisplayParallax(MainData *_data, EditorData *_state, MapParallax* _currentParallax)
{
	// Position calculation
	if (_state->timerTopScreen >= _currentParallax->timerToUpdate)
	{
		_currentParallax->position.y = _currentParallax->startingPosition.y +
			(_state->timerTopScreen - _currentParallax->timerToUpdate)*(_currentParallax->speed / EDITOR_MAP_ZOOM) +
			_currentParallax->timerToUpdate * (_state->level.map.scrollingSpeed / EDITOR_MAP_ZOOM);
	}
	else
	{
		_currentParallax->position.y = _currentParallax->startingPosition.y + _state->offsetMapY;
	}

	// Display Animation
	if (_currentParallax->animation != NULL)
	{
		_currentParallax->entity.position.x = EDITOR_MAP_BACKGROUND_POSITION + _currentParallax->startingPosition.x;
		_currentParallax->entity.position.y = _currentParallax->position.y;
		DisplayAnimation(_data->engine.canvas, _currentParallax->animation, &_currentParallax->entity, V2F_FROM_VALUE(EDITOR_MAP_SCALE), NULL);
	}
	// Display Sprite
	else
	{
		Sprite_Display(_data->engine.canvas, _currentParallax->sprite, V2F(EDITOR_MAP_BACKGROUND_POSITION + _currentParallax->startingPosition.x, _currentParallax->position.y), V2F_FROM_VALUE(EDITOR_MAP_SCALE), _currentParallax->entity.angle, NULL);
	}

	// Display highlight if selected
	if (_state->selectedParallax != NULL && _currentParallax == _state->selectedParallax->data)
	{
		DisplaySelectionMask(_currentParallax, _state->selectionMask, _data->engine.dt, _data->engine.canvas);
	}
}

void Editor_DisplayLastTerrainsRepeatedly(MainData *_data)
{
	EditorData *state = _data->stateData[EDITOR]; // Create a shortcut to the state data
	Terrain* lastTerrain = &state->level.map.terrains[state->level.map.nbTerrains - 1];


	// Init TerrainRepetition
	if (state->nbTerrainRepetition == 0)
	{
		state->tabTerrainRepetition[0].position.y = lastTerrain->position.y - lastTerrain->size.y * EDITOR_MAP_SCALE;
		state->nbTerrainRepetition++;
		for (int i = 1; i < 3; i++)
		{
			state->tabTerrainRepetition[i].position.x = 0;
			state->tabTerrainRepetition[i].position.y = state->tabTerrainRepetition[i - 1].position.y - lastTerrain->size.y * EDITOR_MAP_SCALE;
			state->nbTerrainRepetition++;
		}
	}

	// Add repeted Terrain to Tab
	if (state->tabTerrainRepetition[state->nbTerrainRepetition - 3].position.y + state->offsetMapY > EDITOR_SCREEN_POSITION.y)
	{
		if (state->nbTerrainRepetition + 1 <= state->nbMaxTerrainRepetition)
		{
			state->tabTerrainRepetition[state->nbTerrainRepetition].position.y = state->tabTerrainRepetition[state->nbTerrainRepetition - 1].position.y - lastTerrain->size.y * EDITOR_MAP_SCALE;
			state->nbTerrainRepetition++;
		}
		else
		{
			printf("Error, tabTerrainRepetion is full\n");
		}
	}

	// Case : Animated terrain
	if (lastTerrain->animation != NULL)
	{
		for (int i = 0; i < state->nbTerrainRepetition; i++)
		{
			state->tabTerrainRepetition[i].entity.position.x = EDITOR_MAP_BACKGROUND_POSITION + state->tabTerrainRepetition[i].position.x;
			state->tabTerrainRepetition[i].entity.position.y = state->tabTerrainRepetition[i].position.y + state->offsetMapY;
			state->tabTerrainRepetition[i].entity.currentFrame = lastTerrain->entity.currentFrame;

			DisplayAnimation(_data->engine.canvas, lastTerrain->animation, &state->tabTerrainRepetition[i].entity, V2F_FROM_VALUE(EDITOR_MAP_SCALE), NULL);
		}
	}
	// Case : Terrain Sprite
	else
	{
		// Display Terrains
		for (int i = 0; i < state->nbTerrainRepetition; i++)
		{
			Sprite_Display(_data->engine.canvas, state->level.map.terrains[state->level.map.nbTerrains - 1].sprite, V2F(EDITOR_MAP_BACKGROUND_POSITION, state->tabTerrainRepetition[i].position.y + state->offsetMapY), V2F_FROM_VALUE(EDITOR_MAP_SCALE), 0, NULL);
		}
	}
}

void DisplayLevelMaps(MainData *_data)
{
	EditorData *state = _data->stateData[EDITOR]; // Create a shortcut to the state data

	// Display the background
	if (state->level.map.animatedBackground != NULL)
	{
		state->level.map.entityBackground.position.x = EDITOR_MAP_BACKGROUND_POSITION;
		state->level.map.entityBackground.position.y = EDITOR_SCREEN_POSITION.y;
		DisplayAnimation(_data->engine.canvas, state->level.map.animatedBackground, &state->level.map.entityBackground, V2F_FROM_VALUE(EDITOR_MAP_SCALE), NULL);
	}
	else
	{
		printf("No animation found for the background !\n");
	}

	// Display parallax background
	ListElement *current = state->listParallax.first;
	while (current)
	{
		MapParallax *currentParallax = current->data;
		if (currentParallax->depth < 0)
		{
			DisplayParallax(_data, state, currentParallax);
		}
		// Asign the next element
		current = current->next;
	}

	// Display the terrains
	for (int i = 0; i < state->level.map.nbTerrains; i++)
	{
		// Display Animation
		if (state->level.map.terrains[i].animation != NULL)
		{
			state->level.map.terrains[i].entity.position.x = EDITOR_MAP_BACKGROUND_POSITION + state->level.map.terrains[i].position.x;
			state->level.map.terrains[i].entity.position.y = state->level.map.terrains[i].position.y + state->offsetMapY;
			DisplayAnimation(_data->engine.canvas, state->level.map.terrains[i].animation, &state->level.map.terrains[i].entity, V2F_FROM_VALUE(EDITOR_MAP_SCALE), NULL);

		}
		// Display Sprite
		else
		{
			Sprite_Display(_data->engine.canvas, state->level.map.terrains[i].sprite, V2F(EDITOR_MAP_BACKGROUND_POSITION, state->level.map.terrains[i].position.y + state->offsetMapY), V2F_FROM_VALUE(EDITOR_MAP_SCALE), 0, NULL);

			//// Init repeated Terrain positions
			//if (state->level.map.terrains[state->level.map.nbTerrains - 1].position.y + state->offsetMapY >= EDITOR_SCREEN_POSITION.y + MAP_TRIGGER_REPEATED_TERRAIN_INIT_AT_Y && state->level.terrainsAreRepeated && state->level.map.wasRepeatedTerrainInitialized == 0)
			//{
			//	InitRepeatedTerrainPosition(_data);
			//}
		}
	}

	// If the last terrain position Y is at the top screen or below
	if (state->level.map.nbTerrains > 0 && state->level.map.terrains[state->level.map.nbTerrains - 1].position.y + state->level.map.terrains[state->level.map.nbTerrains - 1].size.y * EDITOR_MAP_SCALE + state->offsetMapY >= EDITOR_SCREEN_POSITION.y && state->level.terrainsAreRepeated)
	{
		Editor_DisplayLastTerrainsRepeatedly(_data);
	}
}

void DisplayLevelParallaxForeground(MainData* _data)
{
	EditorData *state = _data->stateData[EDITOR]; // Create a shortcut to the state data
	ListElement *current = state->listParallax.first;
	while (current)
	{
		MapParallax *currentParallax = current->data;
		if (currentParallax->depth >= 0)
		{
			DisplayParallax(_data, state, currentParallax);
		}
		// Asign the next element
		current = current->next;
	}
}

void DisplaySelectors(MainData* _data)
{
	EditorData *state = _data->stateData[EDITOR]; // Create a shortcut to the state data

	// -- PARALLAX SELECTORS --------------------------------------------------------- //
	ListElement *current = state->listParallax.first;
	while (current)
	{
		MapParallax *currentParallax = current->data;
		sfVector2f selectorPos;
		selectorPos.x = EDITOR_MAP_BACKGROUND_POSITION + EDITOR_SCREEN_WIDTH;
		selectorPos.y = currentParallax->position.y + currentParallax->size.y*EDITOR_MAP_SCALE - EDITOR_SELECTOR_HEIGHT;
		Sprite_Display(_data->engine.canvas, state->parallaxSelector, selectorPos, V2F_FROM_VALUE(1), 0, NULL);

		// Asign the next element
		current = current->next;
	}

	// -- TERRAIN SELECTORS ---------------------------------------------------------- //

	for (int i = 0; i < state->level.map.nbTerrains; i++)
	{
		sfVector2f selectorPos;
		selectorPos.x = EDITOR_MAP_BACKGROUND_POSITION - EDITOR_SELECTOR_WIDTH;
		selectorPos.y = state->level.map.terrains[i].position.y + state->level.map.terrains[i].size.y*EDITOR_MAP_SCALE - EDITOR_SELECTOR_HEIGHT + state->offsetMapY;
		//printf("%d -- %f;%f\n", i, selectorPos.x, selectorPos.y);
		Sprite_Display(_data->engine.canvas, state->terrainSelector, selectorPos, V2F_FROM_VALUE(1), 0, NULL);
	}
}