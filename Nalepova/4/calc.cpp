#include <stdio.h>
#include <stdlib.h>

typedef struct CS {
	char stack[1000];
	int count;
} cstack;

char top(const cstack*c) {
	return c->stack[c->count - 1];
}

void push(cstack*c, char el) {
	c->stack[c->count] = el;
	c->count++;
}

char pop(cstack*c) {
	c->count--;
	return c->stack[c->count];
}

typedef struct IS {
	int stack[1000];
	int count;
} istack;

int top(const istack*c) {
	return c->stack[c->count - 1];
}

void push(istack*c, int el) {
	c->stack[c->count] = el;
	c->count++;
}

int pop(istack*c) {
	c->count--;
	return c->stack[c->count];
}

int ch_to_digit(char k) {
	char alph[] = "0123456789";
	for (int i = 0; i < 11; i++) {
		if (alph[i] == k) {
			return i;
		}
	}
	return -1;
}

int ch_is_oper(char k) {
	char alph[] = "()*/+-";
	for (int i = 0; i < 6; i++) {
		if (alph[i] == k) {
			return 1;
		}
	}
	return 0;
}

char priority(char op) {
	switch (op) {
	case '(':
	case ')':
		return 1;
	case '+':
	case '-':
		return 2;
	case '*':
	case '/':
		return 3;
	}
}

int calc_oper(istack* values, char oper) {
	if (values->count < 2) {
		printf("syntax error");
		return 0;
	}
	if (oper == '/'&&top(values) == 0) {
		printf("division by zero");
		return 0;
	}
	int a = pop(values);
	int b = pop(values);
	switch (oper) {
	case '+':
		push(values, a + b);
		break;
	case '-':
		push(values, b - a);
		break;
	case '*':
		push(values, a*b);
		break;
	case '/':
		push(values, b / a);
		break;
	}
	return 1;
}

int calc(int *result) {
	int dmode = 0;
	istack values;
	values.count = 0;
	cstack opers;
	opers.count = 0;
	while (1 == 1) {
		int k = getchar();
		if ((char)k == '\n' || (char)k == '\r' || k == EOF) {
			break;
		}
		int digit = ch_to_digit(k);
		if (digit >= 0) {
			if (dmode == 1) {
				push(&values, pop(&values) * 10 + digit);
			}
			else {
				push(&values, digit);
			}
			dmode = 1;
		}
		else if (ch_is_oper(k) == 1) {
			if (dmode == 1 && (char)k == '(') {
				printf("syntax error");
				return 0;
			}
			if (dmode == 2 && (char)k == ')' && top(&opers) == '(') {
				printf("syntax error");
				return 0;
			}
			dmode = 0;
			if ((char)k == '(') {
				dmode = 2;
				push(&opers, k);
			}
			else if ((char)k == ')') {
				while (opers.count > 0 && top(&opers) != '(') {
					if (calc_oper(&values, pop(&opers)) == 0) {
						return 0;
					}
				}
				if (opers.count > 0 && top(&opers) == '(') {
					pop(&opers);
				}
				else {
					printf("syntax error");
					return 0;
				}
			}
			else {
				while (opers.count > 0 && priority(top(&opers)) >= priority(k)) {
					if (calc_oper(&values, pop(&opers)) == 0) {
						return 0;
					}
				}
				push(&opers, k);
			}
		}
		else {
			printf("syntax error");
			return 0;
		}
	}
	while (opers.count > 0) {
		if (calc_oper(&values, pop(&opers)) == 0) {
			return 0;
		}
	}
	if (values.count == 1)
		*result = pop(&values);
	else {
		printf("syntax error");
		return 0;
	}
	return 1;
}

int main() {
	int result;
	if (calc(&result) == 1)
		printf("%d", result);
	return 0;
}