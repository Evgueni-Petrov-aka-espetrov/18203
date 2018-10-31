#include <stdio.h>
#include <string.h>
int check1 (char array[], int amount) {
	int i;
	int errornum = 0;
	for (i=0; i<amount; i++) {
		if (array[i] < 40) errornum++;
		if (array[i] > 57) errornum++;
		if (array[i] == 46) errornum++;
		if (array[i] == 44) errornum++;
	}
	return errornum;
}

int convert (char symbols[], int enter[], int length) {
	int i;
	for (i=0; i<length; i++) {
		enter[i] = symbols[i] - 48;
		if (enter[i] == -6) enter[i] = -2;
		if (enter[i] == -5) enter[i] = -4;
		if (enter[i] == -7) enter[i] = -5;
		if (enter[i] == -8) enter[i] = -6;
	}
	for (i=1; i<length; i++) {
		if ((enter[i-1] >= 0) && (enter[i] >= 0)) {
			enter[i-1] = 10*enter[i-1] + enter[i];
			int j;
			for (j=i+1; j<length; j++) enter[j-1] = enter[j];
			length--;
		} 
	}
	return length;
}

int priority (int a) {
	if (a == -1) return 3; 
	if (a == -2) return 3;
	if (a == -3) return 2;
	if (a == -4) return 2;
	if (a == -5) return 1;
	if (a == -6) return 1;
}

void createPol (int enter; int amount) {
	int pol[1000];
	int operands[1000];
	int polcount = 0;
	int opercount = 0;
	int i;
	
	for (i=0; i<amount; i++) {
		if (enter[i] >= 0) {
			pol[polcount] = enter[i];
			polcount++;
		}
		if (enter[i] < 0) {
			if (opercount == 0){
				operands[opercount] = enter[i];
				opercount++;
			}
			else {
				if (priority(operands[opercount-1]) < priority(enter[i])) {
					operands[opercount] = enter[i];
				}
				if (priority(operands[opercount-1]) == priority(enter[i])) {
					if (priority(enter[i]) == 3) {
						if (enter[i] == -5) {
							if (operands[opercount-1] == -6) {
								
							}
						}
					}
					else {
						pol[polcount] = operands[opercount-1];
						polcount++;
						operands[opercount-1] = enter[i];
				}
				
			}
		}
	}
}

int main() {
	char symbols[1000];
	int enter[1000];
	int amount;
	FILE *fout = fopen("out.txt", "w");
	FILE *fin = fopen("in.txt", "r");
	if (fin == NULL)
		fprintf(fout, "File could not be opened.");
	else {
		int amount;
		fscanf(fin, "%s", symbols);
		amount = strlen(symbols);	
		if (check1(symbols, amount)>0) fprintf(fout, "syntax error");
		else {
			amount = convert(symbols, enter, amount);
			
		}
}
