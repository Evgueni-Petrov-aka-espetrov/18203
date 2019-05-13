#include <stdio.h>
#include <stdlib.h>

int findMinUnvisitedIndex(int* minDistanceTable, char* verticesState, int N){
	int minIndex = -1;
	int minDistance;
	int i = 0;
	for (; i < N; ++i){
		if (verticesState[i] == 0 && minDistanceTable[i] != -1){
			minIndex = i;
			minDistance = minDistanceTable[i];
			break;
		}
	}
	for (; i < N; ++i){
		if (verticesState[i] == 0 && minDistanceTable[i] != -1 && minDistanceTable[i] < minDistance){
			minIndex = i;
			minDistance = minDistanceTable[i];
		}
	}
	return minIndex;
}

int main(){
	int N, M, S, F;
	if (scanf("%d %d", &N, &S, &F, &M) != 4) {
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
	int* minDistanceTable = (int*)malloc(N * sizeof(int));
	short* previousVertices = (short*)calloc(N, sizeof(short));
	char* verticesState = (char*)calloc(N, sizeof(char));
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
	for (int i = 0; i < N; ++i){
		minDistanceTable[i] = -1;
	}
	minDistanceTable[S-1] = 0;
	int currentVerticeIndex = S-1;
	while (currentVerticeIndex != -1){
		int currentDistance = minDistanceTable[currentVerticeIndex];
		for (int i = 0; i < N; ++i){
			if (table[N*currentVerticeIndex + i] != 0 && (minDistanceTable[i] > currentDistance + table[N*currentVerticeIndex + i] || minDistanceTable[i] == -1)){
				minDistanceTable[i] = currentDistance + table[N*currentVerticeIndex + i];
				previousVertices[i] = currentVerticeIndex + 1;
			}
			verticesState[currentVerticeIndex] = 1;
		}
		currentVerticeIndex = findMinUnvisitedIndex(minDistanceTable, verticesState, N);
	}

	for (int i = 0; i < N; ++i){
		if (minDistanceTable[i] == -1){
			printf("oo ");
		}
		else{
			printf("%d ", minDistanceTable[i]);
		}
	}
	printf("\n");

	return 0;
}