#include <stdio.h>
#include <stdlib.h>

struct charStack {
	int elementCount;
	char storage[1000];
};

typedef struct charStack TCStack;

char cStackPop(TCStack* stack) {
	if (stack->elementCount != 0) {
		--(stack->elementCount);
		return stack->storage[stack->elementCount];
	}
	else {
		printf("syntax error");
		exit(0);
	}
}

char cStackPeek(const TCStack* stack) {
	if (stack->elementCount != 0) {
		return stack->storage[stack->elementCount - 1];
	}
	else {
		return '\0';
	}
}

int cStackPush(TCStack* stack, char toPush) {
	if (stack->elementCount > 999) {
		return 0;
	}
	else {
		stack->storage[stack->elementCount] = toPush;
		++(stack->elementCount);
		return 1;
	}
}

struct intStack {
	int elementCount;
	int storage[1000];
};

typedef struct intStack TIStack;

int iStackPop(TIStack* stack) {
	if (stack->elementCount != 0) {
		--(stack->elementCount);
		return stack->storage[stack->elementCount];
	}
	else {
		printf("syntax error");
		exit(0);
	}
}

int iStackPeek(const TIStack* stack) {
	if (stack->elementCount != 0) {
		return stack->storage[stack->elementCount - 1];
	}
	else {
		return 0;
	}
}

int iStackPush(TIStack* stack, int toPush) {
	if (stack->elementCount > 999) {
		return 0;
	}
	else {
		stack->storage[stack->elementCount] = toPush;
		++(stack->elementCount);
		return 1;
	}
}

int isNumber(char toCheck) {
	if (toCheck >= '0' && toCheck <= '9') {
		return 1;
	}
	else {
		return 0;
	}
}

void exitWithSyntaxError() {
	printf("syntax error");
	exit(0);
}

void parse(char* toParse, char* result) {
	int resultIndex = 0;
	TCStack charStack;
	charStack.elementCount = 0;
	int operExpected = 0;

	for (int inpIndex = 0;;) {
		if (!operExpected) {
			if (isNumber(toParse[inpIndex])) {
				for (; isNumber(toParse[inpIndex]); ++inpIndex, ++resultIndex) {
					result[resultIndex] = toParse[inpIndex];
				}
				result[resultIndex++] = ' ';
				operExpected = 1;
			}
			else if (toParse[inpIndex] == '(') {
				cStackPush(&charStack, '(');
				++inpIndex;
			}
			else exitWithSyntaxError();
		}
		else {
			switch (toParse[inpIndex])
			{
			case '+':
			case '-':
				for (; cStackPeek(&charStack) != '(' && cStackPeek(&charStack) != '\0';) {
					result[resultIndex++] = cStackPop(&charStack);
					result[resultIndex++] = ' ';
				}
				cStackPush(&charStack, toParse[inpIndex]);
				operExpected = 0;
				break;
			case '/':
			case '*':
				for (; cStackPeek(&charStack) == '/' || cStackPeek(&charStack) == '*';) {
					result[resultIndex++] = cStackPop(&charStack);
					result[resultIndex++] = ' ';
				}
				cStackPush(&charStack, toParse[inpIndex]);
				operExpected = 0;
				break;
			case ')':
				for (; cStackPeek(&charStack) != '(';) {
					result[resultIndex++] = cStackPop(&charStack);
					result[resultIndex++] = ' ';
				}
				cStackPop(&charStack);
				break;
			case '\0':
				for (; cStackPeek(&charStack) != '\0';) {
					if (cStackPeek(&charStack) == '(') {
						exitWithSyntaxError();
					}
					result[resultIndex++] = cStackPop(&charStack);
					result[resultIndex++] = ' ';
				}
				result[resultIndex] = '\0';
				return;
			default:
				exitWithSyntaxError();
			}
			++inpIndex;
		}
	}
}

int calculate(char* parsedString) {
	TIStack intStack;
	intStack.elementCount = 0;
	for (int index = 0; parsedString[index] != '\0'; ++index) {
		if (isNumber(parsedString[index])) {
			iStackPush(&intStack, atoi(parsedString + index));
			for (; parsedString[index] != ' '; ++index);
		}
		else {
			int rvalue = iStackPop(&intStack);
			int lvalue = iStackPop(&intStack);
			switch (parsedString[index]) {
			case '+':
				iStackPush(&intStack, lvalue + rvalue);
				break;
			case '-':
				iStackPush(&intStack, lvalue - rvalue);
				break;
			case '*':
				iStackPush(&intStack, lvalue * rvalue);
				break;
			case '/':
				if (rvalue == 0) {
					printf("division by zero");
					exit(0);
				}
				iStackPush(&intStack, lvalue / rvalue);
				break;
			}
			++index;
		}
	}
	return iStackPop(&intStack);
}

int main() {
	char parsed[2000];
	char input[1001];
	gets_s(input);
	parse(input, parsed);
	//printf("%s\n", parsed);
	int result = calculate(parsed);
	printf("%d", result);
	return 0;
}