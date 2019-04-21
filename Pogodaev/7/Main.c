#include <stdio.h>
#include <stdlib.h>
#include "topsort.h"

int main() {
	int verticesCount;
	VerticesList **adjacentVerticesLists;
	error_t error = GetGraph(stdin, &verticesCount, &adjacentVerticesLists);
	switch (error) {
		case bad_number_of_vertices: printf("bad number of vertices");
		case bad_number_of_edges: 
			printf("bad number of edges");
			break;
		case bad_vertex: 
			printf("bad vertex");
			break;
		case bad_number_of_lines: 
			printf("bad number of lines");
			break;
		case ok:
			if (TopSort(adjacentVerticesLists, verticesCount, stdout)) {
				printf("impossible to sort");
			}
			break;
		default: 
			printf("unnamed error");
			break;
	}
	DeleteGraph(adjacentVerticesLists, verticesCount);
	return 0;
}


