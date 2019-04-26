#pragma once
typedef enum { ok, bad_vertex, bad_number_of_lines, bad_number_of_vertices, bad_number_of_edges, bad_length } error_t;
typedef struct VerticesList {
	int vertexNumber;
	int edgeLength;
	struct VerticesList *next;
} VerticesList;

error_t GetGraph(FILE *in, int *verticesCount, int *edgesCount, VerticesList ***adjacentList);

void DeleteGraph(VerticesList **adjacentList, int verticesCount);
