#include "Shadow.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

// -- MAIN FUNCTION -------------------------------------------------------- //

void Shadow_Display(Engine *_engine, Entity *_entity, sfSprite *_sprite)
{
	sfRenderStates *shadowState = Shader_ShadowEffect();
	Entity shadowEntity = *_entity;
	shadowEntity.position.x += SHADOW_OFFSET_X * _engine->gameScale.x;
	shadowEntity.position.y += SHADOW_OFFSET_Y * _engine->gameScale.y;
	Sprite_Display(_engine->canvas, _sprite, shadowEntity.position, V2F_MUL_VALUE(_engine->gameScale, SHADOW_SCALE), shadowEntity.angle, shadowState);
}

void Shadow_DisplayPlayersShadow(Engine *_engine, Players *_players, sfBool _displayShadow)
{
	if (_displayShadow)
	{
		for (int i = 0; i < _players->number; i++)
		{
			Player *player = _players->array[i];
			Shadow_Display(_engine, &player->entity, player->currentAnim->sprites[player->entity.currentFrame]);
		}
	}
}

void Shadow_DisplayEnemiesShadow(Engine *_engine, Enemies *_enemies, sfBool _displayShadow)
{
	if (_displayShadow)
	{
		for (int i = 0; i < _enemies->number; i++)
		{
			Enemy *enemy = _enemies->array[i];
			Entity shadowEntity = enemy->entity;
			shadowEntity.angle = RAD_TO_DEG(enemy->entity.angle) - 90.f;
			if (shadowEntity.shake.isShaking)
			{
				shadowEntity.position.x += (float)shadowEntity.shake.shakingOffset.x;
				shadowEntity.position.y += (float)shadowEntity.shake.shakingOffset.y;
			}
			Shadow_Display(_engine, &shadowEntity, enemy->sprite);
		}
	}
}

void Shadow_DisplayBossShadow(Engine *_engine, Boss *_boss, sfBool _displayShadow)
{
	if (_displayShadow && _boss && _strcmpi("NULL", _boss->name) != 0 && _boss->currentSprite)
	{
		Entity shadowEntity = _boss->entity;
		shadowEntity.angle = 0.0f;
		if (shadowEntity.shake.isShaking)
		{
			shadowEntity.position.x += (float)shadowEntity.shake.shakingOffset.x;
			shadowEntity.position.y += (float)shadowEntity.shake.shakingOffset.y;
		}
		Shadow_Display(_engine, &shadowEntity, _boss->currentSprite);
	}
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //