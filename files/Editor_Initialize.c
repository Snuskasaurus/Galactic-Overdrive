#include "Editor.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

CarouselData Editor_CreateCarouselFromFolder(MainData *_data, const char *_folder);
CarouselData Editor_CreateCarouselDataEnemies(MainData *_data);
CarouselData Editor_CreateCarouselDataBoss(MainData *_data);
SliderData Editor_CreateSliderFromIntValues(int _value1, int _value2);
SliderData Editor_CreateSliderFromFloatValues(float _value1, float _value2);

// -- MAIN FUNCTION -------------------------------------------------------- //

void Editor_Initialize(MainData *_data)
{
	EditorData *state = NULL; // Create a shortcut to the state data
	Button buttonBlueprint[6];
	Carousel carouselBlueprint[1];
	Slider sliderBlueprint[1];
	sfVector2f scale;
	sfVector2f playerPosition = V2F(EDITOR_SCREEN_POSITION.x + EDITOR_SCREEN_WIDTH / 2.0f, EDITOR_SCREEN_POSITION.y + EDITOR_SCREEN_HEIGHT / 2.0f);

	// Allocate the state data to memory
	_data->stateData[EDITOR] = calloc(1, sizeof(EditorData));
	state = _data->stateData[EDITOR];
	// Create a shortcut for the scale
	scale.x = (float)_data->engine.configuration->scale.x;
	scale.y = (float)_data->engine.configuration->scale.y;
	// Initailize the name of the default level
	strcpy(state->level.name, "placeholder");
	// Intiailize the sprite for the background
	state->background = Sprite_CreateFromFile("Data/Editor/Background.png", 0.0f, 0.0f);
	// Initialize selectors
	state->parallaxSelector = Sprite_CreateFromFile("Data/Editor/selectorParallax.png", 0.0f, 0.0f);
	state->terrainSelector = Sprite_CreateFromFile("Data/Editor/selectorTerrain.png", 0.0f, 0.0f);
	// Initialize selectionMask
	state->selectionMask = sfSprite_create();
	// Initialize the blueprints for the buttons and the carousels
	buttonBlueprint[0] = Button_InitializeBlueprint(&_data->engine, "Data/Editor/Buttons/127x50_Button.data");
	buttonBlueprint[1] = Button_InitializeBlueprint(&_data->engine, "Data/Editor/Buttons/145x50_Button.data");
	buttonBlueprint[2] = Button_InitializeBlueprint(&_data->engine, "Data/Editor/Buttons/265x50_Button.data");
	buttonBlueprint[3] = Button_InitializeBlueprint(&_data->engine, "Data/Editor/Buttons/300x50_Button.data");
	buttonBlueprint[4] = Button_InitializeBlueprint(&_data->engine, "Data/Editor/Buttons/540x50_Button.data");
	buttonBlueprint[5] = Button_InitializeBlueprint(&_data->engine, "Data/Editor/Buttons/Back_To_Menu.data");
	carouselBlueprint[0] = Carousel_InitializeBlueprint(&_data->engine, "Data/Editor/Carousels/500x30_Carousel.data");
	sliderBlueprint[0] = Slider_InitializeBlueprint(&_data->engine, "Data/Editor/Sliders/460x20_Slider.data");
	// Initialize the main interface buttons
	Button_Create(&state->buttons, &buttonBlueprint[3], &Editor_RenameLevel, state->level.name, V2F(20.0f, 20.0f), scale); // Rename level button
	Button_Create(&state->buttons, &buttonBlueprint[1], &Editor_NewLevel, "NEW", V2F(20.0f, 80.0f), scale); // New level button
	Button_Create(&state->buttons, &buttonBlueprint[1], &Editor_SaveLevel, "SAVE", V2F(175.0f, 80.0f), scale); // Save level button
	Button_Create(&state->buttons, &buttonBlueprint[1], &Editor_LoadLevel, "LOAD", V2F(20.0f, 1010), scale); // Load level button
	Button_Create(&state->buttons, &buttonBlueprint[1], &Editor_DeleteLevel, "DELETE", V2F(175, 1010), scale); // Delete level button
	Button_Create(&state->buttons, &buttonBlueprint[4], &Editor_TestLevel, "TEST LEVEL", V2F(1360, 20), scale); // Test level button
	Button_Create(&state->buttons, &buttonBlueprint[4], &EditorFade_EditorToGame, "PLAY LEVEL", V2F(1360, 80), scale); // Play level button
	Button_Create(&state->buttons, &buttonBlueprint[0], &Editor_SelectLevelMode, "LEVEL", V2F(1360, 173), scale); // Select tool button
	Button_Create(&state->buttons, &buttonBlueprint[0], &Editor_SelectPathMode, "PATH", V2F(1497, 173), scale); // Move tool button
	Button_Create(&state->buttons, &buttonBlueprint[0], &Editor_SelectTerrainMode, "TERRAIN", V2F(1636, 173), scale); // Create tool button
	Button_Create(&state->buttons, &buttonBlueprint[0], &Editor_SelectParallaxMode, "PARALLAX", V2F(1773, 173), scale); // Delete tool button
	Button_Create(&state->buttons, &buttonBlueprint[0], &Editor_SelectSelectTool, "SELECT", V2F(1360, 266), scale); // Select tool button
	Button_Create(&state->buttons, &buttonBlueprint[0], &Editor_SelectMoveTool, "MOVE", V2F(1497, 266), scale); // Move tool button
	Button_Create(&state->buttons, &buttonBlueprint[0], &Editor_SelectCreateTool, "CREATE", V2F(1636, 266), scale); // Create tool button
	Button_Create(&state->buttons, &buttonBlueprint[0], &Editor_SelectDeleteTool, "DELETE", V2F(1773, 266), scale); // Delete tool button
	Button_Create(&state->buttons, &buttonBlueprint[5], &EditorFade_EditorToMenu, "RETURN TO MENU", V2F(1360, 1008), scale); // Return to menu button
	// Initialize interface for level parameters
	Carousel_Create(&state->parameters[TOOLMODE_LEVEL].carousels, &carouselBlueprint[0], "World", V2F(1380.0f, 372.0f), scale, Editor_CreateCarouselFromFolder(_data, "Data/Worlds/"));
	Slider_Create(&state->parameters[TOOLMODE_LEVEL].sliders, &sliderBlueprint[0], "Position in World", V2F(1400.0f, 440.0f), scale, Editor_CreateSliderFromIntValues(1, 3));
	Carousel_Create(&state->parameters[TOOLMODE_LEVEL].carousels, &carouselBlueprint[0], "Background", V2F(1380.0f, 508.0f), scale, Editor_CreateCarouselFromFolder(_data, "Data/Sprites/Backgrounds/"));
	Carousel_Create(&state->parameters[TOOLMODE_LEVEL].carousels, &carouselBlueprint[0], "Boss name", V2F(1380.0f, 576.0f), scale, Editor_CreateCarouselDataBoss(_data));
	Slider_Create(&state->parameters[TOOLMODE_LEVEL].sliders, &sliderBlueprint[0], "Boss Spawn", V2F(1400.0f, 644.0f), scale, Editor_CreateSliderFromFloatValues(0.0f, 360));
	Slider_Create(&state->parameters[TOOLMODE_LEVEL].sliders, &sliderBlueprint[0], "Is in space", V2F(1400.0f, 712.0f), scale, Editor_CreateSliderFromIntValues(0, 1));
	Slider_Create(&state->parameters[TOOLMODE_LEVEL].sliders, &sliderBlueprint[0], "Repeat terrains", V2F(1400.0f, 778.0f), scale, Editor_CreateSliderFromIntValues(0, 1));
	Carousel_Create(&state->parameters[TOOLMODE_LEVEL].carousels, &carouselBlueprint[0], "Music", V2F(1380.0f, 844.0f), scale, Editor_CreateCarouselFromFolder(_data, "Data/Musics/InGame/Music_Level"));
	// Initialize interface for waypoints parameters
	Carousel_Create(&state->parameters[TOOLMODE_PATH].carousels, &carouselBlueprint[0], "Enemies to spawn", V2F(1380.0f, 372.0f), scale, Editor_CreateCarouselDataEnemies(_data));
	Slider_Create(&state->parameters[TOOLMODE_PATH].sliders, &sliderBlueprint[0], "Number of enemies", V2F(1400.0f, 440.0f), scale, Editor_CreateSliderFromIntValues(SPAWN_ENEMY_NUMBER_MINIMUM, SPAWN_ENEMY_NUMBER_MAXIMUM));
	Slider_Create(&state->parameters[TOOLMODE_PATH].sliders, &sliderBlueprint[0], "Starting time", V2F(1400.0f, 508.0f), scale, Editor_CreateSliderFromFloatValues(0.0f, LEVEL_MAXIMUM_DURATION));
	Slider_Create(&state->parameters[TOOLMODE_PATH].sliders, &sliderBlueprint[0], "Duration", V2F(1400.0f, 576.0f), scale, Editor_CreateSliderFromFloatValues(SPAWN_DURATION_MINIMUM, SPAWN_DURATION_MAXIMUM));
	Slider_Create(&state->parameters[TOOLMODE_PATH].sliders, &sliderBlueprint[0], "Difficulty", V2F(1400.0f, 644.0f), scale, Editor_CreateSliderFromIntValues(1, 4));
	// Initialize interface for terrain parameters
	Carousel_Create(&state->parameters[TOOLMODE_TERRAIN].carousels, &carouselBlueprint[0], "Type", V2F(1380.0f, 372.0f), scale, Editor_CreateCarouselFromFolder(_data, "Data/Sprites/Terrains/"));
	// Initialize interface for parrallaxes parameters
	Carousel_Create(&state->parameters[TOOLMODE_PARALLAX].carousels, &carouselBlueprint[0], "Type", V2F(1380.0f, 372.0f), scale, Editor_CreateCarouselFromFolder(_data, "Data/Sprites/Parallaxes/"));
	Slider_Create(&state->parameters[TOOLMODE_PARALLAX].sliders, &sliderBlueprint[0], "Time", V2F(1400.0f, 440.0f), scale, Editor_CreateSliderFromFloatValues(0.0f, 360.0f));
	Slider_Create(&state->parameters[TOOLMODE_PARALLAX].sliders, &sliderBlueprint[0], "Speed", V2F(1400.0f, 508.0f), scale, Editor_CreateSliderFromFloatValues(10.0f, 500.0f));
	Slider_Create(&state->parameters[TOOLMODE_PARALLAX].sliders, &sliderBlueprint[0], "Depth", V2F(1400.0f, 576.0f), scale, Editor_CreateSliderFromIntValues(-100, 100));
	Slider_Create(&state->parameters[TOOLMODE_PARALLAX].sliders, &sliderBlueprint[0], "Offset X", V2F(1400.0f, 644.0f), scale, Editor_CreateSliderFromIntValues(-EDITOR_SCREEN_WIDTH, EDITOR_SCREEN_WIDTH));
	Slider_Create(&state->parameters[TOOLMODE_PARALLAX].sliders, &sliderBlueprint[0], "Angle", V2F(1400.0f, 712.0f), scale, Editor_CreateSliderFromFloatValues(-180.0f, 180.0f));
	// Initialize the colliders
	state->mapEditorCollider.position = EDITOR_MAP_POSITION;
	state->mapEditorCollider.size = EDITOR_MAP_SIZE;
	state->mapEditorCollider.centered = FALSE;
	// Initialize various variables
	state->level.paths = Path_Initialize();
	// Inititalize the Entities
	state->enemies = Enemy_Initialize();
	state->lasers = Laser_InitLinkedList();
	// Spawn the player
	Player_Create(&_data->engine, &state->players, playerPosition, 0, "Interceptor");
	// Various initializations
	state->levelList = Level_GetNamesInFolder();
	state->parametersText.size = 30;
	state->parametersText.font = Text_GetFont(_data->engine.text, "Roboto-Regular.ttf");
	state->parametersText.color = sfWhite;
	// Init "Maps" struct variables
	state->level.map.nbTerrains = 0;
	state->level.map.nbOfBckgrndParallax = 0;
	state->level.map.nbOfForgrndParallax = 0;
	// Each tabParallax can contain up to 100 parallax
	state->level.map.tabParallaxBckgrnd = calloc(EDITOR_PARALLAX_MAX, sizeof(MapParallax));
	state->level.map.tabParallaxForgrnd = calloc(EDITOR_PARALLAX_MAX, sizeof(MapParallax));
	state->level.map.terrains = NULL;
	state->level.map.scrollingSpeed = 200;
	InitParallaxList(_data);
	// Init Terrain Repetition
	state->nbTerrainRepetition = 0;
	state->nbMaxTerrainRepetition = EDITOR_INIT_MAX_TERRAIN_REPETITION;
	state->tabTerrainRepetition = calloc(state->nbMaxTerrainRepetition, sizeof(Terrain));
	
	if (_data->isLaunchedFromEditor)
	{
		strcpy(state->level.name, _data->levelName);
		Button_SetText(&_data->engine, state->buttons.array[0], state->level.name);
		Editor_LoadLevel(_data);
		_data->isLaunchedFromEditor = FALSE;
	}
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //

CarouselData Editor_CreateCarouselFromFolder(MainData *_data, const char *_folder)
{
	CarouselData carouselData;
	// Get background
	ExploDir dir = ExploreDirectory(_folder);
	// Get numbers of elements
	carouselData.current = 0;
	carouselData.number = dir.nbrFiles;
	// Get the names of the elements
	carouselData.elements = NULL;
	// Get structures of the elements
	carouselData.names = calloc(carouselData.number, sizeof(char*));
	for (int i = 0; i < carouselData.number; i++)
	{
		carouselData.names[i] = _strdup(dir.fileNames[i]);
	}
	return carouselData;
}

CarouselData Editor_CreateCarouselDataBoss(MainData *_data)
{
	CarouselData carouselData;
	// Get numbers of elements
	carouselData.current = 0;
	carouselData.number = _data->engine.blueprints.boss.number;
	// Get the names of the elements
	carouselData.elements = _data->engine.blueprints.boss.array;
	// Get structures of the elements
	carouselData.names = calloc(carouselData.number, sizeof(char*));
	for (int i = 0; i < carouselData.number; i++)
	{
		carouselData.names[i] = _strdup(_data->engine.blueprints.boss.array[i]->name);
	}
	return carouselData;
}

CarouselData Editor_CreateCarouselDataEnemies(MainData *_data)
{
	CarouselData carouselData;
	// Get numbers of elements
	carouselData.current = 0;
	carouselData.number = _data->engine.blueprints.enemies.number;
	// Get the names of the elements
	carouselData.elements = _data->engine.blueprints.enemies.array;
	// Get structures of the elements
	carouselData.names = calloc(carouselData.number, sizeof(char*));
	for (int i = 0; i < carouselData.number; i++)
	{
		carouselData.names[i] = _strdup(_data->engine.blueprints.enemies.array[i]->name);
	}
	return carouselData;
}

SliderData Editor_CreateSliderFromIntValues(int _value1, int _value2)
{
	SliderData data;
	data.type = TYPE_INT;
	data.ints[SLIDER_VALUE_MINIMUM] = _value1;
	data.ints[SLIDER_VALUE_CURRENT] = (_value2 - _value1) / 2;
	data.ints[SLIDER_VALUE_MAXIMUM] = _value2;
	return data;
}

SliderData Editor_CreateSliderFromFloatValues(float _value1, float _value2)
{
	SliderData data;
	data.type = TYPE_FLOAT;
	data.floats[SLIDER_VALUE_MINIMUM] = _value1;
	data.floats[SLIDER_VALUE_CURRENT] = (_value2 - _value1) / 2.0f;
	data.floats[SLIDER_VALUE_MAXIMUM] = _value2;
	return data;
}