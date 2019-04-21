#pragma once
typedef enum { ok, bad_vertex, bad_number_of_lines, bad_number_of_vertices, bad_number_of_edges } error_t;
typedef struct AdjList {
	int vertexNumber;
	struct AdjList *next;
} VerticesList;

error_t GetGraph(FILE *in, int *verticesCount, VerticesList ***adjacentList);

void DeleteList(VerticesList *list);

void DeleteGraph(VerticesList **adjacentList, int verticesCount);
