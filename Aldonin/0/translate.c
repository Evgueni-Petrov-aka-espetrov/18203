#include "translate.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "stdbool.h"

/*---------------------------Всякое нужное---------------------------*/

int atoi(char c)//Переводит символ в цифру
{
    if(c >= '0' && c <= '9')//От 0 до 9
        return c-'0';

    if(c >= 'A' && c <= 'F')//От 10(A) до 15(F)
        return c-'A'+10;
    if(c >= 'a' && c <= 'f')
        return c-'a'+10;

    return -1;//-1 - если не цифра
}

char itoa(int i)//Переводит цифру в символ
{
    if(i >= 0 && i <= 9)//От 0 до 9
        return i+'0';

    else if(i <= 15)//От 10(A) до 15(F)
        return i+'A'-10;

    return -1;//-1 - если цифра не в диапозоне от 0 до 15
}

int expo(int a, int b)//Замена exp
{
    int c = a;
    for(int i = 1;i < b;i++)
        a *= c;
    return a;
}

bool isInf(double d)
{
    if(d == INFINITY)
        return true;
    return false;
}

/*-------------------------------------------------------------------*/

int failSafe(int b1, int b2, char* number)//Проверка
{
    if(b1 < 2 || b1 > 16)//Если b1 или b2 не входят в диапозон от 2 до 16
        return 1;
    if(b2 < 2 || b2 > 16)
        return 2;

    bool testDot = false;
    for(int i = 0;number[i] != 0;++i)//0 = '\0' = NULL
    {
        int c = atoi(number[i]);
        if(c >= b1)//Если цифра не входит в начальную систему счисления
            return 3;

        if(c == -1 && number[i] != '.')//Если есть лишний символ
            return 4;

        if(i > 13)//Если число слишком большое
            return 5;

        if(number[i] == '.' && !testDot)//Если есть лишняя точка
            testDot = true;
        else if(number[i] == '.' && testDot)
            return 6;

        if(i == 0 && number[i] == '.')//Если точка в начале строки
            return 7;
        if(number[i] == '.' && number[i+1] == 0)//Если точка в конце строки
            return 8;
    }

    return 0;//Успешно
}

double numToDouble(int base, char* number)//Строку в double
{
    double ret = 0,preRet = 0;
    bool isDot = false;

    for(int i = 0;number[i] != 0;++i)
    {
        char c = number[i];

        if(c == '.')
        {
            isDot = true;
            continue;
        }

        double a = atoi(c);
        if(isDot)//Если целая часть
        {
            preRet *= base;
            preRet += a;
            if(!isInf(preRet)) ret = preRet;// <> ? <> : <> обязательно требует 2 выражения, поэтому бесполезен здесь
        }
        else//Если дробная часть
        {
            preRet += a/(expo(base,i-isDot));
            if(!isInf(preRet)) ret = preRet;
        }
    }
    return ret;//И-и-и..
}

int intToNum(char* output, int base,long long int number)//Double в строку
//Имхо, я искренне ненавижу то, что функция не может нормально возвращать строки в C. Ну, или я не знаю как.
{
    if(number == 0)//Слегка ускоряет процесс
    {
        output[0] = '0';
        return 1;
    }

    int size = 0;//Развер строки
    char buffer[255];
    for(int i = 0;number != 0;i++,size++)
    {
        int c = number % base;
        buffer[i] = itoa(c);
        number /= base;
    }

    for(int i = 0;i < size;i++)//Изначально строка перевернута. Это переворачивает ее обратно.
    {
        output[i] = buffer[size-i-1];
    }
    return size;
}

void floatToNum(char* buffer, int base,double number)
{
    if(number == 0.0)
        return 1;

    for(int i = 0;i < 12;i++)
    {
        int c = number*base;
        number = (number*base)-c;
        buffer[i] = itoa(c);
    }
    buffer[12] = 0;
    if(strcmp(buffer,"000000000000")!=0)
    {
        for(int a = 11;a >= 0;a--)
        {
            if(buffer[a] == '0')
                buffer[a] = 0;
            else
                break;
        }
    }
}

void doubleToNum(char* output, int base,double number)
{
    long long int n = number;//Отдает целую часть
    int length = intToNum(&output[0],base,n);//strlen по какой-то причине возвращала слишком большое значение.
    double part = number-n;//Отдает дробную часть
    floatToNum(&output[length+1],base,part);
        output[length] = '.';
        output[length] = 0;
}
