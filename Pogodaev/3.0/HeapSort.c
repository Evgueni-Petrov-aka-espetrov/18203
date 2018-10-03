void swap(int *a, int *b) {
	int t = *a;
	*a = *b;
	*b = t;
}

int Parent(int number) {
	return number / 2;
}

int LeftChild(int number) {
	return number * 2;
}

int RightChild(int number) {
	return number * 2 + 1;
}

void Heapify(int *arr, int heapSize, int number) {
	int max = number;
	if (LeftChild(number) <= heapSize && arr[LeftChild(number) - 1] > arr[number - 1]) max = LeftChild(number);
	if (RightChild(number) <= heapSize && arr[RightChild(number) - 1] > arr[max - 1]) max = RightChild(number);
	if (max != number) {
		swap(&arr[max - 1], &arr[number - 1]);
		Heapify(arr, heapSize, max);
	}
}

void BuildHeap(int *arr, int arrLength) {
	for (int i = arrLength / 2; i > 0; --i) {
		Heapify(arr, arrLength, i);
	}
}

void HeapSort(int* arr, int arrLength) {
	BuildHeap(arr, arrLength);
	int heapSize = arrLength;
	for (int i = arrLength; i > 1; --i) {
		swap(&arr[0], &arr[i - 1]);
		--heapSize;
		Heapify(arr, heapSize, 1);
	}
}