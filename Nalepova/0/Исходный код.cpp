#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string>

int CharToDec(char s[], short int i)
{
	if (s[i] >= '0' && s[i] <= '9')
		return s[i] - '0';
	else if (s[i] >= 'A' && s[i] <= 'F')
		return s[i] - 'A' + 10;
	else if (s[i] >= 'a' && s[i] <= 'f')
		return s[i] - 'a' + 10;
	else
	{
		printf("Bad input");
		exit(0);
	}
}
int CheckSystem(int b1, int b2)
{
	if (b1 > 16 || b1 < 2 || b2 > 16 || b2 < 2)
		return 0;
	else return 1;
}
int main() {
	int b1, b2;
	scanf("%d%d", &b1, &b2);
	if (CheckSystem(b1, b2) == 0)
	{
		printf("Bad input");
		exit(0);
	}

	const int N = 52;
	char number[N];
	scanf("%s", number);
	if (b1 == b2)
	{
		printf("%s", number);
		exit(0);
	}

	int dot = -1;
	for (int i = 0; i < strlen(number); i++)
	if (number[i] == '.')
		dot = i;

	int intPart = 0;
	if (dot == -1)
	{
		dot = strlen(number);
	}

	for (int i = 0; i < dot; i++)
	{
		intPart = intPart + (CharToDec(number, i) * pow(b1, dot - i - 1));
	}

	double fracPart = 0;
	for (int i = dot + 1; i < strlen(number); i++)
	{
		if ((dot + 1) >= strlen(number))
			break;
		fracPart = fracPart + (CharToDec(number, i) * pow(b1, dot - i));
	}

	char symbols[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	for (int i = 0; i < N; i++)
	{
		number[i] = symbols[intPart % b2];
		intPart = intPart / b2;
		if (intPart == 0)
		{
			number[i + 1] = '.';
			dot = i + 1;
			break;
		}
	}
	for (int i = dot + 1; i < N; i++)
	{
		number[i] = symbols[(int)(fracPart * b2)];
		fracPart = (double)(fracPart * b2) - (int)(fracPart * b2);
		if (fracPart == 0)
		{
			number[i + 1] = '\0';
			break;
		}
	}
	
	for (int i = dot - 1; i >= 0; i--)
		printf("%c", number[i]);
	for (int i = dot; i < strlen(number); i++)
		printf("%c", number[i]);
	scanf("%s", number);//
	return 0;
}