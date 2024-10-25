#include "Collider.h"

// -- SHARED FUNCTIONS ------------------------------------------------------- //

void Collider_CreateFromFile(Collider *_collider, FILE *_file)
{
	switch (_collider->type)
	{
		default:
		break;
		case COLLIDER_CIRCLE:
		{
			CircleCollider *pointer = calloc(1, sizeof(CircleCollider));
			fscanf(_file, " POSITION %f %f RADIUS %f\n", &pointer->offset.x, &pointer->offset.y, &pointer->radius);
			pointer->offset.x *= SCALE_SPRITE;
			pointer->offset.y *= SCALE_SPRITE;
			pointer->radius *= SCALE_SPRITE;
			_collider->pointer = pointer;
		}
		break;
		case COLLIDER_RECTANGLE:
		{
			RectangleCollider *pointer = calloc(1, sizeof(RectangleCollider));
			fscanf(_file, " POSITION %f %f SIZE %f %f CENTERED %d\n", &pointer->offset.x, &pointer->offset.y, &pointer->size.x, &pointer->size.y, &pointer->centered);
			pointer->offset.x *= SCALE_SPRITE;
			pointer->offset.y *= SCALE_SPRITE;
			pointer->size.x *= SCALE_SPRITE;
			pointer->size.y *= SCALE_SPRITE;
			_collider->pointer = pointer;
		}
		break;
		case COLLIDER_SHAPE:
		{
			// TODO
		}
		break;
	}
}

Collider Collider_CreateRectangleCollider(sfVector2f _position, sfVector2f _size, int centered)
{
	Collider collider;
	RectangleCollider *pointer = calloc(1, sizeof(RectangleCollider));
	collider.type = COLLIDER_RECTANGLE;
	pointer->position = _position;
	pointer->size = _size;
	pointer->centered = centered;
	collider.pointer = pointer;
	return collider;
}

Collider Collider_CreateCircleCollider(sfVector2f _position, float _radius)
{
	Collider collider;
	CircleCollider *pointer = calloc(1, sizeof(CircleCollider));
	collider.type = COLLIDER_CIRCLE;
	pointer->position = _position;
	pointer->radius = _radius;
	collider.pointer = pointer;
	return collider;
}

Collider Collider_Copy(Collider *_toCopy)
{
	Collider collider;
	collider.type = _toCopy->type;
	switch (collider.type)
	{
		default:
		break;
		case COLLIDER_CIRCLE:
		{
			CircleCollider *pointer = calloc(1, sizeof(CircleCollider));
			CircleCollider *otherPointer = (CircleCollider*)_toCopy->pointer;
			pointer->offset = otherPointer->offset;
			pointer->radius = otherPointer->radius;
			collider.pointer = pointer;
			
		}
		break;
		case COLLIDER_RECTANGLE:
		{
			RectangleCollider *pointer = calloc(1, sizeof(RectangleCollider));
			RectangleCollider *otherPointer = (RectangleCollider*)_toCopy->pointer;
			pointer->offset = otherPointer->offset;
			pointer->size = otherPointer->size;
			pointer->centered = otherPointer->centered;
			collider.pointer = pointer;
			
		}
		break;
		case COLLIDER_SHAPE:
		{
			// TODO
		}
		break;
	}
	return collider;
}

void Collider_UpdatePosition(Collider *_collider, sfVector2f _position)
{
	switch (_collider->type)
	{
		default:
		break;
		case COLLIDER_CIRCLE:
		{
			CircleCollider *pointer = (CircleCollider*)_collider->pointer;
			pointer->position.x = _position.x + pointer->offset.x;
			pointer->position.y = _position.y + pointer->offset.y;
		}
		break;
		case COLLIDER_RECTANGLE:
		{
			RectangleCollider *pointer = (RectangleCollider*)_collider->pointer;
			pointer->position.x = _position.x + pointer->offset.x;
			pointer->position.y = _position.y + pointer->offset.y;
		}
		break;
		case COLLIDER_SHAPE:
		{
			// TODO
		}
		break;
	}
}

int Collider_CheckCollision(Collider *_collider1, Collider *_collider2)
{
	switch (_collider1->type)
	{
		case COLLIDER_CIRCLE:
		{
			CircleCollider *pointer1 = (CircleCollider*)_collider1->pointer;
			switch (_collider2->type)
			{
				case COLLIDER_CIRCLE:
				{
					CircleCollider *pointer2 = (CircleCollider*)_collider2->pointer;
					return IsColliding_Circle_Circle(*pointer1, *pointer2);
				}
				break;
				case COLLIDER_RECTANGLE:
				{
					RectangleCollider *pointer2 = (RectangleCollider*)_collider2->pointer;
					return IsColliding_Circle_Rectangle(*pointer1, *pointer2);
				}
			}
		}
		break;
		case COLLIDER_RECTANGLE:
		{
			RectangleCollider *pointer1 = (RectangleCollider*)_collider1->pointer;
			switch (_collider2->type)
			{
				case COLLIDER_CIRCLE:
				{
					CircleCollider *pointer2 = (CircleCollider*)_collider2->pointer;
					return IsColliding_Circle_Rectangle(*pointer2, *pointer1);
				}				
				break;
				case COLLIDER_RECTANGLE:
				{
					RectangleCollider *pointer2 = (RectangleCollider*)_collider2->pointer;
					return IsColliding_Rectangle_Rectangle(*pointer1, *pointer2);
				}	
				break;
			}
		}
		break;
	}
	return FALSE;
}

int Collider_CheckCollisionWithMouse(Collider *_collider, sfVector2f _mousePosition)
{
	switch (_collider->type)
	{
		case COLLIDER_CIRCLE:
		{
			CircleCollider *pointer = (CircleCollider*)_collider->pointer;
			return IsColliding_Circle_Point(*pointer, _mousePosition);
		}
		break;
		case COLLIDER_RECTANGLE:
		{
			RectangleCollider *pointer = (RectangleCollider*)_collider->pointer;
			return IsColliding_Point_Rectangle(_mousePosition, *pointer);
		}
		break;
	}
	return FALSE;
}

void Collider_Destroy(Collider *_collider)
{
	free(_collider->pointer);
}

void Collider_Display(Engine *_engine, Collider *_collider)
{
	static sfRectangleShape *rectangle = NULL;
	static sfCircleShape *circle = NULL;
	if (!rectangle || !circle)
	{
		rectangle = sfRectangleShape_create();
		circle = sfCircleShape_create();
		sfCircleShape_setFillColor(circle, sfColor_fromRGBA(255, 0, 0, 120));
		sfRectangleShape_setFillColor(rectangle, sfColor_fromRGBA(255, 0, 0, 120));
	}
	switch (_collider->type)
	{
		case COLLIDER_CIRCLE:
		{
			CircleCollider *pointer = (CircleCollider*)_collider->pointer;
			sfCircleShape_setPosition(circle, pointer->position);
			sfCircleShape_setRadius(circle, pointer->radius);
			sfCircleShape_setOrigin(circle, V2F(pointer->radius, pointer->radius));
			sfRenderTexture_drawCircleShape(_engine->canvas, circle, NULL);
		}
		break;
		case COLLIDER_RECTANGLE:
		{
			RectangleCollider *pointer = (RectangleCollider*)_collider->pointer;
			sfRectangleShape_setPosition(rectangle, pointer->position);
			sfRectangleShape_setSize(rectangle, pointer->size);
			if (pointer->centered)
			{
				sfRectangleShape_setOrigin(rectangle, V2F(pointer->size.x / 2.0f, pointer->size.y / 2.0f));
			}
			else
			{
				sfRectangleShape_setOrigin(rectangle, V2F(0.0f, 0.0f));
			}
			sfRenderTexture_drawRectangleShape(_engine->canvas, rectangle, NULL);
		}
		break;
	}
}
