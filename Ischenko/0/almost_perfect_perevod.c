#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

typedef enum { true, false } bool;

bool is_input_bad(int base_of_input, int base_of_output, char number_in_input_base[]) {
	bool r = false;
	if (base_of_input > 16 || base_of_input < 2 || base_of_output>16 || base_of_output < 2) {
		r = true;

	}
	for (int i = 0; i < strlen(number_in_input_base); ++i) {
		switch (number_in_input_base[i]) {

		case '.': if ((i + 1) == strlen(number_in_input_base) || i == 0 || number_in_input_base[i + 1] == '.') { r = true; } break;
		case '2': if (base_of_input <= 2) { r = true; }break;
		case '3': if (base_of_input <= 3) { r = true; }break;
		case '4': if (base_of_input <= 4) { r = true; }break;
		case '5': if (base_of_input <= 5) { r = true; }break;
		case '6': if (base_of_input <= 6) { r = true; }break;
		case '7': if (base_of_input <= 7) { r = true; }break;
		case '8': if (base_of_input <= 8) { r = true; }break;
		case '9': if (base_of_input <= 9) { r = true; }break;
		case 'a':if (base_of_input <= 10) { r = true; }break;
		case 'b':if (base_of_input <= 11) { r = true; }break;
		case 'c':if (base_of_input <= 12) { r = true; }break;
		case 'd':if (base_of_input <= 13) { r = true; }break;
		case 'e':if (base_of_input <= 14) { r = true; }break;
		case 'f':if (base_of_input <= 15) { r = true; }break;
		}
	}
	return r;
}

char transfer_numeral_to_letter(int numeral) {
	char letter = ' ';
	switch (numeral) {
	case 0:case 1:case 2:case 3:case 4:
	case 5:case 6:case 7:case 8:case 9:
		letter = numeral + 48; break;
	case 10:letter = 'a'; break;
	case 11:letter = 'b'; break;
	case 12:letter = 'c'; break;
	case 13:letter = 'd'; break;
	case 14:letter = 'e'; break;
	case 15:letter = 'f'; break;
	}
	return letter;
}

void make_number_in_base_10(char number_in_input_base[], int base_of_input, long double *number_in_base_10) {
	int position_of_point = strcspn(number_in_input_base, "."), lenth = strlen(number_in_input_base);
	int i = 0;
	for (i = 0; i < position_of_point; ++i) {
		char letter = number_in_input_base[i];
		switch (letter) {
		case '0':case '1':case '2':case '3':case '4':
		case '5':case '6':case '7':case '8':case '9':
			*number_in_base_10 += ((int)letter - 48)*pow(base_of_input, position_of_point - i - 1);
			break;
		case 'a':*number_in_base_10 += 10 * pow(base_of_input, position_of_point - i - 1); break;
		case 'b':*number_in_base_10 += 11 * pow(base_of_input, position_of_point - i - 1); break;
		case 'c':*number_in_base_10 += 12 * pow(base_of_input, position_of_point - i - 1); break;
		case 'd':*number_in_base_10 += 13 * pow(base_of_input, position_of_point - i - 1); break;
		case 'e':*number_in_base_10 += 14 * pow(base_of_input, position_of_point - i - 1); break;
		case 'f':*number_in_base_10 += 15 * pow(base_of_input, position_of_point - i - 1); break;
		case 'A':*number_in_base_10 += 10 * pow(base_of_input, position_of_point - i - 1); break;
		case 'B':*number_in_base_10 += 11 * pow(base_of_input, position_of_point - i - 1); break;
		case 'C':*number_in_base_10 += 12 * pow(base_of_input, position_of_point - i - 1); break;
		case 'D':*number_in_base_10 += 13 * pow(base_of_input, position_of_point - i - 1); break;
		case 'E':*number_in_base_10 += 14 * pow(base_of_input, position_of_point - i - 1); break;
		case 'F':*number_in_base_10 += 15 * pow(base_of_input, position_of_point - i - 1); break;
		}
	}
	i = 0;
	for (i = position_of_point + 1; i < lenth; ++i) {
		char letter = number_in_input_base[i];
		switch (letter) {
		case '0':case '1':case '2':case '3':case '4':
		case '5':case '6':case '7':case '8':case '9':
			*number_in_base_10 += ((int)letter - 48)*pow(base_of_input, position_of_point - i);
			break;
		case 'a':*number_in_base_10 += 10 * pow(base_of_input, position_of_point - i); break;
		case 'b':*number_in_base_10 += 11 * pow(base_of_input, position_of_point - i); break;
		case 'c':*number_in_base_10 += 12 * pow(base_of_input, position_of_point - i); break;
		case 'd':*number_in_base_10 += 13 * pow(base_of_input, position_of_point - i); break;
		case 'e':*number_in_base_10 += 14 * pow(base_of_input, position_of_point - i); break;
		case 'f':*number_in_base_10 += 15 * pow(base_of_input, position_of_point - i); break;
		case 'A':*number_in_base_10 += 10 * pow(base_of_input, position_of_point - i); break;
		case 'B':*number_in_base_10 += 11 * pow(base_of_input, position_of_point - i); break;
		case 'C':*number_in_base_10 += 12 * pow(base_of_input, position_of_point - i); break;
		case 'D':*number_in_base_10 += 13 * pow(base_of_input, position_of_point - i); break;
		case 'E':*number_in_base_10 += 14 * pow(base_of_input, position_of_point - i); break;
		case 'F':*number_in_base_10 += 15 * pow(base_of_input, position_of_point - i); break;
		}
	}
}

void make_number_in_output_base(int base_of_output, long double number_in_base_10, char *number_in_output_base) {

	long long int integer_part_of_namber = floor(number_in_base_10), i = 0;
	double fractional_part_of_number = number_in_base_10 - integer_part_of_namber;
	while (integer_part_of_namber >= base_of_output) {
		number_in_output_base[i] = transfer_numeral_to_letter(integer_part_of_namber%base_of_output);
		integer_part_of_namber /= base_of_output;
		i++;
	}
	number_in_output_base[i] = transfer_numeral_to_letter(integer_part_of_namber%base_of_output);
	int position_of_point = i + 1;
	if (fractional_part_of_number != 0) {
		i++;
		number_in_output_base[i] = '.';
		i++;
		for (int j = 0; j < 20; ++j) {
			number_in_output_base[i] = transfer_numeral_to_letter((int)(fractional_part_of_number*base_of_output));
			fractional_part_of_number = (fractional_part_of_number*base_of_output) - (int)(fractional_part_of_number*base_of_output);
			if (fractional_part_of_number == 0)
				break;
			i++;
		}
	}
	char buffchar;
	int j;
	for (j = 0; j < position_of_point / 2; ++j) {
		buffchar = number_in_output_base[j];
		number_in_output_base[j] = number_in_output_base[position_of_point - j - 1];
		number_in_output_base[position_of_point - j - 1] = buffchar;
	}
	if (number_in_output_base[i] == 'M') {
		number_in_output_base[i] = '\0';
	}
	else {
		number_in_output_base[i + 1] = '\0';
	}
}

int main() {
	int base_of_input = 0, base_of_output = 0;
	long double number_in_base_10 = 0;
	char number_in_input_base[50], number_in_output_base[100];
	scanf( "%d%d", &base_of_input, &base_of_output);
	scanf( "%s", number_in_input_base);
	if (is_input_bad(base_of_input, base_of_output, number_in_input_base)==true) {
		printf( "bad input");
		return 0;
	}
	make_number_in_base_10(number_in_input_base, base_of_input, &number_in_base_10);
	if (number_in_base_10 == 0) {
		printf( "0");	
		return 0;
	}
	make_number_in_output_base(base_of_output, number_in_base_10, number_in_output_base);
	printf("%s", number_in_output_base);
	return 0;
}