#ifndef DIRECTORY_MANIPULATION_H
#define DIRECTORY_MANIPULATION_H

#include "_Includes.h"

//Functions

ExploDir ExploreDirectory(const char *repository);

void Destroy_ExploDir(ExploDir *_exploDir);

#endif
