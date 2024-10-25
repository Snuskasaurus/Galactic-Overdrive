#include "Carousel.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

void Carousel_PressingLeftButton(Carousel *_carousel);
void Carousel_PressingRightButton(Carousel *_carousel);

// -- SHARED FUNCTIONS ----------------------------------------------------- //

Carousel Carousel_InitializeBlueprint(Engine *_engine, const char *_filename)
{
	Carousel blueprint = { 0 };
	FILE *file = NULL;

	file = fopen(_filename, "rt");
	if (file)
	{
		char tmp_string[BUFFER_MEDIUM] = { 0 };
		int red = 0;
		int green = 0;
		int blue = 0;
		int alpha = 0;
		// Initialize the blueprints buttons
		fscanf(file, "BUTTON_LEFT FILE %s\n", &tmp_string);
		blueprint.buttonBlueprints[CAROUSEL_BUTTON_LEFT] = Button_InitializeBlueprint(_engine, tmp_string);
		fscanf(file, "BUTTON_RIGHT FILE %s\n", &tmp_string);
		blueprint.buttonBlueprints[CAROUSEL_BUTTON_RIGHT] = Button_InitializeBlueprint(_engine, tmp_string);
		// Initialize the background
		fscanf(file, "BACKGROUND SPRITE %s\n", &tmp_string);
		blueprint.background = Sprite_CreateFromFile(tmp_string, 0.5f, 0.5f);
		// Get font infos for title
		fscanf(file, "FONT TITLE %s SIZE %d COLOR %d %d %d %d\n", &tmp_string, &blueprint.title.size, &red, &green, &blue, &alpha);
		blueprint.title.font = Text_GetFont(_engine->text, tmp_string);
		blueprint.title.color.r = (sfUint8)red;
		blueprint.title.color.g = (sfUint8)green;
		blueprint.title.color.b = (sfUint8)blue;
		blueprint.title.color.a = (sfUint8)alpha;
		// Get font infos for content
		fscanf(file, "FONT CONTENT %s SIZE %d COLOR %d %d %d %d\n", &tmp_string, &blueprint.content.size, &red, &green, &blue, &alpha);
		blueprint.content.font = Text_GetFont(_engine->text, tmp_string);
		blueprint.content.color.r = (sfUint8)red;
		blueprint.content.color.g = (sfUint8)green;
		blueprint.content.color.b = (sfUint8)blue;
		blueprint.content.color.a = (sfUint8)alpha;
		return blueprint;
	}
	return blueprint;
}

void Carousel_Create(Carousels *_carousels, Carousel *_blueprint, const char *_title, sfVector2f _position, sfVector2f _scale, CarouselData _carouselData)
{
	Carousel *carousel = calloc(1, sizeof(Carousel));

	sfVector2f sizeButton;
	sfVector2f sizeBackground;
	sfVector2f positionLeftButton;
	sfVector2f positionRightButton;
	if (carousel)
	{
		// Get the sizes of the sprites for the backgroudn and the buttons
		sizeBackground = V2F_MUL_V2F(Sprite_GetSize(_blueprint->background), _scale);
		sizeButton = V2F_MUL_V2F(Sprite_GetSize(_blueprint->buttonBlueprints[CAROUSEL_BUTTON_LEFT].states[0].sprite), _scale);
		// Set position of the text
		carousel->positionTitle.x = _position.x + sizeButton.x + sizeBackground.x / 2.0f;
		carousel->positionTitle.y = _position.y;
		// Set position of the carousel
		carousel->position.x = _position.x + sizeButton.x + sizeBackground.x / 2.0f;
		carousel->position.y = _position.y + sizeBackground.y / 2.0f + _blueprint->title.size + 5 * _scale.y;
		// Set the position of the left button
		positionLeftButton.x = _position.x;
		positionLeftButton.y = _position.y + _blueprint->title.size + 5 * _scale.y;
		// Set the position of the right button
		positionRightButton.x = _position.x + sizeButton.x + sizeBackground.x;
		positionRightButton.y = _position.y + _blueprint->title.size + 5 * _scale.y;
		// Create the buttons
		Button_Create(&carousel->buttons, &_blueprint->buttonBlueprints[CAROUSEL_BUTTON_LEFT], &Carousel_PressingRightButton, "", positionLeftButton, _scale);
		Button_Create(&carousel->buttons, &_blueprint->buttonBlueprints[CAROUSEL_BUTTON_RIGHT], &Carousel_PressingLeftButton, "", positionRightButton, _scale);
		// Initialize various variables
		carousel->background = _blueprint->background;
		carousel->data = _carouselData;
		carousel->scale = _scale;
		carousel->stringTitle = _strdup(_title);
		carousel->content = _blueprint->content;
		carousel->title = _blueprint->title;
		// Add the created carousel into the carousels database
		_carousels->array[_carousels->number] = carousel;
		_carousels->number++;
	}
	else
	{
		printf("Impossible to create the carousel\n");
	}
}

void Carousel_Update(Engine *_engine, Carousels *_carousels, sfVector2f _mousePosition)
{
	for (int i = 0; i < _carousels->number; i++)
	{
		// Create a shortcut to the current carousel
		Carousel *carousel = _carousels->array[i];
		Button_Update(_engine, &carousel->buttons, _mousePosition);
	}
}

void Carousel_Pressing(Engine *_engine, Carousels *_carousels, sfVector2f _mousePosition)
{
	for (int i = 0; i < _carousels->number; i++)
	{
		// Create a shortcut to the current carousel
		Carousel *carousel = _carousels->array[i];
		Button_Pressing(_engine, &carousel->buttons);
	}
}

void Carousel_Releasing(Engine *_engine, Carousels *_carousels, sfVector2f _mousePosition)
{
	for (int i = 0; i < _carousels->number; i++)
	{
		// Create a shortcut to the current carousel
		Carousel *carousel = _carousels->array[i];
		Button_Releasing(_engine, carousel, &carousel->buttons);
	}
}

void Carousel_Display(Engine *_engine, Carousels *_carousels)
{
	Carousel *carousel = NULL;
	//sfSprite *sprite;
	char *string;
	for (int i = 0; i < _carousels->number; i++)
	{
		// Create a shortcut to the current carousel
		carousel = _carousels->array[i];
		// Display the background
		Sprite_Display(_engine->canvas, carousel->background, carousel->position, carousel->scale, 0, NULL);
		// Display title text
		Text_Display(_engine->canvas, &carousel->title, carousel->stringTitle, carousel->positionTitle, V2F(0.5f, 0.0f));
		// Display content text
		string = carousel->data.names[carousel->data.current];
		Text_Display(_engine->canvas, &carousel->content, string, carousel->position, V2F_FROM_VALUE(0.5f));
		// Display the buttons
		Button_Display(_engine, &carousel->buttons);
	}
}

char *Carousel_GetCurrentName(Carousel *_carousel)
{
	return _carousel->data.names[_carousel->data.current];
}

void Carousel_SetCurrentName(Carousel *_carousel, const char *_string)
{
	if (_string)
	{
		for (int i = 0; i < _carousel->data.number; i++)
		{
			if (_stricmp(_carousel->data.names[i], _string) == 0)
			{
				_carousel->data.current = i;
			}
		}
	}
}


// -- LOCAL FUNCTIONS ------------------------------------------------------ //

void Carousel_PressingLeftButton(Carousel *_carousel)
{
	_carousel->data.current--;
	if (_carousel->data.current < 0)
	{
		_carousel->data.current = _carousel->data.number - 1;
	}
}

void Carousel_PressingRightButton(Carousel *_carousel)
{
	_carousel->data.current++;
	if (_carousel->data.current >= _carousel->data.number)
	{
		_carousel->data.current = 0;
	}
}