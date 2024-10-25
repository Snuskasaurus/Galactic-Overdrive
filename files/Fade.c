#include "Fade.h"


// -- LOCAL FUNCTIONS ------------------------------ //

// -- FUNCTIONS ------------------------------------ //

void Fade_Init(MainData* _data)
{
	sfImage* blackSprite = sfImage_createFromColor(_data->engine.configuration->resolution.x, _data->engine.configuration->resolution.y, sfBlack);
	_data->transitionFade.spriteFade = Sprite_CreateFromImage(blackSprite, 0, 0);
	_data->transitionFade.color = sfTransparent;
	_data->transitionFade.ptrChangeState = NULL;
	_data->transitionFade.state = NO_FADE;
	_data->transitionFade.timer = 0;
}

// -- Notes
// This function will create a transition with a fade o/i effect 
// Once called it will start a fade out
// When the fade out end, your function "_ptrFunctionChangeState" will be called
// And then a fade in start, to slowly show the new state
void Fade_CreateTransition(TransitionFade* _fade, void(*_ptrFunctionChangeState)(MainData*))
{
	if (_fade->state == NO_FADE)
	{
		_fade->color = sfTransparent;
		_fade->state = FADE_OUT;
		_fade->timer = FADE_TIME_TO_IN;
		_fade->ptrChangeState = _ptrFunctionChangeState;
	}
}

void Fade_Update(MainData* _data)
{
	//printf("Fade state = %d	Timer = %f	Alpha = %d\n", _data->transitionFade.state, _data->transitionFade.timer, _data->transitionFade.color.a);

	// -- Fade out ------------------------------ //
	if (_data->transitionFade.state == FADE_OUT)
	{
		TransitionFade* fade = &_data->transitionFade;

		// Update timer
		fade->timer -= _data->engine.dt;
		// Set alpha
		if (fade->timer > 0)
		{
			fade->color.a = (char)Easing_QuarticOut(FADE_TIME_TO_IN - fade->timer, 0, 255, FADE_TIME_TO_IN);
		}
		// Change state and launch fade out
		else
		{
			fade->color.a = 255;
			fade->timer = FADE_TIME_TO_OUT;
			fade->state = FADE_IN;
			fade->ptrChangeState(_data);
		}
	}
	// -- Fade in ------------------------------ //
	else if (_data->transitionFade.state == FADE_IN)
	{
		TransitionFade* fade = &_data->transitionFade;

		// Update timer
		fade->timer -= _data->engine.dt;
		// Set alpha
		if (fade->timer > 0)
		{
			fade->color.a = 255 - (char)Easing_SinusOut(FADE_TIME_TO_IN - fade->timer, 0, 255, FADE_TIME_TO_IN);
		}
		// Change state to NO_FADE
		else
		{
			fade->color.a = 0;
			fade->timer = 0;
			fade->state = NO_FADE;
		}
	}
}

void Fade_Display(MainData* _data)
{
	if (_data->transitionFade.state > NO_FADE)
	{
		sfSprite_setColor(_data->transitionFade.spriteFade, _data->transitionFade.color);
		Sprite_Display(_data->engine.canvas, _data->transitionFade.spriteFade, V2F(0, 0), V2F(1, 1), 0, NULL);
	}
}