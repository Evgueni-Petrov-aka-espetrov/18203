#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h> 
#include<limits.h> 
#include<stdbool.h> 
#include<malloc.h>
#include<stdlib.h>

void Check(int vertex, int edge, FILE* out) 
{
	if (vertex == 0)
	{
		fprintf(out, "%s", "no spanning tree");
		exit(0);
	}
	if (vertex < 0 || vertex > 5000)
	{
		fprintf(out, "%s", "bad number of vertices");
		exit(0);
	}
	if (edge < 0 || edge > vertex * (vertex + 1) / 2)
	{
		fprintf(out, "%s", "bad number of edges");
		exit(0);
	}
	if (edge < vertex - 1)
	{
		fprintf(out, "%s", "no spanning tree");
		exit(0);
	}
}

void IsCorrectVerEdLength(int start, int end, unsigned int length, int vertex, FILE* out)
{
	if (start < 1 || start > vertex || end < 1 || end > vertex)
	{
		fprintf(out, "%s", "bad vertex");
		exit(0);
	}
	if (length < 0 || length > INT_MAX) 
	{
		fprintf(out, "%s", "bad length");
		exit(0);
	}
}

unsigned int **MakeGraph(int vertex, int edge, FILE* in, FILE* out)
{
	unsigned int** graph = (unsigned int**)malloc(vertex * sizeof(unsigned int));

	for (int i = 0; i < vertex; i++)
	{
		graph[i] = (unsigned  int*)calloc(vertex, sizeof(unsigned int));
	}

	for (int i = 0; i < edge; ++i)
	{
		int n = 0, m = 0;
		unsigned int l = 0;
		fscanf(in, "%d", &n);
		fscanf(in, "%d", &m);
		fscanf(in, "%d", &l);
		if (feof(in)) 
		{
			if (i < edge)
			{
				fprintf(out, "%s", "bad number of lines");
				exit(0);
			}
		}

		IsCorrectVerEdLength(n, m, l, vertex, out);
		graph[n - 1][m - 1] = (unsigned int)l;
		graph[m - 1][n - 1] = (unsigned int)l;
	}
	return graph;
}

int MinKey(unsigned int key[], bool mstSet[], int V)
{
	unsigned int min = ULONG_MAX;
	int minIndex;

	for (int v = 0; v < V; v++) 
	{
		if (mstSet[v] == 0 && key[v] < min)
		{
			min = key[v];
			minIndex = v;
		}
	}

	return minIndex;
}

void PrintMST(int parent[], int n, unsigned int **graph, int V, FILE* out)
{

	for (int i = 1; i < V; i++)
	{
		fprintf(out, "%d ", parent[i] + 1);
		fprintf(out, "%d\n", i + 1);
	}
}

void PrimMST(unsigned int **graph, int V, FILE* out)
{
	int *parent = (int*)malloc(V * sizeof(int));
	unsigned int *key = (unsigned int*)malloc(V * sizeof(unsigned int));
	bool *mstSet = (bool*)malloc(V * sizeof(bool));

	for (int i = 0; i < V; i++) {
		key[i] = ULONG_MAX;
		mstSet[i] = 0;
	}

	key[0] = 0;
	parent[0] = -1;

	for (int count = 0; count < V - 1; count++)
	{
		int u = MinKey(key, mstSet, V);

		mstSet[u] = 1;

		for (int v = 0; v < V; v++)

			if (graph[u][v] && mstSet[v] == 0 && graph[u][v] < key[v]) 
			{
				parent[v] = u;
				key[v] = graph[u][v];
			}
	}

	for (int i = 0; i < V; i++)
	{
		if (key[i] == ULONG_MAX)
		{
			fprintf(out, "%s", "no spanning tree");
			exit(0);
		}
	}
	PrintMST(parent, V, graph, V, out);
}

int  main() {
	FILE* in = fopen("in.txt", "r");
	FILE* out = fopen("out.txt", "w");
	int v;
	fscanf(in, "%d", &v);
	int e;
	fscanf(in, "%d", &e);
	Check(v, e, out);
	unsigned int** G = MakeGraph(v, e, in, out);
	PrimMST(G, v, out);
	fclose(in);
	fclose(out);
	return 0;
}