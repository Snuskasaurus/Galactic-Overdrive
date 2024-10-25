#include "Waypoint.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

// -- SHARED FUNCTIONS ----------------------------------------------------- //

Waypoint *Waypoint_Create(Engine *_engine, sfVector2f _position, Waypoint *_previous)
{
	Waypoint *waypoint = NULL;
	waypoint = calloc(1, sizeof(Waypoint));
	if (waypoint)
	{
		waypoint->position = _position;
		waypoint->previous = _previous;
		if (_previous)
		{
			waypoint->previous = _previous;
			waypoint->spawn = _previous->spawn;
		}
	}
	return waypoint;
}

void Waypoint_Display(Engine *_engine, Waypoint *_waypoint)
{
	static sfSprite *sprite = NULL;
	if (!sprite)
	{
		sprite = Sprite_CreateFromFile("Data/Sprites/Engine/Waypoint.png", 0.5f, 0.5f);
	}
	Sprite_Display(_engine->canvas, sprite, _waypoint->position, V2F(1.0, 1.0), 0, NULL);
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //