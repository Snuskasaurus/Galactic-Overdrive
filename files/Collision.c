#include "Collision.h"

// -- SHARED FUNCTIONS ------------------------------------------------------- //

int IsColliding_Circle_Point(CircleCollider _circle, sfVector2f _point)
{
	float distance = Angle_CalculateDistance(_circle.position, _point);
	if (distance <= _circle.radius)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int IsColliding_Point_Rectangle(sfVector2f _pointPosition, RectangleCollider _rectangle)
{
	sfVector2f position = _rectangle.position;

	if (_rectangle.centered)
	{
		position.x -= _rectangle.size.x / 2.0f;
		position.y -= _rectangle.size.y / 2.0f;
	}
	if (_pointPosition.x >= position.x && _pointPosition.x <= position.x + _rectangle.size.x)
	{
		if (_pointPosition.y >= position.y && _pointPosition.y <= position.y + _rectangle.size.y)
		{
			return 1;
		}
	}
	return 0;
}

int IsColliding_Circle_Circle(CircleCollider _circle1, CircleCollider _circle2)
{
	float range = ((_circle2.position.x - _circle1.position.x) * (_circle2.position.x - _circle1.position.x))
		+ ((_circle2.position.y - _circle1.position.y) * (_circle2.position.y - _circle1.position.y));

	if (range < ((_circle1.radius + _circle2.radius) * (_circle1.radius + _circle2.radius)))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int IsColliding_Circle_Rectangle(CircleCollider _circle, RectangleCollider _rectangle)
{
	sfVector2f circleDistance;
	double cornerDistance_sq;

	if (!_rectangle.centered)
	{
		_rectangle.position.x += _rectangle.size.x / 2.0f;
		_rectangle.position.y += _rectangle.size.y / 2.0f;
	}

	circleDistance.x = (float)fabs(_circle.position.x - _rectangle.position.x);
	circleDistance.y = (float)fabs(_circle.position.y - _rectangle.position.y);

	if (circleDistance.x > ((_rectangle.size.x / 2) + _circle.radius))
	{
		return 0;
	}

	if (circleDistance.y > ((_rectangle.size.y / 2) + _circle.radius))
	{
		return 0;
	}

	if (circleDistance.x <= (_rectangle.size.x / 2))
	{
		return 1;
	}

	if (circleDistance.y <= (_rectangle.size.y / 2))
	{
		return 1;
	}

	cornerDistance_sq = (pow((circleDistance.x - _rectangle.size.x / 2), 2) + (pow((circleDistance.y - _rectangle.size.y / 2), 2)));

	if (cornerDistance_sq <= (pow(_circle.radius, 2)))
	{
		return 1;
	}
	return 0;
}

int IsColliding_Rectangle_Rectangle(RectangleCollider _rectangle1, RectangleCollider _rectangle2)
{
	sfFloatRect rectangle1;
	sfFloatRect rectangle2;
	// Create first rectangle
	if (_rectangle1.centered)
	{
		rectangle1.left = _rectangle1.position.x - (_rectangle1.size.x / 2.f);
		rectangle1.top = _rectangle1.position.y - (_rectangle1.size.y / 2.f);
	}
	else
	{
		rectangle1.left = _rectangle1.position.x;
		rectangle1.top = _rectangle1.position.y;
	}
	rectangle1.width = _rectangle1.size.x;
	rectangle1.height = _rectangle1.size.y;
	// Create secondary rectangle
	if (_rectangle2.centered)
	{
		rectangle2.left = _rectangle2.position.x - (_rectangle2.size.x / 2.f);
		rectangle2.top = _rectangle2.position.y - (_rectangle2.size.y / 2.f);
	}
	else
	{
		rectangle2.left = _rectangle2.position.x;
		rectangle2.top = _rectangle2.position.y;
	}
	rectangle2.width = _rectangle2.size.x;
	rectangle2.height = _rectangle2.size.y;

	return (int)sfFloatRect_intersects(&rectangle1, &rectangle2, NULL);
}