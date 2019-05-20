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

void Relax(PQueue *queue, short *parents, unsigned int *estimations, int vertexFrom, int vertexTo, unsigned int edgeLength) {
	if (vertexFrom == vertexTo) return;
	if (estimations[vertexTo - 1] == NOT_ESTIMATED || estimations[vertexTo - 1] > estimations[vertexFrom - 1] + edgeLength) {
		estimations[vertexTo - 1] = estimations[vertexFrom - 1] + edgeLength > (unsigned int)INT_MAX ?
			INFINITY : estimations[vertexFrom - 1] + edgeLength;
		ChangePriority(queue, vertexTo, estimations[vertexTo - 1]);
		parents[vertexTo - 1] = vertexFrom;
	}
	return;
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
	printf("%d ", pathVertexTo);
	if (parents[pathVertexTo - 1] != NIL) {
		PrintPath(parents, pathVertexFrom, parents[pathVertexTo - 1]);
	}
}

int IsOverflowed(const short *parents, const MatrixEdge **adjacentMatrix, int pathVertexTo, int verticesCount) {
	for (int i = 0; i < verticesCount; ++i) {
		MatrixEdge lastEdge = pathVertexTo - 1 > i ?
			adjacentMatrix[pathVertexTo - 1][i] :
			adjacentMatrix[i][pathVertexTo - 1];
		if ((i + 1) != parents[pathVertexTo - 1] && lastEdge.isExist) {
			return 1;
		}
	}
	return 0;
}

void Dijkstra(const MatrixEdge **adjacentMatrix, int verticesCount, int pathVertexFrom, int pathVertexTo) {
	PQueue *queue = CreateQueue();
	short *parents = (short*)malloc(verticesCount * sizeof(short));
	unsigned int *estimations = (unsigned int*)malloc(verticesCount * sizeof(unsigned int));
	InitializeSingleSource(queue, parents, estimations, pathVertexFrom, verticesCount);
	short minEstimationVertex;
	while (ExtractMin(queue, &minEstimationVertex)) {
		for (int i = 0; i < verticesCount; ++i) {
			MatrixEdge edge = minEstimationVertex - 1 > i ?
				adjacentMatrix[minEstimationVertex - 1][i] :
				adjacentMatrix[i][minEstimationVertex - 1];
			if (edge.isExist) {
				Relax(queue, parents, estimations, minEstimationVertex, i + 1, edge.edgeLength);
			}
		}
	}
	DestroyQueue(queue);
	PrintDistances(estimations, verticesCount);
	if (estimations[pathVertexTo - 1] <= (unsigned int)INT_MAX) {
		PrintPath(parents, pathVertexFrom, pathVertexTo);
	}
	else if (estimations[pathVertexTo - 1] == NOT_ESTIMATED) {
		printf("no path");
	}
	else {
		if (!IsOverflowed(parents, adjacentMatrix, pathVertexTo, verticesCount)) {
			PrintPath(parents, pathVertexFrom, pathVertexTo);
		}
		else {
			printf("overflow");
		}
	}
	free(parents);
	free(estimations);
	return;
}
