#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


int numbers_for_sort[2000000];

void quicksort( int start, int end, int lenth) {
	double number_for_spliting;
	int first_point = start, second_point = end;
	if (lenth >= 3) {
		number_for_spliting = (double)(numbers_for_sort[start] + numbers_for_sort[end] + numbers_for_sort[(start + end) / 2]) / 3;
	}
	else {
		number_for_spliting = (double)(numbers_for_sort[start] + numbers_for_sort[end]) / 2;
	}

	if (start < end) {
		while (1) {
			while (numbers_for_sort[second_point] >= number_for_spliting) {
				second_point--;
			}
			while (numbers_for_sort[first_point] < number_for_spliting) {
				first_point++;
			}

			if (second_point <= first_point)
				break;
			if (numbers_for_sort[first_point] > numbers_for_sort[second_point]) {
				int d = numbers_for_sort[first_point];
				numbers_for_sort[first_point] = numbers_for_sort[second_point];
				numbers_for_sort[second_point] = d;
				second_point--;
				first_point++;
			}

		}
		if (first_point - 1 == end) {
			first_point--;
		}
		quicksort(start, first_point - 1, first_point - start);
		if (first_point == start) {
			first_point++;
		}
		quicksort(first_point, end, end - first_point);
	}

}

int main() {

	int count_of_numbers_for_sort = 0;
	scanf("%d", &count_of_numbers_for_sort);
	
	if (count_of_numbers_for_sort == 0) {
		printf(" ");
		return 0;
	}
	if (count_of_numbers_for_sort == 1) {
		scanf("%d", &count_of_numbers_for_sort);
		printf("%d", count_of_numbers_for_sort);
		return 0;
	}
	for (int i = 0; i < count_of_numbers_for_sort; ++i) {
		int buff = 0;
		scanf("%d", &buff);
		numbers_for_sort[i]=buff;
	}
	int start_of_sort=0;
	quicksort(start_of_sort, count_of_numbers_for_sort - 1, count_of_numbers_for_sort);
	for (int i = 0; i < count_of_numbers_for_sort; ++i) {
		printf("%d ", numbers_for_sort[i]);
	}
	return 0;
}