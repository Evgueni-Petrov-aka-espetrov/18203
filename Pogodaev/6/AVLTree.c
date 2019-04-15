#include <stdio.h>
#include <stdlib.h>
#include "avltree.h"

unsigned char Height(const TTree* root) {
	return root ? root->height : 0;
}

void FixHeight(TTree *node) {
	unsigned char leftNodeHeight = Height(node->leftChild);
	unsigned char rightNodeHeight = Height(node->rightChild);
	node->height = (leftNodeHeight > rightNodeHeight ? leftNodeHeight : rightNodeHeight) + 1;
}

int BalanceFactor(TTree *node) {
	return Height(node->rightChild) - Height(node->leftChild);
}

TTree *CreateTree(int startValue) {
	TTree *newTreeNode = (TTree*)malloc(sizeof(TTree));
	if (newTreeNode != NULL) {
		newTreeNode->value = startValue;
		newTreeNode->height = (unsigned char)1;
		newTreeNode->leftChild = NULL;
		newTreeNode->rightChild = NULL;
		return newTreeNode;
	}
	else {
		printf("not enough memory");
		exit(0);
	}
	return newTreeNode;
}

void DeleteTree(TTree *root) {
	if (root == NULL) {
		return;
	}
	DeleteTree(root->leftChild);
	DeleteTree(root->rightChild);
	free(root);
	return;
}

TTree *SmallLeftRotate(TTree *node) {
	TTree *newRoot = node->leftChild;
	node->leftChild = newRoot->rightChild;
	newRoot->rightChild = node;
	FixHeight(newRoot->rightChild);
	FixHeight(newRoot);
	return newRoot;
}

TTree *SmallRightRotate(TTree *node) {
	TTree *newRoot = node->rightChild;
	node->rightChild = newRoot->leftChild;
	newRoot->leftChild = node;
	FixHeight(newRoot->leftChild);
	FixHeight(newRoot);
	return newRoot;
}

TTree *BigLeftRotate(TTree *node) {
	TTree *newRoot = node->leftChild->rightChild;
	node->leftChild->rightChild = newRoot->leftChild;
	newRoot->leftChild = node->leftChild;
	node->leftChild = newRoot->rightChild;
	newRoot->rightChild = node;
	FixHeight(newRoot->leftChild);
	FixHeight(newRoot->rightChild);
	FixHeight(newRoot);
	return newRoot;
}

TTree *BigRightRotate(TTree *node) {
	TTree *newRoot = node->rightChild->leftChild;
	node->rightChild->leftChild = newRoot->rightChild;
	newRoot->rightChild = node->rightChild;
	node->rightChild = newRoot->leftChild;
	newRoot->leftChild = node;
	FixHeight(newRoot->leftChild);
	FixHeight(newRoot->rightChild);
	FixHeight(newRoot);
	return newRoot;
}

TTree *LeftRotate(TTree *node) {
	if (Height(node->leftChild->leftChild) == Height(node->rightChild)) {
		return BigLeftRotate(node);
	}
	else {
		return SmallLeftRotate(node);
	}
}

TTree *RightRotate(TTree *node) {
	if (Height(node->rightChild->rightChild) == Height(node->leftChild)) {
		return BigRightRotate(node);
	}
	else {
		return SmallRightRotate(node);
	}
}

TTree *Balance(TTree *node) {
	if (BalanceFactor(node) >= 2) {
		return RightRotate(node);
	}
	else if (BalanceFactor(node) <= -2) {
		return LeftRotate(node);
	}
	FixHeight(node);
	return node;
}

TTree *Add(TTree* treeRoot, int value) {
	if (treeRoot == NULL) {
		return CreateTree(value);
	}
	if (value < treeRoot->value) {
		treeRoot->leftChild = Add(treeRoot->leftChild, value);
	}
	else {
		treeRoot->rightChild = Add(treeRoot->rightChild, value);
	}
	return Balance(treeRoot);
}
