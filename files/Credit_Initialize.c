#include "Credit.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

// -- MAIN FUNCTION -------------------------------------------------------- //

void Credit_Initialize(MainData *_data)
{
	// Create a shortcut to the state data
	CreditData *state = NULL;
	sfVector2i tempResolution = _data->engine.configuration->resolution;
	sfVector2f tempScale = _data->engine.configuration->scale;
	char tempCredits[18][BUFFER_SMALL] = { "----- Program -----", " ", "Gael ETTENDORF", "Nicolas GRANIER", "Nathaniel LIMA", "Alexis LION", "Theo MAURIN", "Julien ROGEL", " ", " ", " ", "----- Graphisms -----", " ", "Theo FERRET", "Simon GUIVAUDON", "Fabien NIVAULT", "Romain PELOFFI", "Mandy RUIZ GARCIA" };

	// Allocate the state data to memory
	_data->stateData[CREDIT] = (CreditData*)calloc(1, sizeof(CreditData));
	state = _data->stateData[CREDIT];

	state->creditsPos = (float)(tempResolution.y + 15.f);
	for (int i = 0; i < 18; i++)
	{
		strcpy(state->creditsContent[i], tempCredits[i]);
	}

	state->creditsDisplay.size = (int)(30.f * tempScale.x);
	state->creditsDisplay.font = Text_GetFont(_data->engine.text, "Arial.ttf");
	state->creditsDisplay.color = sfWhite;

	state->logoPixelMatter = Sprite_CreateFromFile("Data/Sprites/Splash/logo_pixelmatter.png", 0.5f, 0.5f);
	state->logoCreajeux = Sprite_CreateFromFile("Data/Sprites/Splash/logo_creajeux.png", 0.5f, 0.5f);
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //