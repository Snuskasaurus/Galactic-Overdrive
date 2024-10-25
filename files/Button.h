#ifndef BUTTON__H
#define BUTTON__H

// -- INCLUDES ------------------------------------------------------------- //

#include "_Engine.h"

#include "Collider.h"

// -- TYPES DEFINITIONS ---------------------------------------------------- //


// -- ENUMERATIONS --------------------------------------------------------- //

typedef enum e_ButtonState e_ButtonState;
enum e_ButtonStates
{
	BUTTON_DISABLED, // When the button cannot have interactions
	BUTTON_WAITING, // When the button is waiting for player input
	BUTTON_HOVERED, // When the player have his mouse hover the button
	BUTTON_PRESSED, // When the player is pressing the button
	BUTTON_RELEASED // When the player is releasing the button
};

// -- DEFINES -------------------------------------------------------------- //

#define BUTTON_STATE_NB 5
#define BUTTON_MAXIMUM_NUMBER 30

// -- STRUCTURES ----------------------------------------------------------- //

typedef struct ButtonState ButtonState;
struct ButtonState
{
	sfSprite *sprite;
	Sound *sound;
	sfColor textColor;
};

typedef struct Button Button;
struct Button
{
	sfVector2f position;
	sfVector2f scale;
	void(*function)(void *_data);
	e_ButtonState currentState;
	ButtonState states[BUTTON_STATE_NB];
	Collider collider;
	sfBool manual;
	char *stringContent;
	Text content;

	sfSound* sound;
	float soundTimer;
	sfBool wasButtonReleased;
};

typedef struct Buttons Buttons;
struct Buttons
{
	int number;
	Button *array[BUTTON_MAXIMUM_NUMBER];
};

// -- SHARED FUNCTIONS DEFINITIONS ----------------------------------------- //

Button Button_InitializeBlueprint(Engine *_engine, const char *_filename);

void Button_Create(Buttons *_buttons, Button *_blueprint, void *_function, const char *_text, sfVector2f _position, sfVector2f _scale);

void Button_Update(Engine *_engine, Buttons *_buttons, sfVector2f _mousePosition);

void Button_Pressing(Engine *_engine, Buttons *_buttons);

void Button_Releasing(Engine *_engine, void *_data, Buttons *_buttons);

void Button_Display(Engine *_engine, Buttons *_buttons);

void Button_SetManual(Buttons *_buttons, sfBool _manual);

void Button_Select(Engine *_engine, Buttons *_buttons, Button *_button);

void Button_Press(Engine *_engine, Buttons *_buttons, Button *_button, void *_data);

void Button_SetText(Engine *_engine, Button *_button, const char *_text);

sfBool Button_Destroy(Button *_button);

void Button_DeleteAll(Buttons *_buttons);

#endif // !BUTTON__H