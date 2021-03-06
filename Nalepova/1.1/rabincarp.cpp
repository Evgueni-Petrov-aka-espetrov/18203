#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

int Hash(unsigned char c, int lastStep)
{
	static int degree = 1;
	int res = (c % 3) * degree;
	degree *= 3;
	if (lastStep == 1)
	{
		degree = 1;
	}
	return res;
}

int wordHash(unsigned char* word, int lengthWord)
{
	int sum = 0;
	for (int i = 0; i < lengthWord; i++) {
		sum += Hash(word[i], lengthWord - i);
	}
	return sum;
}

int Check(unsigned char* word, unsigned char* string, const int lengthWord, int fromPosition)
{

	for (int i = 0; i < lengthWord; ++i)
	{
		if (word[i] == string[i])
		{
			printf("%d ", fromPosition + i + 1);
		}
		else
		{

			printf("%d ", fromPosition + i + 1);

			return 0;
		}
	}
	return 1;
}

int Pow(int a, int b)
{
	int result = 1;
	for (int i = 0; i < b; i++)
	{
		result *= a;
	}
	return result;
}

int main()
{

	const int maxlengthPattern = 16;
	
	unsigned char pattern[maxlengthPattern + 1];
	unsigned char string[maxlengthPattern + 1];
	char ch;
	int d = 0;
	do {
		ch = getchar();

		pattern[d] = ch;
		d++;
	} while (ch != '\n');
	d--;
	pattern[d] = '\0';
	int lengthPattern = d;

	int hashOfWord = wordHash(pattern, lengthPattern);

	
	for (int j = 0; j < lengthPattern; j++)
	{
		ch = getchar();
		if (ch == EOF)
		{
			printf("0\n");
			return 0;
		}
		string[j] = ch;
		string[j + 1] = '\0';
	}

	printf("%d ", hashOfWord);


	int hashOfString = wordHash(string, lengthPattern);
	int k = 0;

	int factor = Pow(3, lengthPattern - 1);
	while (1)
	{
		if (hashOfWord == hashOfString)
		{
			Check(pattern, string, lengthPattern, k);

		}
		char c = getchar();
		if (c == EOF)
		{
			break;
		}
		hashOfString = ((hashOfString - string[0] % 3) / 3 + ((unsigned char)c % 3) * factor);
		for (int i = 0; i < lengthPattern - 1; i++)
		{
			string[i] = string[i + 1];
		}
		string[lengthPattern - 1] = c;

		k++;
	}

	return 0;
}
