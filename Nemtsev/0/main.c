#include <stdio.h>
#include <stdlib.h>

// Максимальная длина входной строки
const int MaxInputLength = 13;
const int MaxOutputLength = 53;

double ReadNumber(char * input, int radix);
void WriteNumber(double value, char * output, int radix);

int main()
{
    // test git
    // Ввод оснований системы счисления
    int radix1, radix2;
    scanf("%d %d", &radix1, &radix2);

    if (radix1 < 2 || radix1 > 16)
    {
        printf("Bad input\n");
        return 0;
    }

    if (radix2 < 2 || radix2 > 16)
    {
        printf("Bad input\n");
        return 0;
    }

    // Входная строка
    char input[MaxInputLength + 1];
    scanf("%s", input);

    double value = ReadNumber(input, radix1);

    if (value < 0)
    {
        printf("Bad input\n");
        return 0;
    }

    char output[MaxOutputLength + 1]; // ??

    // Преобразование числа в текстовую запись
    WriteNumber(value, output, radix2);
    printf(output);

    return 0;
}

int GetDigitValue(char ch)
{
    if (ch >= '0' && ch <= '9')
    {
        // Код символа '0' = 48
        return ch - '0';
    }

    if (ch >= 'a' && ch <= 'f')
    {
        // Символ 'a' соответствует числу 10
        return ch - 'a' + 10;
    }

    if (ch >= 'A' && ch <= 'F')
    {
        return ch - 'A' + 10;
    }

    return -1;
}

double ReadNumber(char * input, int radix)
{
    double result = 0;

    int i = 0;
    if (input[i] == '.' || input[i] == '\0')
    {
        return -1;
    }

    while (input[i] != '.' && input[i] != '\0')
    {
        int digit = GetDigitValue(input[i]);
        if (digit < 0)
        {
            return -1;
        }

        if (digit >= radix)
        {
            return -1;
        }

        result = result * radix + digit;
        i++;
    }

    if (input[i] == '\0')
    {
        return result;
    }

    i++;
    if (input[i] == '\0')
    {
        return -1;
    }

    double divider = radix;
    while (input[i] != '\0')
    {
        int digit = GetDigitValue(input[i]);
        if (digit < 0)
        {
            return -1;
        }

        if (digit >= radix)
        {
            return -1;
        }

        result += digit / divider;
        divider *= radix;
        i++;
    }

    return result;
}

char GetCharValue(int digit)
{
    if (digit < 10)
    {
        return digit + '0';
    }

    return digit - 10 + 'A';
}

void WriteNumber(double value, char * output, int radix)
{
    int i = 0;

    double divider = 1;
    while (divider <= value)
    {
        divider *= radix;
    }

    divider /= radix;

    if (value < 1)
    {
        output[i++] = '0';
    }
    else
    {
        while (divider >= 1)
        {
            int digit = (int)(value / divider);
            value -= digit * divider;
            output[i++] = GetCharValue(digit);
            divider /= radix;
        }
    }

    if (value > 0)
    {
        output[i++] = '.';

        for (int j = 0; j < 12; j++)
        {
            int digit = (int)(value / divider);
            value -= digit * divider;
            output[i++] = GetCharValue(digit);
            divider /= radix;
        }
    }

    output[i++] = '\0';
}

