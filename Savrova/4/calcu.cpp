#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <ctype.h>
#include <stdlib.h>
#define MAXOP 100
#define NUMBER '0'
#define MAXVAL 100
#define BUFSIZE 100

using namespace std;
int getop (char s[]);
void push(double);
double pop(void);
int getch(void);
void ungetch(int);
int nextposition = 0;
double val[MAXVAL];
char buf[BUFSIZE];
int main()
{
    int type;
    double op2;
    char s[MAXOP];
    while ((type = getop(s)) != EOF)
    {
        switch(type) {
            case NUMBER:
                 push (atof(s));
                 break;
            case '+':
                push (pop() + pop());
                break;
            case '*':
                push (pop() * pop());
                break;
            case '-':
                op2 = pop();
                push (pop() - op2);
                break;
            case '/':
                op2 = pop();
                if (op2 != 0.0)
                    push (pop() / op2);
                else
                    cout <<"division by zero";
                break;
            case '\n':
                cout << pop();
                break;
            default:
                cout << "unknown command";
                break;

        }
    }
    return 0;
}

void push (double f)
{
    if (nextposition < MAXVAL)
        val[nextposition ++] = f;
    else
        cout << "Stack is full";
}

double pop (void)
{
    if (nextposition > 0)
        return val[nextposition --];
    else {
        cout << "stack is empty";
        return 0;
    }
}

int getop (char s[])
{
    int i, c;
    while ((s[0] = c = getch()) == ' ' || c == '\t');
        s[1] = '\0';
    if (!isdigit(c))
        return c;
    i = 0;
    if (isdigit(c))
        while (isdigit(s[++i] = c = getch()));
    s[i] = '\0';
    if (c != EOF)
        ungetch (c);
    return NUMBER;
}

int bufnextposition = 0;
int getch(void)
{
    if (bufnextposition > 0)
        return buf[--bufnextposition];
    else
        return getchar();
}

void ungetch(int c)
{
    if (bufnextposition >= BUFSIZE)
        cout <<"too many characters";
    else
        buf[bufnextposition ++] = c;
}


