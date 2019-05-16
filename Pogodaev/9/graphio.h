#pragma once
#include <stdio.h>
typedef enum { ok, bad_vertex, bad_number_of_lines, bad_number_of_vertices, bad_number_of_edges, bad_length } error_t;
typedef struct {
	unsigned int edgeLength;
	unsigned char isExist;
} MatrixEdge;

error_t GetGraph(FILE *in, int *verticesCount, int *pathVertexFrom, int *pathVertexTo, MatrixEdge ***adjacentMatrix);

void DeleteGraph(MatrixEdge **adjacentMatrix, int verticesCount);
