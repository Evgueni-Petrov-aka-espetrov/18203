#define  _CRT_SECURE_NO_WARNINGS


#include<stdio.h>
#include<stdlib.h>


typedef enum {false, true} boolean;
boolean used_numbers[10] = { false };

boolean is_input_simbol_good(char simbol) {
	boolean is_good = true;
	if (isdigit(simbol)) {
		int number = (int)simbol - 48;
		if (used_numbers[number]==false) {
			used_numbers[number] = true;
		}
		else {
			is_good = false;
		}
	}
	else {
		is_good = false;
	}
	return is_good;
}

int get_transposition(int *current_transposition, boolean *is_input_good) {
	int lenth_of_transposition = 0;
	char simbol = ' ';
	boolean is_simbol_good = true;
	scanf("%c", &simbol);
	while (simbol != '\n') {
		is_simbol_good = is_input_simbol_good(simbol);
		if (is_simbol_good) {
			current_transposition[lenth_of_transposition] = (int)simbol - 48;
		}
		else {
			*is_input_good = false;
		}
		lenth_of_transposition++;
		scanf("%c", &simbol);
	}
	return lenth_of_transposition;
}

int find_positin_of_order_breaking(const int *current_transposition, const int lenth_of_transposition) {
	int position_of_order_breaking = -1;
	for (int i = lenth_of_transposition - 1; i > 0; i--) {
		if (current_transposition[i] > current_transposition[i - 1]) {
			position_of_order_breaking = i - 1;
			break;
		}
	}
	return position_of_order_breaking;
}

int find_position_of_next_right_order_element(const int *current_transposition, const int lenth_of_transposition, const int position_of_order_breaking) {
	int position_of_right_element = lenth_of_transposition-1, right_element=10;
	for (int i = lenth_of_transposition - 1; i > position_of_order_breaking; i--) {
		if (current_transposition[i]<right_element && current_transposition[i]>current_transposition[position_of_order_breaking]) {
			position_of_right_element = i;
			right_element = current_transposition[i];
		}
	}
	return position_of_right_element;
}

void reverse_end_of_transposition(int *current_transposition,const int position_of_order_breaking,const int lenth_of_transposition) {
	int buff = 0;
	for (int i = 1; i <= (lenth_of_transposition-position_of_order_breaking ) / 2; i++) {
		buff = current_transposition[position_of_order_breaking + i];
		current_transposition[position_of_order_breaking + i] = current_transposition[lenth_of_transposition - i];
		current_transposition[lenth_of_transposition - i] = buff;
	}
}

boolean create_next_transposition(int *current_transposition, int lenth_of_transposition) {
	int position_of_order_breaking=find_positin_of_order_breaking(current_transposition, lenth_of_transposition);
	if (position_of_order_breaking == -1) {
		return false;
	}
	int position_of_next_right_order_element = find_position_of_next_right_order_element(current_transposition, lenth_of_transposition, position_of_order_breaking);
	int buff = current_transposition[position_of_order_breaking];
	current_transposition[position_of_order_breaking] = current_transposition[position_of_next_right_order_element];
	current_transposition[position_of_next_right_order_element] = buff;
	reverse_end_of_transposition(current_transposition, position_of_order_breaking, lenth_of_transposition);
	return true;
}

void print_transposition(const int * current_transposition, const int lenth_of_transposition) {
	for (int i = 0; i < lenth_of_transposition; i++) {
		int number = current_transposition[i];
		printf("%d",number);
	}
	printf("\n");
}

int main() {
	boolean is_input_good = true, is_there_new_transposition=true;
	int current_transposition[10], lenth_of_transposition=0, ammount_of_transpositions=0;
	lenth_of_transposition = get_transposition(current_transposition, &is_input_good);
	scanf("%d", &ammount_of_transpositions);
	if (!is_input_good) {
		printf("bad input");
		return 0;
	}
	for (int i = 0; i < ammount_of_transpositions; i++) {
		is_there_new_transposition=create_next_transposition(current_transposition, lenth_of_transposition);
		if (is_there_new_transposition) {
			print_transposition(current_transposition, lenth_of_transposition);
		}
		else {
			break;
		}
	}
}