#include <stdio.h>
#include <stdlib.h>
#include "graphio.h"
#include "minprqueue.h"
#include "prim.h"

int Prim(MatrixEdge **adjacentMatrix, int verticesCount, FILE* out) {
	if (verticesCount < 1) {
		return 1;
	}
	int isNoSpanningTree = 0;
	short *parent = (short*)malloc(verticesCount * sizeof(short));
	for (int i = 0; i < verticesCount; ++i) {
		parent[i] = -1;
	}
	PQueue *queue = CreateQueue();
	Insert(queue, 0, (short)1);
	for (int i = 2; i <= verticesCount; ++i) {
		Insert(queue, (unsigned int)INT_MAX + 1u, (short)i);
	}
	QueueElement nextElement;
	while (ExtractMin(queue, &nextElement)) {
		for (short i = 0; i < verticesCount; ++i) {
			MatrixEdge edge = i < nextElement.data ?
				adjacentMatrix[nextElement.data - 1][i] :
				adjacentMatrix[i][nextElement.data - 1];
			if (edge.isExist) {
				int number;
				if (SearchQueueElement(queue, i + 1, &number) && edge.edgeLength < queue->heap[number].priority) {
					parent[i] = nextElement.data;
					ChangePriority(queue, number, edge.edgeLength);
				}
			}
		}
	}
	DestroyQueue(queue);
	for (int i = 1; i < verticesCount; ++i) {
		if (parent[i] == -1) {
			isNoSpanningTree = 1;
		}
	}
	if (!isNoSpanningTree) {
		for (int i = 1; i < verticesCount; ++i) {
			printf("%d %d\n", i + 1, parent[i]);
		}
	}
	free(parent);
	return isNoSpanningTree;
}
