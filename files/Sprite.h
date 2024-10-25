#ifndef SPRITE__H
#define SPRITE__H

// -- INCLUDES ------------------------------------------------------------- //

#include "_Includes.h"

// -- DEFINES -------------------------------------------------------------- //

#define SPRITE_FOLDER "Data/Sprites/"
#define SPRITE_DEBUG "Data/Sprites/Debug.png"

// -- PROTOTYPES ----------------------------------------------------------- //


void Sprite_SetOrigin(sfSprite *_sprite, float _xOrigin, float _yOrigin);

// Create a sprite from a file
sfSprite *Sprite_CreateFromFile(const char *_filename, float _xOrigin, float _yOrigin);

// Create a sprite from an image
sfSprite *Sprite_CreateFromImage(sfImage *_image, float _xOrigin, float _yOrigin);

// Update a sprite texture from an new image
void Sprite_UpdateFromImage(sfSprite *_sprite, sfImage *_image);

// Display a sprite
void Sprite_Display(sfRenderTexture *_canvas, sfSprite *_sprite, sfVector2f _position, sfVector2f _size, float _angle, sfRenderStates *_renderState);

sfVector2f Sprite_GetSize(sfSprite *_sprite);

void Texture_DrawRenderTexture(sfRenderWindow *_window, sfRenderTexture *_canvas, sfRenderStates *_shader);

void Canvas_ApplyShader(sfRenderTexture *_canvas, sfRenderStates *_shader);

sfRenderTexture *Canvas_Copy(sfRenderTexture *_canvas, sfVector2u _size, sfRenderStates *_shader);

void Sprite_DisplayLine(sfRenderTexture *_canvas, sfVector2f positions[], sfColor color);

#endif //!SPRITE__H