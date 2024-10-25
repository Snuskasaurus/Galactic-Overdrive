#include "Splash.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

void InitStatesToNull(MainData *_data);
void Splash_InitializeBlueprints(MainData *_data);

// -- MAIN FUNCTION -------------------------------------------------------- //

void Splash_Initialize(MainData *_data)
{
	InitStatesToNull(_data);
	// Allocate the state data to memory
	_data->stateData[SPLASH] = calloc(1, sizeof(SplashData));
	// Create a shortcut to the state data
	SplashData *state = _data->stateData[SPLASH];
	Splash_InitializeBlueprints(_data);
	state->logoPixelMatter = Sprite_CreateFromFile("Data/Sprites/Splash/logo_pixelmatter.png", 0.5f, 0.5f);
	state->logoCreajeux = Sprite_CreateFromFile("Data/Sprites/Splash/logo_creajeux.png", 0.5f, 0.5f);
	state->imgController = Sprite_CreateFromFile("Data/Sprites/Splash/Img_Controller.PNG", 0.5f, 0.5f);
	
	state->currentTime = 0;
	state->state = APPEAR_LOGO1;
#ifdef DEBUG
	state->state = END_SPLASH;
#endif // DEBUG
	state->alpha = 0;
#ifdef DEBUG
	_data->currentState = MENU;
#endif
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //

void InitStatesToNull(MainData *_data)
{
	for (int i = 0; i < NB_GAMESTATE; i++)
	{
		_data->stateData[i] = NULL;
	}
}

void Splash_InitializeBlueprints(MainData *_data)
{
	_data->engine.gameScale = V2F(SCALE_SPRITE, SCALE_SPRITE);
	_data->engine.anims = LoadAnimations("Data/AnimationsData");
	_data->engine.blueprints.weapons = Weapon_LoadBlueprints(_data->engine.anims);
	_data->engine.blueprints.turrets = Turret_LoadBlueprints();
	_data->engine.blueprints.enemies = EnemyLoader(&_data->engine);
	_data->engine.skills = Skill_Load(&_data->engine);
	_data->engine.blueprints.players = Player_LoadBlueprints(&_data->engine);
	_data->engine.blueprints.explosions = Explosion_InitializeBlueprints(&_data->engine);
	_data->engine.blueprints.boss = Boss_InitializeBlueprints(&_data->engine);
	_data->engine.blueprints.difficultyBlueprint = DifficultyData_Load();
	_data->engine.world = World_Init(&_data->engine, "Space.dat");
}