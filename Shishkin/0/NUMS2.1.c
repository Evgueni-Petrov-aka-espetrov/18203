#include <stdio.h>
int amountOfNum (char b[]) {
	int i, j, x;
	i=0;
	while (b[i]!='\0')	i++;                                        //выводит количество чисел в строке  
	return i;
}

int convcheck (char s[], float n[], int b, int c, int am) {
	int i, z, a;
	z = 0;
	for (i=0; i<am; i++) {
		if (((s[i]-48) < 10) && ((s[i]-48) >= 0))  n[i] = s[i] - 46;
		if (((s[i]-65) < 6) && ((s[i]-65) >= 0))  n[i] = s[i] - 53;              //запись из аскикода
		if (((s[i]-97) < 6) && ((s[i]-97) >= 0))  n[i] = s[i] - 85;
		if (s[i] == 46) n[i] = 1;
		}
	for (i=0; i<am; i++) {
		if (n[i] == 0) z++;                      //является ли число б1ричной записью и все ли коэф-ты перевелись
		if (n[i]>b+1) z++;
		}
	for (i=0; i<am; i++)	n[i] = n[i] - 2;
	if (b<2) z++;
	if (b>16) z++;                              //лежат ли б1 и б2 в пределах от 2 до 16
	if (c<2) z++;
	if (c>16) z++;
	if (z != 0) return 1;
		else return 0;
}

long long int step (int a, int n) {
	int i, k;
	long long int z;	
	z=a;
	for (i=1; i<n; i++) {
		z = z*a;
		}
	return z;
}

long long int conv1 (float n[], double m1[], int b, int am) {
	int x, i, j;
	x = 20;
	j = 0;
	double k;
	long long int m2, h;
	for	(i=0; i<am; i++) {
		if (n[i] == -1) {
			x = i;
			j++;
			}
		}
	if (x == 20) {
		m2 = n[am-1];                                           //если точке нет вообще
		for (i=am-2; i>=0; i--) {
			h = n[i]*step(b, am - 1 - i);
			m2 = m2 + h;
			}
		m1[0] = -2;
		}
	if (x == am-1) {
		m2 = 90;                      //точка на последнем месте
		}
	if (x == 0) {
		m2 = 90;                  //точка на первом (нулевом) месте
		}
	if ((x < am - 1) && (x > 0)) {
		m2 = n[x-1];
		for (i=x-2; i>=0; i--) {
			h = n[i] * step(b, x-1-i);
			m2 = m2 + h;                        //точка не по краям
			}
		for (i=x+1; i<am; i++) {
			k = n[i]/step(b, i-x);
			m1[0] = m1[0] + k;
			}
		}
	if (j > 1) m2 = 90;                                        //если больше 1 точки
	return m2;
	printf("%lli ", m2);	
}

void swaparr (int n[], int s) {
	int i, x;
	for (i=0; i <= s/2; i++) {                                     //меняет местами по 2 элемента массива, начиная с краев
		x = n[i];
		n[i] = n[s-i];
		n[s-i] = x;
		}
}

void conv2 (int n[], double m1[], long long int m2, int b) {
	long long int i, l, ch, k, x;
	i = 0;
	double z1, z2;
	z1 = m1[0];
	z2 = m1[0];
	while (m2 >= b) {
		ch = m2/b;
		l = m2 - ch*b;
		n[i] = l;                     //перевод целой части числа
		i++;
		m2 = ch;
		}
	n[i] = m2;
	swaparr(n, i);
	if (z1 != -2) {
		i++;
		n[i] = -1;
		x = i;
		k = 0;
		do {                                                                 //перевод дробной части числа
			i++;
			k++;
			z1 = z1*b;
			n[i] = z1;
			z1 = z1 - n[i];
			} while (z1 > 0);
		}
	if ((i-x) > 12) n[x+13] = -2;
	else n[i+1] = -2;
}

void conv3 (int n[], char s[], FILE *f) {
	int i;
	i = 0;
	while (n[i] != -2) {                                                         //запись в аскикод + вывод
		if (n[i] < 10) s[i] = n[i] + 48;
		if (n[i] >= 10) s[i] = n[i] + 55;
		if (n[i] == -1) s[i] = n[i] + 47; 
		fprintf(f, "%c", s[i]);
		i++;
		}
}

int main () {
	FILE *fin, *fout;
	char symbolsIN[13];
	char symbolsOUT[1000];
	float numberIN[13];
	int numberOUT[1000];
	double number1[0];
	long long int number2;
	int  b1, b2, amount, i;
	fout = fopen("out.txt", "w");
	fin=fopen("in.txt", "r");
	if (fin == NULL) 
		fprintf(fout, "File could not be opened.");
	else {
    	fscanf(fin, "%d", &b1);
		fscanf(fin, "%d", &b2);							//считывает данные из файла			
		fscanf(fin, "%s", symbolsIN);
		amount= amountOfNum(symbolsIN);
		
    	if (convcheck(symbolsIN, numberIN, b1, b2, amount) == 1) fprintf (fout, "bad input");
    	else { 
			number2 = conv1(numberIN, number1, b1, amount);
			if (number2 == 90) fprintf(fout, "bad input");
			else{
				conv2(numberOUT, number1, number2, b2);
				conv3(numberOUT, symbolsOUT, fout);
				}
			}
		}
	fclose(fin);
	fclose(fout);
	return 0; 
}
