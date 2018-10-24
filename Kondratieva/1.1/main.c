#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LEN_STR 1000
#define MAX_LEN_PAT 17

int pow3(int x) {
    static int pow3[MAX_LEN_PAT - 1] = { 0 };
    if (!pow3[0]) {
        pow3[0] = 1;
        for (int i = 1; i < MAX_LEN_PAT - 1; i++) {
           pow3[i] = 3*pow3[i - 1];
        }
    }
    return pow3[x];
}

int hashChar(unsigned char c, int i) {
    return (c%3)*pow3(i);
}

int hashString(unsigned char* str, int len) {
    int hash = 0;
    for (int i = 0; i < len; i++) {
        hash  += hashChar(str[i], i);
    }
    return hash;
}

void shift(unsigned char* str, int len) {
    for (int i = 0; i < len; i++) {
        str[i] = str[strlen(str) - len + i];
    }
}

int getString(int shift, unsigned char* str) {
    int count = fread(&str[shift], 1, (size_t) (MAX_LEN_STR - shift), stdin);
    str[shift + count] = '\0';
    if (count == MAX_LEN_STR - shift)
        return 1;
    return 0;
}

void search(const unsigned char* pat, const unsigned char* str, int* patHash, int* strHash, int* count) {
    int strLength = strlen(str), patLength = strlen(pat);
    *strHash += hashChar(str[patLength - 1], patLength - 1);
    for (int i = 0; i <= strLength - patLength; i++) {
        if (*patHash == *strHash) {
            int j = -1;
            do {
                j++;
                printf("%d ", *count + i + j);
            } while (str[i + j] == pat[j] && j < patLength - 1);
        }
        *strHash -= hashChar(str[i], 0);
        *strHash /= 3;
        if (str[i + patLength] != '\0')
            *strHash += hashChar(str[i + patLength], patLength - 1);
    }
    *count += strLength - patLength + 1;
}

void rabinKarp(unsigned char* pat, unsigned char* str) {
    int textIsOn, patLength, patHash, strHash, count = 1;
    scanf("%[^\n]s", pat);
    getchar();
    patLength = strlen(pat);
    textIsOn = getString(0, str);
    patHash = hashString(pat, patLength);
    strHash = hashString(str, patLength-1);
    printf("%d ", patHash);
    search(pat, str, &patHash, &strHash, &count);
    while (textIsOn) {
        shift(str, patLength - 1);
        textIsOn = getString(patLength-1, str);
        search(pat, str, &patHash, &strHash, &count);
    }
}

int main() {
    unsigned char str[MAX_LEN_STR], pat[MAX_LEN_PAT];
    rabinKarp(pat, str);
	return 0;
}
