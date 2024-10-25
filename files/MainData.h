#ifndef MAINDATA__H
#define MAINDATA__H

// -- INCLUDES ------------------------------------------------------------- //

#include "_Includes.h"

#include "_Engine.h"
#include "Button.h"

// -- ENUMERATIONS --------------------------------------------------------- //

// -- TYPES DEFINITIONS ---------------------------------------------------- //

typedef struct MainData MainData;
typedef enum GameState GameState;
typedef struct GameStatesFunctions GameStatesFunctions;

// -- DEFINES -------------------------------------------------------------- //

#define INITIAL_GAMESTATE MENU
#define NB_GAMESTATE 4

// -- STRUCTURES ----------------------------------------------------------- //

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
	sfRenderWindow *window; // The main Window of the game
	sfRenderTexture *canvas; // The canvas of the used by shaders
	sfClock *clock; // Clock used to calculate dt.
	Engine engine;
	GameState currentState;
	GameStatesFunctions statesFunctions; // Store the functions of all the events
	void *stateData[NB_GAMESTATE]; // Pointers to each states data
};

#endif // !MAINDATA__H