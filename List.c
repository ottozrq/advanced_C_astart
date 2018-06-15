#include "List.h"
#include <stdio.h>

List* newList(compFun comp, prFun pr){
  List* l = (List*)malloc(sizeof(List));
  if (l == NULL)
    return 0;
  l->nelts = 0;
  l->head = NULL;
  l->comp = comp;
  l->pr = pr;
  return l;
}

void delList(List* l){
  Node *n = l->head;
  while (n){
    Node *temp = n;
    n = n->next;
    free(temp);
  }
  free(l);
}

status nthInList(List* l, int n, void** e){
  if (n > l->nelts)
    return ERRINDEX;
  Node *node = l->head;
  for (int i = 0; i < n; i++)
    node = node->next;
  *e = node->val;
  return OK;
}

status addListAt(List* l, int p, void* e){
  if (p > l->nelts)
    return ERRINDEX;
  Node *n = l->head;
  for (int i = 0; i < p; i++)
    n = n->next;
  Node *t = (Node*)malloc(sizeof(Node));
  if (!t)
    return ERRALLOC;
  t->val = e;
  t->next = n->next;
  n = t;
  l->nelts++;
  return OK;
}

status remFromListAt(List* l, int p, void** e){
  if (p > l->nelts)
    return ERRINDEX;
  if (p == 0)
    l->head = l->head->next;
  Node *n = l->head;
  for (int i = 1; i < p; i++)
    n = n->next;
  Node *t = n->next;
  n->next = n->next->next;
  *e = t->val;
  free(t);
  l->nelts--;
  return OK;
}

status remFromList(List* l,void* e){
  if (!l->comp)
    return ERRUNABLE;
  Node *n = l->head;
  Node *pre = n;
  for (; n; pre = n, n = n->next)
    if (l->comp(n->val, e) == 0){
      Node *t = pre->next;
      pre->next = n->next;
      free(t);
      return OK;
    }
  return ERRABSENT;
}

status	displayList(List* l){
  Node *n = l->head;
  printf("[ ");
  for (; n; n = n->next){
    l->pr(n->val);
    putchar(' ');
  }
  putchar(']');
}

void forEach(List* l, void(* f)(void*)){
  Node *n = l->head;
  for(; n; n = n->next)
    f(n->val);
}

int lengthList(List* l){
  return l->nelts;
}

status addList(List* l, void* e){
  if (!l->comp)
    return ERRUNABLE;
  Node *n = l->head;
  Node *pre = n;
  while (n && l->comp(n->val, e) < 0){
    pre = n;
    n = n->next;
  }
  Node *t = (Node*)malloc(sizeof(Node));
  if (!t)
    return ERRALLOC;
  t->val = e;
  t->next = n;
  if (pre)
    pre->next = t;
  else
    l->head = t;
  l->nelts++;
  return OK;
}

Node* isInList(List* l,void* e){
  Node *n = l->head;
  if (!n)
    return 0;
  if (n && l->comp(n->val, e) == 0)
    return 1;
  while (n){
    n = n->next;
    if (n && l->comp(n->val, e) == 0)
      return n;
  }
  return 0;
}
