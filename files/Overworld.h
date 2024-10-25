#ifndef OVERWORLD__H
#define OVERWORLD__H

// -- INCLUDES ------------------------------------------------------------- //

#include "_MainData.h"
#include "Player.h"
#include "Difficulty.h"
#include "Progression.h"
#include "World.h"
#include "Fade.h"

// -- ENUMERATIONS --------------------------------------------------------- //

enum OverworldState
{
	SELECTION,
	LOBBY
};

enum WindowType
{
	LEAVE,
	SPACESHIP,
	COLOR,
	SPECIAL,
	READY
};

// -- DEFINES -------------------------------------------------------------- //

#define LEVELS_ROTATION_SPEED 1
#define SELECTION_INFO_NB 7
#define NB_SPRITE_SELECT 3
#define MOVE_CONTROLS_NB 5

// -- STRUCTURES ----------------------------------------------------------- //

struct OverworldLevel
{
	sfSprite* sprite;
	float x;
	float y;
	float angle;
	int toRight;
	int radius;
	int selected;
	char name[BUFFER_SMALL];
	int unlocked;
};

struct Selection
{
	sfSprite* cursor;
	sfSprite* info[SELECTION_INFO_NB];
	sfSprite* connect;
	sfSprite* selected;
	sfSprite* connected[MAX_PLAYER_NUMBER];
	float x;
	float y;
	float angle;
	float currentTime;
};

struct Lobby
{
	joystickId joystick;
	enum WindowType Window;
	int choiceSpaceship;
	int choiceColor;
	int choiceSpecial;
	int select_frame;
	float currentTime;
	int direction;
};

struct OverworldData
{
	sfSprite* background;
	sfSprite* select[NB_SPRITE_SELECT];
	sfSprite* moveControls[MOVE_CONTROLS_NB];
	sfSprite* path[NB_LEVELS - 1];
	sfSprite* difficulty[NB_DIF_MODE];
	Players outputPlayers;
	enum OverworldState state;
	enum OverWorldDifficulty choosenDifficulty;
	struct OverworldLevel Levels[NB_LEVELS];
	struct Selection selection;
	struct Lobby lobby[MAX_PLAYER_NUMBER];
	int nbPlayers;
	int toConnect;
	sfMusic *music;
	Text difficultyDisplay;
	sfVector2f pathPos[NB_LEVELS - 1];
	GOProgression Progression;
	Anim* levelName[NB_LEVELS];
	Entity levelEntity[NB_LEVELS];
};

// Data used to store all informations about the state

// -- TYPES DEFINITIONS ---------------------------------------------------- //

typedef struct OverworldData OverworldData;

// -- MAIN FUNCTION DEFINITIONS -------------------------------------------- //

void Overworld_Initialize(MainData *_data);
void Overworld_HandleEvent(MainData *_data, sfEvent _event);
void Overworld_Update(MainData *_data, float _dt);
void Overworld_Display(MainData *_data);
void Overworld_Exit(MainData *_data);

#endif // !MENU__H