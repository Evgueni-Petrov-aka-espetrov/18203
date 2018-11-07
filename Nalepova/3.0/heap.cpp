#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "malloc.h"
void Shift(int a[], int size)
{
	int temp;
	for (int i = (size - 1) / 2; i >= 0; i--)
	{
		if (a[2 * i + 2] >= a[i] && (2 * i + 2) < size)
		{
			temp = a[i];
			a[i] = a[2 * i + 2];
			a[2 * i + 2] = temp;
		}
		if (a[2 * i + 1] >= a[i] && (2 * i + 1) < size)
		{
			temp = a[i];
			a[i] = a[2 * i + 1];
			a[2 * i + 1] = temp;
		}
	}

}
void HeapSort(int a[], int size)
{
	int temp;
	Shift(a, size);
	for (int i = size - 1; i >= 0; i--)
	{
		temp = a[0];
		a[0] = a[i];
		a[i] = temp;
		Shift(a, i);
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