#include "Money.h"

void Money_Create(Engine *_engine, Money** _List, sfVector2f _position, MoneyAmount _amount)
{
	Money* NewMoney = (Money*)calloc(1, sizeof(Money));
	NewMoney->Next = *_List;
	// Initialize the aniamtion
	switch (_amount)
	{
	default:
		NewMoney->animation = GetAnim(_engine->anims, "Money", "Money_Copper");
		break;
	case MONEY_500:
		NewMoney->animation = GetAnim(_engine->anims, "Money", "Money_Silver");
		break;
	case MONEY_1000:
		NewMoney->animation = GetAnim(_engine->anims, "Money", "Money_Gold");
		break;
	}
	InitEntityAnim(&NewMoney->entity, NewMoney->animation->maxTime);
	// Various initialization
	NewMoney->entity.position = _position;
	NewMoney->collider = Collider_CreateCircleCollider(NewMoney->entity.position, 16);
	NewMoney->Amount = _amount;
	*_List = NewMoney;
}

void Money_Destroy(Engine *_engine, Money** _List, Money* _ToErase, List *_explosions)
{
	Money* TempMoney = *_List;
	if (_ToErase == *_List)
	{
		*_List = (*_List)->Next;
	}
	else
	{
		while (TempMoney->Next && TempMoney->Next != _ToErase)
		{
			TempMoney = TempMoney->Next;
		}
		TempMoney->Next = _ToErase->Next;
	}
	if (_explosions)
	{
		sfVector2f position = _ToErase->entity.position;
		position.y -= MONEY_OFFSET_TEXT;
		switch (_ToErase->Amount)
		{
		default:
			Explosion_Create(_engine, _explosions, position, EXPLOSION_SIZE_MONEY_100, EXPLOSION_TYPE_TEXT);
			break;
		case MONEY_100:
			Explosion_Create(_engine, _explosions, position, EXPLOSION_SIZE_MONEY_100, EXPLOSION_TYPE_TEXT);
			break;
		case MONEY_500:
			Explosion_Create(_engine, _explosions, position, EXPLOSION_SIZE_MONEY_500, EXPLOSION_TYPE_TEXT);
			break;
		case MONEY_1000:
			Explosion_Create(_engine, _explosions, position, EXPLOSION_SIZE_MONEY_1000, EXPLOSION_TYPE_TEXT);
			break;
		}
	}
	Collider_Destroy(&_ToErase->collider);
	free(_ToErase);
}

void Money_Update(Money** _List, float _dt, int _maxY)
{
	Money* TempMoney = *_List;
	while (TempMoney)
	{
		UpdateAnimation(_dt, TempMoney->animation, &TempMoney->entity);
		TempMoney->entity.position.y += MAP_SCROLLING_SPEED * _dt;
		Collider_UpdatePosition(&TempMoney->collider, TempMoney->entity.position);
		if (TempMoney->entity.position.y >= _maxY)
		{
			Money* TempErase = TempMoney;
			TempMoney = TempMoney->Next;
			Money_Destroy(NULL, _List, TempErase, NULL);
		}
		else
		{
			TempMoney = TempMoney->Next;
		}
	}
}

void Money_Display(Money** _List, sfSprite* _sprite[3], Engine* _engine)
{
	Money* TempMoney = *_List;
	while (TempMoney)
	{
		sfVector2f scale = _engine->gameScale;
		switch (TempMoney->Amount)
		{
		default: // COPPER
			scale = V2F_MUL_VALUE(scale, 0.5f);
			break;
		case MONEY_500: // SILVER
			scale = V2F_MUL_VALUE(scale, 0.75f);
			break;
		case MONEY_1000: // GOLD
			scale = V2F_MUL_VALUE(scale, 1.0f);
			break;
		}
		DisplayAnimation(_engine->canvas, TempMoney->animation, &TempMoney->entity, scale, NULL);
		TempMoney = TempMoney->Next;
	}
}

// Displays the amount of money you have with text
void MoneyAmount_Display(Engine *_engine, unsigned int _moneyAmount, GameData* _stateData)
{
	sfVector2f tempScale = _engine->configuration->scale;
	char money[256];
	sprintf(money, "MONEY :\t %d", _moneyAmount);
	Text_Display(_engine->canvas, &_stateData->parametersText, money, V2F(0.0f, 30.0f * tempScale.y), V2F(0, 0));
}

void Money_DestroyAll(Money** _List)
{
	Money* TempMoney = *_List;
	while (TempMoney)
	{
		Money* TempErase = TempMoney;
		TempMoney = TempMoney->Next;
		Collider_Destroy(&TempErase->collider);
		free(TempErase);
	}
	*_List = NULL;
}