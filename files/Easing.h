// Source : 
// http://gizma.com/easing/
// Thin about going to this website to preview your easing
//
// Note :
// You can fold everything simply with a shortcut
// On visual you can hold "Ctrl" then press "M" then press "O" (You have to press "M "and "O" while holding "Ctrl")

#ifndef EASING_H
#define EASING_H 


#include <math.h>
#include "_Defines.h"

// -- DEFINES ------------------------------------------------------------------- //

///#define PI 3.14159265359 // Already included in "_defines.h"

// -- PROTOTYPES DEFINITIONS ---------------------------------------------------- //

// -- Quadratic ---------------------------- //

/// t = CurrentTime
/// b = Value start
/// c = Value end
// d = Duration
float Easing_QuadOut(float _t, float _b, float _c, float _d);

/// t = CurrentTime
/// b = Value start
/// c = Value end
// d = Duration
float Easing_QuadIn(float _t, float _b, float _c, float _d);

/// t = CurrentTime
/// b = Value start
/// c = Value end
// d = Duration
float Easing_QuadInOut(float _t, float _b, float _c, float _d);

// -- Cubic -------------------------------------------- //

/// t = CurrentTime
/// b = Value start
/// c = Value end
// d = Duration
float Easing_CubicOut(float _t, float _b, float _c, float _d);

/// t = CurrentTime
/// b = Value start
/// c = Value end
// d = Duration
float Easing_CubicIn(float _t, float _b, float _c, float _d);

/// t = CurrentTime
/// b = Value start
/// c = Value end
// d = Duration
float Easing_CubicInOut(float _t, float _b, float _c, float _d);

// -- Exponential -------------------------------------------- //

/// t = CurrentTime
/// b = Value start
/// c = Value end
// d = Duration
float Easing_ExpOut(float _t, float _b, float _c, float _d);

/// t = CurrentTime
/// b = Value start
/// c = Value end
// d = Duration
float Easing_ExpIn(float _t, float _b, float _c, float _d);

/// t = CurrentTime
/// b = Value start
/// c = Value end
// d = Duration
float Easing_ExpInOut(float _t, float _b, float _c, float _d);

// -- Sinusoidal -------------------------------------------- //

/// t = CurrentTime
/// b = Value start
/// c = Value end
// d = Duration
float Easing_SinusOut(float _t, float _b, float _c, float _d);

/// t = CurrentTime
/// b = Value start
/// c = Value end
// d = Duration
float Easing_SinusIn(float _t, float _b, float _c, float _d);

/// t = CurrentTime
/// b = Value start
/// c = Value end
// d = Duration
float Easing_SinusInOut(float _t, float _b, float _c, float _d);

// -- Circular -------------------------------------------- //

/// t = CurrentTime
/// b = Value start
/// c = Value end
// d = Duration
float Easing_CircularOut(float _t, float _b, float _c, float _d);

/// t = CurrentTime
/// b = Value start
/// c = Value end
// d = Duration
float Easing_CircularIn(float _t, float _b, float _c, float _d);

/// t = CurrentTime
/// b = Value start
/// c = Value end
// d = Duration
float Easing_CircularInOut(float _t, float _b, float _c, float _d);

// -- Quartic -------------------------------------------- //

/// t = CurrentTime
/// b = Value start
/// c = Value end
// d = Duration
float Easing_QuarticOut(float _t, float _b, float _c, float _d);

/// t = CurrentTime
/// b = Value start
/// c = Value end
// d = Duration
float Easing_QuarticIn(float _t, float _b, float _c, float _d);

/// t = CurrentTime
/// b = Value start
/// c = Value end
// d = Duration
float Easing_QuarticInOut(float _t, float _b, float _c, float _d);

// -- Quintic ---------------------------------------------- //

/// t = CurrentTime
/// b = Value start
/// c = Value end
// d = Duration
float Easing_QuinticOut(float _t, float _b, float _c, float _d);

/// t = CurrentTime
/// b = Value start
/// c = Value end
// d = Duration
float Easing_QuinticIn(float _t, float _b, float _c, float _d);

/// t = CurrentTime
/// b = Value start
/// c = Value end
// d = Duration
float Easing_QuinticInOut(float _t, float _b, float _c, float _d);


#endif // !EASING_H
