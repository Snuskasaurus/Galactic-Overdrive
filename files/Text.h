#ifndef TEXT__H
#define TEXT__H

// -- INCLUDES ------------------------------------------------------------- //

#include "_Includes.h"

// -- TYPES DEFINITIONS ---------------------------------------------------- //

// -- ENUMERATIONS --------------------------------------------------------- //

// -- DEFINES -------------------------------------------------------------- //

#define BUFFER_SIZE 1000
#define FONT_DEFAULT "Arial.ttf"
#define FONT_FOLDER "Data/Fonts/"

// -- STRUCTURES ----------------------------------------------------------- //

// -- SHARED FUNCTIONS DEFINITIONS ----------------------------------------- //

TextEngine *Text_InitializeEngine();

sfFont *Text_GetFont(TextEngine *_engine, const char *_filename);

void Text_Display(sfRenderTexture *_canvas, Text *_text, const char *_string, sfVector2f _position, sfVector2f _origin);

#endif // !TEXT__H