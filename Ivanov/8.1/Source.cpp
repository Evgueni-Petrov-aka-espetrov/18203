#include <stdio.h>
#include <malloc.h>
#include <limits.h>

inline int findMinDistanceIndex(int* minDistanceTable, int N) {
	if (N == 0 || minDistanceTable == NULL) {
		return -1;
	}

	int min;
	int minIndex = -1;
	for (int i = 0; i < N; ++i) {
		if (minIndex == -1) {
			if (minDistanceTable[i] != 0) {
				minIndex = i;
				min = minDistanceTable[i];
			}
			continue;
		}
		if (minDistanceTable[i] != 0 && minDistanceTable[i] < min) {
			min = minDistanceTable[i];
			minIndex = i;
		}
	}
	return minIndex;
}

inline void recomputeMinDistanceTable(int N, int* table, int* minDistanceTable, char* verticesState, int newVertexIndex) {
	for (int i = 0; i < N; ++i) {
		if (table[N*newVertexIndex + i] != 0 && verticesState[i] == 0 && (minDistanceTable[i] == 0 || table[N*newVertexIndex + i] < minDistanceTable[i])) {
			minDistanceTable[i] = table[N*newVertexIndex + i];
			minDistanceTable[N + i] = newVertexIndex;
		}
	}
}

int main() {
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
	int* table = (int*)calloc(N*N, sizeof(int));
	int* minDistanceTable = (int*)calloc(N*2, sizeof(int));
	char* verticesState = (char*)calloc(N, sizeof(char));
	short* resultEdges = (short*)malloc(sizeof(short)*(N - 1) * 2);
	int resultCount = 0;
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
		table[(v1 - 1)*N + (v2 - 1)] = table[(v2 - 1)*N + (v1 - 1)] = (int)length;
	}
	for (int index = 0;;) {
		verticesState[index] = 1;
		minDistanceTable[index] = 0;
		recomputeMinDistanceTable(N, table, minDistanceTable, verticesState, index);
		index = findMinDistanceIndex(minDistanceTable, N);
		if (index == -1) {
			break;
		}		
		resultEdges[resultCount] = index + 1;
		resultEdges[N - 1 + resultCount] = minDistanceTable[N + index] + 1;
		++resultCount;
	}
	if (resultCount != (N - 1)) {
		printf("no spanning tree");
		return 0;
	}
	else {
		for (int i = 0; i < (N - 1); ++i) {
			printf("%d %d\n", resultEdges[i], resultEdges[N - 1 + i]);
		}
	}

	return 0;
}