#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#include<limits.h>

void Check(int vertex, int edge, FILE* out)
{
	if (vertex < 0 || vertex > 5000)
	{
		fprintf(out, "%s", "bad number of vertices");
		exit(0);
	}
	if (vertex == 0 && edge == 0)
	{
		fprintf(out, "%s", "bad vertex");
		exit(0);
	}
	if (edge < 0 || edge > vertex*(vertex + 1) / 2)
	{
		fprintf(out, "%s", "bad number of edges");
		exit(0);
	}

}

void Check2(int start, int finish, const unsigned int length, int vertex, FILE* out)
{
	if (start < 1 || start > vertex || finish < 1 || finish > vertex)
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

unsigned int **BuildGraph(int vertex, int edge, FILE* in, FILE* out)
{
	unsigned int** graph = (unsigned int**)malloc(vertex * sizeof(unsigned int));

	for (int i = 0; i < vertex; i++)
		graph[i] = (unsigned  int*)calloc(vertex, sizeof(unsigned int));

	for (int i = 0; i < edge; ++i)
	{
		int s = 0, f = 0;
		unsigned int l = 0;
		fscanf(in, "%d", &s);
		fscanf(in, "%d", &f);
		fscanf(in, "%d", &l);
		if (feof(in))
		{
			if (i < edge - 1)
			{
				fprintf(out, "%s", "bad number of lines");
				exit(0);
			}
		}
		Check2(s, f, l, vertex, out);
		graph[s - 1][f - 1] = l;
		graph[f - 1][s - 1] = l;
	}
	return graph;
}

void FindMinPath(unsigned int** graph, unsigned int* result, int S, int F, int v, FILE* out, int correct, int edge) {
	if (correct)
	{
		fprintf(out, "\n");
		fprintf(out, "no path");
		exit(0);
	}

	int* vertex = (int*)malloc(v * sizeof(int));
	int start = S - 1;
	int finish = F - 1;
	if (start == finish)
	{
		fprintf(out, "\n");
		fprintf(out, "%d", start + 1);
		exit(0);
	}
	vertex[0] = finish + 1;
	int k = 1;
	int weight = result[finish];
	while (finish != start)
	{
		for (int i = 0; i < v; i++)
			if (graph[finish][i] != 0)
			{
				int temp = weight - graph[finish][i];
				if (temp == result[i])
				{
					weight = temp;
					finish = i;
					vertex[k] = i + 1;
					k++;
				}
			}
	}
	fprintf(out, "\n");
	unsigned int sum = 0;
	for (int i = 0; i < k - 1; i++)
	{
		sum += graph[vertex[i] - 1][vertex[i + 1] - 1];
	}
	if (sum > INT_MAX && edge != 2)
	{
		fprintf(out, "%s", "overflow");
		exit(0);
	}

	for (int i = 0; i < k; i++) {
		fprintf(out, "%d ", vertex[i]);
	}
	free(vertex);
}

void Dijkstra(unsigned int** graph, int start, int finish, int vertex, int edge, FILE* output, FILE* input) {
	unsigned int *result = (unsigned int*)malloc(vertex * sizeof(unsigned int));
	int *used = (int*)malloc(vertex * sizeof(int));
	unsigned int min, minindex;

	for (int i = 0; i < vertex; i++)
	{
		result[i] = UINT_MAX;
		used[i] = 1;
	}
	result[start - 1] = 0;

	do {
		minindex = UINT_MAX;
		min = UINT_MAX;
		for (int i = 0; i < vertex; i++)
		{
			if ((used[i] == 1) && (result[i] < min))
			{
				min = result[i];
				minindex = i;
			}
		}
		if (minindex != UINT_MAX)
		{
			for (int i = 0; i < vertex; i++)
			{
				if (graph[minindex][i] > 0 && used[i] != 0)
				{
					unsigned int temp = 0;
					temp = min + graph[minindex][i];
					if (temp < result[i])
					{
						result[i] = temp;
					}
				}
			}
			used[minindex] = 0;
		}
	} while (minindex < UINT_MAX);
	short int is_path = 0;
	if (result[start - 1] == UINT_MAX || result[finish - 1] == UINT_MAX)
		is_path = 1;
	for (int i = 0; i < vertex; i++)
	{
		if (result[i] == UINT_MAX)
			fprintf(output, "%s ", "oo");
		else
		{
			if (result[i] > INT_MAX)
				fprintf(output, "%s ", "INT_MAX+");
			else
				fprintf(output, "%u ", result[i]);
		}
	}
	free(used);
	FindMinPath(graph, result, start, finish, vertex, output, is_path, edge);
	free(result);
}

int main()
{
	FILE* in = fopen("in.txt", "r");
	FILE* out = fopen("out.txt", "w");
	int vertex, edge, start, finish;
	fscanf(in, "%d", &vertex);
	fscanf(in, "%d", &start);
	fscanf(in, "%d", &finish);
	fscanf(in, "%d", &edge);
	Check(vertex, edge, out);
	unsigned int** graph = BuildGraph(vertex, edge, in, out);
	Dijkstra(graph, start, finish, vertex, edge, out, in);
	fclose(in);
	fclose(out);
	for (int i = 0; i < vertex; i++)
	{
		free(graph[i]);
	}
	return 0;
}
