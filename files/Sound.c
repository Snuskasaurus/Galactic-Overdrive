#include "Sound.h"

// -- SHARED FUNCTIONS ----------------------------------------------------- //

SoundEngine *Sound_InitializeEngine(int _maximumChannels)
{
	SoundEngine *engine;

	// Initialize the sound structure
	engine = calloc(1, sizeof(SoundEngine));
	if (_maximumChannels >= SOUND_SFML_MAXIMUM)
	{
		_maximumChannels = SOUND_SFML_MAXIMUM;
	}
	engine->maximumChannels = _maximumChannels;
	// Initialize the channels
	engine->currentChannel = 0;
	engine->channels = calloc(engine->maximumChannels, sizeof(sfSound*));
	for (int i = 0; i < engine->maximumChannels; i++)
	{
		engine->channels[i] = sfSound_create();
	}
	return engine;
}

void Sound_SetVolume(SoundEngine *_engine, SoundType _type, float _newVolume)
{
	if (_newVolume >= 0.0f && _newVolume <= 100.0f)
	{
		_engine->volumes[_type] = _newVolume;
	}
	else
	{
		_engine->volumes[_type] = 50.0f;
	}
}

void Sound_ModifyVolume(SoundEngine *_engine, SoundType _type, float _offset)
{
	_engine->volumes[_type] += _offset;
	if (_engine->volumes[_type] < 0.0f)
	{
		_engine->volumes[_type] = 0.0f;
	}
	if (_engine->volumes[_type] > 100.0f)
	{
		_engine->volumes[_type] = 100.0f;
	}
}

void Sound_CreateFromFile(SoundEngine *_engine, const char *_filename, SoundType _type)
{
	Sound *newSound = NULL;
	int sizeFilename;
	char filePath[BUFFER_MEDIUM] = SOUND_FOLDER;
	char fileName[BUFFER_MEDIUM] = {0}; // The _filename without the folders names

	// Create the filename and set the filepath
	strcat(filePath, _filename);
	char *ptr = strrchr(_filename, '/');
	if (ptr)
	{
		strcpy(fileName, strrchr(_filename, '/') + 1);
	}
	else
	{
		strcpy(fileName, _filename);
	}

	// Add a room for a new sound
	_engine->nbSounds++;
	newSound = calloc(1, sizeof(Sound));
	_engine->sounds = realloc(_engine->sounds, _engine->nbSounds * sizeof(Sound*));
	_engine->sounds[_engine->nbSounds - 1] = newSound;
	// Add the new sound
	newSound->buffer = sfSoundBuffer_createFromFile(filePath);
	// Set the type of the new sound
	newSound->type = _type;
	// Copy the filename to the sound
	sizeFilename = (int)(strlen(fileName) + 1);
	newSound->name = malloc(sizeFilename * sizeof(char));
	strcpy(newSound->name, fileName);
}

Sound *Sound_GetFromName(SoundEngine *_engine, const char *_soundName)
{
	Sound *sound = NULL;

	for (int i = 0; i < _engine->nbSounds; i++)
	{
		sound = _engine->sounds[i];
		if (sound->name != NULL)
		{
			if (_strcmpi(_soundName, sound->name) == 0)
			{
				///printf("sound played : %s from %s\n", sound->name, _soundName);
				return sound;
			}
			else
			{
				sound = NULL;
			}
		}
	}
	if (!sound)
	{
		Sound_CreateFromFile(_engine, _soundName, SOUND_EFFECT);
		sound = Sound_GetFromName(_engine, _soundName);
	}
	return sound;
}

sfSound *Sound_Play(SoundEngine *_engine, Sound *_sound)
{
	float volume;
	sfSound *currentChannel = _engine->channels[_engine->currentChannel];
	if (_sound)
	{
		// Reset loop
		sfSound_setLoop(currentChannel, sfFalse);
		// Set the buffer
		sfSound_setBuffer(currentChannel, _sound->buffer);
		// Set the volume
		volume = _engine->volumes[SOUND_GLOBAL] / 100.0f * _engine->volumes[_sound->type];
		sfSound_setVolume(currentChannel, volume);
		// Reset pitch
		sfSound_setPitch(currentChannel, 1.0f);
		// Play the sound
		sfSound_play(currentChannel);
		// Change the next channel
		_engine->currentChannel++;
		if (_engine->currentChannel >= _engine->maximumChannels)
		{
			_engine->currentChannel = 0;
		}
	}
	return currentChannel;
}

sfSound *Sound_PlayFromName(SoundEngine *_engine, const char *_soundName)
{
	Sound *sound = NULL;

	sound = Sound_GetFromName(_engine, _soundName);

	return Sound_Play(_engine, sound);
}

void Sound_StopAll(SoundEngine *_engine)
{
	for (int i = 0; i < _engine->maximumChannels; i++)
	{
		sfSound_stop(_engine->channels[i]);
	}
}