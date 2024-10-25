#include "_MainData.h"
#include "Splash.h"
#include "Menu.h"
#include "Game.h"
#include "Credit.h"
#include "Overworld.h"
#include "Editor.h"
#include "Shop.h"
#include "Fade.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

MainData Main_InitializeMainData();
void Main_InitializeGameStatesFunctions(MainData *_data);
void Main_ManageGameStates(MainData *_data);

// -- MAIN FUNCTION -------------------------------------------------------- //

#ifdef DEBUG
int main()
#else // !DEBUG
int WinMain()
#endif // DEBUG
{
	// Initialize the seed for randomization
	srand((unsigned int)time(NULL));
	// Loading the main data
	MainData data = Main_InitializeMainData();
	// The game start to run;
	do
	{
		Main_ManageGameStates(&data);
	} while (sfRenderWindow_isOpen(data.engine.window));
	return EXIT_SUCCESS;
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //

MainData Main_InitializeMainData()
{
	MainData data = {0};

	// Initialize the configuration of the game
	data.engine.configuration = Configuration_GetValues();
	// Create the window from the settings
	data.engine.window = Configuration_CreateWindow(data.engine.configuration);
	sfImage* icon = sfImage_createFromFile("Data/Sprites/icon.png");
	sfRenderWindow_setIcon(data.engine.window, 32, 32, sfImage_getPixelsPtr(icon));
	// Create a texture for post-processing purpose
	data.engine.canvas = sfRenderTexture_create(data.engine.configuration->resolution.x, data.engine.configuration->resolution.y, 32);
	sfRenderTexture_clear(data.engine.canvas, sfTransparent);
	// Initialize the sound engine and set the volumes
	data.engine.sound = Sound_InitializeEngine(data.engine.configuration->soundChannel);
	for (SoundType soundType = 0; soundType < SOUND_TYPES_NB; soundType++)
	{
		float volume = Configuration_GetVolume(data.engine.configuration, soundType);
		Sound_SetVolume(data.engine.sound, soundType, volume);
	}
	// Initalize the text engine
	data.engine.text = Text_InitializeEngine();
	// Set the initial state
	data.currentState = INITIAL_GAMESTATE;
	// Initialize the functions of the game states
	Main_InitializeGameStatesFunctions(&data);
	// Create the clock used to calculate dt
	data.engine.clock = sfClock_create();
	// Load achievements BP
	data.engine.blueprints.achievements = Achievements_LoadBlueprints(&data);
	// Init Notifications
	Notification_Init(&data.notifications);
	// Load achievements from save file
	Achievement_Load(&data.engine, ACHIEVEMENTS_FILENAME);
	// Init fade
	Fade_Init(&data);
	return data;
}

void Main_InitializeGameStatesFunctions(MainData *_data)
{
	// Splash state
	_data->statesFunctions.Initialize[SPLASH] = Splash_Initialize;
	_data->statesFunctions.HandleEvent[SPLASH] = Splash_HandleEvent;
	_data->statesFunctions.Update[SPLASH] = Splash_Update;
	_data->statesFunctions.Display[SPLASH] = Splash_Display;
	_data->statesFunctions.Exit[SPLASH] = Splash_Exit;
	// Menu state
	_data->statesFunctions.Initialize[MENU] = Menu_Initialize;
	_data->statesFunctions.HandleEvent[MENU] = Menu_HandleEvent;
	_data->statesFunctions.Update[MENU] = Menu_Update;
	_data->statesFunctions.Display[MENU] = Menu_Display;
	_data->statesFunctions.Exit[MENU] = Menu_Exit;
	// Game state
	_data->statesFunctions.Initialize[GAME] = Game_Initialize;
	_data->statesFunctions.HandleEvent[GAME] = Game_HandleEvent;
	_data->statesFunctions.Update[GAME] = Game_Update;
	_data->statesFunctions.Display[GAME] = Game_Display;
	_data->statesFunctions.Exit[GAME] = Game_Exit;
	// Credit state
	_data->statesFunctions.Initialize[CREDIT] = Credit_Initialize;
	_data->statesFunctions.HandleEvent[CREDIT] = Credit_HandleEvent;
	_data->statesFunctions.Update[CREDIT] = Credit_Update;
	_data->statesFunctions.Display[CREDIT] = Credit_Display;
	_data->statesFunctions.Exit[CREDIT] = Credit_Exit;
	// Overworld state
	_data->statesFunctions.Initialize[OVERWORLD] = Overworld_Initialize;
	_data->statesFunctions.HandleEvent[OVERWORLD] = Overworld_HandleEvent;
	_data->statesFunctions.Update[OVERWORLD] = Overworld_Update;
	_data->statesFunctions.Display[OVERWORLD] = Overworld_Display;
	_data->statesFunctions.Exit[OVERWORLD] = Overworld_Exit;
	// Overworld state
	_data->statesFunctions.Initialize[EDITOR] = Editor_Initialize;
	_data->statesFunctions.HandleEvent[EDITOR] = Editor_HandleEvent;
	_data->statesFunctions.Update[EDITOR] = Editor_Update;
	_data->statesFunctions.Display[EDITOR] = Editor_Display;
	_data->statesFunctions.Exit[EDITOR] = Editor_Exit;
	// Shop state
	_data->statesFunctions.Initialize[SHOP] = Shop_Initialize;
	_data->statesFunctions.HandleEvent[SHOP] = Shop_HandleEvent;
	_data->statesFunctions.Update[SHOP] = Shop_Update;
	_data->statesFunctions.Display[SHOP] = Shop_Display;
	_data->statesFunctions.Exit[SHOP] = Shop_Exit;
}

// A function used to handle the events used by all the states
void Main_HandleUniversalEvent(MainData *_data, sfEvent _event)
{
	if (_event.type == sfEvtClosed)
	{
		Sound_StopAll(_data->engine.sound);
		sfWindow_close(_data->engine.window);
	}
}

void Main_ManageGameStates(MainData *_data)
{
	// Shortcut to the functions
	GameStatesFunctions *state = &_data->statesFunctions;
	// The previous state initialized
	GameState previousState = _data->currentState;
	sfEvent event;

	// Initialize the game state
	sfJoystick_update();
	state->Initialize[_data->currentState](_data);
	sfClock_restart(_data->engine.clock);
	do
	{
		// Handle the game state events
		if (_data->currentState == previousState)
		{
			while (sfRenderWindow_pollEvent(_data->engine.window, &event))
			{
				Main_HandleUniversalEvent(_data, event);
				state->HandleEvent[_data->currentState](_data, event);
			}
		}
		// Update the game state
		if (_data->currentState == previousState)
		{
			_data->engine.dt = sfTime_asSeconds(sfClock_getElapsedTime(_data->engine.clock));
			sfJoystick_update();
			state->Update[_data->currentState](_data, _data->engine.dt);

			Achievement_UpdateToUnlock(_data);
			Fade_Update(_data);
			Notification_Update(&_data->notifications, &_data->engine);
			///printf("FPS : %f\n", 1.0f / dt);
			sfClock_restart(_data->engine.clock);
		}
		// Display the game state
		if (_data->currentState == previousState)
		{
			sfRenderWindow_clear(_data->engine.window, sfBlack);
			Texture_DrawRenderTexture(_data->engine.window, _data->engine.canvas, NULL);
			state->Display[_data->currentState](_data);
			Fade_Display(_data);
			Notification_Display(&_data->notifications,&_data->engine);
			sfRenderWindow_display(_data->engine.window);
		}
	} while (_data->currentState == previousState && sfRenderWindow_isOpen(_data->engine.window));
	// Exit the game state
	state->Exit[previousState](_data);
}