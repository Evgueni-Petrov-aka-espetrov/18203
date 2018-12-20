#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string>

void Swap(char *a, char *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

void MakePerestanovka(char *perestanovka, int size)
{
	int index = size - 2;

	while (index != -1 && perestanovka[index] > perestanovka[index + 1])
		index--;
	if (index == -1)
		exit(0);
	
	int pos = index;
	index = size - 1;
	while (perestanovka[pos] > perestanovka[index])
		index--;
	Swap(&perestanovka[pos], &perestanovka[index]);

	int l = pos + 1;
	int r = size - 1;
	while (l < r)
	{
		Swap(&perestanovka[l], &perestanovka[r]);
		l++;
		r--;
	}
}


void main()
{
	const int N = 11;
	char perestanovka[N];
	int amountOfPerestanovok;
	scanf("%s", &perestanovka);
	scanf("%d", &amountOfPerestanovok);

	int sizePerestanovki = strlen(perestanovka);

	for (int i = 0; i < sizePerestanovki; i++)
	{
		if (perestanovka[i] > '9' || perestanovka[i] < '0')
		{
			printf("Bad input");
			exit(0);
		}
	}
	for (int i = 0; i < sizePerestanovki; i++)
	{
		for(int j = i + 1; j < sizePerestanovki; j++)
		{
			if (perestanovka[i] == perestanovka[j])
			{
				printf("Bad input");
				exit(0);
			}
			
		}
	}

	for (int i = 0; i < amountOfPerestanovok; i++)
	{
		MakePerestanovka(perestanovka, sizePerestanovki);
		printf("%s\n", perestanovka);
	}
}
