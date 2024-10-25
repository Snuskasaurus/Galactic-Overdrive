#include "Overworld.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

void SelectionDisplay(MainData *_data);
void LobbyDisplay(MainData *_data);

// -- MAIN FUNCTION -------------------------------------------------------- //

void Overworld_Display(MainData *_data)
{
	OverworldData *state = _data->stateData[OVERWORLD];
	sfVector2f tempScale = _data->engine.configuration->scale;
	Sprite_Display(_data->engine.canvas, state->background, (sfVector2f) { 0, 0 }, (sfVector2f) { 4 * tempScale.x, 4 * tempScale.x }, 0, NULL);
	switch (state->state)
	{
	case SELECTION:
		SelectionDisplay(_data);
		break;
	case LOBBY:
		LobbyDisplay(_data);
		break;
	default:
		break;
	}
	Fade_Display(_data);
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //

void DisplayTextOverworldDifficulty(MainData *_data, sfVector2f _position)
{
	OverworldData *state = _data->stateData[OVERWORLD]; // Create a shortcut to the state data
	sfVector2f tempScale = _data->engine.configuration->scale;
	Sprite_Display(_data->engine.canvas, state->difficulty[state->choosenDifficulty], _position, (sfVector2f) { 3 * tempScale.y, 3 * tempScale.y }, 0, NULL);
}

void DisplayOverworldHighscores(MainData* _data)
{
	OverworldData *state = _data->stateData[OVERWORLD];
	sfVector2i tempResolution = _data->engine.configuration->resolution;

	char score[BUFFER_SMALL] = { 0 };
	for (int i = 0; i < 5; i++)
	{
		sfVector2f tempPos = { (float)(tempResolution.x / 2 + 153.f), (float)(tempResolution.y / 2 - 2 * 87.f + i * 87.f) };
		unsigned long tempScore = 0;
		int world = 0;
		for (int j = 0; j < NB_LEVELS; j++)
		{
			if (state->Levels[j].selected)
			{
				world = j;
			}
		}
		switch (world)
		{
		case 0:
			tempScore = state->Progression.Level1Highscores[state->choosenDifficulty][i];
			break;
		case 1:
			tempScore = state->Progression.Level2Highscores[state->choosenDifficulty][i];
			break;
		case 2:
			tempScore = state->Progression.Level3Highscores[state->choosenDifficulty][i];
			break;
		default:
			break;
		}
		sprintf(score, "%d", tempScore);
		Text_Display(_data->engine.canvas, &state->difficultyDisplay, score, tempPos, (sfVector2f) { 0.5f, 0.5f });
	}
}

void SelectionDisplay(MainData *_data)
{
	sfVector2i tempResolution = _data->engine.configuration->resolution;
	OverworldData *state = _data->stateData[OVERWORLD];
	sfVector2f tempPos = { state->selection.x, state->selection.y };
	sfVector2f tempScale = _data->engine.configuration->scale;

	for (int i = 0; i < NB_LEVELS; i++)
	{
		sfVector2f tempLevelPos = { state->Levels[i].x, state->Levels[i].y };
		Sprite_Display(_data->engine.canvas, state->Levels[i].sprite, tempLevelPos, (sfVector2f) { 3 * tempScale.x, 3 * tempScale.x }, state->Levels[i].angle, NULL);
	}

	Shader_ApplyBloomToCanvas(_data->engine.canvas);

	for (int i = 0; i < NB_LEVELS; i++)
	{
		if (state->Levels[i].unlocked)
		{
			DisplayAnimation(_data->engine.canvas, state->levelName[i], &state->levelEntity[i], (sfVector2f) { 3, 3 }, NULL);
		}
	}

	for (int i = 0; i < NB_LEVELS - 1; i++)
	{
		sfVector2f tempPathPos = { state->pathPos[i].x, state->pathPos[i].y };
		Sprite_Display(_data->engine.canvas, state->path[i], tempPathPos, (sfVector2f) { 3 * tempScale.x, 3 * tempScale.x }, 0, NULL);
	}

	Sprite_Display(_data->engine.canvas, state->selection.cursor, tempPos, (sfVector2f){ 2 * tempScale.x, 2 * tempScale.x
	}, state->selection.angle, NULL);

	for (int i = 0; i < NB_LEVELS; i++)
	{
		if (state->Levels[i].selected)
		{
			Sprite_Display(_data->engine.canvas, state->selection.selected, tempPos, (sfVector2f) { 2 * tempScale.x, 2 * tempScale.x }, 0, NULL);
			if (i < 3)
			{
				if (state->selection.currentTime > 0)
				{
					int frame = (int)(6 * state->selection.currentTime / 0.1f);
					Sprite_Display(_data->engine.canvas, state->selection.info[frame], (sfVector2f) { (float)(tempResolution.x / 2), (float)(tempResolution.y / 2) }, (sfVector2f) { 3 * tempScale.y, 3 * tempScale.y }, 0, NULL);
				}
				if (state->selection.currentTime == 0.1f)
				{
					DisplayTextOverworldDifficulty(_data, (sfVector2f) { (float)(tempResolution.x / 2.f - 153.f), (float)(tempResolution.y / 2.f) });
					DisplayOverworldHighscores(_data);
				}
			}
		}
	}
	///

	for (int i = state->nbPlayers; i < state->toConnect; i++)
	{
		float tempX[MAX_PLAYER_NUMBER] = { (float)(2 * tempResolution.x / 21), (float)(19 * tempResolution.x / 21), (float)(2 * tempResolution.x / 21), (float)(19 * tempResolution.x / 21) };
		float tempY[MAX_PLAYER_NUMBER] = { (float)(1.125f * tempResolution.y / 11), (float)(1.125 * tempResolution.y / 11), (float)(10.125f * tempResolution.y / 11), (float)(10.125f * tempResolution.y / 11) };
		Sprite_Display(_data->engine.canvas, state->selection.connect, (sfVector2f) { tempX[i], tempY[i] }, (sfVector2f) { 2 * tempScale.x, 2 * tempScale.x }, 0, NULL);
	}

	for (int i = 0; i < state->nbPlayers; i++)
	{
		float tempX[MAX_PLAYER_NUMBER] = { (float)(2 * tempResolution.x / 21), (float)(19 * tempResolution.x / 21), (float)(2 * tempResolution.x / 21), (float)(19 * tempResolution.x / 21) };
		float tempY[MAX_PLAYER_NUMBER] = { (float)(1.5f * tempResolution.y / 11), (float)(1.5f * tempResolution.y / 11), (float)(10.5f * tempResolution.y / 11), (float)(10.5f * tempResolution.y / 11) };
		float tempY2[MAX_PLAYER_NUMBER] = { (float)(0.75f * tempResolution.y / 11), (float)(0.75f * tempResolution.y / 11), (float)(9.75f * tempResolution.y / 11), (float)(9.75f * tempResolution.y / 11) };
		Sprite_Display(_data->engine.canvas, state->moveControls[state->lobby[i].direction], (sfVector2f) { tempX[i], tempY[i] }, (sfVector2f) { 4 * tempScale.x, 4 * tempScale.x }, 0, NULL);
		Sprite_Display(_data->engine.canvas, state->selection.connected[i], (sfVector2f) { tempX[i], tempY2[i] }, (sfVector2f) { 2 * tempScale.x, 2 * tempScale.x }, 0, NULL);
	}
}

void DisplayTextSpaceshipDescription(MainData *_data, sfVector2f _position, int _player)
{
	OverworldData *state = _data->stateData[OVERWORLD]; // Create a shortcut to the state data
	state->difficultyDisplay.color = sfWhite;
	Text_Display(_data->engine.canvas, &state->difficultyDisplay, _data->engine.blueprints.players.array[state->lobby[_player].choiceSpaceship]->name, _position, V2F(0.5, 0.5));
}

void DisplayTextColorDescription(MainData *_data, sfVector2f _position, int _player)
{
	OverworldData *state = _data->stateData[OVERWORLD]; // Create a shortcut to the state data
	state->difficultyDisplay.color = sfWhite;
	Text_Display(_data->engine.canvas, &state->difficultyDisplay, _data->spaceShipColorsNames[state->lobby[_player].choiceColor], _position, V2F(0.5, 0.5));
}

void DisplayTextCapacityDescription(MainData *_data, sfVector2f _position, int _player)
{
	OverworldData *state = _data->stateData[OVERWORLD]; // Create a shortcut to the state data
	state->difficultyDisplay.color = sfWhite;
	Text_Display(_data->engine.canvas, &state->difficultyDisplay, _data->engine.skills.array[state->lobby[_player].choiceSpecial]->name, _position, V2F(0.5, 0.5));
}

void LobbyDisplay(MainData *_data)
{
	sfVector2i tempResolution = _data->engine.configuration->resolution;
	OverworldData *state = _data->stateData[OVERWORLD];
	sfVector2f tempScale = _data->engine.configuration->scale;
	Shader_ApplyBloomToCanvas(_data->engine.canvas);
	for (int i = 0; i < state->nbPlayers; i++)
	{
		float tempX[MAX_PLAYER_NUMBER] = { (float)(tempResolution.x / 4), (float)(3 * tempResolution.x / 4), (float)(tempResolution.x / 4), (float)(3 * tempResolution.x / 4) };
		float tempY[MAX_PLAYER_NUMBER] = { (float)(tempResolution.y / 4), (float)(tempResolution.y / 4), (float)(3 * tempResolution.y / 4), (float)(3 * tempResolution.y / 4) };
		sfColor tempColor[NB_COLORS] = { sfWhite, sfRed, sfGreen, sfBlue, sfMagenta, sfYellow, sfCyan };
		Sprite_Display(_data->engine.canvas, state->selection.info[6], (sfVector2f) { tempX[i], tempY[i] }, (sfVector2f) { 2 * tempScale.y, 2 * tempScale.y }, 0, NULL);
		Sprite_Display(_data->engine.canvas, state->select[state->lobby[i].select_frame], (sfVector2f) { tempX[i], tempY[i] + 40 }, (sfVector2f) { 2 * tempScale.y, 2 * tempScale.y }, 0, NULL);
		switch (state->lobby[i].Window)
		{
		case SPACESHIP:
		{
			sfRenderStates *renderState = Shader_PlayerSpaceship(100, 100, sfWhite, sfFalse, 0.f);
			Sprite_Display(_data->engine.canvas, _data->engine.blueprints.players.array[state->lobby[i].choiceSpaceship]->idle->sprites[0], (sfVector2f) { tempX[i] + 5 * tempScale.x, tempY[i] + 20 / tempScale.y }, V2F_FROM_VALUE(4.0f * tempScale.y), 0, renderState);
			DisplayTextSpaceshipDescription(_data, (sfVector2f) { tempX[i], tempY[i] - 100 * tempScale.y }, i);
		}
		break;
		case COLOR:
		{
			sfRenderStates *renderState = Shader_PlayerSpaceship(100, 100, _data->spaceShipColors[state->lobby[i].choiceColor], sfFalse, 0.f);
			Sprite_Display(_data->engine.canvas, _data->engine.blueprints.players.array[state->lobby[i].choiceSpaceship]->idle->sprites[0], (sfVector2f) { tempX[i] + 5 * tempScale.x, tempY[i] + 20 / tempScale.y }, V2F_FROM_VALUE(4.0f * tempScale.y), 0, renderState);
			DisplayTextColorDescription(_data, (sfVector2f) { tempX[i], tempY[i] - 100 * tempScale.y }, i);
		}			
		break;
		case SPECIAL:
			Sprite_Display(_data->engine.canvas, _data->engine.skills.array[state->lobby[i].choiceSpecial]->icon, (sfVector2f) { tempX[i], tempY[i] + 20 / tempScale.y }, (sfVector2f) { 4 * tempScale.y, 4 * tempScale.y }, 0, NULL);
			DisplayTextCapacityDescription(_data, (sfVector2f) { tempX[i], tempY[i] - 100 * tempScale.y }, i);
			break;
		default:
			break;
		}
	}
}