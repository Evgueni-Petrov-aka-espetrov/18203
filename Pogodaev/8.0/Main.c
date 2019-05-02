#include <stdio.h>
#include <stdlib.h>
#include "graphio.h"
#include "kruskal.h"

int main() {
	int verticesCount;
	int edgesCount;
	Edge *graphEdges;
	error_t error = GetGraph(stdin, &verticesCount, &edgesCount, &graphEdges);
	switch (error) {
	case bad_number_of_vertices: printf("bad number of vertices");
	case bad_number_of_edges:
		printf("bad number of edges");
		break;
	case bad_vertex:
		printf("bad vertex");
		break;
	case bad_length:
		printf("bad length");
		break;
	case bad_number_of_lines:
		printf("bad number of lines");
		break;
	case ok:
		if (Kruskal(graphEdges, verticesCount, edgesCount, stdout)) {
			printf("no spanning tree");
		}
		break;
	default:
		printf("unnamed error");
		break;
	}
	DeleteGraph(graphEdges);
	return 0;
}
