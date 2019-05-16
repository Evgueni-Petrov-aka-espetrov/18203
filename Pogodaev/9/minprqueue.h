#pragma once
#define QMAXSIZE 5000
#define INFINITY (unsigned int)INT_MAX + 1u

typedef struct {
	short elements[QMAXSIZE];
	unsigned int priorities[QMAXSIZE];
	int elementsCount;
} PQueue;

PQueue *CreateQueue();
void DestroyQueue(PQueue* queue);
int Insert(PQueue *queue, unsigned int priority, short vertex);
int ExtractMin(PQueue *queue, short *minVertex);
int GetPriority(PQueue *queue, short vertex, unsigned int *priority);
int ChangePriority(PQueue *queue, short vertex, unsigned int newPriority);
