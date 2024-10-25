#include "DirectoryManipulation.h"

//http://sdz.tdct.org/sdz/arcourir-les-dossiers-avec-dirent-h.html

//Functions

ExploDir ExploreDirectory(const char *_repository)
{
	ExploDir exploDir;
	struct dirent *fileName = NULL;
	DIR *rep = opendir(_repository);
	exploDir.fileNames = NULL;
	exploDir.dirNames = NULL;
	exploDir.nbrFiles = 0;
	exploDir.nbrDir = 0;

	// Checks if the repository was open successfully
	if (rep == NULL) 
	{
		perror("");
		//exit(1);
	}
	else
	{
		// Will read every file name until NULL is returned
		while ((fileName = readdir(rep)) != NULL) 
		{
			// Will increment only if he reads a file, not a directory
			if (fileName->d_type == DT_REG)
			{
				exploDir.nbrFiles++;
			}
			if (fileName->d_type == DT_DIR)
			{
				exploDir.nbrDir++;
			}
		}

		// Attributes memory to stock the file names
		exploDir.fileNames = calloc(exploDir.nbrFiles, sizeof(char*));
		exploDir.dirNames = calloc(exploDir.nbrDir, sizeof(char*));
		rewinddir(rep); 
		exploDir.nbrFiles = 0;
		exploDir.nbrDir = 0;

		while ((fileName = readdir(rep)) != NULL)
		{
			if (fileName->d_type == DT_REG)
			{
				//Attributes memory to stock the file name
				exploDir.fileNames[exploDir.nbrFiles] = calloc((strlen(fileName->d_name) + 1), sizeof(char));

				// Puts the file name in the memory slot
				strcpy(exploDir.fileNames[exploDir.nbrFiles], fileName->d_name);
				exploDir.nbrFiles++;
			}
			if (fileName->d_type == DT_DIR)
			{
				if (strcmp(fileName->d_name, ".") != 0 && strcmp(fileName->d_name, "..") != 0)
				{
					//Attributes memory to stock the file name
					exploDir.dirNames[exploDir.nbrDir] = calloc((strlen(fileName->d_name) + 1), sizeof(char));

					// Puts the file name in the memory slot
					strcpy(exploDir.dirNames[exploDir.nbrDir], fileName->d_name);
					exploDir.nbrDir++;
				}
			}
		}
		closedir(rep);
	}

	return exploDir;
}

void Destroy_ExploDir(ExploDir *_exploDir)
{
	free(_exploDir->dirNames);
	free(_exploDir->fileNames);
}
