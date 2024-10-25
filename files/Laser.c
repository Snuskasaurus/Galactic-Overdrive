#include "Laser.h"

void Laser_Create(Engine *_engine, Lasers *_lasers, Weapon *_origin, void *_entity)
{
	Laser *newLaser = calloc(1, sizeof(Laser));

	if (newLaser != NULL)
	{
		sfVector2f pos;
		sfVector2f size;
		size = Sprite_GetSize(_origin->animProj->sprites[0]);
		size.x *= _engine->gameScale.x;
		size.y *= _engine->gameScale.y;
		InitEntityAnim(&newLaser->laserEntityHead, _origin->animProj->maxTime);
		InitEntityAnim(&newLaser->laserEntityBody, _origin->animBody->maxTime);
		InitEntityAnim(&newLaser->laserEntityBase, _origin->animBase->maxTime);

		switch (_origin->owner)
		{
		case WEAPON_OWNER_PLAYER:
		{
			pos = V2F(_origin->position.x, _origin->position.y - size.y);
			newLaser->dir = LASER_UP;
			newLaser->laserEntityHead.angle = 0.f;
			newLaser->laserEntityBody.angle = 0.f;
			newLaser->laserEntityBase.angle = 0.f;
			break;
		}
		default: // for enemy and boss
		{
			pos = V2F(_origin->position.x, _origin->position.y + size.y);
			newLaser->laserEntityHead.angle = 180.f;
			newLaser->laserEntityBody.angle = 180.f;
			newLaser->laserEntityBase.angle = 180.f;
			newLaser->dir = LASER_DOWN;
			break;
		}
		}

		newLaser->entityPtr = _entity;
		newLaser->collider = Collider_CreateRectangleCollider(V2F(pos.x - (size.x / 2.f), pos.y), size, 0);
		newLaser->laserEntityHead.position = pos;
		newLaser->origin = _origin;

		//Add newLaser to linked list ...
		newLaser->nextPtr = _lasers->firstElement[_origin->owner];
		_lasers->firstElement[_origin->owner] = newLaser;
		(_lasers->nbElements[_origin->owner])++;

		// Sound
		_origin->sound = Sound_PlayFromName(_engine->sound, _origin->soundsShooting[LASER_SOUND_START]);
		_origin->firstSoundPlaying = sfTrue;
	}
	else
	{
		printf("Failed to create a newLaser...\n");
	}
}

Lasers *Laser_InitLinkedList()
{
	Lasers *list = calloc(1, sizeof(Lasers));

	if (list != NULL)
	{
		for (WeaponOwner i = 0; i < WEAPON_OWNER_SIZE; i++)
		{
			list->firstElement[i] = NULL;
			list->lastElement[i] = NULL;
			list->nbElements[i] = 0;
		}
	}
	else
	{
		printf("Init : Out of memory...");
		exit(-1);
	}

	return list;
}

void Laser_DeleteFirst(Lasers *_list, WeaponOwner _owner)
{
	if (_list != NULL && _list->firstElement[_owner] != NULL)
	{
		Laser *toDelete = _list->firstElement[_owner];
		_list->firstElement[_owner] = _list->firstElement[_owner]->nextPtr;
		free(toDelete);
		toDelete = NULL;
		(_list->nbElements[_owner])--;
	}
}

Laser *Laser_Delete(Lasers *_list, Laser *_element, Laser *_elementToDelete, WeaponOwner _owner)
{
	if (_list != NULL && _list->nbElements != NULL && _element != NULL && _element->nextPtr != NULL)
	{
		_element->nextPtr = _elementToDelete->nextPtr;
		free(_elementToDelete);
		_elementToDelete = NULL;
		(_list->nbElements[_owner])--;
	}
	else
	{
		Laser_DeleteFirst(_list, _owner);
	}
	return _element;
}

void Laser_DestroyList(Lasers *_list)
{
	if (_list != NULL)
	{
		for (WeaponOwner i = (WeaponOwner)0; i < WEAPON_OWNER_SIZE; i++)
		{
			for (Laser *temp = _list->firstElement[i]; temp != NULL; temp = _list->firstElement[i])
			{
				Laser_DeleteFirst(_list, i);
			}
			_list->nbElements[i] = 0;
		}

		free(_list);
		_list = NULL;
	}
}

// Check if shooting with weapon
sfBool Laser_IsAlreadyShooting(Lasers *_list, void *_entity, Weapon *_weapon)
{
	if (_list != NULL)
	{
		for (Laser *tempLaser = _list->firstElement[_weapon->owner]; tempLaser != NULL; tempLaser = tempLaser->nextPtr)
		{
			if (tempLaser->entityPtr == _entity)
			{
				if (_weapon == tempLaser->origin)
				{
					// Is already shooting...
					return sfTrue;
				}
			}
		}
	}
	// Is not already shooting...
	return sfFalse;
}

void Laser_Update(Lasers *_list, Engine* _engine)
{
	if (_list != NULL)
	{
		for (WeaponOwner iOwner = (WeaponOwner)0; iOwner < WEAPON_OWNER_SIZE; iOwner++)
		{
			for (Laser *tempLaser = _list->firstElement[iOwner]; tempLaser != NULL; tempLaser = tempLaser->nextPtr)
			{
				RectangleCollider *collider = (RectangleCollider*)tempLaser->collider.pointer;
				sfVector2f pos = V2F(tempLaser->origin->position.x, tempLaser->laserEntityHead.position.y);
				sfVector2f size = Sprite_GetSize(tempLaser->origin->animProj->sprites[0]);
				size.y *= SCALE_SPRITE;

				switch (tempLaser->origin->owner)
				{
				case WEAPON_OWNER_PLAYER:
				{
					Collider_UpdatePosition(&tempLaser->collider, V2F(pos.x - (collider->size.x / 2.f), pos.y - (size.y / 2.f)));
					break;
				}
				default: // Triggers for WEAPON_OWNER_ENEMY & WEAPON_OWNER_BOSS
				{
					Collider_UpdatePosition(&tempLaser->collider, V2F(pos.x - (collider->size.x / 2.f), (pos.y + (size.y / 2.f)) - collider->size.y));
					break;
				}
				}

				if (tempLaser->origin->firstSoundPlaying == sfTrue && sfSound_getStatus(tempLaser->origin->sound) == sfStopped)
				{
					tempLaser->origin->firstSoundPlaying = sfFalse;
					tempLaser->origin->sound = Sound_PlayFromName(_engine->sound, tempLaser->origin->soundsShooting[LASER_SOUND_LOOP]);
					sfSound_setLoop(tempLaser->origin->sound, sfTrue);
				}

				tempLaser->laserEntityHead.position = pos;
				tempLaser->laserEntityBase.position = tempLaser->origin->position;
				UpdateAnimation(_engine->dt, tempLaser->origin->animProj, &tempLaser->laserEntityHead);
				UpdateAnimation(_engine->dt, tempLaser->origin->animBody, &tempLaser->laserEntityBody);
				UpdateAnimation(_engine->dt, tempLaser->origin->animBase, &tempLaser->laserEntityBase);
			}
		}
	}
}

void Laser_StopShooting(Lasers *_lasers, void *_entity, Weapon *_weapon)
{
	if (Laser_IsAlreadyShooting(_lasers, _entity, _weapon) == sfTrue)
	{
		Laser *tempPreviousLaser = NULL;
		for (Laser *tempLaser = _lasers->firstElement[_weapon->owner]; tempLaser != NULL; tempLaser = tempLaser->nextPtr)
		{
			if (tempLaser->entityPtr == _entity)
			{
				if (_weapon == tempLaser->origin)
				{
					if (_weapon->sound)
					{
						_weapon->firstSoundPlaying = sfFalse;
						sfSound_stop(_weapon->sound);
					}
					tempLaser = Laser_Delete(_lasers, tempPreviousLaser, tempLaser, _weapon->owner);
					if (tempLaser == NULL)
					{
						return;
					}
				}
			}
			tempPreviousLaser = tempLaser;
		}
	}
}

void Laser_Display(Engine *_engine, Lasers *_list)
{
	if (_list != NULL)
	{
		for (WeaponOwner i = (WeaponOwner)0; i < WEAPON_OWNER_SIZE; i++)
		{
			for (Laser *tempLaser = _list->firstElement[i]; tempLaser != NULL; tempLaser = tempLaser->nextPtr)
			{
				if (tempLaser->origin->behaviour == WEAPON_STRAIGHTED_LASER_ORIENTED)
				{
					RectangleCollider *pointer = (RectangleCollider*)tempLaser->collider.pointer;
					int nbPixel = (int)(pointer->size.y / _engine->gameScale.y);
					for (int j = 0; j < nbPixel; j++)
					{
						sfVector2f pos = { tempLaser->laserEntityHead.position.x, tempLaser->laserEntityHead.position.y + (tempLaser->dir * (j * _engine->gameScale.y)) };
						tempLaser->laserEntityBody.position = pos;
						DisplayAnimation(_engine->canvas, tempLaser->origin->animBody, &tempLaser->laserEntityBody, _engine->gameScale, NULL);
					}
					DisplayAnimation(_engine->canvas, tempLaser->origin->animBase, &tempLaser->laserEntityBase, _engine->gameScale, NULL);
					DisplayAnimation(_engine->canvas, tempLaser->origin->animProj, &tempLaser->laserEntityHead, _engine->gameScale, NULL);

					// To see colliders...
					/*sfRectangleShape *rec = sfRectangleShape_create();
					sfRectangleShape_setPosition(rec, pointer->position);
					sfRectangleShape_setSize(rec, pointer->size);
					sfRenderWindow_drawRectangleShape(_engine->window, rec, NULL);
					sfRectangleShape_destroy(rec);*/
				}
			}
		}
	}
}