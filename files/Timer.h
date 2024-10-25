#ifndef TIMER_H
#define TIMER_H

#include "_Structures.h"

//Functions

Timer Timer_Create(float _maxTime);

void Timer_Update(Timer *_timer, float _dt);

// Returns true if the timer is finished
sfBool Timer_Check(Timer *_timer);

void Timer_Reset(Timer *_timer);

void Timer_Pause(Timer *_timer);

#endif