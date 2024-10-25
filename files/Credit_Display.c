#include "Credit.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

// -- MAIN FUNCTION -------------------------------------------------------- //

void Credit_Display(MainData *_data)
{
	// Create a shortcut to the state data
	CreditData *state = _data->stateData[CREDIT];
	sfVector2i tempResolution = _data->engine.configuration->resolution;

	for (int i = 0; i < 18; i++)
	{
		sfVector2f tempPos = { (float)(tempResolution.x / 2), (float)(15.f + i * 30.f + state->creditsPos) };
		Text_Display(_data->engine.canvas, &state->creditsDisplay, state->creditsContent[i], tempPos, (sfVector2f) { 0.5f, 0.5f });
	}
	Sprite_Display(_data->engine.canvas, state->logoPixelMatter, (sfVector2f) { (float)(tempResolution.x / 2), (float)(21 * 30.f + 51.f + state->creditsPos) }, (sfVector2f) { 0.25f, 0.25f }, 0, NULL);
	Sprite_Display(_data->engine.canvas, state->logoCreajeux, (sfVector2f) { (float)(tempResolution.x / 2), (float)(25 * 30.f + 102.f + 64.f + state->creditsPos) }, (sfVector2f) { 0.35f, 0.35f }, 0, NULL);//257
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //