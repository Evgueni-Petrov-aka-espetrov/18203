#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>

int get_amount( FILE* fin) {
	int amount=0;
	fscanf(fin, "%d", &amount);
	return amount;
}

void get_array_for_sort(const FILE* fin, const int amount, int * array_for_sort) {
	int number = 0;
	for (int i = 0; i < amount; i++) {
		fscanf(fin, "%d", &number);
		array_for_sort[i] = number;
	}
}

void rebalance(int* array_for_sort, int distance, int root_pos) {
	if (root_pos * 2 + 1 < distance) {
		if (root_pos * 2 + 2 < distance) {
			if (array_for_sort[root_pos * 2 + 1] < array_for_sort[root_pos * 2 + 2]) {
				if (array_for_sort[root_pos] < array_for_sort[root_pos * 2 + 2]) {
					int buff = array_for_sort[root_pos];
					array_for_sort[root_pos]= array_for_sort[root_pos * 2 + 2];
					array_for_sort[root_pos * 2 + 2] =buff;
					rebalance(array_for_sort, distance, root_pos * 2 + 2);
				} 
			}
			else {
				if (array_for_sort[root_pos] < array_for_sort[root_pos * 2 + 1]) {
					int buff = array_for_sort[root_pos];
					array_for_sort[root_pos] = array_for_sort[root_pos * 2 + 1];
					array_for_sort[root_pos * 2 + 1] =buff;
					rebalance(array_for_sort, distance, root_pos * 2 + 1);
				}
			}
		}
		else {
			if (array_for_sort[root_pos] < array_for_sort[root_pos * 2 + 1]) {
				int buff = array_for_sort[root_pos];
				array_for_sort[root_pos] = array_for_sort[root_pos * 2 + 1];
				array_for_sort[root_pos * 2 + 1] = buff;
			}
		}
	}
}

void create_sorting_tree(int* array_for_sort, int distance, int root_pos) {
	if (root_pos * 2 + 1 < distance) {
		create_sorting_tree(array_for_sort, distance, root_pos * 2 + 1);
	}
	if (root_pos * 2 + 2 < distance) {
		create_sorting_tree(array_for_sort, distance, root_pos * 2 + 2);
	}
	rebalance(array_for_sort, distance, root_pos);
}

void heap_sort(int* array_for_sort, int distance) {
	for (int i = distance; i > 1; --i) {
		rebalance(array_for_sort, i, 0);
		int buff = array_for_sort[0];
		array_for_sort[0] = array_for_sort[i - 1];
		array_for_sort[i - 1] = buff;
	}
}

void print_sorted_array(FILE* fout, int* array_for_sort,int amount) {
	for (int i = 0; i < amount; i++) {
		fprintf(fout, "%d ", array_for_sort[i]);
	}
}

int main() {
	FILE *fin, *fout;
	fin = fopen("in.txt", "r");
	fout = fopen("out.txt", "w");

	int amount = get_amount(fin);
	int *array_for_sort = (int*)malloc(amount * sizeof(int));
	get_array_for_sort(fin, amount, array_for_sort);
	fclose(fin);
	create_sorting_tree(array_for_sort, amount, 0);

	heap_sort(array_for_sort, amount);

	print_sorted_array(fout, array_for_sort, amount);
	free(array_for_sort);
	fclose(fout);

	return 0;
}