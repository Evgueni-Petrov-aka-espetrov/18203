#include<cstdio>
#include<iostream>
#include<fstream>
#include<cstdlib>
#include<malloc.h>
using namespace std;

bool change_stroka(int add, int *position, ifstream &fin, char *stroka, int len){
	char simbol;
	while (add != 0) {
		*position=*position+1;
		if (!fin.eof()) {
			fin.get(simbol);
		}
		else {
			return true;
			
		}
		stroka[(*position)%len] = simbol;
		add--;
	}
	return false;
}

int check(char stroka[], int stop[],char shab[], int len, int position,ofstream &fon,int dobavok){
	int posshab = len - 1;
	int add = 0;
		for (int pos = position; pos > position - len; pos--) {
			fon << pos + 1 << " ";
			add = len - stop[int(stroka[position%len]) + 34] - 1 + dobavok;
			if (stroka[pos%len] != shab[posshab]) {
				int posprer = stop[int(stroka[position%len]) + 34];
				if (posprer == -1) {
					add = len;
				}
				else {
					add = len - posprer - 1;
				}
				break;
			}
			posshab--;
		}
		return add;
}

int make_shab(char *shab,ifstream &fin) {
	char simbol;
	int i = 0;
	while (fin.peek()!='\n')
	{
		fin.get(simbol);
		shab[i] = simbol;
		i++;
	}
	return i;
}

void make_stoptable(int *stop,char *shab, ifstream &fin,int len) {
	for (int i = 0; i < 300; i++) {
		stop[i] = -1;
	}
	for (int i = 0; i < len - 1; i++) {
		stop[(int)shab[i] + 34] = i;
	}
}

int main() {
	ifstream fin("in.txt");
	ofstream fon("out.txt");
	char shab[16], simbol=' ';
	int i = 0, stop[300], dob = 0,nomer=0;
	bool m = false;
	int len=make_shab(shab,fin);
	make_stoptable(stop, shab, fin, len);
	if (len == 1) {
		dob = 1;
	}		
	int ned = len;
	char *stroka = (char*) malloc(len * sizeof(char));		
	fin.get(simbol);
	fin.get(simbol);
	stroka[nomer] = simbol;
	ned--;
	
	while (!fin.eof()) {
		
		m = change_stroka(ned, &nomer, fin, stroka, len);
		
		if (fin.eof()) {
			m = true;
		}
		if(!m){
			ned =check(stroka,stop,shab,len,nomer,fon,dob );
		}
	}
	fin.close();
	fon.close();
}