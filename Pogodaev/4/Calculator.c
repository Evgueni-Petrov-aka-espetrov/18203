#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "stack.h"
#include "calculator.h"
//
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
//
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
//
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
//
int ComparePriorities(char firstOp, char secOp) {
	if (GetOpPriority(firstOp) > GetOpPriority(secOp)) return 1;
	else if (GetOpPriority(firstOp) == GetOpPriority(secOp)) return 0;
	else return -1;
}
//
void AddSeparator(char *expression, int *position) {
	expression[*position] = ' ';
	++(*position);
	return;
}
//
void AddOpToExpression(char operator, Stack *opStack, char *expressionInPolish, int *positionInExpression) {
	AddSeparator(expressionInPolish, positionInExpression);
	while (!IsEmpty(opStack) &&
		ComparePriorities(operator, (char)Peek(opStack)) <= 0) {
		expressionInPolish[*positionInExpression] = (char)Pop(opStack);
		++(*positionInExpression);
		AddSeparator(expressionInPolish, positionInExpression);
	}
	Push(opStack, operator);
}
//
void ToPolishNotation(const char *initialExpression, char *expressionInPolish) {
	Stack *operationsStack = Create();
	int j = 0;
	for (int i = 0; initialExpression[i] != '\0'; ++i) {
		if (initialExpression[i] == '(') {
			Push(operationsStack, initialExpression[i]);
		}
		else 
			if (IsOperator(initialExpression[i])) {
				AddOpToExpression(initialExpression[i], operationsStack, expressionInPolish, &j);
			}
			else
				if (initialExpression[i] == ')') {
					char c;
					while ((c = (char)Pop(operationsStack)) != '(') {
						AddSeparator(expressionInPolish, &j);
						expressionInPolish[j] = c;
						++j;
					}
				}
				else {
					expressionInPolish[j] = initialExpression[i];
					++j;
				}
	}
	while (!IsEmpty(operationsStack)) {
		AddSeparator(expressionInPolish, &j);
		expressionInPolish[j] = (char)Pop(operationsStack);
		++j;
	}
	expressionInPolish[j] = '\0';
	Destroy(operationsStack);
}
//
int DivideStackTopValues(Stack *calcStack) {
	int denominator = Pop(calcStack);
	int numerator = Pop(calcStack);
	if (denominator == 0) {
		printf("division by zero");
		exit(0);
	}
	return numerator / denominator;
}
//
int SubtractStackTopValues(Stack *calcStack) {
	int b = Pop(calcStack);
	int a = Pop(calcStack);
	return a - b;
}
//
int PowerStackTopValues(Stack *calcStack) {
	int power = Pop(calcStack);
	int base = Pop(calcStack);
	return Power(base, power);
}
//
int CalculateStackTopValues(char operator, Stack *calcStack) {
	int result;
	switch (operator) {
		case '+':
			result = Pop(calcStack) + Pop(calcStack);
			break;
		case '-':
			result = SubtractStackTopValues(calcStack);
			break;
		case '*':
			result = Pop(calcStack) * Pop(calcStack);
			break;
		case '/':
			result = DivideStackTopValues(calcStack);
			break;
		case '^':
			result = PowerStackTopValues(calcStack);
			break;
	}
	return result;
}
//
int GetNumberToken(const char *expression, int *startPos) {
	int number = 0;
	while (expression[*startPos] != ' ' &&
		expression[*startPos] != '\0') {
		number = number * 10 + (expression[*startPos] - '0');
		*startPos += 1;
	}
	if (expression[*startPos] == ' ') {
		*startPos += 1;
	}
	return number;
}
//
int GetOperatorToken(const char *expression, int *startPos) {
	if (expression[*startPos + 1] == ' ') {
		*startPos += 2;
		return expression[*startPos - 2];
	}
	// if == '\0'
	else {
		*startPos += 1;
		return expression[*startPos - 1];
	}
}
//
int GetToken(const char *expression, int *startPos, int *flag) {
	//flag is 1 if token is number
	//flag is 0 if token is operation
	//flag is -1 if there is no tokens anymore (end of expression)
	if (expression[*startPos] == '\0') {
		*flag = -1;
		return -1;
	}
	else
		if (isdigit(expression[*startPos])) {
			*flag = 1;
			return GetNumberToken(expression, startPos);
		}
		else {
			*flag = 0;
			return GetOperatorToken(expression, startPos);
		}
}
//
int CalculateInPolishNotation(const char *expression) {
	int i = 0;
	int flag;
	int token = GetToken(expression, &i, &flag);
	Stack *calcStack = Create();
	while (flag != -1) {
		if (flag == 1) {
			Push(calcStack, token);
		}
		else {
			Push(calcStack, CalculateStackTopValues((char)token, calcStack));
		}
		token = GetToken(expression, &i, &flag);
	}
	int result = Pop(calcStack);
	Destroy(calcStack);
	return result;
}
//
int Calculate(const char *expression) {
	char expressionInPolishNotation[MAX_LEN * 2];
	ToPolishNotation(expression, expressionInPolishNotation);
	return CalculateInPolishNotation(expressionInPolishNotation);
}
//
int OperatorValidation(const char *expression, int position) {
	if (position == 0 ||
		expression[position + 1] == ')' ||
		IsOperator(expression[position + 1]) ||
		expression[position + 1] == '\0') {
		return 0;
	}
	return 1;
}
//
int OpenedBracketValidation(const char *expression, int position) {
	if (expression[position + 1] == ')' ||
		IsOperator(expression[position + 1])) {
		return 0;
	}
	return 1;
}
//
int ClosedBracketValidation(const char *expression, int position) {
	if (expression[position + 1] == '(' ||
		isdigit(expression[position + 1])) {
		return 0;
	}
	return 1;
}
//
int DigitValidation(const char *expression, int position) {
	if (expression[position + 1] == '(') {
		return 0;
	}
	return 1;
}
//
int Validation(const char *expression) {
	int openedBracketsCount = 0;
	int closedBracketsCount = 0;
	int i;
	for (i = 0; expression[i] != '\0'; ++i) {
		if (isdigit(expression[i])) {
			if (!DigitValidation(expression, i)) {
				return 0;
			}
		}
		else
			if (IsOperator(expression[i])) {
				if (!OperatorValidation(expression, i)) {
					return 0;
				}
			}
			else
				if (expression[i] == '(') {
					if (!OpenedBracketValidation(expression, i)) {
						return 0;
					}
					++openedBracketsCount;
				}
				else
					if (expression[i] == ')') {
						if (!ClosedBracketValidation(expression, i)) {
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
//
int IsEnd(char c) {
	if (c == EOF || c == '\0' || c == '\n') return 1;
	else return 0;
}
//
int DoubleNumberCheck(char ch, int *flag) {
	if (isdigit(ch)) {
		if (*flag) {
			return 0;
		}
	}
	else {
		*flag = 0;
	}
	return 1;
}
//
int Input(char *expression) {
	int i = 0;
	int doubleNumberFlag = 0;
	for (;;) {
		int c = getc(stdin);
		if (IsEnd(c) || i == MAX_LEN) {
			expression[i] = '\0';
			break;
		}
		else
			if (c != ' ' && c != '\t') {
				if (!DoubleNumberCheck(c, &doubleNumberFlag)) {
					return 0;
				}
				expression[i] = c;
				++i;
			}
			else {
				if (isdigit(expression[i - 1])) {
					doubleNumberFlag = 1;
				}
			}
	}
	return Validation(expression);
}
