#include "Map.h"
#include <stdio.h>
#include <strings.h>

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
  printf("%s\n", c->name);
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

int aStarRC(List *open, List *close, City *s, char *end){
  Node *n = s->neighbour->head;
  if (open->nelts == 0)
    return -1;
  displayList(open);
  void *ct = malloc(sizeof(City));
  remFromListAt(open, 1, &ct);
  printf("City:%s", ((City*)ct)->name);
  addList(close, ct);
  if (strcasecmp(((City*)ct)->name, end) == 0)
    return 0;
  while (n){
    Neighbour *nb = n->val;
    City *c = nb->city;
    printf("Neighbour:%s\n", c->name);
    c->distFromStart = s->distFromStart + nb->distence;
    int sum = c->distFromStart + c->distToGoal;
    if (findCityDist(open, c->name) != -1 && sum > findCityDist(open, c->name))
      continue;
    if (findCityDist(close, c->name) != -1 && sum > findCityDist(close, c->name))
      continue;
    remFromList(open, c);
    remFromList(close, c);
    //c->link = s;
    addList(open, c);
    n = n->next;
  }
  aStarRC(open, close, open->head->val, end);
}

int aStar(List *map, char *start, char *end){
  List *open = newList(cmpOpen, printOpen);
  List *close = newList(compClose, printClose);
  City *s = findCity(map, start);
  s->distFromStart = 0;
  addList(open, s);
  aStarRC(open, close, s, end);
  displayList(close);
  return 0;
}

int main(int argc, char *argv[]){
  List *map = loadMap("docs/FRANCE.MAP");
  //printf("%d\n", map->nelts);
  computeDist(map, "paris");
  displayList(map);
  aStar(map, "Lyon", "Paris");
  
  return 0;
}
