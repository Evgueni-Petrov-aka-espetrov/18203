#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STRING 10


int check1(char string[], int length);
int check2(char string[], int length);
void swap_symbol(char string[], int i, int j);
int new_string(char string[], int length);
void print(char string[], int length, int N);

int main() {
	char string[MAX_STRING + 1];
	int N;
	gets(string);
	scanf("%d", &N);
	if (check1(string, strlen(string)) && check2(string, strlen(string)))
		print(string, strlen(string), N);
	return 0;
}

int check1(char string[], int length){
    int i;
    char test[] = "1234567890";

    /* strchr(a,b) ищет первое вхождение символа b в строку a и возвращает указатель на искомый символ, если он найден в строке str, иначе NULL. */

    for (i = 0; i < length; i++){
        if (strchr(test, string[i]) == 0){
            printf("bad input");
            return 0;
        }
    }
    return 1;
}

int check2(char string[], int length){
    int i, j;
    for (i = 0; i < length; i++) {
        for (j = i + 1; j < length; j++) {
            if (string[i] == string[j]){
                printf("bad input");
                return 0;
            }
        }
    }
    return 1;
}

void swap_symbol(char string[], int i, int j) {
	char a;
	a = string[j];
	string[j] = string[i];
	string[i] = a;
}

int new_string(char string[], int length) {
	int i = length - 2;
	while (i >= 0 && string[i] > string[i+1])
        i--;
    if (i == -1)
        return 0;
	int j = length;
	while (j>=0 && string[j] < string[i])
        j--;
	swap_symbol(string, i, j);
        i--;
	j = length-1;
	while (i < j)
	{
		swap_symbol(string, i, j);
            i++;
            j--;
	}
	return 1;
}

void print(char string[], int length, int N) {
	int i = 0;
	while (i<N && new_string(string, length) == 1) {
		printf("%s\n", string);
            i++;
	}
}
