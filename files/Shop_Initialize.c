#include "Shop.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

void Shop_LoadSprites(MainData* _data);

// -- MAIN FUNCTION -------------------------------------------------------- //

void Shop_Initialize(MainData *_data)
{
	// Create a shortcut to the state data
	GameData *state = _data->stateData[SHOP];
	sfVector2i tempResolution = _data->engine.configuration->resolution;

	// Inititalize the Entities
	state->enemies = Enemy_Initialize();
	state->lasers = Laser_InitLinkedList();
	// Initialize various variables
	state->screenCollider = Collider_CreateRectangleCollider(V2F(0.0f, 0.0f), V2F_FROM_V2I(_data->engine.configuration->resolution), FALSE);
	// Create players
	for (int i = 0; i < state->players.number; i++)
	{
		state->players.array[i]->entity.position.x = (float)((i * 2 + 1) * tempResolution.x / 8);
		state->players.array[i]->entity.position.y = (float)((tempResolution.y - tempResolution.x / 4) / 2);
		state->players.array[i]->armor = state->players.array[i]->maxArmor;
		state->players.array[i]->isAlive = sfTrue;
		state->players.array[i]->money += state->collectedBills;

		state->currentShopState[i] = SHOP_EQUIP;
		state->shopCursorPos[i] = 0;
		state->shopCursorOffset[i] = 0;
		state->shopIsEquiping[i] = sfFalse;
		state->shopEquipCursorPos[i] = 0;
		state->shopEquipCursorOffset[i] = 0;
		state->testWeapon[i] = (Weapon*)malloc(sizeof(Weapon));
		(*state->testWeapon[i]) = GetWeapon(_data->engine.blueprints.weapons.array[0]->name, &_data->engine.blueprints.weapons, WEAPON_OWNER_PLAYER, state->players.array[i]);
		state->isMakingTransaction[i] = sfFalse;
	}

	Shop_LoadSprites(_data);

	// Init money list to NULL
	state->moneyBills = NULL;

	// Init money counter to 0
	state->collectedBills = 0;

	state->parametersText.size = 30 * tempResolution.x / 1920;
	state->parametersText.font = Text_GetFont(_data->engine.text, "AXIS Extra Bold 800.otf");
	state->parametersText.color = sfWhite;

	_data->engine.time = 0.f;
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //

void Shop_LoadSprites(MainData* _data)
{
	GameData *state = _data->stateData[SHOP];
	char player[BUFFER_SMALL];

	state->background = Sprite_CreateFromFile("Data/Sprites/Backgrounds/Background_OverWorld.png", 0.0f, 0.0f);

	state->shopSprite = Sprite_CreateFromFile("Data/Sprites/Interfaces/shop.png", 0.0f, 0.0f);

	state->frames[0] = Sprite_CreateFromFile("Data/Sprites/Interfaces/frame_bronze.png", 0.0f, 0.0f);
	state->frames[1] = Sprite_CreateFromFile("Data/Sprites/Interfaces/frame_silver.png", 0.0f, 0.0f);
	state->frames[2] = Sprite_CreateFromFile("Data/Sprites/Interfaces/frame_gold.png", 0.0f, 0.0f);

	state->shopState[SHOP_BUY] = Sprite_CreateFromFile("Data/Sprites/Interfaces/Texte/buy.png", 0.5f, 0.5f);
	state->shopState[SHOP_SELL] = Sprite_CreateFromFile("Data/Sprites/Interfaces/Texte/Sell.png", 0.5f, 0.5f);
	state->shopState[SHOP_EQUIP] = Sprite_CreateFromFile("Data/Sprites/Interfaces/Texte/Inventory.png", 0.5f, 0.5f);

	for (int i = 0; i < MAX_PLAYER_NUMBER; i++)
	{
		sprintf(player, "Data/Sprites/Interfaces/Texte/j%d.png", (i + 1));
		state->shopPlayerID[i] = Sprite_CreateFromFile(player, 0.5f, 0.5f);
	}

	state->shopCursor = Sprite_CreateFromFile("Data/Sprites/Interfaces/frame_cursor.png", 0.0f, 0.0f);

	state->numbers[0] = Sprite_CreateFromFile("Data/Sprites/Interfaces/Numbers/0.png", 0.5f, 0.5f);
	state->numbers[1] = Sprite_CreateFromFile("Data/Sprites/Interfaces/Numbers/1.png", 0.5f, 0.5f);
	state->numbers[2] = Sprite_CreateFromFile("Data/Sprites/Interfaces/Numbers/2.png", 0.5f, 0.5f);
	state->numbers[3] = Sprite_CreateFromFile("Data/Sprites/Interfaces/Numbers/3.png", 0.5f, 0.5f);
	state->numbers[4] = Sprite_CreateFromFile("Data/Sprites/Interfaces/Numbers/4.png", 0.5f, 0.5f);
	state->numbers[5] = Sprite_CreateFromFile("Data/Sprites/Interfaces/Numbers/5.png", 0.5f, 0.5f);
	state->numbers[6] = Sprite_CreateFromFile("Data/Sprites/Interfaces/Numbers/6.png", 0.5f, 0.5f);
	state->numbers[7] = Sprite_CreateFromFile("Data/Sprites/Interfaces/Numbers/7.png", 0.5f, 0.5f);
	state->numbers[8] = Sprite_CreateFromFile("Data/Sprites/Interfaces/Numbers/8.png", 0.5f, 0.5f);
	state->numbers[9] = Sprite_CreateFromFile("Data/Sprites/Interfaces/Numbers/9.png", 0.5f, 0.5f);
}