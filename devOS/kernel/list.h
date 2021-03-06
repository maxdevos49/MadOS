#ifndef __KERNEL_LIST_H
#define __KERNEL_LIST_H 1

#include <stddef.h>

//https://www.oreilly.com/library/view/linux-device-drivers/0596000081/ch10s05.html

struct list_head;

struct list_head
{
    struct list_head *prev;
    struct list_head *next;
};

#define WRITE_ONCE(x, val) x = (val)

#define LIST_HEAD_INIT(name) \
    {                        \
        &(name), &(name)     \
    }

#define LIST_HEAD(name) \
    struct list_head name = LIST_HEAD_INIT(name)

#define list_entry(ptr, type, member) \
    ((type *)((char *)(ptr)-offsetof(type, member)))


static inline void INIT_LIST_HEAD(struct list_head *list)
{
        list->next = list;
        list->prev = list;
}

/**
 * Internal do not use
 * */
static inline void __list_add(struct list_head *entry, struct list_head *prev, struct list_head *next)
{
    next->prev = entry;
    entry->next = next;
    entry->prev = prev;
    WRITE_ONCE(prev->next, entry);
}

static inline void __list_del(struct list_head *prev, struct list_head *next)
{
    next->prev = prev;
    WRITE_ONCE(prev->next, next);
}

static inline void list_add(struct list_head *entry, struct list_head *head)
{
    __list_add(entry, head, head->next);//TODO Broken??
}

static inline void list_add_tail(struct list_head *entry, struct list_head *head)
{
    __list_add(entry, head->prev, head);
}

static inline void list_del(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);

    entry->next = NULL;
    entry->prev = NULL;
}

static inline int list_empty(struct list_head *head)
{
    return head->next == head;
}

#endif