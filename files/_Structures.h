#ifndef STRUCTURES__H
#define STRUCTURES__H

#include "_Defines.h"
#include "_Enumerations.h"
#include "_Typedef.h"

// External Dependencies
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <SFML/System.h>
#include <SFML/Window.h>
#include <SFML/Graphics.h>
#include <SFML/OpenGL.h>
#include <SFML/Audio.h>

// Internal Dependencies
#include "Collision.h"
#include "List.h"

typedef struct ExploDir
{
	char **fileNames;
	int nbrFiles;
	char **dirNames;
	int nbrDir;
}ExploDir;

typedef struct Anim
{
	char *name;
	char state[BUFFER_EXTRA_SMALL];
	int loop;
	int nbrFrames;
	sfSprite **sprites;
	ExploDir fileNames;
	float maxTime;
}Anim;

typedef struct Animations
{
	int *nbAnimPerName;
	Anim **array;
	int nbAnims;
}Animations;

typedef struct Timer
{
	float currentTime;
	float maxTime;
	sfBool isPlaying;
}Timer;

typedef struct ShakingInfos ShakingInfos;
struct ShakingInfos
{
	sfVector2i shakingOffset;
	int shakingPower;
	int isShaking;
	float timer;
	float timerMax;
};

typedef struct Entity Entity;
struct Entity
{
	float angle;
	float angleDisplayed;
	sfVector2f origin;
	sfVector2f position;
	sfVector2f axisPosition;
	sfVector2f goal;
	sfVector2f velocity;
	Timer animationTimer;
	int currentFrame;
	sfVector2f size;
	ShakingInfos shake;
};

// -- WEAPONS -------------------------------------------------------------- //

typedef struct Weapon_SinusoidalParameters Weapon_SinusoidalParameters;
struct Weapon_SinusoidalParameters
{
	float sign;
	sfBool isInverted;
	float amplitude; // Amplitude of curved projectiles
	float period; // Period of curved projectiles
	float range;
};

typedef struct Weapon Weapon;
struct Weapon
{
	Weapon* Next;
	// Variables set by the files
	char name[BUFFER_EXTRA_SMALL];
	Entity entity;
	sfSprite *spriteW; // The sprite of the weapon, displayed in shop
	sfSprite *spriteProjectile; // The sprite of the weapon, displayed in shop
	Anim *animProj;
	Anim *animBody;
	Anim *animBase;
	Anim *weaponAnim;
	WeaponType type;
	WeaponBehaviour behaviour;
	float damage;
	float baseDamage;
	Timer rate; // Rate of fire, can fire when the timer is finished
	float baseFireRateMaxTime;
	int nbShot; // Number of shoot to fire simultaneously
	float dispersionShotAngle;
	float shotOffset;
	float defaultAngle;
	float angle;
	float precision;
	float precisionAngle;
	float speed;
	WeaponAiming aimType;
	float aimingRotationSpeed;
	int nbSoundsShooting; // number of sounds while shooting
	char **soundsShooting; // Store the names of the sounds while shooting
	sfSound* sound; // Used with the lasers
	int firstSoundPlaying;

	// For sinusoidal movements
	float amplitude; // Amplitude of curved projectiles
	float period; // Period of curved projectiles
	float range;
	sfBool isInvertedCurve;
	float curveDirection;

	// Homing missiles
	float rotationSpeed;
	float fuel;

	// DirectionalShield
	float life;
	float lifeMax;
	float lifeRegenPerSecond;
	float timerToRegen; // How many seconds to wait before starting regenerating life
	float attackRadius;
	int attackCharges;
	int attackChargesMax;
	int shieldAngleCovered; // Angle of protection covered by the shield
	int canBeActivated;
	int rotationDirection;
	int isActive; // Are we using the weapon this frame ? TRUE/FALSE

	// Variables modified in game
	sfVector2f position; // the current position of the weapon in the world
	WeaponOwner owner; // To keep track the entity ownership of the projectiles
	void *player; // To keep track of the player ownership, NULL if sent by enemies

	char* upgrades[BUFFER_MEDIUM];
	int nbUpgardes;
	eWeaponTier upgradeTier;
	unsigned int price;
	sfBool isVisibleInShop;

	///char *type;
};

typedef struct Collider Collider;
struct Collider
{
	ColliderType type;
	void *pointer;
};

// -- LASERS --------------------------------------------------------------- //

typedef struct Laser Laser;
struct Laser
{
	void *entityPtr;
	Entity laserEntityHead;
	Entity laserEntityBody;
	Entity laserEntityBase;
	Weapon *origin;
	Collider collider;
	LaserDir dir;
	Laser *nextPtr;
};

typedef struct Lasers
{
	int nbElements[WEAPON_OWNER_SIZE];
	Laser *firstElement[WEAPON_OWNER_SIZE];
	Laser *lastElement[WEAPON_OWNER_SIZE];
}Lasers;

typedef struct WeaponSlot
{
	sfVector2f offset;
	Weapon weapon;
	WeaponType type;
}WeaponSlot;

typedef struct FiringMode FiringMode;
struct FiringMode
{
	char *name;
	int number;
	sfVector2f *offsets;
	Weapon weapon;
	Timer rate;
	float angle;
};

// -- TURRET --------------------------------------------------------------- //

typedef struct Turret Turret;
struct Turret
{
	char name[BUFFER_MEDIUM];
	char weaponName[BUFFER_MEDIUM];
	Weapon weapon;
	Entity entity;
	float turnRate;
	TurretBehaviour behaviour;
	DamageType type;
	sfVector2f weaponOffset;
	float maximumHealth;
	float health;
	Collider collider;
	sfSprite *sprite;
	Timer firingTimer;
	Timer reloadTimer;
	float defaultAngle;
	sfVector2f offset;
	Timer timerFeedback;
};

// -- PROJECTILES ---------------------------------------------------------- //

typedef struct ProjectileData ProjectileData;
struct ProjectileData
{
	Entity entity;
	Weapon weapon;
	WeaponType type;
	Collider collider;
	float angle;
	float curveDirection; // The direction of the curved
	float fuel;
};

// -- SKILLS --------------------------------------------------------------- //

typedef struct Skill
{
	/// Non skill specific
	char name[BUFFER_EXTRA_SMALL];
	sfSprite *icon;
	eSkill type;
	Anim *anim;
	Timer duration;
	Entity entity;
	sfBool isActive;
	sfBool bypass;
	sfBool autoDisplay;
	char soundName[BUFFER_EXTRA_SMALL];
	unsigned long scoreToUnlock;
	unsigned long currentScore;
	int skillStorageCap;
	int nbSkillInStorage;
	float effectRadius;
	/// Skill specific
	/// Regen skill :
	float regenSpeed;
	/// Dmg Boost :
	float boostPerCent_Dmg;
	float boostPerCent_FireRate;
	sfBool hasBeenActivated_Boost;
	/// Protection shield :
	int nbProtectionShield;
	/// Rez :
	int *indexOfPlayerRez;
	int nbIndex;
	Entity entityRez;
	Anim *animRez;
	/// TP
	Entity entityDisappearTP;
	Entity entityBlastTP;
	Anim *animDisappear;
	Anim *animBlast;
	sfBool hasDisappeared;
	Collider cockpitColliderSaved;
	/// Dash
	int dashRange;
	sfBool isDashing;
}Skill;

typedef struct Skills
{
	Skill **array;
	int nbr;
}Skills;

// -- PLAYERS -------------------------------------------------------------- //

typedef struct Player Player;
struct Player
{
	Entity entity;
	sfBool internalDisplay;
	Anim *leftAnim;
	Anim *rightAnim;
	Anim *idle;
	Anim *currentAnim;
	joystickId joystick; // The id of the joystick atributed for this player
	WeaponSlot weaponSlots[WEAPON_SLOTS_MAX];
	float changeWeapon;
	int nbSlotsMax;

	FiringMode weapons[3];
	int currentFireMode;
	char *name;
	float armor;
	float maxArmor;
	int shield;
	int maxShield;
	int protectionShield;
	sfBool isAlive;
	float shieldRegenSpeed;
	Timer shieldCooldown;
	Collider cockpitCollider;
	Collider spaceshipCollider;
	Timer invulnerability;
	float dmgMultiplier;
	float fireRateMultiplier;
	Timer LaserBoosted;
	float laserBoostReceived;

	Skill skillInfo;

	// If a player can be targeted by enemies
	sfBool canBeTargeted;

	sfColor color;

	unsigned long score;

	// for the shop
	Weapon* inventory;
	int inventorySize;
	unsigned int money;
};

typedef struct Players Players;
struct Players
{
	int number;
	Player *array[MAX_PLAYER_NUMBER];
};

// -- WAYPOINTS AND PATHS -------------------------------------------------- //

typedef struct Waypoint_Spawn Waypoint_Spawn;
struct Waypoint_Spawn
{
	float timeStart;
	float duration;
	float since; // Time since a enemy has spawned
	float rate; // Speed at wich enemies are spawning
	int spawned; // Number of enemies spawned
	int maximum; // Maximum number of enemies spawing
	char enemyName[BUFFER_SMALL];
	int difficulty;
};

typedef struct Path Path;
typedef struct Waypoint Waypoint;

struct Waypoint
{
	WaypointType type;
	void *content; // A address towards info about the current waypoint data structure
	sfVector2f position;
	Path *path; // The address path of the owner of this waypoint
	Waypoint *previous;
	Waypoint *next;
};

struct Path
{
	int number;
	Waypoint *first;
	Waypoint *last;
};

typedef struct Paths Paths;
struct Paths
{
	int maximumNumber;
	int number;
	Path **array;
	Waypoint *selected;
};

// -- ENEMIES -------------------------------------------------------------- //

typedef struct Enemy Enemy;
struct Enemy
{
	Entity entity;
	float health;
	sfBool isAlive;
	Timer invulnerability;
	sfSprite *sprite;
	sfBool haveWeapon;
	Weapon weapon;
	EnemyType behaviour;
	DamageType type;
	Collider smallCollider;
	Collider largeCollider;
	Waypoint *currentWaypoint;
	ExplosionType explosionsType;
	ExplosionSize explosionsSize;
	sfBool haveTurretLive;
	sfVector2f turretOffset;
	Turret turret;
	char name[BUFFER_EXTRA_SMALL];
	Player *target;
	float speed;
	int isHit;
	int hasBeenDisplayedHit;
	Hit_Type hitType;
	Timer timerToDisplayWhite;
	Timer timerWaitingTime;
	Timer timerToDisplayColor;
	int totalScorePool;
	int activeScorePool;
	int hitAmount;
	int goodHitAmount;
	Timer timerBeforeShooting;
	Timer timerWhileShooting;
	sfBool isSlowDown;
	sfBool canSlowDown;
};

typedef struct Enemies Enemies;
struct Enemies
{
	int number;
	Enemy *array[MAX_ENEMY_NUMBER];
};

// -- BOSS ----------------------------------------------------------------- //

typedef struct BossWeakpoint BossWeakpoint;
struct BossWeakpoint
{
	Collider collider;
	sfColor color;
	int isHit;
	Timer timerFeedback;
};

typedef struct BossPatternWeapon BossPatternWeapon;
struct BossPatternWeapon
{
	FiringMode *pointer;
	Timer wait;
	Timer duration;
	sfBool isRepeated;
};


typedef struct BossEnemySpawn BossEnemySpawn;
struct BossEnemySpawn
{
	int number;
	Boss_EnemySpawnDirection direction;
	char enemyName[BUFFER_MEDIUM];
	sfVector2f startingPosition;
	float offset;
	Timer timer;
};

typedef struct BossPattern BossPattern;
struct BossPattern
{
	Timer wait;
	Timer duration;
	int nbWeaponSlot;
	BossPatternWeapon *weaponSlot;
	
	sfBool haveXMovement;
	sfBool isMovingRight;
	float speedXMovement;
	float minXMovement;
	float maxXMovement;

	sfBool haveYMovement;
	sfBool isMovingBottom;
	float speedYMovement;
	float minYMovement;
	float maxYMovement;

	int nbEnemySpawn;
	BossEnemySpawn *enemySpawn;
};


typedef struct BossPhase BossPhase;
struct BossPhase
{
	float health;
	Boss_IntroductionType introduction;
	Boss_PhaseEndingType ending;
	int nbWeapons;
	FiringMode *weapons;
	Anim *animation;
	Anim *introductionAnimation;
	Entity overlayAnimationEntity;
	int currentPattern;
	int nbPatterns;
	BossPattern *patterns;
	sfBool isInvulnerable;
	sfVector2f goalPosition;
	float velocity;
	Timer endingTimer;
	Boss_DamagedFrom damagedFrom;
	Timer timerTurretActivation;
	// Turrets
	int nbTurrets;
	Turret *turrets;
	sfBool turretsAreBehind;
	// Colliders of the entire boss for collision with the players
	int nbColliders;
	Collider *colliders;
	// Colliders for weakpoints : where the boss will take damages
	sfTexture *maskWeakpoints;
	int nbWeakpoints;
	BossWeakpoint *weakpoints;
};

typedef struct Boss Boss;
struct Boss
{
	Entity entity;
	Entity introductionAnimationEntity;
	char* name;
	int currentPhase;
	int nbPhases;
	BossPhase *phases;
	Collider logicalCollider; // Used to movement
	Timer invulnerability;
	sfBool isAlive;
	sfBool isActive;
	Anim* intro;
	Entity introEntity;
	sfBool isInIntroduction;
	sfSprite *currentSprite;
};

// -- EXPLOSIONS ----------------------------------------------------------- //

typedef struct ExplosionData ExplosionData;
struct ExplosionData
{
	ExplosionType type;
	ExplosionSize size;
	Entity entity;
	Anim *animation;
	float scale;
	int nbSounds;
	char **soundNames;
};
typedef struct List Explosions;

// -- LEVELS --------------------------------------------------------------- //

typedef struct MapParallax MapParallax;
struct MapParallax
{
	sfVector2f startingPosition;
	sfVector2f position;
	sfVector2f size;
	sfSprite* sprite;
	Anim* animation;
	Entity entity;
	// At which time should we update the parallax
	float timerToUpdate;
	// Speed in pixels per second
	float speed;
	int depth;
	char filePath[BUFFER_SMALL];
	char isSpriteLoaded;
};

typedef struct Terrain Terrain;
struct Terrain
{
	sfVector2f position;
	sfVector2f size;
	sfSprite* sprite;
	Anim* animation;
	Entity entity;
	char filePath[BUFFER_SMALL];
	char isSpriteLoaded;
};

typedef struct Map Map;
struct Map
{
	Terrain *selectedTerrain;
	float time;
	float scrollingSpeed;
	int nbTerrains;
	int activeMap;
	char nbOfForgrndParallax;
	char nbOfBckgrndParallax;
	char backgroundFilepath[BUFFER_SMALL];
	Anim* animatedBackground;
	Entity entityBackground;
	Terrain* terrains;
	MapParallax* tabParallaxForgrnd; // Contains every background Parallax
	MapParallax* tabParallaxBckgrnd; // Contains every foreground Parallax
	float positionRepeatedTerrainY[MAP_TERRAIN_REPETITION_NB];
	char wasRepeatedTerrainInitialized;
};

typedef struct Level Level;
struct Level
{
	char name[BUFFER_SMALL];
	Paths paths;
	Map map;
	float bossTime;
	char bossName[BUFFER_SMALL];
	char worldName[BUFFER_SMALL];
	int positionWorld;
	char musicName[BUFFER_SMALL];
	sfBool isInSpace;
	sfBool terrainsAreRepeated;
};

typedef struct LevelNames LevelNames;
struct LevelNames
{
	int number;
	char **array;
};

typedef struct World
{
	LevelNames levelNames;
	int currentLevel;
}World;

typedef struct StarParallax StarParallax;
struct StarParallax
{
	sfVector2f position;
	float depth;
	float scale;
	int spriteIndex;
};

typedef struct StarTrail StarTrail;
struct StarTrail
{
	sfVector2f position;
	float speed;
	float timeToDecay;
	float decayTimer;
	float scale;
	int spriteIndex;
};

typedef struct StarsBackground StarsBackground;
struct StarsBackground
{
	int nbParallaxes;
	int nbTrails;
	sfSprite* poolStarsParallax[STAR_PARALLAX_POOL_SIZE];
	sfSprite* poolTrail[STAR_TRAIL_POOL_SIZE];
	StarParallax tabParallaxes[STAR_PARALLAX_MAX];
	StarTrail tabTrails[STAR_TRAIL_MAX];
};

// -- MONEY ---------------------------------------------------------------- //

typedef struct Money Money;
struct Money
{
	Money* Next;
	Entity entity;
	Anim *animation;
	Collider collider;
	MoneyAmount Amount;
};

// -- DIFFICUTY ------------------------------------------------------------ //
typedef struct Difficulty
{
	eDifficultyMode mode;
	int nbPlayer;
	int stage;

	float weaponMultiplierGood;
	float weaponMultiplierBad;
	float scoreMultiplier;//done
	float enemiesHealthMultiplier;//done
	float enemiesPrecisionMultiplier;//done
	float enemiesDamageMultiplier;//done
	float enemiesSpawnMultiplier;//done
	float objectCostMultplier;
	float enemiesSpeedMultiplier;//done
	float enemiesProjSpeedMultiplier;//done
	float enemiesFiringRateMultiplier;//done
}Difficulty;

typedef struct Difficulties
{
	Difficulty playerNumberDif[MAX_PLAYER_NUMBER];
	Difficulty gamemodeDif[NB_DIF_MODE];
	Difficulty stageDif[DIF_MAX_NB_STAGES];
}Difficulties;

// -- Text ----------------------------------------------------------------- //

typedef struct Font Font;
struct Font
{
	sfFont *pointer;
	char *name;
};

typedef struct TextEngine TextEngine;
struct TextEngine
{
	int nbFonts;
	Font fonts[FONTS_NB];
};

typedef struct Text Text;
struct Text
{
	int size;
	sfFont *font;
	sfColor color;
};

//// -- FADE ----------------------------------------------------------------- //
//
//typedef struct TransitionFade TransitionFade;
//struct TransitionFade
//{
//	sfSprite* spriteFade;
//	sfColor color;
//	FadeState state;
//	void(*ptrChangeState)(MainData*);
//};

// -- ACHIEVEMENTS --------------------------------------------------------- //

typedef struct Achievement Achievement;
struct Achievement
{
	char id_Name[BUFFER_SMALL];
	char icon_Name[BUFFER_SMALL];
	char title[BUFFER_SMALL];
	char description[BUFFER_MEDIUM];
	sfBool isUnlocked;
	sfSprite* icon_Sprite;
	sfVector2u icon_Size;
	Text parametersTextTitle;
	Text parametersTextDesc;
};

typedef struct AchievementVariablesToUnlock AchievementVariablesToUnlock;
struct AchievementVariablesToUnlock
{
	sfBool doPlayerTookDamage;
	sfBool hasBoughtWeaponInTerra;
	sfBool hasBoughtWeaponInSpace;
	sfBool hasBoughtWeaponInAlien;
	int nbEnemiesSpawned;
	int nbEnemiesKilled;
};

// -- NOTIFICATION --------------------------------------------------------- //

typedef struct Notification Notification;
struct Notification
{
	Entity entity;
	void* data;
	Notification_Type dataType;
	sfSprite* background;
	float timeToLive;
	float timeToDisapear;
	sfColor color;
};

typedef struct Notifications Notifications;
struct Notifications
{
	int number;
	Notification arrNotifs[NOTIFICATION_MAX];
};

// -- BLUEPRINTS ----------------------------------------------------------- //
typedef struct BP_Achievements BP_Achievements;
struct BP_Achievements
{
	int number;
	Achievement **array;
};

typedef struct BP_Weapons BP_Weapons;
struct BP_Weapons
{
	int number;
	Weapon **array;
};

typedef struct BP_Turrets BP_Turrets;
struct BP_Turrets
{
	int number;
	Turret **array;
};

typedef struct BP_Players
{
	int number;
	Player **array;
}BP_Players;

typedef struct BP_Enemies
{
	int number;
	Enemy **array;
}BP_Enemies;

typedef struct BP_Explosions
{
	int number;
	ExplosionData **array;
}BP_Explosions;

typedef struct BP_Boss
{
	int number;
	Boss **array;
}BP_Boss;

typedef struct Blueprint Blueprint;
struct Blueprint
{
	BP_Weapons weapons;
	BP_Turrets turrets;
	BP_Players players;
	BP_Enemies enemies;
	BP_Explosions explosions;
	BP_Achievements achievements;
	BP_Boss boss;
	Difficulties difficultyBlueprint;
};



// GameState
struct GameHUD
{
	sfSprite* HUD;
	sfSprite* shield;
	sfSprite* armorBegin;
	sfSprite* armorBody;
	sfSprite* armorEnd;
	sfSprite* energyBegin;
	sfSprite* energyBody;
	sfSprite* energyEnd;
	sfSprite* bossHUD;
	sfSprite* bossArmor;
};

// Data used to store all informations about the state
typedef struct GameData GameData;
struct GameData
{
	Players players;
	Enemies *enemies;
	Boss boss;
	List projectiles[WEAPON_OWNER_SIZE];
	List explosions;
	Level level;
	int levelIndex;
	Lasers *lasers;
	Collider screenCollider;
	Money* moneyBills;
	sfSprite* bill[3];
	unsigned int collectedBills;
	struct GameHUD HUD;
	unsigned long squadScore;
	sfMusic *music;
	Text parametersText;
	sfSprite* intro;
	sfVector2f introSpawn;
	float outroCooldown;
	sfSprite* scoreboard;
	sfVector2f scoreboardPos;
	sfSprite* background;
	sfSprite* shopSprite;
	sfSprite* frames[3];
	sfSprite* shopCursor;
	sfSprite* shopState[SHOP_READY];
	sfSprite* shopPlayerID[MAX_PLAYER_NUMBER];
	sfSprite* numbers[10];
	ShopState currentShopState[MAX_PLAYER_NUMBER];
	int shopCursorPos[MAX_PLAYER_NUMBER];
	int shopCursorOffset[MAX_PLAYER_NUMBER];
	sfBool isDefeated;
	Entity defeatSreenEntity;
	Anim *defeatScreen;
	int shopIsEquiping[MAX_PLAYER_NUMBER];
	int shopEquipCursorPos[MAX_PLAYER_NUMBER];
	int shopEquipCursorOffset[MAX_PLAYER_NUMBER];
	sfBool isIndestructible;
	StarsBackground starBackground;
	Weapon* testWeapon[MAX_PLAYER_NUMBER];
	int isMakingTransaction[MAX_PLAYER_NUMBER];
	sfBool hasClearedEverything;
	sfBool hasInterfaceDisplayed;
};

#endif // !STRUCTURES__H