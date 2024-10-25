#include "Overworld.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

// -- MAIN FUNCTION -------------------------------------------------------- //

void Overworld_Initialize(MainData *_data)
{
	sfVector2i tempResolution = _data->engine.configuration->resolution;
	_data->stateData[OVERWORLD] = calloc(1, sizeof(OverworldData));
	OverworldData *state = _data->stateData[OVERWORLD];
	sfVector2f tempScale = _data->engine.configuration->scale;
	float tempLevelX[NB_LEVELS] = { 540.f, 1200.f, 1740.f, 180.f, 840.f };
	float tempLevelY[NB_LEVELS] = { 720.f, 930.f, 480.f, 300.f, 180.f };
	float tempPathX[NB_LEVELS - 1] = { 360.f, 870.f, 1470.f, 510.f };
	float tempPathY[NB_LEVELS - 1] = { 510.f, 840.f, 750.f, 270.f };
	float tempNameX[NB_LEVELS] = { 690.f, 1290.f, 1830.f, 360.f, 1020.f };
	float tempNameY[NB_LEVELS] = { 600.f, 810.f, 330.f, 180.f, 90.f };
	float tempToRight[NB_LEVELS] = { sfFalse, sfTrue, sfFalse, sfTrue, sfFalse };
	float tempRadius[NB_LEVELS] = { 96.f, 81.f, 111.f, 96.f, 96.f };
	int tempUnlocked[NB_LEVELS] = { sfFalse, sfFalse, sfFalse, sfTrue, sfTrue };
	char filePath[BUFFER_SMALL] = { 0 };
	char* levelSpriteName[NB_LEVELS] = { "Terra", "Space", "Sheth", "Simulation_Custom", "Simulation_Custom" };
	char* pathSpriteName[NB_LEVELS - 1] = { "Simulation_to_earth", "Terra_to_Space", "Space_to_alien", "Simulation_to_Custom" };
	char* tempAnim[NB_LEVELS] = { "Name_Terre", "Name_Space", "Name_Sheth", "Name_Simulation", "Name_Custom" };
	char* tempName[NB_LEVELS] = { "HumanPlanet.dat", "Space.dat", "AlienPlanet.dat", "Tutorial.dat", "Simulation.dat" };
	state->state = SELECTION;

	for (int i = 0; i < NB_LEVELS; i++)
	{
		sprintf(filePath, "Data/Sprites/Overworld/World/%s.png", levelSpriteName[i]);
		state->Levels[i].sprite = Sprite_CreateFromFile(filePath, 0.5f, 0.5f);
		state->Levels[i].x = tempLevelX[i] * tempScale.x;
		state->Levels[i].y = tempLevelY[i] * tempScale.y;
		state->Levels[i].angle = 0;
		state->Levels[i].toRight = (int)tempToRight[i];
		state->Levels[i].radius = (int)tempRadius[i];
		state->Levels[i].selected = sfFalse;
		state->Levels[i].unlocked = tempUnlocked[i];
		strcpy(state->Levels[i].name, tempName[i]);
	}

	for (int i = 0; i < NB_LEVELS - 1; i++)
	{
		sprintf(filePath, "Data/Sprites/Overworld/Path/%s.png", pathSpriteName[i]);
		state->path[i] = Sprite_CreateFromFile(filePath, 0.5f, 0.5f);
		state->pathPos[i].x = tempPathX[i] * tempScale.x;
		state->pathPos[i].y = tempPathY[i] * tempScale.y;
	}

	state->selection.x = (float)(tempResolution.x / 2);
	state->selection.y = (float)(tempResolution.y / 2);
	state->selection.angle = -90;
	state->selection.currentTime = 0;

	state->nbPlayers = 0;
	state->toConnect = 0;
	for (int i = 0; i < MAX_PLAYER_NUMBER; i++)
	{
		state->lobby[i].joystick = -1;
		state->lobby[i].Window = SPACESHIP;
		state->lobby[i].choiceSpaceship = 0;
		state->lobby[i].choiceColor = 0;
		state->lobby[i].choiceSpecial = 0;
		state->lobby[i].select_frame = 0;
		state->lobby[i].currentTime = 0;
		if (sfJoystick_isConnected(i))
		{
			state->toConnect++;
		}
	}
	if (state->toConnect < MAX_PLAYER_NUMBER)
	{
		state->toConnect++;
	}

	state->selection.cursor = Sprite_CreateFromFile("Data/Sprites/vaisseau.png", 0.5f, 0.5f);
	state->selection.info[0] = Sprite_CreateFromFile("Data/Sprites/info/window1.png", 0.5f, 0.5f);
	state->selection.info[1] = Sprite_CreateFromFile("Data/Sprites/info/window2.png", 0.5f, 0.5f);
	state->selection.info[2] = Sprite_CreateFromFile("Data/Sprites/info/window3.png", 0.5f, 0.5f);
	state->selection.info[3] = Sprite_CreateFromFile("Data/Sprites/info/window4.png", 0.5f, 0.5f);
	state->selection.info[4] = Sprite_CreateFromFile("Data/Sprites/info/window5.png", 0.5f, 0.5f);
	state->selection.info[5] = Sprite_CreateFromFile("Data/Sprites/info/window6.png", 0.5f, 0.5f);
	state->selection.info[6] = Sprite_CreateFromFile("Data/Sprites/info/window7.png", 0.5f, 0.5f);
	state->selection.connect = Sprite_CreateFromFile("Data/Sprites/connexion/connect.png", 0.5f, 0.5f);
	state->selection.selected = Sprite_CreateFromFile("Data/Sprites/inputs/B down 1.png", 0.5f, 0.5f);
	state->selection.connected[0] = Sprite_CreateFromFile("Data/Sprites/connexion/Player1.png", 0.5f, 0.5f);
	state->selection.connected[1] = Sprite_CreateFromFile("Data/Sprites/connexion/Player2.png", 0.5f, 0.5f);
	state->selection.connected[2] = Sprite_CreateFromFile("Data/Sprites/connexion/Player3.png", 0.5f, 0.5f);
	state->selection.connected[3] = Sprite_CreateFromFile("Data/Sprites/connexion/Player4.png", 0.5f, 0.5f);
	state->background = Sprite_CreateFromFile("Data/Sprites/Backgrounds/Background_OverWorld.png", 0, 0);
	state->select[0] = Sprite_CreateFromFile("Data/Sprites/select/Idle.png", 0.5f, 0.5f);
	state->select[1] = Sprite_CreateFromFile("Data/Sprites/select/fleche gauche.png", 0.5f, 0.5f);
	state->select[2] = Sprite_CreateFromFile("Data/Sprites/select/fleche droite.png", 0.5f, 0.5f);
	state->moveControls[0] = Sprite_CreateFromFile("Data/Sprites/inputs/left_stick/SL static.png", 0.5f, 0.5f);
	state->moveControls[1] = Sprite_CreateFromFile("Data/Sprites/inputs/left_stick/SL left static.png", 0.5f, 0.5f);
	state->moveControls[2] = Sprite_CreateFromFile("Data/Sprites/inputs/left_stick/SL right static.png", 0.5f, 0.5f);
	state->moveControls[3] = Sprite_CreateFromFile("Data/Sprites/inputs/left_stick/SL up static.png", 0.5f, 0.5f);
	state->moveControls[4] = Sprite_CreateFromFile("Data/Sprites/inputs/left_stick/SL down static.png", 0.5f, 0.5f);
	state->difficulty[0] = Sprite_CreateFromFile("Data/Sprites/Overworld/Rank/Recruit.png", 0.5f, 0.5f);
	state->difficulty[1] = Sprite_CreateFromFile("Data/Sprites/Overworld/Rank/2nd_class.png", 0.5f, 0.5f);
	state->difficulty[2] = Sprite_CreateFromFile("Data/Sprites/Overworld/Rank/COMMANDER.png", 0.5f, 0.5f);

	// Initialize spaceship colors from the file
	if (!_data->spaceShipColors || _data->spaceShipColorsNames)
	{
		FILE *colorFile = fopen("Data/SpaceshipColor.data", "rt");
		_data->nbColors = 0;
		if (colorFile)
		{
			fscanf(colorFile, "NB_COLOR %d\n", &_data->nbColors);
			_data->spaceShipColors = calloc(_data->nbColors, sizeof(sfColor));
			_data->spaceShipColorsNames = calloc(_data->nbColors, sizeof(char*));
			for (int i = 0; i < _data->nbColors; i++)
			{
				char name[BUFFER_MEDIUM];
				int red = 0;
				int green = 0;
				int blue = 0;
				fscanf(colorFile, "%s %d %d %d\n", name, &red, &green, &blue);
				_data->spaceShipColors[i] = sfColor_fromRGB(red, green, blue);
				_data->spaceShipColorsNames[i] = _strdup(name);
				///printf("name of the color : %s\n", _data->spaceShipColorsNames[i]);
			}
			fclose(colorFile);
		}
		else
		{
			printf("Failed to open spaceshipColor file\n");
		}
	}

	// Music
	state->music = sfMusic_createFromFile("Data/Musics/Menu/Simon Bichbihler - In the 1980s.wav");
	sfMusic_play(state->music);
	sfMusic_setLoop(state->music, sfTrue);
	sfMusic_setVolume(state->music, (float)_data->engine.configuration->musicVolume);

	// Text Difficulty
	state->choosenDifficulty = NORMAL;
	state->difficultyDisplay.size = (int)(30.f * tempScale.x);
	state->difficultyDisplay.font = Text_GetFont(_data->engine.text, "AXIS Extra Bold 800.otf");
	state->difficultyDisplay.color = sfWhite;

	LoadProgression(&state->Progression, _data);
	for (int i = 0; i < state->Progression.progress; i++)
	{
		state->Levels[i].unlocked = sfTrue;
	}

	for (int i = 0; i < NB_LEVELS; i++)
	{
		state->levelName[i] = GetAnim(_data->engine.anims, "OW_Names", tempAnim[i]);
		state->levelEntity[i].position.x = tempNameX[i] * tempScale.x;
		state->levelEntity[i].position.y = tempNameY[i] * tempScale.y;
		state->levelEntity[i].angle = 0;
		InitEntityAnim(&state->levelEntity[i], state->levelName[i]->maxTime);
		ResetAnim(&state->levelEntity[i]);
	}
}


// -- LOCAL FUNCTIONS ------------------------------------------------------ //