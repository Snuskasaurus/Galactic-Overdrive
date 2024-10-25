#include "AnimatedStarsBackground.h"

// -- LOCAL FUNCTIONS ------------------------------------------------ //

void SetNewStarParallax(StarParallax* _parallax, int _spawnArea, sfSprite* _tabSprite[], sfVector2i _screenSize);
void CreateNewTrail(StarsBackground* _background, StarTrail* _trail, sfVector2i _screenSize);


// Set variable of your parallax
// _spawnArea possible value
// 0 : Can spawn on the screen visible area
// 1 : Spawn on screen top
void SetNewStarParallax(StarParallax* _parallax, int _spawnArea, sfSprite* _tabSprite[], sfVector2i _screenSize) // <--------------------------------------------------
{
	_parallax->depth = STAR_DEPTH_MIN + (float)(rand()) / ((float)(RAND_MAX / (STAR_DEPTH_NB - 1)));
	_parallax->scale = SCALE / _parallax->depth;
	_parallax->spriteIndex = rand() % STAR_PARALLAX_POOL_SIZE;
	sfVector2u spriteSize = sfTexture_getSize(sfSprite_getTexture(_tabSprite[_parallax->spriteIndex]));

	_parallax->position.x = (float)((int)(0 - spriteSize.x / 2) + rand() % (int)(_screenSize.x + (spriteSize.x / 2 / _parallax->scale) / 2));
	if (_spawnArea == 0)
	{
		_parallax->position.y = (float)((int)(0 - spriteSize.y / 2) + rand() % (int)(_screenSize.y + (spriteSize.y / 2 * _parallax->scale) / 2));
	}
	else
	{
		_parallax->position.y = -STAR_PARALLAX_H * _parallax->scale;
	}
}

void CreateNewTrail(StarsBackground* _background, StarTrail* _trail, sfVector2i _screenSize) // <--------------------------------------------------
{
	_trail->spriteIndex = rand() % STAR_TRAIL_POOL_SIZE;
	//sfVector2u spriteSize = sfTexture_getSize(sfSprite_getTexture(_background->poolTrail[_trail->spriteIndex]));
	_trail->position.x = (float)(rand() % _screenSize.x);
	_trail->position.y = (float)(STAR_TRAIL_START_SPAWN_Y + rand() % (_screenSize.y - STAR_TRAIL_START_SPAWN_Y));
	_trail->decayTimer = STAR_TRAIL_DECAY_TIMER_MIN + (float)(rand()) / ((float)(RAND_MAX / STAR_TRAIL_DECAY_TIMER_MAX));
	if (_trail->decayTimer > STAR_TRAIL_DECAY_TIMER_MAX)
	{
		_trail->decayTimer -= STAR_TRAIL_DECAY_TIMER_MIN;
	}
	_trail->timeToDecay = _trail->decayTimer;
	_trail->speed = STAR_TRAIL_SPEED_MIN + (float)(rand()) / ((float)(RAND_MAX / (STAR_TRAIL_SPEED_MAX - STAR_TRAIL_SPEED_MIN)));
	_trail->scale = (float)(SCALE / (1 + rand() % (STAR_TRAIL_DEPTH_MAX - 1)));
}

// Fill the screen with StarParallaxes
void InitBackgroundStars(StarsBackground* _background, Engine* _engine)
{
	for (int i = 0; i < STAR_PARALLAX_MAX; i++)
	{
		SetNewStarParallax(&_background->tabParallaxes[i], 0, _background->poolStarsParallax, _engine->configuration->resolution);
		_background->nbParallaxes++;
	}
}

void InitTrail(StarsBackground* _background, Engine* _engine)
{
	_background->nbTrails = 0;
	for (int i = 0; i < STAR_TRAIL_MAX; i++)
	{
		CreateNewTrail(_background, &_background->tabTrails[i], _engine->configuration->resolution);
		_background->nbTrails++;
	}
}

void StarsBackground_Init(StarsBackground* _background, Engine* _engine)
{
	_background->nbParallaxes = 0;
	_background->poolStarsParallax[0] = Sprite_CreateFromFile("Data/Sprites/star_1.png", 0,0);
	_background->poolStarsParallax[1] = Sprite_CreateFromFile("Data/Sprites/star_2.png", 0, 0);
	_background->poolTrail[0] = Sprite_CreateFromFile("Data/Sprites/Sprite-0006.png", 0, 0);
	_background->poolTrail[1] = Sprite_CreateFromFile("Data/Sprites/Sprite-0007.png", 0, 0);

	InitBackgroundStars(_background, _engine);
	InitTrail(_background, _engine);
}

// Move Trails and create new when decayed
void UpdateTrails(StarsBackground* _background, float _dt, Engine* _engine)
{
	for (int i = 0; i < _background->nbTrails; i++)
	{
		StarTrail* currentTrail = &_background->tabTrails[i];
		currentTrail->position.y += currentTrail->speed*_dt;
		currentTrail->decayTimer -= _dt;
		if (currentTrail->decayTimer < 0)
		{
			CreateNewTrail(_background, currentTrail, _engine->configuration->resolution);
		}
	}
}

// Move parallaxes
void UpdateParallaxes(StarsBackground* _background, float _dt, Engine* _engine)
{
	for (int i = 0; i < _background->nbParallaxes; i++)
	{
		_background->tabParallaxes[i].position.y += STAR_PARALLAX_SPEED / _background->tabParallaxes[i].depth * _dt;
		if (_background->tabParallaxes[i].position.y > _engine->configuration->resolution.y)
		{
			SetNewStarParallax(&_background->tabParallaxes[i], 1, _background->poolStarsParallax, _engine->configuration->resolution);
		}
	}
}

void StarsBackground_Update(StarsBackground* _background, float _dt, Engine* _engine)
{
	UpdateTrails(_background, _dt, _engine);
	UpdateParallaxes(_background, _dt, _engine);
}

void StarsBackground_DisplayStars(StarsBackground* _background, sfRenderTexture* _canvas)
{
	for (int i = 0; i < _background->nbParallaxes; i++)
	{
		StarParallax* parallax = &_background->tabParallaxes[i];
		///BlitSprite(_background->poolStarsParallax[parallax->spriteIndex], (int)parallax->position.x, (int)parallax->position.y, 0, parallax->scale, parallax->scale, sfWhite, _window);
		sfVector2f position_convertedToInt = parallax->position;
		position_convertedToInt.x = floorf(position_convertedToInt.x);
		position_convertedToInt.y = floorf(position_convertedToInt.y);
		Sprite_Display(_canvas, _background->poolStarsParallax[parallax->spriteIndex], position_convertedToInt, V2F(parallax->scale, parallax->scale), 0, NULL);
	}
}

void StarsBackground_DisplayTrails(StarsBackground* _background, sfRenderTexture* _canvas)
{
	for (int i = 0; i < _background->nbTrails; i++)
	{
		StarTrail* currentTrail = &_background->tabTrails[i];

		// Alpha changes
		sfColor color = { 255,255,255,255 };
		if (currentTrail->decayTimer < STAR_TRAIL_COLOR_DECAY_DURATION)
		{
			color.a = (char)(currentTrail->decayTimer / STAR_TRAIL_COLOR_DECAY_DURATION * 255);
		}
		else if (currentTrail->decayTimer > currentTrail->timeToDecay - STAR_TRAIL_TIME_TO_APPEAR)
		{
			/// appearingTimer start from "STAR_TRAIL_TIME_TO_APPEAR" and goes to 0
			float appearingTimer = currentTrail->decayTimer - currentTrail->timeToDecay - STAR_TRAIL_TIME_TO_APPEAR;
			color.a = (char)(255 - (appearingTimer / STAR_TRAIL_TIME_TO_APPEAR * 255));
		}

		sfVector2f position_convertedToInt = currentTrail->position;
		position_convertedToInt.x = floorf(position_convertedToInt.x);
		position_convertedToInt.y = floorf(position_convertedToInt.y);

		sfSprite_setColor(_background->poolTrail[currentTrail->spriteIndex], color);

		Sprite_Display(_canvas, _background->poolTrail[currentTrail->spriteIndex], position_convertedToInt, V2F(currentTrail->scale, currentTrail->scale), 0, NULL);
	}
}