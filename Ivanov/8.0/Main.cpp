#include <stdio.h>
#include <malloc.h>
#include <limits.h>
#include "heapsort.h"

int findRootIndex(short* parents, int vertexIndex) {
	return parents[vertexIndex] == vertexIndex ? vertexIndex : findRootIndex(parents, parents[vertexIndex]);
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
		printf("bad number of edges");
		return 0;
	}
	if (N == 1 && M == 0) {
		return 0;
	}
	if (M == 0) {
		printf("no spanning tree");
		return 0;
	}
	TEdge* edgeStorage = (TEdge*)malloc(sizeof(TEdge)*M);
	for (int i = 0; i < M; ++i) {
		int v1, v2;
		long long length;
		if (scanf("%d %d %lld", &v1, &v2, &length) != 3) {
			printf("bad number of lines");
			return 0;
		}
		if (!(1 <= v1 && v1 <= N) || !(1 <= v2 && v2 <= N)) {
			printf("bad vertex");
			return 0;
		}
		if (!(0 <= length && length <= INT_MAX)) {
			printf("bad length");
			return 0;
		}
		edgeStorage[i].v1 = (short)v1;
		edgeStorage[i].v2 = (short)v2;
		edgeStorage[i].length = (int)length;
	}
	heapsort(edgeStorage, M);
	short* parents = (short*)malloc(sizeof(short)*N);
	short* heights = (short*)malloc(sizeof(short)*N);
	for (int i = 0; i < N; ++i) {
		parents[i] = i;
		heights[i] = 0;
	}
	TEdge** resultStorage = (TEdge**)malloc(sizeof(TEdge*)*(N-1));
	int resultStorageSize = 0;
	for (int i = 0; i < M; ++i) {
		if (findRootIndex(parents, edgeStorage[i].v1-1) != findRootIndex(parents, edgeStorage[i].v2-1)) {
			resultStorage[resultStorageSize++] = edgeStorage + i;
			mergeTrees(parents, heights, findRootIndex(parents, edgeStorage[i].v1 - 1), findRootIndex(parents, edgeStorage[i].v2 - 1));
		}
	}
	for (int i = 1; i < N; ++i){
		if (findRootIndex(parents, i) != findRootIndex(parents, 0)){
			printf("no spanning tree");
			return 0;
		}
	}
	for (int i = 0; i < resultStorageSize; ++i){
		printf("%d %d\n", resultStorage[i]->v1, resultStorage[i]->v2);
	}

	free(edgeStorage);
	free(resultStorage);
	free(heights);
	free(parents);
	return 0;
}