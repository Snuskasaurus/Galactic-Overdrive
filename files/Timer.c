#include "Timer.h"


Timer Timer_Create(float _maxTime)
{
	Timer timer;
	timer.currentTime = 0.0f;
	timer.maxTime = _maxTime;
	timer.isPlaying = sfTrue;
	return timer;
}

void Timer_Update(Timer *_timer, float _dt)
{
	if (_timer->isPlaying == sfTrue)
	{
		_timer->currentTime = _timer->currentTime + _dt;
	}
}

// Returns true if the timer is finished
sfBool Timer_Check(Timer *_timer)
{
	if (_timer->currentTime >= _timer->maxTime)
	{
		return sfTrue;
	}
	else
	{
		return sfFalse;
	}
}

void Timer_Reset(Timer *_timer)
{
	_timer->currentTime = 0;
	_timer->isPlaying = sfTrue;
}

void Timer_Pause(Timer *_timer)
{
	_timer->isPlaying = sfFalse;
}