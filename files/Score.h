#ifndef SCORE__H
#define SCORE__H

// -- INCLUDES ------------------------------------------------------------- //

#include "_Engine.h"

// Components
#include "Collider.h"
#include "Entity.h"

// Mechanics
#include "Skill.h"
#include "Achievements.h"

// Others Entities
#include "Explosion.h"


// -- FUNCTIONS ------------------------------------------------------------ //

void Score_GainByWeaponDamage(unsigned long* _score, Enemy* _enemy, float _damage, Players *_players, Difficulty *_currentDif);

void Score_GainByDeath(MainData *_data, unsigned long * _score, Enemy * _enemy, Players *_players, List *_explosions, Difficulty *_currentDif);

void Score_Display(unsigned long _score, Engine *_engine, Text *_parametersText);

#endif // !SCORE__H