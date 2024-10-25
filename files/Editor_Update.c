#include "Editor.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

void UpdateTestingMode(MainData *_data, EditorData *_state, sfVector2f _mousePosition, float _dt);
void UpdateEditingMode(MainData *_data, EditorData *_state, sfVector2f _mousePosition, float _dt);
void Editor_ChangeLevelValues(MainData *_data, EditorData *_state);
void Editor_ChangeWaypointValues(MainData *_data, EditorData *_state);
void Editor_ChangeParallaxValues(MainData *_data, EditorData *_state);
void Editor_UpdateMapAnims(EditorData* _state, float _dt);

// -- MAIN FUNCTION -------------------------------------------------------- //

void Editor_Update(MainData *_data, float _dt)
{
	EditorData *state = _data->stateData[EDITOR]; // Create a shortcut to the state data
	sfVector2f mousePosition = V2F_FROM_V2I(sfMouse_getPositionRenderWindow(_data->engine.window));
	Button_Update(&_data->engine, &state->buttons, mousePosition);
	Editor_UpdateTimerTopScreen(_data);
	Editor_UpdateMapAnims(state, _dt);

	if (state->isTesting)
	{
		UpdateTestingMode(_data, state, mousePosition, _dt);
	}
	else
	{
		UpdateEditingMode(_data, state, mousePosition, _dt);
	}
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //

void UpdateTestingMode(MainData *_data, EditorData *_state, sfVector2f _mousePosition, float _dt)
{
	Money* nullMoney = NULL;
	Mechanic_SpawnEnemies(_data, &_state->level, _state->enemies, &_state->players, _dt, NULL, NULL);
	Enemy_Update(&_data->engine, _state->enemies, _dt, _state->projectiles, &_state->explosions, _state->lasers, &nullMoney);
	Projectile_Update(&_data->engine, _dt, _state->projectiles, NULL, _data->currentState);

	_state->offsetMapY += _state->level.map.scrollingSpeed * _dt / EDITOR_MAP_ZOOM;
	_state->level.map.time = _state->timerTopScreen;
}

void UpdateEditingMode(MainData *_data, EditorData *_state, sfVector2f _mousePosition, float _dt)
{
	if (IsColliding_Point_Rectangle(_mousePosition, _state->mapEditorCollider))
	{
		if (_state->toolModeSelected == TOOLMODE_PATH)
		{
			if (_state->toolSelected == TOOL_MOVE)
			{
				if (sfMouse_isButtonPressed(sfMouseLeft) && _state->level.paths.selected)
				{
					_state->level.paths.selected->position = _mousePosition;
				}
			}
		}
	}
	else
	{
		Slider_Update(&_data->engine, &_state->parameters[_state->toolModeSelected].sliders, _mousePosition);
		Carousel_Update(&_data->engine, &_state->parameters[_state->toolModeSelected].carousels, _mousePosition);
	}
	switch (_state->toolModeSelected)
	{
	default:
		break;
	case TOOLMODE_LEVEL:
		Editor_ChangeLevelValues(_data, _state);
		break;
	case TOOLMODE_PATH:
		Editor_ChangeWaypointValues(_data, _state);
		break;
	case TOOLMODE_PARALLAX:
		Editor_ChangeParallaxValues(_data, _state);
		break;
	}
}

void Editor_ChangeLevelValues(MainData *_data, EditorData *_state)
{
	Editor_SetBackground(_data, Carousel_GetCurrentName(_state->parameters[TOOLMODE_LEVEL].carousels.array[1]));
	strcpy(_state->level.bossName, Carousel_GetCurrentName(_state->parameters[TOOLMODE_LEVEL].carousels.array[2]));
	_state->level.bossTime = Slider_GetFloatValue(_state->parameters[TOOLMODE_LEVEL].sliders.array[1]);
	_state->worldName = Carousel_GetCurrentName(_state->parameters[TOOLMODE_LEVEL].carousels.array[0]);
	_state->worldPosition = Slider_GetIntValue(_state->parameters[TOOLMODE_LEVEL].sliders.array[0]);
	_state->level.isInSpace = Slider_GetIntValue(_state->parameters[TOOLMODE_LEVEL].sliders.array[2]);
	_state->level.terrainsAreRepeated = Slider_GetIntValue(_state->parameters[TOOLMODE_LEVEL].sliders.array[3]);
	strcpy(_state->level.musicName, Carousel_GetCurrentName(_state->parameters[TOOLMODE_LEVEL].carousels.array[3]));
}

// Parallaxes are sorted in ascending order 
// By so we return an index appropriate for your parallax compared to the others parallaxes depth
int Search_ApropriateNewIndexFrom_ParallaxDepth(List* _list, MapParallax* _selectedParallax)
{
	ListElement *current = _list->first;
	int selectedValue = _selectedParallax->depth;
	int newIndex = 0;
	int foundSelf = 0;
	int indexSelf = 0;

	while (current)
	{
		MapParallax* currentParallax = current->data;
		int currentValue = currentParallax->depth;

		if (currentParallax == _selectedParallax)
		{
			foundSelf = 1;
			indexSelf = newIndex;
		}
		else if (selectedValue <= currentValue)
		{
			if (foundSelf == 1)
			{
				return newIndex - 1;
			}
			else
			{
				return newIndex;
			}
		}
		newIndex++;
		// Asign the next element
		current = current->next;
	}
	return newIndex;
}

void Editor_ChangeParallaxValues(MainData *_data, EditorData *_state)
{
	if (_state->selectedParallax)
	{
		MapParallax* selectedParallax = _state->selectedParallax->data;
		int baseDepth = selectedParallax->depth;

		// -- TIMER TO UPDATE -------- //
		selectedParallax->timerToUpdate = Slider_GetFloatValue(_state->parameters[TOOLMODE_PARALLAX].sliders.array[PARALLAX_TIME_TO_UPDATE]);
		selectedParallax->startingPosition.y =
			EDITOR_SCREEN_POSITION.y -
			selectedParallax->timerToUpdate * (_state->level.map.scrollingSpeed / EDITOR_MAP_ZOOM) -
			selectedParallax->size.y * EDITOR_MAP_SCALE;
		// -- SPEED ------------------ //
		selectedParallax->speed = Slider_GetFloatValue(_state->parameters[TOOLMODE_PARALLAX].sliders.array[PARALLAX_SPEED]);
		// -- DEPTH ------------------ //
		selectedParallax->depth = Slider_GetIntValue(_state->parameters[TOOLMODE_PARALLAX].sliders.array[PARALLAX_DEPTH]);
		// -- OFFSET X --------------- //
		selectedParallax->startingPosition.x = (float)Slider_GetIntValue(_state->parameters[TOOLMODE_PARALLAX].sliders.array[PARALLAX_OFFSET_X]);
		selectedParallax->entity.angle = (float)Slider_GetFloatValue(_state->parameters[TOOLMODE_PARALLAX].sliders.array[PARALLAX_ANGLE]);
		if (selectedParallax->depth != baseDepth && _state->listParallax.number > 1)
		{
			int newIndex = Search_ApropriateNewIndexFrom_ParallaxDepth(&_state->listParallax, selectedParallax);
			if (newIndex >= 0)
			{
				List_Move_PointerToIndex(&_state->listParallax, _state->selectedParallax, newIndex);
			}
		}
	}
}

void Editor_UpdateMapAnims(EditorData* _state, float _dt)
{
	// Update Background
	if (_state->level.map.animatedBackground != NULL)
	{
		UpdateAnimation(_dt, _state->level.map.animatedBackground, &_state->level.map.entityBackground);
	}

	// Update Parallaxes
	ListElement *current = _state->listParallax.first;
	while (current)
	{
		MapParallax *currentParallax = current->data;
		if (currentParallax->animation != NULL)
		{
			UpdateAnimation(_dt, currentParallax->animation, &currentParallax->entity);
		}
		// Asign the next element
		current = current->next;
	}

	// Update Terrain
	for (int i = 0; i < _state->level.map.nbTerrains; i++)
	{
		if (_state->level.map.terrains[i].animation != NULL)
		{
			UpdateAnimation(_dt, _state->level.map.terrains[i].animation, &_state->level.map.terrains[i].entity);
		}
	}
}