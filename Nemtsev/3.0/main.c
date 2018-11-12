#include <stdio.h>
#include <stdlib.h>

void heap (int *arr, int length)
{
    int temp;
    for (int i = (length - 1)/2; i >= 0; i--)
    {
        if (arr[2 * i + 2] >= arr[i] && (2 * i + 2) < length )
        {
            temp = arr[i];
            arr[i] = arr[2 * i + 2];
            arr[2 * i + 2] = temp;
        }
    	if (arr[2 * i + 1] >= arr[i] && (2 * i + 1) < length )
        {
            temp = arr[i];
            arr[i] = arr[2 * i + 1];
            arr[2 * i + 1] = temp;
        }

    }
}
void sort(int *arr, int length)
{
    int temp;
    heap (arr, length);
    for (int i = length - 1; i >= 0; i--)
    {
        temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        heap (arr, i);
    }
}
int main()
{
    int length;

    scanf("%d", &length);
    int *arr = malloc(length * sizeof(int));

	for (int i = 0; i < length; i++)
		scanf("%d",&arr[i]);

	sort (arr, length);

	for (int i = 0; i < length; i++)
		printf("%d ", arr[i]);

    free (arr);
    return 0;
}
