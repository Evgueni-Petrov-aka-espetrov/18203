#include "TEdge.h"
void swap(TEdge* a, TEdge* b) {
	TEdge tmp = *a;
	*a = *b;
	*b = tmp;
}

void heapsort(TEdge* arr, int numberOfElements) {
	for (int i = numberOfElements / 2 - 1; i >= 0; --i) {
		for (int index = i;;) {
			if (index * 2 + 2 >= numberOfElements) {
				if (index * 2 + 1 >= numberOfElements) break;
				else {  // in case if node has only one child
					if (arr[index * 2 + 1].length > arr[index].length)
						swap(arr + index * 2 + 1, arr + index);
					break;
				}
			}
			int nextIndex = arr[index * 2 + 1].length < arr[index * 2 + 2].length ? index * 2 + 2 : index * 2 + 1;
			if (arr[nextIndex].length >= arr[index].length)
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
					if (arr[index * 2 + 1].length > arr[index].length)
						swap(arr + index * 2 + 1, arr + index);
					break;
				}
			}
			int nextIndex = arr[index * 2 + 1].length < arr[index * 2 + 2].length ? index * 2 + 2 : index * 2 + 1;
			if (arr[nextIndex].length >= arr[index].length)
				swap(arr + index, arr + nextIndex);
			else break;
			index = nextIndex;
		}
	}
}