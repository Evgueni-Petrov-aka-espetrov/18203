#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "calculator.h"

int main() {
	char expression[MAX_LEN];
	Input(expression);
	if (!Validation(expression)) {
		printf("syntax error");
		exit(0);
	}
	printf("%d", Calculate(expression));
	return 0;
}