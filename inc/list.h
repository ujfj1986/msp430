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

typedef int (*compareNode)(ListNode* , ListNode*);
int insert(ListNode* node, compareNode comp);

int add(ListNode* node);
ListNode* del();

#endif //_LIST_H_