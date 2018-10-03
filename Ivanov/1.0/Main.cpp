#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MAX_LENGTH = 17;

bool fillBuffer(char* buffer, int shift, int length){
	strcpy(buffer, buffer + shift);
	for (int i = length - shift; i < length; ++i){
		int charcode;
		if ((charcode = getchar()) == EOF){
			buffer[i] = '\0';
			return false;
		}
		else{
			buffer[i] = (char)charcode;
		}
	}
	buffer[length] = '\0';
	return true;
}

void buildShiftTable(char* pattern, int* table){

}

void search(char* pattern){
	int length = 0;
	for (; pattern[length] != '\0'; ++length);
	++length;
	char* buffer = (char*)malloc(sizeof(char)*length);
	fillBuffer(buffer, length - 1, length);
	int currentPosition = 1;
	for (;;){
		for (int i = length - 1; i >= 0; --i){
			if (pattern[i])//
		}
	}
}

int main(){
	return 0;
}