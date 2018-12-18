#pragma once
#include <stdlib.h>
#include <assert.h>

typedef struct comp {
	int value;
	struct comp* next;
} node;

int empty(const node*);
void push(node **, int);
int pop(node **);
int top(const node*);
