#ifndef MAINDATA__H
#define MAINDATA__H

// -- INCLUDES ------------------------------------------------------------- //

#include "_Engine.h"
#include "Button.h"
#include "Timer.h"


// -- ENUMERATIONS --------------------------------------------------------- //

// -- TYPES DEFINITIONS ---------------------------------------------------- //

typedef struct MainData MainData;
typedef struct GameStatesFunctions GameStatesFunctions;

// -- DEFINES -------------------------------------------------------------- //

#define INITIAL_GAMESTATE SPLASH

// -- STRUCTURES ----------------------------------------------------------- //

typedef struct TransitionFade TransitionFade;
struct TransitionFade
{
	sfSprite* spriteFade;
	sfColor color;
	FadeState state;
	float timer;
	void(*ptrChangeState)(MainData*);
};

struct GameStatesFunctions
{
	// Pointer to the game states "Initialize" function
	void(*Initialize[NB_GAMESTATE])(MainData *_data);

	// Pointer to the game states "HandleEvent" function
	void(*HandleEvent[NB_GAMESTATE])(MainData *_data, sfEvent _event);

	// Pointer to the game states "Update" function
	void(*Update[NB_GAMESTATE])(MainData *_data, float _dt);

	// Pointer to the game states "Display" function
	void(*Display[NB_GAMESTATE])(MainData *_data);

	// Pointer to the game states "Exit" function
	void(*Exit[NB_GAMESTATE])(MainData *_data);
};

// Main data used to store all informations about the game
struct MainData
{
	Engine engine;
	GameState currentState;
	GameStatesFunctions statesFunctions; // Store the functions of all the events
	void *stateData[NB_GAMESTATE]; // Pointers to each states data
	int choiceSpaceship;
	int choosenColors[MAX_PLAYER_NUMBER];
	int nbColors;
	sfColor *spaceShipColors;
	char **spaceShipColorsNames;
	int choosenSpecials[MAX_PLAYER_NUMBER];
	Players customizedPlayers;
	Notifications notifications;
	AchievementVariablesToUnlock achievementsUnlockVar;
	TransitionFade transitionFade;
	sfBool isLaunchedFromEditor;
	char levelName[BUFFER_LARGE];
};

#endif // !MAINDATA__H