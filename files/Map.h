#ifndef MAP_H
#define MAP_H

// -- INCLUDES ------------------------------------------------------------- //

#include "_Includes.h"
#include "_Engine.h"
#include "Level.h"
#include "AnimatedStarsBackground.h"

// -- TYPES DEFINITIONS ---------------------------------------------------- //


// -- ENUMERATIONS --------------------------------------------------------- //

// -- DEFINES -------------------------------------------------------------- //

#define MAP_MAX 4
#define MAP_SCROLLING_SPEED 200
#define MAP_SCALE 3.0f
#define MAP_GENERATE_AT_Y -200

// -- STRUCTURES ----------------------------------------------------------- //

// -- FUNCTIONS ------------------------------------------------------------ //

Map* Map_Initialize();

void Map_DisplayForeground(Engine *_engine, Map _maps);
void Map_Update(Engine *_engine, Map* _maps, float _dt);
void Map_Display(Engine *_engine, StarsBackground* _starbackground, Level* _level);


#endif
