#include <stdlib.h>
#include <stack.h>

void init(Stack *s,int size)
{
    s->size = size;
    s->stack = (int*) malloc((size)*sizeof(int));
    s->iter = &s->stack[0];
    s->iterNum = 0;
}

void push(Stack *s,int c)
{
    if(s->iterNum == s->size)
    {
        int size = s->size;
        s->size*=2;
        s->stack = (int*) realloc(s->stack,(s->size)*sizeof(int));
        s->iter = &s->stack[size];
    }
    *s->iter = c;
    s->iter++;
    s->iterNum++;
}

int pop(Stack *s)
{
    if(s->iterNum>=0)
    {
        s->iter--;
        s->iterNum--;
    }
    else
        return -1;
    return *s->iter;
}

void mod(Stack *s, int c)
{
    s->iter--;
    *s->iter = c;
    s->iter++;
}

int sneak(Stack *s)
{
    s->iter--;
    int a = *s->iter;
    s->iter++;
    if(s->iterNum != 0)
        return a;
    else
        return -2000;
}
