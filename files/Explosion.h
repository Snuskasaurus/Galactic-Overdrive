#ifndef EXPLOSION__H
#define EXPLOSION__H

// -- INCLUDES ------------------------------------------------------------- //
#include "_Engine.h"

// Components
#include "Entity.h"
#include "Collider.h"
#include "Timer.h"
#include "Animation.h"

// -- FUNCTIONS ------------------------------------------------------------ //

BP_Explosions Explosion_InitializeBlueprints(Engine *_engine);

void Explosion_Create(Engine *_engine, List *_explosions, sfVector2f _position, ExplosionSize _size, ExplosionType _type);

void Explosion_Update(Engine *_engine, List *_explosions, float _dt);

void Explosion_Display(Engine *_engine, List *_explosions);

#endif // !EXPLOSION__H