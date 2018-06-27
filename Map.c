#include <stdio.h>
#include <string.h>
#include "Map.h"

static int compString (void * s1, void * s2) {
  return strcmp((char*)s1, (char*)s2);
}

static void prString(void * s) {
  printf("%s",(char*)s);
}


City* initCity(List *map, char *name, int x, int y){
  City *city = malloc(sizeof(City));
  if (city == NULL)
    return 0;
  city->name = name;
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

Neighbour* initNeighbour(){
}

List* loadMap(char *path){
  FILE *f = fopen(path, "r");
  if (f == NULL)
    return 0;
  List *map = newList(compString, prString);
  char *cityName = malloc(16);
  int x = -1;
  int y = -1;
  while (fscanf(f, "%s %d %d", cityName, &x, &y) != -1){
    if (y == -1){ // is a Neighbour
      printf("%s %d\n", cityName, x);
    } else { // is a City
      printf("%s %d %d\n", cityName, x, y);
    }
    x = -1;
    y = -1;
  }
  return map;
}
