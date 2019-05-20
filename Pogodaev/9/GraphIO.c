#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "graphio.h"
#define max_vertices 5000
#define max_edges(N) N*(N-1)/2

error_t GetGraphCountsAndPath(FILE *in, int *verticesCount, int *pathVertexFrom, int *pathVertexTo, int *edgesCount) {
	if (fscanf(in, "%d", verticesCount) != 1) {
		return bad_number_of_lines;
	}
	if (*verticesCount < 0 || *verticesCount > max_vertices) {
		return bad_number_of_vertices;
	}
	if (fscanf(in, "%d %d", pathVertexFrom, pathVertexTo) != 2) {
		return bad_number_of_lines;
	}
	if (*pathVertexFrom <= 0 || *pathVertexTo <= 0 || *pathVertexFrom > *verticesCount || *pathVertexTo > *verticesCount) {
		return bad_vertex;
	}
	if (fscanf(in, "%d", edgesCount) != 1) {
		return bad_number_of_lines;
	}
	if (*edgesCount < 0 || *edgesCount > max_edges(*verticesCount)) {
		return bad_number_of_edges;
	}
	return ok;
}

void AddEdge(MatrixEdge **adjacentMatrix, int vertexFrom, int vertexTo, int edgeLength) {
	if (vertexFrom >= vertexTo) {
		adjacentMatrix[vertexFrom - 1][vertexTo - 1].isExist = 1;
		adjacentMatrix[vertexFrom - 1][vertexTo - 1].edgeLength = edgeLength;
	}
	else {
		adjacentMatrix[vertexTo - 1][vertexFrom - 1].isExist = 1;
		adjacentMatrix[vertexTo - 1][vertexFrom - 1].edgeLength = edgeLength;
	}
	return;
}

void CreateAdjacentMatrix(MatrixEdge ***adjacentMatrix, int verticesCount) {
	*adjacentMatrix = (MatrixEdge**)malloc(verticesCount * sizeof(MatrixEdge*));
	assert(*adjacentMatrix != NULL);
	for (int i = 0; i < verticesCount; ++i) {
		(*adjacentMatrix)[i] = (MatrixEdge*)malloc((i + 1) * sizeof(MatrixEdge));
		assert((*adjacentMatrix)[i] != NULL);
		for (int j = 0; j < i + 1; ++j) {
			(*adjacentMatrix)[i][j].isExist = 0;
			(*adjacentMatrix)[i][j].edgeLength = 0;
		}
	}
}

error_t GetGraph(FILE *in, int *verticesCount, int *pathVertexFrom, int *pathVertexTo, MatrixEdge ***adjacentMatrix) {
	int edgesCount;
	error_t error = GetGraphCountsAndPath(in, verticesCount, pathVertexFrom, pathVertexTo, &edgesCount);
	if (error != ok) {
		*adjacentMatrix = NULL;
		return error;
	}
	CreateAdjacentMatrix(adjacentMatrix, *verticesCount);
	for (int i = 0; i < edgesCount; ++i) {
		int vertexFrom;
		int vertexTo;
		unsigned int edgeLength;
		if (fscanf(in, "%d %d %u", &vertexFrom, &vertexTo, &edgeLength) != 3) {
			return bad_number_of_lines;
		}
		if (vertexFrom <= 0 || vertexTo <= 0 || vertexFrom > *verticesCount || vertexTo > *verticesCount) {
			return bad_vertex;
		}
		if (edgeLength > INT_MAX) {
			return bad_length;
		}
		AddEdge(*adjacentMatrix, vertexFrom, vertexTo, edgeLength);
	}
	return ok;
}

void DeleteGraph(MatrixEdge **adjacentMatrix, int verticesCount) {
	if (!adjacentMatrix) {
		return;
	}
	for (int i = 0; i < verticesCount; ++i) {
		free(adjacentMatrix[i]);
	}
	free(adjacentMatrix);
}
