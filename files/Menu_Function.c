#include "Menu.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

// -- MAIN FUNCTIONS ------------------------------------------------------- //

// -- Main Menu --------------------------------------------
void Menu_StartGame(MainData *_data)
{
	_data->currentState = OVERWORLD;
}

void MenuToCredits(MainData* _data)
{
	_data->currentState = CREDIT;
}

void Menu_StartOption(MainData *_data)
{
	MenuData *state = _data->stateData[MENU];
	Configuration* tempConfig = _data->engine.configuration;
	sfVector2i tempResolution = tempConfig->resolution;
	sfVector2i tempResolutions[15] = { { 1024, 768 }, { 1152, 864 }, { 1280, 600 }, { 1280, 720 }, { 1280, 768 }, { 1280, 800 }, { 1280, 960 }, { 1280, 1024 }, { 1360, 768 }, { 1366, 768 }, { 1400, 1050 }, { 1440, 900 }, { 1600, 900 }, { 1680, 1050 }, { 1920, 1080 } };
	for (int i = 0; i < 15; i++)
	{
		state->currentResolution = tempConfig->resolution.x == tempResolutions[i].x && tempConfig->resolution.y == tempResolutions[i].y ? i : state->currentResolution;
	}
	state->resolution = tempResolutions[state->currentResolution];
	for (int i = 0; i < 17; i++)
	{
		state->buttons[MENU_OPTION].array[i]->content.size = 30 * tempResolution.x / 1920;
	}
	state->parametersText.size = 30 * tempResolution.x / 1920;
	state->subState = MENU_OPTION;
}

void Menu_StartAchievement(MainData *_data)
{
	MenuData *state = _data->stateData[MENU];
	state->subState = MENU_ACHIEVEMENTS;
}

void Menu_Quit(MainData *_data)
{
	_data->currentState = -1;
	sfRenderWindow_close(_data->engine.window);
}

void Menu_StartEditor(MainData *_data)
{
	_data->currentState = EDITOR;
}


// -- Settings Menu ----------------------------------------
void Settings_Apply(MainData* _data)
{
	MenuData* state = _data->stateData[MENU];
	Configuration* tempConfig = _data->engine.configuration;
	SoundEngine* tempSound = _data->engine.sound;
	sfVector2i* tempResolution = &tempConfig->resolution;
	sfVector2f* tempScale = &tempConfig->scale;

	// Recreate window
	tempConfig->resolution = state->resolution;
	Configuration_Save(_data->engine.configuration);
	sfRenderWindow_close(_data->engine.window);
	_data->engine.window = Configuration_CreateWindow(_data->engine.configuration);

	// Recreate canvas
	sfRenderTexture_destroy(_data->engine.canvas);
	_data->engine.canvas = sfRenderTexture_create(_data->engine.configuration->resolution.x, _data->engine.configuration->resolution.y, 32);
	sfRenderTexture_clear(_data->engine.canvas, sfTransparent);

	// Change Volume
	tempSound->volumes[0] = (float)tempConfig->effectVolume;
	tempSound->volumes[1] = (float)tempConfig->globalVolume;
	tempSound->volumes[2] = (float)tempConfig->musicVolume;

	// update scale
	tempScale->x = (float)tempResolution->x / 1920;
	tempScale->y = (float)tempResolution->y / 1080;

	_data->engine.gameScale.x = 4.0f * tempScale->x;
	_data->engine.gameScale.y = 4.0f * tempScale->y;

	for (int i = 0; i < 5; i++)
	{
		state->buttons[MENU_MAIN].array[i]->position.x = (float)(tempResolution->x / 2);
		state->buttons[MENU_MAIN].array[i]->position.y = (float)(2 * tempResolution->y / 3 + i * 40 * tempResolution->y / 1080);
		state->buttons[MENU_MAIN].array[i]->scale.x = tempScale->x * 2.0f;
		state->buttons[MENU_MAIN].array[i]->scale.y = tempScale->y * 2.0f;
	}

	state->buttons[MENU_OPTION].array[0]->position.x = (float)(tempResolution->x / 2.15);
	state->buttons[MENU_OPTION].array[0]->position.y = (float)(tempResolution->y / 10);
	state->buttons[MENU_OPTION].array[1]->position.x = (float)(tempResolution->x / 1.65);
	state->buttons[MENU_OPTION].array[1]->position.y = (float)(tempResolution->y / 10);
	state->buttons[MENU_OPTION].array[2]->position.x = (float)(tempResolution->x / 2.15);
	state->buttons[MENU_OPTION].array[2]->position.y = (float)(2 * tempResolution->y / 10);
	state->buttons[MENU_OPTION].array[3]->position.x = (float)(tempResolution->x / 1.8);
	state->buttons[MENU_OPTION].array[3]->position.y = (float)(2 * tempResolution->y / 10);
	state->buttons[MENU_OPTION].array[4]->position.x = (float)(tempResolution->x / 2.15);
	state->buttons[MENU_OPTION].array[4]->position.y = (float)(3 * tempResolution->y / 10);
	state->buttons[MENU_OPTION].array[5]->position.x = (float)(tempResolution->x / 1.8);
	state->buttons[MENU_OPTION].array[5]->position.y = (float)(3 * tempResolution->y / 10);
	state->buttons[MENU_OPTION].array[6]->position.x = (float)(tempResolution->x / 2.15);
	state->buttons[MENU_OPTION].array[6]->position.y = (float)(4 * tempResolution->y / 10);
	state->buttons[MENU_OPTION].array[7]->position.x = (float)(tempResolution->x / 1.8);
	state->buttons[MENU_OPTION].array[7]->position.y = (float)(4 * tempResolution->y / 10);

	state->buttons[MENU_OPTION].array[8]->position.x = (float)(tempResolution->x / 2.15);
	state->buttons[MENU_OPTION].array[8]->position.y = (float)(5 * tempResolution->y / 10);
	state->buttons[MENU_OPTION].array[9]->position.x = (float)(tempResolution->x / 1.5);
	state->buttons[MENU_OPTION].array[9]->position.y = (float)(5 * tempResolution->y / 10);
	state->buttons[MENU_OPTION].array[10]->position.x = (float)(tempResolution->x / 2.15);
	state->buttons[MENU_OPTION].array[10]->position.y = (float)(6 * tempResolution->y / 10);
	state->buttons[MENU_OPTION].array[11]->position.x = (float)(tempResolution->x / 1.5);
	state->buttons[MENU_OPTION].array[11]->position.y = (float)(6 * tempResolution->y / 10);
	state->buttons[MENU_OPTION].array[12]->position.x = (float)(tempResolution->x / 2.15);
	state->buttons[MENU_OPTION].array[12]->position.y = (float)(7 * tempResolution->y / 10);
	state->buttons[MENU_OPTION].array[13]->position.x = (float)(tempResolution->x / 1.5);
	state->buttons[MENU_OPTION].array[13]->position.y = (float)(7 * tempResolution->y / 10);

	state->buttons[MENU_OPTION].array[14]->position.x = (float)(tempResolution->x / 2.5);
	state->buttons[MENU_OPTION].array[14]->position.y = (float)(9 * tempResolution->y / 10);
	state->buttons[MENU_OPTION].array[15]->position.x = (float)(tempResolution->x / 2);
	state->buttons[MENU_OPTION].array[15]->position.y = (float)(9 * tempResolution->y / 10);
	state->buttons[MENU_OPTION].array[16]->position.x = (float)(tempResolution->x * (1 - 1 / 2.5));
	state->buttons[MENU_OPTION].array[16]->position.y = (float)(9 * tempResolution->y / 10);

	for (int i = 0; i < 17; i++)
	{
		state->buttons[MENU_OPTION].array[i]->content.size = 30 * tempResolution->x / 1920;
	}

	state->parametersText.size = 30 * tempResolution->x / 1920;
}

void Settings_Credits(MainData* _data)
{
	Fade_CreateTransition(&_data->transitionFade, MenuToCredits);
}

void Settings_LeftArrowFramerate(MainData* _data)
{
	MenuData* state = _data->stateData[MENU];
	Configuration* tempConfig = _data->engine.configuration;
	tempConfig->resolution = state->resolution;
	tempConfig->framerate /= 2;
	tempConfig->framerate = tempConfig->framerate < 30 ? 30 : tempConfig->framerate;
}

void Settings_LeftArrowResolution(MainData* _data)
{
	MenuData* state = _data->stateData[MENU];
	Configuration* tempConfig = _data->engine.configuration;
	sfVector2i tempResolutions[15] = { { 1024, 768 }, { 1152, 864 }, { 1280, 600 }, { 1280, 720 }, { 1280, 768 }, { 1280, 800 }, { 1280, 960 }, { 1280, 1024 }, { 1360, 768 }, { 1366, 768 }, { 1400, 1050 }, { 1440, 900 }, { 1600, 900 }, { 1680, 1050 }, { 1920, 1080 } };
	(state->currentResolution)--;
	state->currentResolution = state->currentResolution < 0 ? 0 : state->currentResolution;
	state->resolution = tempResolutions[state->currentResolution];
}

void Settings_MinusMainVolume(MainData* _data)
{
	Configuration* tempConfig = _data->engine.configuration;
	tempConfig->globalVolume -= 5;
	tempConfig->globalVolume = tempConfig->globalVolume < 0 ? 0 : tempConfig->globalVolume;
}

void Settings_MinusMusicVolume(MainData* _data)
{
	Configuration* tempConfig = _data->engine.configuration;
	tempConfig->musicVolume -= 5;
	tempConfig->musicVolume = tempConfig->musicVolume < 0 ? 0 : tempConfig->musicVolume;
}

void Settings_MinusSoundVolume(MainData* _data)
{
	Configuration* tempConfig = _data->engine.configuration;
	tempConfig->effectVolume -= 5;
	tempConfig->effectVolume = tempConfig->effectVolume < 0 ? 0 : tempConfig->effectVolume;
}

void Settings_Ok(MainData* _data)
{
	MenuData *state = _data->stateData[MENU];
	_data->engine.configuration = Configuration_GetValues();
	state->subState = MENU_MAIN;
}

void Settings_PlusMainVolume(MainData* _data)
{
	Configuration* tempConfig = _data->engine.configuration;
	tempConfig->globalVolume += 5;
	tempConfig->globalVolume = tempConfig->globalVolume > 100 ? 100 : tempConfig->globalVolume;
}

void Settings_PlusMusicVolume(MainData* _data)
{
	Configuration* tempConfig = _data->engine.configuration;
	tempConfig->musicVolume += 5;
	tempConfig->musicVolume = tempConfig->musicVolume > 100 ? 100 : tempConfig->musicVolume;
}

void Settings_PlusSoundVolume(MainData* _data)
{
	Configuration* tempConfig = _data->engine.configuration;
	tempConfig->effectVolume += 5;
	tempConfig->effectVolume = tempConfig->effectVolume > 100 ? 100 : tempConfig->effectVolume;
}

void Settings_ReverseFullscreen(MainData* _data)
{
	Configuration* tempConfig = _data->engine.configuration;
	tempConfig->fullscreen = !tempConfig->fullscreen;
}

void Settings_ReverseVsync(MainData* _data)
{
	Configuration* tempConfig = _data->engine.configuration;
	tempConfig->vsync = !tempConfig->vsync;
}

void Settings_RightArrowFramerate(MainData* _data)
{
	Configuration* tempConfig = _data->engine.configuration;
	tempConfig->framerate *= 2;
	tempConfig->framerate = tempConfig->framerate > 120 ? 120 : tempConfig->framerate;
}

void Settings_RightArrowResolution(MainData* _data)
{
	MenuData* state = _data->stateData[MENU];
	Configuration* tempConfig = _data->engine.configuration;
	sfVector2i tempResolutions[15] = { { 1024, 768 }, { 1152, 864 }, { 1280, 600 }, { 1280, 720 }, { 1280, 768 }, { 1280, 800 }, { 1280, 960 }, { 1280, 1024 }, { 1360, 768 }, { 1366, 768 }, { 1400, 1050 }, { 1440, 900 }, { 1600, 900 }, { 1680, 1050 }, { 1920, 1080 } };
	(state->currentResolution)++;
	state->currentResolution = state->currentResolution > 14 ? 14 : state->currentResolution;
	state->resolution = tempResolutions[state->currentResolution];
}

// -- Achievements Menu ------------------------------------
void AchievementMenu_Update(MainData *_data)
{
	MenuData *state = _data->stateData[MENU];

	if (state->subState == MENU_ACHIEVEMENTS)
	{
		// -- Navigation Arrow States ------ //
		// Arrow Left
		if (state->achievementCurrentPage - 1 < 1)
		{
			state->buttons[MENU_ACHIEVEMENTS].array[1]->currentState = BUTTON_DISABLED;
		}
		else if(state->buttons[MENU_ACHIEVEMENTS].array[1]->currentState == BUTTON_DISABLED)
		{
			state->buttons[MENU_ACHIEVEMENTS].array[1]->currentState = BUTTON_WAITING;
		}
		// Arrow Right
		if (state->achievementCurrentPage + 1 > state->achievementPageMax)
		{
			state->buttons[MENU_ACHIEVEMENTS].array[0]->currentState = BUTTON_DISABLED;
		}
		else if (state->buttons[MENU_ACHIEVEMENTS].array[0]->currentState == BUTTON_DISABLED)
		{
			state->buttons[MENU_ACHIEVEMENTS].array[0]->currentState = BUTTON_WAITING;
		}
		// --------------------------------- //
	}
}

void AchievementMenu_ArrowPageLeft(MainData* _data)
{
	MenuData* state = _data->stateData[MENU];
	if (state->achievementCurrentPage - 1 >= 1)
	{
		state->achievementCurrentPage--;
	}
}

void AchievementMenu_ArrowPageRight(MainData* _data)
{
	MenuData* state = _data->stateData[MENU];
	if (state->achievementCurrentPage + 1 <= state->achievementPageMax)
	{
		state->achievementCurrentPage++;
	}
}

void AchievementMenu_BackToMenu(MainData* _data)
{
	MenuData* state = _data->stateData[MENU];
	state->subState = MENU_MAIN;
}

// -- Buttons Change state Functions -----------------------
/// Menu
void MenuFade_MenuToGame(MainData *_data)
{
	Fade_CreateTransition(&_data->transitionFade, &Menu_StartGame);
}

void MenuFade_MenuToEditor(MainData *_data)
{
	Fade_CreateTransition(&_data->transitionFade, &Menu_StartEditor);
}

void MenuFade_MenuToAchievement(MainData *_data)
{
	Fade_CreateTransition(&_data->transitionFade, &Menu_StartAchievement);
}

void MenuFade_MenuToOption(MainData *_data)
{
	Fade_CreateTransition(&_data->transitionFade, &Menu_StartOption);
}

/// Option
void MenuFade_OptionToMenu(MainData *_data)
{
	Fade_CreateTransition(&_data->transitionFade, &Settings_Ok);
}

/// Achievement
void MenuFade_AchievementToMenu(MainData *_data)
{
	Fade_CreateTransition(&_data->transitionFade, &AchievementMenu_BackToMenu);
}