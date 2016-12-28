/*
* list.c - implement the list interface.
*
* Author: Shan Jiejing
* Date: 2016-12-28
*/
#include <stdio.h>
#include "list.h"

int initList(List* list) {
  if (NULL == list) return -1;
  list->head = list->end = NULL;
  list->len = 0;
  return 0;
}
int deleteList(List* list) {
  if (NULL == list) return -1;
  ListNode* p = 

typedef int (*compareNode)(ListNode* , ListNode*);
int insert(List* list, ListNode* node, compareNode comp);

int add(List* list, ListNode* node);
ListNode* del(List* list, ListNode* node);