#include "Map.h"
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <err.h>

typedef struct{
  char* start;
  char* end;
  char* path;
  int help;
  int showMap;
  int showOpen;
  int showClose;
} Flag;

static int cmpOpen(void *e1, void *e2){
  City *c1 = e1;
  City *c2 = e2;
  int sum1 = c1->distFromStart + c1->distToGoal;
  int sum2 = c2->distFromStart + c2->distToGoal;
  if (sum1 < sum2)
    return -1;
  else if (sum1 > sum2)
    return 1;
  else
    return 0;
}

static int compClose(void *e1, void *e2){
  return -1;
}

static void printOpen(void *e){
  City *c = e;
  printf("Name:%s\tStart:%d\tEnd:%d\tSum:%d\n",
   c->name, c->distFromStart, c->distToGoal,
   c->distFromStart + c->distToGoal);
}

static void printClose(void *e){
  City *c = e;
  printf("%s - %d\n", c->name, c->distFromStart + c->distToGoal);
}

int findCityDist(List *l, char *name){
  Node *n = l->head;
  while(n){
    City *c = n->val;
    if (strcasecmp(c->name, name) == 0)
      return c->distFromStart + c->distToGoal;
    n = n->next;
  }
  return -1;
}

int aStarRC(List *open, List *close, City *s, char *end, Flag *f){
  Node *n = s->neighbour->head;
  static int count = 1;
  if (open->nelts == 0)
    return -1;
  if (f->showOpen || f->showClose)
    printf("============== %d search ============\n", count++);
  if (f->showOpen){
    puts("------------ open---------------");
    displayList(open);
  }
  if (f->showClose){
    puts("------------ close ---------------");
    displayList(close);
  }
  void *ct = malloc(sizeof(City));
  remFromListAt(open, 1, &ct);
  //printf("City:%s\n", ((City*)ct)->name);
  addList(close, ct);
  if (strcasecmp(((City*)ct)->name, end) == 0)
    return 0;
  while (n){
    Neighbour *nb = n->val;
    City *c = nb->city;
    //puts(c->name);
    //printf("Neighbour:%s\n", c->name);
    c->distFromStart = s->distFromStart + nb->distence;
    int sum = c->distFromStart + c->distToGoal;
    //printf("%d > %d\n", sum, findCityDist(open, c->name));
    if (findCityDist(open, c->name) != -1 && sum > findCityDist(open, c->name))
      continue;
    if (findCityDist(close, c->name) != -1 && sum > findCityDist(close, c->name))
      continue;
    remFromList(open, c);
    remFromList(close, c);
    //c->link = s;
    if (strcasecmp(f->start, c->name) != 0)
      addList(open, c);
    n = n->next;
  }
  aStarRC(open, close, open->head->val, end, f);
  return 0;
}

int aStar(List *map, Flag *f){
  char *start = f->start;
  char *end = f->end;
  List *open = newList(cmpOpen, printOpen);
  List *close = newList(compClose, printClose);
  City *s = findCity(map, start);
  s->distFromStart = 0;
  addList(open, s);
  aStarRC(open, close, s, end, f);
  printf("The Min distence is:%d\n", ((City*)close->head->val)->distFromStart);
  puts("The way will be (end->start):");
  displayList(close);
  return 0;
}

void help(){
  puts("Usage: ./aStar.static [commands]");
  puts("");
  puts("commands:");
  puts("\t-h/--help\tLooking for help");
  puts("\t-p/--path\tGiving the path of the map file");
  puts("\t-s/--start\tDefind starting city");
  puts("\t-e/--end\tDefind end city");
  puts("\t-lm/--listmap\tShow the generated graph");
  puts("\t-lo/--listopen\tShow the open list");
  puts("\t-lc/--listclose\tShow the close list");
}

Flag *initFlag(int argc, char *argv[]){
  Flag *f = calloc(1,sizeof(Flag));
  f->path = "FRANCE.MAP";
  if (argc == 0)
    return f;
  for(int i = 1; argv[i]; i++){
    if (strcmp(argv[i], "-s") == 0
	|| strcmp(argv[i], "--start") == 0){
      if (argv[i+1])
	f->start = argv[i+1];
      else
	err(1, "ERROR: Missing arg for command -s/--start, see -h for more help");
      i++;
    } else if (strcmp(argv[i], "-e") == 0
	       || strcmp(argv[i], "--end") == 0){
      if (argv[i+1])
	f->end = argv[i+1];
      else
	err(1, "ERROR: Missing arg for command -e/--end, see -h for more help");
      i++;
    } else if (strcmp(argv[i], "-p") == 0
	       || strcmp(argv[i], "--path") == 0){
      if (argv[i+1])
	f->path = argv[i+1];
      else
	err(1, "ERROR: Missing arg for command -p/--path, see -h for more help");
      i++;
    } else if (strcmp(argv[i], "-lm") == 0
	       || strcmp(argv[i], "--listmap") == 0){
      f->showMap = 1;
    } else if (strcmp(argv[i], "-lo") == 0
	       || strcmp(argv[i], "--listopen") == 0){
      f->showOpen = 1;
    } else if (strcmp(argv[i], "-lc") == 0
	       || strcmp(argv[i], "--listclose") == 0){
      f->showClose = 1;
    } else if (strcmp(argv[i], "-h") == 0
	       || strcmp(argv[i], "--help") == 0){
      f->help = 1;
    } else {
      err(1, "ERROR: Invalid command, see -h/--help for more help");
    }
  }
  return f;
}

void getCity(Flag *f){
  char *start = malloc(16);
  char *end = malloc (16);
  printf("Input the name of starting City:");
  scanf("%s", start);
  printf("Input the name of ending City:");
  scanf("%s", end);
  f->start = start;
  f->end = end;
}

int main(int argc, char *argv[]){
  Flag *f = initFlag(argc, argv);
  if (f->help){
    help();
    return 0;
  }
  if (f->start == NULL || f->end == NULL)
    getCity(f);
  List *map = loadMap(f->path);
  computeDist(map, f->end);
  if (f->showMap)
    displayList(map);
  aStar(map, f);
  
  return 0;
}
