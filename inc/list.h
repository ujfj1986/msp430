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

struct ListNode {
  struct ListNode* next;
}

struct List {
  struct ListNode* head;
  struct ListNode* end;
  int len;
}
int initList(List* list);
int deleteList(List* list);

typedef int (*compareNode)(ListNode* , ListNode*);
int insert(List* list, ListNode* node, compareNode comp);

int add(List* list, ListNode* node);
ListNode* del(List* list, ListNode* node);

#endif //_LIST_H_