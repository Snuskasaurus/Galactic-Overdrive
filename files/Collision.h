#ifndef COLLISION_H
#define COLLISION_H

// -- HEADERS -------------------------------------------------------------- //

#include <stdio.h>
#include <stdlib.h>
#include "Math.h"
#include <SFML/System.h>
#include <SFML/Window.h>
#include <SFML/Graphics.h>

// -- STRUCTURES ----------------------------------------------------------- //

typedef struct RectangleCollider RectangleCollider;
struct RectangleCollider
{
	int centered;
	sfVector2f offset;
	sfVector2f position;
	sfVector2f size;
};

typedef struct CircleCollider CircleCollider;
struct CircleCollider
{
	sfVector2f offset;
	sfVector2f position;
	float radius;
};

// -- FUNCTIONS ------------------------------------------------------------ //

int IsColliding_Circle_Point(CircleCollider _circle, sfVector2f _point);

int IsColliding_Point_Rectangle(sfVector2f _pointPosition, RectangleCollider _rectangle);

int IsColliding_Circle_Circle(CircleCollider _circle1, CircleCollider _circle2);

int IsColliding_Circle_Rectangle(CircleCollider _circle, RectangleCollider _rectangle);

int IsColliding_Rectangle_Rectangle(RectangleCollider _rectangle1, RectangleCollider _rectangle2);

#endif