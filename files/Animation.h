#ifndef ANIMATION_H
#define ANIMATION_H

// -- COMMON HEADER ---------------------------------------------------------- //

#include "_Includes.h"
#include "Entity.h"
#include "Sprite.h"
#include "DirectoryManipulation.h"

// -- FUNCTIONS -------------------------------------------------------------- //

Anim CreateAnimation(const char *_directory, float _maxTime);

Animations *LoadAnimations(const char *_directory);

Anim *GetAnim(Animations *_anims, char *_name, char *_state);

void InitEntityAnim(Entity *_entity, float _maxTime);

void ResetAnim(Entity *_entity);

void UpdateAnimation(float _dt, Anim *_anim, Entity *_entity);

void DisplayAnimation(sfRenderTexture *_canvas, Anim *_currentAnim, Entity *_entity, sfVector2f _scale, sfRenderStates *_renderState);

void DestroyAnim(Anim *_animToDestroy);

#endif