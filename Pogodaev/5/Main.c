#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

int main() {
	FILE *fileIn = fopen("in.txt", "rb");
	FILE *fileOut = fopen("out.txt", "wb");
	if (ferror(fileIn) || ferror(fileOut)) {
		printf("file error");
		exit(0);
	}
	int workMode = fgetc(fileIn);
	if (fseek(fileIn, 2L, SEEK_CUR)) {
		fprintf(stderr, "file error\n");
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
			fprintf(stderr, "invalid mode\n");
		}
	fclose(fileIn);
	fclose(fileOut);
	return 0;
}
