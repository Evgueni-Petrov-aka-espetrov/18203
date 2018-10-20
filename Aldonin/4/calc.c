#include <stdbool.h>
#include <calc.h>

#define DIV_ERR -1000

int atoi(char c)
{
    if(c >= '0' && c <= '9')
        return c-'0';

    return -1;
}

int doCalc(int a, int b, char symbol)
{
    switch (symbol)
    {
    case '+':
        return a+b;
    case '-':
        return a-b;
    case '*':
        return a*b;
    case '/':
        if(b!=0)
            return a/b;
    }
    return DIV_ERR;
}

int calc(FILE* input,Stack* numbers,Stack* symbols)
{
    bool prevIsDigit = false;
    char c = fgetc(input);
    while(c != '\n')
    {
        int num = atoi(c);
        if(num == -1)
        {
            if(c == '(')
            {
                if(prevIsDigit)
                    return -1;
            }
            else if(c == '*' || c == '/')
            {
                if(sneak(symbols) == '*' || sneak(symbols) == '/')
                {
                    push(numbers,doCalc(pop(numbers),pop(numbers),pop(symbols)));
                    if(sneak(numbers) == DIV_ERR)
                        return -2;
                }
            }
            else if(c == '+' || c == '-')
            {
                char a = sneak(symbols);
                if(a == '+' || a == '-' || a == '*' || a == '/')
                {
                    push(numbers,doCalc(pop(numbers),pop(numbers),pop(symbols)));
                    if(sneak(numbers) == DIV_ERR)
                        return -2;
                }
            }
            else if(c == ')')
            {
                char test = pop(symbols);
                bool emptyBrackets = true;
                while(test != '(')
                {
                    if(symbols->iterNum <= 0)
                        return -1;
                    if(emptyBrackets)
                        emptyBrackets = false;
                    push(numbers,doCalc(pop(numbers),pop(numbers),test));
                    test = pop(symbols);
                    if(sneak(numbers) == DIV_ERR)
                        return -2;
                }
                if(emptyBrackets&&!prevIsDigit)
                    return -1;
                c = fgetc(input);
                //prevIsDigit = false;
                continue;
            }
            else
            {
                return -1;
            }
            push(symbols,c);
            prevIsDigit = false;
        }
        else
        {
            if(prevIsDigit)
            {
                mod(numbers,(sneak(numbers)*10)+num);
            }
            else
            {
                push(numbers,num);
                prevIsDigit = true;
            }
        }
        c = fgetc(input);
    }
    while(symbols->iterNum >= 0)
    {
        if(numbers->iterNum == 0)
            return -1;
        push(numbers,doCalc(pop(numbers),pop(numbers),pop(symbols)));
        if(sneak(numbers) == DIV_ERR)
            return -2;
    }
    if(numbers->iterNum != 0)
        return -1;
    return 0;
}
