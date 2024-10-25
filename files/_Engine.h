#ifndef ENGINE__H
#define ENGINE__H

// -- INCLUDES ------------------------------------------------------------- //

#include "_Includes.h"

#include "Configuration.h"
#include "Sprite.h"
#include "Sound.h"
#include "Text.h"
#include "Animation.h"
#include "Shaders.h"

// -- TYPES DEFINITIONS ---------------------------------------------------- //

typedef struct Engine Engine;

// -- DEFINES -------------------------------------------------------------- //

// -- STRUCTURES ----------------------------------------------------------- //

struct Engine
{
	sfRenderWindow *window; // The main Window of the game
	sfRenderTexture *canvas; // The canvas of the used by shaders
	sfClock *clock; // Clock used to calculate dt.
	float dt;
	float time;
	Configuration *configuration;
	SoundEngine *sound;
	TextEngine *text;
	Animations *anims;
	Blueprint blueprints;
	Skills skills;
	sfVector2f gameScale;
	Difficulty currentDifficulty;
	World world;
};

#endif // !ENGINE__H