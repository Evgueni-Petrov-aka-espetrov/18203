#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
void Sort(int *a, int begin, int end)
{
	int temp;
	int left = begin, right = end;
	int pivot = a[(right + left) / 2];

	
	while (left < right)
	{
		while (a[right] > pivot)
			right--;
		while (a[left] < pivot)
			left++;
		if (left <= right)
		{
			temp = a[left];
			a[left] = a[right];
			a[right] = temp;
			left++;
			right--;
		}
	} 
	if (begin < right)
		Sort(a, begin, right);
	if (left < end)
		Sort(a, left, end);
}
void main()
{
	int *a;
	int n;
	scanf("%d", &n);
	a = (int*)malloc(n * sizeof(int));
	for (int i = 0; i < n; i++)
		scanf("%d", &a[i]);

	Sort(a, 0, n - 1);
	for (int i = 0; i < n; i++)
		printf("%d ", a[i]);
	scanf("%d", &n);
}

