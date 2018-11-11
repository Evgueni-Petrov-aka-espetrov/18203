#include <stdio.h> 
#include <string.h> 
int readfile(FILE *fin, char symbols[1000]) {
	int i;
	char symbol;
	while ((symbol = getc(fin)) != '\n'){
			symbols[i] = symbol;
			i++;
		}
	return i;
}

int check1(char array[], int amount) { 
	int i; 
	int errornum = 0; 
	for (i = 0; i<amount; i++) { 
		if (array[i] < 40) errornum++; 
		if (array[i] > 57) errornum++; 
		if (array[i] == 46) errornum++; 
		if (array[i] == 44) errornum++; 
	} 
	return errornum; 
} 

int priority (int a) { 
	if (a == -1) return 3; 
	if (a == -2) return 3; 
	if (a == -3) return 2; 
	if (a == -4) return 2; 
	if (a == -5) return 1; 
	if (a == -6) return 1; 
} 

int convert(char symbols[], int enter[], int length) { 
	int i; 
	for (i = 0; i<length; i++) { 
		enter[i] = symbols[i] - 48; 
		if (enter[i] == -6) enter[i] = -2; 
		if (enter[i] == -5) enter[i] = -4; 
		if (enter[i] == -7) enter[i] = -5; 
		if (enter[i] == -8) enter[i] = -6; 
	} 
	for (i = 1; i<length; i++) { 
		if ((enter[i - 1] >= 0) && (enter[i] >= 0)) { 
			enter[i - 1] = 10 * enter[i - 1] + enter[i]; 
			int j; 
			for (j = i + 1; j<length; j++) enter[j - 1] = enter[j]; 
			length--; 
			i--;
		} 
	} 
	return length; 
} 

int check2(int array[], int amount) { 
	int i; 
	int checkarray[1000]; 
	int amountob = 0; 
	int amountcb = 0; 
	int errornum = 0; 
	int j = 0; 
	for (i = 0; i < amount; i++) { 
		if (array[i] == -5) { 
			checkarray[i] = array[i]; 
			amountcb++; 
		} 
		if (array[i] == -6) { 
			checkarray[i] = array[i]; 
			amountob++; 
		} 
	} 
	//amount rule
	if ((amount == 1)&&(array[0] < 0)) errornum++;
	if (amount == 2) errornum++;
	if (amount == 0) errornum++; 
	//bracket rule 
	if (amountob != amountcb) errornum++; 
	if (errornum == 0) { 
		i = 0; 
		j = 0; 
		while (amountcb > 0) { 
			while ((checkarray[i] != -6)&&(i < amount)) i++; 
			while ((checkarray[j] != -5)&&(j < amount)) j++; 
			if (i < j) { 
				checkarray[i] = 0; 
				checkarray[j] = 0; 
				i++; 
				j++; 
				amountcb--; 
			} 
			else { 
				errornum++; 
				amountcb = 0; 
			} 
		} 
	}
	//pair rule
	if (errornum == 0) {
		for (i = 1; i < amount-1; i++) {
			if (array[i] < 0) {
				if (priority(array[i]) > 1) {
					if ((array[i-1] < 0) && (array[i-1] > -5)) errornum++;
					if ((array[i+1] < 0) && (array[i+1] > -5)) errornum++;
					if (array[i-1] == -6) errornum++;
					if (array[i+1] == -5) errornum++;
				}
				else {
					if ((array[i] == -6) && (array[i-1] > 0)) errornum++;
					if ((array[i] == -5) && (array[i+1] > 0)) errornum++;
					if ((array[i] == -6) && (array[i+1] == -5)) errornum++;
				}
			}
		}
		if ((array[0] < 0) && (array[0] > -5)) errornum++;
		if ((array[amount-1] < 0) && (array[amount-1] > -5)) errornum++;
	} 
	return errornum; 
} 

int createPol(int enter[], int pol[], int amount) { 
	int operands[1000]; 
	int polcount = 0; 
	int opercount = 0;
	int i;
	for (i = 0; i<amount; i++) { 
		int changeopercount = 0; 
		int changepolcount = 0;
		if (enter[i] >= 0) { 
			pol[polcount] = enter[i]; 
			changepolcount = 1; 
		} 
		if (enter[i] < 0) { 
			if (opercount == 0){ 
				operands[opercount] = enter[i]; 
				changeopercount = 1; 
			} 
			else { 
				if (priority(operands[opercount - 1]) < priority(enter[i])) { 
					operands[opercount] = enter[i]; 
					changeopercount = 1; 
				} 
				if (priority(operands[opercount - 1]) >= priority(enter[i])) { 
					if (priority(enter[i]) == 1) { 
						if (enter[i] == -5) { 
							int j = opercount - 1;
							while (operands[j] != -6) j--;
							int k;
							for (k=opercount-1; k>j; k--) {
								pol[polcount] = operands[k];
								polcount++;
							}
							opercount = j;
						} 
						if (enter[i] == -6) {
							operands[opercount] = enter[i];
							changeopercount = 1;
						}
					} 
					else { 
						while ((priority(operands[opercount - 1]) >= priority(enter[i])) && (opercount > 0)) {
							pol[polcount] = operands[opercount - 1]; 
							polcount++; 
							opercount--;
 
						}
						operands[opercount] = enter[i];
						opercount++;
					} 
				} 
			} 
		}
		opercount = opercount + changeopercount;
		polcount = polcount + changepolcount; 
	}
	for (i=opercount-1; i>=0; i--) {
		pol[polcount] = operands[i];
		polcount++;
	}
	return polcount;
}

int readPol (int pol[], int amount, int divisionbyzero[]) {
	int result[1000];
	int i, j;
	int k = 0;
	for (i=0; i < amount; i++) {
		if (pol[i] >= 0) {
			result[k] = pol[i];
			k++;
		}
		if (pol[i] < 0) {
			if (pol[i] == -1) {
				if (result[k-1] == 0) {
					divisionbyzero[0] = 1;
					return 1;
				}
				else result[k-2] = result[k-2] / result[k-1];
			}
			if (pol[i] == -2) result[k-2] = result[k-2] * result[k-1];
			if (pol[i] == -3) result[k-2] = result[k-2] - result[k-1];
			if (pol[i] == -4) result[k-2] = result[k-2] + result[k-1];
			k = k - 1;
		}
	}
	return result[0];	
}

int main() { 
	FILE *fout = fopen("out.txt", "w"); 
	FILE *fin = fopen("in.txt", "r"); 
	if (fin == NULL) 
		fprintf(fout, "File could not be opened."); 
	else { 
		char symbols[1000]; 
		int amount = readfile(fin, symbols); 
		if (check1(symbols, amount) > 0) fprintf(fout, "syntax error1"); 
		else { 
			int enter[1000];
			amount = convert(symbols, enter, amount); 
			if (check2(enter, amount) > 0) fprintf(fout, "syntax error2"); 
			else {
				int pol[1000]; 
				amount = createPol(enter, pol, amount);
				int dbz[1];
				int result = readPol(pol, amount, dbz);
				if (dbz[0] == 1) fprintf(fout, "division by zero");
				else fprintf(fout, "%d", result);
			}
		}
	}
	return 0;
}




