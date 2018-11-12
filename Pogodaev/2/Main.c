#include <string.h>
#include "permutation.h"

int main() {
	char string[ALPHABET_SIZE];
	int permNumber;
	Input(string, &permNumber);
	int stringSize = strlen(string);
	Validation(string, stringSize);
	Output(string, stringSize, permNumber);
	return 0;
}
