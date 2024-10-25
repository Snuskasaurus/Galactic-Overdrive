#ifndef ANIMATED_STARS_BACKGROUND_H
#define ANIMATED_STARS_BACKGROUND_H 

// -- INCLUDES ------------------------------------------------------------- //

#include "_Includes.h"
#include "_Engine.h"

// -- FUNCTIONS ----------------------------------------------------------- //

void StarsBackground_Init(StarsBackground* _background, Engine* _engine);

void StarsBackground_Update(StarsBackground* _background, float _dt, Engine* _engine);

void StarsBackground_DisplayStars(StarsBackground* _background, sfRenderTexture* _canvas);
void StarsBackground_DisplayTrails(StarsBackground* _background, sfRenderTexture* _canvas);

#endif // !ANIMATED_STARS_BACKGROUND_H
