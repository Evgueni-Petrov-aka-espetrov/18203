void swap(int *a, int *b) {
	int t = *a;
	*a = *b;
	*b = t;
}

void QuickSort(int *arr, int left, int right) {
	int pivot = arr[(left + right) / 2];
	int l = left, r = right;
	do {
		while (arr[l] < pivot) ++l;
		while (arr[r] > pivot) --r;
		if (l <= r) {
			swap(&arr[l], &arr[r]);
			++l;
			--r;
		}
	} while (l <= r);
	if (left < r) QuickSort(arr, left, r);
	if (right > l) QuickSort(arr, l, right);
}