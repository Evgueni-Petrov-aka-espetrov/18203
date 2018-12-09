#include <stdio.h>
#include <stdlib.h>
#include "calculator.h"

int main() {
	char expression[MAX_LEN];
	if (!Input(expression)) {
		printf("syntax error");
		exit(0);
	}
	printf("%d", Calculate(expression));
	return 0;
}
