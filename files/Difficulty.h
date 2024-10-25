#ifndef DIFFICULTY__H
#define DIFFICULTY__H

// -- INCLUDES ------------------------------------------------------------- //

#include "_Engine.h"

// Components
#include "Entity.h"

// Mechanics

// Others Entities


// -- FUNCTIONS ------------------------------------------------------------ //

Difficulties DifficultyData_Load();

Difficulty SetDifficulty(Difficulties _difficultyData, int _numberPlayers, eDifficultyMode _gameModeDif, int _stage);

#endif // !DIFFICULTY__H