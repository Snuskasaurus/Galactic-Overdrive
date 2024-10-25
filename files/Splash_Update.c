#include "Splash.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

// -- MAIN FUNCTION -------------------------------------------------------- //

void Splash_Update(MainData *_data, float _dt)
{
	// Create a shortcut to the state data
	SplashData *state = _data->stateData[SPLASH];

	state->currentTime += _dt;
	if (state->state == APPEAR_LOGO3 || state->state == FADE_LOGO3)
	{
		state->state += state->currentTime >= (SPLASH_DELAY_LONGER) ? 1 : 0;
		state->currentTime = state->currentTime >= (SPLASH_DELAY_LONGER) ? 0 : state->currentTime;
	}
	else
	{
		state->state += state->currentTime >= SPLASH_DELAY ? 1 : 0;
		state->currentTime = state->currentTime >= SPLASH_DELAY ? 0 : state->currentTime;
	}
	switch (state->state)
	{
	case APPEAR_LOGO1:
		state->alpha = (char)(255 * (state->currentTime / SPLASH_DELAY));
		break;
	case FADE_LOGO1:
		state->alpha = (char)(255 * ((SPLASH_DELAY - state->currentTime) / SPLASH_DELAY));
		break;
	case APPEAR_LOGO2:
		state->alpha = (char)(255 * (state->currentTime / SPLASH_DELAY));
		break;
	case FADE_LOGO2:
		state->alpha = (char)(255 * ((SPLASH_DELAY - state->currentTime) / SPLASH_DELAY));
		break;
	case APPEAR_LOGO3:
		state->alpha = (char)(255 * (state->currentTime / SPLASH_DELAY_LONGER));
		break;
	case FADE_LOGO3:
		state->alpha = (char)(255 * ((SPLASH_DELAY_LONGER - state->currentTime) / SPLASH_DELAY_LONGER));
		break;
	default:
		break;
	}
	_data->currentState = state->state == END_SPLASH ? MENU : _data->currentState;
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //