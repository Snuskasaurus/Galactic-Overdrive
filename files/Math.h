#ifndef MATH__H
#define MATH__H

// -- HEADERS -------------------------------------------------------------- //

#include <SFML/System.h>
#include <SFML/Window.h>
#include <SFML/Graphics.h>
#include <Math.h>

// -- DEFINES -------------------------------------------------------------- //

#define PI 3.14f
#define MAX_DEGREES 360.f
#define HALF_MAX_DEGREES 180.f
#define MIN_DEGREES 0
#define RAD_TO_DEG(_radian)		(_radian * (HALF_MAX_DEGREES / PI))
#define DEG_TO_RAD(_degres)		(_degres * (PI / HALF_MAX_DEGREES))

// -- FUNCTIONS ------------------------------------------------------------ //

// Convert an angle in radiant to an angle in degres
float Angle_RadToDeg(float _radian);

// Convert an angle in degres to an angle in radiant
float Angle_DegToRad(float _degres);

// Calculate and return an angle in radian between two int positions
float Angle_RadGetAngle(sfVector2f _position1, sfVector2f _position2);

// Calculate and return an angle in degres between two int positions
float Angle_DegGetAngle(sfVector2f _position1, sfVector2f _position2);

float Angle_CalculateDistance(sfVector2f origin1, sfVector2f origin2);

float Math_DotProduct(sfVector2f u, sfVector2f v);

float Math_CrossProduct(sfVector2f u, sfVector2f v);

float Math_Norm(sfVector2f u);

// Convert a negative degree angle to a positive one
// For exemple -90° would become his equivalant 270°
float Angle_DegPositive(float x);

// Get the primary measure of an angle
// x must be in degree
// positiveReturn must be TRUE or FALSE
float Angle_PrimaryMeasure(float x, int positiveReturn);

#endif //!MATH__H