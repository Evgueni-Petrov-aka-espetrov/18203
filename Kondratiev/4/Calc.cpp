#include <stdio.h>
#include <stdlib.h>

int calc(char* s);
int calcLayer(char* s, int* position);
int getNumber(char* s, int* position);
int actionResult(int number1, char action, int number2);
bool goodInput(char* s);

int main()
{
	char s[1001] = "";
	gets_s(s);
	if (goodInput(s)) {
		printf_s("%d", calc(s));
	}else
		printf_s("%s", "syntax error");
}

int calc(char* s) {
	int position = 0;
	return calcLayer(s, &position);
}

int calcLayer(char* s, int* position) {
	int numberBuffer1 = 0, numberBuffer2, numberBuffer3;
	char actionBuffer1 = '+', actionBuffer2;
	numberBuffer2 = getNumber(s, position);
	while (s[*position] != ')' && s[*position] != '\0') {
		actionBuffer2 = s[(*position)++];
		numberBuffer3 = getNumber(s, position);
		if ((actionBuffer1 == '+' || actionBuffer1 == '-') && (actionBuffer2 == '*' || actionBuffer2 == '/')) {
			numberBuffer2 = actionResult(numberBuffer2, actionBuffer2, numberBuffer3);
		}else {
			numberBuffer1 = actionResult(numberBuffer1, actionBuffer1, numberBuffer2);
			actionBuffer1 = actionBuffer2;
			numberBuffer2 = numberBuffer3;
		}
	}
	(*position)++;
	return actionResult(numberBuffer1, actionBuffer1, numberBuffer2);
}

int getNumber(char* s, int* position) {
	int number = 0;
	if (s[*position] == '(') {
		(*position)++;
		number = calcLayer(s, position);
	}
	else
		while (s[*position] >= '0' && s[*position] <= '9')
			number = number * 10 + (s[(*position)++] - '0');
	return number;
}

int actionResult(int number1, char action, int number2) {
	int result;
	switch (action){
	case '+': result = number1 + number2; break;
	case '-': result = number1 - number2; break;
	case '*': result = number1 * number2; break;
	case '/':
		if (number2 == 0) {
			printf_s("%s","division by zero");
			exit(0);
		}
		result = number1 / number2;
	}
}

bool goodInput(char* s) {
	int layer = 0;		//кол-во пройденных '(' - кол-во пройденных ')'
	int i;
	if ((s[0] < '0' || s[0] > '9') && s[0] != '(') return false;
	if (s[0] == '(') layer++;
	for (i = 1; s[i]; i++) {
		switch (s[i]) {
		case '(':
			if (s[i - 1] == ')' || (s[i - 1] >= '0' && s[i - 1] <= '9')) return false;
			layer++;
			break;
		case ')':
			if (--layer < 0) return false;
		case '+': case '-': case '*': case '/':
			if ((s[i - 1] < '0' || s[i - 1] > '9') && s[i - 1] != ')') return false;
			break;
		default:
			if (s[i] < '0' || s[i] > '9') return false;
		}
	}
	if ((s[i - 1] < '0' || s[i - 1] > '9') && s[i - 1] != ')') return false;
	if (layer != 0) return false;
	return true;
}