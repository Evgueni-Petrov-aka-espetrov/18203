#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "convert.h"
#define CH_COUNT 14   // max 13 characters + \0

int main() {
	// Input
	// 1st string (number's systems)
	int ss_in = 0, ss_out = 0;
	if (scanf("%d %d", &ss_in, &ss_out) != 2) {
		printf("bad input");
		exit(0);
	}
	// 2nd string (number)
	char snum[CH_COUNT];
	if (scanf("%s", snum) != 1) {
		printf("bad input");
		exit(0);
	}
	// Input errors check
	if (!Validation(ss_in, ss_out, snum)) {
		printf("bad input");
		exit(0);
	}
	// Converting
	double result = ToDec(snum, ss_in);
	char *sresult = FromDec(result, ss_out);
	// Output
	if (sresult != NULL) printf("%s", sresult);
	else {
		printf("out of memory");
		exit(0);
	}
	// Free used memory
	free(sresult);
	return 0;
}
// COMPLETED 30/30