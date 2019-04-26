#pragma once
#define N 5000

typedef struct {
	int parent[N];
	int rank[N];
} DisjointSets;

DisjointSets *CreateSet();
void MakeSet(DisjointSets *sets, int num);
int FindSet(DisjointSets *sets, int num);
void Union(DisjointSets *sets, int firstNum, int secondNum);
void DeleteSet(DisjointSets *sets);
