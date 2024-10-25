#ifndef EDITOR__H
#define EDITOR__H

// -- INCLUDES ------------------------------------------------------------- //

#include "_MainData.h"

// Entities
#include "Level.h"
#include "Player.h"
#include "Enemy.h"
#include "Projectiles.h"
#include "Path.h"
#include "World.h"

// Interfaces
#include "Button.h"
#include "Carousel.h"
#include "Slider.h"
#include "Map.h"
#include "List.h"
#include "Fade.h"

// Mechanics
#include "SpawnEnemies.h"
#include "Difficulty.h"

// -- ENUMERATIONS --------------------------------------------------------- //

enum SLIDER_PARALLAX
{
	PARALLAX_TIME_TO_UPDATE,
	PARALLAX_SPEED,
	PARALLAX_DEPTH,
	PARALLAX_OFFSET_X,
	PARALLAX_ANGLE,
};

// -- DEFINES -------------------------------------------------------------- //

#define EDITOR_MAP_BACKGROUND_POSITION 456.0f
#define EDITOR_MAP_SCALE 1.2f
#define EDITOR_SLECTION_MASK_ALPHA_MIN 50
#define EDITOR_SLECTION_MASK_ALPHA_MAX 125

// -- STRUCTURES ----------------------------------------------------------- //

typedef struct Parameters Parameters;
struct Parameters
{
	Carousels carousels;
	Buttons buttons;
	Sliders sliders;
};

// Data used to store all informations about the state
typedef struct EditorData EditorData;
struct EditorData
{
	char levelName[BUFFER_MEDIUM];
	float offsetMapY;
	float timerTopScreen;
	sfSprite *background; // Maybe rename it "interfaceBackground" to be more precise
	sfSprite* parallaxSelector;
	sfSprite* terrainSelector;
	sfSprite* selectionMask;
	sfImage* selectionMaskImage;
	Buttons buttons;
	RectangleCollider mapEditorCollider;
	ToolMode toolModeSelected;
	Tool toolSelected;
	sfBool isTesting;
	LevelNames *levelList; // List of all levels into the folders
	Level level;
	Enemies *enemies;
	Players players;
	List projectiles[WEAPON_OWNER_SIZE];
	sfVector2f playerPosition;
	Parameters parameters[TOOLMODE_NUMBER];
	Text parametersText;
	ListElement* selectedParallax;
	Terrain* selectedTerrain;
	List listParallax;
	List explosions;
	Lasers *lasers;
	char *worldName;
	int worldPosition;
	sfBool isEditingLevelName;
	sfBool haveFasterScrolling;

	// Terrain Repetition
	Terrain* tabTerrainRepetition;
	int nbTerrainRepetition;
	int nbMaxTerrainRepetition;
};

// -- MAIN FUNCTION DEFINITIONS -------------------------------------------- //

void Editor_Initialize(MainData *_data);
void Editor_HandleEvent(MainData *_data, sfEvent _event);
void Editor_Update(MainData *_data, float _dt);
void Editor_Display(MainData *_data);
void Editor_Exit(MainData *_data);

void Editor_ChangeWaypointValues(MainData *_data, EditorData *_state);
void Editor_RenameLevel(MainData *_data);
void Editor_SaveLevel(MainData *_data);
void Editor_NewLevel(MainData *_data);
void Editor_LoadLevel(MainData *_data);
void Editor_DeleteLevel(MainData *_data);
void Editor_TestLevel(MainData *_data);
void Editor_PlayLevel(MainData *_data);
void Editor_SelectLevelMode(MainData *_data);
void Editor_SelectPathMode(MainData *_data);
void Editor_SelectTerrainMode(MainData *_data);
void Editor_SelectParallaxMode(MainData *_data);
void Editor_SelectSelectTool(MainData *_data);
void Editor_SelectMoveTool(MainData *_data);
void Editor_SelectCreateTool(MainData *_data);
void Editor_SelectDeleteTool(MainData *_data);
void Editor_ReturnToMenu(MainData *_data);
void Editor_SetBackground(MainData* _data, const char *_name);
void Editor_SetBoss(MainData* _data, const char *_name);
void Editor_AddTerrain(MainData* _data, const char *_name);
void Editor_AddParallax(MainData* _data);
void Editor_UpdateTimerTopScreen(MainData* _data);
void InitParallaxList(MainData* _data);
void Editor_SelectParallaxOrTerrain(MainData* _data, sfVector2f _mousePos);
void Editor_PrintParallaxList(MainData* _data);

char* Editor_removeDotPNG(const char* _imgName, char* _tempChar);
char* ExtractFilenameFromFilepath(char* _filePath);

void EditorFade_EditorToMenu(MainData *_data);
void EditorFade_EditorToGame(MainData *_data);


#endif // !EDITOR__H