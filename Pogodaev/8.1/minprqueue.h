#pragma once
typedef short T;
#define QMAXSIZE 5000

typedef struct {
	unsigned int priority;
	T data;
} QueueElement;

typedef struct {
	QueueElement heap[QMAXSIZE];
	int heapSize;
} PQueue;

PQueue *CreateQueue();
void DestroyQueue(PQueue* queue);
int Insert(PQueue *queue, unsigned int priority, T data);
int GetMin(PQueue *queue, QueueElement *toGet);
int ExtractMin(PQueue *queue, QueueElement *toExtract);
int SearchQueueElement(PQueue *queue, T data, int *number);
void ChangePriority(PQueue *queue, int number, unsigned int newPriority);
