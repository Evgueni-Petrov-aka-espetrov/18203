#include <stdio.h>
#include <stdlib.h>
#include "minprqueue.h"
#define NIL (unsigned int)INT_MAX + 2u 

PQueue *CreateQueue() {
	PQueue *queue = (PQueue*)malloc(sizeof(PQueue));
	if (queue != NULL) {
		queue->elementsCount = 0;
		for (int i = 0; i < QMAXSIZE; ++i) {
			queue->priorities[i] = NIL;
		}
		return queue;
	}
	else {
		fprintf(stderr, "not enough memory\n");
		exit(0);
	}
}

void DestroyQueue(PQueue *queue) {
	free(queue);
}

void swap(short *a, short *b) {
	short t = *a;
	*a = *b;
	*b = t;
}

int Insert(PQueue *queue, unsigned int priority, short vertex) {
	if (queue->elementsCount >= QMAXSIZE) return 0;
	if (queue->priorities[vertex - 1] != NIL) {
		ChangePriority(queue, vertex, priority);
		return 1;
	}
	queue->elements[queue->elementsCount] = vertex;
	queue->priorities[vertex - 1] = priority;
	++queue->elementsCount;
	return 1;
}

int ExtractMin(PQueue *queue, short *minVertex) {
	if (queue->elementsCount <= 0) {
		return 0;
	}
	unsigned int minPriority = INFINITY;
	int minPriorityElementIndex;
	for (int i = 0; i < queue->elementsCount; ++i) {
		if (queue->priorities[queue->elements[i] - 1] <= minPriority) {
			minPriority = queue->priorities[queue->elements[i] - 1];
			minPriorityElementIndex = i;
		}
	}
	*minVertex = queue->elements[minPriorityElementIndex];
	queue->priorities[*minVertex - 1] = NIL;
	swap(&queue->elements[minPriorityElementIndex], &queue->elements[queue->elementsCount - 1]);
	--queue->elementsCount;
	return 1;
}

int GetPriority(PQueue *queue, short vertex, unsigned int *priority) {
	if (queue->priorities[vertex - 1] <= INFINITY) {
		*priority = queue->priorities[vertex - 1];
		return 1;
	}
	else {
		return 0;
	}
}

int ChangePriority(PQueue *queue, short vertex, unsigned int newPriority) {
	if (queue->priorities[vertex - 1] != NIL) {
		queue->priorities[vertex - 1] = newPriority;
		return 1;
	}
	else {
		return 0;
	}
}
