#include <stdio.h>
#include <stdlib.h>

inline int greaterThan(long long a, long long b) { // -2 <-> greater than INT_MAX, -1 <-> infinity
	if (a == b) return 0;
	if (a == -1) return 1;
	if (b == -1) return 0;
	if (a == -2) return 1;
	if (b == -2) return 0;
	return a > b ? 1 : 0;
}

int findMinUnvisitedIndex(int* minDistanceTable, char* verticesState, int N) {
	int minIndex = -1;
	int minDistance = -1;
	for (int i = 0; i < N; ++i) {
		if (verticesState[i] == 0 && greaterThan(minDistance, minDistanceTable[i])) {
			minIndex = i;
			minDistance = minDistanceTable[i];
		}
	}
	return minIndex;
}

int main() {
	int N, M, S, F;
	if (scanf("%d %d %d %d", &N, &S, &F, &M) != 4) {
		printf("bad number of lines");
		return 0;
	}
	if (!(0 <= N && N <= 5000)) {
		printf("bad number of vertices");
		return 0;
	}
	if (!(1 <= S && S <= N) || !(1 <= F && F <= N)) {
		printf("bad vertex");
		return 0;
	}
	if (!(0 <= M && M <= (N*(N - 1) / 2))) {
		printf("bad number of edges");
		return 0;
	}
	int* table = (int*)calloc(N*N, sizeof(int));
	short* previousVertices = (short*)calloc(N, sizeof(short));
	char* verticesState = (char*)calloc(N, sizeof(char));
	int* minDistanceTable = (int*)malloc(N * sizeof(int));
	for (int i = 0; i < N; ++i) {
		minDistanceTable[i] = -1;
	}
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

	minDistanceTable[S - 1] = 0;
	int currentVerticeIndex = S - 1;
	while (currentVerticeIndex != -1) {
		int currentDistance = minDistanceTable[currentVerticeIndex];
		for (int i = 0; i < N; ++i) {
			if (table[N*currentVerticeIndex + i] != 0 && greaterThan(minDistanceTable[i], (currentDistance == -2 || (long long)currentDistance + (long long)table[N*currentVerticeIndex + i] > INT_MAX ? -2 : currentDistance + table[N*currentVerticeIndex + i]))) {
				minDistanceTable[i] = currentDistance == -2 || (long long)currentDistance + (long long)table[N*currentVerticeIndex + i] > INT_MAX ? -2 : currentDistance + table[N*currentVerticeIndex + i];
				previousVertices[i] = currentVerticeIndex + 1;
			}
			verticesState[currentVerticeIndex] = 1;
		}
		currentVerticeIndex = findMinUnvisitedIndex(minDistanceTable, verticesState, N);
	}

	for (int i = 0; i < N; ++i) {
		if (minDistanceTable[i] == -1) {
			printf("oo ");
		}
		else if (minDistanceTable[i] == -2) {
			printf("INT_MAX+ ");
		}
		else {
			printf("%d ", minDistanceTable[i]);
		}
	}
	printf("\n");
	if (minDistanceTable[F - 1] == -1) {
		printf("no path");
	}
	else if (minDistanceTable[F - 1] == -2) {
		printf("overflow");
	}
	else {
		int currentVertice = F;
		while (currentVertice != 0) {
			printf("%d ", currentVertice);
			currentVertice = previousVertices[currentVertice - 1];
		}
	}

	return 0;
}