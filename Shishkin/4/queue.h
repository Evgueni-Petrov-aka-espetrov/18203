struct queue {
	int data;
	struct queue* next;
};
typedef struct queue queue;

int isemptyqueue(queue *);
void enqueue(queue **, queue **, int);
int dequeue(queue **, queue **);