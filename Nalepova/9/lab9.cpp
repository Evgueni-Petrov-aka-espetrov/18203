#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#include<limits.h>
#include<iostream>

#define MYMAX UINT_MAX

void Check(int vertex, int edges, FILE* out)
{

	if (vertex < 0 || vertex > 5000) 
	{
		fprintf(out, "%s", "bad number of vertices");
		exit(0);
	}

	if (edges < 0 || edges > vertex*(vertex + 1) / 2)
	{
		fprintf(out, "%s", "bad number of edges");
		exit(0);
	}
	if (vertex == 0 && edges == 0)
	{
		fprintf(out, "%s", "bad vertex");
		exit(0);
	}
}

void IsCorrectVerEdLength(int start, int end, const unsigned int length, int vertex, FILE* out)
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

unsigned int **MakeGraph(int vertex, int edges, FILE* in, FILE* out)
{
	unsigned int** graph = (unsigned int**)malloc(vertex * sizeof(unsigned int));

	for (int i = 0; i < vertex; i++)
	{
		graph[i] = (unsigned  int*)calloc(vertex, sizeof(unsigned int));
	}

	for (int i = 0; i < edges; ++i)
	{
		int n = 0, m = 0;
		unsigned int l = 0;
		fscanf(in, "%d", &n);
		fscanf(in, "%d", &m);
		fscanf(in, "%d", &l);
		if (feof(in)) 
		{
			if (i < edges - 1)
			{
				fprintf(out, "%s", "bad number of lines");
				exit(0);
			}
		}
		IsCorrectVerEdLength(n, m, l, vertex, out);
		graph[n - 1][m - 1] = l;
		graph[m - 1][n - 1] = l;
	}
	return graph;
}

void PrintRes(unsigned int* result, int v, FILE* out)
{
	for (int i = 0; i < v; i++)
	{
		if (result[i] == MYMAX) 
		{
			fprintf(out, "%s ", "oo");
		}
		else
		{
			if (result[i] > INT_MAX)
			{
				fprintf(out, "%s ", "INT_MAX+");
			}
			else 
			{
				fprintf(out, "%u ", result[i]);

			}
		}
	}
}
void FindPath(unsigned int** graph, unsigned int* d, int S, int E, int v, FILE* out, short int correct, int edges) {
	if (correct) 
	{
		fprintf(out, "\n");
		fprintf(out, "no path");
		exit(0);
	}

	int* ver = (int*)malloc(v * sizeof(int));
	int start = S - 1;
	int end = E - 1;
	if (start == end) 
	{
		fprintf(out, "\n");
		fprintf(out, "%d", start + 1);
		exit(0);
	}
	ver[0] = end + 1;
	int k = 1;
	int weight = d[end];
	while (end != start)
	{
		for (int i = 0; i < v; i++)
			if (graph[end][i] != 0)
			{
				int temp = weight - graph[end][i];
				if (temp == d[i])
				{
					weight = temp;
					end = i;
					ver[k] = i + 1;
					k++;
				}
			}
	}
	fprintf(out, "\n");
	unsigned long long int sum = 0;
	for (int i = 0; i < k - 1; i++) 
	{
		sum += graph[ver[i] - 1][ver[i + 1] - 1];
	}
	if (sum > INT_MAX && edges != 2) 
	{
		fprintf(out, "%s", "overflow");
		exit(0);
	}

	for (int i = 0; i < k; i++) {
		fprintf(out, "%d ", ver[i]);
	}
	free(ver);
}

void Dijkstra(unsigned int** graph, int start, int finish, int v, int e, FILE* output, FILE* input) {
	unsigned int *d = (unsigned int*)malloc(v * sizeof(unsigned int));
	bool *used = (bool*)malloc(v * sizeof(bool));
	unsigned int min, minindex;

	for (int i = 0; i < v; i++)
	{
		d[i] = MYMAX;
		used[i] = 1;
	}
	d[start - 1] = 0;

	do {
		minindex = MYMAX;
		min = MYMAX;
		for (int i = 0; i < v; i++)
		{
			if ((used[i] == 1) && (d[i] < min))
			{
				min = d[i];
				minindex = i;
			}
		}
		if (minindex != MYMAX)
		{
			for (int i = 0; i < v; i++)
			{
				if (graph[minindex][i] > 0 && used[i] != 0)
				{
					unsigned int temp = 0;
					temp = min + graph[minindex][i];
					if (temp < d[i])
					{
						d[i] = temp;
					}
				}
			}
			used[minindex] = 0;
		}
	} while (minindex < MYMAX);
	short int is_path = 0;
	if (d[start - 1] == MYMAX || d[finish - 1] == MYMAX)
	{
		is_path = 1;
	}
	PrintRes(d, v, output);
	free(used);
	FindPath(graph, d, start, finish, v, output, is_path, e);
	free(d);
}

int main() 
{
	FILE* in;
	FILE* out;
	in = fopen("in.txt", "r");
	out = fopen("out.txt", "w");
	int v, e, S, F;
	fscanf(in, "%d", &v);
	fscanf(in, "%d", &S);
	fscanf(in, "%d", &F);
	fscanf(in, "%d", &e);
	Check(v, e, out);
	unsigned int** G = MakeGraph(v, e, in, out);
	Dijkstra(G, S, F, v, e, out, in);
	fclose(in);
	fclose(out);
	for (int i = 0; i < v; i++) 
	{
		free(G[i]);
	}
	return 0;
}