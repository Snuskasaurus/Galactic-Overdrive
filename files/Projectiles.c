#include "Projectiles.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

void L_ProjectileCreateCurvedProjectile(ProjectileData *_projectile);
void L_Projectile_UpdateCurvedProjectile(ProjectileData *_projectile, float _dt);
void Projectile_UpdateStraightedProjectileGuided(ProjectileData* _Projectile, GameData *_state, GameState _gameState, float _dt);

// -- SHARED FUNCTIONS ----------------------------------------------------- //

void Projectile_Create(Engine *_engine, List *_projectiles, Weapon *_weapon)
{
	sfVector2f velocity;
	static sfSprite *sprite = NULL; // PLEASE DESTROY AFTER WE USE FILES
	float tempEntityAngle = _weapon->angle - _weapon->dispersionShotAngle / 2.f;
	float tempShotAngle = _weapon->dispersionShotAngle / _weapon->nbShot;
	sfVector2f tempOffset = { _weapon->shotOffset * cosf(_weapon->angle), _weapon->shotOffset * -sinf(_weapon->angle) };
	for (int i = 0; i < _weapon->nbShot; i++)
	{
		ProjectileData *projectile = calloc(1, sizeof(ProjectileData));
		if (projectile)
		{
			float randomValue = RAND((int)_weapon->precision, 100) / 100.0f;
			float randomSign = (float)RAND_SIGN();
			float precisionAngle = randomSign * _weapon->precisionAngle * randomValue;

			if (_weapon->dispersionShotAngle != 0)
			{
				// Calcualte and set positions and velocities
				projectile->entity.angle = tempEntityAngle + tempShotAngle * (i + 0.5f) + precisionAngle / 2.0f;
				velocity.x = cosf(projectile->entity.angle) * _weapon->speed;
				velocity.y = sinf(projectile->entity.angle) * _weapon->speed;
				projectile->entity.position = _weapon->position;
				projectile->entity.velocity = velocity;
			}
			else
			{
				sfVector2f pos = { _weapon->position.x - (tempOffset.y * (_weapon->nbShot / 2.f)) + (tempOffset.y * (i + 0.5f)), _weapon->position.y - (tempOffset.x * (_weapon->nbShot / 2.f)) + (tempOffset.x * (i + 0.5f)) };
				velocity.x = cosf(_weapon->angle + precisionAngle) * _weapon->speed;
				velocity.y = sinf(_weapon->angle + precisionAngle) * _weapon->speed;
				projectile->entity = Entity_Create(pos, velocity);
				projectile->entity.angle = _weapon->angle + precisionAngle;
			}
			// Set the weapon originated from
			projectile->weapon = *_weapon;
			projectile->type = _weapon->type;
			// Set specifi behaviours
			switch (projectile->weapon.behaviour)
			{
			case WEAPON_CURVED_PROJECTILE_ORIENTED:
				L_ProjectileCreateCurvedProjectile(projectile);
				break;
			case WEAPON_STRAIGHTED_PROJECTILE_GUIDED:
				projectile->fuel = projectile->weapon.fuel;
				break;
			default:
				break;
			}
			// Setup Animation
			if (_weapon->animProj)
			{
				InitEntityAnim(&projectile->entity, _weapon->animProj->maxTime);
			}
			// Create the collider
			projectile->collider = Collider_CreateCircleCollider(projectile->entity.position, 2 * SCALE_SPRITE);
			// Add a new projectile
			List_InsertElement(_projectiles, 0, projectile);
		}
	}
}

void Projectile_Update(Engine *_engine, float _dt, List *_projectiles, GameData *_state, GameState _gameState)
{
	ListElement *current = _projectiles->first;
	while (current)
	{
		if (!current->mustBeCleared)
		{
			ProjectileData *projectile = (ProjectileData*)current->data;
			UpdateAnimation(_dt, projectile->weapon.animProj, &projectile->entity);
			switch (projectile->weapon.behaviour)
			{
			default:
			{
				Entity_UpdatePosition(&projectile->entity, _dt);
				projectile->entity.angleDisplayed = projectile->entity.angle;
			}
			break;
			case WEAPON_STRAIGHTED_LASER_ORIENTED:
			{
				Entity_UpdatePosition(&projectile->entity, _dt);
				projectile->entity.position.x = projectile->weapon.position.x;
			}
			break;
			case WEAPON_CURVED_PROJECTILE_ORIENTED:
			{
				L_Projectile_UpdateCurvedProjectile(projectile, _dt);
				projectile->entity.angleDisplayed = projectile->entity.angle;
			}
			break;
			case WEAPON_STRAIGHTED_PROJECTILE_GUIDED:
			{
				Projectile_UpdateStraightedProjectileGuided(projectile, _state, _gameState, _dt);
			}
			break;
			}
			Collider_UpdatePosition(&projectile->collider, projectile->entity.position); // Update collider position
		}
		// Asign the next element
		current = current->next;
	}
}

void Projectile_Display(Engine *_engine, List *_projectiles)
{
	ListElement *current = _projectiles->first;

	while (current)
	{
		if (!current->mustBeCleared)
		{
			ProjectileData *projectile = current->data;
			float angle = RAD_TO_DEG(projectile->entity.angleDisplayed) + 90.f;
			if (projectile->weapon.spriteProjectile)
			{
				Sprite_Display(_engine->canvas, projectile->weapon.spriteProjectile, projectile->entity.position, _engine->gameScale, angle, NULL);
			}
			else
			{
				DisplayAnimation(_engine->canvas, projectile->weapon.animProj, &projectile->entity, _engine->gameScale, NULL);
			}
		}
		// Asign the next element
		current = current->next;
	}
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //

void L_ProjectileCreateCurvedProjectile(ProjectileData *_projectile)
{
	Entity *entity = &_projectile->entity;
	Weapon *weapon = &_projectile->weapon;
	entity->goal.x = entity->position.x + weapon->range * cosf(entity->angle);
	entity->goal.y = entity->position.y + weapon->range * sinf(entity->angle);
	entity->origin = entity->position;
	entity->axisPosition = entity->position;
	_projectile->curveDirection = 1;
}

void L_Projectile_UpdateCurvedProjectile(ProjectileData *_projectile, float _dt)
{
	Entity *entity = &_projectile->entity;
	Weapon *weapon = &_projectile->weapon;
	sfVector2f traveled;
	sfVector2f trajectory;
	sfVector2f trajectory_Normalized = V2F_FROM_VALUE(0.0f);
	float trajectory_Norme = 0;
	float traveled_Norme = 0;
	float speed = 0;

	entity->position.x = entity->goal.x - entity->origin.x;
	trajectory.x = entity->goal.x - _projectile->entity.origin.x;
	trajectory.y = entity->goal.y - _projectile->entity.origin.y;
	traveled.x = entity->axisPosition.x - entity->origin.x;
	traveled.y = entity->axisPosition.y - entity->origin.y;
	trajectory_Norme = sqrtf(trajectory.x * trajectory.x + trajectory.y * trajectory.y);
	if (trajectory_Norme != 0)
	{
		traveled_Norme = sqrtf(traveled.x * traveled.x + traveled.y * traveled.y);
		speed = 1 - traveled_Norme / trajectory_Norme;
		trajectory_Normalized.x = trajectory.x / trajectory_Norme;
		trajectory_Normalized.y = trajectory.y / trajectory_Norme;
		entity->axisPosition.x += trajectory_Normalized.x * speed * weapon->speed * _dt;
		entity->axisPosition.y += trajectory_Normalized.y * speed * weapon->speed * _dt;
	}
	entity->position.x = entity->axisPosition.x - weapon->amplitude * -trajectory_Normalized.y * sinf(2 * PI * speed * weapon->period) * _projectile->curveDirection;
	entity->position.y = entity->axisPosition.y - weapon->amplitude * trajectory_Normalized.x * sinf(2 * PI * speed * weapon->period) * _projectile->curveDirection;
}

void Projectile_UpdateStraightedProjectileGuided(ProjectileData* _Projectile, GameData *_state, GameState _gameState, float _dt)
{
	if (_gameState != EDITOR)
	{
		Entity* entity = &_Projectile->entity;
		sfVector2f posProjectile = entity->position;
		sfVector2f posTarget = { 0, 0 };
		sfVector2f u = { cosf((entity->angle - 90.f) * PI / 180.f), sinf((entity->angle - 90.f) * PI / 180.f) };
		sfVector2f v = { 0, 0 };
		Enemies* tempEnemies = _state->enemies;
		Players* tempPlayers = &_state->players;
		float distance = 5000.f;
		int index = 0;
		float crossProduct = 0;
		float speed = _Projectile->weapon.speed;
		float rotationSpeed = _Projectile->weapon.rotationSpeed;
		if (_gameState == GAME && _Projectile->fuel > 0.f)
		{
			_Projectile->fuel -= _dt;
			if (_Projectile->weapon.owner == WEAPON_OWNER_PLAYER)
			{
				if (tempEnemies->number)
				{
					for (int i = 0; i < tempEnemies->number; i++)
					{
						sfVector2f posEnemy = tempEnemies->array[i]->entity.position;
						float tempDist = (float)sqrt(pow(posEnemy.x - posProjectile.x, 2) + pow(posEnemy.y - posProjectile.y, 2));
						index = tempDist < distance ? i : index;
						distance = tempDist < distance ? tempDist : distance;
					}
					posTarget = tempEnemies->array[index]->entity.position;
					v.x = posTarget.x - posProjectile.x;
					v.y = posTarget.y - posProjectile.y;
					crossProduct = u.x * v.y - u.y * v.x;
					entity->angle += crossProduct >= 0 ? rotationSpeed * _dt : -rotationSpeed * _dt;
				}
				else if (_state->boss.isActive)
				{
					if (_state->boss.phases[_state->boss.currentPhase].nbTurrets)
					{
						for (int i = 0; i < _state->boss.phases[_state->boss.currentPhase].nbTurrets; i++)
						{
							sfVector2f posTurret = _state->boss.phases[_state->boss.currentPhase].turrets[i].entity.position;
							float tempDist = (float)sqrt(pow(posTurret.x - posProjectile.x, 2) + pow(posTurret.y - posProjectile.y, 2));
							index = tempDist < distance ? i : index;
							distance = tempDist < distance ? tempDist : distance;
						}
						posTarget = _state->boss.phases[_state->boss.currentPhase].turrets[index].entity.position;
					}
					else if (_state->boss.phases[_state->boss.currentPhase].nbWeakpoints)
					{
						CircleCollider* tempCircle = NULL;
						RectangleCollider* tempRect = NULL;
						for (int i = 0; i < _state->boss.phases[_state->boss.currentPhase].nbWeakpoints; i++)
						{
							sfVector2f posCollider;
							float tempDist = 0;
							switch (_state->boss.phases[_state->boss.currentPhase].weakpoints[i].collider.type)
							{
							case COLLIDER_CIRCLE:
								tempCircle = _state->boss.phases[_state->boss.currentPhase].weakpoints[i].collider.pointer;
								posCollider = tempCircle->position;
								break;
							case COLLIDER_RECTANGLE:
								tempRect = _state->boss.phases[_state->boss.currentPhase].weakpoints[i].collider.pointer;
								posCollider = tempRect->position;
								break;
							default:
								break;
							}
							tempDist = (float)sqrt(pow(posCollider.x - posProjectile.x, 2) + pow(posCollider.y - posProjectile.y, 2));
							index = tempDist < distance ? i : index;
							distance = tempDist < distance ? tempDist : distance;

						}
						switch (_state->boss.phases[_state->boss.currentPhase].weakpoints[index].collider.type)
						{
						case COLLIDER_CIRCLE:
							tempCircle = _state->boss.phases[_state->boss.currentPhase].weakpoints[index].collider.pointer;
							posTarget = tempCircle->position;
							break;
						case COLLIDER_RECTANGLE:
							tempRect = _state->boss.phases[_state->boss.currentPhase].weakpoints[index].collider.pointer;
							posTarget = tempRect->position;
							break;
						default:
							break;
						}
					}
					else
					{
						posTarget = _state->boss.entity.position;
					}
					v.x = posTarget.x - posProjectile.x;
					v.y = posTarget.y - posProjectile.y;
					crossProduct = u.x * v.y - u.y * v.x;
					entity->angle += crossProduct >= 0 ? rotationSpeed * _dt : -rotationSpeed * _dt;
				}
			}
			else
			{
				for (int i = 0; i < _state->players.number; i++)
				{
					if (tempPlayers->array[i]->isAlive)
					{
						sfVector2f posPlayer = tempPlayers->array[i]->entity.position;
						float tempDist = (float)sqrt(pow(posPlayer.x - posProjectile.x, 2) + pow(posPlayer.y - posProjectile.y, 2));
						index = tempDist < distance ? i : index;
						distance = tempDist < distance ? tempDist : distance;
					}
				}
				if (tempPlayers->array[index]->isAlive)
				{
					posTarget = tempEnemies->array[index]->entity.position;
					v.x = posTarget.x - posProjectile.x;
					v.y = posTarget.y - posProjectile.y;
					crossProduct = u.x * v.y - u.y * v.x;
					entity->angle += crossProduct >= 0 ? rotationSpeed * _dt : -rotationSpeed * _dt;
				}
			}
		}
		entity->position.x += speed * cosf((entity->angle - 90.f) * PI / 180.f) * _dt;
		entity->position.y += speed * sinf((entity->angle - 90.f) * PI / 180.f) * _dt;
	}
}