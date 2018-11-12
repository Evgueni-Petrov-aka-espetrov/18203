#include <stdio.h>
#include <stdlib.h>
 void quickSort(int *numbers, int left, int right)
{
  int pivot;
  int l_hold = left;
  int r_hold = right;
  pivot = numbers[(right + left) / 2];
  while (left < right)
  {
    while ((numbers[right] >= pivot) && (left < right))
      right--;
    if (left != right)
    {
      numbers[left] = numbers[right];
      left++;
    }
    while ((numbers[left] <= pivot) && (left < right))
      left++;
    if (left != right)
    {
      numbers[right] = numbers[left];
      right--;
    }
  }
  numbers[left] = pivot;
  pivot = left;
  left = l_hold;
  right = r_hold;
  if (left < pivot)
    quickSort(numbers, left, pivot - 1);
  if (right > pivot)
    quickSort(numbers, pivot + 1, right);
}
 int main()
{
    int numbersCount;
    fprintf(stderr, "Enter array size :");
    scanf("%d", &numbersCount);
     int *numbers = malloc(numbersCount *sizeof(numbers[0]));
    int idx;
     for (idx = 0; idx < numbersCount; ++idx)
    {
        scanf("%d", &numbers[idx]);
    }
     quickSort(numbers, 0, numbersCount - 1);
     for (idx = 0; idx < numbersCount; ++idx)
    {
        printf("%d ", numbers[idx]);
    }
   free(numbers);
  getchar();
   return 0;
}
