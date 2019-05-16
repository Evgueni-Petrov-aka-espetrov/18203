#include <stdlib.h>
#include "graphio.h"
#include "minprqueue.h"
#include "dijkstra.h"
#define NIL 0
#define NOT_ESTIMATED UINT_MAX

void InitializeSingleSource(PQueue *queue, short *parents, unsigned int *estimations, int sourceVertex, int verticesCount) {
	for (int i = 1; i <= verticesCount; ++i) {
		Insert(queue, INFINITY, (short)i);
		estimations[i - 1] = NOT_ESTIMATED;
		parents[i - 1] = NIL;
	}
	ChangePriority(queue, sourceVertex, 0);
	estimations[sourceVertex - 1] = 0;
	return;
}

int Relax(PQueue *queue, short *parents, unsigned int *estimations, int vertexFrom, int vertexTo, unsigned int edgeLength) {
	unsigned int vertexFromPriority;
	unsigned int vertexToPriority;
	if (!GetPriority(queue, vertexFrom, &vertexFromPriority) || !GetPriority(queue, vertexTo, &vertexToPriority)) {
		return 0;
	}
	if (vertexToPriority > vertexFromPriority + edgeLength) {
		estimations[vertexTo - 1] = vertexFromPriority + edgeLength;
		ChangePriority(queue, vertexTo, vertexFromPriority + edgeLength);
		parents[vertexTo - 1] = vertexFrom;
	}
	return 1;
}

void PrintDistances(unsigned int *estimations, int verticesCount) {
	for (int i = 0; i < verticesCount; ++i) {
		if (estimations[i] <= (unsigned int)INT_MAX) {
			printf("%u ", estimations[i]);
		}
		else if (estimations[i] == NOT_ESTIMATED) {
			printf("oo ");
		}
		else {
			printf("INT_MAX+ ");
		}
	}
	printf("\n");
}

void PrintPath(short *parents, int pathVertexFrom, int pathVertexTo) {
	if (parents[pathVertexTo - 1] != NIL) {
		PrintPath(parents, pathVertexFrom, parents[pathVertexTo - 1]);
	}
	printf("%d", pathVertexTo);
}

void Dijkstra(const MatrixEdge **adjacentMatrix, int verticesCount, int pathVertexFrom, int pathVertexTo) {
	PQueue *queue = CreateQueue();
	short parents[5000];
	unsigned int estimations[5000];
	//short *parents = (short*)malloc(verticesCount * sizeof(short));
	//unsigned int *estimations = (unsigned int*)malloc(verticesCount * sizeof(unsigned int));
	InitializeSingleSource(queue, parents, estimations, pathVertexFrom, verticesCount);
	short minEstimationVertex;
	while (ExtractMin(queue, &minEstimationVertex)) {
		for (int i = 0; i < verticesCount; ++i) {
			MatrixEdge edge = adjacentMatrix[minEstimationVertex - 1][i];
			if (edge.isExist) {
				Relax(queue, parents, estimations, minEstimationVertex, i + 1, edge.edgeLength);
			}
		}
	}
	PrintDistances(estimations, verticesCount);
	if (estimations[pathVertexTo - 1] <= (unsigned int)INT_MAX) {
		PrintPath(parents, pathVertexFrom, pathVertexTo);
	}
	else if (estimations[pathVertexTo - 1] == NOT_ESTIMATED) {
		printf("no path");
	}
	else {
		printf("overflow");
	}
	free(parents);
	free(estimations);
	return;
}