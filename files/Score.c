#include "Score.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //



// -- SHARED FUNCTIONS ----------------------------------------------------- //

// Score giving by dealing damage to the enemy
void Score_GainByWeaponDamage(unsigned long* _score, Enemy* _enemy, float _damage, Players *_players, Difficulty *_currentDif)
{
	if (_damage >= _enemy->health)
	{
		long long int tempScore = (long long int)(_enemy->activeScorePool * _currentDif->scoreMultiplier);
		// Giving all the score pool if the enemy is dead
		if (tempScore > 0)
		{
			*_score += (unsigned long)tempScore;
			Skill_AddSkillScore(_players, (unsigned long)tempScore, _currentDif);
			_enemy->activeScorePool = 0;
		}
	}
	else
	{
		// Giving an ratio of the score pool of the enemy
		long long int tempScore = (long long int)(_enemy->totalScorePool * (_damage / _enemy->health));

		if (tempScore > 0)
		{
			*_score += (unsigned long)tempScore;
			Skill_AddSkillScore(_players, (unsigned long)tempScore, _currentDif);
			_enemy->activeScorePool -= (int)tempScore;
		}
	}
}

// Score giving by killing the enemy
void Score_GainByDeath(MainData *_data, unsigned long * _score, Enemy * _enemy, Players *_players, List *_explosions, Difficulty *_currentDif)
{
	sfVector2f positionText = _enemy->entity.position;
	positionText.y += SCORE_OFFSET_TEXT;
	_data->achievementsUnlockVar.nbEnemiesKilled++;
	if (_enemy->goodHitAmount == _enemy->hitAmount) // PERFECT
	{
		unsigned long tempScore = (unsigned long)((_enemy->totalScorePool * SCORE_EFFICIENT) * _currentDif->scoreMultiplier);
		*_score += tempScore;
		Skill_AddSkillScore(_players, tempScore, _currentDif);
		Explosion_Create(&_data->engine, _explosions, positionText, EXPLOSION_SIZE_PERFECT, EXPLOSION_TYPE_TEXT);

		// Achievements
		if (_enemy->type == TYPE_CINETIC)
		{
			Achievement_Unclock(&_data->notifications, &_data->engine,"Perfect_Kinetic");
		}
		else if(_enemy->type == TYPE_ENERGY)
		{
			Achievement_Unclock(&_data->notifications, &_data->engine, "Perfect_Energetic");
		}
		else if (_enemy->type == TYPE_ARMOR)
		{
			Achievement_Unclock(&_data->notifications, &_data->engine, "Perfect_Explosive");
		}
	}
	else if (_enemy->goodHitAmount >= _enemy->hitAmount * SCORE_GOOD_PERCENTAGE) // GOOD
	{
		unsigned long tempScore = (unsigned long)((_enemy->totalScorePool * SCORE_NORMAL) * _currentDif->scoreMultiplier);
		*_score += tempScore;
		Skill_AddSkillScore(_players, tempScore, _currentDif);
		Explosion_Create(&_data->engine, _explosions, positionText, EXPLOSION_SIZE_GOOD, EXPLOSION_TYPE_TEXT);
	}
	else // NORMAL
	{
		unsigned long tempScore = (unsigned long)((_enemy->totalScorePool * SCORE_INEFFICIENT) * _currentDif->scoreMultiplier);
		*_score += tempScore;
		Skill_AddSkillScore(_players, tempScore, _currentDif);
	}
}

void Score_Display(unsigned long _score, Engine *_engine, Text *_parametersText)
{
	char score[256];
	sprintf(score, "SCORE : \t %ld", _score);
	Text_Display(_engine->canvas, _parametersText, score, V2F(0, 0), V2F(0, 0));
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //

