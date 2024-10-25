#include "Game.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //
void Game_LoadSprites(MainData* _data);
void Game_InitIntro(MainData* _data);
void Game_InitOutro(MainData* _data);
void Game_RescaleWaypoints(MainData* _data);

// -- MAIN FUNCTION -------------------------------------------------------- //

void Game_Initialize(MainData *_data)
{
	// Create a shortcut to the state data
	GameData *state = _data->stateData[GAME];
	sfVector2i tempResolution = _data->engine.configuration->resolution;

	if (_data->stateData[GAME] == NULL)
	{
		// Allocate the state data to memory
		_data->stateData[GAME] = calloc(1, sizeof(GameData));
		state = _data->stateData[GAME];
	}

	// Inititalize the Entities
	state->enemies = Enemy_Initialize();
	state->lasers = Laser_InitLinkedList();
	// Initialize various variables
	state->screenCollider = Collider_CreateRectangleCollider(V2F(0.0f, 0.0f), V2F_FROM_V2I(_data->engine.configuration->resolution), FALSE);
	// Initialize the defeat screen
	state->defeatScreen = GetAnim(_data->engine.anims, "DefeatScreen", "DefeatScreen");
	InitEntityAnim(&state->defeatSreenEntity, state->defeatScreen->maxTime);
	state->defeatSreenEntity.position = V2F(tempResolution.x / 2.0f, tempResolution.y / 2.0f);
	// Load the map
	if (_data->isLaunchedFromEditor)
	{
		state->level = Level_Load(_data->levelName, sfFalse, _data);
	}
	else
	{
		state->level = Level_Load(_data->engine.world.levelNames.array[_data->engine.world.currentLevel], sfFalse, _data);
		state->levelIndex = _data->engine.world.currentLevel;
	}
	// Create boss
	state->boss = Boss_Create(&_data->engine, state->level.bossName);

	// Init AnimatedStarsBackground
	StarsBackground_Init(&state->starBackground, &_data->engine);

	// Create players
	if (_data->isLaunchedFromEditor)
	{
		Player_Create(&_data->engine, &state->players, V2F_FROM_VALUE(0.0f), 4, "Interceptor");
	}
	else
	{
		if (_data->stateData[SHOP] == NULL)
		{
			// If SHOP state doesn't have info to give
			state->players = _data->customizedPlayers;
			state->squadScore = 0;
			for (int i = 0; i < state->players.number; i++)
			{
				state->players.array[i]->score = 0;
			}
		}
		else
		{
			GameData *shopState = _data->stateData[SHOP];
			state->players = shopState->players;

		}
		for (int i = 0; i < state->players.number; i++)
		{
			state->players.array[i]->color = _data->spaceShipColors[_data->choosenColors[i]];
		}
	}

	Game_LoadSprites(_data);

	// Init money list to NULL
	state->moneyBills = NULL;

	// Init money counter to 0
	state->collectedBills = 0;

	state->parametersText.size = 30 * tempResolution.x / 1920;
	state->parametersText.font = Text_GetFont(_data->engine.text, "AXIS Extra Bold 800.otf");
	state->parametersText.color = sfWhite;

	_data->engine.time = 0.f;
	state->hasClearedEverything = sfFalse;

	// MUSIC
	char musicFilePath[BUFFER_SMALL] = GAME_MUSIC_LEVEL_FILEPATH;
	strcat(musicFilePath, state->level.musicName);
	state->music = sfMusic_createFromFile(musicFilePath);
	if (state->music == NULL)
	{
		state->music = sfMusic_createFromFile("Data/Musics/InGame/music_stage.wav");
	}
	sfMusic_play(state->music);
	sfMusic_setLoop(state->music, sfTrue);
	sfMusic_setVolume(state->music, (float)_data->engine.configuration->musicVolume);

	Game_RescaleWaypoints(_data);

	Game_InitIntro(_data);
	Game_InitOutro(_data);

	// Achievements
	Achievement_GameInit(_data);

	state->hasInterfaceDisplayed = TRUE;
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //

void Game_LoadSprites(MainData* _data)
{
	GameData *state = _data->stateData[GAME];

	state->bill[MONEY_100] = Sprite_CreateFromFile("Data/Sprites/Money/Coin 100.png", 0.5f, 0.5f);
	state->bill[MONEY_500] = Sprite_CreateFromFile("Data/Sprites/Money/Coin 500.png", 0.5f, 0.5f);
	state->bill[MONEY_1000] = Sprite_CreateFromFile("Data/Sprites/Money/Coin 1000.png", 0.5f, 0.5f);

	state->HUD.HUD = Sprite_CreateFromFile("Data/Sprites/Ath/ath empty.png", 0.5f, 0.5f);
	state->HUD.shield = Sprite_CreateFromFile("Data/Sprites/Ath/shield/Ath Shield.png", 0.5f, 0.5f);
	state->HUD.armorBegin = Sprite_CreateFromFile("Data/Sprites/Ath/vie/AthhealthStart.png", 0.5f, 0.5f);
	state->HUD.armorBody = Sprite_CreateFromFile("Data/Sprites/Ath/vie/Athhealthbody.png", 0.5f, 0.5f);
	state->HUD.armorEnd = Sprite_CreateFromFile("Data/Sprites/Ath/vie/AthhealthEnd.png", 0.5f, 0.5f);
	state->HUD.energyBegin = Sprite_CreateFromFile("Data/Sprites/Ath/ulti/AthUltStart.png", 0.5f, 0.5f);
	state->HUD.energyBody = Sprite_CreateFromFile("Data/Sprites/Ath/ulti/AthUltBody.png", 0.5f, 0.5f);
	state->HUD.energyEnd = Sprite_CreateFromFile("Data/Sprites/Ath/ulti/AthUltEnd.png", 0.5f, 0.5f);
	state->HUD.bossHUD = Sprite_CreateFromFile("Data/Sprites/Ath/Boss/AthBossEmpty.png", 0.5f, 0.5f);
	state->HUD.bossArmor = Sprite_CreateFromFile("Data/Sprites/Ath/Boss/AthBossHealth.png", 0.5f, 0.5f);

	state->intro = Sprite_CreateFromFile("Data/Sprites/vaisseaux_spawn.png", 0.5f, 0.5f);

	state->scoreboard = Sprite_CreateFromFile("Data/Sprites/info/window7.png", 0.5f, 0.5f);
}

void Game_RescaleWaypoints(MainData* _data)
{
	GameData *state = _data->stateData[GAME];
	sfVector2f tempScale = _data->engine.configuration->scale;
	for (int i = 0; i < state->level.paths.number; i++)
	{
		Waypoint* TempWaypoint = state->level.paths.array[i]->first;
		while (TempWaypoint)
		{
			TempWaypoint->position.x *= tempScale.x;
			TempWaypoint->position.y *= tempScale.y;
			TempWaypoint = TempWaypoint->next;
		}
	}
}

void Game_InitIntro(MainData* _data)
{
	GameData *state = _data->stateData[GAME];
	sfVector2f playerSpawn = { _data->engine.configuration->resolution.x / 2.0f, _data->engine.configuration->resolution.y / 2.0f };
	sfVector2i tempResolution = _data->engine.configuration->resolution;
	sfVector2f tempScale = _data->engine.configuration->scale;
	for (int i = 0; i < state->players.number; i++)
	{
		state->players.array[i]->entity.position = playerSpawn;
		state->players.array[i]->entity.position.x += i * 32 * tempScale.x;
		state->players.array[i]->entity.position.y += i * 32 * tempScale.y;
		state->players.array[i]->invulnerability.currentTime = state->players.array[i]->invulnerability.maxTime;
	}
	state->level.map.time = -INTRO_DURATION;
	state->introSpawn = (sfVector2f) { (float)(tempResolution.x / 2), (float)(tempResolution.y / 2) };
}

void Game_InitOutro(MainData* _data)
{
	GameData *state = _data->stateData[GAME];
	sfVector2i tempResolution = _data->engine.configuration->resolution;
	state->outroCooldown = 0;
	state->scoreboardPos.x = (float)(-tempResolution.x / 2);
	state->scoreboardPos.y = (float)(tempResolution.y / 2);
}