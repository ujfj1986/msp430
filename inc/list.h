/*
* list.h - define the list interface.
*
* define the list struct interface.
*
* Author: Shan Jiejing
* Date: 2016-12-25
*/
#ifndef _LIST_H_
#define _LIST_H_

typedef struct ListNode {
  struct ListNode* next;
} ListNode;

typedef struct List {
  struct ListNode* head;
  struct ListNode* end;
  int len;
} List;
int initList(List* list);
int deleteList(List* list);

typedef int (*compareNode)(ListNode* , ListNode*);
int insert(List* list, ListNode* node, compareNode comp);

int addNode(List* list, ListNode* node);
ListNode* delNode(List* list);

#endif //_LIST_H_