#include "check.h"

int symbolcheck(char array[], int amount) {
	int i;
	int errornum = 0;
	for (i = 0; i<amount; i++) {
		if (array[i] < '(') errornum++;
		if (array[i] > '9') errornum++;
		if (array[i] == '.') errornum++;
		if (array[i] == ',') errornum++;
	}
	return errornum;
}

int positioncheck(int array[], int amount) {
	int i;
	int checkarray[CHECKARRAY_SIZE];
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
	if ((amount == 1) && (array[0] < 0)) errornum++;
	if (amount == 2) errornum++;
	if (amount == 0) errornum++;
	//bracket rule 
	if (amountob != amountcb) errornum++;
	if (errornum == 0) {
		i = 0;
		j = 0;
		while (amountcb > 0) {
			while ((checkarray[i] != -6) && (i < amount)) i++;
			while ((checkarray[j] != -5) && (j < amount)) j++;
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
		for (i = 1; i < amount - 1; i++) {
			if (array[i] < 0) {
				if (priority(array[i]) > 1) {
					if ((array[i - 1] < 0) && (array[i - 1] > -5)) errornum++;
					if ((array[i + 1] < 0) && (array[i + 1] > -5)) errornum++;
					if (array[i - 1] == -6) errornum++;
					if (array[i + 1] == -5) errornum++;
				}
				else {
					if ((array[i] == -6) && (array[i - 1] > 0)) errornum++;
					if ((array[i] == -5) && (array[i + 1] > 0)) errornum++;
					if ((array[i] == -6) && (array[i + 1] == -5)) errornum++;
				}
			}
		}
		if ((array[0] < 0) && (array[0] > -5)) errornum++;
		if ((array[amount - 1] < 0) && (array[amount - 1] > -5)) errornum++;
	}
	return errornum;
}