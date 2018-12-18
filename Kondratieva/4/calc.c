#include <stdio.h>
#include "stack.h"
#include "calc.h"

static int isDigit(char digit) {
	return digit <= '9' && digit >= '0';
}

static int isOperator(char op) {
	return op == '+' || op == '-' || op == '*' || op == '/';
}

static int getNumber(char* buf) {
	int num = 0;
	for (int i = 0; buf[i]; i++)
		num = 10 * num + buf[i] - '0';
	return num;
}

static int prior(char op) {
	switch (op) {
	case '+':
	case '-':
		return 0;
	case '*':
	case '/':
		return 1;
	default:
		return -1;
	}
}

static void addSpace(char* expr, int* i) {
	if (!isOperator(expr[*i - 1]) && expr[*i - 1] != ' ')
		expr[(*i)++] = ' ';
}

void convert(const char* infix, char* postfix) {
	int j = 0;
	node* stack = NULL;
	for (int i = 0; infix[i]; i++)
		if (isDigit(infix[i]))
			postfix[j++] = infix[i];
		else if (infix[i] == '(')
			push(&stack, infix[i]);
		else if (infix[i] == ')') {
			addSpace(postfix, &j);
			while (top(stack) != '(')
				postfix[j++] = pop(&stack);
			pop(&stack);
		}
		else { // operator
			addSpace(postfix, &j);
			while (!empty(stack) && prior(infix[i]) <= prior(top(stack)))
				postfix[j++] = pop(&stack);
			push(&stack, infix[i]);
		}
	addSpace(postfix, &j);
	while (!empty(stack))
		postfix[j++] = pop(&stack);
}

static int calc(int a, int b, char op) {
	switch (op) {
	case '+':
		return a + b;
	case '-':
		return a - b;
	case '*':
		return a * b;
	case '/':
		if (!b) {
			printf("division by zero\n");
			exit(0);
		}
		return a / b;
	}

}

int calculate(const char* expr, char* buf) {
	int num = 0;
	node* stack = NULL;
	for (int i = 0, j = 0; expr[i]; i++)
		if (isDigit(expr[i]))
			buf[j++] = expr[i];
		else if (expr[i] == ' ') {
			buf[j] = '\0';
			num = getNumber(buf);
			j = 0;
			push(&stack, num);
		}
		else { // operator
			int a, b, res;
			b = pop(&stack);
			a = pop(&stack);
			res = calc(a, b, expr[i]);
			push(&stack, res);
		}
	return pop(&stack);
}

int checkInput(const char* expr) {
	int i = 0, cnt = 0, prevOp, prevDig;
	if (expr[i] == '(')
		cnt++;
	else if (!isDigit(expr[i]))
		return 0;
	for (i++; expr[i]; i++) {
		prevOp = isOperator(expr[i - 1]);
		prevDig = isDigit(expr[i - 1]);
		if (isDigit(expr[i])) {
			if (expr[i - 1] != '(' && !prevDig && !prevOp)
				return 0;
		}
		else if (expr[i] == '(') {
			cnt++;
			if (expr[i - 1] != '(' && !prevOp)
				return 0;
		}
		else if (expr[i] == ')') {
			cnt--;
			if (expr[i - 1] != ')' && !prevDig || cnt < 0)
				return 0;
		}
		else if (isOperator(expr[i])) {
			if (expr[i - 1] != ')' && !prevDig)
				return 0;
		}
		else
			return 0;
	}
	if (cnt || isOperator(expr[i - 1]))
		return 0;
	return 1;
}

