#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void* mallocSafe(size_t size);
int checkInput(int a, int b, char* numberA);
int atoi(char a);
char* itoa(int i);
double fromAToDec(char const* numberA, int a);
char* fromDecIntToB(long long dec, int b);
char* fromDecFracToB(double dec, int b);

int main() {
	int a = 0;
	int b = 0;
	char numberA[14];
	scanf("%i %i %s", &a, &b, numberA);

	if (!checkInput(a, b, numberA)) {
		printf("bad input");
		return 0;
	}

	double dec = fromAToDec(numberA, a);

	char* decPart;
	char* fracPart = fromDecFracToB(dec, b);
	if ((long long)dec == 0) {
		printf("0.");
	}
	else {
		decPart = fromDecIntToB((long long)dec, b);
		printf("%s.", decPart);
		free(decPart);
	}
	printf("%s\n", fracPart);
	free(fracPart);
	return 0;
}

int atoi(char a) {
	int result = 17;
	if (a >= '0' && a <= '9')
		result = a - '0';
	else if (a >= 'a' && a <= 'f')
		result = a - 'a' + 10;
	else if (a >= 'A' && a <= 'F')
		result = a - 'A' + 10;

	return result;
}

char* itoa(int i) {
	char* result = (char*)mallocSafe(sizeof(char) * 2);
	result[1] = '\0';
	if (i >= 0 && i <= 9)
		result[0] = (char)(i + '0');
	else if (i >= 10 && i <= 16)
		result[0] = (char)(i + 'a' - 10);
	else result[0] = '\0';

	return result;
}

double fromAToDec(char const* numberA, int a) {
	double result = 0;
	long long factor = 1;
	char const* dot = numberA;
	for (; (*dot != '.') && (*dot != '\0'); ++dot);
	for (char const* ptr = dot - 1; ptr >= numberA; --ptr) {
		int fromChar = atoi(*ptr);
		result += fromChar*factor;
		factor *= a;
	}
	factor = a;
	if (*dot == '\0') {
		return result;
	}
	for (char const* ptr = dot + 1; *ptr != '\0'; ++ptr) {
		int fromChar = atoi(*ptr);
		if (fromChar >= a) return -1;
		result += (double)fromChar / factor;
		factor *= a;
	}

	return result;
}

char* fromDecIntToB(long long dec, int b) {

	if (dec == 0) {
		char* space = (char*)mallocSafe(sizeof(char) * 100);
		space[0] = '\0';
		return space;
	}

	char* sym = itoa(dec%b);
	char* result = strcat(fromDecIntToB(dec / b, b), sym);
	free(sym);
	return result;
}

char* fromDecFracToB(double dec, int b) {

	dec = dec - (int)dec;
	char* result = (char*)mallocSafe(sizeof(char) * 13);
	result[0] = '\0';
	for (int i = 0; i < 12; ++i) {
		dec *= b;
		int integerPart = (int)dec;
		strcat(result, itoa(integerPart));
		if (integerPart != 0) dec -= integerPart;
	}

	return result;
}

int checkInput(int a, int b, char* numberA) {
	if (a<2 || a>16 || b<2 || b>16)
		return 0;

	int numberOfDots = 0;
	for (int i = 0; i < 14; ++i) {
		if (numberA[i] == '.') {
			if (numberOfDots != 0 || i == 0 || numberA[i + 1] == '\0')
				return 0;
			++numberOfDots;
		}
		else if (numberA[i] == '\0')
			return 1;
		else if (atoi(numberA[i]) >= a)
			return 0;
	}

	return 0;
}

void* mallocSafe(size_t size) {
	void* allocated = malloc(size);
	if (!allocated) {
		printf("ERROR: BAD_ALLOCATION");
		exit(1);
	}
	return allocated;
}