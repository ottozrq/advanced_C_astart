#ifndef _Map_H
#define _Map_H

#include <stdlib.h>
#include "List.h"
#include "status.h"

typedef struct{
	char *name;
	List *neighbour;
	int x;
	int y;
	int distFromStart;
	int distToGoal;
	Node *link;
}City;

typedef struct{
	City *city;
	int distence;
}Neighbour;

List* loadMap(char *path);

City *findCity(List *map, char *name);

status computeDist(List *map, char *end);

#endif
