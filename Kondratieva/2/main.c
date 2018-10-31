#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 11

void swap(char* a, int i, int j) {
    int t = a[i];
    a[i] = a[j];
    a[j] = t;
}

int nextPermutation(char* a, int n) {
    int i = n - 1;
    while (i > 0 && a[i - 1] >= a[i])
        i--;
    if (!i)
        return 0;
    int j = n - 1;
    while (a[j] <= a[i - 1])
        j--;
    swap(a, i - 1, j);
    j = n - 1;
    while (i++ < j--)
        swap(a, i, j);
    return 1;
}

int checkInput(char* a) {
    int s[MAX_LENGTH - 1] = { 0 };
    for (int i  = 0; i < strlen(a); i++) {
        if(s[a[i] - '0'] || a[i] > '9' || a[i] < '0') {
            return 0;
        }
        s[a[i] - '0'] = 1;
    }
    return 1;
}

void printAnswer(char* a, int n) {
    if (!checkInput(a)) {
        printf("bad input");
        return;
    }
    while (nextPermutation(a, strlen(a)) && n--)
        printf("%s\n", a);
}

int main()
{
    int n;
    char a[MAX_LENGTH];
    scanf("%s%d", a, &n);
    printAnswer(a, n);
    return 0;
}
