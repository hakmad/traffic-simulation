/* Compiler directives. */

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef __UTIL_H
#define __UTIL_H
#include <util.h>
#endif

/* Structure definitions. */

/* Node structure, used for storing individual pieces of data. */
struct node {
	void *data;
	struct node *next;
};
typedef struct node NODE;

/* Queue structure, used for storing multiple nodes. */
struct queue {
	NODE *head;
	NODE *tail;
};
typedef struct queue QUEUE;

/* Function prototypes. */

NODE *new_node(void *data);
QUEUE *new_queue();
BOOL is_empty(QUEUE *queue);
void enqueue(QUEUE *queue, void *data);
void *dequeue(QUEUE *queue);
void print_queue(NODE *node);
