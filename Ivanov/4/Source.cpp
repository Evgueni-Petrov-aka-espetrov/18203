#include <stdio.h>
#include <stdlib.h>

struct charStack{
	int elementCount;
	char storage[1000];
};

typedef struct charStack TCStack;

char cStackPop(TCStack* stack){
	if (stack->elementCount != 0){
		--(stack->elementCount);
		return stack->storage[stack->elementCount];
	}
	else{
		printf("syntax error");
		exit(0);
	}
}

char cStackPeek(TCStack* stack){
	if (stack->elementCount != 0){
		return stack->storage[stack->elementCount-1];
	}
	else{
		return '\0';
	}
}

int cStackPush(TCStack* stack, char toPush){
	if (stack->elementCount > 999){
		return 0;
	}
	else{
		stack->storage[stack->elementCount] = toPush;
		++(stack->elementCount);
		return 1;
	}
}

struct intStack{
	int elementCount;
	int storage[1000];
};

typedef struct intStack TIStack;

int iStackPop(TIStack* stack){
	if (stack->elementCount != 0){
		--(stack->elementCount);
		return stack->storage[stack->elementCount];
	}
	else{
		printf("syntax error");
		exit(0);
	}
}

int iStackPeek(TIStack* stack){
	if (stack->elementCount != 0){
		return stack->storage[stack->elementCount - 1];
	}
	else{
		return 0;
	}
}

int iStackPush(TIStack* stack, int toPush){
	if (stack->elementCount > 999){
		return 0;
	}
	else{
		stack->storage[stack->elementCount] = toPush;
		++(stack->elementCount);
		return 1;
	}
}

int isNumber(char toCheck){
	if (toCheck >= '0' && toCheck <= '9'){
		return 1;
	}
	else{
		return 0;
	}
}

void parse(char* toParse, char* result){
	int resultIndex = 0;
	TCStack charStack;
	charStack.elementCount = 0;

	for (int inpIndex = 0;;){
		if (isNumber(toParse[inpIndex])){
			for (; isNumber(toParse[inpIndex]); ++inpIndex, ++resultIndex){
				result[resultIndex] = toParse[inpIndex];
			}
			result[resultIndex++] = ' ';
		}
		else{
			switch (toParse[inpIndex])
			{
			case '+':
			case '-':
				for (char stackTop = cStackPeek(&charStack); stackTop != '(' && stackTop != '\0'; stackTop = cStackPeek(&charStack)){
					result[resultIndex++] = cStackPop(&charStack);
					result[resultIndex++] = ' ';
				}
				cStackPush(&charStack, toParse[inpIndex]);
				break;
			case '/':
			case '*':
				for (char stackTop = cStackPeek(&charStack); stackTop == '/' || stackTop == '*'; stackTop = cStackPeek(&charStack)){
					result[resultIndex++] = cStackPop(&charStack);
					result[resultIndex++] = ' ';
				}
				cStackPush(&charStack, toParse[inpIndex]);
				break;
			case '(':
				cStackPush(&charStack, '(');
				break;
			case ')':
				for (char stackTop = cStackPeek(&charStack); stackTop != '('; stackTop = cStackPeek(&charStack)){
					result[resultIndex++] = cStackPop(&charStack);
					result[resultIndex++] = ' ';
				}
				cStackPop(&charStack);
				break;
			case '\0':
				for (char stackTop = cStackPeek(&charStack); stackTop != '\0'; stackTop = cStackPeek(&charStack)){
					result[resultIndex++] = cStackPop(&charStack);
					result[resultIndex++] = ' ';
				}
				result[resultIndex] = '\0';
				return;
				break;
			default:
				printf("syntax error");
				exit(0);
			}
			++inpIndex;
		}
	}
}

int calculate(char* parsedString){
	TIStack intStack;
	intStack.elementCount = 0;
	for (int index = 0; parsedString[index] != '\0'; ++index){
		if (isNumber(parsedString[index])){
			iStackPush(&intStack, atoi(parsedString + index));
			for (; parsedString[index] != ' '; ++index);
		}
		else{
			int rvalue = iStackPop(&intStack);
			int lvalue = iStackPop(&intStack);
			switch (parsedString[index]){
			case '+':
				iStackPush(&intStack, lvalue + rvalue);
				break;
			case '-':
				iStackPush(&intStack, lvalue - rvalue);
				break;
			case '*':
				iStackPush(&intStack, lvalue + rvalue);
				break;
			case '/':
				if (rvalue == 0){
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

int checkInput(char* input){
	int brCount = 0;
	for (int index = 0; input[index] != '\0'; ++index){
		if (input[index] == '('){
			++brCount;
		}
		if (input[index] == ')'){
			--brCount;
			if (brCount < 0){
				return 0;
			}
		}
	}
	if (brCount == 0){
		return 1;
	}
	else{
		return 0;
	}
}

int main(){
	char parsed[2000];
	char input[1000];
	scanf("%s", input);
	if (!checkInput(input)){
		printf("syntax error");
		return 0;
	}
	parse(input, parsed);
	//printf("%s\n", parsed);
	int result = calculate(parsed);
	printf("%d", result);
	return 0;
}