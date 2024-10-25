#include "DirectionalShield.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

// -- SHARED FUNCTIONS ----------------------------------------------------- //

int DirectionalShield_isColliding(Player* _player, ProjectileData* _projectile)
{
	float angleProj = Angle_PrimaryMeasure(Angle_DegGetAngle(_player->entity.position, _projectile->entity.position), 1);
	float angleWeapon = Angle_PrimaryMeasure(Angle_RadToDeg(_player->weapons[_player->currentFireMode].weapon.entity.angle) + 90, 1);

	int angleMargin = _player->weapons[_player->currentFireMode].weapon.shieldAngleCovered / 2;
	float angleMin = angleWeapon - angleMargin;
	float angleMax = angleWeapon + angleMargin;

	// If the projectile is in the covered angle interval :
	if (angleProj > angleMin && angleProj < angleMax)
	{
		float distance = Angle_CalculateDistance(_player->entity.position, _projectile->entity.position);
		// If the projectile is close enough to cause a collision :
		if (distance < SHIELD_DISTANCE_UNSCALED*SCALE_SPRITE &&
			distance >(SHIELD_DISTANCE_UNSCALED*SCALE_SPRITE) / 2)
		{
			return TRUE;
		}
	}
	/// Fix a problem : When angleMax exceeded 360 we couldn't collide with projectile at the 360°+ angle
	/// To fix that we simply look for a collision into the interval [0 ; angleMax-360]
	else if (angleMax > 360)
	{
		if (angleProj < angleMax - 360 || angleProj > angleMin)
		{
			float distance = Angle_CalculateDistance(_player->entity.position, _projectile->entity.position);
			if (distance < SHIELD_DISTANCE_UNSCALED*SCALE_SPRITE &&
				distance >(SHIELD_DISTANCE_UNSCALED*SCALE_SPRITE) / 2)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}


// Called when we activate the shield
void DirectionalShield_ActivationOperation(Weapon *_weapon, joystickId _joystick, Engine* _engine)
{
	if (_weapon->isActive == FALSE)
	{
		Sound_PlayFromName(_engine->sound, "DirectionalShield_ShieldActivate.wav");
	}
	// Specify that we have activated the weapon this frame
	_weapon->isActive = TRUE;
	// Set Default rotation Direction of attacks
	_weapon->rotationDirection = RIGHT;
	// Set angle of attacks
	_weapon->angle = _weapon->defaultAngle;
	// Orientate the shield with the second joystick
	Weapon_UpdateOrientation(_weapon, _joystick);
}

void DirectionalShield_AbsorbBullet(ProjectileData* _projectile, Weapon* _directionalShield, Engine* _engine)
{
	// Lose life
	_directionalShield->life -= _projectile->weapon.damage;
	// Unable to regen life during "TIMER_MAX_TO_REGEN" second
	_directionalShield->timerToRegen = SHIELD_TIMER_MAX_TO_REGEN;
	// Absorb attack
	_directionalShield->attackCharges += (int)_projectile->weapon.damage;
	// Set pitch and play sound
	sfSound_setPitch(Sound_PlayFromName(_engine->sound, "DirectionalShield_DeflectProjectile.wav"), 0.5f + _directionalShield->attackCharges / (_directionalShield->attackChargesMax / 1.5f));
}

void DirectionalShield_Regenlife(Weapon* _directionalShield, float _dt, Engine* _engine)
{
	_directionalShield->timerToRegen -= _dt;
	if (_directionalShield->timerToRegen <= 0)
	{
		_directionalShield->life += _directionalShield->lifeRegenPerSecond * _dt;
		if (_directionalShield->life > _directionalShield->lifeMax)
		{
			_directionalShield->life = _directionalShield->lifeMax;
			if (_directionalShield->canBeActivated == FALSE)
			{
				_directionalShield->canBeActivated = TRUE;
				// Sound "ShieldUp"
				///printf("Shield Up !\n");
				Sound_PlayFromName(_engine->sound, "DirectionalShield_MagneticShieldUp.wav");
			}
		}
	}
	if (_directionalShield->life < 0 && _directionalShield->canBeActivated == TRUE)
	{
		_directionalShield->canBeActivated = FALSE;
		// Shield broke !
		///printf("Shield broke !\n");
		Sound_PlayFromName(_engine->sound, "DirectionalShield_ShieldDisactivate.wav");
		_directionalShield->isActive = FALSE;
	}
}

void DirectionalShield_Attack(Engine *_engine, List *_projectiles, Weapon *_directionalShield)
{
	// If we have some attack charges
	if (_directionalShield->attackCharges > SHIELD_ATTACK_CHARGE_PRICE)
	{
		int marginRadius = (int)(_directionalShield->attackRadius / 2.f);
		float defaultAngle = Angle_RadToDeg(_directionalShield->defaultAngle);
		// -- Update Angle ---- //
		// Change the unit from Radiant to degree for coding comfort
		_directionalShield->angle = Angle_RadToDeg(_directionalShield->angle);

		if (_directionalShield->rotationDirection == RIGHT)
		{
			_directionalShield->angle += SHIELD_ANGLE_ROTATION_SPEED * _engine->dt;

			if (_directionalShield->angle > defaultAngle + marginRadius)
			{
				_directionalShield->rotationDirection = LEFT;
				_directionalShield->angle = defaultAngle + marginRadius;
			}
		}
		else
		{
			_directionalShield->angle -= SHIELD_ANGLE_ROTATION_SPEED * _engine->dt;
			if (_directionalShield->angle < defaultAngle - marginRadius)
			{
				_directionalShield->rotationDirection = RIGHT;
				_directionalShield->angle = defaultAngle - marginRadius;
			}
		}
		// Reverse the unit change back to Radiant
		_directionalShield->angle = Angle_DegToRad(_directionalShield->angle);

		// -- Fire if timer is ready
		if (Timer_Check(&_directionalShield->rate))
		{
			Projectile_Create(_engine, _projectiles, _directionalShield);
			_directionalShield->attackCharges -= SHIELD_ATTACK_CHARGE_PRICE;
			Timer_Reset(&_directionalShield->rate);
			Sound_PlayFromName(_engine->sound, "Shoot_energetic_3.wav");
		}
	}
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //