#ifndef MONEY__H
#define MONEY__H

#include "_Engine.h"
#include "Entity.h"
#include "Map.h"
#include "Collider.h"
#include "Explosion.h"

void Money_Create(Engine *_engine, Money** _List, sfVector2f _position, MoneyAmount _amount);
void Money_Destroy(Engine *_engine, Money** _List, Money* _ToErase, List *_explosions);
void Money_Update(Money** _List, float _dt, int _maxY);
void Money_Display(Money** _List, sfSprite* _sprite[3], Engine* _engine);
void MoneyAmount_Display(Engine *_engine, unsigned int _moneyAmount, GameData* _stateData);
void Money_DestroyAll(Money** _List);

#endif