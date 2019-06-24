#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void topsort(int*, int*, int, int, int*, int*);
void PaintArray(int*, int, int);
void CheckInput(int*, int*);

int main() {
	int amount_of_vertices = 0;
	int amount_of_edges = 0;

	CheckInput(&amount_of_vertices, &amount_of_edges);

	int *graph_array = (int *)malloc(amount_of_vertices*amount_of_vertices * sizeof(int));
	memset(graph_array, 0, amount_of_vertices*amount_of_vertices * sizeof(int));

	int *color_array = (int *)malloc(amount_of_vertices * sizeof(int));
	memset(color_array, 0, amount_of_vertices * sizeof(int));

	int* result = (int *)malloc(amount_of_vertices * sizeof(int));
	
	PaintArray(graph_array, amount_of_edges, amount_of_vertices);

	int counter = 0;
	for (int i = 0; i < amount_of_vertices; i++)
		topsort(graph_array, color_array, amount_of_vertices, i, result, &counter);

	for (int i = 0; i < amount_of_vertices; i++)
		printf("%d ", result[i]);

	return 0;
}

void CheckInput(int *amount_of_vertices, int *amount_of_edges) {

	if (scanf("%d", amount_of_vertices) != 1) {
		printf("bad number of lines");
		exit(0);
	}

	if (scanf("%d", amount_of_edges) != 1) {
		printf("bad number of lines");
		exit(0);
	}

	if (*amount_of_vertices > 1000 || *amount_of_vertices < 0) {
		printf("bad number of vertices");
		exit(0);
	}

	if (*amount_of_edges > (*amount_of_vertices*(*amount_of_vertices + 1) / 2) || *amount_of_edges < 0) {
		printf("bad number of edges");
		exit(0);
	}
}

void PaintArray(int *graph_array, int amount_of_edges, int amount_of_vertices) {
	int x = 0, y = 0;
	for (int i = 0; i < amount_of_edges; i++) {
		if (scanf("%d %d", &x, &y) != 2) {
			printf("bad number of lines");
			exit(0);
		}
		if (x<1 || y<1 || x>amount_of_vertices || y>amount_of_vertices) {
			printf("bad vertex");
			exit(0);
		}
		graph_array[(x - 1)*amount_of_vertices + (y - 1)] = 1;
	}
}

void topsort(int* graph_array, int* color_array, int amount_of_vertices, int current_vertex, int* result, int* counter) {
	if (color_array[current_vertex] == 2)
		return;

	if (color_array[current_vertex] == 1) {
		printf("impossible to sort");
		exit(0);
	}
	color_array[current_vertex] = 1;

	for (int i = 0; i < amount_of_vertices; i++) 
		if (graph_array[(i)*amount_of_vertices + current_vertex] == 1) 
			topsort(graph_array, color_array, amount_of_vertices, i, result, counter);

	color_array[current_vertex] = 2;
	result[*counter] = current_vertex + 1;
	(*counter)++;
}
