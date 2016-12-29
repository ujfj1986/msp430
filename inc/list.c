/*
* list.c - implement the list interface.
*
* Author: Shan Jiejing
* Date: 2016-12-28
*/
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

int initList(List* list) {
  if (NULL == list) return -1;
  list->head = list->end = NULL;
  list->len = 0;
  return 0;
}
int deleteList(List* list) {
  if (NULL == list) return -1;
  if (0 == list->len) return 0;
  ListNode* p = list->head;
  for (p = list->head; p != NULL; p = list->head) {
    list->head = list->head->next;
    free(p);
  }
  list->head = list->end = NULL;
  list->len = 0;
  return 0;
}

int insert(List* list, ListNode* node, compareNode comp) {
  if (NULL == list || NULL == node || NULL == comp) return -1;
  if (0 == list->len) {
    list->head = node;
    list->end = node;
    list->len = 1;
  } else {
    ListNode* p = list->head;
    ListNode* q = list->head;
    int r = 0;
    
    do {
      r = comp(p, node);
      if ( r >= 0) {
        if (p == list->head) {
          list->head = node;
          node->next = p;
        } else {
          q->next = node;
          node->next = p;
        }
        break;
      } else {
        q = p;
        p = p->next;
        if (q == list->end) {
          q->next = node;
          list->end = node;
        }
      }
    } while (p != NULL);
    list->len ++;
  }
  return 0;
}
        
    
//add node to the list end.
int add(List* list, ListNode* node) {
  if (NULL == list || NULL == node) return -1;
  if (0 == list->len) {
    list->head = list->end = node;
    list->len ++;
  } else {
    ListNode* p = list->head;
    int find = 0;
    
    while(p != NULL) {
      if (p == node) {
        find = 1;
        break;
      }
      p = p->next;
    }
    if (1 == find) return -1;
    list->end->next = node;
    list->end = node;
  }
  return 0;
}
//del node in the list
ListNode* del(List* list, ListNode* node) {
  return NULL;
}
