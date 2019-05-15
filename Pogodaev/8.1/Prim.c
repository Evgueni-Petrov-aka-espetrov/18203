#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "graphio.h"
#include "minprqueue.h"
#include "prim.h"
#define NIL 0

int Prim(MatrixEdge **adjacentMatrix, int verticesCount, FILE* out) {
	if (verticesCount < 1) {
		return 1;
	}
	int isNoSpanningTree = 0;
	short *parent = (short*)malloc(verticesCount * sizeof(short));
	for (int i = 0; i < verticesCount; ++i) {
		parent[i] = NIL;
	}
	PQueue *queue = CreateQueue();
	Insert(queue, 0, (short)1);
	for (int i = 2; i <= verticesCount; ++i) {
		Insert(queue, INFINITY, (short)i);
	}
	short nextMinVertex;
	while (ExtractMin(queue, &nextMinVertex)) {
		for (short i = 0; i < verticesCount; ++i) {
			MatrixEdge edge = (i + 1) < nextMinVertex ?
				adjacentMatrix[nextMinVertex - 1][i] :
				adjacentMatrix[i][nextMinVertex - 1];
			if (edge.isExist) {
				unsigned int ithVertexPriority;
				if (GetPriority(queue, i + 1, &ithVertexPriority) && edge.edgeLength < ithVertexPriority) {
					parent[i] = nextMinVertex;
					ChangePriority(queue, i + 1, edge.edgeLength);
				}
			}
		}
	}
	DestroyQueue(queue);
	for (int i = 1; i < verticesCount; ++i) {
		if (parent[i] == NIL) {
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
