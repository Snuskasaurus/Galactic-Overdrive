#ifndef COLLIDER__H
#define COLLIDER__H

// -- HEADERS -------------------------------------------------------------- //

#include "_Engine.h"

// -- FUNCTIONS ------------------------------------------------------------ //

void Collider_CreateFromFile(Collider *_collider, FILE *_file);

Collider Collider_CreateCircleCollider(sfVector2f _position, float _radius);

Collider Collider_CreateRectangleCollider(sfVector2f _position, sfVector2f _size, int centered);

Collider Collider_Copy(Collider *_toCopy);

void Collider_UpdatePosition(Collider *_collider, sfVector2f _position);

int Collider_CheckCollision(Collider *_collider1, Collider *_collider2);

int Collider_CheckCollisionWithMouse(Collider *_collider, sfVector2f _mousePosition);

void Collider_Destroy(Collider *_collider);

void Collider_Display(Engine *_engine, Collider *_collider);

#endif // !COLLIDER__H