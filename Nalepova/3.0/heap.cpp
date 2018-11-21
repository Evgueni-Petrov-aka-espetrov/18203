#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "malloc.h"
void Swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

void Shift(int a[], int size, int i)
{
	int indexOfMax = i;
	if (a[2 * i + 1] >= a[indexOfMax] && (2 * i + 1) < size)
		indexOfMax = 2 * i + 1;
	if (a[2 * i + 2] >= a[indexOfMax] && (2 * i + 2) < size)
		indexOfMax = 2 * i + 2;
		
	if (a[indexOfMax] != a[i])
	{
		Swap(&a[i], &a[indexOfMax]);
		Shift(a, size, indexOfMax);
	}
}
void HeapSort(int a[], int size)
{
	for (int i = size / 2 - 1; i >= 0; i--) 
	{
		Shift(a, size, i);
	}
	for (int i = size - 1; i >= 0; i--)
	{
		Swap(&a[0], &a[i]);
		Shift(a, i, 0);
	}
}
void main()
{
	int *a;
	int n;
	scanf("%d", &n);
	a = (int *)malloc(n * sizeof(int));
	if (a != NULL)
	{
		for (int i = 0; i < n; i++)
			scanf("%d", &a[i]);
		HeapSort(a, n);
		for (int i = 0; i < n; i++)
			printf("%d ", a[i]);
	}
	free(a);
	scanf("%d", &n);
}
