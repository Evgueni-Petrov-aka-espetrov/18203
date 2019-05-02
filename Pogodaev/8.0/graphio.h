#pragma once
typedef enum { ok, bad_vertex, bad_number_of_lines, bad_number_of_vertices, bad_number_of_edges, bad_length } error_t;
typedef struct {
	int edgeLength;
	short vertexFrom;
	short vertexTo;
} Edge;

error_t GetGraph(FILE *in, int *verticesCount, int *edgesCount, Edge **graphEdges);

void DeleteGraph(Edge *graphEdges);
