#ifndef ENUMERATIONS__H
#define ENUMERATIONS__H

typedef enum LaserSound LaserSound;
enum LaserSound
{
	LASER_SOUND_START,
	LASER_SOUND_LOOP,
};

typedef enum FadeState FadeState;
enum FadeState
{
	NO_FADE,
	FADE_IN,
	FADE_OUT,
};

// The states of the game
typedef enum GameState GameState;
enum GameState
{
	SPLASH,
	MENU,
	GAME,
	CREDIT,
	OVERWORLD,
	EDITOR,
	SHOP,
	NB_GAMESTATE
};

typedef enum VariableType VariableType;
enum VariableType
{
	TYPE_INT,
	TYPE_FLOAT,
	TYPE_BOOLEAN
};

typedef enum ToolMode ToolMode;
enum ToolMode
{
	TOOLMODE_DEFAULT = 0,
	TOOLMODE_LEVEL = 0,
	TOOLMODE_PATH,
	TOOLMODE_TERRAIN,
	TOOLMODE_PARALLAX,
	TOOLMODE_NUMBER
};

typedef enum Tool Tool;
enum Tool
{
	TOOL_NONE,
	TOOL_MOVE,
	TOOL_SELECTION,
	TOOL_DELETION,
	TOOL_CREATION
};

typedef enum WaypointType WaypointType;
enum WaypointType
{
	WAYPOINT_SPAWN,
	WAYPOINT_MOVE,
	WAYPOINT_ATTACK,
	WAYPOINT_WAIT,
	WAYPOINT_NUMBER_ELEMENTS
};

typedef enum EnemyType EnemyType;
enum EnemyType
{
	ENEMY_TYPE_DEFAULT,
	ENEMY_TYPE_WAYPOINT = 0,
	ENEMY_TYPE_FOLLOWER,
	ENEMY_TYPE_ARTILLERY,	// nope
	ENEMY_TYPE_THIEF,
	ENEMY_TYPE_NB
};

typedef enum DamageType DamageType;
enum DamageType
{
	TYPE_DEFAULT = 0,
	TYPE_CINETIC = 0,
	TYPE_ENERGY = 1,
	TYPE_ARMOR = 2,
	TYPE_DMG_NB // Keep last
};

typedef enum TurretBehaviour TurretBehaviour;
enum TurretBehaviour
{
	TURRET_BEHAVIOUR_STATIC,
	TURRET_BEHAVIOUR_CLOSEST,
	TURRET_BEHAVIOUR_ROTATE,
};

typedef enum WeaponOwner WeaponOwner;
enum WeaponOwner
{
	WEAPON_OWNER_PLAYER,
	WEAPON_OWNER_ENEMY,
	WEAPON_OWNER_BOSS,
	WEAPON_OWNER_SIZE
};

typedef enum Hit_Type Hit_Type;
enum Hit_Type
{
	HIT_DEFAULT,
	HIT_GOOD,
	HIT_BAD,
};

typedef enum WeaponType WeaponType;
enum WeaponType
{
	WEAPON_CINETIC,
	WEAPON_ENERGETIC,
	WEAPON_EXPLOSIVE,
	WEAPON_HEAL,
	WEAPON_DMG_BOOST,
	WEAPON_NB_TYPES // Keep last
};

typedef enum ColliderType ColliderType;
enum ColliderType
{
	COLLIDER_NONE = 0,
	COLLIDER_CIRCLE,
	COLLIDER_RECTANGLE,
	COLLIDER_IMAGE,
	COLLIDER_SHAPE
};

typedef enum ExplosionType ExplosionType;
enum ExplosionType
{
	EXPLOSION_TYPE_DEFAULT = 0,
	EXPLOSION_TYPE_CINETIC = 0,
	EXPLOSION_TYPE_ENERGY = 1,
	EXPLOSION_TYPE_EXPLOSIVE = 2,
	EXPLOSION_TYPE_TEXT = 3,
	EXPLOSION_TYPE_NB // Keep last
};

typedef enum ExplosionSize ExplosionSize;
enum ExplosionSize
{
	EXPLOSION_SIZE_DEFAULT = 0,
	EXPLOSION_SIZE_SMALL = 0,
	EXPLOSION_SIZE_MEDIUM = 1,
	EXPLOSION_SIZE_BIG = 2,
	EXPLOSION_SIZE_IMPACT = 3,
	EXPLOSION_SIZE_GOOD = 4,
	EXPLOSION_SIZE_PERFECT = 5,
	EXPLOSION_SIZE_MONEY_100 = 6,
	EXPLOSION_SIZE_MONEY_500 = 7,
	EXPLOSION_SIZE_MONEY_1000 = 8,
};

// Define the behaviour of the projectile launched by the weapon
// STRAIGHTED : Simple straight trajectory
// CURVED : Follow a curved trajectory
// ORIENTED : Stay with the initial trajectory
// GUIDED : Follow a target
typedef enum WeaponBehaviour WeaponBehaviour;
enum WeaponBehaviour
{
	// Straigthed projectiles
	WEAPON_STRAIGHTED_PROJECTILE_ORIENTED,
	WEAPON_STRAIGHTED_PROJECTILE_GUIDED,
	// Curved projectiles
	WEAPON_CURVED_PROJECTILE_ORIENTED,
	WEAPON_CURVED_PROJECTILE_GUIDED,
	// Straigthed lasers
	WEAPON_STRAIGHTED_LASER_ORIENTED,
	WEAPON_STRAIGHTED_LASER_GUIDED,
	// Curved lasers
	WEAPON_CURVED_LASER_ORIENTED,
	WEAPON_CURVED_LASER_GUIDED,
	// Others behaviours
	WEAPON_CONE_ORIENTED,
	WEAPON_CONE_GUIDED,
	WEAPON_AURA,
	WEAPON_SHIELD
};

typedef enum WeaponAiming WeaponAiming;
enum WeaponAiming
{
	WEAPON_AIMING_STATIC,
	WEAPON_AIMING_PLAYER,
	WEAPON_AIMING_ROTATION,
};

typedef enum eSkill
{
	REGEN,
	ATK_BOOST,
	ENEMY_CLEAR,
	CLEAR_PROJ,
	PROTECTION_SHIELD,
	REZ,
	TP,
	DASH,
	NB_SKILLS // keep last
}eSkill;

typedef enum Boss_EnemySpawnDirection Boss_EnemySpawnDirection;
enum Boss_EnemySpawnDirection
{
	BOSS_ENEMY_SPAWN_TOP,
	BOSS_ENEMY_SPAWN_RIGHT,
	BOSS_ENEMY_SPAWN_BOTTOM,
	BOSS_ENEMY_SPAWN_LEFT,
};

typedef enum Boss_IntroductionType Boss_IntroductionType;
enum Boss_IntroductionType
{
	BOSS_INTRODUCTION_NONE = 0,

	BOSS_INTRODUCTION_FROM_TOP = 1,
	BOSS_INTRODUCTION_FROM_BOTTOM = 2,
	BOSS_INTRODUCTION_FROM_LEFT = 3,
	BOSS_INTRODUCTION_FROM_RIGHT = 4,

	BOSS_INTRODUCTION_TO_TOP = 5,
	BOSS_INTRODUCTION_TO_BOTTOM = 6,
	BOSS_INTRODUCTION_TO_LEFT = 7,
	BOSS_INTRODUCTION_TO_RIGHT = 8,

	BOSS_INTRODUCTION_ANIMATION = 9,
};

typedef enum Boss_PhaseEndingType Boss_PhaseEndingType;
enum Boss_PhaseEndingType
{
	BOSS_PHASE_ENDING_DEFAULT = 0,
	BOSS_PHASE_ENDING_HEALTH = 0,
	BOSS_PHASE_ENDING_POSITION = 1,
	BOSS_PHASE_ENDING_ANIMATION = 2,
	BOSS_PHASE_ENDING_TIME = 3,
};

typedef enum Boss_DamagedFrom Boss_DamagedFrom;
enum Boss_DamagedFrom
{
	BOSS_DAMAGED_FROM_WEAKPOINT = 0,
	BOSS_DAMAGED_FROM_TURRET = 1,
};

typedef enum Boss_MovementAxis Boss_MovementAxis;
enum Boss_MovementAxis
{
	BOSS_MOVEMENT_AXIS_Y = 0,
	BOSS_MOVEMENT_AXIS_X = 1,
};

typedef enum MoneyAmount MoneyAmount;
enum MoneyAmount
{
	MONEY_100,
	MONEY_500,
	MONEY_1000,
	MONEY_NB_TYPE // Keep last
};

typedef enum LaserDir
{
	LASER_UP = 1,
	LASER_DOWN = -1
}LaserDir;


typedef enum Direction Direction;
enum Direction
{
	NONE,
	FRONT,
	BACK,
	LEFT,
	RIGHT,
};

typedef enum DifficultyMode
{
	EASY,
	NORMAL,
	HARD,
	NB_DIF_MODE // Keep last
}eDifficultyMode;

typedef enum WeaponTier
{
	TIER_0,
	TIER_1,
	TIER_2,
	TIER_3,
	NB_TIER,
}eWeaponTier;

typedef enum
{
	SHOP_BUY,
	SHOP_SELL,
	SHOP_EQUIP,
	SHOP_READY,
	NB_SHOP_STATES
} ShopState;

typedef enum Notification_Type
{
	NOTIFY_NO_DATA,
	NOTIFY_ACHIEVEMENT,
}Notification_Type;

typedef enum SettingsType
{
	RESOLUTION,
	FULL_SCREEN,
	VSYNC,
	FPS_CAP,
	MAIN_VOL,
	SOUND_VOL,
	MUSIC_VOL,
	NB_SETTINGS // Keep last
}eSettings;

#endif // !ENUMERATIONS__H