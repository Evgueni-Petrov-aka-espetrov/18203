#include <math.h>
#include <stdlib.h>
#include <ctype.h>

double ToNumSys(int ns_in, char ch, double result_num) {
	return (ns_in * result_num + (isdigit(ch) ? (ch - '0') : (ch - '7'))); //'A' - '7' = 10
}

long long int power(long long int base, int p)
{
	long long int result = 1;
	while (p)
	{
		if (p & 1) result *= base;
		base *= base;
		p = p >> 1;
	}
	return result;
}

double ToDec(char s[], int ns_in) {
	double num = 0.0;
	int i = 0;

	// integer part to dec
	for (; s[i] != '.' && s[i] != '\0'; ++i)
		num = ToNumSys(ns_in, s[i], num);
	
	//fractal part to dec
	if (s[i] == '.') {
		double fractal = 0;
		int frac_count = 0;
		for (++i; s[i] != '\0'; ++i) {
			fractal = ToNumSys(ns_in, s[i], fractal);
			++frac_count;
		}		
		num += fractal / power(ns_in, frac_count);
	}
	return num;
}
// completed

void FromDec(char result[], int len, double dec, int ns_out) {
	//integer part from dec
	long long int int_part = (long long int)floor(dec);
	int i = 0;
	if (int_part != 0) {
		for (; int_part > 0; ++i) {
			result[i] = (char)(((int_part % ns_out) < 10) ? (int_part % ns_out + '0') : (int_part % ns_out + '7')); // 10 + '7' = 'A'
			int_part /= ns_out;
		}
		int j, k;
		for (j = i - 1, k = 0; j > k; --j, ++k) {
			char t = result[j];
			result[j] = result[k];
			result[k] = t;
		}
	}
	else {
		result[i] = '0';
		++i;
	}

	// return if number is integer
	if ((dec - floor(dec)) > 0)
		result[i++] = '.';
	else {
		result[i] = '\0';
		return;
	}

	// fractal part from dec
	double fractal = dec - floor(dec);
	for (; i < len - 1; ++i) {
		result[i] = (char)floor(fractal * ns_out) + (((fractal * ns_out) < 10) ? '0' : '7'); // 10 + '7' = 'A'
		fractal = (fractal * ns_out) - floor(fractal * ns_out);
		if (fractal == 0) {
			++i;
			break;
		}
	}
	result[i] = '\0';
	return;
}
// completed

bool Validation(int ns_in, int ns_out, char *str) {
	// 1st string check
	if (ns_in > 16 || ns_in < 2 || ns_out > 16 || ns_out < 2) {
		return false;
	}

	// 2nd string check
	int dots_count = 0;
	for (int i = 0; str[i] != '\0'; ++i) {
		if (str[i] == '.') {
			dots_count++;
			if (i == 0 || str[i + 1] == '\0' || dots_count > 1) {
				return false;
			}
		}
		str[i] = toupper(str[i]);
		if (!(str[i] >= '0' && str[i] < ('0' + ns_in) || str[i] >= 'A' && str[i] < ('7' + ns_in) || str[i] == '.')) {
			return false;
		}
	}
	return true;
}