#ifndef CREDIT__H
#define CREDIT__H

// -- INCLUDES ------------------------------------------------------------- //

#include "_MainData.h"
#include "Map.h"
#include "Fade.h"

// -- ENUMERATIONS --------------------------------------------------------- //

// -- DEFINES -------------------------------------------------------------- //

// -- STRUCTURES ----------------------------------------------------------- //

// Data used to store all informations about the state
struct CreditData
{
	int test;
	float creditsPos;
	char creditsContent[18][BUFFER_SMALL];
	Text creditsDisplay;
	sfSprite* logoPixelMatter;
	sfSprite* logoCreajeux;
};

// -- TYPES DEFINITIONS ---------------------------------------------------- //

typedef struct CreditData CreditData;

// -- MAIN FUNCTION DEFINITIONS -------------------------------------------- //

void Credit_Initialize(MainData *_data);
void Credit_HandleEvent(MainData *_data, sfEvent _event);
void Credit_Update(MainData *_data, float _dt);
void Credit_Display(MainData *_data);
void Credit_Exit(MainData *_data);

#endif // !CREDIT__H