#include "Map.h"
#include <stdio.h>

int main(int argc, char* argv[]){
  List *map = loadMap(argv[1]);
  printf("%d\n", map->nelts);
  displayList(map);
  return 0;
}
