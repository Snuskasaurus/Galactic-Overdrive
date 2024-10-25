#ifndef SLIDER__H
#define SLIDER__H

// -- INCLUDES ------------------------------------------------------------- //

#include "_Engine.h"

#include "Collider.h"

// -- TYPES DEFINITIONS ---------------------------------------------------- //

// -- ENUMERATIONS --------------------------------------------------------- //

typedef enum Slider_Value Slider_Value;
enum Slider_Value
{
	SLIDER_VALUE_MINIMUM,
	SLIDER_VALUE_MAXIMUM,
	SLIDER_VALUE_CURRENT,
	SLIDER_VALUE_NUMBER
};

typedef enum Slider_State Slider_State;
enum Slider_State
{
	SLIDER_STATE_WAITING,
	SLIDER_STATE_HOVERED,
	SLIDER_STATE_PRESSED,
	SLIDER_STATE_NUMBER
};

// -- DEFINES -------------------------------------------------------------- //

// -- STRUCTURES ----------------------------------------------------------- //

typedef struct SliderData SliderData;
struct SliderData
{
	VariableType type;
	int increments;
	// for float variables
	float floats[SLIDER_VALUE_NUMBER];
	// for int variables
	int ints[SLIDER_VALUE_NUMBER];
};

typedef struct Slider Slider;
struct Slider
{
	// Various variables
	sfVector2f scale;
	// Variables about the background
	sfSprite *backgroundSprite;
	sfVector2f backgroundPosition;
	// Variables about the cursor
	sfVector2f cursorPosition;
	sfSprite *cursorsprites[SLIDER_STATE_NUMBER];
	Slider_State cursorState; 
	Collider cursorCollider;
	sfBool cursorIsMoving;
	// Variables about values
	Text valuesText;
	SliderData values;
	sfVector2f leftPosition;
	sfVector2f centerPosition;
	sfVector2f rightPosition;
	// Variables about title
	char *titleString;
	sfVector2f titlePosition;
	Text titleText;
};

typedef struct Sliders Sliders;
struct Sliders
{
	int number;
	Slider *array[SLIDER_MAXIMUM_NUMBER];
};

// -- SHARED FUNCTIONS DEFINITIONS ----------------------------------------- //

Slider Slider_InitializeBlueprint(Engine *_engine, const char *_filename);
void Slider_Create(Sliders *_sliders, Slider *_blueprint, const char *_title, sfVector2f _position, sfVector2f _scale, SliderData _values);
void Slider_Update(Engine *_engine, Sliders *_sliders, sfVector2f _mousePosition);
void Slider_Display(Engine *_engine, Sliders *_sliders);

int Slider_GetIntValue(Slider *_slider);
float Slider_GetFloatValue(Slider *_slider);

void Slider_SetIntValue(Slider *_slider, int _value);
void Slider_SetFloatValue(Slider *_slider, float _value);

#endif // !SLIDER__H