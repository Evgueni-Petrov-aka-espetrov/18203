#include <stdio.h>
#include <stdlib.h>


void swap(int *array, int l, int r) {
	int temp = array[l];
	array[l] = array[r];
	array[r] = temp;
}

void heapify(int *array, int i, int n) {
	int max = i;
	int left = 2 * i;
	int right = 2 * i + 1;

	if ((right <= n) && (array[right] > array[max])) {
		max = right;
	};

	if ((left <= n) && (array[left] > array[max])) {
		max = left;
	};

	if (max != i) {
		swap(array, i, max);
		heapify(array, max, n);
	}


}

void HeapSort(int *array, int n) {
	int i;
	for (i = n / 2; i > 0; i--) {
		heapify(array, i, n);
	}
	for (i = n; i > 0; i--) {
		swap(array, 1, i);
		if (i!=1)
            heapify(array, 1, i-1);
	}
}

int main() {

	int n, i;
	scanf("%d", &n);
	int *array = malloc(n * sizeof(int));
	for (i = 0; i < n; i++) {
		scanf("%d", &array[i]);
	}
	array--;
	HeapSort(array, n);
	for (i=1;i<=n;i++) {
		printf("%d ", array[i]);
	}
	free(array + 1);
	return 0;
}
