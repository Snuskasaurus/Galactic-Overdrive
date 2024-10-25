#ifndef SHADERS__H
#define SHADERS__H

// -- INCLUDES ------------------------------------------------------------- //

#include "_Includes.h"
#include "Sprite.h"

// -- ENUMERATIONS --------------------------------------------------------- //

// -- TYPES DEFINITIONS ---------------------------------------------------- //

// -- DEFINES -------------------------------------------------------------- //

// -- STRUCTURES ----------------------------------------------------------- //

// -- FUNCTIONS ------------------------------------------------------------ //

sfRenderStates *Shader_ToWhite();

sfRenderStates *Shader_ShadowEffect();

sfRenderStates *Shader_SeeThrough();

sfRenderStates *Shader_TakingDamage(Hit_Type _type);

sfRenderStates *Shader_PlayerSpaceship(int _health, int _maxHealth, sfColor _textureColor, sfBool _seeThrough, float _time);

sfRenderStates *Shader_ShipColoration();

sfRenderStates *Shader_Contrast(int _intensity);

sfRenderStates *Shader_Blurr(sfVector2f size, float _intensity);

void Shader_ApplyBloomToCanvas(sfRenderTexture *_canvas);

sfRenderStates *Shader_Boss(sfColor *_colors, int _nbColors, sfTexture *_mask);

#endif // !SHADERS__H