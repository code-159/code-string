//////////////////////////////////////////////////////////////////
//
//  Copyright(C), 2013-2016, GEC Tech. Co., Ltd.
//
//  File name: GPLE/ch03/3.1/list.h
//
//  Author: Linux Kernel
//
//  Date: 2016-3
//  
//  Description: Linux内核链表源代码
//
//  GitHub: github.com/vincent040   Bug Report: 2437231462@qq.com
//
//////////////////////////////////////////////////////////////////

#ifndef __DLIST_H
#define __DLIST_H

/* This file is from Linux Kernel (include/linux/list.h)
* and modified by simply removing hardware prefetching of list items.
* Here by copyright, credits attributed to wherever they belong.
* Kulesh Shanmugasundaram (kulesh [squiggly] isis.poly.edu)
*/

/*
* Simple doubly linked list implementation.
*
* Some of the internal functions (“__xxx”) are useful when
* manipulating whole lists rather than single entries, as
* sometimes we already know the next/prev entries and we can
* generate better code by using them directly rather than
* using the generic single-entry routines.
*/
/**
 * container_of - cast a member of a structure out to the containing structure
 *	
 * @ptr:	the pointer to the member.
 * @type:	the type of the container struct this is embedded in.
 * @member:	the name of the member within the struct.
 *
 */
 //统计小个结构体距离大个结构体的偏移量是多少
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

//获取大个结构体的地址，并不是很通用
#define container_of(ptr, type, member) ({			\
        const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
        (type *)( (char *)__mptr - offsetof(type,member) );})
/*
 * These are non-NULL pointers that will result in page faults
 * under normal circumstances, used to verify that nobody uses
 * non-initialized list entries.
 */
 //在内核当中的两个非法地址
#define LIST_POISON1  ((void *) 0x00100100)
#define LIST_POISON2  ((void *) 0x00200)

/*
	内核链表小个结构体的声明
*/
struct list_head {
	struct list_head *next, *prev;
};


//为了配合小个结构体的初始化，就是小个结构体变量的声明
#define LIST_HEAD_INIT(name) { &(name), &(name) }
//诞生一个小个结构体变量，并且让里面prev指针跟next都指向自己
#define LIST_HEAD(name) \
struct list_head name = LIST_HEAD_INIT(name)

/*
	初始化头节点
*/
#define INIT_LIST_HEAD(ptr) do { \
	(ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while (0)

/*
* Insert a new entry between two known consecutive entries.
*
* This is only for internal list manipulation where we know
* the prev/next entries already!
*/

//插入节点,new是新节点，prev上一个节点是谁，next下一个节点是谁
static inline void __list_add(struct list_head *new,
				struct list_head *prev,
				struct list_head *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

/**
* list_add – add a new entry
* @new: new entry to be added
* @head: list head to add it after
*
* Insert a new entry after the specified head.
* This is good for implementing stacks.
*/
//插入new节点到head后面去
static inline void list_add(struct list_head *new, struct list_head *head)
{
	__list_add(new, head, head->next);
}

/**
* list_add_tail – add a new entry
* @new: new entry to be added
* @head: list head to add it before
*
* Insert a new entry before the specified head.
* This is useful for implementing queues.
*/

//插入new节点到head前面去
static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
	__list_add(new, head->prev, head);
}

/*
* Delete a list entry by making the prev/next entries
* point to each other.
*
* This is only for internal list manipulation where we know
* the prev/next entries already!
*/
//移除prev跟next两个节点中间的节点
static inline void __list_del(struct list_head *prev, struct list_head *next)
{
	next->prev = prev;
	prev->next = next;
}

/**
* list_del – deletes entry from list.
* @entry: the element to delete from the list.
* Note: list_empty on entry does not return true after this, the entry is in an undefined state.
*/
//将entry节点从链表当中移除，将entry节点里面的上下个指针指向NULL
static inline void list_del(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	entry->next = (void *) 0;
	entry->prev = (void *) 0;
}

/**
* list_del_init – deletes entry from list and reinitialize it.
* @entry: the element to delete from the list.
*/
//将entry节点从链表当中移除，将entry节点里面的上下个指针指向entry
static inline void list_del_init(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	INIT_LIST_HEAD(entry);
}

/**
* list_move – delete from one list and add as another’s head
* @list: the entry to move
* @head: the head that will precede our entry
*/
//将链表中的list节点移动到head后面去
static inline void list_move(struct list_head *list,
				struct list_head *head)
{
	__list_del(list->prev, list->next);
	list_add(list, head);
}

/**
* list_move_tail – delete from one list and add as another’s tail
* @list: the entry to move
* @head: the head that will follow our entry
*/
//将链表中的list节点移动到head前面去
static inline void list_move_tail(struct list_head *list,
					struct list_head *head)
{
	__list_del(list->prev, list->next);
	list_add_tail(list, head);
}

/**
* list_empty – tests whether a list is empty
* @head: the list to test.
*/
//判断链表是否为空链表
static inline int list_empty(struct list_head *head)
{
	return head->next == head;
}
//接合list跟head两条链表
static inline void __list_splice(struct list_head *list,
					struct list_head *head)
{
	struct list_head *first = list->next;
	struct list_head *last = list->prev;
	struct list_head *at = head->next;

	first->prev = head;
	head->next = first;

	last->next = at;
	at->prev = last;
}

/**
* list_splice – join two lists
* @list: the new list to add.
* @head: the place to add it in the first list.
*/
//将两条链表合并成为一条链表
static inline void list_splice(struct list_head *list, struct list_head *head)
{
if (!list_empty(list))
__list_splice(list, head);
}

/**
* list_splice_init – join two lists and reinitialise the emptied list.
* @list: the new list to add.
* @head: the place to add it in the first list.
*
* The list at @list is reinitialised
*/
//将两条链表合并成为一条链表，顺便将list这个头节点初始化成为指向自己的头节点
static inline void list_splice_init(struct list_head *list,
struct list_head *head)
{
if (!list_empty(list)) {
__list_splice(list, head);
INIT_LIST_HEAD(list);
}
}

/**
* list_entry – get the struct for this entry通过小个结构体地址获取大个结构体地址的方式
* @ptr:    the &struct list_head pointer.小个结构体地址
* @type:    the type of the struct this is embedded in.大个结构体类型名字
* @member:    the name of the list_struct within the struct.小个结构体类型名字
*/
//通过小个结构体地址获取大个结构体地址的方式
#define list_entry(ptr, type, member) \
((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

/**
* list_for_each    -    iterate over a list
* @pos:    the &struct list_head to use as a loop counter.
* @head:    the head for your list.
*/
//不安全遍历方式，向下遍历
#define list_for_each(pos, head) \
for (pos = (head)->next; pos != (head); \
pos = pos->next)
/**
* list_for_each_prev    -    iterate over a list backwards
* @pos:    the &struct list_head to use as a loop counter.
* @head:    the head for your list.
*/
//不安全遍历方式，向上遍历
#define list_for_each_prev(pos, head) \
for (pos = (head)->prev; pos != (head); \
pos = pos->prev)

/**
* list_for_each_safe    -    iterate over a list safe against removal of list entry
* @pos:    the &struct list_head to use as a loop counter.
* @n:        another &struct list_head to use as temporary storage
* @head:    the head for your list.
*/
//安全的遍历方式
#define list_for_each_safe(pos, n, head) \
for (pos = (head)->next, n = pos->next; pos != (head); \
pos = n, n = pos->next)

/**
* list_for_each_entry    -    iterate over list of given type
* @pos:    the type * to use as a loop counter.
* @head:    the head for your list.
* @member:    the name of the list_struct within the struct.
*/
//以不安全的形式遍历链表的每个元素，并且获取大个结构体放到pos里面
#define list_for_each_entry(pos, head, member)                \
for (pos = list_entry((head)->next, typeof(*pos), member);    \
&pos->member != (head);                     \
pos = list_entry(pos->member.next, typeof(*pos), member))

/**
* list_for_each_entry_safe – iterate over list of given type safe against removal of list entry
* @pos:    the type * to use as a loop counter.
* @n:        another type * to use as temporary storage
* @head:    the head for your list.
* @member:    the name of the list_struct within the struct.
*/
//以安全的形式遍历链表的每个元素，并且获取大个结构体放到pos里面
#define list_for_each_entry_safe(pos, n, head, member)            \
for (pos = list_entry((head)->next, typeof(*pos), member),    \
n = list_entry(pos->member.next, typeof(*pos), member);    \
&pos->member != (head);                     \
pos = n, n = list_entry(n->member.next, typeof(*n), member))

#endif
