#include "Explosion.h"

// -- LOCAL FUNCTIONS DEFINITIONS ------------------------------------------ //

ExplosionData* L_Explosion_GetRandomBlueprint(Engine *_engine, ExplosionSize _size, ExplosionType _type);

// -- SHARED FUNCTIONS ----------------------------------------------------- //

BP_Explosions Explosion_InitializeBlueprints(Engine *_engine)
{
	printf("Initialization of explosions blueprints : \n");
	BP_Explosions blueprints;
	ExploDir directory = ExploreDirectory(EXPLOSION_DIRECTORY);
	Animations* animations = _engine->anims;

	blueprints.number = directory.nbrFiles;
	blueprints.array = calloc(directory.nbrFiles, sizeof(ExplosionData*));

	for (int i = 0; i < directory.nbrFiles; i++)
	{
		char filepath[BUFFER_LARGE] = EXPLOSION_DIRECTORY;
		strcat(filepath, "/");
		strcat(filepath, directory.fileNames[i]);
		FILE *file = fopen(filepath, "rt");
		ExplosionData *explosion = calloc(1, sizeof(ExplosionData));
		if (file && explosion)
		{
			explosion->scale = 1.0f;
			char animationName[BUFFER_MEDIUM] = "NONE";
			
			while (!feof(file))
			{
				char keyWord[BUFFER_SMALL] = { 0 };
				sfBool didntFoundKeyWord = sfTrue;
				fscanf(file, "%s", keyWord);

				if (strcmp(keyWord, "TYPE") == 0)
				{
					fscanf(file, "%d\n", &explosion->type);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "SIZE") == 0)
				{
					fscanf(file, "%d\n", &explosion->size);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "ANIMATION") == 0)
				{
					fscanf(file, "%s\n", &animationName);
					explosion->animation = GetAnim(animations, "Explosions", animationName);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "SCALE") == 0)
				{
					fscanf(file, "%f\n", &explosion->scale);
					didntFoundKeyWord = sfFalse;
				}

				if (strcmp(keyWord, "SOUNDS_EXPLO") == 0)
				{
					fscanf(file, "%d", &explosion->nbSounds);
					explosion->soundNames = calloc(explosion->nbSounds, sizeof(char*));
					for (int i = 0; i < explosion->nbSounds; i++)
					{
						char buffer[BUFFER_SMALL];
						fscanf(file, " %s\n", buffer);
						explosion->soundNames[i] = _strdup(buffer);
					}
					didntFoundKeyWord = sfFalse;
				}

				if (didntFoundKeyWord == sfTrue)
				{
					fscanf(file, "%[^\n]\n", keyWord);
				}
			}
			fclose(file);
			// Add the weapon to the blueprints
			printf("(%d/%d)\t%s\n", i + 1, directory.nbrFiles, directory.fileNames[i]);
			blueprints.array[i] = explosion;
		}
		else
		{
			printf("Problem while trying to create the blueprint explosion %s\n", directory.fileNames[i]);
		}
	}
	printf("\n");
	return blueprints;
}

void Explosion_Create(Engine *_engine, List *_explosions, sfVector2f _position, ExplosionSize _size, ExplosionType _type)
{
	ExplosionData *explosion = calloc(1, sizeof(ExplosionData));
	ExplosionData *blueprint = NULL;
	int idSound = 0;
	
	blueprint = L_Explosion_GetRandomBlueprint(_engine, _size, _type);
	if (blueprint && explosion)
	{
		// Simple copy
		*explosion = *blueprint;
		
		explosion->entity.position = _position;
		explosion->entity.animationTimer = Timer_Create(blueprint->animation->maxTime);
		
		// Play random sound
		if (explosion->nbSounds > 0)
		{
			idSound = RAND(0, explosion->nbSounds - 1);
			Sound_PlayFromName(_engine->sound, explosion->soundNames[idSound]);
		}

		List_InsertElement(_explosions, 0, explosion);
	}
	else
	{
		printf("Failed to create an explosion\n");
	}
}

void Explosion_Update(Engine *_engine, List *_explosions, float _dt)
{
	ListElement *current = _explosions->first;
	while (current)
	{
		if (!current->mustBeCleared)
		{
			ExplosionData *explosion = current->data;
			if (Timer_Check(&explosion->entity.animationTimer))
			{
				current->mustBeCleared = TRUE;
			}
			else
			{
				UpdateAnimation(_dt, explosion->animation, &explosion->entity);
			}
		}
		current = current->next;
	}
}

void Explosion_Display(Engine *_engine, List *_explosions)
{
	ListElement *current = _explosions->first;
	while (current)
	{
		if (!current->mustBeCleared)
		{
			ExplosionData *explosion = current->data;
			sfVector2f scale = V2F_MUL_VALUE(_engine->gameScale, explosion->scale);
			DisplayAnimation(_engine->canvas, explosion->animation, &explosion->entity, scale, NULL);
		}
		current = current->next;
	}
}

// -- LOCAL FUNCTIONS ------------------------------------------------------ //

ExplosionData* L_Explosion_GetRandomBlueprint(Engine *_engine, ExplosionSize _size, ExplosionType _type)
{
	BP_Explosions *blueprints = &_engine->blueprints.explosions;
	ExplosionData* explosions[EXPLOSION_BUFFER] = { 0 };
	int nbExplosions = 0;

	// Get all the explosions 
	for (int i = 0; i < blueprints->number; i++)
	{
		if (blueprints->array[i]->type == _type && blueprints->array[i]->size == _size && nbExplosions < EXPLOSION_BUFFER)
		{
			nbExplosions++;
			explosions[nbExplosions - 1] = blueprints->array[i];
		}
	}
	if (nbExplosions == 0)
	{
		blueprints->array[0];
	}
	else
	{
		return explosions[RAND(0, nbExplosions - 1)];
	}
	return NULL;
}