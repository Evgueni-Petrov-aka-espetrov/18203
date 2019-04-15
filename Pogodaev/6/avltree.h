#pragma once
typedef struct TTree {
	struct TTree *leftChild;
	struct TTree *rightChild;
	unsigned char height;
	int value;
} TTree;

TTree *Add(TTree*, int);
void DeleteTree(TTree*);
unsigned char Height(const TTree*);
