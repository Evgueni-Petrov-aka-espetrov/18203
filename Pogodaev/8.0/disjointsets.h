#pragma once
#define N 5000

typedef struct {
	short parent[N];
	short rank[N];
} DisjointSets;

DisjointSets *CreateSets();
void MakeSet(DisjointSets *sets, short num);
short FindSet(DisjointSets *sets, short num);
void Union(DisjointSets *sets, short firstNum, short secondNum);
void DeleteSet(DisjointSets *sets);
