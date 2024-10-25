#include "Math.h"

// -- SHARED FUNCTIONS ----------------------------------------------------- //

// Convert an angle in radiant to an angle in degres
float Angle_RadToDeg(float _radian)
{
	return _radian * (180.0f / PI);
}

// Convert an angle in degres to an angle in radiant
float Angle_DegToRad(float _degres)
{
	return _degres * (PI / 180.0f);
}

// Calculate and return an angle in radian between two positions
float Angle_RadGetAngle(sfVector2f _position1, sfVector2f _position2)
{
	return atan2f(_position2.y - _position1.y, _position2.x - _position1.x);
}

// Calculate and return an angle in degres between two positions
float Angle_DegGetAngle(sfVector2f _position1, sfVector2f _position2)
{
	float angle = atan2f(_position2.y - _position1.y, _position2.x - _position1.x);
	return Angle_RadToDeg(angle);
}

float Angle_CalculateDistance(sfVector2f origin1, sfVector2f origin2)
{
	float xDistance = 0.0f;
	float yDistance = 0.0f;

	xDistance = origin1.x - origin2.x;
	yDistance = origin1.y - origin2.y;
	return (float)sqrt((double)(xDistance * xDistance + yDistance * yDistance));
}

float Math_DotProduct(sfVector2f u, sfVector2f v)
{
	return (float)(u.x * v.x + u.y * v.y);
}

float Math_CrossProduct(sfVector2f u, sfVector2f v)
{
	return u.x * v.y - u.y * v.x;
}

float Math_Norm(sfVector2f u)
{
	return (float)sqrt(u.x * u.x + u.y * u.y);
}

// Convert a negative degree angle to a positive one
// For exemple -90° would become his equivalant 270°
float Angle_DegPositive(float x)
{
	if (x < 0.f)
	{
		x = 180.f + (180.f - (float)abs((int)x));
	}
	return x;
}

// Get the primary measure of an angle
// x must be in degree
// positiveReturn must be TRUE or FALSE
float Angle_PrimaryMeasure(float x, int positiveReturn)
{
	if (x < 0)
	{
		while (x <= -180)
		{
			x = x + 360;
		}
	}
	else
	{
		while (x > 180)
		{
			x = x - 360;
		}
	}
	if (positiveReturn == 1)
	{
		x = Angle_DegPositive(x);
	}
	return x;
}