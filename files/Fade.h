#ifndef FADE_H
#define FADE_H
// -- INCLUDES ------------------------------------------------------------- //
#include "_Includes.h"
#include "_Engine.h"
#include "_MainData.h"

// -- PROTOTYPES DEFINITIONS ---------------------------------------------------- //

void Fade_Init(MainData* _data);

// ----- Notes :
// -- This function will create a transition with a fade o/i effect 
// -- Once called it will start a fade out
// -- When the fade out end, your function "_ptrFunctionChangeState" will be called
// -- And then a fade in start, to slowly show the new state
void Fade_CreateTransition(TransitionFade* _fade, void(*_ptrFunctionChangeState)(MainData*));

void Fade_Update(MainData* _data);
void Fade_Display(MainData* _data);


#endif // !FADE_H
