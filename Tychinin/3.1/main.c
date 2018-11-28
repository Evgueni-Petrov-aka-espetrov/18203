#include <stdio.h>
#include <stdlib.h>

void QuickSort(int nums[], int left, int right);

int main(void)
{
    int *nums;
    int n, i;

    scanf("%d", &n);
    nums = malloc(sizeof(int) * n);
    for (i = 0; i < n; ++i) {
        scanf("%d", &nums[i]);
    }

    QuickSort(nums, 0, n-1);

    for (i = 0; i < n; ++i) {
        printf("%d ", nums[i]);
    }
    free(nums);

    return 0;
}

void QuickSort(int nums[], int left, int right)
{
    int pivot, i, j, tmp;

    i = left;
    j = right;
    pivot = nums[(left + right) / 2];
    if (left < right) {
        while (i <= j) {
            while (nums[i] < pivot) ++i;
            while (nums[j] > pivot) --j;
            if (i <= j) {
                tmp = nums[i];
                nums[i] = nums[j];
                nums[j] = tmp;
                ++i;
                --j;
            }
        }
        QuickSort(nums, left, j);
        QuickSort(nums, i, right);
    }
}