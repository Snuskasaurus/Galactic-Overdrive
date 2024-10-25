#include "Animation.h"

// -- FUNCTIONS -------------------------------------------------------------- //

Anim CreateAnimation(const char *_directory, float _maxTime)
{
	char repository[BUFFER_LARGE] = { 0 };
	Anim animation = { 0 };
	animation.maxTime = _maxTime;
	animation.fileNames = ExploreDirectory(_directory);
	animation.sprites = calloc(animation.fileNames.nbrFiles, sizeof(sfSprite*));
	animation.nbrFrames = animation.fileNames.nbrFiles;

	for (int n = 0; n < animation.fileNames.nbrFiles; n++)
	{
		strcpy(repository, _directory);
		strcat(repository, "/");
		strcat(repository, animation.fileNames.fileNames[n]);
		animation.sprites[n] = Sprite_CreateFromFile(repository, 0.5f, 0.5f);
	}

	return animation;
}

Animations *LoadAnimations(const char *_directory)
{
	Animations *animations = calloc(1, sizeof(Animations));
	ExploDir dirExplored = ExploreDirectory(_directory);
	char filePath[BUFFER_MEDIUM] = { 0 };
	char line[BUFFER_MEDIUM] = { 0 };
	animations->nbAnims = 0;

	printf("Initialization of the animations : \n");
	if (animations != NULL)
	{
		animations->array = calloc(dirExplored.nbrFiles, sizeof(Anim*));
		animations->nbAnimPerName = calloc(dirExplored.nbrFiles, sizeof(int));
		animations->nbAnims = dirExplored.nbrFiles;

		if (animations->array != NULL)
		{
			for (int i = 0; i < dirExplored.nbrFiles; i++)
			{
				FILE *file = NULL;
				char name[BUFFER_MEDIUM] = { 0 };

				strcpy(filePath, _directory);
				strcat(filePath, "/");
				strcat(filePath, dirExplored.fileNames[i]);

				file = fopen(filePath, "rt");
				if (file != NULL)
				{
					animations->nbAnimPerName[i] = 0;
					fscanf(file, "NAME %s\n", name);
					printf("(%d/%d)\t%s\n", i + 1, dirExplored.nbrFiles, name);

					while (!feof(file))
					{
						fscanf(file, "%[^\n]\n", line);
						animations->nbAnimPerName[i]++;
					}

					rewind(file);

					animations->array[i] = calloc(animations->nbAnimPerName[i], sizeof(Anim));

					fscanf(file, "%[^\n]\n", line);

					for (int j = 0; j < animations->nbAnimPerName[i]; j++)
					{
						char repository[BUFFER_MEDIUM] = { 0 };
						char state[BUFFER_MEDIUM] = { 0 };
						float maxTime = 0;
						int loop = 0;
						fscanf(file, "STATE %s MAX_TIME %f LOOP %d DIR %s\n", state, &maxTime, &loop, repository);
						animations->array[i][j] = CreateAnimation(repository, maxTime);
						strcpy(animations->array[i][j].state, state);
						printf("\t - %s\n", state);
						animations->array[i][j].name = _strdup(name);
						animations->array[i][j].loop = loop;
						//printf("%s %s\n", shipAnimations->animations[i][j].name, shipAnimations->animations[i][j].state);
					}
					fclose(file);
				}
			}
		}
	}
	printf("\n");
	return animations;
}

Anim *GetAnim(Animations *_anims, char *_name, char *_state)
{
	Anim *anim = NULL;
	
	for (int i = 0; i < _anims->nbAnims; i++)
	{
		if (strcmp(_anims->array[i][0].name, _name) == 0)
		{
			for (int j = 0; j < _anims->nbAnimPerName[i]; j++)
			{
				if (strcmp(_anims->array[i][j].state, _state) == 0)
				{
					return &_anims->array[i][j];
				}
			}
		}
	}
	return anim;
}

void InitEntityAnim(Entity *_entity, float _maxTime)
{
	_entity->currentFrame = 0;
	_entity->animationTimer = Timer_Create(_maxTime);
}

void ResetAnim(Entity *_entity)
{
	_entity->currentFrame = 0;
	Timer_Reset(&_entity->animationTimer);
}

void UpdateAnimation(float _dt, Anim *_anim, Entity *_entity)
{
	if (_anim != NULL)
	{
		Timer_Update(&_entity->animationTimer, _dt);
		if (_anim->nbrFrames > 1)
		{
			if ((_anim->loop == 1) || (_entity->currentFrame < _anim->nbrFrames - 1))
			{
				if (_entity->animationTimer.currentTime >= _entity->animationTimer.maxTime)
				{
					if (_anim->loop == sfTrue)
					{
						int overTime = (int)(_entity->animationTimer.currentTime / _entity->animationTimer.maxTime);
						_entity->animationTimer.currentTime -= overTime * _entity->animationTimer.maxTime;
					}
					else
					{
						_entity->animationTimer.currentTime = _entity->animationTimer.maxTime;
					}
				}
				_entity->currentFrame = (int)(_anim->nbrFrames * (_entity->animationTimer.currentTime / _entity->animationTimer.maxTime));
				if (_entity->currentFrame >= _anim->nbrFrames)
				{
					_entity->currentFrame = _anim->nbrFrames - 1;
				}
			}
		}
	}
}

void DisplayAnimation(sfRenderTexture *_canvas, Anim *_currentAnim, Entity *_entity, sfVector2f _scale, sfRenderStates *_renderState)
{
	if (_currentAnim != NULL)
	{
		Sprite_Display(_canvas, _currentAnim->sprites[_entity->currentFrame], _entity->position, _scale, _entity->angle, _renderState);
	}
}

void DestroyAnim(Anim *_animToDestroy)
{
	if (_animToDestroy != NULL)
	{
		if (_animToDestroy->sprites != NULL)
		{
			for (int i = 0; i < _animToDestroy->nbrFrames; i++)
			{
				sfSprite_destroy(_animToDestroy->sprites[i]);
			}
			free(_animToDestroy->sprites);
			_animToDestroy->sprites = NULL;
		}
		free(_animToDestroy);
		_animToDestroy = NULL;
	}
}