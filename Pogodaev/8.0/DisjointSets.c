#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "disjointsets.h"

DisjointSets *CreateSets() {
	DisjointSets *sets = (DisjointSets*)malloc(sizeof(DisjointSets));
	if (!sets) {
		printf("not enough memory");
		exit(0);
	}
	memset(sets->parent, 0, sizeof(sets->parent));
	memset(sets->parent, 0, sizeof(sets->rank));
	return sets;
}

void MakeSet(DisjointSets *sets, short num) {
	if (num < 1 || num > N) {
		printf("%d is invalid number\n", num);
		return;
	}
	if (sets->parent[num - 1] != 0) {
		printf("%d already in set\n", num);
		return;
	}
	sets->parent[num - 1] = num;
	sets->rank[num - 1] = 0;
	return;
}

short FindSet(DisjointSets *sets, short num) {
	if (num < 1 || num > N) {
		printf("%d is invalid number\n", num);
		return 0;
	}
	if (sets->parent[num - 1] == 0) {
		printf("%d not in set\n", num);
		return 0;
	}
	if (num != sets->parent[num - 1]) {
		sets->parent[num - 1] = FindSet(sets, sets->parent[num - 1]);
	}
	return sets->parent[num - 1];
}

void Union(DisjointSets *sets, short firstNum, short secondNum) {
	short firstNumID = FindSet(sets, firstNum);
	short secondNumID = FindSet(sets, secondNum);
	if (firstNumID == 0 || secondNumID == 0) {
		printf("union error\n");
		return;
	}
	if (firstNumID == secondNumID) {
		return;
	}
	if (sets->rank[firstNumID - 1] > sets->rank[secondNumID - 1]) {
		sets->parent[secondNumID - 1] = firstNumID;
	}
	else {
		sets->parent[firstNumID - 1] = secondNumID;
		if (sets->rank[firstNumID - 1] == sets->rank[secondNumID - 1]) {
			sets->rank[secondNumID - 1] += 1;
		}
	}
	return;
}

void DeleteSet(DisjointSets *sets) {
	free(sets);
	return;
}
