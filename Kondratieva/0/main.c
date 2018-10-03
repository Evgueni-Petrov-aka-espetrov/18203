#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ULL unsigned long long
#define MAX_IN 14
#define MAX_OUT 50

//возвращает число соответствующее опр. символу
int GetNum(char c) {
    if (c >= '0' && c <= '9')
        return c - '0';
    return c - 'a' + 10;
}

//функция обратная getnum
char GetChar(int n) {
    if (n >= 0 && n <= 9)
        return n + '0';
    return n + 'a' - 10;
}

//переводит целую часть числа в десятичную систму счисления
ULL ToIntDec(char *s, int b) {
    ULL x = 0;
    for (int i = 0; s[i] != '\0'; i++)
        x = x*b + GetNum(s[i]);
    return x;
}

//дробную часть числа в дес. сс
double ToFracDec(char *s, int b) {
    double x = 0, p = 1.0/b;
    for (int i = 0; s[i] != '\0'; i++) {
        x += p*GetNum(s[i]);
        p /= b;
    }
    return x;
}

//целую часть числа из дес. сс в требуемую
void FromIntDec(char *s, ULL a, int b) {
    if (!a) return;
    FromIntDec(s, a/b,  b);
    s[strlen(s)] = GetChar(a%b);
}

//дробную часть чилса из дес. сс в требуемую
void FromFracDec(char *s, double a, int b) {
    int n = 12;
    for (int i = 0; i < n; i++) {
        a *= b;
        s[i] = GetChar((int)a);
        a -= (int)a;
    }
}

//переводит символ в нижний регистр
char Down(char c) {
    if (c <= 'Z' && c >= 'A')
        return c + 32;
    return c;
}

//проверяет основания
int CheckBases(int b1, int b2) {
    return (b1 > 16 || b1 < 2 || b2 > 16 || b2 < 2);
}

//проверяет число поступившее на вход
int CheckString(char *s, int b) {
    int cnt = 0;
    for (int i = 0; s[i] != '\0'; i++) {
        s[i] = Down(s[i]);
        if (s[i] == '.') {
            cnt++;
            if (cnt > 1 || i == 0 || s[i+1] == '\0')
                return 1;
        }
        else if (GetNum(s[i]) >= b)
            return 1;
    }
    return 0;
}

//выводит ответ
void PrintAns(int b1, int b2, char *s) {
    char ans[MAX_OUT] = {};
    char *p = strchr(s, '.');
    if (p != NULL)
        *p++ = '\0';
    FromIntDec(ans, ToIntDec(s, b1), b2);
    printf("%s", ans);
    if (s[0] == '0')
        printf("0");
    if (p != NULL) {
        FromFracDec(ans, ToFracDec(p, b1), b2);
        printf(".%s", ans);
    }
}

int main() {
    int b1, b2;
    char x[MAX_IN];
    scanf("%d%d%s", &b1, &b2, x);

    if (CheckBases(b1, b2) || CheckString(x, b1)) {
        printf("bad input");
        exit(0);
    }

    PrintAns(b1, b2, x);

    return 0;
}
