#include <stdio.h>
#include <string.h>

void RK(unsigned char needle[], unsigned char hay[], int needlelength, FILE *fin, FILE *fout);

int main()
{
	FILE* fout = fopen("out.txt", "w");
	FILE* fin = fopen("in.txt", "r");
	if (fin == NULL)
		fprintf(fout, "Error");
	else
	{
		unsigned char symbol, needle[16], haystack[16];
		int i = 0;
		while ((symbol = getc(fin)) != '\n')
		{
			needle[i] = symbol;
			i++;
		}
		int needlelength = i;
		RK (needle, haystack, needlelength, fin, fout);
	}
	fclose(fin);
	fclose(fout);
	return 0;

}

int pow3(int n)
{
	 int pow3[16] = { 0 };
    if (!pow3[0])
    {
        pow3[0] = 1;
        for (int i = 1; i < 16; i++)
        {
           pow3[i] = 3*pow3[i - 1];
        }
    }
    return pow3[n];
}

void printarr (int start, int length, FILE *fout)
 {
	int i;
	for (i = start; i < start + length; i++)
        fprintf(fout, "%d ", i + 1);
}

int startread(unsigned char hay[], int needlelength, FILE *fin)
{
	int i, position(0), check(0);
	char symbol;
	for (i = 0; i < needlelength; i++)
	{
		symbol = getc(fin);
		if (symbol == EOF)
		{
		    i = needlelength + 1;
			check++;
		}
		else
		{
			hay[position] = symbol;
			position++;
		}
	}
	if (check > 0)
        return 1;
	else
        return 0;
}

int nextread(unsigned char hay[], int needlelength, FILE *fin)
{
	char symbol;
	int i, check(0);
	for (i = 1; i < needlelength; i++)
	{
		hay[i - 1] = hay[i];
	}
	symbol = getc(fin);
	if (symbol == EOF)
		check++;
	else
		hay[needlelength - 1] = symbol;
	if (check > 0)
        return 1;
	else
        return 0;
}

int counthash(unsigned char array[], int needlelength)
{
	int i, result(0);
	for (i = 0; i <= needlelength - 1; i++)
	{
		result = result + ((array[i] % 3) * pow3(i));
	}
	return result;
}

void compare(unsigned char needle[], unsigned char hay[], int start, int needlelength, int hash, float hayhash, FILE *fout)
{
	if (hash == hayhash)
	{
		int i = 0;
		do
		{
			fprintf(fout, "%d ", i + 1 + start);
			i++;
		}
		while ((i <= needlelength - 1) && (needle[i-1] == hay[i-1]));
	}
}
void RK(unsigned char needle[], unsigned char hay[], int needlelength, FILE *fin, FILE *fout)
{
	int start(0);
	int hash = counthash(needle, needlelength);
	float phash;
	fprintf(fout, "%d ", hash);
	int readresult = startread(hay, needlelength, fin);
	float hayhash = counthash(hay, needlelength);
	while (readresult != 1)
	{
		compare(needle, hay, start, needlelength, hash, hayhash, fout);
		start ++;
		phash = (hayhash - hay[0] % 3) / 3;
		readresult = nextread(hay, needlelength, fin);
		hayhash = phash + (hay[needlelength - 1] % 3) * pow3(needlelength - 1);
	}
}



