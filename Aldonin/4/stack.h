#ifndef STACK_H
#define STACK_H

typedef struct Stack
{
    int *stack;
    int* iter;
    int iterNum;
    int size;
} Stack;

void init(Stack *s,int size);
void push(Stack *s,int c);
int pop(Stack *s);
void mod(Stack *s,int c);
int sneak(Stack *s);

#endif // STACK_H
