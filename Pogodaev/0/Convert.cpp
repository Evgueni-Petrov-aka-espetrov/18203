#include <math.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LEN 50 // FFFF FFFF FFFF to 2 n.s. have 48 digits + '.' + '\0'

double ToDec(char s[], int ss_in) {
	double num = 0;
	int i;

	// integer part to dec
	for (i = 0; s[i] != '.' && s[i] != '\0'; i++)
		num = ss_in * num + (isdigit(s[i]) ? (s[i] - '0') : (s[i] - '7'));

	//fractal part to dec
	if (s[i] != '\0') {
		__int64 fractal = 0;
		int frac_count = 0;
		for (i++; s[i] != '\0' && s[i] != '.'; i++) {
			fractal = ss_in * fractal + (isdigit(s[i]) ? (s[i] - '0') : (s[i] - '7'));
			frac_count++;
		}
		num += (double)fractal / pow(ss_in, frac_count);
	}
	return num;
}
// completed

char *FromDec(double dec, int ss_out) {

	//allocating memory
	char *result;
	if ((result = (char *)malloc(MAX_LEN * sizeof(char))) == NULL) return NULL;

	//integer part from dec
	__int64 int_part = (__int64)floor(dec);
	int i = 0;
	if (int_part != 0) {
		for (; int_part > 0; i++) {
			result[i] = (char)(((int_part % ss_out) < 10) ? (int_part % ss_out + '0') : (int_part % ss_out + '7'));
			int_part /= ss_out;
		}
		int j, k;
		for (j = i - 1, k = 0; j > k; j--, k++) {
			char t = result[j];
			result[j] = result[k];
			result[k] = t;
		}
	}
	else {
		result[0] = '0';
		i++;
	}

	// return if number is integer
	if ((dec - floor(dec)) > 0)
		result[i++] = '.';
	else {
		result[i] = '\0';
		return result;
	}

	// fractal part from dec
	double fractal = dec - floor(dec);
	for (; i < MAX_LEN - 1; i++) {
		result[i] = (char)floor(fractal * ss_out) + (((fractal * ss_out) < 10) ? '0' : '7');
		fractal = (fractal * ss_out) - floor(fractal * ss_out);
		if (fractal == 0) {
			i++;
			break;
		}
	}
	result[i] = '\0';
	return result;
}
// completed

bool Validation(int ss_in, int ss_out, char *str) {
	// 1st string check
	if (ss_in > 16 || ss_in < 2 || ss_out > 16 || ss_out < 2) {
		return false;
	}

	// 2nd string check
	int dots_count = 0;
	for (int i = 0; str[i] != '\0'; i++) {
		if (str[i] == '.') {
			dots_count++;
			if (i == 0 || str[i + 1] == '\0' || dots_count > 1) {
				return false;
			}
		}
		str[i] = toupper(str[i]);
		if (!(str[i] >= '0' && str[i] < ('0' + ss_in) || str[i] >= 'A' && str[i] < ('7' + ss_in) || str[i] == '.')) {
			return false;
		}
	}
	return true;
}