#include "Path.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

void ReallocatePath(Paths *_paths);
void DestroyWaypoint(Waypoint *_waypoint);
Waypoint_Spawn *CreateSpawn();
void _DisplayWaypoint(Engine *_engine, Waypoint *_waypoint, Waypoint *_selected);

// -- SHARED FUNCTIONS ----------------------------------------------------- //

Paths Path_Initialize()
{
	Paths paths;
	paths.maximumNumber = PATH_INITIALIZATION_NUMBER;
	paths.number = 0;
	paths.array = calloc(paths.maximumNumber, sizeof(Path*));
	paths.selected = NULL;
	return paths;
}

Path *Path_Create(Paths *_paths, sfVector2f _position)
{
	Path *newPath = calloc(1, sizeof(Path));
	if (newPath)
	{
		_paths->number++;
		if (_paths->number >= _paths->maximumNumber)
		{
			ReallocatePath(_paths);
		}
		_paths->array[_paths->number - 1] = newPath;
		printf("Create a new path\n");
	}
	return newPath;
}

void Path_Display(Engine *_engine, Paths *_paths, float _timeLevel)
{
	for (int i = 0; i < _paths->number; i++)
	{
		sfVector2f positions[2];
		Waypoint *current = _paths->array[i]->first;
		Waypoint_Spawn *spawn = _paths->array[i]->first->content;
		// Check is the spawn is not out of the map
		if ((spawn->timeStart <= _timeLevel && spawn->timeStart + spawn->duration + 1.0f >= _timeLevel) || current == _paths->selected)
		{
			do
			{
				// Display a line to the next waypoint
				if (current->next)
				{
					positions[0] = current->position;
					positions[1] = current->next->position;
					Sprite_DisplayLine(_engine->canvas, positions, sfColor_fromRGBA(255, 255, 255, 255));
				}
				// Display the waypoint
				_DisplayWaypoint(_engine, current, _paths->selected);
				// Set the next waypoint
				current = current->next;
			} while (current);
		}
	}
}

void Path_SaveToFile(FILE *_file, Paths *_paths)
{
	// Write the number of paths
	fwrite(&_paths->number, 1, sizeof(int), _file);
	for (int i = 0; i < _paths->number; i++)
	{
		// Write the number of waypoints
		fwrite(&_paths->array[i]->number, 1, sizeof(int), _file);
		Waypoint *current = _paths->array[i]->first;
		do
		{
			// Write the waypoint position
			fwrite(&current->position, 1, sizeof(sfVector2f), _file);
			// Write the waypoint type
			fwrite(&current->type, 1, sizeof(WaypointType), _file);
			// Write the waypoint content
			switch (current->type)
			{
			case WAYPOINT_SPAWN:
			{
				Waypoint_Spawn *spawn = current->content;
				int sizeName = 0;
				// Write the structure
				fwrite(&current->position, 1, sizeof(Waypoint_Spawn), _file);
				// write the enemy name
				sizeName = (int)(strlen(spawn->enemyName) + 1);
				fwrite(&sizeName, 1, sizeof(int), _file);
				fwrite(&spawn->enemyName, sizeName, sizeof(char), _file);
				break;
			}
			}
		} while (current);
	}
}

void Path_AddFromFile(FILE *_file, Paths *_paths)
{

}

void Path_Destroy(Paths *_paths, Path *_toDestroy)
{
	int index = 0;
	Path *currentPath = NULL;
	// Search the index of the path
	do
	{
		currentPath = _paths->array[index];
		index++;
	} while (currentPath != _toDestroy);
	// Destroy all the waypoints of this path
	Waypoint *currentWaypoint = _toDestroy->first;
	Waypoint *temporaryWaypoint = _toDestroy->first;
	while (currentWaypoint)
	{
		temporaryWaypoint = currentWaypoint;
		currentWaypoint = currentWaypoint->next;
		DestroyWaypoint(temporaryWaypoint);
	}
	// Destroy the element and move the paths
	_paths->number--;
	free(_toDestroy);
	for (int i = index - 1; i < _paths->number; i++)
	{
		_paths->array[i] = _paths->array[i + 1];
	}
}

Waypoint *_Waypoint_Create(Paths *_paths, WaypointType _type, sfVector2f _position)
{
	Waypoint *newWaypoint = calloc(1, sizeof(Waypoint));
	if (newWaypoint)
	{
		if (_paths->selected != NULL && _type != WAYPOINT_SPAWN)
		{
			newWaypoint->previous = _paths->selected;
			newWaypoint->next = _paths->selected->next;
			newWaypoint->path = _paths->selected->path;
			newWaypoint->path->number++;
			_paths->selected->next = newWaypoint;
			if (_paths->selected->next)
			{
				_paths->selected->next->previous = newWaypoint;
			}
		}
		else
		{
			_type = WAYPOINT_SPAWN;
		}
		switch (_type)
		{
		case WAYPOINT_SPAWN:
		{
			newWaypoint->path = Path_Create(_paths, _position);
			newWaypoint->path->first = newWaypoint;
			newWaypoint->path->last = newWaypoint;
			newWaypoint->path->number = 1;
			newWaypoint->content = (void*)CreateSpawn();
			break;
		}
		}
		_paths->selected = newWaypoint;
		newWaypoint->position = _position;
		newWaypoint->type = _type;
		return newWaypoint;
	}
	return newWaypoint;
}

void Waypoint_Remove(Paths *_paths)
{
	// The selected waypoint have a next and a previosu waypoint
	if (_paths->selected->previous && _paths->selected->next)
	{
		_paths->selected->previous->next = _paths->selected->next;
		_paths->selected->next->previous = _paths->selected->previous;
		DestroyWaypoint(_paths->selected);
		_paths->selected = NULL;
	}
	// The selected waypoint is the only one of his path
	else if (_paths->selected->next == NULL && _paths->selected->previous == NULL)
	{
		Path_Destroy(_paths, _paths->selected->path);
		_paths->selected = NULL;
	}
	// The selected waypoint is the last of his path
	else if (_paths->selected->next == NULL)
	{
		_paths->selected->path->last = _paths->selected->previous;
		_paths->selected->previous = NULL;
		DestroyWaypoint(_paths->selected);
		_paths->selected = NULL;
	}
	// The selected waypoint is the first of his path
	else if (_paths->selected->previous == NULL)
	{
		_paths->selected->path->first = _paths->selected->next;
		DestroyWaypoint(_paths->selected);
		_paths->selected = NULL;
	}
}

Waypoint *Path_SelectWaypoint(Paths *_paths, sfVector2f _position, float _timeLevel)
{
	CircleCollider collider;
	collider.radius = 10.0f;
	for (int i = 0; i < _paths->number; i++)
	{
		Waypoint_Spawn *spawn = _paths->array[i]->first->content;
		if (spawn->timeStart <= _timeLevel && spawn->timeStart + 1.0f + spawn->duration >= _timeLevel)
		{
			//sfVector2f positions[2];
			Waypoint *current = _paths->array[i]->first;
			do
			{
				collider.position = current->position;
				if (IsColliding_Circle_Point(collider, _position))
				{
					_paths->selected = current;
					return _paths->selected;
				}
				// Set the next waypoint
				current = current->next;
			} while (current);
		}
	}
	_paths->selected = NULL;
	return _paths->selected;
}

void Path_RecalculateData(Paths *_paths)
{
	for (int i = 0; i < _paths->number; i++)
	{
		Waypoint_Spawn *spawn = _paths->array[i]->first->content;
		if (spawn->duration == 0.0f)
		{
			spawn->duration = 0.01f;
		}
		spawn->rate = spawn->duration / (float)spawn->maximum;
		///printf("spawn at %.2f + %.2f\n", spawn->timeStart, spawn->duration);
		///printf("RESET SPAWNRATE = %f/%d = %f\n", spawn->duration, spawn->maximum, spawn->rate);
	}
}

void Path_SetDifficulty(Paths *_paths, float _multiplier)
{
	for (int i = 0; i < _paths->number; i++)
	{
		Waypoint_Spawn *spawn = _paths->array[i]->first->content;
		spawn->maximum = (int)(spawn->maximum * _multiplier);
	}
	Path_RecalculateData(_paths);
}


// -- LOCAL FUNCTIONS ------------------------------------------------------ //

void ReallocatePath(Paths *_paths)
{
	_paths->maximumNumber += PATH_REALLOC_OFFSET;
	_paths->array = realloc(_paths->array, _paths->maximumNumber * sizeof(Path*));
}

void DestroyWaypoint(Waypoint *_waypoint)
{
	switch (_waypoint->type)
	{
	default:

		break;
	case WAYPOINT_SPAWN:
		free(_waypoint->content);
		break;
	}
	free(_waypoint);
}

void _DisplayWaypoint(Engine *_engine, Waypoint *_waypoint, Waypoint *_selected)
{
	static sfSprite *selectedWaypoint = NULL;
	static sfSprite *defaultWaypoint = NULL;
	static sfSprite *waypoint_Movement = NULL;
	static sfSprite *waypoint_Spawn = NULL;
	if (!selectedWaypoint || !defaultWaypoint || !waypoint_Movement || !waypoint_Spawn)
	{
		selectedWaypoint = Sprite_CreateFromFile("Data/Editor/Waypoint_Selected.png", 0.5f, 0.5f);
		defaultWaypoint = Sprite_CreateFromFile("Data/Editor/Waypoint_Default.png", 0.5f, 0.5f);
		waypoint_Movement = Sprite_CreateFromFile("Data/Editor/Waypoint_Movement.png", 0.5f, 0.5f);
		waypoint_Spawn = Sprite_CreateFromFile("Data/Editor/Waypoint_Spawn.png", 0.5f, 0.5f);
	}
	// Display the the background
	if (_waypoint == _selected)
	{
		Sprite_Display(_engine->canvas, selectedWaypoint, _waypoint->position, V2F(1.0f, 1.0f), 0, NULL);
		sfSprite_setColor(waypoint_Movement, sfColor_fromRGB(255, 120, 0));
		sfSprite_setColor(waypoint_Spawn, sfColor_fromRGB(255, 120, 0));
	}
	else
	{
		Sprite_Display(_engine->canvas, defaultWaypoint, _waypoint->position, V2F(1.0f, 1.0f), 0, NULL);
		sfSprite_setColor(waypoint_Movement, sfWhite);
		sfSprite_setColor(waypoint_Spawn, sfWhite);
	}
	// Display the content
	switch (_waypoint->type)
	{
	default:
		break;
	case WAYPOINT_MOVE:
		Sprite_Display(_engine->canvas, waypoint_Movement, _waypoint->position, V2F(1.0f, 1.0f), 0, NULL);
		break;
	case WAYPOINT_SPAWN:
		Sprite_Display(_engine->canvas, waypoint_Spawn, _waypoint->position, V2F(1.0f, 1.0f), 0, NULL);
		break;
	}
}

Waypoint_Spawn *CreateSpawn()
{
	Waypoint_Spawn *spawn = calloc(1, sizeof(Waypoint_Spawn));
	spawn->timeStart = 0.0f;
	spawn->duration = SPAWN_DURATION_MAXIMUM / 2.0f;
	spawn->maximum = SPAWN_ENEMY_NUMBER_MAXIMUM - SPAWN_ENEMY_NUMBER_MINIMUM / 2;
	spawn->rate = spawn->duration / (float)spawn->maximum;
	strcpy(spawn->enemyName, SPAWN_DEFAULT_ENEMYNAME);
	return spawn;
}