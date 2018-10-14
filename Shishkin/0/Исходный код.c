#include <stdio.h>
#include <string.h>
void CharToB1(char sIN[], float nIN[], int amount) {
	int i;
	for (i = 0; i<amount; i++) {
		if (((sIN[i] - 48) < 10) && ((sIN[i] - 48) >= 0))  nIN[i] = sIN[i] - 46;
		if (((sIN[i] - 65) < 6) && ((sIN[i] - 65) >= 0))  nIN[i] = sIN[i] - 53;              //Ã§Ã Ã¯Ã¨Ã±Ã¼ Ã¨Ã§ Ã Ã±ÃªÃ¨ÃªÃ®Ã¤Ã 
		if (((sIN[i] - 97) < 6) && ((sIN[i] - 97) >= 0))  nIN[i] = sIN[i] - 85;
		if (sIN[i] == 46) nIN[i] = 1;
	}
}
void fallOn2(float nIN[], int amount) {
	int i;													//Ã¢Ã»Ã·Ã¨Ã²Ã Ã¥Ã² Ã¨Ã§ ÃªÃ Ã¦Ã¤Ã®Ã£Ã® Ã½Ã«Ã¥Ã¬Ã¥Ã­Ã²Ã  Ã¬Ã Ã±Ã±Ã¨Ã¢Ã  2
	for (i = 0; i<amount; i++)	nIN[i] = nIN[i] - 2;
}
int check(float nIN[], int b1, int b2, int amount) {
	int i, erroramount;
	erroramount = 0;
	for (i = 0; i<amount; i++) {
		if (nIN[i] == 0) erroramount++;                      //Ã¿Ã¢Ã«Ã¿Ã¥Ã²Ã±Ã¿ Ã«Ã¨ Ã·Ã¨Ã±Ã«Ã® Ã¡1Ã°Ã¨Ã·Ã­Ã®Ã© Ã§Ã Ã¯Ã¨Ã±Ã¼Ã¾ Ã¨ Ã¢Ã±Ã¥ Ã«Ã¨ ÃªÃ®Ã½Ã´-Ã²Ã» Ã¯Ã¥Ã°Ã¥Ã¢Ã¥Ã«Ã¨Ã±Ã¼
		if (nIN[i]>b1 + 1) erroramount++;
	}
	if (b1<2) erroramount++;
	if (b1>16) erroramount++;                              //Ã«Ã¥Ã¦Ã Ã² Ã«Ã¨ Ã¡1 Ã¨ Ã¡2 Ã¢ Ã¯Ã°Ã¥Ã¤Ã¥Ã«Ã Ãµ Ã®Ã² 2 Ã¤Ã® 16
	if (b2<2) erroramount++;
	if (b2>16) erroramount++;
	if (erroramount != 0) return 1;
	else return 0;
}
long long int powerint(int a, int n) {
	int i;
	long long int t;
	t = a;									//Ã¢Ã®Ã§Ã¢Ã®Ã¤Ã¨Ã² Ã·Ã¨Ã±Ã«Ã® Ã  Ã¢ Ã±Ã²Ã¥Ã¯Ã¥Ã­Ã¼ Ã¯
	for (i = 1; i<n; i++) {
		t = t*a;
	}
	return t;
}
float powerfloat(int a, int n) {
	int i;
	float t;
	t = a;									//Ã¢Ã®Ã§Ã¢Ã®Ã¤Ã¨Ã² Ã·Ã¨Ã±Ã«Ã® Ã  Ã¢ Ã±Ã²Ã¥Ã¯Ã¥Ã­Ã¼ Ã¯
	for (i = 1; i<n; i++) {
		t = t*a;
	}
	return t;
}
int pointCheck(int pointplace, int amount, int pointcount) {
	int result;
	if (pointplace == amount - 1) {
		result = -1;                      									//Ã²Ã®Ã·ÃªÃ  Ã­Ã  Ã¯Ã®Ã±Ã«Ã¥Ã¤Ã­Ã¥Ã¬ Ã¬Ã¥Ã±Ã²Ã¥
	}
	if (pointplace == 0) {
		result = -1;                  										//Ã²Ã®Ã·ÃªÃ  Ã­Ã  Ã¯Ã¥Ã°Ã¢Ã®Ã¬ (Ã­Ã³Ã«Ã¥Ã¢Ã®Ã¬) Ã¬Ã¥Ã±Ã²Ã¥
	}
	if (pointplace == amount + 1) result = 1;								//Ã­Ã¥Ã² Ã²Ã®Ã·ÃªÃ¨
	if ((pointplace < amount - 1) && (pointplace > 0)) result = 0;			//Ã²Ã®Ã·ÃªÃ  Ã­Ã¥ Ã¯Ã® ÃªÃ°Ã Ã¿Ã¬
	if (pointcount > 1) result = -1;										//Ã¡Ã®Ã«Ã¼Ã¸Ã¥ 1 Ã²Ã®Ã·ÃªÃ¨
	return result;
}
long long int fullPartB1ToTen(float nIN[], int b1, int place){
	int i;
	long long int number2, partnum2;
	number2 = nIN[place - 1];
	for (i = place - 2; i >= 0; i--) {
		partnum2 = nIN[i] * powerint(b1, place - 1 - i);
		number2 = number2 + partnum2;
	}
	return number2;
}
void drobPartB1ToTen(double number1[], int start, int finish, int b1, float nIN[]) {
	int i;
	double partnum;
	for (i = start + 1; i<finish; i++) {
		partnum = nIN[i] / powerfloat(b1, i - start);
		number1[0] = number1[0] + partnum;
	}
}
long long int B1ToTen(float nIN[], double number1[], int b1, int amount) {
	int pointplace, i, pointcount, result;
	pointplace = amount + 1;
	pointcount = 0;
	long long int number2;
	for (i = 0; i<amount; i++) {
		if (nIN[i] == -1) {
			pointplace = i;
			pointcount++;
		}
	}
	result = pointCheck(pointplace, amount, pointcount);
	if (result == 1) {
		number2 = fullPartB1ToTen(nIN, b1, amount);
		number1[0] = -2;
	}
	if (result == 0) {
		number2 = fullPartB1ToTen(nIN, b1, pointplace);
		drobPartB1ToTen(number1, pointplace, amount, b1, nIN);
	}
	if (result == -1) number2 = result;
	
	return number2;
	
}
void reverseArray(int array[], int amount) {
	int i, t;
	for (i = 0; i <= amount / 2; i++) {                                     //Ã¬Ã¥Ã­Ã¿Ã¥Ã² Ã¬Ã¥Ã±Ã²Ã Ã¬Ã¨ Ã¯Ã® 2 Ã½Ã«Ã¥Ã¬Ã¥Ã­Ã²Ã  Ã¬Ã Ã±Ã±Ã¨Ã¢Ã , Ã­Ã Ã·Ã¨Ã­Ã Ã¿ Ã± ÃªÃ°Ã Ã¥Ã¢
		t = array[i];
		array[i] = array[amount - i];
		array[amount - i] = t;
	}
}
void TenToB2(int nOUT[], double number1[], long long int number2, int b2) {
	long long int i, left, quot, pointplace;
	i = 0;
	double number11;
	number11 = number1[0];
	while (number2 >= b2) {
		quot = number2 / b2;
		left = number2 - quot*b2;
		nOUT[i] = left;                     //Ã¯Ã¥Ã°Ã¥Ã¢Ã®Ã¤ Ã¶Ã¥Ã«Ã®Ã© Ã·Ã Ã±Ã²Ã¨ Ã·Ã¨Ã±Ã«Ã 
		i++;
		number2 = quot;
	}
	nOUT[i] = number2;
	reverseArray(nOUT, i);
	if (number11 != -2) {
		i++;
		nOUT[i] = -1;
		pointplace = i;
		do {                                                                 //Ã¯Ã¥Ã°Ã¥Ã¢Ã®Ã¤ Ã¤Ã°Ã®Ã¡Ã­Ã®Ã© Ã·Ã Ã±Ã²Ã¨ Ã·Ã¨Ã±Ã«Ã 
			i++;
			number11 = number11*b2;
			nOUT[i] = number11;
			number11 = number11 - nOUT[i];
		} while (number11 > 0);
	}
	if ((i - pointplace) > 12) nOUT[pointplace + 13] = -2;
	else nOUT[i + 1] = -2;
}
void B2ToChar(int nOUT[], char sOUT[], FILE *fout) {
	int i;
	i = 0;
	while (nOUT[i] != -2) {                                                         //Ã§Ã Ã¯Ã¨Ã±Ã¼ Ã¢ Ã Ã±ÃªÃ¨ÃªÃ®Ã¤ + Ã¢Ã»Ã¢Ã®Ã¤
		if (nOUT[i] < 10) sOUT[i] = nOUT[i] + 48;
		if (nOUT[i] >= 10) sOUT[i] = nOUT[i] + 55;
		if (nOUT[i] == -1) sOUT[i] = nOUT[i] + 47;
		fprintf(fout, "%c", sOUT[i]);
		i++;
	}
}
int main() {
	FILE *fin, *fout;
	char symbolsIN[13];
	char symbolsOUT[1000];
	float numberIN[13];
	int numberOUT[1000];
	double drobnayachast[1] = { 0 };
	long long int celayachast;
	int  b1, b2, amount;
	fout = fopen("out.txt", "w");
	fin = fopen("in.txt", "r");
	if (fin == NULL)
		fprintf(fout, "File could not be opened.");
	else {
		fscanf(fin, "%d", &b1);
		fscanf(fin, "%d", &b2);							//Ã±Ã·Ã¨Ã²Ã»Ã¢Ã Ã¥Ã² Ã¤Ã Ã­Ã­Ã»Ã¥ Ã¨Ã§ Ã´Ã Ã©Ã«Ã 			
		fscanf(fin, "%s", symbolsIN);
		amount = strlen(symbolsIN);
		CharToB1(symbolsIN, numberIN, amount);
		if (check(numberIN, b1, b2, amount) == 1) fprintf(fout, "bad input");
		else {
			fallOn2(numberIN, amount);
			celayachast = B1ToTen(numberIN, drobnayachast, b1, amount);
			if (celayachast == -1) fprintf(fout, "bad input");
			else{
				TenToB2(numberOUT, drobnayachast, celayachast, b2);
				B2ToChar(numberOUT, symbolsOUT, fout);
			}
		}
	}
	fclose(fin);
	fclose(fout);
	return 0;
}
