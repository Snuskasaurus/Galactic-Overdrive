#include "Splash.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

// -- MAIN FUNCTION -------------------------------------------------------- //

void Splash_Display(MainData *_data)
{
	// Create a shortcut to the state data
	SplashData *state = _data->stateData[SPLASH];
	sfVector2i tempResolution = _data->engine.configuration->resolution;
	sfVector2f tempScale = _data->engine.configuration->scale;
	sfVector2f splashPos = { (float)(tempResolution.x / 2), (float)(tempResolution.y / 2) };
	if (state->state == APPEAR_LOGO1 || state->state == FADE_LOGO1)
	{
		sfSprite_setColor(state->logoPixelMatter, (sfColor) { 255, 255, 255, state->alpha });
		Sprite_Display(_data->engine.canvas, state->logoPixelMatter, splashPos, tempScale, 0, NULL);
	}
	else if (state->state == APPEAR_LOGO2 || state->state == FADE_LOGO2)
	{
		sfSprite_setColor(state->logoCreajeux, (sfColor) { 255, 255, 255, state->alpha });
		Sprite_Display(_data->engine.canvas, state->logoCreajeux, splashPos, tempScale, 0, NULL);
	}
	else if (state->state == APPEAR_LOGO3 || state->state == FADE_LOGO3)
	{
		sfSprite_setColor(state->imgController, (sfColor) { 255, 255, 255, state->alpha });
		Sprite_Display(_data->engine.canvas, state->imgController, splashPos, V2F(tempScale.x * _data->engine.gameScale.x, tempScale.y * _data->engine.gameScale.y), 0, NULL);
	}
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //