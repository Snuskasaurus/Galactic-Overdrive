#ifndef SOUND__H
#define SOUND__H

// -- INCLUDES ------------------------------------------------------------- //

#include "_Includes.h"

// -- TYPES DEFINITIONS ---------------------------------------------------- //

typedef enum SoundType SoundType;
typedef struct SoundEngine SoundEngine;
typedef struct Sound Sound;

// -- ENUMERATIONS --------------------------------------------------------- //

// -- DEFINES -------------------------------------------------------------- //

#define SOUND_FOLDER "Data/Sounds/"
#define SOUND_SFML_MAXIMUM 126
#define SOUND_TYPES_NB 4

// -- STRUCTURES ----------------------------------------------------------- //

enum SoundType
{
	SOUND_GLOBAL,
	SOUND_EFFECT,
	SOUND_VOICE,
	SOUND_MUSIC,
};

struct Sound
{
	char *name; // The name of the buffer with his extension
	sfSoundBuffer *buffer;
	SoundType type;
};

struct SoundEngine
{
	float volumes[SOUND_TYPES_NB];
	int maximumChannels; // Maximum sound in the game at the same time
	int currentChannel; // The next channel to use
	sfSound **channels;
	int nbSounds;
	Sound **sounds;
};


// -- SHARED FUNCTIONS DEFINITIONS ----------------------------------------- //

SoundEngine *Sound_InitializeEngine(int _maximumChannels);

void Sound_SetVolume(SoundEngine *_engine, SoundType _type, float _newVolume);

void Sound_ModifyVolume(SoundEngine *_engine, SoundType _type, float _offset);

void Sound_CreateFromFile(SoundEngine *_engine, const char *_fileName, SoundType _type);

Sound *Sound_GetFromName(SoundEngine *_engine, const char *_soundName);

sfSound *Sound_PlayFromName(SoundEngine *_engine, const char *_soundName);

sfSound *Sound_Play(SoundEngine *_engine, Sound *_sound);

void Sound_StopAll(SoundEngine *_engine);

#endif // !SOUND__H