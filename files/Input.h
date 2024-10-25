#ifndef INPUT__H
#define INPUT__H

// -- INCLUDES ------------------------------------------------------------- //

#include "Includes.h"
#include "Engine.h"

// -- TYPES DEFINITIONS ---------------------------------------------------- //

typedef struct ButtonState ButtonState;
typedef struct Button Button;
typedef enum e_ButtonState e_ButtonState;

// -- ENUMERATIONS --------------------------------------------------------- //

enum e_ButtonStates
{
	INPUT_DISABLED, // When the button cannot have interactions
	INPUT_WAITING, // When the button is waiting for player input
	INPUT_HOVERED, // When the player have his mouse hover the button
	INPUT_PRESSED, // When the player is pressing the button*
};

// -- DEFINES -------------------------------------------------------------- //

#define INPUT_STATE_NB 4

// -- STRUCTURES ----------------------------------------------------------- //

struct IputState
{
	sfSprite *sprite;
	Sound *sound;
};

struct Button
{
	sfVector2f position;
	sfVector2f scale;
	void(*function)(void *_data);
	e_ButtonState currentState;
	ButtonState states[BUTTON_STATE_NB];
	ColliderType colliderType;
	void *collider;
};

// -- SHARED FUNCTIONS DEFINITIONS ----------------------------------------- //

Button Button_InitializeBlueprint(Engine *_engine, const char *_filename);

Button *Button_Create(Button *_blueprint, void *_function, sfVector2f _position, sfVector2f _scale);

void Button_Update(Engine *_engine, Button **_buttons, int _nbButton, sfVector2f _mousePosition);

void Button_Pressing(Engine *_engine, Button **_buttons, int _nbButton, sfVector2f _mousePosition);

void Button_Releasing(Engine *_engine, void *_data, Button **_buttons, int _nbButton, sfVector2f _mousePosition);

void Button_Display(sfRenderWindow *_window, Button **_buttons, int _nbButton);

#endif // !BUTTON__H