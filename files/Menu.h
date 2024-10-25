#ifndef MENU__H
#define MENU__H

// -- INCLUDES ------------------------------------------------------------- //

#include "_MainData.h"
#include "Timer.h"
#include "Achievements.h"
#include "Fade.h"

// -- DEFINES -------------------------------------------------------------- //

#define MENU_TIMER_MAX_INPUT 0.3f
#define MENU_NB_BUTTONS 5
#define SETTINGS_NB_BUTTONS 17
#define SETTINGS_BAR "---------------------"

// -- ENUMERATIONS --------------------------------------------------------- //

typedef enum Menu_SubState Menu_SubState;
enum Menu_SubState
{
	MENU_STARTING,
	MENU_MAIN,
	MENU_OPTION,
	MENU_ACHIEVEMENTS,
	MENU_SUBSTATES_NUMBER
};

// -- STRUCTURES ----------------------------------------------------------- //

// Data used to store all informations about the state
typedef struct MenuData MenuData;
struct MenuData
{
	sfBool usingJoystick;
	int currentButton;
	sfSprite *background;
	Menu_SubState subState;
	Menu_SubState previousSubState;
	Buttons buttons[MENU_SUBSTATES_NUMBER];
	Timer timeSinceInput;
	Text parametersText;
	sfVector2i resolution;
	int currentResolution;

	int achievementPageMax;
	int achievementNbPerPage;
	int achievementCurrentPage;
	sfSprite* achievementMenu;
	sfSprite* achievementMenuBackground;
	sfSprite* achievementArrowLeftInput;
	sfSprite* achievementArrowRightInput;
	sfSprite* achievementSectionUnlockedBackground;
	sfSprite* achievementSectionLockedBackground;
	sfVector2f sizeAchievementSectionBackground;
	sfVector2f sizeAchievementBackground;
};

// -- MAIN FUNCTION DEFINITIONS -------------------------------------------- //

void Menu_Initialize(MainData *_data);
void Menu_HandleEvent(MainData *_data, sfEvent _event);
void Menu_Update(MainData *_data, float _dt);
void Menu_Display(MainData *_data);
void Menu_Exit(MainData *_data);

void Menu_StartGame(MainData *_data);
void Menu_StartOption(MainData *_data);
void Menu_StartAchievement(MainData *_data);
void Menu_StartGame(MainData *_data);
void Menu_Quit(MainData *_data);
void Menu_StartEditor(MainData *_data);

void Settings_Apply(MainData* _data);
void Settings_Credits(MainData* _data);
void Settings_LeftArrowFramerate(MainData* _data);
void Settings_LeftArrowResolution(MainData* _data);
void Settings_MinusMainVolume(MainData* _data);
void Settings_MinusMusicVolume(MainData* _data);
void Settings_MinusSoundVolume(MainData* _data);
void Settings_Ok(MainData* _data);
void Settings_PlusMainVolume(MainData* _data);
void Settings_PlusMusicVolume(MainData* _data);
void Settings_PlusSoundVolume(MainData* _data);
void Settings_ReverseFullscreen(MainData* _data);
void Settings_ReverseVsync(MainData* _data);
void Settings_RightArrowFramerate(MainData* _data);
void Settings_RightArrowResolution(MainData* _data);

void AchievementMenu_ArrowPageRight(MainData* _data); 
void AchievementMenu_ArrowPageLeft(MainData* _data);
void AchievementMenu_BackToMenu(MainData* _data);
void AchievementMenu_Update(MainData *_data);

void MenuFade_MenuToGame(MainData *_data);
void MenuFade_MenuToEditor(MainData *_data);
void MenuFade_MenuToAchievement(MainData *_data);
void MenuFade_MenuToOption(MainData *_data);
void MenuFade_OptionToMenu(MainData *_data);
void MenuFade_AchievementToMenu(MainData *_data);

sfBool Menu_ChangeCurrentButton(Engine *_engine, MenuData *_state, float _position, int _maxNbButtons);

#endif // !MENU__H