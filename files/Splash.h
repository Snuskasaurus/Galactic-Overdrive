#ifndef SPLASH__H
#define SPLASH__H

// -- INCLUDES ------------------------------------------------------------- //

#include "_MainData.h"

// Entities
#include "Player.h"
#include "Weapon.h"
#include "Turret.h"
#include "Projectiles.h"
#include "Enemy.h"
#include "Explosion.h"
#include "Boss.h"
#include "Difficulty.h"
#include "World.h"

// Mechanics
#include "Skill.h"

// -- ENUMERATIONS --------------------------------------------------------- //

enum SplashState
{
	APPEAR_LOGO1,
	FADE_LOGO1,
	APPEAR_LOGO2,
	FADE_LOGO2,
	APPEAR_LOGO3,
	FADE_LOGO3,
	END_SPLASH
};

// -- DEFINES -------------------------------------------------------------- //

#define SPLASH_DELAY 2.0f
#define SPLASH_DELAY_LONGER 5.0f

// -- STRUCTURES ----------------------------------------------------------- //

// Data used to store all informations about the state
struct SplashData
{
	sfSprite* logoPixelMatter;
	sfSprite* logoCreajeux;
	sfSprite* imgController;
	float currentTime;
	enum SplashState state;
	char alpha;
	sfBool keyIsPressed;
};

// -- TYPES DEFINITIONS ---------------------------------------------------- //

typedef struct SplashData SplashData;

// -- MAIN FUNCTION DEFINITIONS -------------------------------------------- //

void Splash_Initialize(MainData *_data);
void Splash_HandleEvent(MainData *_data, sfEvent _event);
void Splash_Update(MainData *_data, float _dt);
void Splash_Display(MainData *_data);
void Splash_Exit(MainData *_data);

#endif // !SPLASH__H