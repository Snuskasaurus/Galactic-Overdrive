#include "Sprite.h"

// -- FUNCTIONS -------------------------------------------------------------- //

void Sprite_SetOrigin(sfSprite *_sprite, float _xOrigin, float _yOrigin)
{
	const sfTexture *texture;
	sfVector2u textureSize;
	sfVector2f origin;

	// Get the texture
	texture = sfSprite_getTexture(_sprite);
	// Set the sprite origin
	if (_xOrigin && _yOrigin)
	{
		if (_xOrigin > 1.0f)
		{
			_xOrigin = 1.0f;
		}
		if (_yOrigin > 1.0f)
		{
			_yOrigin = 1.0f;
		}
		textureSize = sfTexture_getSize(texture);
		origin.x = (float)textureSize.x * _xOrigin;
		origin.y = (float)textureSize.y * _yOrigin;
		sfSprite_setOrigin(_sprite, origin);
	}
}

// Create a sprite from a file
sfSprite *Sprite_CreateFromFile(const char *_filename, float _xOrigin, float _yOrigin)
{
	sfSprite *sprite;
	sfTexture *texture;

	// Load the texture from the file
	texture = sfTexture_createFromFile(_filename, NULL);
	if (texture)
	{
		// Create the sprite and assign the texture
		sprite = sfSprite_create();
		sfSprite_setTexture(sprite, texture, sfFalse);
		Sprite_SetOrigin(sprite, _xOrigin, _yOrigin);

		return sprite;
	}
	else
	{
		return NULL;
	}
	
}

// Create a sprite from an image
sfSprite *Sprite_CreateFromImage(sfImage *_image, float _xOrigin, float _yOrigin)
{
	sfSprite *sprite;
	sfTexture *texture;

	// Load the texture from the image
	texture = sfTexture_createFromImage(_image, NULL);
	// Create the sprite and assign the texture
	sprite = sfSprite_create();
	sfSprite_setTexture(sprite, texture, sfFalse);
	Sprite_SetOrigin(sprite, _xOrigin, _yOrigin);

	return sprite;
}

// Update a sprite texture from an new image
void Sprite_UpdateFromImage(sfSprite *_sprite, sfImage *_image)
{
	const sfTexture *constTexture = NULL;
	sfTexture *texture = NULL;

	constTexture = sfSprite_getTexture(_sprite);
	texture = constTexture;
	sfTexture_updateFromImage(texture, _image, 0, 0);
}

// Display a sprite
void Sprite_Display(sfRenderTexture *_canvas, sfSprite *_sprite, sfVector2f _position, sfVector2f _size, float _angle, sfRenderStates *_renderState)
{
	static sfSprite *nullSprite = NULL;
	if (_sprite)
	{
		sfSprite_setPosition(_sprite, _position);
		sfSprite_setRotation(_sprite, _angle);
		sfSprite_setScale(_sprite, _size);
		sfRenderTexture_drawSprite(_canvas, _sprite, _renderState);
	}
}

sfVector2f Sprite_GetSize(sfSprite *_sprite)
{
	const sfTexture *texture = sfSprite_getTexture(_sprite);
	sfVector2u scale = sfTexture_getSize(texture);
	return (sfVector2f) { (float)scale.x, (float)scale.y };
}

void Texture_DrawRenderTexture(sfRenderWindow *_window, sfRenderTexture *_canvas, sfRenderStates *_shader)
{
	static sfSprite* sprite = NULL;

	if (!sprite)
	{
		sprite = sfSprite_create();
	}
	sfRenderTexture_display(_canvas);
	sfSprite_setTexture(sprite, sfRenderTexture_getTexture(_canvas), sfTrue);
	sfRenderWindow_drawSprite(_window, sprite, _shader);
	sfRenderTexture_clear(_canvas, sfTransparent);
}

void Canvas_ApplyShader(sfRenderTexture *_canvas, sfRenderStates *_shader)
{
	static sfSprite* sprite = NULL;

	if (!sprite)
	{
		sprite = sfSprite_create();
	}
	///printf("size of the canvas : %u %u\n", sfRenderTexture_getSize(_canvas).x, sfRenderTexture_getSize(_canvas).y);
	sfRenderTexture_display(_canvas);
	sfSprite_setTexture(sprite, sfRenderTexture_getTexture(_canvas), sfTrue);
	sfRenderTexture_drawSprite(_canvas, sprite, _shader);
	sfRenderTexture_display(_canvas);
}

sfRenderTexture *Canvas_Copy(sfRenderTexture *_canvas, sfVector2u _size, sfRenderStates *_shader)
{
	static sfSprite* sprite = NULL;
	sfRenderTexture* copy = sfRenderTexture_create(_size.x, _size.y, sfFalse);
	if (!sprite)
	{
		sprite = sfSprite_create();
	}
	sfRenderTexture_display(_canvas);
	sfSprite_setTexture(sprite, sfRenderTexture_getTexture(_canvas), sfTrue);
	sfRenderTexture_drawSprite(copy, sprite, _shader);
	sfRenderTexture_display(copy);
	return copy;
}

void Sprite_DisplayLine(sfRenderTexture *_canvas, sfVector2f positions[], sfColor color)
{
	static sfRectangleShape *shape = NULL;
	float angle;
	float distance;
	if (!shape)
	{
		shape = sfRectangleShape_create();
	}
	sfRectangleShape_setFillColor(shape, color);
	sfRectangleShape_setPosition(shape, positions[0]);
	angle = Angle_DegGetAngle(positions[0], positions[1]);
	distance = Angle_CalculateDistance(positions[0], positions[1]);
	sfRectangleShape_setSize(shape, V2F(distance, 2.0f));
	sfRectangleShape_setRotation(shape, angle);
	sfRenderTexture_drawRectangleShape(_canvas, shape, NULL);
}