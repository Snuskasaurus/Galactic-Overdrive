#include "Text.h"

// -- SHARED FUNCTIONS ----------------------------------------------------- //

TextEngine *Text_InitializeEngine()
{
	TextEngine *engine;

	engine = calloc(1, sizeof(TextEngine));
	engine->nbFonts = 0;

	return engine;
}

sfFont *Text_GetFont(TextEngine *_engine, const char *_filename)
{
	char filePath[BUFFER_SIZE] = FONT_FOLDER;
	Font *newFont = NULL;

	// Explore all the fonts and return the font
	for (int i = 0; i < _engine->nbFonts; i++)
	{
		if (_engine->fonts[i].name)
		{
			if (_stricmp(_filename, _engine->fonts[i].name) == 0)
			{
				return _engine->fonts[i].pointer;
			}
		}
	}
	// Add the font if none was found with this name
	if (_engine->nbFonts < FONTS_NB)
	{
		strcat(filePath, _filename);
		printf("Adding a font : %s \n", filePath);
		_engine->fonts[_engine->nbFonts].pointer = sfFont_createFromFile(filePath);
		_engine->fonts[_engine->nbFonts].name = _strdup(_filename);
		_engine->nbFonts++;
		return _engine->fonts[_engine->nbFonts - 1].pointer;
	}
	else
	{
		printf("Maximum of fonts has been reached\n");
		return NULL;
	}
}

sfVector2i Text_GetSize(sfText *_text)
{
	sfVector2i size;
	sfFloatRect rect;

	rect = sfText_getLocalBounds(_text);
	size.x = (int)rect.width;
	size.y = (int)rect.height;

	return size;
}

//sfText *Text_Create(const char *_content, sfFont *_font, int _characterSize, sfVector2f _origin)
//{
//	sfText *text = NULL;
//	sfVector2i sizeText;
//	float offset = 0.0f;
//
//	text = sfText_create();
//	if (text)
//	{
//		sfText_setFont(text, _font);
//		sfText_setString(text, _content);
//		sfText_setCharacterSize(text, _characterSize);
//		sfText_setPosition(text, (sfVector2f) { 0.0f, 0.0f });
//		sfText_setColor(text, sfWhite);
//		// Set the origin
//		sizeText = Text_GetSize(text);
//		offset = sfText_getLocalBounds(text).top;
//		sfText_setOrigin(text, (sfVector2f) { sizeText.x * _origin.x, sizeText.y * _origin.y + offset});
//	}
//
//	return text;
//}

//void Text_Display(sfRenderTexture *_canvas, sfText *_text, sfVector2f _position, sfColor _color)
//{
//	sfText_setPosition(_text, _position);
//	sfText_setColor(_text, _color);
//	sfRenderTexture_drawText(_canvas, _text, NULL);
//}

void Text_Display(sfRenderTexture *_canvas, Text *_text, const char *_string, sfVector2f _position, sfVector2f _origin)
{
	sfVector2i sizeText;
	float offset = 0.0f;
	static sfText *text = NULL;
	if (!text)
	{
		text = sfText_create();
	}
	sfText_setFont(text, _text->font);
	sfText_setString(text, _string);
	sfText_setCharacterSize(text, _text->size);
	sfText_setColor(text, _text->color);
	sfText_setPosition(text, _position);
	// Set the origin
	sizeText = Text_GetSize(text);
	offset = sfText_getLocalBounds(text).top;
	sfText_setOrigin(text, (sfVector2f) { sizeText.x * _origin.x, sizeText.y * _origin.y + offset });
	sfRenderTexture_drawText(_canvas, text, NULL);
}

void Text_DisplayOnCanvas()
{

}