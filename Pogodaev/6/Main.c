#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "avltree.h"

int main() {
	int nodesCount;
	scanf("%d", &nodesCount);
	TTree *treeRoot = NULL;
	for (int i = 0; i < nodesCount; ++i) {
		int nextValue;
		scanf("%d", &nextValue);
		treeRoot = Add(treeRoot, nextValue);
	}
	printf("%d", (int)Height(treeRoot));
	DeleteTree(treeRoot);
	return 0;
}