#ifndef ENTITY_H
#define ENTITY_H

// -- INCLUDES --------------------------------------------------------------- //

#include "_Includes.h"

// -- SHARED PROTOTYPES ------------------------------------------------------ //

Entity Entity_Create(sfVector2f _position, sfVector2f _velocity);
void Entity_UpdatePosition(Entity *_entity, float _dt);
void Entity_CreateShaking(ShakingInfos* _infos, int _power, float _duration);
void Entity_DoShake(ShakingInfos* _infos, const float _dt);

#endif