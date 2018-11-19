#include <stdio.h>
#include <ctype.h>

#define MAXLEN 1000

int IsCorrectInput(int radixFrom, int radixTo, char sourceNum[]);
void ConvertNum(int radixFrom, int radixTo, char sourceNum[], char resNum[]);
int CharToNum(char digit);
char NumToChar(int digit);
void ConvertIntPart(int radixFrom, int radixTo, char intPart[], char resNum[]);
void ConvertFrancPart(int radixFrom, int radixTo, char francPart[], char resNum[]);
void ReverseList(char list[]);

int main()
{
    int radixFrom, radixTo, i, c;
    char sourceNum[MAXLEN], resNum[MAXLEN];

    scanf("%d %d", &radixFrom, &radixTo);
    getchar();
    for (i = 0; (c = getchar()) != '\n'; ++i) {
        sourceNum[i] = c;
    }
    sourceNum[i] = '\0';

    if (IsCorrectInput(radixFrom, radixTo, sourceNum)) {
        ConvertNum(radixFrom, radixTo, sourceNum, resNum);
        printf("%s", resNum);
    }
    else {
        printf("bad input");
    }

    return 0;
}

int IsCorrectInput(int radixFrom, int radixTo, char num[])
{
    int i, prev, dotCount;

    if (radixFrom < 2 || radixFrom > 16 || radixTo < 2 || radixTo > 16) {
        return 0;
    }

    prev = 'a';
    dotCount = 0;
    for (i = 0; num[i] != '\0'; ++i) {
        if (CharToNum(num[i]) >= radixFrom && num[i] != '.') {
            return 0;
        }
        prev = num[i];
        if (num[i] == '.') {
            ++dotCount;
        }
    }
    if (prev == '.') {
        return 0;
    }
    if (num[0] == '.') {
        return 0;
    }
    if (dotCount > 1) {
        return 0;
    }

    return 1;
}

void ConvertNum(int radixFrom, int radixTo, char sourceNum[], char resNum[])
{
    char intPart[MAXLEN], francPart[MAXLEN];
    int i, j;

    for (i = 0; sourceNum[i] != '.' && sourceNum[i] != '\0'; ++i) {
        intPart[i] = sourceNum[i];
    }
    intPart[i] = '\0';
    ConvertIntPart(radixFrom, radixTo, intPart, resNum);

    if (sourceNum[i] == '.') {
        ++i;
        for (j = 0; sourceNum[i] != '\0'; ++j, ++i) {
            francPart[j] = sourceNum[i];
        }
        francPart[j] = '\0';
        ConvertFrancPart(radixFrom, radixTo, francPart, resNum);
    }
}

int CharToNum(char digit)
{
    if (digit >= '0' && digit <= '9') {
        return digit - '0';
    }
    else {
        return tolower(digit) - 'a' + 10;
    }
}

char NumToChar(int digit)
{
    if (digit >= 0 && digit <= 9) {
        return digit + '0';
    }
    else {
        return digit + 'a' - 10;
    }
}

void ConvertIntPart(int radixFrom, int radixTo, char intPart[], char resNum[])
{
    int i;
    unsigned long long num, degree;

    ReverseList(intPart);
    num = 0;
    degree = 1;
    for (i = 0; intPart[i] != '\0'; ++i) {
        num += degree * CharToNum(intPart[i]);
        degree *= radixFrom;
    }

    for (i = 0; num; ++i) {
        resNum[i] = NumToChar(num % radixTo);
        num /= radixTo;
    }
    resNum[i] = '\0';
    ReverseList(resNum);
    if (resNum[0] == '\0') {
        resNum[0] = '0';
        resNum[1] = '\0';
    }
}

void ConvertFrancPart(int radixFrom, int radixTo, char francPart[], char resNum[])
{
    int i, j;
    long double num;
    long double degree;

    num = 0;
    degree = 1.0 / radixFrom;
    for (i = 0; francPart[i] != '\0'; ++i) {
        num += CharToNum(francPart[i]) * degree;
        degree /= radixFrom;
    }

    for (i = 0; resNum[i] != '\0'; ++i)
        ;
    resNum[i] = '.';
    ++i;

    for (j = 0; j < 12; ++j, ++i) {
        resNum[i] = NumToChar((int)(num * radixTo));
        num = num * radixTo - (int)(num * radixTo);
    }
    resNum[i] = '\0';
}

void ReverseList(char list[])
{
    int len, i;
    char temp;

    for (i = 0; list[i] != '\0'; ++i)
        ;
    len = i;

    for (i = 0; len / 2 != i; ++i) {
        temp = list[i];
        list[i] = list[len - i - 1];
        list[len - i - 1] = temp;
    }
}