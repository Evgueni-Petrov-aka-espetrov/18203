#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

int main() {
	FILE *fileIn;
	FILE *fileOut;
	if (fopen_s(&fileIn, "in.txt", "rb")) {
		fprintf(stderr, "file error");
		exit(0);
	}
	if (fopen_s(&fileOut, "out.txt", "wb")) {
		fprintf(stderr, "file error");
		exit(0);
	}
	int workMode = fgetc(fileIn);
	if (fseek(fileIn, 2L, SEEK_CUR)) {
		fprintf(stderr, "file error");
		exit(0);
	}
	if (workMode == 'c') {
		Compress(fileIn, fileOut);
	}
	else
		if (workMode == 'd') {
			Decompress(fileIn, fileOut);
		}
		else {
			fprintf(stderr, "invalid mode");
		}
	fclose(fileIn);
	fclose(fileOut);
	return 0;
}
