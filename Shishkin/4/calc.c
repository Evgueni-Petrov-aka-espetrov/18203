#include "calc.h"
#include <string.h> 
#include <malloc.h>
#include "queue.h"
#include "pol.h"
#include "check.h"

int priority(int a) {
	int c = 0;
	if (a == -1) c = 3;
	if (a == -2) c = 3;
	if (a == -3) c = 2;
	if (a == -4) c = 2;
	if (a == -5) c = 1;
	if (a == -6) c = 1;
	return c;
}

int tokenize(char symbols[], int tokens[], int length) {
	int i;
	for (i = 0; i<length; i++) {
		tokens[i] = symbols[i] - '0';
		if (tokens[i] == -6) tokens[i] = -2;
		if (tokens[i] == -5) tokens[i] = -4;
		if (tokens[i] == -7) tokens[i] = -5;
		if (tokens[i] == -8) tokens[i] = -6;
	}
	for (i = 1; i<length; i++) {
		if ((tokens[i - 1] >= 0) && (tokens[i] >= 0)) {
			tokens[i - 1] = 10 * tokens[i - 1] + tokens[i];
			int j;
			for (j = i + 1; j<length; j++) tokens[j - 1] = tokens[j];
			length--;
			i--;
		}
	}
	return length;
}

int calc(char symbols[], int amount, int *error) {
	if (symbolcheck(symbols, amount) > 0) *error = 1; 
	else {
		int enter[MAX_LEN];
		int length = tokenize(symbols, enter, amount);
		if (positioncheck(enter, length) > 0) *error = 1; 
		else {
			queue *polhead = NULL;
			queue *poltail = NULL;
			createPol(enter, &polhead, &poltail, length);
			int dbz = 0;
			int result = readPol(&polhead, &poltail, &dbz);
			if (dbz == 1) {
				*error = 2; 
				return 0;
			}
			else return result;
		}
	}
}