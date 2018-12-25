#include "qsort.h"

void swap(int *a, int *b) {
	int t = *a;
	*a = *b;
	*b = t;
}

int Partition(int *arr, int left, int right) {
	// select middle element as a pivot
	int pivot = arr[(left + right) / 2];
	// partitioning
	while (left <= right) {
		// find elements in wrong positions
		while (arr[left] < pivot) ++left;
		while (arr[right] > pivot) --right;
		// and swap them
		if (left <= right) {
			swap(&arr[left], &arr[right]);
			++left;
			--right;
		}
	}
	return left;
}

void QuickSort(int *arr, int left, int right) {
	// part an array
	int point = Partition(arr, left, right);
	// sort left and right parts
	if (right > point) QuickSort(arr, point, right);
	if (left < point - 1) QuickSort(arr, left, point - 1);
}
