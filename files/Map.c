#include "Map.h"

// NOTES :
// Possibilité de se passer de .position en calculant les coordonées directement quand on draw
// Faire le load de sprite automatique pour les parallax

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //
void Map_UpdateAnimation(Map* _maps, float _dt);
void Display_Parallax(Engine *_engine, Map _maps, MapParallax* _parallax, int _nbParallax);
// -- SHARED FUNCTIONS ----------------------------------------------------- //

// -- LOCAL FUNCTIONS ------------------------------------------------------ //

Map* Map_Initialize()
{
	return calloc(1, sizeof(Map));
}

// Load the sprite of the parallax
// Occur just before the parallax show up on screen
void Map_LoadIncomingParallax(float _time, MapParallax* _currentParallax, Animations* _anims)
{
	if (_currentParallax->isSpriteLoaded == 0 && _time > _currentParallax->timerToUpdate - 1 - INTRO_DURATION)
	{
		_currentParallax->startingPosition.y = -_currentParallax->size.y * MAP_SCALE;
		_currentParallax->isSpriteLoaded = 1;

		// Animation
		char tempChar[BUFFER_SMALL] = { 0 };
		_currentParallax->animation = GetAnim(_anims, Editor_removeDotPNG(ExtractFilenameFromFilepath(_currentParallax->filePath), tempChar), "loop");
		if (_currentParallax->animation != NULL)
		{
			// Size unscaled
			_currentParallax->size = Sprite_GetSize(_currentParallax->animation->sprites[0]);
			// Set TimeMax
			_currentParallax->entity.animationTimer.maxTime = _currentParallax->animation->maxTime;
			_currentParallax->entity.animationTimer.isPlaying = sfTrue;
			for (int i = 0; i < _currentParallax->animation->nbrFrames; i++)
			{
				sfSprite_setOrigin(_currentParallax->animation->sprites[i], V2F(0, 0));
			}
		}
		// Sprite
		else if (_currentParallax->animation == NULL)
		{
			_currentParallax->sprite = Sprite_CreateFromFile(_currentParallax->filePath, 0, 0);
			// Size unscaled
			_currentParallax->size = Sprite_GetSize(_currentParallax->sprite);
		}
	}
}

void Map_UpdatePosition(Map* _maps, float _dt, Engine* _engine)
{
	// Update 3 maps at a time :
	// The active map and both the previous and next map	

	int forStart = _maps->activeMap - 1;
	int forEnd = _maps->activeMap + 1;
	if (forStart < 0)
	{
		forStart = 0;
	}
	if (forEnd > _maps->nbTerrains)
	{
		forEnd = _maps->nbTerrains - 1;
	}

	for (int i = forStart; i <= forEnd; i++)
	{
		// Scroll the map
		_maps->terrains[i].position.y += _maps->scrollingSpeed * _dt;

		// If the active map Y is higher then 0, then the next map become the new active map
		if (i > _maps->activeMap && _maps->terrains[_maps->activeMap].position.y > 0)
		{
			_maps->activeMap = i;
		}
	}

	// Update backgrounds parallaxes
	for (int b = 0; b < _maps->nbOfBckgrndParallax; b++)
	{
		Map_LoadIncomingParallax(_maps->time, &_maps->tabParallaxBckgrnd[b], _engine->anims);
		//if (_maps->time - INTRO_DURATION > _maps->tabParallaxBckgrnd[b].timerToUpdate)
		if (_maps->time > _maps->tabParallaxBckgrnd[b].timerToUpdate - INTRO_DURATION)
		{
			_maps->tabParallaxBckgrnd[b].position.y = _maps->tabParallaxBckgrnd[b].startingPosition.y + (_maps->time - (_maps->tabParallaxBckgrnd[b].timerToUpdate - INTRO_DURATION)) * _maps->tabParallaxBckgrnd[b].speed;
			///_maps->tabParallaxBckgrnd[b].position.y = _maps->tabParallaxBckgrnd[b].startingPosition.y + (_maps->time - _maps->tabParallaxBckgrnd[b].timerToUpdate) * _maps->tabParallaxBckgrnd[b].speed;
		}
	}
	// Update foregrounds parallaxes
	for (int f = 0; f < _maps->nbOfForgrndParallax; f++)
	{
		Map_LoadIncomingParallax(_maps->time, &_maps->tabParallaxForgrnd[f], _engine->anims);

		if (_maps->time > _maps->tabParallaxForgrnd[f].timerToUpdate - INTRO_DURATION)
		{
			_maps->tabParallaxForgrnd[f].position.y = _maps->tabParallaxForgrnd[f].startingPosition.y + (_maps->time - (_maps->tabParallaxForgrnd[f].timerToUpdate - INTRO_DURATION)) * _maps->tabParallaxForgrnd[f].speed;
		}
	}
}

void Map_LoadNextMap(Map* _maps, Animations* _anims)
{
	// If the current map is above Y -200 (on screen) then we load the next map sprite
	if (_maps->terrains[_maps->activeMap].position.y > MAP_GENERATE_AT_Y)
	{
		if (_maps->activeMap < _maps->nbTerrains - 1 && _maps->terrains[_maps->activeMap + 1].isSpriteLoaded == 0)
		{
			Terrain* currentTerrain = &_maps->terrains[_maps->activeMap + 1];
			// Try to load Animation
			currentTerrain->animation = NULL;
			char tempChar[BUFFER_SMALL] = { 0 };
			currentTerrain->animation = GetAnim(_anims, Editor_removeDotPNG(ExtractFilenameFromFilepath(currentTerrain->filePath), tempChar), "loop");
			if (currentTerrain->animation != NULL)
			{
				currentTerrain->entity.animationTimer.maxTime = currentTerrain->animation->maxTime;
				currentTerrain->entity.animationTimer.currentTime = 0;
				currentTerrain->entity.animationTimer.isPlaying = sfTrue;
				currentTerrain->entity.angle = 0;
				for (int i = 0; i < currentTerrain->animation->nbrFrames; i++)
				{
					sfSprite_setOrigin(currentTerrain->animation->sprites[i], V2F(0, 0));
				}
			}
			// Load Sprite
			else
			{
				currentTerrain->sprite = Sprite_CreateFromFile(_maps->terrains[_maps->activeMap + 1].filePath, 0, 0);
				// Get the map size unscaled
				currentTerrain->size = Sprite_GetSize(_maps->terrains[_maps->activeMap + 1].sprite);
			}
			// Set the Y
			_maps->terrains[_maps->activeMap + 1].position.x = 0;
			_maps->terrains[_maps->activeMap + 1].position.y = _maps->terrains[_maps->activeMap].position.y - (_maps->terrains[_maps->activeMap + 1].size.y*MAP_SCALE);
			// Specify that this map has been loaded
			_maps->terrains[_maps->activeMap + 1].isSpriteLoaded = 1;
		}
	}
}

void Map_Update(Engine *_engine, Map* _maps, float _dt)
{
	_maps->time += _dt;
	Map_UpdatePosition(_maps, _dt, _engine);
	Map_LoadNextMap(_maps, _engine->anims);
	Map_UpdateAnimation(_maps, _dt);
}

void Map_DisplayParallax(Engine* _engine, MapParallax* _currentParallax)
{
	// Display Animation
	if (_currentParallax->animation != NULL)
	{
		_currentParallax->entity.position.x = _currentParallax->startingPosition.x + _currentParallax->position.x;
		_currentParallax->entity.position.y = _currentParallax->position.y;
		DisplayAnimation(_engine->canvas, _currentParallax->animation, &_currentParallax->entity, V2F_FROM_VALUE(MAP_SCALE), NULL);
	}
	// Display Sprite
	else
	{
		Sprite_Display(_engine->canvas, _currentParallax->sprite, V2F(_currentParallax->startingPosition.x, _currentParallax->position.y), V2F_FROM_VALUE(MAP_SCALE), _currentParallax->entity.angle, NULL);
	}
}

void InitRepeatedTerrainPosition(Map *_map)
{
	float position1 = floorf(_map->terrains[_map->nbTerrains - 1].position.y - _map->terrains[_map->nbTerrains - 1].size.y * MAP_SCALE);
	float position2 = floorf(position1 - _map->terrains[_map->nbTerrains - 1].size.y * MAP_SCALE);
	float position3 = floorf(position2 - _map->terrains[_map->nbTerrains - 1].size.y * MAP_SCALE);

	_map->positionRepeatedTerrainY[0] = position1;
	_map->positionRepeatedTerrainY[1] = position2;
	_map->positionRepeatedTerrainY[2] = position3;

	_map->wasRepeatedTerrainInitialized = 1;
}

void Map_DisplayLastTerrainsRepeatedly(Level* _level, Engine * _engine)
{
	// Once the last terrain has been shawn
	// We create 3 copy of the lastTerrain and use them indefinitely to create this impression of endless scrolling
	// When one of the copy get below the bottom of the screen it will be instantly replaced on top of the 2 others
	//  3rd	     1	    2	   3
	//  2nd -->  3 -->  1 -->  2 --> Over and over again
	//  1st	     2	    3      1
	Terrain* lastTerrain = &_level->map.terrains[_level->map.nbTerrains - 1];
	if (_level->map.wasRepeatedTerrainInitialized == 0)
	{
		InitRepeatedTerrainPosition(&_level->map);
	}

	// -- Update position
	for (int i = 0; i < MAP_TERRAIN_REPETITION_NB; i++)
	{
		_level->map.positionRepeatedTerrainY[i] += _level->map.scrollingSpeed * _engine->dt;
	}

	// -- Reposition if out of screen
	for (int i = 0; i < MAP_TERRAIN_REPETITION_NB; i++)
	{
		if (_level->map.positionRepeatedTerrainY[i] >= _engine->configuration->resolution.y)
		{
			_level->map.positionRepeatedTerrainY[i] = floorf(_level->map.positionRepeatedTerrainY[(i + 2) % 3] - lastTerrain->size.y * MAP_SCALE);
		}
	}

	// -- Draw
	for (int i = 0; i < MAP_TERRAIN_REPETITION_NB; i++)
	{
		if (lastTerrain->animation != NULL)
		{
			Entity currentTerrainEntity = { 0 };
			currentTerrainEntity.position.x = 0;
			currentTerrainEntity.position.y = floorf(_level->map.positionRepeatedTerrainY[i]);
			currentTerrainEntity.currentFrame = lastTerrain->entity.currentFrame;

			DisplayAnimation(_engine->canvas, lastTerrain->animation, &currentTerrainEntity, V2F_FROM_VALUE(MAP_SCALE), NULL);
		}
		else
		{
			// Display
			Sprite_Display(_engine->canvas, _level->map.terrains[_level->map.nbTerrains - 1].sprite, V2F(0, (int)(_level->map.positionRepeatedTerrainY[i])), V2F_FROM_VALUE(MAP_SCALE), 0, NULL);
		}
	}

}

void Map_Display(Engine *_engine, StarsBackground* _starbackground, Level* _level)
{
	Map* map = &_level->map;
	int forStart = map->activeMap - 1;
	int forEnd = map->activeMap + 1;
	if (forStart < 0)
	{
		forStart = 0;
	}
	if (forEnd > map->nbTerrains)
	{
		forEnd = map->nbTerrains;
	}

	// Display the background
	if (map->animatedBackground != NULL)
	{
		DisplayAnimation(_engine->canvas, map->animatedBackground, &map->entityBackground, V2F_FROM_VALUE(MAP_SCALE), NULL);
	}

	if (_level->isInSpace)
	{
		StarsBackground_DisplayStars(_starbackground, _engine->canvas);
	}

	// Draw background parallax
	Display_Parallax(_engine, *map, map->tabParallaxBckgrnd, map->nbOfBckgrndParallax);

	for (int i = forStart; i < forEnd; i++)
	{
		// Draw the terrains
		if (map->terrains[i].animation != NULL)
		{
			map->terrains[i].entity.position.x = map->terrains[i].position.x;
			map->terrains[i].entity.position.y = map->terrains[i].position.y;
			DisplayAnimation(_engine->canvas, map->terrains[i].animation, &map->terrains[i].entity, V2F_FROM_VALUE(MAP_SCALE), NULL);
		}
		else
		{
			Sprite_Display(_engine->canvas, map->terrains[i].sprite, map->terrains[i].position, V2F(MAP_SCALE, MAP_SCALE), 0, NULL);
		}
	}

	// Draw last Terrain repetitions
	// If the last terrain position Y is at the top screen or below
	if (map->nbTerrains > 0 && map->terrains[map->nbTerrains - 1].position.y + map->terrains[map->nbTerrains - 1].size.y * MAP_SCALE >= 0 && _level->terrainsAreRepeated)
	{
		Map_DisplayLastTerrainsRepeatedly(_level, _engine);
	}

	if (_level->isInSpace)
	{
		StarsBackground_DisplayTrails(_starbackground, _engine->canvas);
	}
}

void Display_Parallax(Engine *_engine, Map _maps, MapParallax* _parallax, int _nbParallax)
{
	for (int i = 0; i < _nbParallax; i++)
	{
		float paraDiagLenght = (float)sqrt((_parallax[i].size.y*_parallax[i].size.y)*MAP_SCALE + (_parallax[i].size.x*_parallax[i].size.x)*MAP_SCALE);
		if (_maps.time >= _parallax[i].timerToUpdate - INTRO_DURATION && _parallax[i].position.y < _engine->configuration->resolution.y + paraDiagLenght * 1.5)
		{
			Map_DisplayParallax(_engine, &_parallax[i]);
		}
	}
}

void Map_DisplayForeground(Engine *_engine, Map _maps)
{
	// Draw the foreground parallax
	Display_Parallax(_engine, _maps, _maps.tabParallaxForgrnd, _maps.nbOfForgrndParallax);
}

void Map_UpdateAnimation(Map* _maps, float _dt)
{
	// -- Update Background ------------------------------
	if (_maps->animatedBackground != NULL)
	{
		UpdateAnimation(_dt, _maps->animatedBackground, &_maps->entityBackground);
	}

	// -- Update Parallaxes ------------------------------
	for (int i = 0; i < _maps->nbOfBckgrndParallax; i++)
	{
		if (_maps->tabParallaxBckgrnd[i].isSpriteLoaded)
		{
			UpdateAnimation(_dt, _maps->tabParallaxBckgrnd[i].animation, &_maps->tabParallaxBckgrnd[i].entity);
		}
	}
	for (int i = 0; i < _maps->nbOfForgrndParallax; i++)
	{
		if (_maps->tabParallaxForgrnd[i].isSpriteLoaded)
		{
			UpdateAnimation(_dt, _maps->tabParallaxForgrnd[i].animation, &_maps->tabParallaxForgrnd[i].entity);
		}
	}

	// -- Update Terrain ---------------------------------
	int forStart = _maps->activeMap - 1;
	int forEnd = _maps->activeMap + 1;
	if (forStart < 0)
	{
		forStart = 0;
	}
	if (forEnd > _maps->nbTerrains)
	{
		forEnd = _maps->nbTerrains;
	}
	for (int i = forStart; i < forEnd; i++)
	{
		if (_maps->terrains[i].animation != NULL)
		{
			UpdateAnimation(_dt, _maps->terrains[i].animation, &_maps->terrains[i].entity);
		}
	}
}