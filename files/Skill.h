#ifndef SKILL_H
#define SKILL_H

// -- INCLUDES --------------------------------------------------------------- //

#include "_Includes.h"
#include "_Engine.h"
#include "Animation.h"
#include "Collider.h"
#include "List.h"
#include "Laser.h"

// -- SHARED PROTOTYPES ------------------------------------------------------ //

Skills Skill_Load(Engine *_engine);

Skill Skill_Get(eSkill _skillType, Skills *_skills);

void Skill_EventButtonPressed(joystickId _joystick, joystickButton _button, GameData *_state, Engine *_engine);

void Skill_EventButtonReleased(joystickId _joystick, joystickButton _button, GameData *_state);

void Skill_AddSkillScore(Players *_players, unsigned long _scoreToAdd, Difficulty *_currentDif);

void Skill_Update(float _dt, GameData *_state);

void Skill_Display(Engine *_engine, GameData *_state);

#endif