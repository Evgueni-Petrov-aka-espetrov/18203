#include <stdio.h>
#include <string.h>
#include <malloc.h>
void swap(int symbols[], int n, int m) {
	int midchar = symbols[n];
	symbols[n] = symbols[m];
	symbols[m] = midchar;
}
void sort(int array[], int last, int pos) {
	while (2 * pos + 1 <= last) {
		int var = 0;
		if (array[pos] >= array[2 * pos + 1]) var = var + 1;
		if ((array[pos] >= array[2 * pos + 2]) && (2 * pos + 2 <= last)) var = var + 2;

		if (var == 0) break;
		if (var == 1) {
			swap(array, pos, 2 * pos + 1);
			pos = 2 * pos + 1;
		}
		if (var == 2) {
			swap(array, pos, 2 * pos + 2);
			pos = 2 * pos + 2;
		}
		if (var == 3) {
			if (array[2 * pos + 1] >= array[2 * pos + 2]) {
				swap(array, pos, 2 * pos + 2);
				pos = 2 * pos + 2;
			}
			else {
				swap(array, pos, 2 * pos + 1);
				pos = 2 * pos + 1;
			}
		}
	}
}

void firstsort(int array[], int length) {
	int elemnum = (length / 2) - 1;
	int i = elemnum;
	while (i >= 0) {
		sort(array, length - 1, i);
		i--;
	}
}

void heapsort(int array[], int length) {
	int i;
	firstsort(array, length);
	for (i = length - 1; i >= 0; i--) {
		swap(array, 0, i);
		sort(array, i - 1, 0);
	}
}

int main() {
	FILE *fin = fopen("in.txt", "r");
	FILE *fout = fopen("out.txt", "w");
	if (fin == NULL)
		fprintf(fout, "File could not be opened.");
	else {
		int length;
		fscanf(fin, "%d", &length);
		int *array = (int*)malloc(length * sizeof(int));
		if (array == NULL) printf("out of memory");
		else {
			int i;
			for (i = 0; i<length; i++) {
				fscanf(fin, "%d ", &array[i]);
			}
			heapsort(array, length);
			for (i = length - 1; i >= 0; i--)
				fprintf(fout, "%d ", array[i]);
		}
		free(array);
	}
	fclose(fin);
	fclose(fout);
	return 0;
}
