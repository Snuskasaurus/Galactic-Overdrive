#ifndef CAROUSEL__H
#define CAROUSEL__H

// -- INCLUDES ------------------------------------------------------------- //

#include "_Engine.h"

#include "Button.h"
#include "Collider.h"

// -- TYPES DEFINITIONS ---------------------------------------------------- //

// -- ENUMERATIONS --------------------------------------------------------- //

// -- DEFINES -------------------------------------------------------------- //

typedef enum Carousel_Button Carousel_Button;
enum Carousel_Button
{
	CAROUSEL_BUTTON_LEFT,
	CAROUSEL_BUTTON_RIGHT,
	CAROUSEL_NUMBER_BUTTONS
};

// -- STRUCTURES ----------------------------------------------------------- //


typedef struct CarouselData CarouselData;
struct CarouselData
{
	int number;
	int current;
	void **elements;
	char **names;
};

typedef struct Carousel Carousel;
struct Carousel
{
	sfVector2f position;
	sfVector2f scale;
	Button buttonBlueprints[CAROUSEL_NUMBER_BUTTONS];
	Buttons buttons;
	CarouselData data;
	char *stringTitle;
	sfVector2f positionTitle;
	Text title;
	Text content;
	sfSprite *background;
};

typedef struct Carousels Carousels;
struct Carousels
{
	int number;
	Carousel *array[BUTTON_MAXIMUM_NUMBER];
};

// -- SHARED FUNCTIONS DEFINITIONS ----------------------------------------- //

Carousel Carousel_InitializeBlueprint(Engine *_engine, const char *_filename);

void Carousel_Create(Carousels *_carousels, Carousel *_blueprint, const char *_title, sfVector2f _position, sfVector2f _scale, CarouselData _carouselData);

void Carousel_Update(Engine *_engine, Carousels *_carousels, sfVector2f _mousePosition);

void Carousel_Pressing(Engine *_engine, Carousels *_carousels, sfVector2f _mousePosition);

void Carousel_Releasing(Engine *_engine, Carousels *_carousels, sfVector2f _mousePosition);

void Carousel_Display(Engine *_engine, Carousels *_carousels);

char *Carousel_GetCurrentName(Carousel *_carousel);

void Carousel_SetCurrentName(Carousel *_carousel, const char *_name);

#endif // !CAROUSEL__H