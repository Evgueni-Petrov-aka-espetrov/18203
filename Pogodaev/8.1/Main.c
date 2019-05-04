#include <stdio.h>
#include <stdlib.h>
#include "graphio.h"
#include "prim.h"

int main() {
	int verticesCount;
	MatrixEdge **adjacentMatrix;
	error_t error = GetGraph(stdin, &verticesCount, &adjacentMatrix);
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
		if (Prim(adjacentMatrix, verticesCount, stdout)) {
			printf("no spanning tree");
		}
		break;
	default:
		printf("unnamed error");
		break;
	}
	DeleteGraph(adjacentMatrix, verticesCount);
	return 0;
}
