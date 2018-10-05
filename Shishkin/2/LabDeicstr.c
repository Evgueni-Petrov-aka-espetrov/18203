#include <stdio.h>
#include <string.h>
int read (FILE *fin,char str[]) {
	int moves;
	fgets (str, 20, fin);						//чтение цифр из файла и запись в строку
	fscanf(fin, "%d", &moves);
	return moves;
}
	
int check (char symbols[], int num) {                              //проверка прочтенных цифр(return correct=0 or incorrect=1)
	int i, errornum, j;
	errornum=0;
	for (i=0; i < num-1; i++){
		for (j=0; j<num-1; j++)                
			if ((symbols[i] == symbols[j]) && (i != j)) errornum++; 
	}
	for (i=0; i<num-1; i++) {
		if ((symbols[i]-48) >= 10) errornum++;
		if ((symbols[i]-48) < 0) errornum++;
	}
	if (errornum != 0) return 1;
	else return 0;	
}

void swap (char symbols[], int n, int m) {
	char midchar;
	midchar = symbols[n];
	symbols[n] = symbols[m];
	symbols[m] = midchar;
}

void bigswap (char symbols[], int num) {
	int j, i, z, k;
	char c1, c2;
	c1 = 0;
	i = num-2;
	j = num-2;
	while (symbols[j] < symbols[j-1]) j--;
	while (symbols[i] < symbols[j-1]) i--;                                    //создает 1 новую(следующую) перестановку 
	swap (symbols, j-1, i);
	z = (num-j-2)/2;
	for (k=0;  k<= z; k++) {
		swap(symbols, j+k, num-2-k);
	}
}

int main () {
	FILE *fin, *fout;
	char symbols[20];
	int  num,moves,i, j, swapvar;
	fout = fopen("out.txt", "w");
	if ((fin=fopen("in.txt", "r")) == NULL) 
		printf("File could not be opened.");
	else {
		moves=read (fin, symbols);
		num = strlen(symbols);
		if (check(symbols,num) == 1) fprintf(fout, "bad input");
		else {
			for (i=0; i<moves; i++) {
				swapvar = 0;
				for (j = 0; j<num-1; j++) {
					if (symbols[j] < symbols[j+1]) swapvar++;
				}
				if (swapvar > 0) {
						bigswap(symbols, num);
						fprintf(fout,"%s", symbols);
				}
			} 
		}
	fclose(fin);
	fclose(fout);
	}
	return 0; 
}

