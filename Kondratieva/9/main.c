#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <string.h>
#define VERTICES_MAX_NUMBER 5000 

void freeMatrix(int count, int** matrix) {
	for (int i = 0; i < count; i++) {
		free(matrix[i]);
	}
	free(matrix);
}

int* allocArray(int count, int value) {
	int* arr = malloc(sizeof(int) * count);
	assert(arr);
	for (int i = 0; i < count; i++) {
		arr[i] = value;
	}
	return arr;
}

int** allocMatrix(int count, int value) {
	int** matrix = malloc(sizeof(int*) * count);
	assert(matrix);
	for (int i = 0; i < count; i++) {
		matrix[i] = allocArray(count, -1);
	}
	return matrix;
}

int buildGraph(int vNum, int eNum, int** graph) {
	int begin, end, length;
	for (int i = 0; i < eNum; i++) {
		if (scanf("%d%d%d", &begin, &end, &length) < 3) {
			printf("bad number of lines");
			return 0;
		}
		if (begin < 1 || begin > vNum || end < 1 || end > vNum) {
			printf("bad vertex");
			return 0;
		}
		if (length < 0 || length > INT_MAX) {
			printf("bad length");
			return 0;
		}
		graph[begin - 1][end - 1] = length;
		graph[end - 1][begin - 1] = length;
	}
	return 1;
}

int minDistance(int vNum, int* used, unsigned int* distance) {
	int v = -1;
	for (int u = 0; u < vNum; u++) {
		if (!used[u] && (v == -1 || distance[u] < distance[v])) {
			v = u;
		}
	}
	return v;
}

void findMinPath(int start, int vNum, unsigned int* distance, int* path, int* pathCounter, int** graph) {
	int* used = allocArray(vNum, 0);
	distance[start - 1] = 0;
	path[start - 1] = start - 1;
	pathCounter[start - 1] = 1;
	for (int i = 0; i < vNum; i++) {
		int v = minDistance(vNum, used, distance);
		if (distance[v] == UINT_MAX) {
			return;
		}
		for (int u = 0; u < vNum; u++) {
			if (!used[u] && graph[v][u] != -1) {
				pathCounter[u] += pathCounter[v];
				unsigned int relax = distance[v] + graph[v][u];
				if (relax > INT_MAX) {
					relax = (unsigned int)INT_MAX + 1;
				}
				if (relax < distance[u]) {
					distance[u] = relax;
					path[u] = v;
				}
			}
		}
		used[v] = 1;
	}
	free(used);
}

void printDistance(int vNum, unsigned int* distance) {
	for (int i = 0; i < vNum; i++) {
		if (distance[i] == UINT_MAX) {
			printf("oo ");
		}
		else if (distance[i] > INT_MAX) {
			printf("INT_MAX+ ");
		}
		else {
			printf("%d ", distance[i]);
		}
	}
	printf("\n");
}

void printMinPath(int finish, int* path) {
	int vert = finish - 1;
	printf("%d ", finish);
	while (path[vert] != vert) {
		vert = path[vert];
		printf("%d ", vert + 1);
	}
}

void printResult(int start, int finish, int vNum, int* path, unsigned int* distance, int* pathCounter) {
	printDistance(vNum, distance);
	if (path[finish - 1] == -1) {
		printf("no path");
		return;
	}
	if (distance[finish - 1] > INT_MAX && pathCounter[finish - 1] > 1) {
		printf("overflow");
		return;
	}
	printMinPath(finish, path);
}

void dijkstra(int start, int finish, int vNum, int eNum) {
	int** graph = allocMatrix(vNum, -1);
	if (!buildGraph(vNum, eNum, graph)) {
		freeMatrix(vNum, graph);
		return;
	}
	unsigned int* distance = allocArray(vNum, -1);
	int* path = allocArray(vNum, -1);
	int* pathCounter = allocArray(vNum, 0);
	findMinPath(start, vNum, distance, path, pathCounter, graph);
	printResult(start, finish, vNum, path, distance, pathCounter);
	free(pathCounter);
	free(distance);
	free(path);
	freeMatrix(vNum, graph);
}

int main() {
	int vNum, eNum, finish, start;
	if (scanf("%d%d%d%d", &vNum, &start, &finish, &eNum) < 4) {
		printf("bad number of lines");
		return 0;
	}
	if (vNum < 0 || vNum > VERTICES_MAX_NUMBER) {
		printf("bad number of vertices");
		return 0;
	}
	if (start < 1 || start > vNum || finish < 0 || finish > vNum) {
		printf("bad vertex");
		return 0;
	}
	if (eNum < 0 || eNum > vNum*(vNum - 1) / 2) {
		printf("bad number of edges");
		return 0;
	}
	dijkstra(start, finish, vNum, eNum);
	return 0;
}
