#ifndef WAYPOINT__H
#define WAYPOINT__H

// -- INCLUDES ------------------------------------------------------------- //

#include "_Engine.h"

// -- FUNCTIONS ------------------------------------------------------------ //

Waypoint *Waypoint_Create(Engine *_engine, sfVector2f _position, Waypoint *_previous);

void Waypoint_Display(Engine *_engine, Waypoint *_waypoint);

#endif // !WEAPONS__H