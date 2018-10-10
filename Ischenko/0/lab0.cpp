#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <cmath>
#include <fstream>

using namespace std;

bool proverka(int s1, int s2, char ch1[]) {
	bool r = false;
	if (s1 > 16 || s1 < 2 || s2>16 || s2 < 2) {
		r = true;

	}

	for (int i = 0; i < strlen(ch1); ++i) {
		switch (ch1[i]) {

		case '.': if ((i + 1) == strlen(ch1) || i == 0 || ch1[i + 1] == '.') { r = true; } break;
		case '2': if (s1 <= 2) { r = true; }break;
		case '3': if (s1 <= 3) { r = true; }break;
		case '4': if (s1 <= 4) { r = true; }break;
		case '5': if (s1 <= 5) { r = true; }break;
		case '6': if (s1 <= 6) { r = true; }break;
		case '7': if (s1 <= 7) { r = true; }break;
		case '8': if (s1 <= 8) { r = true; }break;
		case '9': if (s1 <= 9) { r = true; }break;
		case 'a':if (s1 <= 10) { r = true; }break;
		case 'b':if (s1 <= 11) { r = true; }break;
		case 'c':if (s1 <= 12) { r = true; }break;
		case 'd':if (s1 <= 13) { r = true; }break;
		case 'e':if (s1 <= 14) { r = true; }break;
		case 'f':if (s1 <= 15) { r = true; }break;
		}
	}
	return r;
}
char perevod(int a) {
	char cifra = ' ';
	switch (a) {
	case 0:case 1:case 2:case 3:case 4:
	case 5:case 6:case 7:case 8:case 9:
		cifra = a + 48; break;
	case 10:cifra = 'a'; break;
	case 11:cifra = 'b'; break;
	case 12:cifra = 'c'; break;
	case 13:cifra = 'd'; break;
	case 14:cifra = 'e'; break;
	case 15:cifra = 'f'; break;
	}
	return cifra;
}
void obr_10(char ch1[], int s1, long double *p1) {
	int n1 = strcspn(ch1, "."), n2 = strlen(ch1);
	for (int i = 0; i < n1; ++i) {
		char a = ch1[i];
		switch (a) {
		case '0':case '1':case '2':case '3':case '4':
		case '5':case '6':case '7':case '8':case '9':
			*p1 += ((int)a - 48)*pow(s1, n1 - i - 1);
			break;
		case 'a':*p1 += 10 * pow(s1, n1 - i - 1); break;
		case 'b':*p1 += 11 * pow(s1, n1 - i - 1); break;
		case 'c':*p1 += 12 * pow(s1, n1 - i - 1); break;
		case 'd':*p1 += 13 * pow(s1, n1 - i - 1); break;
		case 'e':*p1 += 14 * pow(s1, n1 - i - 1); break;
		case 'f':*p1 += 15 * pow(s1, n1 - i - 1); break;
		case 'A':*p1 += 10 * pow(s1, n1 - i - 1); break;
		case 'B':*p1 += 11 * pow(s1, n1 - i - 1); break;
		case 'C':*p1 += 12 * pow(s1, n1 - i - 1); break;
		case 'D':*p1 += 13 * pow(s1, n1 - i - 1); break;
		case 'E':*p1 += 14 * pow(s1, n1 - i - 1); break;
		case 'F':*p1 += 15 * pow(s1, n1 - i - 1); break;
		}
	}
	for (int i = n1 + 1; i < n2; ++i) {
		char a = ch1[i];
		switch (a) {
		case '0':case '1':case '2':case '3':case '4':
		case '5':case '6':case '7':case '8':case '9':
			*p1 += ((int)a - 48)*pow(s1, n1 - i);
			break;
		case 'a':*p1 += 10 * pow(s1, n1 - i); break;
		case 'b':*p1 += 11 * pow(s1, n1 - i); break;
		case 'c':*p1 += 12 * pow(s1, n1 - i); break;
		case 'd':*p1 += 13 * pow(s1, n1 - i); break;
		case 'e':*p1 += 14 * pow(s1, n1 - i); break;
		case 'f':*p1 += 15 * pow(s1, n1 - i); break;
		case 'A':*p1 += 10 * pow(s1, n1 - i); break;
		case 'B':*p1 += 11 * pow(s1, n1 - i); break;
		case 'C':*p1 += 12 * pow(s1, n1 - i); break;
		case 'D':*p1 += 13 * pow(s1, n1 - i); break;
		case 'E':*p1 += 14 * pow(s1, n1 - i); break;
		case 'F':*p1 += 15 * pow(s1, n1 - i); break;
		}
	}
}
void obr_s2(int s2, long double p1, char *baff) {

	long long int cch = floor(p1), i = 0;
	double dch = p1 - cch;
	while (cch >= s2) {
		baff[i] = perevod(cch%s2);
		cch /= s2;
		i++;
	}
	baff[i] = perevod(cch%s2);
	int c = i + 1;
	if (dch != 0) {
		i++;
		baff[i] = '.';
		i++;
		for (int j = 0; j < 20; ++j) {
			baff[i] = perevod((int)(dch*s2));
			dch = (dch*s2) - (int)(dch*s2);
			if (dch == 0)
				break;
			i++;
		}
	}
	char e;
	int j;
	for (j = 0; j < c / 2; ++j) {
		e = baff[j];
		baff[j] = baff[c - j - 1];
		baff[c - j - 1] = e;
	}
	if (baff[i] == 'M') {
		baff[i] = '\0';
	}
	else {
		baff[i + 1] = '\0';
	}
}
int main() {
	int s1, s2;
	long double p1 = 0;
	char ch1[50], ch2[100];
	ifstream fin("in.txt");
	ofstream fon("out.txt");
	fin >> s1 >> s2;
	fin >> ch1;
	fin.close();
	if (proverka(s1, s2, ch1)) {
		fon << "bad input";
		return 0;
	}
	obr_10(ch1, s1, &p1);
	if (p1 == 0) {
		fon << '0';
		return 0;
	}
	obr_s2(s2, p1, ch2);
	fon << ch2;
	fon.close();
	return 0;
}