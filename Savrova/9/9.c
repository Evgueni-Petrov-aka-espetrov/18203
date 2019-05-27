#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <string.h>

int Check1 (int start, int finish, int length, int vertex) {
    if (scanf("%d%d%d", &start, &finish, &length) < 3) {
			printf("bad number of lines");
			return 0;
		}
		if (start < 1 || start > vertex || finish < 1 || finish > vertex) {
			printf("bad vertex");
			return 0;
		}
		if (length < 0 || length > INT_MAX) {
			printf("bad length");
			return 0;
		}
		return 1;
    }

int Check2 (int vertex, int edge,int start,int finish){
    if (scanf("%d%d%d%d", &vertex, &start, &finish, &edge) < 4) {
		printf("bad number of lines");
		return 0;
	}
	if (vertex < 0 || vertex > 5000) {
		printf("bad number of vertices");
		return 0;
	}
	if (start < 1 || start > vertex || finish < 0 || finish > vertex) {
		printf("bad vertex");
		return 0;
	}
	if (edge < 0 || edge > vertex*(vertex - 1) / 2) {
		printf("bad number of edges");
		return 0;
	}
	return  1;
    }

    void freegraph(int count, int** graph) {
	for (int i = 0; i < count; i++) {
		free(graph[i]);
	}
	free(graph);
}

int BuildGraph(int vertex, int edge, int **graph){
    int length, start, finish;
	for (int i = 0; i < edge; i++){
		if (Check1(start, finish, length, vertex)){
		graph[start - 1][finish - 1] = length;
		graph[finish - 1][start - 1] = length;
		}
	}
	return 1;
}
int MinLength(int vertex, int* used, unsigned int* distance) {
	int l = -1;
	for (int i = 0; i < vertex; i++) {
		if (!used[i] && (l == -1 || distance[i] < distance[l])) {
			l = i;
		}
	}
	return l;
}

void findMinPath(int start, int vertex, unsigned int* distance, int* path, int* pathcount, int** graph) {
	int *used = malloc(sizeof(int) * vertex);
	for (int i = 0; i < vertex; i++) { // memset????????
		used[i] = 0;
	}
	distance[start - 1] = 0;
	path[start - 1] = start - 1;
	pathcount[start - 1] = 1;
	for (int i = 0; i < vertex; i++) {
		int l = MinLength(vertex, used, distance);

		if (distance[l] == UINT_MAX) {
			return;
		}

		for (int v = 0; v < vertex; v++) {
			if (!used[v] && graph[l][v] != -1) {
				pathcount[v] += pathcount[l];
				unsigned int dif = distance[l] + graph[l][v];
				if (dif > INT_MAX) {
					 dif = (unsigned int)INT_MAX + 1;
				}
				if (dif < distance[v]) {
					distance[v] = dif;
					path[v] = l;
				}
			}
		}
		used[l] = 1;
	}
	free(used);
}

void printResult(int start, int finish, int vertex, int* path, unsigned int* distance, int* pathcount) {
	for (int i = 0; i < vertex; i++) {
		if (distance[i] == UINT_MAX)
			printf("oo");
		else if (distance[i] > INT_MAX)
			printf("INT_MAX+");
		else
			printf("%d", distance[i]);
	}
	printf("\n");
	if (path[finish - 1] == -1) {
		printf("no path");
		return;
	}
	if (distance[finish - 1] > INT_MAX && pathcount[finish - 1] > 1) {
		printf("overflow");
		return;
	}
	int v = finish - 1;
	printf("%d", finish);
	while (path[v] != v) {
		v = path[v];
		printf("%d", v + 1);
	}
}

void dijkstra(int start, int finish, int vertex, int edge) {
	int **graph = malloc(sizeof(int*) * vertex);
	for (int i = 0; i < vertex; i++) {
        graph[i] = malloc(sizeof(int) * vertex);
        for (int j = 0; j < vertex; j++) {
            *graph[j] = -1;
        }
	}
	if (!BuildGraph(vertex, edge, graph)) {
		freegraph(vertex, graph);
		return;
	}
	unsigned int* distance = malloc(sizeof(int) * vertex);
	for (int i = 0; i < vertex; i++) {
		distance[i] = -1;
	}
	int* path = malloc(sizeof(int) * vertex);
	for (int i = 0; i < vertex; i++) {
		path[i] = -1;
	}
	int* pathcount = malloc(sizeof(int) * vertex);
	for (int i = 0; i < vertex; i++) {
		pathcount[i] = 0;
	}
	findMinPath(start, vertex, distance, path, pathcount, graph);
	printResult(start, finish, vertex, path, distance, pathcount);
	free(pathcount);
	free(distance);
	free(path);
	freegraph(vertex, graph);
}

int main() {
	int vertex, edge, start, finish;
	if (Check2(vertex, edge, start, finish)){
        dijkstra(start, finish, vertex, edge);
	}
	return 0;
}
