#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct tNode {
	int key;
	unsigned char height;
	struct tNode* left;
	struct tNode* right;
} node;

node* newNode(node* tree, int val) {
	tree = (node*)malloc(sizeof(node));
	assert(tree);
	tree->key = val;
	tree->height = 1;
	tree->left = tree->right = NULL;
	return tree;
}

int getHeight(node* tree) {
	return tree ? tree->height : 0;
}

void updateHeight(node* tree) {
	int leftHeight = getHeight(tree->left);
	int rightHeight = getHeight(tree->right);
	if (leftHeight > rightHeight)
		tree->height = leftHeight + 1;
	else
		tree->height = rightHeight + 1;
}

node* leftRotate(node* tree) {
	node* tmp = tree->right;
	tree->right	 = tmp->left;
	tmp->left = tree;
	updateHeight(tree);
	updateHeight(tmp);
	return tmp;
}

node* rightRotate(node* tree) {
	node* tmp = tree->left;
	tree->left = tmp->right;
	tmp->right = tree;
	updateHeight(tree);
	updateHeight(tmp);
	return tmp;
}

node* updateBalance(node* tree) {
	int balanceFactor = getHeight(tree->right) - getHeight(tree->left);
	if (balanceFactor == 2) {
		if (getHeight(tree->right->left) > getHeight(tree->right->right))
			tree->right = rightRotate(tree->right);
		return leftRotate(tree);
	}
	if (balanceFactor == -2) {
		if (getHeight(tree->left->right) > getHeight(tree->left->left))
			tree->left = leftRotate(tree->left);
		return rightRotate(tree);
	}
	return tree;
}

node* insert(node* tree, int val) {
	if (!tree)
		return newNode(tree, val);
	if (tree->key >= val)
		tree->left = insert(tree->left, val);
	else 
		tree->right = insert(tree->right, val);
	updateHeight(tree);
	return updateBalance(tree);
}

void freeTree(node* tree) {
	if (tree->left)
		freeTree(tree->left);
	if (tree->right)
		freeTree(tree->right);
	free(tree);
}

void printTree(node* tree) {
	if (tree->left)
		printTree(tree->left);
	printf("%d %d\n", tree->key, tree->height);
	if (tree->right)
		printTree(tree->right);
}

int main() {
	int n, val;
	node* tree = NULL;
	scanf("%d", &n);
	for (int i = 0; i < n; i++) {
		scanf("%d", &val);
		tree = insert(tree, val);
	}
	printf("%d ", getHeight(tree));
	freeTree(tree);
	return 0;
}
