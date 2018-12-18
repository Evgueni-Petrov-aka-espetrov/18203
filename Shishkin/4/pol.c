#include "queue.h"
#include "stack.h"
#include "pol.h"
#include <stddef.h> 

void createPol(int enter[], queue **polhead, queue **poltail, int amount) {
	stack *operhead = NULL;
	int i;
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
					if (priority(enter[i]) == 1) {	//brackets
						if (enter[i] == -5) {
							while ((operhead)->data != -6) {			
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
						do {

							int value = pop(&operhead);
							enqueue(polhead, poltail, value);

						} while ((isemptystack(operhead) != 0) && (priority((operhead)->data) >= priority(enter[i])));			
						push(&operhead, enter[i]);
					}
				}
			}
		}
	}
	while (isemptystack(operhead) != 0) {
		int value = pop(&operhead);
		enqueue(polhead, poltail, value);
	}
}

int readPol(queue **polhead, queue **poltail, int *dbz) {
	stack *reshead = NULL;
	while (isemptyqueue(*polhead) != 0) {
		int value = dequeue(polhead, poltail);
		if (value >= 0) push(&reshead, value);
		else {
			int value1 = pop(&reshead);
			int value2 = pop(&reshead);
			switch (value) {
				case -1:
					if (value1 == 0) {
						*dbz = 1;
						return 1;
					}
					else push(&reshead, value2 / value1);
					break;
				case -2:
					push(&reshead, value2 * value1);
					break;
				case -3:
					push(&reshead, value2 - value1);
					break;
				case -4:
					push(&reshead, value2 + value1);
					break;
			}
		}
	}
	return pop(&reshead);
}