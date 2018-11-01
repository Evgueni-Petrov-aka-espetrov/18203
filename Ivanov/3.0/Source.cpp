#include <stdio.h>
#include <stdlib.h>

void swap(int* a, int* b) {
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void sort(int* arr, int numberOfElements) {
	for (int i = numberOfElements / 2 - 1; i >= 0; --i) {
		for (int index = i;;) {
			if (index * 2 + 2 >= numberOfElements) {
				if (index * 2 + 1 >= numberOfElements) break;
				else {  // in case if node has only one child
					if (arr[index * 2 + 1] > arr[index])
						swap(arr + index * 2 + 1, arr + index);
					break;
				}
			}
			int nextIndex = arr[index * 2 + 1] < arr[index * 2 + 2] ? index * 2 + 2 : index * 2 + 1;
			if (arr[nextIndex] >= arr[index])
				swap(arr + index, arr + nextIndex);
			else break;
			index = nextIndex;
		}
	}
	for (int i = numberOfElements - 1; i > 0; --i) {
		--numberOfElements;
		swap(arr + i, arr);
		for (int index = 0;;) {
			if (index * 2 + 2 >= numberOfElements) {
				if (index * 2 + 1 >= numberOfElements) break;
				else {
					if (arr[index * 2 + 1] > arr[index])
						swap(arr + index * 2 + 1, arr + index);
					break;
				}
			}
			int nextIndex = arr[index * 2 + 1] < arr[index * 2 + 2] ? index * 2 + 2 : index * 2 + 1;
			if (arr[nextIndex] >= arr[index])
				swap(arr + index, arr + nextIndex);
			else break;
			index = nextIndex;
		}
	}
}

int main() {
	int n;
	scanf("%d", &n);
	int* arr = (int*)malloc(sizeof(int)*n);
	if (arr == NULL) {
		printf("no memory");
		return 1;
	}
	for (int i = 0; i < n; ++i)
		scanf("%d", arr + i);
	sort(arr, n);
	for (int i = 0; i < n; ++i)
		printf("%d ", arr[i]);
	free(arr);
	return 0;
}