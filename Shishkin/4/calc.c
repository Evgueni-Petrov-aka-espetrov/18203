#include <stdio.h> 
#include <string.h> 
#include <malloc.h>
#include <assert.h>
struct stack {
	int data;
	struct stack* next;
};
typedef struct stack stack;

struct queue {
	int data;
	struct queue* next;
};
typedef struct queue queue;

int readfile(FILE *fin, char symbols[]) {
	int i = 0;
	char symbol;
	while ((symbol = getc(fin)) != EOF){
		symbols[i] = symbol;
		i++;
	}
	i--;
	return i;
}

int check1(char array[], int amount) {
	int i;
	//printf("%d ", amount);
	int errornum = 0;
	for (i = 0; i<amount; i++) {
		if (array[i] < 40) errornum++;
		//printf("%d", errornum);
		if (array[i] > 57) errornum++;
		//printf("%d  ", errornum);
		if (array[i] == 46) errornum++;
		if (array[i] == 44) errornum++;
	}
	//printf("%d", errornum);
	return errornum;
}

int priority(int a) {
	int c = 0;
	if (a == -1) c = 3;
	if (a == -2) c = 3;
	if (a == -3) c = 2;
	if (a == -4) c = 2;
	if (a == -5) c = 1;
	if (a == -6) c = 1;
	//	if (a == NULL) c=0;
	return c;
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
	//printf("%d", amount);
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
void push(stack **head, int value) {
	stack *tmp = (stack*)malloc(sizeof(stack));
	tmp->data = value;
	tmp->next = (*head);
	(*head) = tmp;
}
int pop(stack **head) {
	assert(*head != NULL);
	int value = (*head)->data;
	stack *tmp = (*head)->next;
	free(*head);
	*head = tmp;
	return value;
}
int isemptyqueue(queue *head) {
	if (head == NULL) return 0;
	else return 1;
}
int isemptystack(stack *head){
	if (head == NULL) return 0;
	else return 1;
}
void enqueue(queue **head, queue **tail, int value) {
	queue *new = (queue*)malloc(sizeof(queue));
	new->data = value;
	new->next = NULL;
	if (isemptyqueue(*head) == 0) *head = new;
	else (*tail)->next = new;
	*tail = new;
	//printf("%d", value);
}
int dequeue(queue **head, queue **tail) {
	assert(*head != NULL);
	int value = (*head)->data;
	queue *new = *head;
	*head = (*head)->next;
	if (isemptyqueue(*head) == 0) *tail = NULL;
	free(new);
	return value;
}
void createPol(int enter[], queue **polhead, queue **poltail, int amount) {
	stack *operhead = NULL;
	int i;
	//printf("fdfd");
	for (i = 0; i<amount; i++) {
		if (enter[i] >= 0) {
			enqueue(polhead, poltail, enter[i]);
		}
		if (enter[i] < 0) {
			if (isemptystack(operhead) == 0){
				push(&operhead, enter[i]);
			}
			else {
				if (priority((operhead)->data) < priority(enter[i])) {
					push(&operhead, enter[i]);
				}
				else {
					//printf("fdfd");

					if (priority(enter[i]) == 1) {	//brackets
						if (enter[i] == -5) {
							while ((operhead)->data != -6) {			///////
								int value = pop(&operhead);
								enqueue(polhead, poltail, value);
							}
							pop(&operhead);
						}
						if (enter[i] == -6) {
							push(&operhead, enter[i]);
						}
					}
					else {	//not brackets
						//while ((priority((operhead)->data) >= priority(enter[i])) && (isemptystack(operhead) != 0)) {
						do {

							int value = pop(&operhead);
							enqueue(polhead, poltail, value);

						} while ((isemptystack(operhead) != 0) && (priority((operhead)->data) >= priority(enter[i])));			///
						push(&operhead, enter[i]);
					}
				}
			}
		}
	}
	//printf("fdfd");

	while (isemptystack(operhead) != 0) {
		int value = pop(&operhead);
		enqueue(polhead, poltail, value);
	}
}


int readPol(queue **polhead, queue **poltail, int *dbz) {
	stack *reshead = NULL;
	while (isemptyqueue(*polhead) != 0) {
		int value = dequeue(polhead, poltail);
		printf("%d   ", value);
		if (value >= 0) push(&reshead, value);
		else {
			if (value == -1) {
				int value1 = pop(&reshead);
				int value2 = pop(&reshead);
				if (value1 == 0) {
					*dbz = 1;
					return 1;
				}
				else push(&reshead, value2 / value1);
			}
			if (value == -2) {
				int value1 = pop(&reshead);
				int value2 = pop(&reshead);
				push(&reshead, value2 * value1);
			}
			if (value == -3) {
				int value1 = pop(&reshead);
				int value2 = pop(&reshead);
				push(&reshead, value2 - value1);
			}
			if (value == -4) {
				int value1 = pop(&reshead);
				int value2 = pop(&reshead);
				push(&reshead, value2 + value1);
			}
		}
	}
	return (reshead)->data;
}
int main() {
	FILE *fout = fopen("out.txt", "w");
	FILE *fin = fopen("in.txt", "r");
	if (fin == NULL)
		fprintf(fout, "File could not be opened.");
	else {
		char symbols[1002];
		int amount = readfile(fin, symbols);
		fprintf(stderr, "%d", amount);
		if (check1(symbols, amount) > 0) fprintf(fout, "syntax error1");
		else {
			int enter[1002];
			amount = convert(symbols, enter, amount);
			if (check2(enter, amount) > 0) fprintf(fout, "syntax error2");
			else {
				queue *polhead = NULL;
				queue *poltail = NULL;
				createPol(enter, &polhead, &poltail, amount);
				int dbz = 0;
				int result = readPol(&polhead, &poltail, &dbz);
				if (dbz == 1) fprintf(fout, "division by zero");
				else fprintf(fout, "%d", result);
			}
		}
	}
	return 0;
}
