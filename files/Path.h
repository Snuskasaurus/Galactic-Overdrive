#ifndef PATH__H
#define PATH__H

// -- INCLUDES ------------------------------------------------------------- //

#include "_Includes.h"
#include "_Engine.h"

// -- FUNCTIONS ------------------------------------------------------------ //

Paths Path_Initialize();

Path *Path_Create(Paths *_paths, sfVector2f _position);

void Path_Display(Engine *_engine, Paths *_paths, float _timeLevel);

void Path_Destroy(Paths *_paths, Path *_toDestroy);

Waypoint *_Waypoint_Create(Paths *_paths, WaypointType _type, sfVector2f _position);

void Waypoint_Remove(Paths *_paths);

Waypoint *Path_SelectWaypoint(Paths *_paths, sfVector2f _position, float _timeLevel);

void Path_RecalculateData(Paths *_paths);

void Path_SetDifficulty(Paths *_paths, float _multiplier);

#endif // !LIST__H