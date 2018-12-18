#include "stack.h"

int empty(const node* stack) {
	return stack == NULL;
}

void push(node** stack, int value) {
	node* tmp = malloc(sizeof(node));
	tmp->value = value;
	tmp->next = *stack;
	*stack = tmp;
}

int pop(node** stack) {
	assert(stack);
	int val = (*stack)->value;
	node* tmp = *stack;
	*stack = (*stack)->next;
	free(tmp);
	return val;
}

int top(const node* stack) {
	assert(stack);
	return stack->value;
}
