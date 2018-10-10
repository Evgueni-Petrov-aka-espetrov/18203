#include <stdio.h>
#include "rabinkarp.h"

int main() {
	// Input
	// 1 string (pattern)
	unsigned char pattern[MAX_PAT_LEN];
	GetString(pattern);
	// Searching a pattern in text and logging it
	RKSearch(pattern);
	return 0;
}
