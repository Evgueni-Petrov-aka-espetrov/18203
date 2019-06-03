#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include <string.h>

void Check(int v, int e){

	if (v > 1000 || v < 0) {
		printf("bad number of vertices\n");
		exit(0);
	}
	if (e > e*(e + 1)/2 || e < 0) {
		printf("bad number of edges\n");
		exit(0);
	}
}

void topsort(int* graph, int* colors, int vertex, int currentvertex, int* result, int* count ) {

	if (colors[currentvertex] == 2)
		return;
	if (colors[currentvertex] == 1) {
		printf("impossible to sort\n");
		exit(0);
	}
	colors[currentvertex] = 1;
	for (int i = 0; i < vertex; i++) {
		if (graph[i*vertex + currentvertex] == 1)
			topsort(graph, colors, vertex, i, result, count);
	}
	colors[currentvertex] = 2;
	result[*count] = currentvertex + 1;
	(*count)++;
}
int main(){
	int vertex = 0;
	if (scanf("%d", &vertex) != 1) {
		printf("bad number of lines\n");
		exit(0);
	}
	int edge = 0;
	if (scanf("%d", &edge) != 1) {
		printf("bad number of lines\n");
		exit(0);
	}
	Check(vertex, edge);
    int* graph = (int*)malloc(vertex * vertex * sizeof(int));
	memset (graph, 0, vertex * vertex* sizeof(int));
    int s = 0,f = 0;
	for (int i = 0; i < edge; i++){
		if (scanf("%d %d", &s, &f) != 2) {
			printf("bad number of lines\n");
			return 0;
		}
        if (s < 1 || s > vertex || f < 1 || f > vertex){
		printf("%s", "bad vertex");
		return 0;
        }
		graph[(s-1)*vertex + (f-1)] = 1;
	}
	int *colors = (int*)malloc(vertex * sizeof(int));
	memset(colors, 0, vertex * sizeof(int));
	int* result = (int*)malloc(vertex * sizeof(int));
	int count = 0;
	for (int i = 0; i < vertex; i++) {
		topsort(graph, colors, vertex, i, result, &count);
	}
	for (int i = 0; i < vertex; i++) {
		printf("%d ", result[i]);
	}
	return 0;
}
