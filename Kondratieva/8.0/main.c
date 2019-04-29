#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#define VERTECES_MAX_NUMBER 5000 

typedef struct TEdge {
	int begin;
	int end;
	int length;
	int used;
} Edge;

int fillEdges(int vNum, int eNum, Edge* edges) {
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
		edges[i].begin = begin - 1;
		edges[i].end = end - 1;
		edges[i].length = length;
		edges[i].used = 0;
	}
	return 1;
}

int compare(const Edge* a, const Edge* b) {
	return a->length - b->length;
}

int minSpanningTree(int vNum, int eNum, Edge* edges) {
	if (!vNum) {
		return 0;
	}
	qsort(edges, eNum, sizeof(Edge), compare);
	int* components = malloc(sizeof(int) * vNum);
	assert(components);
	for (int i = 0; i < vNum; i++) {
		components[i] = i;
	}
	for (int i = 0; i < eNum; i++) {
		int compBegin = components[edges[i].begin];
		int compEnd = components[edges[i].end];
		if (compBegin != compEnd) {
			edges[i].used = 1;
			for (int j = 0; j < vNum; j++) {
				if (components[j] == compBegin) {
					components[j] = compEnd;
				}
			}
		}
	}
	for (int i = 1; i < vNum; i++) {
		if (components[0] != components[i]) {
			free(components);
			return 0;
		}
	}
	free(components);
	return 1;
}

void printSpanningTree(int eNum, Edge* edges) {
	for (int i = 0; i < eNum; i++) {
		if (edges[i].used) {
			printf("%d %d\n", edges[i].begin + 1, edges[i].end + 1);
		}
	}
}

void kruskal(int vNum, int eNum) {
	Edge* edges = malloc(sizeof(Edge) * eNum);
	assert(edges);
	if (!fillEdges(vNum, eNum, edges)) {
		return;
	}
	if (minSpanningTree(vNum, eNum, edges)) {
		printSpanningTree(eNum, edges);
	}
	else {
		printf("no spanning tree");
	}
	free(edges);
}

int main() {
	int vNum, eNum;
	if (scanf("%d%d", &vNum, &eNum) < 2) {
		printf("bad number of lines");
		return 0;
	}
	if (vNum < 0 || vNum > VERTECES_MAX_NUMBER) {
		printf("bad number of vertices");
		return 0;
	}
	if (eNum < 0 || eNum > vNum*(vNum - 1) / 2) {
		printf("bad number of edges");
		return 0;
	}
	kruskal(vNum, eNum);
	return 0;
}