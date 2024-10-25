#include "Shaders.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

// -- MAIN FUNCTION -------------------------------------------------------- //

sfRenderStates *Shader_ShipColoration()
{
	static sfShader *shader = NULL;
	static sfRenderStates tempState;
	if (!shader)
	{
		shader = sfShader_createFromFile(NULL, NULL, SHADER_SHIP_COLORATION);
		tempState.shader = shader;
		tempState.blendMode = sfBlendAlpha;
		tempState.transform = sfTransform_Identity;
		tempState.texture = NULL;
	}
	return &tempState;
}

sfRenderStates *Shader_ToWhite()
{
	static sfShader *shader = NULL;
	static sfRenderStates tempState;
	if (!shader)
	{
		shader = sfShader_createFromFile(NULL, NULL, SHADER_WHITE);
		tempState.shader = shader;
		tempState.blendMode = sfBlendAlpha;
		tempState.transform = sfTransform_Identity;
		tempState.texture = NULL;
	}
	return &tempState;
}

sfRenderStates *Shader_ShadowEffect()
{
	static sfShader *shader = NULL;
	static sfRenderStates tempState;
	if (!shader)
	{
		shader = sfShader_createFromFile(NULL, NULL, SHADER_GREY);
		tempState.shader = shader;
		tempState.blendMode = sfBlendAlpha;
		tempState.transform = sfTransform_Identity;
		tempState.texture = NULL;
	}
	return &tempState;
}

sfRenderStates *Shader_SeeThrough()
{
	static sfShader *shader = NULL;
	static sfRenderStates tempState;
	if (!shader)
	{
		shader = sfShader_createFromFile(NULL, NULL, SHADER_SEE_THROUGH);
		tempState.shader = shader;
		tempState.blendMode = sfBlendAlpha;
		tempState.transform = sfTransform_Identity;
		tempState.texture = NULL;
	}
	return &tempState;
}

sfRenderStates *Shader_TakingDamage(Hit_Type _type)
{
	static sfShader *shader = NULL;
	static sfRenderStates tempState;
	sfColor color;
	if (!shader)
	{
		shader = sfShader_createFromFile(NULL, NULL, SHADER_GRAY);
		tempState.shader = shader;
		tempState.blendMode = sfBlendAlpha;
		tempState.transform = sfTransform_Identity;
		tempState.texture = NULL;
	}
	switch (_type)
	{
		case HIT_DEFAULT: color = SHADER_COLOR_DEFAULT_HIT;  break;
		case HIT_GOOD: color = SHADER_COLOR_GOOD_HIT; break;
		case HIT_BAD: color = SHADER_COLOR_BAD_HIT; break;
	}
	sfShader_setColorUniform(shader, "color", color);
	return &tempState;
}

sfRenderStates *Shader_PlayerSpaceship(int _health, int _maxHealth, sfColor _textureColor, sfBool _seeThrough, float _time)
{
	static sfShader *shader = NULL;
	static sfRenderStates tempState;
	static sfImage *healthImage = NULL;
	static sfSprite *healthSprite = NULL;
	static sfVector2f spriteSize;
	static sfColor cockpitColor;
	float healthRatio = (float)_health / (float)_maxHealth;
	int pixelX = (int)(spriteSize.x * healthRatio) - 1;

	if (healthSprite == NULL)
	{
		healthImage = sfImage_createFromFile(SHADER_HEALTH_SPRITE);
		healthSprite = Sprite_CreateFromImage(healthImage, 0, 0);
		spriteSize = Sprite_GetSize(healthSprite);
	}

	if (pixelX < 0 || pixelX > 100)
	{
		pixelX = 0;
	}

	cockpitColor = sfImage_getPixel(healthImage, pixelX, 0);

	if (!shader)
	{
		shader = sfShader_createFromFile(NULL, NULL, SHADER_SHIP_COLORATION);
		tempState.shader = shader;
		tempState.blendMode = sfBlendAlpha;
		tempState.transform = sfTransform_Identity;
		tempState.texture = NULL;
	}
	sfShader_setColorUniform(shader, "cockpitColor", cockpitColor);
	sfShader_setColorUniform(shader, "textureColor", _textureColor);
	sfShader_setIntUniform(shader, "seeThrough", (int)_seeThrough);

	if ((float)_health > (float)_maxHealth * SHADER_BLINKING_THRESHOLD)
	{
		// Stop blinking effect
		_time = 0.f;
	}
	sfShader_setFloatUniform(shader, "time", _time);

	return &tempState;
}

sfRenderStates *Shader_Contrast(int _intensity)
{
	static sfShader *shader = NULL;
	static sfRenderStates tempState;
	if (!shader)
	{
		shader = sfShader_createFromFile(NULL, NULL, SHADER_CONTRAST);
		tempState.shader = shader;
		tempState.blendMode = sfBlendAlpha;
		tempState.transform = sfTransform_Identity;
		tempState.texture = NULL;
	}
	sfShader_setIntUniform(shader, "intensity", 12);
	return &tempState;
}

sfRenderStates *Shader_Blurr(sfVector2f size, float _intensity)
{
	static sfShader *shader = NULL;
	static sfRenderStates tempState;
	if (!shader)
	{
		shader = sfShader_createFromFile(NULL, NULL, SHADER_BLURR);
		tempState.shader = shader;
		tempState.blendMode = sfBlendAlpha;
		tempState.transform = sfTransform_Identity;
		tempState.texture = NULL;
	}
	sfShader_setFloatUniform(shader, "width", 1.0f / size.x);
	sfShader_setFloatUniform(shader, "height", 1.0f / size.y);
	sfShader_setFloatUniform(shader, "intensity", _intensity);
	return &tempState;
}

void Shader_ApplyBloomToCanvas(sfRenderTexture *_canvas)
{
	sfRenderTexture *copy;
	sfVector2u sizeCanvas;
	static sfShader *shader = NULL;
	static sfRenderStates renderState;

	sizeCanvas = sfRenderTexture_getSize(_canvas);
	// Copy the canvas to create a contrasted and blurred canvas
	copy = Canvas_Copy(_canvas, sizeCanvas, Shader_Contrast(SHADER_CONTRAST_INTENSITY));
	Canvas_ApplyShader(copy, Shader_Blurr(V2F_FROM_V2I(sizeCanvas), SHADER_BLURR_INTENSITY));
	// Initialize the merge shader
	if (!shader)
	{
		shader = sfShader_createFromFile(NULL, NULL, SHADER_MERGE);
		renderState.shader = shader;
		renderState.blendMode = sfBlendAlpha;
		renderState.transform = sfTransform_Identity;
		renderState.texture = NULL;
	}
	// merge the copy with the current canvas
	sfShader_setTextureUniform(shader, "copy", sfRenderTexture_getTexture(copy));
	Canvas_ApplyShader(_canvas, &renderState);
	// Destroy the copy to avoid memory leaks
	sfRenderTexture_destroy(copy);
}

sfRenderStates *Shader_Boss(sfColor *_colors, int _nbColors, sfTexture *_mask)
{
	static sfShader *shader = NULL;
	static sfRenderStates tempState;
	if (!shader)
	{
		shader = sfShader_createFromFile(NULL, NULL, SHADER_BOSS);
		tempState.shader = shader;
		tempState.blendMode = sfBlendAlpha;
		tempState.transform = sfTransform_Identity;
		tempState.texture = NULL;
	}
	if (_mask)
	{
		// Send colors
		for (int i = 0; i < 4; i++)
		{
			char nameColor[BUFFER_SMALL];
			sprintf(nameColor, "color%d", i);
			sfColor colorToSend = sfWhite;
			if (i < _nbColors)
			{
				colorToSend = _colors[i];
			}
			sfShader_setColorUniform(shader, nameColor, colorToSend);
		}
		// Send the mask
		sfShader_setTextureUniform(shader, "mask", _mask);
		return &tempState;
	}
	else
	{
		return NULL;
	}
}


// -- LOCAL FUNCTIONS ------------------------------------------------------ //