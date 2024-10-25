#ifndef PROGRESSION__H
#define PROGRESSION__H

#include "_MainData.h"
#include "Difficulty.h"
#include "World.h"

typedef struct
{
	int progress;
	unsigned long Level1Highscores[NB_DIF_MODE][6];
	unsigned long Level2Highscores[NB_DIF_MODE][6];
	unsigned long Level3Highscores[NB_DIF_MODE][6];
} GOProgression;

void LoadProgression(GOProgression* _Progression, MainData* _data);
void SaveProgression(GOProgression* _Progression, MainData* _data);
void SaveFinalScore(MainData* _data, unsigned long _finalScore, char* _levelName);

#endif