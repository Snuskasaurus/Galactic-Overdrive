#ifndef SHADOW__H
#define SHADOW__H

// -- INCLUDES ------------------------------------------------------------- //

#include "_Engine.h"

// -- FUNCTIONS ------------------------------------------------------------ //

void Shadow_Display(Engine *_engine, Entity *_entity, sfSprite *_sprite);

void Shadow_DisplayPlayersShadow(Engine *_engine, Players *_players, sfBool _displayShadow);

void Shadow_DisplayEnemiesShadow(Engine *_engine, Enemies *_enemies, sfBool _displayShadow);

void Shadow_DisplayBossShadow(Engine *_engine, Boss *_boss, sfBool _displayShadow);

#endif // !SHADOW__H