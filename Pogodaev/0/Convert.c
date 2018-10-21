#define _CRT_SECURE_NO_WARNINGS
#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

long long int Power(long long int base, int p)
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

void Swap(char *a, char *b) {
	char t = *a;
	*a = *b;
	*b = t;
}

void ReverseString(char *string, int stringSize) {
	int i, j;
	for (i = stringSize - 1, j = 0; i > j; --i, ++j) {
		Swap(&string[i], &string[j]);
	}
}

int IntToDecNumSys(double *decResult, int numSysIn, const char *number) {
	int i = 0;
	for (; number[i] != '.' && number[i] != '\0'; ++i) {
		*decResult = numSysIn * (*decResult) + (isdigit(number[i]) ? (number[i] - '0') : (number[i] - '7')); //'A' - '7' = 10
	}
	return i; // return position of dot or position of '\0'
}

void FracToDecNumSys(double *decResult, int numSysIn, const char *number, int dotPos) {
	int i = ++dotPos;
	double fractal = 0;
	int fracCount = 0;
	for (; number[i] != '\0'; ++i) {
		fractal = numSysIn * (fractal) + (isdigit(number[i]) ? (number[i] - '0') : (number[i] - '7')); //'A' - '7' = 10
		++fracCount;
	}
	*decResult += fractal / Power(numSysIn, fracCount);
}

int IntFromDecNumSys(char *result, int numSysOut, long long intDecNumber) {
	int i = 0;
	for (; intDecNumber > 0; ++i) {
		result[i] = (char)(((intDecNumber % numSysOut) < 10) ? (intDecNumber % numSysOut + '0') : (intDecNumber % numSysOut + '7')); // 10 + '7' = 'A'
		intDecNumber /= numSysOut;
	}
	return i; // return position of dot or position of '\0'
}

void FracFromDecNymSys(char *result, int resultLength, int numSysOut, double fracDecNumber, int dotPos) {
	int i = ++dotPos;
	for (; i < resultLength - 1; ++i) {
		result[i] = (char)floor(fracDecNumber * numSysOut) + (((fracDecNumber * numSysOut) < 10) ? '0' : '7'); // 10 + '7' = 'A'
		fracDecNumber = (fracDecNumber * numSysOut) - floor(fracDecNumber * numSysOut);
		if (fracDecNumber == 0) {
			++i;
			break;
		}
	}
	result[i] = '\0';
}

double ToDec(const char *number, int numSysIn) {
	double decNumber = 0.0;
	// integer part to dec
	int i = IntToDecNumSys(&decNumber, numSysIn, number);
	//fractal part to dec
	if (number[i] == '.') {
		FracToDecNumSys(&decNumber, numSysIn, number, i);
	}
	return decNumber;
}

void FromDec(char result[], int resultLength, double decNumber, int numSysOut) {
	//integer part from dec
	long long intPart = (long long)floor(decNumber);
	int i = 0;
	if (intPart != 0) {
		i = IntFromDecNumSys(result, numSysOut, intPart);
		ReverseString(result, i);
	}
	else {
		result[i] = '0';
		++i;
	}
	// return if number is integer
	double fractalPart = decNumber - floor(decNumber);
	if (fractalPart > 0) {
		result[i] = '.';
	}
	else {
		result[i] = '\0';
		return;
	}
	// fractal part from dec
	FracFromDecNymSys(result, resultLength, numSysOut, fractalPart, i);
}

void Convert(const char *number, char *result, int maxResultSize, int numSysIn, int numSysOut) {
	FromDec(result, maxResultSize, ToDec(number, numSysIn), numSysOut);
}

int Validation(int numSysIn, int numSysOut, char *number) {
	// 1st string check
	if (numSysIn > 16 || numSysIn < 2 || numSysOut > 16 || numSysOut < 2) {
		return 0;
	}

	// 2nd string check
	int dotsCount = 0;
	for (int i = 0; number[i] != '\0'; ++i) {
		if (number[i] == '.') {
			++dotsCount;
			if (i == 0 || number[i + 1] == '\0' || dotsCount > 1) {
				return 0;
			}
		}
		number[i] = toupper(number[i]);
		if (!(number[i] >= '0' && number[i] < ('0' + numSysIn) || number[i] >= 'A' && number[i] < ('7' + numSysIn) || number[i] == '.')) {
			return 0;
		}
	}
	return 1;
}

void Input(int *numSysIn, int *numSysOut, char *number) {
	// 1st string (number systems)
	if (scanf("%d %d", numSysIn, numSysOut) != 2) {
		printf("bad input");
		exit(0);
	}
	// 2nd string (number)
	if (scanf("%s", number) != 1) {
		printf("bad input");
		exit(0);
	}
}
