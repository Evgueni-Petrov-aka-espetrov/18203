#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "convert.h"
#define CH_COUNT 14   // max 13 characters + \0
#define MAX_LEN 50 // FFFF FFFF FFFF to 2 n.s. have 48 digits + '.' + '\0'

int main() {
	// Input
	// 1st string (number's systems)
	int ns_in = 0, ns_out = 0;
	if (scanf("%d %d", &ns_in, &ns_out) != 2) {
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
	if (!Validation(ns_in, ns_out, snum)) {
		printf("bad input");
		exit(0);
	}
	// Converting
	double result = ToDec(snum, ns_in);
	
	char sresult[MAX_LEN];
	FromDec(sresult, MAX_LEN, result, ns_out);
	
	// Output
	printf("%s", sresult);
	
	return 0;
}
// COMPLETED 30/30