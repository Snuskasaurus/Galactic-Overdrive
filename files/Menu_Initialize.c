#include "Menu.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

// -- MAIN FUNCTION -------------------------------------------------------- //

void Menu_Initialize(MainData *_data)
{
	// Allocate the state data to memory
	_data->stateData[MENU] = calloc(1, sizeof(MenuData));
	// Create a shortcut to the state data
	MenuData *state = _data->stateData[MENU];
	Button buttonBlueprints[15];
	sfVector2i tempResolution = _data->engine.configuration->resolution;
	sfVector2f scale;
	scale.x = (float)_data->engine.configuration->scale.x;
	scale.y = (float)_data->engine.configuration->scale.y;

	_data->engine.gameScale.x = 4.0f * scale.x;
	_data->engine.gameScale.y = 4.0f * scale.y;

	state->usingJoystick = sfFalse;
	state->currentButton = 0;
	///buttonBlueprints[0] = Button_InitializeBlueprint(&_data->engine, "Data/Interfaces/Buttons/Default.data");
	buttonBlueprints[0] = Button_InitializeBlueprint(&_data->engine, "Data/Interfaces/Buttons/AchievementButton.data");
	buttonBlueprints[1] = Button_InitializeBlueprint(&_data->engine, "Data/Interfaces/Buttons/EditorButton.data");
	buttonBlueprints[2] = Button_InitializeBlueprint(&_data->engine, "Data/Interfaces/Buttons/PlayButton.data");
	buttonBlueprints[3] = Button_InitializeBlueprint(&_data->engine, "Data/Interfaces/Buttons/QuitButton.data");
	buttonBlueprints[4] = Button_InitializeBlueprint(&_data->engine, "Data/Interfaces/Buttons/SettingsButton.data");
	buttonBlueprints[5] = Button_InitializeBlueprint(&_data->engine, "Data/Interfaces/Buttons/Settings/ApplyButton.data");
	buttonBlueprints[6] = Button_InitializeBlueprint(&_data->engine, "Data/Interfaces/Buttons/Settings/CreditsButton.data");
	buttonBlueprints[7] = Button_InitializeBlueprint(&_data->engine, "Data/Interfaces/Buttons/Settings/LeftArrowButton.data");
	buttonBlueprints[8] = Button_InitializeBlueprint(&_data->engine, "Data/Interfaces/Buttons/Settings/MinusButton.data");
	buttonBlueprints[9] = Button_InitializeBlueprint(&_data->engine, "Data/Interfaces/Buttons/Settings/OkButton.data");
	buttonBlueprints[10] = Button_InitializeBlueprint(&_data->engine, "Data/Interfaces/Buttons/Settings/PlusButton.data");
	buttonBlueprints[11] = Button_InitializeBlueprint(&_data->engine, "Data/Interfaces/Buttons/Settings/RightArrowButton.data");
	buttonBlueprints[12] = Button_InitializeBlueprint(&_data->engine, "Data/Interfaces/Buttons/AchievementMenu/BackToMenu.data");
	buttonBlueprints[13] = Button_InitializeBlueprint(&_data->engine, "Data/Interfaces/Buttons/AchievementMenu/LeftArrowButton.data");
	buttonBlueprints[14] = Button_InitializeBlueprint(&_data->engine, "Data/Interfaces/Buttons/AchievementMenu/RightArrowButton.data");
	state->subState = MENU_MAIN;
	state->previousSubState = state->subState;
	state->timeSinceInput = Timer_Create(MENU_TIMER_MAX_INPUT);

	// Initialize the background
	state->background = Sprite_CreateFromFile("Data/Sprites/Interfaces/BackgroundMenu.png", 0.0f, 0.0f);

	// Initialize the buttons for the main menu
	//Button_Create(&state->buttons[MENU_MAIN], &buttonBlueprints[2], &Menu_StartGame, NULL, (sfVector2f) { 965.0f, 730.0f }, V2F_MUL_VALUE(scale, 2.0f));
	Button_Create(&state->buttons[MENU_MAIN], &buttonBlueprints[2], &MenuFade_MenuToGame, NULL, (sfVector2f) { 965.0f, 730.0f }, V2F_MUL_VALUE(scale, 2.0f));
	Button_Create(&state->buttons[MENU_MAIN], &buttonBlueprints[1], &MenuFade_MenuToEditor, NULL, (sfVector2f) { 960.0f, 770.0f }, V2F_MUL_VALUE(scale, 2.0f));
	Button_Create(&state->buttons[MENU_MAIN], &buttonBlueprints[0], &MenuFade_MenuToAchievement, NULL, (sfVector2f) { 935.0f, 810.0f }, V2F_MUL_VALUE(scale, 2.0f));
	Button_Create(&state->buttons[MENU_MAIN], &buttonBlueprints[4], &MenuFade_MenuToOption, NULL, (sfVector2f) { 955.0f, 850.0f }, V2F_MUL_VALUE(scale, 2.0f));
	Button_Create(&state->buttons[MENU_MAIN], &buttonBlueprints[3], &Menu_Quit, NULL, (sfVector2f) { 973.0f, 890.0f }, V2F_MUL_VALUE(scale, 2.0f));

	for (int i = 0; i < MENU_NB_BUTTONS; i++)
	{
		state->buttons[MENU_MAIN].array[i]->position.x = (float)(tempResolution.x / 2);
		state->buttons[MENU_MAIN].array[i]->position.y = (float)(2 * tempResolution.y / 3 + i * 40 * tempResolution.y / 1080);
	}

	// Initialize the buttons for the option menu
	Button_Create(&state->buttons[MENU_OPTION], &buttonBlueprints[7], &Settings_LeftArrowResolution, "<", (sfVector2f) { (float)(tempResolution.x / 2.15), (float)(tempResolution.y / 10)}, V2F_MUL_VALUE(scale, 2.0f));
	Button_Create(&state->buttons[MENU_OPTION], &buttonBlueprints[11], &Settings_RightArrowResolution, ">", (sfVector2f) { (float)(tempResolution.x / 1.65), (float)(tempResolution.y / 10)}, V2F_MUL_VALUE(scale, 2.0f));
	Button_Create(&state->buttons[MENU_OPTION], &buttonBlueprints[7], &Settings_ReverseFullscreen, "<", (sfVector2f) { (float)(tempResolution.x / 2.15), (float)(2 * tempResolution.y / 10)}, V2F_MUL_VALUE(scale, 2.0f));
	Button_Create(&state->buttons[MENU_OPTION], &buttonBlueprints[11], &Settings_ReverseFullscreen, ">", (sfVector2f) { (float)(tempResolution.x / 1.8), (float)(2 * tempResolution.y / 10)}, V2F_MUL_VALUE(scale, 2.0f));
	Button_Create(&state->buttons[MENU_OPTION], &buttonBlueprints[7], &Settings_ReverseVsync, "<", (sfVector2f) { (float)(tempResolution.x / 2.15), (float)(3 * tempResolution.y / 10)}, V2F_MUL_VALUE(scale, 2.0f));
	Button_Create(&state->buttons[MENU_OPTION], &buttonBlueprints[11], &Settings_ReverseVsync, ">", (sfVector2f) { (float)(tempResolution.x / 1.8), (float)(3 * tempResolution.y / 10)}, V2F_MUL_VALUE(scale, 2.0f));
	Button_Create(&state->buttons[MENU_OPTION], &buttonBlueprints[7], &Settings_LeftArrowFramerate, "<", (sfVector2f) { (float)(tempResolution.x / 2.15), (float)(4 * tempResolution.y / 10)}, V2F_MUL_VALUE(scale, 2.0f));
	Button_Create(&state->buttons[MENU_OPTION], &buttonBlueprints[11], &Settings_RightArrowFramerate, ">", (sfVector2f) { (float)(tempResolution.x / 1.8), (float)(4 * tempResolution.y / 10)}, V2F_MUL_VALUE(scale, 2.0f));

	Button_Create(&state->buttons[MENU_OPTION], &buttonBlueprints[8], &Settings_MinusMainVolume, "-", (sfVector2f) { (float)(tempResolution.x / 2.15), (float)(5 * tempResolution.y / 10)}, V2F_MUL_VALUE(scale, 2.0f));
	Button_Create(&state->buttons[MENU_OPTION], &buttonBlueprints[10], &Settings_PlusMainVolume, "+", (sfVector2f) { (float)(tempResolution.x / 1.5), (float)(5 * tempResolution.y / 10)}, V2F_MUL_VALUE(scale, 2.0f));
	Button_Create(&state->buttons[MENU_OPTION], &buttonBlueprints[8], &Settings_MinusSoundVolume, "-", (sfVector2f) { (float)(tempResolution.x / 2.15), (float)(6 * tempResolution.y / 10)}, V2F_MUL_VALUE(scale, 2.0f));
	Button_Create(&state->buttons[MENU_OPTION], &buttonBlueprints[10], &Settings_PlusSoundVolume, "+", (sfVector2f) { (float)(tempResolution.x / 1.5), (float)(6 * tempResolution.y / 10)}, V2F_MUL_VALUE(scale, 2.0f));
	Button_Create(&state->buttons[MENU_OPTION], &buttonBlueprints[8], &Settings_MinusMusicVolume, "-", (sfVector2f) { (float)(tempResolution.x / 2.15), (float)(7 * tempResolution.y / 10)}, V2F_MUL_VALUE(scale, 2.0f));
	Button_Create(&state->buttons[MENU_OPTION], &buttonBlueprints[10], &Settings_PlusMusicVolume, "+", (sfVector2f) { (float)(tempResolution.x / 1.5), (float)(7 * tempResolution.y / 10)}, V2F_MUL_VALUE(scale, 2.0f));

	Button_Create(&state->buttons[MENU_OPTION], &buttonBlueprints[9], &MenuFade_OptionToMenu, "ok", (sfVector2f) { (float)(tempResolution.x / 2.5), (float)(9 * tempResolution.y / 10)}, V2F_MUL_VALUE(scale, 2.0f));
	Button_Create(&state->buttons[MENU_OPTION], &buttonBlueprints[5], &Settings_Apply, "apply", (sfVector2f) { (float)(tempResolution.x / 2), (float)(9 * tempResolution.y / 10)}, V2F_MUL_VALUE(scale, 2.0f));
	Button_Create(&state->buttons[MENU_OPTION], &buttonBlueprints[6], &Settings_Credits, "credits", (sfVector2f) { (float)(tempResolution.x * (1 - 1 / 2.5)), (float)(9 * tempResolution.y / 10)}, V2F_MUL_VALUE(scale, 2.0f));

	for (int i = 0; i < state->buttons[MENU_OPTION].number; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			state->buttons[MENU_OPTION].array[i]->states[j].sprite = NULL;
		}
		state->buttons[MENU_OPTION].array[i]->content.font = Text_GetFont(_data->engine.text, "AXIS Extra Bold 800.otf");
	}

	state->resolution = (sfVector2i){ 0, 0 };
	state->currentResolution = 0;

	// Initialize parameters for text in the settings menu
	state->parametersText.size = 30;
	state->parametersText.font = Text_GetFont(_data->engine.text, "AXIS Extra Bold 800.otf");
	state->parametersText.color = sfWhite;

	// -- Init AchievementMenu ----------------------------- //
	/// -- Buttons --- //
	// --  Next
	Button_Create(&state->buttons[MENU_ACHIEVEMENTS], &buttonBlueprints[14], &AchievementMenu_ArrowPageRight, "", (sfVector2f) { (float)(tempResolution.x * ACHIEVEMENT_MENU_BUTTON_RIGHT_POSITION_MULTIPLICATOR_X), (float)(tempResolution.y * ACHIEVEMENT_MENU_BUTTON_RIGHT_POSITION_MULTIPLICATOR_Y) }, V2F_MUL_VALUE(V2F_FROM_VALUE(ACHIEVEMENT_MENU_SCALE), 1.0f));
	// -- Previous
	Button_Create(&state->buttons[MENU_ACHIEVEMENTS], &buttonBlueprints[13], &AchievementMenu_ArrowPageLeft, "", (sfVector2f) { (float)(tempResolution.x * ACHIEVEMENT_MENU_BUTTON_LEFT_POSITION_MULTIPLICATOR_X), (float)(tempResolution.y * ACHIEVEMENT_MENU_BUTTON_LEFT_POSITION_MULTIPLICATOR_Y) }, V2F_MUL_VALUE(V2F_FROM_VALUE(ACHIEVEMENT_MENU_SCALE), 1.0f));
	// -- Back to main menu
	Button_Create(&state->buttons[MENU_ACHIEVEMENTS], &buttonBlueprints[12], &MenuFade_AchievementToMenu, "", (sfVector2f) { (float)(tempResolution.x * ACHIEVEMENT_MENU_BUTTON_BACKTM_POSITION_MULTIPLICATOR_X), (float)(tempResolution.y * ACHIEVEMENT_MENU_BUTTON_BACKTM_POSITION_MULTIPLICATOR_Y) }, V2F_MUL_VALUE(V2F_FROM_VALUE(0.75f), 1.0f));
	
	/// -- Sprites 
	state->achievementMenu = Sprite_CreateFromFile("Data/Sprites/Interfaces/Achievement/Fond_Bleue.png", 0, 0);
	state->achievementMenuBackground = Sprite_CreateFromFile("Data/Sprites/Backgrounds/Background_OverWorld.png", 0, 0);
	state->achievementSectionUnlockedBackground = Sprite_CreateFromFile("Data/Sprites/Interfaces/Achievement/Cadre_Vert_V2.png", 0, 0);
	state->achievementSectionLockedBackground = Sprite_CreateFromFile("Data/Sprites/Interfaces/Achievement/Cadre_Gris.png", 0, 0);
	state->achievementArrowLeftInput = Sprite_CreateFromFile("Data/Sprites/Interfaces/Achievement/Ldown static 1.png", 0.5f, 0.5f);
	state->achievementArrowRightInput = Sprite_CreateFromFile("Data/Sprites/Interfaces/Achievement/Rdown static.png", 0.5f, 0.5f);
	
	/// -- Sprites size unscaled 
	state->sizeAchievementSectionBackground.x = (float)sfTexture_getSize(sfSprite_getTexture(state->achievementSectionUnlockedBackground)).x;
	state->sizeAchievementSectionBackground.y = (float)sfTexture_getSize(sfSprite_getTexture(state->achievementSectionUnlockedBackground)).y;
	state->sizeAchievementBackground.x = (float)sfTexture_getSize(sfSprite_getTexture(state->achievementMenu)).x - ACHIEVEMENT_MENU_INNERSPRITE_OFFSET_X*2;
	state->sizeAchievementBackground.y = (float)sfTexture_getSize(sfSprite_getTexture(state->achievementMenu)).y - ACHIEVEMENT_MENU_INNERSPRITE_OFFSET_Y*2;
	
	/// -- Variables init
	state->achievementCurrentPage = 1;
	state->achievementNbPerPage = 5;
	state->achievementPageMax = (int)ceil((float)_data->engine.blueprints.achievements.number / (float)state->achievementNbPerPage);
	
	//// Test
	//Achievement_Unclock(&_data->notifications, &_data->engine, "PlaceHolder");

}


// -- LOCAL FUNCTIONS ------------------------------------------------------ //