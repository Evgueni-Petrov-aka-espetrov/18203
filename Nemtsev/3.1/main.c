#include <stdio.h>
#include <stdlib.h>

void QuickSort(int* A, int numbersCount) {
	int high = numbersCount - 1;
	int low = 0;
	int pivot = A[numbersCount / 2];

	while (high > low) {
		while (A[high] > pivot) --high;
		while (A[low] < pivot) ++low;
		if (high > low) {
			if (A[high] == A[low]) {
				high--;
				low++;
			} else {
				int tmp = A[low];
				A[low] = A[high];
				A[high] = tmp;
			}
		}
	} while (high > low);

	if (high > 0)
        QuickSort(A , high);
	if (numbersCount > low + 1)
	    QuickSort(A + low, numbersCount - low);
}

int main()
{

	int numbersCount;
	scanf("%d", &numbersCount);
	int* A = malloc(sizeof(int)*numbersCount);


	for (int i = 0; i < numbersCount; i++) {
		scanf("%d", &A[i]);
	}

	QuickSort(A, numbersCount);

	for (int i = 0; i < numbersCount; i++) {
		printf("%d ", A[i]);
	}

	free(A);
	return 0;
}
