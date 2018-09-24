#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "qsort.h"

int main() {
	// Input
	// 1 string (Array length)
	int arrLength = 0;
	if (scanf("%d", &arrLength) != 1 || arrLength < 0) {
		printf("bad input");
		exit(0);
	}

	// Allocating memory for array
	int *arr;
	if ((arr = (int*)malloc(arrLength * sizeof(int))) == NULL) {
		printf("out of memory");
		exit(0);
	}
	// 2 string (Array to sort)
	for (int i = 0; i < arrLength; ++i) {
		if (scanf("%d", &arr[i]) != 1) {
			printf("bad input");
			exit(0);
		}
	}

	// Sorting
	QuickSort(arr, 0, arrLength - 1);
	
	// Output
	for (int i = 0; i < arrLength; ++i) {
		printf("%d ", arr[i]);
	}

	// Free allocated memory
	free(arr);
	return 0;
}