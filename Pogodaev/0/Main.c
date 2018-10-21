#include <stdio.h>
#include <stdlib.h>
#include "convert.h"

int main() {
	int numSysIn, numSysOut;
	char number[NUM_SIZE];
	// Input
	Input(&numSysIn, &numSysOut, number);
	// Errors check
	if (!Validation(numSysIn, numSysOut, number)) {
		printf("bad input");
		exit(0);
	}
	// Converting
	char result[RESULT_SIZE];
	Convert(number, result, RESULT_SIZE, numSysIn, numSysOut);
	// Output
	printf("%s", result);
	return 0;
}
