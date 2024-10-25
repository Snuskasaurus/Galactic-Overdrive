#ifndef CONFIGURATION_H
#define CONFIGURATION_H

// -- INCLUDES --------------------------------------------------------------- //

#include "_Includes.h"
#include "Sound.h"

// -- DEFINES ---------------------------------------------------------------- //

// Used to store data in Documents folder
#define MAIN_FOLDER "My Games"
// Name of the game
// Used to store data in Documents folder inside the GAME_STUDIO folder
// Used to name the game window
#define GAME_NAME "Galactic Overdrive"
// Keep track of the game version
// x.0.0.0 = Major build number : Major milestone in the game.
// 0.x.0.0 = Minor build number : Used for feature updates, large bug fixes.
// 0.0.x.0 = Revision : Minor alterations on existing features, small bug fixes.
#define GAME_VERSION "01.00.01"

#define CONFIG_INITIAL_SCREEN_WIDTH 1920
#define CONFIG_INITIAL_SCREEN_HEIGHT 1080
#define CONFIG_INITIAL_FULLSCREEN 1
#define CONFIG_INITIAL_VSYNC 1
#define CONFIG_INITIAL_FRAMERATE 120
#define CONFIG_INITIAL_SOUND_CHANNEL 127
#define CONFIG_INITIAL_GLOBAL_VOLUME 50
#define CONFIG_INITIAL_EFFECT_VOLUME 50
#define CONFIG_INITIAL_VOICE_VOLUME 50
#define CONFIG_INITIAL_MUSIC_VOLUME 50

#define SCREEN_WIDTH_USED_IN_SCALE 1920 // X resolution used to calculate the scale of the game (CURRENT_RESOLUTION / RESOLUTION_USED_IN_SCALE)
#define SCREEN_HEIGHT_USED_IN_SCALE 1080 // Y resolution used to calculate the scale of the game (CURRENT_RESOLUTION / RESOLUTION_USED_IN_SCALE)

// -- ENUMERATIONS ----------------------------------------------------------- //

// -- STRUCTURES ------------------------------------------------------------- //

struct Configuration
{
	char *gameFilePath;
	sfBool isInitial;
	char version[10];
	sfVector2i resolution;
	int fullscreen;
	int vsync;
	int framerate; // Maximum number of frames each seconds
	sfVector2f scale;
	float minScale;
	int soundChannel;
	int globalVolume;
	int effectVolume;
	int voiceVolume;
	int musicVolume;
};
typedef struct Configuration Configuration;

// -- PROTOTYPES ------------------------------------------------------------- //

Configuration *Configuration_GetValues();

void Configuration_Save(Configuration *_struct);

void Configuration_RestoreToDefault(Configuration *_struct);

void Configuration_Destroy(Configuration *_struct);

sfRenderWindow *Configuration_CreateWindow(Configuration *_struct);

sfVector2f Configuration_GetScale(Configuration *_struct);

float Configuration_GetVolume(Configuration *_configuration, SoundType _type);

#endif