#ifndef LEVEL__H
#define LEVEL__H

// -- INCLUDES ------------------------------------------------------------- //

#include "_Engine.h"
#include "_MainData.h"
#include "Path.h"

// -- ENUMS ---------------------------------------------------------------- //

typedef enum MapType MapType;
enum MapType
{
	MAP_BACKGROUND,
	MAP_TERRAIN,
	MAP_PARALLAX,
};

// -- FUNCTIONS ------------------------------------------------------------ //

void Level_Save(Engine *_engine, const char *_filename, Level *_level, List _listParallax);

Level *Level_Init(int _nbLevels);

Level Level_Load(const char *_filename, sfBool _isInEditor, MainData* _data);

LevelNames *Level_GetNamesInFolder();

void Level_DestroyLevelNames(LevelNames *_levelNames);
char* Editor_removeDotPNG(const char* _imgName,char* _tempChar);
char* ExtractFilenameFromFilepath(char* _filePath);

#endif // !LEVEL__H

