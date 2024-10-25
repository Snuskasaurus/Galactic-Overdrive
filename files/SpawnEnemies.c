#include "SpawnEnemies.h"

// -- SHARED FUNCTIONS ----------------------------------------------------- //

void Mechanic_SpawnEnemies(MainData *_data, Level *_level, Enemies *_enemies, Players *players, float _dt, Boss *_boss, Difficulty *_currentDif)
{
	if (_level->map.time < _level->bossTime)
	{
		for (int i = 0; i < _level->paths.number; i++)
		{
			Waypoint_Spawn *spawn = _level->paths.array[i]->first->content;
			Player *target = NULL;

			if (spawn->difficulty <= players->number)
			{
				if (_level->map.time >= spawn->timeStart && _level->map.time <= spawn->timeStart + spawn->duration)
				{
					// Spawn an enemy if the timer is reseted and the maximum of enemies is not reached
					if (spawn->since == 0.0f && spawn->spawned < spawn->maximum)
					{
						// Choose a random player for a target
						if (players && players->number > 0)
						{
							target = players->array[RAND(0, players->number - 1)];
						}
						// Create the enemy
						Enemy_Create(&_data->engine, _enemies, _level->paths.array[i]->first, target, _currentDif);
						spawn->spawned++;

						if (_data->currentState == GAME)
						{
							_data->achievementsUnlockVar.nbEnemiesSpawned++;
						}
					}
					// Update the time and reset it if the maximum is reached
					spawn->since += _dt;
					if (spawn->since >= spawn->rate)
					{
						spawn->since = 0.0f;
					}
				}
			}
		}
	}
	else
	{
		if (_boss && _strcmpi("NULL", _boss->name) == 0)
		{
			_boss->isActive = sfTrue;
			_boss->isAlive = sfFalse;
		}
		else if (_boss)
		{
			_boss->isActive = sfTrue;
		}
	}
}