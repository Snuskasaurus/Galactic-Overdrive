#include "Button.h"

// -- SHARED FUNCTIONS ----------------------------------------------------- //

Button Button_InitializeBlueprint(Engine *_engine, const char *_filename)
{
	Button blueprint;
	FILE *file = NULL;
	char collider_filename[BUFFER_SIZE] = { 0 };
	char font_filename[BUFFER_SIZE] = { 0 };
	int font_size = 10;

	file = fopen(_filename, "r+");
	if (file)
	{
		for (int state = 0; state < BUTTON_STATE_NB; state++)
		{
			char type[BUFFER_SIZE] = { 0 };
			char sprite_filename[BUFFER_SIZE] = { 0 };
			char sound_filename[BUFFER_SIZE] = { 0 };
			int red = 0;
			int green = 0;
			int blue = 0;
			int alpha = 0;

			// Get the datas
			fscanf(file, "%s SPRITE %s SOUND %s COLOR %d %d %d %d\n", type, sprite_filename, sound_filename, &red, &green, &blue, &alpha);
			///printf("rgb loaded : %d %d %d %d\n", red, green, blue, alpha);
			// Load the sprite
			if (_stricmp(sprite_filename, "NULL") == 0 || sprite_filename[0] == '\0')
			{
				blueprint.states[state].sprite = NULL;
			}
			else
			{
				blueprint.states[state].sprite = Sprite_CreateFromFile(sprite_filename, 0.5f, 0.5f);
			}
			// Load the sound
			if (_stricmp(sound_filename, "NULL") == 0 || sound_filename[0] == '\0')
			{
				blueprint.states[state].sound = NULL;
			}
			else
			{
				Sound_CreateFromFile(_engine->sound, sound_filename, SOUND_EFFECT);
				strcpy(sound_filename, strrchr(sound_filename, '/') + 1);
				blueprint.states[state].sound = Sound_GetFromName(_engine->sound, sound_filename);
			}
			// Load the colors
			blueprint.states[state].textColor.r = red;
			blueprint.states[state].textColor.g = green;
			blueprint.states[state].textColor.b = blue;
			blueprint.states[state].textColor.a = alpha;

		}
		// Initialize the collider
		fscanf(file, "COLLIDER %s\n", collider_filename);
		if (_stricmp(collider_filename, "NULL") == 0 || collider_filename[0] == '\0')
		{
			// Rectangle a rectangle collider when default value
			blueprint.collider = Collider_CreateRectangleCollider(V2F(0.0f, 0.0f), Sprite_GetSize(blueprint.states[BUTTON_WAITING].sprite), TRUE);
		}
		else
		{
			//blueprint.colliderType = COLLIDER_IMAGE;
			//blueprint.collider = sfImage_createFromFile(collider_filename);
		}
		// Initialize the font
		fscanf(file, "FONT %s %d\n", font_filename, &blueprint.content.size);
		if (_stricmp(font_filename, "NULL") == 0 || font_filename[0] == '\0')
		{
			blueprint.content.font = Text_GetFont(_engine->text, "Arial.ttf");
			blueprint.content.size = 30;
		}
		else
		{
			blueprint.content.font = Text_GetFont(_engine->text, font_filename);
		}
	}
	else
	{
		printf("Could not open %s\n", _filename);
	}
	blueprint.scale = (sfVector2f) { 1.0f, 1.0f };
	return blueprint;
}

void Button_Create(Buttons *_buttons, Button *_blueprint, void *_function, const char *_text, sfVector2f _position, sfVector2f _scale)
{
	Button *button = NULL;
	// Allocate memory
	if (_buttons->number + 1 <= BUTTON_MAXIMUM_NUMBER)
	{
		_buttons->number++;
		_buttons->array[_buttons->number - 1] = calloc(1, sizeof(Button));;
		button = _buttons->array[_buttons->number - 1];
		if (button)
		{
			// Set the states
			button->states[BUTTON_DISABLED] = _blueprint->states[BUTTON_DISABLED];
			button->states[BUTTON_WAITING] = _blueprint->states[BUTTON_WAITING];
			button->states[BUTTON_HOVERED] = _blueprint->states[BUTTON_HOVERED];
			button->states[BUTTON_PRESSED] = _blueprint->states[BUTTON_PRESSED];
			button->states[BUTTON_RELEASED] = _blueprint->states[BUTTON_RELEASED];
			// Set various variables
			button->scale = _blueprint->scale;
			button->scale.x = _blueprint->scale.x * _scale.x;
			button->scale.y = _blueprint->scale.y * _scale.y;
			button->position.x = _position.x + Sprite_GetSize(button->states[BUTTON_WAITING].sprite).x * _blueprint->scale.x / 2.0f;
			button->position.y = _position.y + Sprite_GetSize(button->states[BUTTON_WAITING].sprite).y * _blueprint->scale.y / 2.0f;
			button->function = _function;
			// Set the collider
			button->collider = Collider_Copy(&_blueprint->collider);
			// Set if enabled or not
			if (!button->function)
			{
				button->currentState = BUTTON_DISABLED;
			}
			else
			{
				button->currentState = BUTTON_WAITING;
			}
			// Initialize various variables
			button->stringContent = _strdup(_text);
			button->content = _blueprint->content;
		}
		else
		{
			printf("Error to allocate memory for a button\n");
		}
	}
}

void Button_Update(Engine *_engine, Buttons *_buttons, sfVector2f _mousePosition)
{
	Button *button = NULL;
	//sfVector2f buttonPosition;
	//sfVector2f buttonSize;
	for (int i = 0; i < _buttons->number; i++)
	{
		// Create a shortcut to the current button
		button = _buttons->array[i];
		if (button->wasButtonReleased && button->states[BUTTON_RELEASED].sound != NULL)
		{
			button->wasButtonReleased = sfFalse;
			button->soundTimer = sfTime_asSeconds(sfSoundBuffer_getDuration(sfSound_getBuffer(button->sound)));
		}
		button->soundTimer -= _engine->dt;
		if (!button->manual)
		{
			if (button->currentState != BUTTON_DISABLED)
			{
				Collider_UpdatePosition(&button->collider, button->position);
				if (Collider_CheckCollisionWithMouse(&button->collider, _mousePosition))
				{
					// Set the button to hovered if the mouse is not pressed
					if (!sfMouse_isButtonPressed(sfMouseLeft) && button->currentState != BUTTON_HOVERED)
					{
						button->currentState = BUTTON_HOVERED;
						if (button->soundTimer < 0)
						{
							Sound_Play(_engine->sound, button->states[BUTTON_HOVERED].sound);
						}
					}
				}
				else
				{
					button->currentState = BUTTON_WAITING;
				}
			}
		}
	}
}

void Button_Pressing(Engine *_engine, Buttons *_buttons)
{
	Button *button = NULL;
	for (int i = 0; i < _buttons->number; i++)
	{
		// Create a shortcut to the current button
		button = _buttons->array[i];
		// Change state to pressed if the button was in hovered state
		if (button->currentState == BUTTON_HOVERED)
		{
			button->currentState = BUTTON_PRESSED;
			Sound_Play(_engine->sound, button->states[BUTTON_PRESSED].sound);
		}
	}
}

void Button_Releasing(Engine *_engine, void *_data, Buttons *_buttons)
{
	Button *button = NULL;
	for (int i = 0; i < _buttons->number; i++)
	{
		// Create a shortcut to the current button
		button = _buttons->array[i];
		// Start the atributed function if the button was in pressed state
		if (button->currentState == BUTTON_PRESSED)
		{
			button->currentState = BUTTON_RELEASED;
			button->sound = Sound_Play(_engine->sound, button->states[BUTTON_RELEASED].sound);
			button->wasButtonReleased = sfTrue;
			button->function(_data);
		}
	}
}

void Button_Display(Engine *_engine, Buttons *_buttons)
{
	Button *button = NULL;
	sfSprite *sprite;
	//sfColor color;
	for (int i = 0; i < _buttons->number; i++)
	{
		button = _buttons->array[i];
		sprite = button->states[button->currentState].sprite;
		button->content.color = button->states[button->currentState].textColor;
		Sprite_Display(_engine->canvas, sprite, button->position, button->scale, 0, NULL);
		Text_Display(_engine->canvas, &button->content, button->stringContent, button->position, V2F_FROM_VALUE(0.5f));
	}
}

void Button_SetManual(Buttons *_buttons, sfBool _manual)
{
	for (int i = 0; i < _buttons->number; i++)
	{
		_buttons->array[i]->currentState = BUTTON_WAITING;
		_buttons->array[i]->manual = _manual;
	}
}

void Button_Select(Engine *_engine, Buttons *_buttons, Button *_button)
{
	Button_SetManual(_buttons, sfTrue);
	Sound_Play(_engine->sound, _button->states[BUTTON_HOVERED].sound);
	_button->currentState = BUTTON_HOVERED;
}

void Button_Press(Engine *_engine, Buttons *_buttons, Button *_button, void *_data)
{
	Button_SetManual(_buttons, sfTrue);
	_button->currentState = BUTTON_RELEASED;
	Sound_Play(_engine->sound, _button->states[BUTTON_RELEASED].sound);
	_button->function(_data);
}

void Button_SetText(Engine *_engine, Button *_button, const char *_text)
{
	free(_button->stringContent);
	_button->stringContent = _strdup(_text);
}

// Return true if deletion successful...
sfBool Button_Destroy(Button *_button)
{
	if (_button != NULL)
	{
		for (int i = 0; i < BUTTON_STATE_NB; i++)
		{
			/*if (_button->states[i].sound != NULL)
			{
				if (_button->states[i].sound->name != NULL)
				{
					free(_button->states[i].sound->name);
					_button->states[i].sound->name = NULL;
				}
				if (_button->states[i].sound->buffer != NULL)
				{
					sfSoundBuffer_destroy(_button->states[i].sound->buffer);
					_button->states[i].sound->buffer = NULL;
				}
				_button->states[i].sound = NULL;
			}*/
			if (_button->states[i].sprite != NULL)
			{
				sfSprite_destroy(_button->states[i].sprite);
				_button->states[i].sprite = NULL;
			}
		}
		/*if (_button->sound != NULL)
		{
			sfSound_destroy(_button->sound);
			_button->sound = NULL;
		}*/
		if (_button->stringContent != NULL)
		{
			free(_button->stringContent);
			_button->stringContent = NULL;
		}
		Collider_Destroy(&_button->collider);
		free(_button);
		_button = NULL;

		return sfTrue;
	}
	return sfFalse;
}

void Button_DeleteAll(Buttons *_buttons)
{
	if (_buttons != NULL)
	{
		for (int i = 0; i < _buttons->number; i++)
		{
			Button_Destroy(_buttons->array[i]);
		}
	}
}