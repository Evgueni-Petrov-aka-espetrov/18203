#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

Stack *Create() {
	Stack *temp = (Stack*)malloc(sizeof(Stack));
	temp->position = 0;
	return temp;
}

void Destroy(Stack *stack) {
	free(stack);
}

void Push(Stack *stack, T value) {
	if (stack->position >= STACK_SIZE) {
		printf("Stack overflow");
		return;
	}
	(stack->buffer)[stack->position] = value;
	stack->position += 1;
}

T Pop(Stack *stack) {
	if (stack->position == 0) {
		printf("stack is empty");
		return 0;
	}
	stack->position -= 1;
	return (stack->buffer)[stack->position];
}

T Peek(const Stack *stack) {
	if (stack->position == 0) {
		printf("stack is empty");
		return 0;
	}
	return (stack->buffer)[stack->position - 1];
}

int IsEmpty(const Stack *stack) {
	if (stack->position == 0) return 1;
	else return 0;
}
