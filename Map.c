#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <math.h>
#include <err.h>
#include "Map.h"

static int compString (void * s1, void * s2) {
  return strcmp((char*)s1, (char*)s2);
}

static int compCity(void *c1, void *c2){
  return strcmp(((City*)c1)->name, ((City*)c2)->name);
}

static int compN(void *n1, void *n2){
  return 1;
}

static void prString(void * s) {
  printf("%s",(char*)s);
}

static void printCity(void *c) {
  printf("+-Name:%s\tX:%d\tY:%d\tDistToGoal:%d\n",
	 ((City*)c)->name, ((City*)c)->x, ((City*)c)->y,
	 ((City*)c)->distToGoal);
  forEach(((City*)c)->neighbour, ((City*)c)->neighbour->pr);
}

static void printN(void *n) {
  printf("|__Name:%s\tDist:%d\tX:%d\tY:%d\n",
	 ((Neighbour*)n)->city->name, ((Neighbour*)n)->distence, ((Neighbour*)n)->city->x, ((Neighbour*)n)->city->y);
}


City* initCity(char *name, int x, int y){
  City *city = malloc(sizeof(City));
  if (city == NULL)
    return 0;
  city->name = malloc(16);
  strcpy(city->name, name);
  city->neighbour = newList(compString, prString);
  if (city == NULL)
    return 0;
  city->x = x;
  city->y = y;
  city->distFromStart = 0;
  city->distToGoal = 0;
  city->link = NULL;
  return city;
}

Neighbour* initNeighbour(char *name, int dist){
  Neighbour *n = malloc(sizeof(Neighbour));
  if (n == NULL)
    return 0;
  n->city = initCity(name, -1, -1);
  n->distence = dist;
  return n;
}

List* buildGraph(FILE *f, List *root, int isCity){
  List *map = NULL;
  if (isCity)
    map = newList(compCity, printCity);
  else
    map = newList(compN, printN);
  char *cityName = malloc(16);
  for (int x = -1, y = -1;
       fscanf(f, "%s %d %d\n", cityName, &x, &y) != -1;
       x = -1, y = -1){
    if (y == -1){ // is a Neighbour
      //printf("%s %d\n", cityName, x);
      addList(map, initNeighbour(cityName, x));
    } else { // is a City
      if (! isCity){
	//printf("%s %d %d\n", cityName, x, y);
	City *city = initCity(cityName, x, y);
	city->neighbour = buildGraph(f, root, 0);
	addList(root, city);
	break;
      }
      //printf("%s %d %d\n", cityName, x, y);
      City *city = initCity(cityName, x, y);
      Node *n = NULL;
      if (n = isInList(map, city)){
        
      }
      city->neighbour = buildGraph(f, map, 0);
      addList(map, city);
    }
  }
  return map;
}

void linkCities(List* map){
  Node *city = map->head;
  while (city){
    Node *n = ((City*)city->val)->neighbour->head;
    while (n){
      Node *tmp = map->head;
      while (tmp){
	City *c = tmp->val;
	Neighbour *nb = n->val;
	if (strcmp(c->name, nb->city->name) == 0){
	  free(nb->city);
	  nb->city = c;
	}
	tmp = tmp->next;
      }
      n = n->next;
    }
    city = city->next;
  }
}

List* loadMap(char *path){
  FILE *f = fopen(path, "r");
  if (f == NULL)
    err(1, "can not find file %s", path);
  List *map = buildGraph(f, NULL, 1);
  if (map == NULL)
    return 0;
  linkCities(map);
  return map;
}

int compDist(int x1, int y1, int x2, int y2){
  return sqrt(pow(x1-x2, 2) + pow(y1-y2, 2));
}

City *findCity(List *map, char *name){
  Node *t = map->head;
  while (t){
    City *c = t->val;
    if (strcasecmp(name, c->name) == 0)
      return c;
    t = t->next;
  }
  return NULL;
}

status computeDist(List *map, char* end){
  Node *t = map->head;
  City* c = findCity(map, end);
  if (c != NULL && strcasecmp(end, c->name) != 0)
    return ERREXIST;
  while (t){
    City *city = t->val;
    city->distToGoal = compDist(c->x, c->y, city->x, city->y);
    t = t->next;
  }
  return OK;
}
