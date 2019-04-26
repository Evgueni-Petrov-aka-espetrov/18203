#include <stdio.h>
#include <stdlib.h>
#include "graphio.h"
#include "disjointsets.h"
#include "kruskal.h"

typedef struct {
	int vertexFrom;
	int vertexTo;
	int edgeLength;
} Edge;

void AddEdgesToArray(const VerticesList **adjacentVerticesLists, int verticesCount, int edgesCount, Edge *edgesArray) {
	for (int i = 0, j = 0; i < verticesCount; ++i) {
		const VerticesList *adjacentVerticesList = adjacentVerticesLists[i];
		for (; adjacentVerticesList != NULL; adjacentVerticesList = adjacentVerticesList->next) {
			if (j >= edgesCount) {
				return;
			}
			if (adjacentVerticesList->vertexNumber >= i + 1) {
				edgesArray[j].vertexFrom = i + 1;
				edgesArray[j].vertexTo = adjacentVerticesList->vertexNumber;
				edgesArray[j].edgeLength = adjacentVerticesList->edgeLength;
				++j;
			}
		}
	}
	return;
}

int CompareEdges(const Edge *firstEdge, const Edge *secondEdge) {
	if (firstEdge->edgeLength > secondEdge->edgeLength) {
		return 1;
	}
	else if (firstEdge->edgeLength < secondEdge->edgeLength) {
		return -1;
	}
	else {
		return 0;
	}
}

int Kruskal(const VerticesList** adjacentVerticesLists, int verticesCount, int edgesCount, FILE* out) {
	int isNoSpanningTree = 0;
	Edge *spanningTreeEdges = (Edge*)malloc((verticesCount - 1) * sizeof(Edge));
	DisjointSets *sets = CreateSet();
	for (int i = 1; i <= verticesCount; ++i) {
		MakeSet(sets, i);
	}
	Edge *graphEdges = (Edge*)malloc(edgesCount * sizeof(Edge));
	AddEdgesToArray(adjacentVerticesLists, verticesCount, edgesCount, graphEdges);
	qsort(graphEdges, edgesCount, sizeof(Edge), CompareEdges);
	int spanningTreeEdgesNumber = 0;
	for (int i = 0; i < edgesCount && spanningTreeEdgesNumber != verticesCount - 1; ++i) {
		if (FindSet(sets, graphEdges[i].vertexFrom) != FindSet(sets, graphEdges[i].vertexTo)) {
			spanningTreeEdges[spanningTreeEdgesNumber] = graphEdges[i];
			++spanningTreeEdgesNumber;
			Union(sets, graphEdges[i].vertexFrom, graphEdges[i].vertexTo);
		}
	}
	DeleteSet(sets);
	free(graphEdges);
	if (spanningTreeEdgesNumber == verticesCount - 1) {
		for (int i = 0; i < spanningTreeEdgesNumber; ++i) {
			printf("%d %d\n", spanningTreeEdges[i].vertexFrom, spanningTreeEdges[i].vertexTo);
		}
		isNoSpanningTree = 0;
	}
	else {
		isNoSpanningTree = 1;
	}
	free(spanningTreeEdges);
	return isNoSpanningTree;
}