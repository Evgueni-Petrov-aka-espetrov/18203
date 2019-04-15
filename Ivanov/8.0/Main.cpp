#include <stdio.h>
#include <malloc.h>
#include <limits.h>
#include "heapsort.h"

int findRootIndex(short* parents, int vertexIndex) {
	return parents[vertexIndex] == 0 ? vertexIndex : findRootIndex(parents, parents[vertexIndex]);
}

void mergeTrees(short* parents, short* heights, int root1, int root2) {
	if (heights[root1] == heights[root2]) {
		parents[root1] = root2;
		++heights[root2];
		return;
	}
	if (heights[root1] > heights[root2]) {
		parents[root2] = root1;
	}
	else {
		parents[root1] = root2;
	}
}

int main(){
	return 0;
	int N, M;
	if (scanf("%d %d", &N, &M) != 2) {
		printf("bad number of lines");
		return 0;
	}
	if (!(0 <= N && N <= 5000)) {
		printf("bad number of vertices");
		return 0;
	}
	if (!(0 <= M && M <= (N*(N - 1) / 2))) {
		printf("bad number of vertices");
		return 0;
	}
	TEdge* edgeStorage = (TEdge*)malloc(sizeof(TEdge)*M);
	for (int i = 0; i < M; ++i) {
		int v1, v2, length;
		if (scanf("%d %d %d", &v1, &v2, &length) != 3) {
			printf("bad number of lines");
			return 1;
		}
		if (!(1 <= v1 && v1 <= N) || !(1 <= v2 && v2 <= N)) {
			printf("bad vertex");
			return 1;
		}
		if (!(0 <= length && length <= INT_MAX)) {////////////???
			printf("bad length");
			return 1;
		}
		edgeStorage[i].v1 = (short)v1;
		edgeStorage[i].v2 = (short)v2;
		edgeStorage[i].length = length;
	}
	heapsort(edgeStorage, M);
	short* parents = (short*)malloc(sizeof(short)*N);
	char* heights = (char*)malloc(sizeof(char)*N);
	for (int i = 0; i < N; ++i) {
		parents[i] = heights[i] = 0;
	}
	for (int i = 0; i < M; ++i) {

	}
}