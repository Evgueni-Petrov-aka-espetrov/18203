#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
	int data;
	Node* next;
} Node;

typedef struct Stack
{
	Node* first;
} Stack;

typedef struct VerTable
{
	Stack** v;
	int* color;
} VerTable;

typedef struct Edge
{
	int v1;
	int v2;
} Edge;

void FreeNode(Node* node)
{
	if (node != NULL)
	{
		FreeNode(node->next);
		free(node);
	}
}

void FreeStack(Stack* stack)
{
	FreeNode(stack->first);
	free(stack);
}

void FreeTable(VerTable* table, int NumVer)
{
	for (int i = 1; i <= NumVer; ++i)
	{
		FreeStack(table->v[i]);
	}
	free(table->color);
	free(table);
}

Stack* NewStack()
{
	Stack* newStack = (Stack*)malloc(sizeof(Stack));
	newStack->first = NULL;
	return newStack;
}

Node* PushNode(int value)
{
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->data = value;
	newNode->next = NULL;
	return newNode;
}

void PushStack(Stack* stack, int value)
{
	Node* nodeInStack = PushNode(value);
	nodeInStack->next = stack->first;
	stack->first = nodeInStack;
}

int PopStack(Stack* stack)
{
	Node* nodeFromStack = stack->first;
	int value = nodeFromStack->data;
	stack->first = stack->first->next;
	free(nodeFromStack);
	return value;
}

void PushEdge(VerTable* table, Edge edge)
{
	PushStack(table->v[edge.v1], edge.v2);
}

VerTable* NewTable(int vertex)
{

	VerTable* table = (VerTable*)malloc(sizeof(VerTable));

	table->v = (Stack**)malloc(sizeof(Stack*)*(vertex + 1));
	for (int i = 1; i <= vertex; ++i)
	{
		table->v[i] = NewStack();
	}

	table->color = (int*)calloc(vertex + 1, sizeof(int));
	return table;
}


void TopSort(int TmpVer, VerTable* table, Stack* stackRes)
{
	if (table->color[TmpVer] == 0)
	{
		table->color[TmpVer] = 1;

		while (table->v[TmpVer]->first != NULL)
		{
			TopSort(PopStack(table->v[TmpVer]), table, stackRes);
		}
		table->color[TmpVer] = 2;

		PushStack(stackRes, TmpVer);
		return;
	}
	else if (table->color[TmpVer] == 1)
	{
		printf("impossible to sort");
		exit(0);
	}
}

int main() {
	Stack* stack = NewStack();
	int vertex;
	int edge;

	if (scanf("%d ", &vertex) == 1)
	{
		if (vertex > 1000)
		{
			printf("bad number of vertices");
			return 0;
		}
	}
	else
	{
		printf("bad number of lines");
		return 0;
	}

	if (scanf("%d ", &edge) == 1)
	{
		if (edge > vertex*(vertex + 1) / 2)
		{
			printf("bad number of edges");
			return 0;
		}
	}
	else
	{
		printf("bad number of lines");
		return 0;
	}

	VerTable* table = NewTable(vertex);

	for (int i = 0; i < edge; ++i)
	{
		Edge tmp;
		if (scanf("%d%d", &tmp.v1, &tmp.v2) < 2)
		{
			printf("bad number of lines");
			return 0;
		}

		if ((tmp.v1 > vertex) || (tmp.v2 > vertex) || (tmp.v1 < 0) || (tmp.v2 < 0))
		{
			printf("bad vertex");
			return 0;
		}
		if (tmp.v1 == tmp.v2)
		{
			printf("impossible to sort");
			return 0;
		}
		PushEdge(table, tmp);
	}

	Stack* resStack = NewStack();
	for (int i = 1; i <= vertex; ++i)
	{
		TopSort(i, table, resStack);
	}

	while (resStack->first != NULL)
	{
		printf("%d ", PopStack(resStack));
	}
	FreeTable(table, vertex);
	FreeStack(resStack);

	return 0;
}