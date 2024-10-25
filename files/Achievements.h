#ifndef ACHIEVEMENTS_H
#define ACHIEVEMENTS_H 

// -- INCLUDES ------------------------------------------------------------- //

#include "_Engine.h"
#include "_MainData.h"

#include "Notification.h"

// -- FUNCTIONS ------------------------------------------------------------ //

BP_Achievements Achievements_LoadBlueprints(MainData* _data);
void Achievement_Unclock(Notifications* _notifications, Engine* _engine, const char* _ID_Name);
void Achievement_UpdateStageAchievement(MainData* _data);
void Achievement_UpdateToUnlock(MainData* _data);
void Achievement_GameInit(MainData* _data);
void Achievement_Load(Engine* _engine, const char* _filename);



#endif // !ACHIEVEMENTS_H
