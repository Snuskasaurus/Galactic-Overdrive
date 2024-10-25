#include "Credit.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

// -- MAIN FUNCTION -------------------------------------------------------- //

void CreditToMenu(MainData* _data)
{
	_data->currentState = MENU;
}

void Credit_Update(MainData *_data, float _dt)
{

	// Create a shortcut to the state data
	CreditData *state = _data->stateData[CREDIT];
	sfVector2i tempResolution = _data->engine.configuration->resolution;

	state->creditsPos -= MAP_SCROLLING_SPEED * 0.5f * _dt;

	if (state->creditsPos < -tempResolution.y)
	{
		Fade_CreateTransition(&_data->transitionFade, CreditToMenu);
	}
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //