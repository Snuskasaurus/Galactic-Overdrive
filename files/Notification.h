#ifndef NOTIFICATION_H
#define NOTIFICATION_H 

// -- INCLUDES ------------------------------------------------------------- //

#include "_Engine.h"


// -- FUNCTIONS ------------------------------------------------------------ //

void Notification_Create(Notifications* _notifications, Engine* _engine, void* _data, Notification_Type _dataType);
void Notification_Init(Notifications* _notifications);
void Notification_Update(Notifications* _notifications, Engine* _engine);
void Notification_Display(Notifications* _notifications, Engine* _engine);

#endif // !NOTIFICATION_H