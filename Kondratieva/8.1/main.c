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
		matrix[i] = allocArray(count, value);
	}
	return matrix;
}

int buildGraph(int vNum, int eNum, unsigned int** graph) {
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
		if (length < 0 || length > INT_MAX){
			printf("bad length");
			return 0;
		}
		graph[begin - 1][end - 1] = length;
		graph[end - 1][begin - 1] = length;
	}	
   	return 1;
}

int minVertex(int vNum, int* used, unsigned int* distance) {
	int v = -1;
	for (int u = 0; u < vNum; u++) {
		if (!used[u] && (v == -1 || distance[u] < distance[v])) {
			v = u;
		}
	}
	return v;
}

int findMST(int vNum, int* MST, unsigned int** graph) {
	unsigned int* distance = allocArray(vNum, -1);
	int* used = allocArray(vNum, 0);
	distance[0] = 0;
	for (int i = 0; i < vNum; i++) {
		int v = minVertex(vNum, used, distance);
		if (distance[v] == UINT_MAX) {
			return 0;
		}
		used[v] = 1;
		for (int u = 0; u < vNum; u++) {
			if (graph[v][u] < distance[u] && !used[u]) {
				distance[u] = graph[v][u];
				MST[u] = v;
			}
		}
	}
	free(distance);
	free(used);
	return 1;
}

void printMST(int vNum, int* MST) {
	for (int i = 0; i < vNum; i++) {
		if (MST[i] != -1) {
			printf("%d %d\n", i + 1, MST[i] + 1);
		}
	}
}

void prim(int vNum, int eNum) {
	unsigned int** graph = allocMatrix(vNum, -1);
	if (!buildGraph(vNum, eNum, graph)) {
		freeMatrix(vNum, graph);
		return;
	}
	int* MST = allocArray(vNum, -1);
	if (!vNum || !findMST(vNum, MST, graph)) {
		printf("no spanning tree");
	}
	else {
		printMST(vNum, MST);
	}
	free(MST);
	freeMatrix(vNum, graph);
}

int main() {
	int vNum, eNum;
	if (scanf("%d%d", &vNum, &eNum) < 2) {
		printf("bad number of lines");
		return 0;
	}
	if (vNum < 0 || vNum > VERTICES_MAX_NUMBER) {
		printf("bad number of vertices");
		return 0;
	}
	if (eNum < 0 || eNum > vNum*(vNum - 1) / 2) {
		printf("bad number of edges");
		return 0;
	}
	prim(vNum, eNum);
	return 0;
}
