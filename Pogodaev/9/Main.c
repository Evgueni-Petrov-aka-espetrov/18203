#include "graphio.h"
#include "dijkstra.h"

int main() {
	int verticesCount;
	int pathVertexFrom;
	int pathVertexTo;
	MatrixEdge **adjacentMatrix;
	error_t error = GetGraph(stdin, &verticesCount, &pathVertexFrom, &pathVertexTo, &adjacentMatrix);
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
		Dijkstra(adjacentMatrix, verticesCount, pathVertexFrom, pathVertexTo);
		break;
	default:
		printf("unnamed error");
		break;
	}
	DeleteGraph(adjacentMatrix, verticesCount);
	return 0;
}
