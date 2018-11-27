#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "stack.h"
#include "calculator.h"

int IsOperator(char ch) {
	if (ch == '+' ||
		ch == '-' ||
		ch == '*' ||
		ch == '/' ||
		ch == '^') {
		return 1;
	}
	else {
		return 0;
	}
}

int Power(int base, int p)
{
	int result = 1;
	while (p)
	{
		if ((p % 2) != 0) result *= base;
		base *= base;
		p /= 2;
	}
	return result;
}

int GetOpPriority(const char op) {
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
	case '^':
		return 4;
	default:
		return 0;
	}
}

void ToPolishNotation(const char *initialExpression, char *expressionInPolish) {
	Stack *operationsStack = Create();
	int j = 0;
	for (int i = 0; initialExpression[i] != '\0'; ++i) {
		switch (initialExpression[i]) {
			case '(':
				Push(operationsStack, '(');
				break;
			case '+':
			case '-':
			case '*':
			case '/':
			case '^':
				if (IsEmpty(operationsStack) || 
					GetOpPriority(initialExpression[i]) > 
					GetOpPriority((char)Peek(operationsStack))) {
					expressionInPolish[j] = ' ';
					++j;
					Push(operationsStack, initialExpression[i]);
				}
				else{
					while (!IsEmpty(operationsStack) && 
						GetOpPriority(initialExpression[i]) <= 
						GetOpPriority((char)Peek(operationsStack))) {
						expressionInPolish[j] = (char)Pop(operationsStack);
						++j;
					}
					Push(operationsStack, initialExpression[i]);
				}
				break;
			case ')':
				{
					char c;
					while ((c = (char)Pop(operationsStack)) != '(') {
						expressionInPolish[j] = c;
						++j;
					}
				}
				break;
			default: // is digit
				expressionInPolish[j] = initialExpression[i];
				++j;
				break;
		}
	}
	while (!IsEmpty(operationsStack)) {
		expressionInPolish[j] = (char)Pop(operationsStack);
		++j;
	}
	expressionInPolish[j] = '\0';
	Destroy(operationsStack);
}

int GetToken(const char *expression, int *startPos, int *flag) {
	//flag is 1 if token is number
	//flag is 0 if token is operation
	//flag is -1 if there is no tokens (end of expression)
	if (expression[*startPos] == '\0') {
		*flag = -1;
		return 0;
	}
	else if (isdigit(expression[*startPos])) {
		*flag = 1;
		int number = 0;
		while (expression[*startPos] != ' ' &&
			!IsOperator(expression[*startPos]) && 
			expression[*startPos] != '\0') {
			number = number * 10 + (expression[*startPos] - '0');
			*startPos += 1;
		}
		if (expression[*startPos] == ' ') {
			*startPos += 1;
		}
		return number;
	}
	else {
		*flag = 0;
		*startPos += 1;
		return expression[*startPos - 1];
	}
}

int CalculateInPolishNotation(const char *expression) {
	int result;
	int i = 0;
	int flag;
	int token;
	Stack *calcStack = Create();
	for (token = GetToken(expression, &i, &flag); flag != -1; token = GetToken(expression, &i, &flag)) {
		if (flag == 1) {
			Push(calcStack, token);
		}
		else {
			switch ((char)token) {
				case '+':
					result = Pop(calcStack) + Pop(calcStack);
					break;
				case '-':
					{
						int b = Pop(calcStack);
						int a = Pop(calcStack);
						result = a - b;
					}
					break;
				case '*':
					result = Pop(calcStack) * Pop(calcStack);
					break;
				case '/':
					{
						int denominator = Pop(calcStack);
						int numerator = Pop(calcStack);
						if (denominator == 0) {
							printf("division by zero");
							exit(0);
						}
						result = numerator / denominator;
					}
					break;
				case '^':
					{
						int power = Pop(calcStack);
						int base = Pop(calcStack);
						result = Power(base, power);
					}
					break;
			}
			Push(calcStack, result);
		}
	}
	result = Pop(calcStack);
	Destroy(calcStack);
	return result;
}

int Calculate(const char *expression) {
	char expressionInPolishNotation[MAX_LEN * 2];
	ToPolishNotation(expression, expressionInPolishNotation);
	return CalculateInPolishNotation(expressionInPolishNotation);
}

void Input(char *expression) {
	int i = 0;
	int digitFlag = 0;
	for (;;) {
		int c = getc(stdin);
		switch (c) {
			case '\0':
			case '\n':
			case EOF:
				expression[i] = '\0';
				return;
			case '\t':
			case ' ':
				if (isdigit(expression[i - 1])) {
					digitFlag = 1;
				}
				break;
			default:
				if (isdigit(c)) {
					if (digitFlag == 1) {
						printf("syntax error");
						exit(0);
					}
				}
				else {
					digitFlag = 0;
				}
				if (i <= MAX_LEN - 1) {
					expression[i] = c;
					++i;
				}
				else {
					expression[i] = '\0';
					return;
				}
				break;
		}
	}
	expression[i] = '\0';
}
// ??
int Validation(const char *expression) {
	int openedBracketsCount = 0;
	int closedBracketsCount = 0;
	int i;
	for (i = 0; expression[i] != '\0'; ++i) {
		if (isdigit(expression[i])) {
			if (expression[i + 1] == '(') {
				return 0;
			}
		}
		else 
			if (IsOperator(expression[i])) {
					if (i == 0 || expression[i+1] == ')' || IsOperator(expression[i+1]) || expression[i + 1] == '\0') {
						return 0;
					}
			}
			else 
				if (expression[i] == '(') {
					if (expression[i+1] == ')' || IsOperator(expression[i + 1])) {
						return 0;
					}
					++openedBracketsCount;
				}
				else
					if (expression[i] == ')') {
						if (i == 0 || expression[i + 1] == '(' || isdigit(expression[i + 1])) {
							return 0;
						}
						++closedBracketsCount;
					}
					else {
						return 0;
					}
		if (closedBracketsCount > openedBracketsCount) {
			return 0;
		}
	}
	if (i == 0 || openedBracketsCount != closedBracketsCount) {
		return 0;
	}
	return 1;
}
