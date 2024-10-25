#include "Menu.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

void Menu_DisplaySettings(MainData *_data);
void AchievementsMenu_Display(MainData *_data);

// -- MAIN FUNCTION -------------------------------------------------------- //

void Menu_Display(MainData *_data)
{
	// Create a shortcut to the state data
	MenuData *state = _data->stateData[MENU];
	sfVector2f tempScale = _data->engine.configuration->scale;

	if (state->subState == MENU_OPTION)
	{
		Menu_DisplaySettings(_data);
	}
	else if (state->subState == MENU_ACHIEVEMENTS)
	{
		AchievementsMenu_Display(_data);
	}
	else
	{
		Sprite_Display(_data->engine.canvas, state->background, V2F_FROM_VALUE(0.0f), (sfVector2f) { 3.0f * tempScale.x, 3.0f * tempScale.y }, 0.0f, NULL);
	}

	Button_Display(&_data->engine, &state->buttons[state->subState]);
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //

void Menu_DisplaySettings(MainData *_data)
{
	MenuData *state = _data->stateData[MENU];// Create a shortcut to the state data
	sfVector2i tempResolution = _data->engine.configuration->resolution;
	char settings[NB_SETTINGS][BUFFER_SMALL] = { "resolution", "full screen", "vsync", "fps", "main volume", "sound volume", "music volume" };
	char version[BUFFER_SMALL];
	char resolution[BUFFER_SMALL];
	char y_n[2][4] = { "no", "yes" };
	char framerate[BUFFER_SMALL];
	char volume[3][BUFFER_SMALL] = { SETTINGS_BAR, SETTINGS_BAR, SETTINGS_BAR };
	Sprite_Display(_data->engine.canvas, state->achievementMenuBackground, V2F_FROM_VALUE(0.0f), (sfVector2f) { 3.0f * _data->engine.configuration->scale.x, 3.0f * _data->engine.configuration->scale.y }, 0.0f, NULL);
	for (int i = 0; i < NB_SETTINGS; i++)
	{
		Text_Display(_data->engine.canvas, &state->parametersText, settings[i], (sfVector2f) { (float)(tempResolution.x / 2.5f), (float)((1 + i) * tempResolution.y / 10) }, (sfVector2f) { 1.0f, 0.5f });
	}
	sprintf(version, "galactic overdrive v%s", _data->engine.configuration->version);
	Text_Display(_data->engine.canvas, &state->parametersText, version, (sfVector2f) { (float)tempResolution.x, (float)tempResolution.y }, (sfVector2f) { 1.1f, 2.0f });
	sprintf(resolution, "%dx%d", state->resolution.x, state->resolution.y);
	Text_Display(_data->engine.canvas, &state->parametersText, resolution, (sfVector2f) { (float)(tempResolution.x / 2), (float)(tempResolution.y / 10) }, (sfVector2f) { 0.0f, 0.5f });
	Text_Display(_data->engine.canvas, &state->parametersText, y_n[_data->engine.configuration->fullscreen], (sfVector2f) { (float)(tempResolution.x / 2), (float)(2 * tempResolution.y / 10) }, (sfVector2f) { 0.0f, 0.5f });
	Text_Display(_data->engine.canvas, &state->parametersText, y_n[_data->engine.configuration->vsync], (sfVector2f) { (float)(tempResolution.x / 2), (float)(3 * tempResolution.y / 10) }, (sfVector2f) { 0.0f, 0.5f });

	sprintf(framerate, "%d", _data->engine.configuration->framerate);
	Text_Display(_data->engine.canvas, &state->parametersText, framerate, (sfVector2f) { (float)(tempResolution.x / 2), (float)(4 * tempResolution.y / 10) }, (sfVector2f) { 0.0f, 0.5f });
	volume[0][(int)(20 * _data->engine.configuration->globalVolume / 100)] = '|';
	Text_Display(_data->engine.canvas, &state->parametersText, volume[0], (sfVector2f) { (float)(tempResolution.x / 2), (float)(5 * tempResolution.y / 10) }, (sfVector2f) { 0.0f, 0.5f });
	volume[1][(int)(20 * _data->engine.configuration->effectVolume / 100)] = '|';
	Text_Display(_data->engine.canvas, &state->parametersText, volume[1], (sfVector2f) { (float)(tempResolution.x / 2), (float)(6 * tempResolution.y / 10) }, (sfVector2f) { 0.0f, 0.5f });
	volume[2][(int)(20 * _data->engine.configuration->musicVolume / 100)] = '|';
	Text_Display(_data->engine.canvas, &state->parametersText, volume[2], (sfVector2f) { (float)(tempResolution.x / 2), (float)(7 * tempResolution.y / 10) }, (sfVector2f) { 0.0f, 0.5f });
}

void AchievementsMenu_Display(MainData *_data)
{
	MenuData *state = _data->stateData[MENU];// Create a shortcut to the state data
	BP_Achievements* BP_Achievement = &_data->engine.blueprints.achievements;
	sfVector2f positionArrowLeft = { (float)(_data->engine.configuration->resolution.x * ACHIEVEMENT_MENU_BUTTON_LEFT_POSITION_MULTIPLICATOR_X), (float)(_data->engine.configuration->resolution.y * ACHIEVEMENT_MENU_BUTTON_LEFT_POSITION_MULTIPLICATOR_Y) };
	sfVector2f positionArrowRight = { (float)(_data->engine.configuration->resolution.x * ACHIEVEMENT_MENU_BUTTON_RIGHT_POSITION_MULTIPLICATOR_X), (float)(_data->engine.configuration->resolution.y * ACHIEVEMENT_MENU_BUTTON_RIGHT_POSITION_MULTIPLICATOR_Y) };
	sfVector2f positionBackToMenu = { (float)(_data->engine.configuration->resolution.x * ACHIEVEMENT_MENU_BUTTON_BACKTM_POSITION_MULTIPLICATOR_X), (float)(_data->engine.configuration->resolution.y * ACHIEVEMENT_MENU_BUTTON_BACKTM_POSITION_MULTIPLICATOR_Y) };

	// -- Draw background ------------------------------------------
	Sprite_Display(_data->engine.canvas, state->achievementMenuBackground, V2F_FROM_VALUE(0.0f), (sfVector2f) { 3.0f * _data->engine.configuration->scale.x, 3.0f * _data->engine.configuration->scale.y }, 0.0f, NULL);

	// -- Draw Menu ------------------------------------------------
	/// -- Background
	sfVector2f positionMenu = { 0, 0 };
	Sprite_Display(_data->engine.canvas, state->achievementMenu, positionMenu, V2F(ACHIEVEMENT_MENU_SCALE, ACHIEVEMENT_MENU_SCALE), 0, NULL);
	/// -- Draw page text
	char txtPage[20] = { 0 };
	sprintf(txtPage, "%d/%d", state->achievementCurrentPage, state->achievementPageMax);
	sfVector2f textPagePosition = { _data->engine.configuration->resolution.x * 0.50f,  _data->engine.configuration->resolution.y *0.1f };
	Text textPageParameters;
	textPageParameters.font = BP_Achievement->array[0]->parametersTextDesc.font;
	textPageParameters.size = 22 * _data->engine.configuration->resolution.x / 1920;
	textPageParameters.color = sfWhite;
	Text_Display(_data->engine.canvas, &textPageParameters, txtPage, textPagePosition, V2F(0.5f, 0));
	/// -- Arrow left Input
	Sprite_Display(_data->engine.canvas, state->achievementArrowLeftInput, V2F(positionArrowLeft.x + (ACHIEVEMENT_MENU_BUTTON_ARROW_LEFT_SIZE_X + ACHIEVEMENT_MENU_BUTTON_INPUT_LEFT_SIZE_X / 2) * ACHIEVEMENT_MENU_SCALE, positionArrowLeft.y + ACHIEVEMENT_MENU_BUTTON_INPUT_LEFT_SIZE_Y / 2), V2F_FROM_VALUE(ACHIEVEMENT_MENU_SCALE), 0, NULL);
	Sprite_Display(_data->engine.canvas, state->achievementArrowRightInput, V2F(positionArrowRight.x - (ACHIEVEMENT_MENU_BUTTON_ARROW_LEFT_SIZE_X/2 + ACHIEVEMENT_MENU_BUTTON_INPUT_LEFT_SIZE_X / 2) * ACHIEVEMENT_MENU_SCALE, positionArrowLeft.y + ACHIEVEMENT_MENU_BUTTON_INPUT_LEFT_SIZE_Y / 2), V2F_FROM_VALUE(ACHIEVEMENT_MENU_SCALE), 0, NULL);


	/// Achievements
	int indexStart = (state->achievementCurrentPage - 1)* state->achievementNbPerPage;

	for (int i = indexStart; i < indexStart + state->achievementNbPerPage; i++)
	{
		if (i < BP_Achievement->number)
		{
			Achievement* currentAchievement = BP_Achievement->array[i];
			int currentAchievementHeight = i % state->achievementNbPerPage;
			// -- Draw Section ---------- //
			sfVector2f positionSection = {
				positionMenu.x + ACHIEVEMENT_MENU_INNERSPRITE_OFFSET_X * ACHIEVEMENT_MENU_SCALE + ACHIEVEMENT_MENU_MARGIN_BORDER_X * ACHIEVEMENT_MENU_SCALE,
				positionMenu.y + (ACHIEVEMENT_MENU_INNERSPRITE_OFFSET_Y* ACHIEVEMENT_MENU_SCALE) + currentAchievementHeight * state->sizeAchievementSectionBackground.y*ACHIEVEMENT_MENU_SCALE + ACHIEVEMENT_MENU_MARGIN_BORDER_Y * ACHIEVEMENT_MENU_SCALE
			};
			/// -- Draw icon
			sfVector2f positionIcon = { positionSection.x + ACHIEVEMENT_MENU_SECTION_INNERSPRITE_OFFSET_X* ACHIEVEMENT_MENU_SCALE ,positionSection.y + ACHIEVEMENT_MENU_SECTION_INNERSPRITE_OFFSET_Y * ACHIEVEMENT_MENU_SCALE };
			if (currentAchievement->icon_Sprite != NULL)
			{
				sfSprite_setColor(currentAchievement->icon_Sprite, sfWhite);
			}
			Sprite_Display(_data->engine.canvas, currentAchievement->icon_Sprite, positionIcon, V2F(ACHIEVEMENT_MENU_SCALE, ACHIEVEMENT_MENU_SCALE), 0, NULL);
			/// -- Draw title
			sfVector2f positionTitle = { positionIcon.x + (currentAchievement->icon_Size.x + ACHIEVEMENT_MENU_TEXT_MARGIN_LEFT) * ACHIEVEMENT_MENU_SCALE ,positionIcon.y + ACHIEVEMENT_MENU_TEXT_MARGIN_TOP * ACHIEVEMENT_MENU_SCALE };
			Text textTitleParameters;
			textTitleParameters.font = currentAchievement->parametersTextTitle.font;
			textTitleParameters.size = ACHIEVEMENT_MENU_TITLE_FONT_SIZE * _data->engine.configuration->resolution.x / 1920;
			textTitleParameters.color = sfWhite;
			Text_Display(_data->engine.canvas, &textTitleParameters, currentAchievement->title, positionTitle, V2F(0, 0));
			/// -- Draw description			
			sfVector2f positionDesc = { positionTitle.x,positionTitle.y + textTitleParameters.size + state->sizeAchievementSectionBackground.y*ACHIEVEMENT_MENU_DESC_OFFSET_MULTIPLICATOR_Y };
			Text textDescParameters;
			textDescParameters.font = currentAchievement->parametersTextDesc.font;
			textDescParameters.size = ACHIEVEMENT_MENU_DESC_FONT_SIZE * _data->engine.configuration->resolution.x / 1920;
			textDescParameters.color = sfWhite;
			Text_Display(_data->engine.canvas, &textDescParameters, currentAchievement->description, positionDesc, V2F(0, 0));
			/// -- Draw Frame
			if (currentAchievement->isUnlocked)
			{
				Sprite_Display(_data->engine.canvas, state->achievementSectionUnlockedBackground, positionSection, V2F(ACHIEVEMENT_MENU_SCALE, ACHIEVEMENT_MENU_SCALE), 0, NULL);
			}
			else
			{
				Sprite_Display(_data->engine.canvas, state->achievementSectionLockedBackground, positionSection, V2F(ACHIEVEMENT_MENU_SCALE, ACHIEVEMENT_MENU_SCALE), 0, NULL);
			}
		}
	}
}