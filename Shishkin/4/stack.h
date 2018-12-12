struct stack {
	int data;
	struct stack* next;
};
typedef struct stack stack;

void push(stack **, int);
int pop(stack **);
int isemptystack(stack *);