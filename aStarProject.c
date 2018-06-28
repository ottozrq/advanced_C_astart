#include "Map.h"
#include <stdio.h>

int aStar(List *map){
  return 0;
}

int main(int argc, char* argv[]){
  List *map = loadMap(argv[1]);
  printf("%d\n", map->nelts);
  computeDist(map, "paris");
  displayList(map);
  return 0;
}
