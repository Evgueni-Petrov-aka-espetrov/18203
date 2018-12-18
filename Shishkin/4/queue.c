#include "queue.h"
#include <malloc.h>
#include <stddef.h>
#include <assert.h>

int isemptyqueue(queue *head) {
	if (head == NULL) return 0;
	else return 1;
}
void enqueue(queue **head, queue **tail, int value) {
	queue *new = (queue*)malloc(sizeof(queue));
	assert(new != NULL);
	new->data = value;
	new->next = NULL;
	if (isemptyqueue(*head) == 0) *head = new;
	else (*tail)->next = new;
	*tail = new;
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