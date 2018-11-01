#include <stdio.h>
#include <stdlib.h>

void swap(int* a, int* b) {
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

int split(int*arr, int lb, int rb) {
	int baseIndex = lb + (rb - lb) / 2;
	int baseElement = arr[baseIndex];
	int l = lb;
	int r = rb;
	for (;;) {
		for (;; ++l) {
			if (l == r) {

				swap(arr + baseIndex, arr + l);
				return l;
			}
			if (arr[l] >= baseElement) break;
		}
		for (;; --r) {
			if (l == r) {
				swap(arr + baseIndex, arr + l);
				return l;
			}
			if (arr[r] < baseElement) break;
		}
		if (baseIndex == l) {
			baseIndex = r;
		}
		else if (baseIndex == r) {
			baseIndex = l;
		}
		swap(arr + l, arr + r);
	}
}

void qsort(int* arr, int lb, int rb) {
	if (rb - lb < 1) return;
	int baseIndex = split(arr, lb, rb);
	qsort(arr, lb, baseIndex - 1);
	qsort(arr, baseIndex + 1, rb);
}

int main() {
	int n;
	scanf("%d", &n);
	int* arr = (int*)malloc(sizeof(int)*n);
	for (int i = 0; i < n; ++i) {
		scanf("%d", arr + i);
	}

	qsort(arr, 0, n - 1);

	for (int i = 0; i < n; ++i) {
		printf("%d ", arr[i]);
	}

	free(arr);
	return 0;
}