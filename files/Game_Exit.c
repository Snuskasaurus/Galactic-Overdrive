#include "Game.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

void DeleteMoneySprites(GameData *_state);
void DeleteHUDSprites(GameData *_state);
void DeleteGameSprites(GameData *_state);
void DeleteLevel(GameData *_state);
void DeleteMusic(GameData *_state);

// -- MAIN FUNCTION -------------------------------------------------------- //

void Game_Exit(MainData *_data)
{
	// Create a shortcut to the state data
	GameData *state = _data->stateData[GAME];
	// Delete entities
	Enemy_DestroyAll(state->enemies, state->lasers);
	free(state->enemies);
	if (_data->currentState != SHOP)
	{
		Player_DestroyAll(&state->players);
	}
	Laser_DestroyList(state->lasers);
	Money_DestroyAll(&state->moneyBills);

	if (_strcmpi("Null", state->boss.name))
	{
		free(state->boss.phases);
		state->boss.phases = NULL;
	}

	for (int i = 0; i < WEAPON_OWNER_SIZE; i++)
	{
		List_Destroy(&state->projectiles[i]);
	}
	List_Destroy(&state->explosions);
	DeleteMusic(state);
	DeleteLevel(state);
	DeleteGameSprites(state);

	if (_data->currentState != SHOP)
	{
		// Delete the state from memory
		free(_data->stateData[GAME]);
		_data->stateData[GAME] = NULL;
		_data->stateData[SHOP] = NULL;
	}
	else if (_data->currentState == SHOP)
	{
		_data->stateData[SHOP] = _data->stateData[GAME];
	}
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //

void DeleteMoneySprites(GameData *_state)
{
	for (int i = 0; i < MONEY_NB_TYPE; i++)
	{
		sfSprite_destroy(_state->bill[i]);
	}
}

void DeleteHUDSprites(GameData *_state)
{
	sfSprite_destroy(_state->HUD.HUD);
	sfSprite_destroy(_state->HUD.shield);
	sfSprite_destroy(_state->HUD.armorBegin);
	sfSprite_destroy(_state->HUD.armorBody);
	sfSprite_destroy(_state->HUD.armorEnd);
	sfSprite_destroy(_state->HUD.energyBegin);
	sfSprite_destroy(_state->HUD.energyBody);
	sfSprite_destroy(_state->HUD.energyEnd);
	sfSprite_destroy(_state->HUD.bossHUD);
	sfSprite_destroy(_state->HUD.bossArmor);

	sfSprite_destroy(_state->intro);
	sfSprite_destroy(_state->scoreboard);
}

void DeleteGameSprites(GameData *_state)
{
	DeleteMoneySprites(_state);
	DeleteHUDSprites(_state);
}

void DeleteLevel(GameData *_state)
{
	free(_state->level.map.selectedTerrain);
	_state->level.map.selectedTerrain = NULL;

	free(_state->level.map.terrains);
	_state->level.map.terrains = NULL;

	if (_state->level.map.nbOfBckgrndParallax > 0)
	{
		free(_state->level.map.tabParallaxBckgrnd);
		_state->level.map.tabParallaxBckgrnd = NULL;
	}

	if (_state->level.map.nbOfForgrndParallax > 0)
	{
		free(_state->level.map.tabParallaxForgrnd);
		_state->level.map.tabParallaxForgrnd = NULL;
	}

	if (_state->level.paths.selected != NULL)
	{
		free(_state->level.paths.selected);
		_state->level.paths.selected = NULL;
	}

	for (int i = 0; i < _state->level.paths.number; i++)
	{
		Waypoint* tempWaypoint = _state->level.paths.array[i]->last;

		while (tempWaypoint != _state->level.paths.array[i]->first)
		{
			tempWaypoint = tempWaypoint->previous;
			free(tempWaypoint->next);
		}
		///free(tempWaypoint);

		///free(state->level.paths.array[i]);
	}
}

void DeleteMusic(GameData *_state)
{
	if (_state->music != NULL)
	{
		sfMusic_stop(_state->music);
		sfMusic_destroy(_state->music);
		_state->music = NULL;
	}
}