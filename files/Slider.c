#include "Slider.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

// -- SHARED FUNCTIONS ----------------------------------------------------- //

Slider Slider_InitializeBlueprint(Engine *_engine, const char *_filename)
{
	Slider blueprint = { 0 };
	FILE *file = NULL;

	file = fopen(_filename, "rt");
	if (file)
	{
		char tmp_string[BUFFER_MEDIUM] = { 0 };
		Text text;
		int red = 0;
		int green = 0;
		int blue = 0;
		int alpha = 0;
		// Initialize the sprites for the cursor
		fscanf(file, "CURSOR FILE_WAITING SPRITE %s\n", tmp_string);
		blueprint.cursorsprites[SLIDER_STATE_WAITING] = Sprite_CreateFromFile(tmp_string, 0.5f, 0.5f);
		fscanf(file, "CURSOR FILE_HOVERED SPRITE %s\n", tmp_string);
		blueprint.cursorsprites[SLIDER_STATE_HOVERED] = Sprite_CreateFromFile(tmp_string, 0.5f, 0.5f);
		fscanf(file, "CURSOR FILE_PRESSED SPRITE %s\n", tmp_string);
		blueprint.cursorsprites[SLIDER_STATE_PRESSED] = Sprite_CreateFromFile(tmp_string, 0.5f, 0.5f);
		// Initialize the background
		fscanf(file, "BACKGROUND SPRITE %s\n", &tmp_string);
		blueprint.backgroundSprite = Sprite_CreateFromFile(tmp_string, 0.5f, 0.5f);
		// Get font infos for title
		fscanf(file, "FONT TITLE %s SIZE %d COLOR %d %d %d %d\n", tmp_string, &text.size, &red, &green, &blue, &alpha);
		text.font = Text_GetFont(_engine->text, tmp_string);
		text.color.r = (sfUint8)red;
		text.color.g = (sfUint8)green;
		text.color.b = (sfUint8)blue;
		text.color.a = (sfUint8)alpha;
		blueprint.titleText = text;
		// Get font infos for values
		fscanf(file, "FONT VALUES %s SIZE %d COLOR %d %d %d %d\n", tmp_string, &text.size, &red, &green, &blue, &alpha);
		text.font = Text_GetFont(_engine->text, tmp_string);
		text.color.r = (sfUint8)red;
		text.color.g = (sfUint8)green;
		text.color.b = (sfUint8)blue;
		text.color.a = (sfUint8)alpha;
		blueprint.valuesText = text;
		return blueprint;
	}
	return blueprint;
}

void Slider_Create(Sliders *_sliders, Slider *_blueprint, const char *_title, sfVector2f _position, sfVector2f _scale, SliderData _values)
{
	sfVector2f sizeBackground;
	Slider *slider = calloc(1, sizeof(Slider));
	float cursorRadius;
	if (slider)
	{
		// Get the sizes of the sprites for the background
		sizeBackground = V2F_MUL_V2F(Sprite_GetSize(_blueprint->backgroundSprite), _scale);
		// Set position title variables
		slider->titlePosition.x = _position.x + sizeBackground.x / 2.0f;
		slider->titlePosition.y = _position.y;
		slider->titleString = _strdup(_title);
		slider->titleText = _blueprint->titleText;
		// Set position background variables
		slider->backgroundPosition.x = _position.x + sizeBackground.x / 2.0f;
		slider->backgroundPosition.y = _position.y + sizeBackground.y / 2.0f + _blueprint->titleText.size + 5;
		slider->backgroundSprite = _blueprint->backgroundSprite;
		// Set positions for the values text
		slider->leftPosition.x = slider->backgroundPosition.x - sizeBackground.x / 2.0f;
		slider->leftPosition.y = slider->backgroundPosition.y + sizeBackground.y / 2.0f + 5;
		slider->centerPosition.x = slider->backgroundPosition.x;
		slider->centerPosition.y = slider->backgroundPosition.y + sizeBackground.y / 2.0f + 5;
		slider->rightPosition.x = slider->backgroundPosition.x + sizeBackground.x / 2.0f;
		slider->rightPosition.y = slider->backgroundPosition.y + sizeBackground.y / 2.0f + 5;
		// Initialize values
		slider->valuesText = _blueprint->valuesText;
		slider->values = _values;
		// Initialize cursor variables
		slider->cursorPosition.x = slider->backgroundPosition.x;
		slider->cursorPosition.y = slider->backgroundPosition.y;
		slider->cursorsprites[SLIDER_STATE_WAITING] = _blueprint->cursorsprites[SLIDER_STATE_WAITING];
		slider->cursorsprites[SLIDER_STATE_HOVERED] = _blueprint->cursorsprites[SLIDER_STATE_HOVERED];
		slider->cursorsprites[SLIDER_STATE_PRESSED] = _blueprint->cursorsprites[SLIDER_STATE_PRESSED];
		slider->cursorState = SLIDER_STATE_WAITING;
		cursorRadius = Sprite_GetSize(slider->cursorsprites[SLIDER_STATE_HOVERED]).x / 2.0f * _scale.x;
		slider->cursorCollider = Collider_CreateCircleCollider(slider->cursorPosition, cursorRadius);
		// Initialize various variables
		slider->scale = _scale;
		// Add the created carousel into the sliders database
		_sliders->array[_sliders->number] = slider;
		_sliders->number++;
	}
	else
	{
		printf("Impossible to create the slider\n");
	}
}

void Slider_Update(Engine *_engine, Sliders *_sliders, sfVector2f _mousePosition)
{
	for (int i = 0; i < _sliders->number; i++)
	{
		// Create a shortcut to the current slider
		Slider *slider = _sliders->array[i];

		float distanceTotal = slider->rightPosition.x - slider->leftPosition.x;
		float distanceFromStart = slider->cursorPosition.x - slider->leftPosition.x;
		float percentage = distanceFromStart / distanceTotal;
		// Calculate current value
		if (slider->values.type == TYPE_INT)
		{
			int valueTotal = slider->values.ints[SLIDER_VALUE_MAXIMUM] - slider->values.ints[SLIDER_VALUE_MINIMUM];
			slider->values.ints[SLIDER_VALUE_CURRENT] = (int)(slider->values.ints[SLIDER_VALUE_MINIMUM] + valueTotal * percentage);
		}
		else if (slider->values.type == TYPE_FLOAT)
		{
			float valueTotal = slider->values.floats[SLIDER_VALUE_MAXIMUM] - slider->values.floats[SLIDER_VALUE_MINIMUM];
			slider->values.floats[SLIDER_VALUE_CURRENT] = slider->values.floats[SLIDER_VALUE_MINIMUM] + valueTotal * percentage;
		}
		// Update cursor position 
		if (Collider_CheckCollisionWithMouse(&slider->cursorCollider, _mousePosition))
		{
			if (sfMouse_isButtonPressed(sfMouseLeft))
			{
				slider->cursorIsMoving = sfTrue;
			}
			else
			{
				slider->cursorState = SLIDER_STATE_HOVERED;
			}
		}
		else
		{
			slider->cursorState = SLIDER_STATE_WAITING;
		}
		if (slider->cursorIsMoving)
		{
			slider->cursorState = SLIDER_STATE_PRESSED;
			if (!sfMouse_isButtonPressed(sfMouseLeft))
			{
				slider->cursorIsMoving = sfFalse;
				slider->cursorState = SLIDER_STATE_WAITING;
			}
			slider->cursorPosition.x = _mousePosition.x;
			if (slider->cursorPosition.x > slider->rightPosition.x)
			{
				slider->cursorPosition.x = slider->rightPosition.x;
			}
			else if (slider->cursorPosition.x < slider->leftPosition.x)
			{
				slider->cursorPosition.x = slider->leftPosition.x;
			}
			Collider_UpdatePosition(&slider->cursorCollider, slider->cursorPosition);
		}
	}
}
void Slider_Display(Engine *_engine, Sliders *_sliders)
{
	//sfSprite *sprite;
	char * tmp_string = NULL;
	for (int i = 0; i < _sliders->number; i++)
	{
		// Create a shortcut to the current slider
		Slider *slider = _sliders->array[i];
		// Display the background
		Sprite_Display(_engine->canvas, slider->backgroundSprite, slider->backgroundPosition, slider->scale, 0, NULL);
		// Display the cursor
		Sprite_Display(_engine->canvas, slider->cursorsprites[slider->cursorState], slider->cursorPosition, slider->scale, 0, NULL);
		// Display title text
		Text_Display(_engine->canvas, &slider->titleText, slider->titleString, slider->titlePosition, V2F(0.5f, 0.0f));
		// Display values text
		for (int i = 0; i < 3; i++)
		{
			char string[BUFFER_EXTRA_SMALL] = { 0 };
			if (slider->values.type == TYPE_INT)
			{
				sprintf(string, "%d", slider->values.ints[i]);
			}
			else
			{
				sprintf(string, "%.3f", slider->values.floats[i]);
			}
			switch (i)
			{
			case SLIDER_VALUE_MINIMUM:
				Text_Display(_engine->canvas, &slider->valuesText, string, slider->leftPosition, V2F(0.0f, 0.0f));
				break;
			case SLIDER_VALUE_MAXIMUM:
				Text_Display(_engine->canvas, &slider->valuesText, string, slider->rightPosition, V2F(1.0f, 0.0f));
				break;
			case SLIDER_VALUE_CURRENT:
				Text_Display(_engine->canvas, &slider->valuesText, string, slider->centerPosition, V2F(0.5f, 0.0f));
				break;
			}
		}
	}
}

int Slider_GetIntValue(Slider *_slider)
{
	return _slider->values.ints[SLIDER_VALUE_CURRENT];
}

float Slider_GetFloatValue(Slider *_slider)
{
	return _slider->values.floats[SLIDER_VALUE_CURRENT];
}

void Slider_SetIntValue(Slider *_slider, int _value)
{
	int valueFromStart = _value - _slider->values.ints[SLIDER_VALUE_MINIMUM];
	int valueTotal = _slider->values.ints[SLIDER_VALUE_MAXIMUM] - _slider->values.ints[SLIDER_VALUE_MINIMUM];
	float width = _slider->rightPosition.x - _slider->leftPosition.x;
	float percentage = (float)valueFromStart / (float)valueTotal;
	_slider->values.ints[SLIDER_VALUE_CURRENT] = _value;
	_slider->cursorPosition.x = _slider->leftPosition.x + width * percentage;
	Collider_UpdatePosition(&_slider->cursorCollider, _slider->cursorPosition);
}

void Slider_SetFloatValue(Slider *_slider, float _value)
{
	float valueFromStart = _value - _slider->values.floats[SLIDER_VALUE_MINIMUM];
	float valueTotal = _slider->values.floats[SLIDER_VALUE_MAXIMUM] - _slider->values.floats[SLIDER_VALUE_MINIMUM];
	float width = _slider->rightPosition.x - _slider->leftPosition.x;
	float percentage = valueFromStart / valueTotal;
	_slider->values.floats[SLIDER_VALUE_CURRENT] = _value;
	_slider->cursorPosition.x = _slider->leftPosition.x + width * percentage;
	Collider_UpdatePosition(&_slider->cursorCollider, _slider->cursorPosition);
}

void Slider_Destroy()
{
	// Destroy the collider
	// Destroy the title string
	// Remove from list
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //
