#include<stdio.h>
#include<limits.h>
#include<stdbool.h>
#include<malloc.h>
#include<stdlib.h>
#include<string.h>

int FindMinIndex(unsigned int key[], bool mstSet[], int V){
	unsigned int min = UINT_MAX;
	int minIndex;
	for (int v = 0; v < V; v++){
		if (mstSet[v] == false && key[v] < min){
			min = key[v];
			minIndex = v;
		}
	}
	return minIndex;
}

void CheckLengh(int start, int end, unsigned int length, int vertex, FILE* out){
	if (start < 1 || start > vertex || end < 1 || end > vertex){
		fprintf(out, "%s", "bad vertex");
		exit(0);
	}
	if (length < 0 || length > INT_MAX){
		fprintf(out, "%s", "bad length");
		exit(0);
	}
}

unsigned int **BuildGraph(int vertex, int edge, FILE* in, FILE* out){
	unsigned int** G = (unsigned int**)malloc(vertex * sizeof(unsigned int));
	for (int i = 0; i < vertex; i++)
        G[i] = (unsigned  int*)calloc(vertex, sizeof(unsigned int));
	for (int i = 0; i < edge; i++){
		int n = 0;
		fscanf(in, "%d", &n);
		int m = 0;
		fscanf(in, "%d", &m);
		unsigned int l = 0;
		fscanf(in, "%d", &l);
		if (feof(in) && i < edge){
            fprintf(out, "%s", "bad number of lines");
            exit(0);
            }
		CheckLengh(n, m, l, vertex, out);
		G[n - 1][m - 1] = l;
		G[m - 1][n - 1] = l;
	}
	return G;
}
void PrintMST(int parent[], int n, unsigned int **G, int V, FILE* out){
	for (int i = 1; i < V; i++){
		fprintf(out, "%d ", parent[i] + 1);
		fprintf(out, "%d\n", i + 1);
	}
}

void PrimMST(unsigned int **G, int V, FILE* out){
	int *parent = (int*)malloc(V * sizeof(int));
	unsigned int *key = (unsigned int*)malloc(V * sizeof(unsigned int));
	bool *mstSet = (bool*)malloc(V * sizeof(bool));
	for (int i = 0; i < V; i++){
		key[i] = UINT_MAX;
		mstSet[i] = false;
	}
	key[0] = 0;
	parent[0] = -1;
	for (int count = 0; count < V - 1; count++){
		int mk = FindMinIndex(key, mstSet, V);
		mstSet[mk] = true;
		for (int v = 0; v < V; v++)
			if (G[mk][v] && mstSet[v] == false && G[mk][v] < key[v]){
				parent[v] = mk;
				key[v] = G[mk][v];
			}
	}
	for (int i = 0; i < V; i++){
		if (key[i] == UINT_MAX){
			fprintf(out, "%s", "no spanning tree");
			exit(0);
		}
	}
	PrintMST(parent, V, G, V, out);
}


void Check(int v, int e,  FILE* out){
    if (v == 0){
		fprintf(out, "%s", "no spanning tree");
		exit(0);
	}
	if (v < 0 || v > 5000){
		fprintf(out, "%s", "bad number of vertices");
		exit(0);
	}
	if (e < 0 || e > v * (v + 1) / 2){
		fprintf(out, "%s", "bad number of edges");
		exit(0);
	}
	if (e < v - 1){
		fprintf(out, "%s", "no spanning tree");
		exit(0);
	}
}

int  main() {
	FILE* in = fopen("in.txt", "r");
	FILE* out = fopen("out.txt", "w");
	int vertex, edge;
	fscanf(in, "%d", &vertex);
	fscanf(in, "%d", &edge);
	Check(vertex, edge, out);
	unsigned int** G = BuildGraph(vertex, edge, in, out);
	PrimMST(G, vertex, out);
	fclose(in);
	fclose(out);
	return 0;
}
