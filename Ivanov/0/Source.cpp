#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void* malloc_s(size_t size);
bool checkInput(int a, int b, char* numberA);
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
	
	if ((long long)dec == 0) {
		printf("0.");
	}
	else {
		printf("%s.", fromDecIntToB((long long)dec, b));
	}
	printf("%s\n", fromDecFracToB(dec, b));
	return 0;
}

int atoi(char a) {
	int result = 17;
	if (a >= '0' && a <= '9') // ? charcode('0') < charcode('9')
		result = a - '0';
	else if (a >= 'a' && a <= 'f')
		result = a - 'a' + 10;
	else if (a >= 'A' && a <= 'F')
		result = a - 'A' + 10;

	return result;
}

char* itoa(int i) {
	char* result = (char*)malloc_s(sizeof(char) * 2);
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
	for (; (*dot != '.')&&(*dot != '\0'); ++dot);
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
		result += (double)fromChar*1.0/(double)factor;
		factor *= a;
	}

	return result;
}

char* fromDecIntToB(long long dec, int b) {

	if (dec == 0) {
		char* space = (char*)malloc_s(sizeof(char) * 100);
		space[0] = '\0';
		return space;
	}

	return strcat(fromDecIntToB(dec / b, b), itoa(dec%b));
}

char* fromDecFracToB(double dec, int b) {

	dec = dec - (int)dec;
	char* result = (char*)malloc_s(sizeof(char) * 13);
	result[0] = '\0';
	for (int i = 0; i < 12; ++i) {
		dec *= b;
		int integerPart = (int)dec;
		strcat(result, itoa(integerPart));
		if (integerPart != 0) dec -= integerPart;
	}

	return result;
}

bool checkInput(int a, int b, char* numberA) {
	if (a<2 || a>16 || b<2 || b>16)
		return false;

	int numberOfDots = 0;
	for (int i = 0; i < 14; ++i) {
		if (numberA[i] == '.') {
			if (numberOfDots != 0 || i == 0 || numberA[i + 1] == '\0')
				return false;
			++numberOfDots;
		}
		else if (numberA[i] == '\0')
			return true;
		else if (atoi(numberA[i]) >= a)
			return false;
	}

	return false;
}

void* malloc_s(size_t size) {
	void* allocated = malloc(size);
	if (!allocated) {
		printf("ERROR: BAD_ALLOCATION");
		exit(1);
	}
	return allocated;
}