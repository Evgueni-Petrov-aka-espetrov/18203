#define MAX_LEN 1001
#include <stdio.h>
#include "calc.h"

int main() {
	char buf1[MAX_LEN] = { 0 }, buf2[2 * MAX_LEN - 1] = { 0 };
	scanf("%[^\n]s", buf1);
	if (!checkInput(buf1))
		printf("syntax error");
	else {
		convert(buf1, buf2);
		printf("%d\n", calculate(buf2, buf1));
	}
	return 0;
}