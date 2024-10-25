#include "Notification.h"

void Notification_Create(Notifications* _notifications, Engine* _engine, void* _data, Notification_Type _dataType)
{
	if (_notifications->number < NOTIFICATION_MAX)
	{
		if (_dataType == NOTIFY_ACHIEVEMENT)
		{
			Notification* currentNotif = &_notifications->arrNotifs[_notifications->number];
			Achievement* achievement = _data;
			sfVector2u backgroundSize;
			sfColor color = { 255,255,255,NOTIFICATION_ACHIEVEMENT_ALPHA_DEFAULT };

			currentNotif->dataType = _dataType;
			currentNotif->data = (Achievement*)_data;
			currentNotif->background = Sprite_CreateFromFile("Data/Sprites/Interfaces/Achievement/Cadre_Notif_Achievement.png", 0, 0);
			backgroundSize = sfTexture_getSize(sfSprite_getTexture(currentNotif->background));
			currentNotif->color = color;
			currentNotif->timeToLive = NOTIFICATION_ACHIEVEMENT_TTL;
			currentNotif->timeToDisapear = NOTIFICATION_ACHIEVEMENT_TTD;

			// -- Init Text parameters
			achievement->parametersTextTitle.size = NOTIFICATION_ACHIEVEMENT_TITLE_FONT_SIZE * _engine->configuration->resolution.x / 1920;
			achievement->parametersTextDesc.size = NOTIFICATION_ACHIEVEMENT_DESC_FONT_SIZE * _engine->configuration->resolution.x / 1920;

			// -- Init entity
			currentNotif->entity.size.x = (float)backgroundSize.x;
			currentNotif->entity.size.y = (float)backgroundSize.y;
			currentNotif->entity.position.x = _engine->configuration->resolution.x / 2 - (currentNotif->entity.size.x*NOTIFICATION_ACHIEVEMENT_SCALE) / 2;
			currentNotif->entity.position.y = _engine->configuration->resolution.y - (currentNotif->entity.size.y*NOTIFICATION_ACHIEVEMENT_SCALE) - NOTIFICATION_ACHIEVEMENT_POSITION_Y_MARGIN;
			Entity_CreateShaking(&currentNotif->entity.shake, NOTIFICATION_ACHIEVEMENT_SHAKE_POWER, NOTIFICATION_ACHIEVEMENT_SHAKE_DURATION);

			_notifications->number++;
		}
		else
		{
			printf("Notification_Create() Error : Wrong dataType\n");
		}
	}
}

void Notification_Init(Notifications* _notifications)
{
	for (int i = 0; i < NOTIFICATION_MAX; i++)
	{
		Notification* currentNotif = &_notifications->arrNotifs[i];
		currentNotif->dataType = NOTIFY_NO_DATA;
	}
}

void Notification_Clean(Notification* _notifToClean, Notifications* _notifications)
{
	Notification cleanNotif = { 0 };

	// Destroy sprite
	sfSprite_destroy(_notifToClean->background);
	// Replace the expired notification by the last one
	*_notifToClean = _notifications->arrNotifs[_notifications->number - 1];
	// The last notif is cleaned
	_notifications->arrNotifs[_notifications->number - 1] = cleanNotif;
	// Decrease the number of notifs
	_notifications->number--;
}

void Notification_UpdateAchievement(Notification* _notification, const float _dt)
{
	Achievement* achievement = (Achievement *)_notification->data;
	// Update alpha
	if (_notification->timeToDisapear < NOTIFICATION_ACHIEVEMENT_TTD)
	{
		_notification->color.a = (char)(NOTIFICATION_ACHIEVEMENT_ALPHA_DEFAULT - (NOTIFICATION_ACHIEVEMENT_TTD - _notification->timeToDisapear) * NOTIFICATION_ACHIEVEMENT_ALPHA_DEFAULT);

	}
	achievement->parametersTextDesc.color.a = _notification->color.a;
	achievement->parametersTextTitle.color.a = _notification->color.a;

	_notification->entity.shake.shakingPower = (int)(1 + (NOTIFICATION_ACHIEVEMENT_SHAKE_DURATION - _notification->entity.shake.timer) * NOTIFICATION_ACHIEVEMENT_SHAKE_POWER);

	Entity_DoShake(&_notification->entity.shake, _dt);
}

void Notification_Update(Notifications* _notifications, Engine* _engine)
{
	// -- Update notifications
	for (int i = 0; i < _notifications->number; i++)
	{
		if (_notifications->arrNotifs[i].dataType != NOTIFY_NO_DATA)
		{
			Notification* currentNotif = &_notifications->arrNotifs[i];

			// Decrease life time
			currentNotif->timeToLive -= _engine->dt;
			if (currentNotif->timeToLive < 0)
			{
				currentNotif->timeToDisapear -= _engine->dt;
			}

			if (_notifications->arrNotifs[i].dataType == NOTIFY_ACHIEVEMENT)
			{
				Notification_UpdateAchievement(currentNotif, _engine->dt);
			}
		}
	}

	// -- Clean expired notifications
	for (int i = 0; i < _notifications->number; i++)
	{
		Notification* currentNotif = &_notifications->arrNotifs[i];
		if (currentNotif->timeToDisapear < 0)
		{
			Notification_Clean(currentNotif, _notifications);
		}
	}
}

void Notification_DisplayAchievement(Notifications* _notifications, Engine* _engine, int _indexAchievement)
{
	Notification* currentNotification = &_notifications->arrNotifs[_indexAchievement];
	Achievement* achievement = (Achievement*)currentNotification->data;
	sfRenderStates* shader = NULL;
	float scale = NOTIFICATION_ACHIEVEMENT_SCALE;
	// Set position with shaking offset
	sfVector2f tempPosition = V2F(
		currentNotification->entity.position.x + currentNotification->entity.shake.shakingOffset.x,
		currentNotification->entity.position.y + currentNotification->entity.shake.shakingOffset.y);

	// -- Apply flash effet
	//if (currentNotification->timeToLive > NOTIFICATION_ACHIEVEMENT_TTL - NOTIFICATION_ACHIEVEMENT_FLASH_DURATION)
	//{
	//	shader = Shader_ToWhite();
	//}


	// -- Icon
	/// Set position
	sfVector2f iconPosition = V2F(
		tempPosition.x + (ACHIEVEMENT_MENU_SECTION_INNERSPRITE_OFFSET_X)*scale,
		tempPosition.y + ACHIEVEMENT_MENU_SECTION_INNERSPRITE_OFFSET_Y * scale);

	sfSprite_setColor(achievement->icon_Sprite, currentNotification->color);
	Sprite_Display(_engine->canvas, achievement->icon_Sprite, iconPosition, V2F_FROM_VALUE(scale), 0, shader);

	// -- Background
	sfSprite_setColor(currentNotification->background, currentNotification->color);
	/// Set position with shaking offset
	Sprite_Display(_engine->canvas, currentNotification->background, tempPosition, V2F_FROM_VALUE(scale), 0, shader);

	// -- Title
	/// Set position
	sfVector2f titlePosition = V2F(
		tempPosition.x + (achievement->icon_Size.x + ACHIEVEMENT_MENU_SECTION_INNERSPRITE_OFFSET_X + ACHIEVEMENT_MENU_TEXT_MARGIN_LEFT) * NOTIFICATION_ACHIEVEMENT_SCALE,
		tempPosition.y + ACHIEVEMENT_MENU_TEXT_MARGIN_TOP * NOTIFICATION_ACHIEVEMENT_SCALE);
	Text_Display(_engine->canvas, &achievement->parametersTextTitle, achievement->title, titlePosition, V2F(0, 0));

	// -- Desc
	sfVector2f descPosition = V2F(
		titlePosition.x,
		tempPosition.y + (currentNotification->entity.size.y*NOTIFICATION_ACHIEVEMENT_SCALE)/2);
	
	Text_Display(_engine->canvas, &achievement->parametersTextDesc, achievement->description, descPosition, V2F(0, 0));

}

void Notification_Display(Notifications* _notifications, Engine* _engine)
{
	for (int i = 0; i < _notifications->number; i++)
	{
		if (_notifications->arrNotifs[i].dataType != NOTIFY_NO_DATA)
		{
			if (_notifications->arrNotifs[i].dataType == NOTIFY_ACHIEVEMENT)
			{
				Notification_DisplayAchievement(_notifications, _engine, i);
			}
			else
			{
				printf("Notification_Display Error : Wrong data type");
			}
		}
	}
}