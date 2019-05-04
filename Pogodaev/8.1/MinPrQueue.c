#include <stdio.h>
#include <stdlib.h>
#include "minprqueue.h"

PQueue *CreateQueue() {
	PQueue *queue = (PQueue*)malloc(sizeof(PQueue));
	if (queue != NULL) {
		queue->heapSize = 0;
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

void swap(QueueElement *a, QueueElement *b) {
	QueueElement t = *a;
	*a = *b;
	*b = t;
}

int Parent(int number) {
	++number;
	return (number / 2) - 1;
}

int LeftChild(int number) {
	++number;
	return (number * 2) - 1;
}

int RightChild(int number) {
	++number;
	return number * 2;
}

void SiftUp(QueueElement *heap, int number) {
	if (number > 0 && heap[number].priority < heap[Parent(number)].priority) {
		swap(&heap[number], &heap[Parent(number)]);
		SiftUp(heap, Parent(number));
	}
	else return;
}

void SiftDown(QueueElement *heap, int number, int size) {
	int min = number;
	if (LeftChild(number) < size && heap[LeftChild(number)].priority < heap[number].priority) {
		min = LeftChild(number);
	}
	if (RightChild(number) < size && heap[RightChild(number)].priority < heap[min].priority) {
		min = RightChild(number);
	}
	if (min != number) {
		swap(&heap[number], &heap[min]);
		SiftDown(heap, min, size);
	}
	return;
}

int GetMin(PQueue *queue, QueueElement *toGet) {
	if (queue->heapSize <= 0) return 0;
	*toGet = queue->heap[0];
	return 1;
}

int Insert(PQueue *queue, unsigned int priority, T data) {
	if (queue->heapSize >= QMAXSIZE) return 0;
	queue->heap[queue->heapSize].data = data;
	queue->heap[queue->heapSize].priority = priority;
	SiftUp(queue->heap, queue->heapSize);
	++queue->heapSize;
	return 1;
}

int ExtractMin(PQueue *queue, QueueElement *toExtract) {
	if (queue->heapSize <= 0) return 0;
	*toExtract = queue->heap[0];
	queue->heap[0] = queue->heap[queue->heapSize - 1];
	--queue->heapSize;
	SiftDown(queue->heap, 0, queue->heapSize);
	return 1;
}

int SearchQueueElement(PQueue *queue, T data, int *number) {
	for (int i = 0; i < queue->heapSize; ++i) {
		if (queue->heap[i].data == data) {
			*number = i;
			return 1;
		}
	}
	return 0;
}

void ChangePriority(PQueue *queue, int number, unsigned int newPriority) {
	unsigned int oldPriority = queue->heap[number].priority;
	queue->heap[number].priority = newPriority;
	if (newPriority > oldPriority) {
		SiftDown(queue->heap, number, queue->heapSize);
	}
	else {
		SiftUp(queue->heap, number);
	}
}
