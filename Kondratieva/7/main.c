#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct TStack {
	int value;
	struct TStack* next;
} Stack;

typedef struct TGraph {
	int color;
	Stack* adjacent;
} Graph;

void push(Stack** stack, int value) {
	Stack* tmp = malloc(sizeof(Stack));
	assert(tmp);
	tmp->value = value;
	tmp->next = *stack;
	*stack = tmp;
}

int pop(Stack** stack) {
	int value = (*stack)->value;
	Stack* tmp = *stack;
	*stack = (*stack)->next;
	free(tmp);
	return value;
}


void freeStack(Stack* stack) {
	while (stack) {
		pop(&stack);
	}
}

void freeGraph(Graph* graph, int n) {
	for (int i = 0; i < n; i++) {
		freeStack(graph[i].adjacent);
	}
	free(graph);
}

Graph* buildGraph(int n, int m) {
	Graph* graph = malloc(sizeof(Graph) * n);
	assert(graph);
	for (int i = 0; i < n; i++) {
		graph[i].color = 0;
		graph[i].adjacent = NULL;
	}
	int begin, end;
	for (int i = 0; i < m; i++) {
		if (scanf("%d%d", &begin, &end) < 2) {
			freeGraph(graph, n);
			printf("bad number of lines");
			return NULL;
		}
		if (begin < 1 || begin > n || end < 1 || end > n) {
			freeGraph(graph, n);
			printf("bad vertex");  
			return NULL;
		}
		push(&graph[begin - 1].adjacent, end - 1);
	}
	return graph;
}

void dfs(Graph* graph, int vertex, Stack** sortedGraph, int* cycle) {
	graph[vertex].color = 1;
	Stack* tmp = graph[vertex].adjacent;
	while (tmp) {
		if (graph[tmp->value].color == 0) {
			dfs(graph, tmp->value, sortedGraph, cycle);
		}
		if (graph[tmp->value].color == 1) {
			*cycle = 1;
		}
		tmp = tmp->next;
	}
	graph[vertex].color = 2;
	push(sortedGraph, vertex);
}


Stack* topSort(Graph* graph, int n) {
	int cycle = 0;
	Stack* sortedGraph = NULL;
	for (int i = 0; i < n; i++) {
		if (graph[i].color == 0) {
			dfs(graph, i, &sortedGraph, &cycle);
		}
		if (cycle) {
			printf("impossible to sort");
			freeStack(sortedGraph);
			return NULL;
		}
	}
	return sortedGraph;
}

void printGraph(Stack* sortedGraph) {
	while (sortedGraph) {
		printf("%d ", pop(&sortedGraph) + 1);
	}
}

int main() {
	int n, m;
	if (scanf("%d%d", &n, &m) < 2) {
		printf("bad number of lines");
		return 0;
	}
	if (n < 0 || n > 1000) {
		printf("bad number of vertices");
		return 0;
	}
	if (m < 0 || m > n*(n - 1) / 2) {
		printf("bad number of edges");
		return 0;
	}
	Graph* graph = buildGraph(n, m);	
	if (!graph) {
		return 0;
	}
	Stack* sortedGraph = topSort(graph, n);
	if (sortedGraph) {
		printGraph(sortedGraph);
	}
	freeGraph(graph, n);
	return 0;
}
