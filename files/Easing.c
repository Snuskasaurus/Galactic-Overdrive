// Source : 
// http://gizma.com/easing/
// Thin about going to this website to preview your easing
//
// Note :
// You can fold everything simply with a shortcut
// On visual you can hold "Ctrl" then press "M" then press "O" (You have to press "M "and "O" while holding "Ctrl")

#include "Easing.h"

// -- Quadratic -------------------------------------------- //

// t = CurrentTime
// b = Value start
// c = Value end
// d = Duration
float Easing_QuadOut(float _t, float _b, float _c, float _d)
{
	_t /= _d;
	return -_c * _t*(_t - 2) + _b;
}

// t = CurrentTime
// b = Value start
// c = Value end
// d = Duration
float Easing_QuadIn(float _t, float _b, float _c, float _d)
{
	_t /= _d;
	return _c * _t*_t + _b;
}

// t = CurrentTime
// b = Value start
// c = Value end
// d = Duration
float Easing_QuadInOut(float _t, float _b, float _c, float _d)
{
	_t /= _d / 2;
	if (_t < 1) return _c / 2 * _t*_t + _b;
	_t--;
	return -_c / 2 * (_t*(_t - 2) - 1) + _b;
}
// -- Cubic ------------------------------------------------ //

// t = CurrentTime
// b = Value start
// c = Value end
// d = Duration
float Easing_CubicOut(float _t, float _b, float _c, float _d)
{
	_t /= _d;
	_t--;
	return _c * (_t*_t*_t + 1) + _b;
}

// t = CurrentTime
// b = Value start
// c = Value end
// d = Duration
float Easing_CubicIn(float _t, float _b, float _c, float _d)
{
	_t /= _d;
	return _c * _t*_t*_t + _b;
}

// t = CurrentTime
// b = Value start
// c = Value end
// d = Duration
float Easing_CubicInOut(float _t, float _b, float _c, float _d)
{
	_t /= _d / 2;
	if (_t < 1) return _c / 2 * _t*_t*_t + _b;
	_t -= 2;
	return _c / 2 * (_t*_t*_t + 2) + _b;
}

// -- Exponential ------------------------------------------ //

// t = CurrentTime
// b = Value start
// c = Value end
// d = Duration
float Easing_ExpOut(float _t, float _b, float _c, float _d)
{
	return _c * ((float)-pow(2, -10 * _t / _d) + 1) + _b;
}

// t = CurrentTime
// b = Value start
// c = Value end
// d = Duration
float Easing_ExpIn(float _t, float _b, float _c, float _d)
{
	return _c * (float)pow(2, 10 * (_t / _d - 1)) + _b;
}

// t = CurrentTime
// b = Value start
// c = Value end
// d = Duration
float Easing_ExpInOut(float _t, float _b, float _c, float _d)
{
	_t /= _d / 2;
	if (_t < 1) return _c / 2 * (float)pow(2, 10 * (_t - 1)) + _b;
	_t--;
	return _c / 2 * ((float)-pow(2, -10 * _t) + 2) + _b;
}

// -- Sinusoidal ------------------------------------------- //

// t = CurrentTime
// b = Value start
// c = Value end
// d = Duration
float Easing_SinusOut(float _t, float _b, float _c, float _d)
{
	return _c * (float)sin(_t / _d * (PI / 2)) + _b;
}

// t = CurrentTime
// b = Value start
// c = Value end
// d = Duration
float Easing_SinusIn(float _t, float _b, float _c, float _d)
{
	return -_c * (float)cos(_t / _d * (PI / 2)) + _c + _b;
}

// t = CurrentTime
// b = Value start
// c = Value end
// d = Duration
float Easing_SinusInOut(float _t, float _b, float _c, float _d)
{
	return -_c / 2 * ((float)cos(PI*_t / _d) - 1) + _b;
}

// -- Circular --------------------------------------------- //

// t = CurrentTime
// b = Value start
// c = Value end
// d = Duration
float Easing_CircularOut(float _t, float _b, float _c, float _d)
{
	_t /= _d;
	_t--;
	return _c * (float)sqrt(1 - _t * _t) + _b;
}

// t = CurrentTime
// b = Value start
// c = Value end
// d = Duration
float Easing_CircularIn(float _t, float _b, float _c, float _d)
{
	_t /= _d;
	return -_c * ((float)sqrt(1 - _t * _t) - 1) + _b;
}

// t = CurrentTime
// b = Value start
// c = Value end
// d = Duration
float Easing_CircularInOut(float _t, float _b, float _c, float _d)
{
	_t /= _d / 2;
	if (_t < 1) return -_c / 2 * ((float)sqrt(1 - _t * _t) - 1) + _b;
	_t -= 2;
	return _c / 2 * ((float)sqrt(1 - _t * _t) + 1) + _b;
}

// -- Quartic ---------------------------------------------- //

// t = CurrentTime
// b = Value start
// c = Value end
// d = Duration
float Easing_QuarticOut(float _t, float _b, float _c, float _d)
{
	_t /= _d;
	_t--;
	return -_c * (_t*_t*_t*_t - 1) + _b;
}

// t = CurrentTime
// b = Value start
// c = Value end
// d = Duration
float Easing_QuarticIn(float _t, float _b, float _c, float _d)
{
	_t /= _d;
	return _c * _t*_t*_t*_t + _b;
}

// t = CurrentTime
// b = Value start
// c = Value end
// d = Duration
float Easing_QuarticInOut(float _t, float _b, float _c, float _d)
{
	_t /= _d / 2;
	if (_t < 1) return _c / 2 * _t*_t*_t*_t + _b;
	_t -= 2;
	return -_c / 2 * (_t*_t*_t*_t - 2) + _b;
}

// -- Quintic ---------------------------------------------- //

// t = CurrentTime
// b = Value start
// c = Value end
// d = Duration
float Easing_QuinticOut(float _t, float _b, float _c, float _d)
{
	_t /= _d;
	_t--;
	return _c * (_t*_t*_t*_t*_t + 1) + _b;
}

// t = CurrentTime
// b = Value start
// c = Value end
// d = Duration
float Easing_QuinticIn(float _t, float _b, float _c, float _d)
{
	_t /= _d;
	return _c * _t*_t*_t*_t*_t + _b;
}

// t = CurrentTime
// b = Value start
// c = Value end
// d = Duration
float Easing_QuinticInOut(float _t, float _b, float _c, float _d)
{
	_t /= _d / 2;
	if (_t < 1) return _c / 2 * _t*_t*_t*_t*_t + _b;
	_t -= 2;
	return _c / 2 * (_t*_t*_t*_t*_t + 2) + _b;
}