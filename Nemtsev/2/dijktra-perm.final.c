#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STRING_SIZE 10

int check(char string[], int length);
void swap(char string[], int i, int j);
int permute(char string[], int length);

int main() {
	char string[STRING_SIZE + 1];
	int n, i = 0;
	gets(string);
	scanf("%d", &n);
    check(string, strlen(string));
    while (i<n && permute(string, strlen(string)) == 1) {
		printf("%s\n", string);
		i++;
    }
	return 0;
}

int check(char string[], int length) {
	int i,j;
	char test[] = "1234567890";
	for (i = 0; i < length; i++) {
		if (strchr(test, string[i]) != 0) {
			for (j = i+1; j < length; j++) {
				if (string[i] == string[j]) {
					printf("bad input");
					exit(0);
				}
			}
		}
		else {
			printf("bad input");
			exit(0);
		}
	}
	return 1;
}

void swap(char string[], int i, int j) {
	char a;
	a = string[j];
	string[j] = string[i];
	string[i] = a;
}

int permute(char string[], int length) {
	int i = length - 2;
	while (i >= 0 && string[i] > string[i+1])
		i--;
    if (i == -1)
        return 0;
	int j = length - 1;
	while (j>=0 && string[j] < string[i])
		j--;
	swap(string, i, j);
	i++;
	j = length-1;
	while (i < j)
	{
		swap(string, i, j);
		i++;
		j--;
	}
	return 1;
}
