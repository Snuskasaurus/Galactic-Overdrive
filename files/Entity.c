#include "Entity.h"

// -- LOCAL PROTOTYPES ------------------------------------------------------- //

// -- SHARED FUNCTIONS ------------------------------------------------------- //

Entity Entity_Create(sfVector2f _position, sfVector2f _velocity)
{
	Entity entity;

	entity.angle = 0.0f;
	entity.position = _position;
	entity.velocity = _velocity;
	return entity;
}

void Entity_UpdatePosition(Entity *_entity, float _dt)
{
	_entity->position.x += _entity->velocity.x * _dt;
	_entity->position.y += _entity->velocity.y * _dt;
}

void Entity_CreateShaking(ShakingInfos* _infos, int _power, float _duration)
{
	_infos->isShaking = sfTrue;
	_infos->shakingPower = _power;
	_infos->timer = 0;
	_infos->timerMax = _duration;
}

void Entity_DoShake(ShakingInfos* _infos, const float _dt)
{
	_infos->timer += _dt;
	if (_infos->timer < _infos->timerMax)
	{
		_infos->shakingOffset.x = rand() % _infos->shakingPower;
		_infos->shakingOffset.y = rand() % _infos->shakingPower;
	}
	else
	{
		_infos->isShaking = sfFalse;
		_infos->shakingOffset.x = 0;
		_infos->shakingOffset.y = 0;
	}
}

// -- LOCAL FUNCTIONS -------------------------------------------------------- //
