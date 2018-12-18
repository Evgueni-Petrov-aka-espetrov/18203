#include "stack.h"
#include <malloc.h>
#include <string.h>
#include <assert.h>

void push(stack **head, int value) {
	stack *tmp = (stack*)malloc(sizeof(stack));
	assert(tmp != NULL);
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
int isemptystack(stack *head){
	if (head == NULL) return 0;
	else return 1;
}