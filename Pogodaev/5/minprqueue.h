#pragma once
typedef void* T;
#define QMAXSIZE 256

typedef struct {
	unsigned long long priority;
	T data;
} QueueElement;

typedef struct {
	QueueElement heap[QMAXSIZE];
	int heapSize;
} PQueue;

PQueue *CreateQueue();
void DestroyQueue(PQueue* queue);
int Insert(PQueue *queue, unsigned long long priority, T data);
int GetMin(PQueue *queue, QueueElement *toGet);
int ExtractMin(PQueue* queue, QueueElement *toExtract);
