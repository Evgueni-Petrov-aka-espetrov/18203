#include <stdio.h>
#include "boyermoore.h"
#define MAX_PAT_LEN 17 // 16 characters + '\0'

void GetString(unsigned char*);

int main() {
	// Input
	// 1 string (pattern)
	unsigned char pattern[MAX_PAT_LEN];
	GetString(pattern);
	// Searching a pattern in text and logging it
	BMSearch(pattern);
	return 0;
}

void GetString(unsigned char *str) {
	int c;
	int i = 0;
	while ((c = getc(stdin)) != '\n') {
		str[i] = c;
		++i;
	}
	str[i] = '\0';
}
