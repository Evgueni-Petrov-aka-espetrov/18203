#include <stdio.h>
#include <stdlib.h>

void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int* getElements(int* arr, int arrSize) {
    arr = (int*) malloc(sizeof(int)*arrSize);
    for (int i = 0; i < arrSize; i++) {
        scanf("%d", &arr[i]);
    }
    return arr;
}

void printAnswer(int* arr, int arrSize) {
    for (int i = 0; i < arrSize; i++) {
        printf("%d ", arr[i]);
    }
}

int partition(int* arr, int left, int right) {
    int pivot = arr[(left + right)/2];
    while (left <= right) {
        while(arr[left] < pivot) left++;
        while(arr[right] > pivot) right--;
        if (left <= right) {
            swap(&arr[left], &arr[right]);
            left++;
            right--;
        }
    }
    return left;
}

void quickSort(int* arr, int left, int right) {
    int delimiter = partition(arr, left, right);
    if (delimiter < right)
        quickSort(arr, delimiter, right);
    if (delimiter - 1 > left)
        quickSort(arr, left, delimiter - 1);
}

int main() {
    int arrSize;
    int* arr;
    scanf("%d", &arrSize);
    arr = getElements(arr, arrSize);
    quickSort(arr, 0, arrSize - 1);
    printAnswer(arr, arrSize);
    free(arr);
    return 0;
}
