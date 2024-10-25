#include "Configuration.h"

void Configuration_CreateGameFileDirectory(Configuration *_configuration);

// -- FUNCTIONS -------------------------------------------------------------- //

// Read configuration on file
void GetConfiguration(FILE *_file, Configuration *_struct)
{
	fscanf(_file, "version : %s\n", &_struct->version); // Set the current version fo the game
	fscanf(_file, "resolution : %d %d\n", &_struct->resolution.x, &_struct->resolution.y); // Set the resolution
	fscanf(_file, "fullscreen : %d\n", &_struct->fullscreen); // Set if fullscreen
	fscanf(_file, "vsync : %d\n", &_struct->vsync); // Set if vsync
	fscanf(_file, "framerate : %d\n", &_struct->framerate); // Set the max framerate
	fscanf(_file, "soundChannel : %d\n", &_struct->soundChannel); // Set the number of channel used in the game
	fscanf(_file, "globalVolume : %d\n", &_struct->globalVolume); // Set the global volume
	fscanf(_file, "effectVolume : %d\n", &_struct->effectVolume); // Set the effect volume
	fscanf(_file, "voiceVolume : %d\n", &_struct->voiceVolume); // Set the voice volume
	fscanf(_file, "musicVolume : %d", &_struct->musicVolume); // Set the music volume
}

// Write configuration on file
void SetConfiguration(FILE *_file, Configuration *_struct)
{
	fprintf(_file, "version : %s\n", _struct->version); // Set the current version fo the game
	fprintf(_file, "resolution : %d %d\n", _struct->resolution.x, _struct->resolution.y); // Set the resolution
	fprintf(_file, "fullscreen : %d\n", _struct->fullscreen); // Set if fullscreen
	fprintf(_file, "vsync : %d\n", _struct->vsync); // Set if vsync
	fprintf(_file, "framerate : %d\n", _struct->framerate); // Set the max framerate
	fprintf(_file, "soundChannel : %d\n", _struct->soundChannel); // Set the number of channel used in the game
	fprintf(_file, "globalVolume : %d\n", _struct->globalVolume); // Set the global volume
	fprintf(_file, "effectVolume : %d\n", _struct->effectVolume); // Set the effect volume
	fprintf(_file, "voiceVolume : %d\n", _struct->voiceVolume); // Set the voice volume
	fprintf(_file, "musicVolume : %d", _struct->musicVolume); // Set the music volume
}

// Set the initial values in the structure
void SetDefaultConfiguration(Configuration *_struct)
{
	strcpy(_struct->version, GAME_VERSION); // Copy the game version to the configuration
	_struct->resolution.x = CONFIG_INITIAL_SCREEN_WIDTH;
	_struct->resolution.y = CONFIG_INITIAL_SCREEN_HEIGHT;
	_struct->fullscreen = CONFIG_INITIAL_FULLSCREEN;
	_struct->vsync = CONFIG_INITIAL_VSYNC;
	_struct->framerate = CONFIG_INITIAL_FRAMERATE;
	_struct->soundChannel = CONFIG_INITIAL_SOUND_CHANNEL;
	_struct->globalVolume = CONFIG_INITIAL_GLOBAL_VOLUME;
	_struct->effectVolume = CONFIG_INITIAL_EFFECT_VOLUME;
	_struct->voiceVolume = CONFIG_INITIAL_VOICE_VOLUME;
	_struct->musicVolume = CONFIG_INITIAL_MUSIC_VOLUME;
}

Configuration *Configuration_GetValues()
{
	Configuration *configuration = NULL;
	FILE *file = NULL;
	char gameSettingsFile[3000];

	// Allocate the structure settings
	configuration = calloc(1, sizeof(Configuration));
	// Get the setting file of the game
	Configuration_CreateGameFileDirectory(configuration);
	strcpy(gameSettingsFile, configuration->gameFilePath);
	strcat(gameSettingsFile, "/Settings.data");
	file = fopen(gameSettingsFile, "r");
	if (file != NULL)
	{
		GetConfiguration(file, configuration);
		fclose(file);
	}
	// Set the structure to default and create the file if no file or deprecated file was found
	if (file == NULL || strcmp(configuration->version, GAME_VERSION) != 0)
	{
		configuration->isInitial = sfTrue;
		file = fopen(gameSettingsFile, "w+");
		SetDefaultConfiguration(configuration);
		SetConfiguration(file, configuration);
		fclose(file);
	}
	// Calculate the scale
	configuration->scale.x = (float)configuration->resolution.x / SCREEN_WIDTH_USED_IN_SCALE;
	configuration->scale.y = (float)configuration->resolution.y / SCREEN_HEIGHT_USED_IN_SCALE;
	configuration->minScale = fminf(configuration->scale.x, configuration->scale.y);

	return configuration;
}

void Configuration_Save(Configuration *_struct)
{
	FILE *file = NULL;
	char path[3000];

	strcpy(path, _struct->gameFilePath);
	strcat(path, "/Settings.data");
	file = fopen(path, "w+");
	SetConfiguration(file, _struct);
	fclose(file);
}

void Configuration_RestoreToDefault(Configuration *_struct)
{
	SetDefaultConfiguration(_struct);
	Configuration_Save(_struct);
}

void Configuration_Destroy(Configuration *_struct)
{
	free(_struct);
}

sfVector2f Configuration_GetScale(Configuration *_struct)
{
	return _struct->scale;
}

sfRenderWindow *Configuration_CreateWindow(Configuration *_configuration)
{
	sfRenderWindow *window;
	sfVideoMode mode = { _configuration->resolution.x ,_configuration->resolution.y, 32 };

	// Creating the window
	if (_configuration->fullscreen >= 1)
	{
		window = sfRenderWindow_create(mode, GAME_NAME, sfClose | sfFullscreen, NULL);
	}
	else
	{
		window = sfRenderWindow_create(mode, GAME_NAME, sfClose, NULL);
	}
	// Setting the Vsync
	if (_configuration->vsync >= 1)
	{
		sfRenderWindow_setVerticalSyncEnabled(window, sfTrue);
	}
	// Setting the framerate
	sfWindow_setFramerateLimit(window, _configuration->framerate);

	return window;
}

float Configuration_GetVolume(Configuration *_configuration, SoundType _type)
{
	switch (_type)
	{
	default:
		return 100.0f;
		break;
	case SOUND_GLOBAL:
		return (float)_configuration->globalVolume;
		break;
	case SOUND_EFFECT:
		return (float)_configuration->effectVolume;
		break;
	case SOUND_VOICE:
		return (float)_configuration->voiceVolume;
		break;
	case SOUND_MUSIC:
		return (float)_configuration->musicVolume;
		break;
	}
}

void Configuration_CreateGameFileDirectory(Configuration *_configuration)
{
	struct stat st = { 0 };
	char gameUserDirectory[3000];
	strcpy(gameUserDirectory, getenv("USERPROFILE"));
	strcat(gameUserDirectory, "/Documents/");
	strcat(gameUserDirectory, MAIN_FOLDER);
	if (stat(gameUserDirectory, &st) == -1)
	{
		_mkdir(gameUserDirectory);
	}
	strcat(gameUserDirectory, "/");
	strcat(gameUserDirectory, GAME_NAME);
	if (stat(gameUserDirectory, &st) == -1)
	{
		_mkdir(gameUserDirectory);
	}
	_configuration->gameFilePath = _strdup(gameUserDirectory);
	printf("Create or open the user files directory : %s\n", _configuration->gameFilePath);
}