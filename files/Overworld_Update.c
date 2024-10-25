#include "Overworld.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

void SelectionUpdate(MainData *_data, float _dt);
void LobbyUpdate(MainData *_data, float _dt);
void Overworld_SelectionToGame(MainData *_data);


// -- MAIN FUNCTION -------------------------------------------------------- //

void Overworld_Update(MainData *_data, float _dt)
{
	OverworldData *state = _data->stateData[OVERWORLD];
	switch (state->state)
	{
	case SELECTION:
		SelectionUpdate(_data, _dt);
		break;
	case LOBBY:
		LobbyUpdate(_data, _dt);
		break;
	default:
		break;
	}
	Fade_Update(_data);
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //

void SelectionUpdate(MainData *_data, float _dt)
{
	sfVector2i tempResolution = _data->engine.configuration->resolution;
	OverworldData *state = _data->stateData[OVERWORLD];
	float tempSpeedX = 0;
	float tempSpeedY = 0;
	int selected = sfFalse;

	for (int i = 0; i < NB_LEVELS; i++)
	{
		if (state->Levels[i].toRight)
		{
			state->Levels[i].angle -= LEVELS_ROTATION_SPEED * _dt;
		}
		else
		{
			state->Levels[i].angle += LEVELS_ROTATION_SPEED * _dt;
		}
	}

	for (int i = 0; i < state->nbPlayers; i++)
	{
		if (state->lobby[i].joystick == 4)
		{
			if (sfKeyboard_isKeyPressed(sfKeyQ))
			{
				tempSpeedX -= PLAYER_SPEED;
			}
			if (sfKeyboard_isKeyPressed(sfKeyD))
			{
				tempSpeedX += PLAYER_SPEED;
			}
			if (sfKeyboard_isKeyPressed(sfKeyZ))
			{
				tempSpeedY -= PLAYER_SPEED;
			}
			if (sfKeyboard_isKeyPressed(sfKeyS))
			{
				tempSpeedY += PLAYER_SPEED;
			}
		}
		else
		{
			sfVector2f axisPosition;
			state->lobby[i].direction = 0;

			axisPosition.x = sfJoystick_getAxisPosition(state->lobby[i].joystick, sfJoystickPovX);
			if (axisPosition.x == 0.0f)
			{
				axisPosition.x = sfJoystick_getAxisPosition(state->lobby[i].joystick, sfJoystickX);
			}
			if (axisPosition.x >= -JOYSTICK_MINIMUM_SENSITIVITY && axisPosition.x <= JOYSTICK_MINIMUM_SENSITIVITY)
			{
				axisPosition.x = 0.0f;
			}
			else
			{
				state->lobby[i].direction = axisPosition.x > 0 ? 2 : 1 ;
			}

			axisPosition.y = -sfJoystick_getAxisPosition(state->lobby[i].joystick, sfJoystickPovY);
			if (axisPosition.y == 0.0f)
			{
				axisPosition.y = sfJoystick_getAxisPosition(state->lobby[i].joystick, sfJoystickY);
			}
			if (axisPosition.y >= -JOYSTICK_MINIMUM_SENSITIVITY && axisPosition.y <= JOYSTICK_MINIMUM_SENSITIVITY)
			{
				axisPosition.y = 0.0f;
			}
			else
			{
				state->lobby[i].direction = axisPosition.y > 0 ? 4 : 3;
			}

			tempSpeedX += axisPosition.x / 100.0f * PLAYER_SPEED;
			tempSpeedY += axisPosition.y / 100.0f * PLAYER_SPEED;
		}
	}

	if (state->nbPlayers)
	{
		tempSpeedX /= state->nbPlayers;
		tempSpeedY /= state->nbPlayers;

		state->selection.x += tempSpeedX * _dt;
		state->selection.y += tempSpeedY * _dt;
	}

	if (state->selection.x < 0)
	{
		state->selection.x = 0;
	}
	if (state->selection.x >= tempResolution.x)
	{
		state->selection.x = (float)(tempResolution.x - 1);
	}
	if (state->selection.y < 0)
	{
		state->selection.y = 0;
	}
	if (state->selection.y >= tempResolution.y)
	{
		state->selection.y = (float)(tempResolution.y - 1);
	}
	if (tempSpeedX || tempSpeedY)
	{
		state->selection.angle = ((float)atan2(tempSpeedY, tempSpeedX) * 180 / PI) + 90;
	}

	for (int i = 0; i < NB_LEVELS; i++)
	{
		if (state->Levels[i].unlocked)
		{
			int distance = (int)sqrt(pow(state->selection.x - state->Levels[i].x, 2) + pow(state->selection.y - state->Levels[i].y, 2));
			state->Levels[i].selected = distance <= state->Levels[i].radius ? sfTrue : sfFalse;
			selected = state->Levels[i].selected ? sfTrue : selected;
			UpdateAnimation(_dt, state->levelName[i], &state->levelEntity[i]);
		}
	}
	state->selection.currentTime += selected ? _dt : -_dt;
	state->selection.currentTime = state->selection.currentTime <= 0.0f ? 0.0f : state->selection.currentTime;
	state->selection.currentTime = state->selection.currentTime >= 0.1f ? 0.1f : state->selection.currentTime;

	_data->choiceSpaceship = state->lobby->choiceSpaceship;
}

void LeaveLobby(MainData* _data)
{
	OverworldData *state = _data->stateData[OVERWORLD];
	for (int i = 0; i < state->nbPlayers; i++)
	{
		state->lobby[i].Window = SPACESHIP;
		state->lobby[i].choiceSpaceship = 0;
		state->lobby[i].choiceColor = 0;
		state->lobby[i].choiceSpecial = 0;
	}
	state->state = SELECTION;
}

void LobbyUpdate(MainData *_data, float _dt)
{
	OverworldData *state = _data->stateData[OVERWORLD];
	int tempLeaveCount = 0;
	int tempReadyCount = 0;
	for (int i = 0; i < state->nbPlayers; i++)
	{
		switch (state->lobby[i].Window)
		{
		case LEAVE:
			tempLeaveCount++;
			break;
		case READY:
			tempReadyCount++;
			break;
		default:
			break;
		}
		state->lobby[i].currentTime += _dt;
		state->lobby[i].select_frame = state->lobby[i].select_frame != 0 && state->lobby[i].currentTime >= 0.1f ? 0 : state->lobby[i].select_frame;
	}
	if (tempLeaveCount == state->nbPlayers)
	{
		Fade_CreateTransition(&_data->transitionFade, LeaveLobby);
	}
	if (tempReadyCount == state->nbPlayers)
	{
		Fade_CreateTransition(&_data->transitionFade, &Overworld_SelectionToGame);
		// Si tu cherche où est passé le contenu de ton if
		// Il est dans la foncion "Overworld_SelectionToGame" juste en dessous
		// -- Nicolas
	}
}

void Overworld_SelectionToGame(MainData *_data)
{
	OverworldData *state = _data->stateData[OVERWORLD];
	for (int i = 0; i < state->nbPlayers; i++)
	{
		for (int j = 0; j < NB_LEVELS; j++)
		{
			if (state->Levels[j].selected)
			{
				_data->engine.world = World_Init(&_data->engine, state->Levels[j].name);
				strcpy(_data->levelName, state->Levels[j].name);
			}
		}
		_data->choosenColors[i] = state->lobby[i].choiceColor;
		_data->choosenSpecials[i] = state->lobby[i].choiceSpecial;
		///_data->customizedPlayers.array[i] = state->spaceshipsSprites[state->lobby[i].choiceSpaceship].array[0];
		///_data->customizedPlayers.array[i]->joystick = state->lobby[i].joystick;
		Player_Create(&_data->engine, &state->outputPlayers, (sfVector2f) { 0, 0 }, state->lobby[i].joystick, _data->engine.blueprints.players.array[state->lobby[i].choiceSpaceship]->name);
		state->outputPlayers.array[i]->skillInfo = Skill_Get(_data->engine.skills.array[state->lobby[i].choiceSpecial]->type, &_data->engine.skills);
		///state->outputPlayers.array[i]->skillInfo.type = state->lobby[i].choiceSpecial;
	}
	_data->customizedPlayers = state->outputPlayers;
	_data->currentState = GAME;
}