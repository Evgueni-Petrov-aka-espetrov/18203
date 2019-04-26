#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "graphio.h"
#define max_vertices 5000
#define max_edges(N) N*(N-1)/2

error_t GetGraphCounts(FILE *in, int *verticesCount, int *edgesCount) {
	if (fscanf(stdin, "%d", verticesCount) != 1) {
		return bad_number_of_lines;
	}
	if (*verticesCount < 0 || *verticesCount > max_vertices) {
		return bad_number_of_vertices;
	}
	if (fscanf(stdin, "%d", edgesCount) != 1) {
		return bad_number_of_lines;
	}
	if (*edgesCount < 0 || *edgesCount > max_edges(*verticesCount)) {
		return bad_number_of_edges;
	}
	return ok;
}

void AddEdge(VerticesList **adjacentVerticesLists, int vertexFrom, int vertexTo, int edgeLength) {
	VerticesList *newEdge = (VerticesList*)malloc(sizeof(VerticesList));
	assert(newEdge != NULL);
	newEdge->vertexNumber = vertexTo;
	newEdge->edgeLength = edgeLength;
	newEdge->next = adjacentVerticesLists[vertexFrom - 1];
	adjacentVerticesLists[vertexFrom - 1] = newEdge;
}

error_t GetGraph(FILE *in, int *verticesCount, int *edgesCount, VerticesList ***adjacentVerticesLists) {
	error_t error = GetGraphCounts(in, verticesCount, edgesCount);
	if (error != ok) {
		*adjacentVerticesLists = NULL;
		return error;
	}
	*adjacentVerticesLists = (VerticesList**)malloc(*verticesCount * sizeof(VerticesList*));
	assert(*adjacentVerticesLists != NULL);
	for (int i = 0; i < *verticesCount; ++i) {
		(*adjacentVerticesLists)[i] = NULL;
	}
	for (int i = 0; i < *edgesCount; ++i) {
		int vertexFrom;
		int vertexTo;
		int edgeLength;
		if (fscanf(in, "%d %d %d", &vertexFrom, &vertexTo, &edgeLength) != 3) {
			return bad_number_of_lines;
		}
		if (vertexFrom <= 0 || vertexTo <= 0 || vertexFrom > *verticesCount || vertexTo > *verticesCount) {
			return bad_vertex;
		}
		if (edgeLength < 0 || edgeLength > INT_MAX) {
			return bad_length;
		}
		AddEdge(*adjacentVerticesLists, vertexFrom, vertexTo, edgeLength);
		if (vertexFrom != vertexTo) {
			AddEdge(*adjacentVerticesLists, vertexTo, vertexFrom, edgeLength);
		}
	}
	return ok;
}

void DeleteVerticesList(VerticesList *list) {
	if (!list) {
		return;
	}
	DeleteVerticesList(list->next);
	free(list);
}

void DeleteGraph(VerticesList **adjacentVerticesLists, int verticesCount) {
	if (!adjacentVerticesLists) {
		return;
	}
	for (int i = 0; i < verticesCount; ++i) {
		DeleteVerticesList(adjacentVerticesLists[i]);
	}
	free(adjacentVerticesLists);
}
