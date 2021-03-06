#include <stdio.h>
#include <cstdlib>

void qsort(int* a, int rightIndex);
void swap(int* a, int* b);

int main() {
	int length;
	scanf_s("%d /n", &length);
	int* a = (int*)malloc(sizeof(int) * length);
	if (a) {
		for (int i = 0; i != length; i++) { scanf_s("%d", &a[i]); }

		if (length >= 1)
			qsort(&a[0], length - 1);

		for (int i = 0; i != length; i++) {
			printf_s("%d %c", a[i], ' ');
		}

		free(a);
		return(0);
	}
	else return -1;
}

void qsort(int* a, int rightIndex) {
	int iLeft = 0; int iRight = rightIndex; int b = *(a + rightIndex);	//b - "граничный" элемент
	do {											//собст. сортировка
		while (*(a + iLeft) < b) iLeft++;	//a[iLeft] - первый слева элем >b
		while (*(a + iRight) > b) iRight--;	//a[iRight] - первый справа элем <=b
		if (iLeft <= iRight) {
			swap(a + iLeft, a + iRight);
			iLeft++;
			iRight--;
		}
	} while (iLeft <= iRight);
	if (iRight > 0) qsort(a, iRight);
	if (iLeft < rightIndex) qsort(a + iLeft, rightIndex - iLeft);
}

void swap(int* a, int* b) {
	int t = *a;
	*a = *b;
	*b = t;
}