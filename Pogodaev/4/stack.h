#pragma once
#define STACK_SIZE 1000
typedef int T;
typedef struct {
	T buffer[STACK_SIZE];
	int position;
} Stack;

Stack *Create(void);
void Destroy(Stack*);
void Push(Stack*, T);
T Pop(Stack*);
T Peek(const Stack*);
int IsEmpty(const Stack*);
