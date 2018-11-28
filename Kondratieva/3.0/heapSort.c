#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

void printAnswer(int* heap, int heapSize) {
    for (int i = 0; i < heapSize; i++)
        printf("%d ", heap[i]);
}

void getElements(int* heap, int heapSize) {
	for (int i = 0; i < heapSize; i++)
        scanf("%d", &heap[i]);
}

void siftDown(int* heap, int heapSize, int i) {
    int left, right, j;
    while (2*i + 1 < heapSize) {
        left = 2*i + 1;
        right = 2*i + 2;
        j = left;
        if (right < heapSize)
            j = (heap[j] < heap[right]) ? right : j;
        if (heap[i] >= heap[j])
            break;
        swap(&heap[i], &heap[j]);
        i = j;
    }
}

void buildHeap(int* heap, int heapSize) {
    for (int i = heapSize/2 - 1; i >= 0; i--) {
        siftDown(heap, heapSize, i);
    }
}

void heapSort(int* heap, int heapSize) {
    buildHeap(heap, heapSize);
    for (int i = heapSize - 1; i > 0; i--) {
        swap(&heap[0], &heap[i]);
        siftDown(heap, i, 0);
    }
}

int main() {
    int heapSize;
	int* heap;
	scanf("%d", &heapSize);
    heap = (int*)malloc(sizeof(int)*heapSize);
	if (heap == NULL)
		return 0;
    getElements(heap, heapSize);
    heapSort(heap, heapSize);
    printAnswer(heap, heapSize);
	free(heap);
    return 0;
}
