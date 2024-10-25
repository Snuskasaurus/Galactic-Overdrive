#include "Editor.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

void Editor_TextEntered(MainData *_data, char _c);
void Editor_KeyPressed(MainData *_data, sfKeyCode _key);
void Editor_KeyReleased(MainData *_data, sfKeyCode _key);
void Editor_MouseButtonScrolled(MainData *_data, float _delta);
void Editor_MouseButtonPressed(MainData *_data, sfMouseButton _button);
void Editor_MouseButtonReleased(MainData *_data, sfMouseButton _button);
void Editor_SelectingNewWaypoint(MainData *_data, EditorData *_state);
void Editor_ChangeWaypointValues(MainData *_data, EditorData *_state);

// -- MAIN FUNCTION -------------------------------------------------------- //

void Editor_HandleEvent(MainData *_data, sfEvent _event)
{
	switch (_event.type)
	{
	case sfEvtTextEntered:
		Editor_TextEntered(_data, (char)_event.text.unicode);
		break;
	case sfEvtKeyPressed:
		Editor_KeyPressed(_data, _event.key.code);
		break;
	case sfEvtKeyReleased:
		Editor_KeyReleased(_data, _event.key.code);
		break;
	case sfEvtMouseWheelScrolled:
		Editor_MouseButtonScrolled(_data, _event.mouseWheelScroll.delta);
		break;
	case sfEvtMouseButtonPressed:
		Editor_MouseButtonPressed(_data, _event.mouseButton.button);
		break;
	case sfEvtMouseButtonReleased:
		Editor_MouseButtonReleased(_data, _event.mouseButton.button);
		break;
	default:
		break;
	}
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //

void Editor_TextEntered(MainData *_data, char _c)
{
	char tempLevelName[BUFFER_SMALL] = { 0 };
	char tmp_s[2];
	// Create a shortcut to the state data
	EditorData *state = _data->stateData[EDITOR];

	// Create the string
	tmp_s[0] = _c;
	tmp_s[1] = '\0';
	if (state->isEditingLevelName)
	{
		if (state->level.name)
		{
			if (tmp_s[0] == '\b')
			{
				strcpy(tempLevelName, state->level.name);
				if (strlen(tempLevelName) >= 1)
				{
					tempLevelName[strlen(tempLevelName) - 1] = 0;
				}
			}
			else
			{
				strcpy(tempLevelName, state->level.name);
				strcat(tempLevelName, tmp_s);
			}
			strcpy(state->level.name, tempLevelName);
			///printf("name : %s\n", state->level.name);
		}
		else
		{
			strcpy(state->level.name, tmp_s);
			//state->level.name = _strdup(tmp_s);
		}
		Button_SetText(&_data->engine, state->buttons.array[0], state->level.name);
	}
}

void Editor_KeyPressed(MainData *_data, sfKeyCode _key)
{
	// Create a shortcut to the state data
	EditorData *state = _data->stateData[EDITOR];
	if (_key == sfKeyLShift)
	{
		state->haveFasterScrolling = TRUE;
	}
	if (_key == sfKeyT)
	{
		if (state->level.paths.selected && state->level.paths.selected->type == WAYPOINT_SPAWN)
		{
			Waypoint_Spawn *spawn = state->level.paths.selected->content;
			// Set starting time to the current level time
			spawn->timeStart = state->level.map.time;
			Slider_SetFloatValue(state->parameters[TOOLMODE_PATH].sliders.array[1], spawn->timeStart);
		}
	}
	if (_key == sfKeyReturn)
	{
		if (state->isEditingLevelName)
		{
			state->isEditingLevelName = FALSE;
		}
	}
}

void Editor_KeyReleased(MainData *_data, sfKeyCode _key)
{
	// Create a shortcut to the state data
	EditorData *state = _data->stateData[EDITOR];
	if (_key == sfKeyLShift)
	{
		state->haveFasterScrolling = FALSE;
	}
}

void Editor_MouseButtonScrolled(MainData *_data, float _delta)
{
	// Create a shortcut to the state data
	EditorData *state = _data->stateData[EDITOR];
	float speedMultiplier = 1.0f;
	if (state->haveFasterScrolling)
	{
		speedMultiplier = EDITOR_SPEED_MULTIPLIER;
	}
	else
	{
		speedMultiplier = 1.0f;
	}
	state->offsetMapY += _delta * (30.0f / EDITOR_MAP_ZOOM) * speedMultiplier;
}

void Editor_MouseButtonPressed(MainData *_data, sfMouseButton _button)
{
	EditorData *state = _data->stateData[EDITOR]; // Create a shortcut to the state data
	sfVector2f mousePosition = V2F_FROM_V2I(sfMouse_getPositionRenderWindow(_data->engine.window));

	if (IsColliding_Point_Rectangle(mousePosition, state->mapEditorCollider))
	{
		if (state->toolModeSelected == TOOLMODE_PATH)
		{
			if (state->toolSelected == TOOL_CREATION)
			{
				Slider_SetIntValue(state->parameters[TOOLMODE_PATH].sliders.array[3], 1);
				Slider_SetFloatValue(state->parameters[TOOLMODE_PATH].sliders.array[1], state->timerTopScreen);
				_Waypoint_Create(&state->level.paths, WAYPOINT_MOVE, mousePosition);
				Editor_ChangeWaypointValues(_data, state);
			}
			else if (state->toolSelected == TOOL_SELECTION)
			{
				Path_SelectWaypoint(&state->level.paths, mousePosition, state->timerTopScreen);
			}
			else if (state->toolSelected == TOOL_MOVE)
			{
				Path_SelectWaypoint(&state->level.paths, mousePosition, state->timerTopScreen);
			}
			Editor_SelectingNewWaypoint(_data, state);
		}
		Editor_SelectParallaxOrTerrain(_data, mousePosition);
	}
	else
	{
		Button_Pressing(&_data->engine, &state->buttons);
		Carousel_Pressing(&_data->engine, &state->parameters[state->toolModeSelected].carousels, mousePosition);
	}
}

void Editor_MouseButtonReleased(MainData *_data, sfMouseButton _button)
{
	EditorData *state = _data->stateData[EDITOR]; // Create a shortcut to the state data
	sfVector2f mousePosition = V2F_FROM_V2I(sfMouse_getPositionRenderWindow(_data->engine.window));
	Button_Releasing(&_data->engine, _data, &state->buttons);
	Carousel_Releasing(&_data->engine, &state->parameters[state->toolModeSelected].carousels, mousePosition);
}

void Editor_SelectingNewWaypoint(MainData *_data, EditorData *_state)
{
	if (_state->level.paths.selected)
	{
		switch (_state->level.paths.selected->type)
		{
		case WAYPOINT_SPAWN:
		{
			Waypoint_Spawn *spawn = _state->level.paths.selected->content;
			// Enemy to spawn
			Carousel_SetCurrentName(_state->parameters[TOOLMODE_PATH].carousels.array[0], spawn->enemyName);
			// Number of enemies to spawn
			Slider_SetIntValue(_state->parameters[TOOLMODE_PATH].sliders.array[0], spawn->maximum);
			// Starting time
			Slider_SetFloatValue(_state->parameters[TOOLMODE_PATH].sliders.array[1], spawn->timeStart);
			// Duration
			Slider_SetFloatValue(_state->parameters[TOOLMODE_PATH].sliders.array[2], spawn->duration);
			// Diffculty
			Slider_SetIntValue(_state->parameters[TOOLMODE_PATH].sliders.array[3], spawn->difficulty);
			break;
		}
		}
	}
}