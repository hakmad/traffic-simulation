/* Compiler directives. */

#include <queue.h>

/* Function definitions. */

/* Create a new node. */
NODE *new_node(void *data) {
	/* Allocate memory for node structure. */
	NODE *node = (NODE *) safe_malloc(sizeof(NODE));

	/* Set node attributes. */
	node->next = NULL;
	node->data = data;

	/* Return new node. */
	return node;
}

/* Create a new queue. */
QUEUE *new_queue() {
	/* Allocate memory for queue structure. */
	QUEUE *queue = (QUEUE *) safe_malloc(sizeof(QUEUE));

	/* Set queue attributes. */
	queue->head = NULL;
	queue->tail = NULL;

	/* Return new queue. */
	return queue;
}

/* Check if a queue is empty. */
BOOL is_empty(QUEUE *queue) {
	/* Use the head and tail pointers to check the state of the queue. */
	if (queue->head == NULL && queue->tail == NULL) {
		/* Head and tail pointers are not set, queue is empty. */
		return true;
	}
	else {
		/* Queue is not empty. */
		return false;
	}
}

/* Add some data to the tail of the queue. */
void enqueue(QUEUE *queue, void *data) {
	/* Create a new node with the given data. */
	NODE *node = new_node(data);

	/* Check queue state. */
	if (is_empty(queue)) {
		/* Queue is empty, set head and tail to point to node. */ 
		queue->head = node;
		queue->tail = node;
	}
	else {
		/* Queue is not empty, add node to end of queue. */
		queue->tail->next = node;
		queue->tail = node;
	}
}

/* Return the data of the node at the head of the queue. */
void *dequeue(QUEUE *queue) {
	/* Check queue state. */
	if (!(is_empty(queue))) {
		/* Get node from head of queue and get data from node. */
		NODE *node = queue->head;
		void *data = node->data;

		/* Update queue head to point to the next node. */
		queue->head = queue->head->next;

		/* Check queue state. */
		if (queue->head == NULL) {
			/* Queue has been emptied. */
			queue->tail = NULL;
		}

		/* Free memory used by node. */
		free(node);

		/* Return data from node. */
		return data;
	}
	else {
		/* Attempting to dequeue items from empty queue, throw error. */
		fprintf(stderr, "Fatal! Attempting to dequeue an empty queue.\n");
		exit(EXIT_FAILURE);
	}
}

/* Print a queue, node-wise. Shows pointers rather than actual data. */
void print_queue(NODE *node) {
	/* Check the current node. */
	if (node == NULL) {
		/* Node doesn't exist, end of queue. */
		printf("\n");
	}
	else {
		/* Node exists, print pointer and recurse to next node. */
		printf("%p -> ", node);
		print_queue(node->next);
	}
}
