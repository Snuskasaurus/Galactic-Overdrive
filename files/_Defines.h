#ifndef DEFINES__H
#define DEFINES__H

// Mathematics used in all files
#define PI 3.14f
#define RAND(_min, _max)(rand() % ((_max) - (_min) + 1) + (_min))
#define RAND_SIGN()(rand() % 2 ? 1 : -1)
#define GET_SIGN(value)(value / abs(value))


// Vectors used in all files
#define V2I(x, y)(sfVector2i){(int)x, (int)y}
#define V2I_FROM_V2F(vector)(sfVector2i){(int)vector.x, (int)vector.y}

// Float vectors used in all files
#define V2F_NULL (sfVector2f){0.0f, 0.0f}
#define V2F(x, y)(sfVector2f){(float)x, (float)y}
#define V2F_FROM_V2I(vector)(sfVector2f){(float)vector.x, (float)vector.y}
#define V2F_FROM_VALUE(value)(sfVector2f){(float)value, (float)value}
#define V2F_COMPARE(vector1, vector2)  vector1.x == vector2.x && vector1.y == vector2.y ? TRUE : FALSE

#define V2F_MUL_V2F(vector1, vector2)(sfVector2f){(float)vector1.x * (float)vector2.x, (float)vector1.y * (float)vector2.y}
#define V2F_ADD_V2F(vector1, vector2)(sfVector2f){(float)vector1.x + (float)vector2.x, (float)vector1.y + (float)vector2.y}
#define V2F_SUB_V2F(vector1, vector2)(sfVector2f){(float)vector1.x - (float)vector2.x, (float)vector1.y - (float)vector2.y}

#define V2F_MUL_VALUE(vector, value)(sfVector2f){(float)vector.x * (float)value, (float)vector.y * (float)value}
#define V2F_ADD_VALUE(vector, value)(sfVector2f){(float)vector.x + (float)value, (float)vector.y + (float)value}
#define V2F_SUB_VALUE(vector, value)(sfVector2f){(float)vector.x - (float)value, (float)vector.y - (float)value}


// Strings used in all files
#define BUFFER_EXTRA_SMALL 50 // Used to create a temporary string of extra small size
#define BUFFER_SMALL 250 // Used to create a temporary string of small size
#define BUFFER_MEDIUM 500 // Used to create a temporary string of medium size
#define BUFFER_LARGE 1000 // Used to create a temporary string of large size
#define BUFFER_EXTRA_LARGE 3000 // Used to create a temporary string of extra large size

// Time
#define TO_MS(value) (float)value * 1000.0f
#define TO_S(value) (float)value / 1000.0f

// Xbox buttons used in all files
#define XBOX_A 0
#define XBOX_B 1
#define XBOX_X 2
#define XBOX_Y 3
#define XBOX_LB 4
#define XBOX_RB 5

// Various used in all files
#define SCALE_SPRITE 4.0f
#define JOYSTICK_MINIMUM_SENSITIVITY 25.0f // Value a which the joystick movement will not be detected
#define MAXIMUM_PLAYER_NUMBER 4
#define NB_SPACESHIPS 4
#define NB_COLORS 7
#define NB_SPECIALS 6
#define NB_LEVELS 5
#define FILES_FORMAT ".data"

// Used in Text.h
#define FONTS_NB 10

// Used in "Projectile.h"
#define PROJECTILE_MAXIMUM 3000 // Maximum for players and enemies, thats means maximum by 2
#define PROJECTILE_SPEED 850.0f

// Used in "Player.h"
#define PLAYER_SHIPS_DIR "Data/Players"
#define MAX_PLAYER_NUMBER 4
#define PLAYER_SPEED 505.0f
#define WEAPON_SLOTS_MAX 3
#define PLAYER_DEFAULT_DAMAGE 10.0f
#define PLAYER_LEFT_GUN_X_OFFSET -6.0f
#define PLAYER_LEFT_GUN_Y_OFFSET -6.0f
#define PLAYER_RIGHT_GUN_X_OFFSET 6.0f
#define PLAYER_RIGHT_GUN_Y_OFFSET -6.0f
#define PLAYER_X_VELOCITY_MULTIPLIER 1.6f
#define PLAYER_INVULNERABILITY_TIME 0.05f

// Used in "Weapon.h"
#define WEAPONS_DIR "Data/Weapons"
#define WEAPON_ROTATION_SPEED 12.2f
#define WEAPON_TYPE_MULIPLIER_BAD 0.5f
#define WEAPON_TYPE_MULIPLIER_GOOD 2.0f

// Used in "Turret.h"
#define TURRETS_DIR "Data/Turrets"

// Used in "Explosion.h"
#define EXPLOSION_DIRECTORY "Data/Explosions"
#define EXPLOSION_BUFFER 15

// Used in "Boss.h"
#define BOSS_DIRECTORY "Data/Boss"
#define BOSS_DEFAULT_DAMAGE 100.0f
#define BOSS_TIME_INTRODUCTION 3.0f
#define MUSIC_FADE_RATIO 0.5f

// Used in "Enemy.h"
#define ENEMY_SHIPS_DIR "Data/Enemies"
#define MAX_ENEMY_NUMBER 100
#define ENEMY_SPEED 120.0f
#define ENEMY_INVULNERABILITY_TIME 0.001f

#define DEATH_MULTIPLIER 50

// Used in "Score.h"
#define SCORE_EFFICIENT 20
#define SCORE_INEFFICIENT 5
#define SCORE_NORMAL 10
#define SCORE_GOOD_PERCENTAGE 0.25f
#define SCORE_OFFSET_TEXT -60.0f

// Used in "Money.h"
#define MONEY_OFFSET_TEXT -60.0f

// Used in "Explosion.h"
#define EXPLOSION_INITIALIZATION_NUMBER 5 // The maximum number of explosions at the begining
#define EXPLOSION_REALLOC_OFFSET 3 // The speed a wich the explosions will realoc

// Used in "Game.h"
#define GAME_MAXIMUM_DISTANCE_FROM_BORDERS 100.0f

// Used in "Editor.h" and "Level.h"
#define EDITOR_MAP_POSITION (sfVector2f){ 340.0f, 20.0f }
#define EDITOR_SCREEN_POSITION (sfVector2f){ 456.0f, 324.0f }
#define EDITOR_SCREEN_HEIGHT 432
#define EDITOR_SCREEN_WIDTH 768
#define EDITOR_PARALLAX_MAX 300
#define EDITOR_MAP_SIZE (sfVector2f){ 1000.0f, 1040.0f }
#define EDITOR_SPEED_MULTIPLIER 6.0f
#define EDITOR_MAP_ZOOM 2.5f
#define LEVEL_MAXIMUM_DURATION 360.0f // Max duration of the level in seconds
#define EDITOR_SELECTOR_WIDTH 30
#define EDITOR_SELECTOR_HEIGHT 15
#define EDITOR_INIT_MAX_TERRAIN_REPETITION 120

// Used in "Path.h"
#define PATH_INITIALIZATION_NUMBER 5 // The maximum number of paths at the begining
#define PATH_REALLOC_OFFSET 3 // The speed a wich the spawnpoints will realoc the paths structure
#define SPAWN_DEFAULT_RATE 1.5f // The default rate a wich the enemies will spawn in waypoints

#define SPAWN_ENEMY_NUMBER_MINIMUM 1
#define SPAWN_ENEMY_NUMBER_MAXIMUM 30
#define SPAWN_DEFAULT_ENEMYNAME "Enemy1"
#define SPAWN_DURATION_MINIMUM 0.0f // Minimum duration of the spawn in seconds
#define SPAWN_DURATION_MAXIMUM 240.0f // Maximum duration of the spawn in seconds

// Used in "Level.h"
#define LEVEL_FOLDER "Data/Levels/"

// Used in interfaces files
#define SLIDER_MAXIMUM_NUMBER 10

// Used in Shader.h
#define SHADER_CONTRAST_INTENSITY 10000
#define SHADER_BLURR_INTENSITY 1.2f
#define SHADER_CONTRAST "Data/Shaders/Contrast.shader"
#define SHADER_BLURR "Data/Shaders/Blurr.shader"
#define SHADER_MERGE "Data/Shaders/Merge.shader"
#define SHADER_WHITE "Data/Shaders/ToWhite.shader"
#define SHADER_GREY "Data/Shaders/ToGrey.shader"
#define SHADER_OUTLINE "Data/Shaders/Outline.shader"
#define SHADER_GRAY "Data/Shaders/ToGrayAndColor.shader"
#define SHADER_HEALTH_LIGHT "Data/Shaders/HealthLight.shader"
#define SHADER_SHIP_COLORATION "Data/Shaders/PlayerSpaceship.shader"
#define SHADER_SEE_THROUGH "Data/Shaders/SeeThrough.shader"
#define SHADER_BOSS "Data/Shaders/Boss.shader"

#define SHADER_COLOR_DEFAULT_HIT sfColor_fromRGB(95, 95, 95)
#define SHADER_COLOR_GOOD_HIT sfColor_fromRGB(55, 255, 125)
#define SHADER_COLOR_BAD_HIT sfColor_fromRGB(255, 75, 75)
#define SHADER_HEALTH_SPRITE "Data/Sprites/HealthLight.png"
#define SHADER_BLINKING_THRESHOLD 0.2f

// Used in "Skill.h"
#define SKILL_BUTTON XBOX_A
#define SKILL_DIR "Data/Skills"
#define SKILL_ANIM_CLASS "Skills"

// Used in "Difficulty.h"
#define DIFFICULTY_DIR "Data/Difficulty"

// Used with Shaking
#define SHAKE_POWER 4
#define SHAKE_DURATION 0.05f

// Used by the feedbacks
#define FEEDBACK_TIME_WHITE 0.03f
#define FEEDBACK_TIME_WAIT 0.03f
#define FEEDBACK_TIME_COLORED 0.03f

// Others
#define CINETIC_TYPE 0
#define EXPLOSIVE_TYPE 1
#define ENERGETIC_TYPE 2
#define KEYBOARD_JOYSTICK_ID 4

// Used in "DirectionalShield.h"
#define SHIELD_TIMER_MAX_TO_REGEN 0.5f // In second
#define SHIELD_ANGLE_ROTATION_SPEED 550 // In Deg per second
#define SHIELD_ATTACK_CHARGE_PRICE 10 
#define SHIELD_DISTANCE_UNSCALED 25

// Used in "Difficulty.c"
#define DIF_PLAYER_FOLDER_NAME "PlayersNumber"
#define DIF_GMODE_FOLDER_NAME "GamemodeDifficulty"
#define DIF_STAGE_FOLDER_NAME "Stage"
#define DIF_WEAPON_DMG_MUL_FOLDER_NAME "DamageMultiplierWeapon"
#define DIF_MAX_NB_STAGES 3

// Used in "World.c"
#define WORLD_DIR_PATH "Data/Worlds"

// Used in "AnimatedStarsBackground.c"
#define STAR_DEPTH_MIN 1
#define STAR_DEPTH_NB 4
#define STAR_PARALLAX_POOL_SIZE 2
#define STAR_PARALLAX_MAX 250
#define STAR_PARALLAX_W 640
#define STAR_PARALLAX_H 360
#define STAR_PARALLAX_SPEED 17
#define STAR_TRAIL_MAX 10
#define STAR_TRAIL_DEPTH_MAX 3
#define STAR_TRAIL_START_SPAWN_Y -700
#define STAR_TRAIL_DECAY_TIMER_MIN 0.5f
#define STAR_TRAIL_DECAY_TIMER_MAX 1
#define STAR_TRAIL_COLOR_DECAY_DURATION 0.2f
#define STAR_TRAIL_TIME_TO_APPEAR 0.2f
#define STAR_TRAIL_SPEED_MIN 1300
#define STAR_TRAIL_SPEED_MAX 2200
#define STAR_TRAIL_POOL_SIZE 2
#define SCALE 1
#define BACKGROUND_SPEED 4

// Used in "Achievements.h"
#define ACHIEVEMENTS_DIR "Data/Achievements"
#define ACHIEVEMENTS_SPRITES_DIR "Data/Sprites/Achievements"
#define ACHIEVEMENTS_FILENAME "Achievements"

// Used for Achievement Menu
#define ACHIEVEMENT_MENU_POSITION_X 460
#define ACHIEVEMENT_MENU_POSITION_Y 140
#define ACHIEVEMENT_MENU_SCALE 3.0f
#define ACHIEVEMENT_MENU_INNERSPRITE_OFFSET_X 128 // pixel (unscaled)
#define ACHIEVEMENT_MENU_INNERSPRITE_OFFSET_Y 20 // pixel (unscaled)
#define ACHIEVEMENT_MENU_MARGIN_BORDER_X 25
#define ACHIEVEMENT_MENU_MARGIN_BORDER_Y 35
#define ACHIEVEMENT_MENU_SIZE_X 1000
#define ACHIEVEMENT_MENU_SIZE_Y 800
#define ACHIEVEMENT_MENU_BORDER_MARGIN ACHIEVEMENT_MENU_SIZE_X*0.05f // out of date
#define ACHIEVEMENT_MENU_BORDER_MARGIN_MULTIPLICATOR 0.05f
#define ACHIEVEMENT_MENU_SECTION_SIZE_X ACHIEVEMENT_MENU_SIZE_X - (ACHIEVEMENT_MENU_BORDER_MARGIN*2) // out of date
#define ACHIEVEMENT_MENU_SECTION_MARGIN_TOP 10 // out of date
#define ACHIEVEMENT_MENU_SECTION_INNERSPRITE_OFFSET_X 6 // pixels
#define ACHIEVEMENT_MENU_SECTION_INNERSPRITE_OFFSET_Y 2
#define ACHIEVEMENT_MENU_ICON_OFFSET_X (ACHIEVEMENT_MENU_SECTION_SIZE_X)*0.02f
#define ACHIEVEMENT_MENU_ICON_OFFSET_Y ACHIEVEMENT_MENU_ICON_OFFSET_X
#define ACHIEVEMENT_MENU_TITLE_FONT_SIZE 22
#define ACHIEVEMENT_MENU_DESC_FONT_SIZE 17
#define ACHIEVEMENT_MENU_DESC_OFFSET_MULTIPLICATOR_Y 0.4f
#define ACHIEVEMENT_MENU_TEXT_MARGIN_TOP 9
#define ACHIEVEMENT_MENU_TEXT_MARGIN_LEFT 15
#define ACHIEVEMENT_MENU_BUTTON_LEFT_POSITION_MULTIPLICATOR_X 0.25f
#define ACHIEVEMENT_MENU_BUTTON_LEFT_POSITION_MULTIPLICATOR_Y 0.10f
#define ACHIEVEMENT_MENU_BUTTON_ARROW_LEFT_SIZE_X 22 // pixel
#define ACHIEVEMENT_MENU_BUTTON_INPUT_LEFT_SIZE_X 13 // pixel
#define ACHIEVEMENT_MENU_BUTTON_INPUT_LEFT_SIZE_Y 11 // pixel
#define ACHIEVEMENT_MENU_BUTTON_RIGHT_POSITION_MULTIPLICATOR_X 0.75f
#define ACHIEVEMENT_MENU_BUTTON_RIGHT_POSITION_MULTIPLICATOR_Y 0.10f
#define ACHIEVEMENT_MENU_BUTTON_INPUT_RIGHT_SIZE_X ACHIEVEMENT_MENU_BUTTON_INPUT_LEFT_SIZE_X // pixels
#define ACHIEVEMENT_MENU_BUTTON_INPUT_RIGHT_SIZE_Y ACHIEVEMENT_MENU_BUTTON_INPUT_LEFT_SIZE_Y // pixels
#define ACHIEVEMENT_MENU_BUTTON_BACKTM_POSITION_MULTIPLICATOR_X 0.60f
#define ACHIEVEMENT_MENU_BUTTON_BACKTM_POSITION_MULTIPLICATOR_Y 0.87f
#define ACHIEVEMENT_MENU_BUTTON_BACKTM_SIZE_X 86 // pixels


// Used in "Shadow.c"
#define SHADOW_OFFSET_X 25
#define SHADOW_OFFSET_Y 25
#define SHADOW_SCALE 0.75f


// Used in Map and Editor
#define MAP_TRIGGER_REPEATED_TERRAIN_INIT_AT_Y -500
#define MAP_TERRAIN_REPETITION_NB 3

// Used in "Notification.c"
#define NOTIFICATION_MAX 20
#define NOTIFICATION_ACHIEVEMENT_SHAKE_POWER 10
#define NOTIFICATION_ACHIEVEMENT_SHAKE_DURATION 0.8f
#define NOTIFICATION_ACHIEVEMENT_FLASH_DURATION 0.1f
#define NOTIFICATION_ACHIEVEMENT_ALPHA_DEFAULT 175
#define NOTIFICATION_ACHIEVEMENT_POSITION_Y_MARGIN 75
#define NOTIFICATION_ACHIEVEMENT_ICON_OFFSET_X 10
#define NOTIFICATION_ACHIEVEMENT_ICON_OFFSET_Y NOTIFICATION_ACHIEVEMENT_ICON_OFFSET_X
#define NOTIFICATION_ACHIEVEMENT_TITLE_FONT_SIZE 20 
#define NOTIFICATION_ACHIEVEMENT_TITLE_MARGIN_LEFT 10 
#define NOTIFICATION_ACHIEVEMENT_TITLE_OFFSET_Y NOTIFICATION_ACHIEVEMENT_ICON_OFFSET_Y
#define NOTIFICATION_ACHIEVEMENT_DESC_FONT_SIZE 17
#define NOTIFICATION_ACHIEVEMENT_DESC_MARGIN_LEFT NOTIFICATION_ACHIEVEMENT_TITLE_MARGIN_LEFT
#define NOTIFICATION_ACHIEVEMENT_DESC_OFFSET_Y 12
#define NOTIFICATION_ACHIEVEMENT_TTL 5.0f // Time To Live
#define NOTIFICATION_ACHIEVEMENT_TTD 1.0f // Time To Disapear
#define NOTIFICATION_ACHIEVEMENT_SCALE 2.5f 

// Used in "Fade.c"
#define FADE_TIME_TO_IN 0.13f
#define FADE_TIME_TO_OUT FADE_TIME_TO_IN

// Used in "Game_Initialize.c"
#define INTRO_DURATION 3.0f
#define GAME_MUSIC_LEVEL_FILEPATH "Data/Musics/InGame/Music_Level/"

// Used in "Game_Update.c"
#define GAME_SCOREBOARD_TIMER_MAX 4.0f
#define GAME_DROP_RATE 20

#endif // !DEFINES__H
