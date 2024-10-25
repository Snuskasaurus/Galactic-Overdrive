#ifndef WORLD_H
#define WORLD_H

// -- INCLUDES ------------------------------------------------------------- //

#include "_Engine.h"

#include "Level.h"

// -- FUNCTIONS ------------------------------------------------------------ //

World World_Init(Engine *_engine, const char *_worldFileName);

void World_Create(const char *_levelName, const char *_worldName, int _position);

void World_Free(World *_world);

#endif // !WORLD_H