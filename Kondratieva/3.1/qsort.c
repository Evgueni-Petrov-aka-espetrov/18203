#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

void swap(int* a, int* b) {
	int t = *a;
	*a = *b;
	*b = t;
}


void quickSort(int* arr, int first, int last) {
	int pivot = 0, left = first, right = last;
	pivot = arr[(left + right) / 2];
	while (left < right) {
		while (arr[left] < pivot) left++;
		while (arr[right] > pivot) right--;
		if (left <= right) {
			swap(&arr[left], &arr[right]);
			left++;
			right--;
		}
	}
	if (first < right) quickSort(arr, first, right);
	if (left < last) quickSort(arr, left, last);
}

int main() {
	int arrSize;
	scanf("%d", &arrSize);
	int* arr = (int*)malloc(sizeof(int)*arrSize);
	for (int i = 0; i < arrSize; i++) {
		scanf("%d", &arr[i]);
	}
	quickSort(arr, 0, arrSize - 1);
	for (int i = 0; i < arrSize; i++) {
		printf("%d ", arr[i]);
	}
	free(arr);
	return 0;
}
