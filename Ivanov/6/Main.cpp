#include <stdio.h>
#include <malloc.h>

struct TTreeNode {
	TTreeNode* left;
	TTreeNode* right;
	int data;
	char height;
};

TTreeNode* leftFlip(TTreeNode* root);
TTreeNode* rightFlip(TTreeNode* root);
TTreeNode* addNodeToTree(TTreeNode* root, TTreeNode* newNode);
int getTreeHeight(TTreeNode* root);
void updateHeight(TTreeNode* root);

int getTreeHeight(TTreeNode* root) {
	return root == NULL ? 0 : root->height;
}

void updateHeight(TTreeNode* root) {
	root->height = getTreeHeight(root->left) > getTreeHeight(root->right) ? getTreeHeight(root->left) + 1 : getTreeHeight(root->right) + 1;
}

TTreeNode* leftFlip(TTreeNode* root) {
	TTreeNode* newRoot = root->right;
	root->right = newRoot->left;
	newRoot->left = root;
	updateHeight(root);
	updateHeight(newRoot);
	return newRoot;
}

TTreeNode* rightFlip(TTreeNode* root) {
	TTreeNode* newRoot = root->left;
	root->left = newRoot->right;
	newRoot->right = root;
	updateHeight(root);
	updateHeight(newRoot);
	return newRoot;
}

TTreeNode* addNodeToTree(TTreeNode* root, TTreeNode* newNode) {
	if (root->data > newNode->data) {
		root->left = root->left == NULL ? newNode : addNodeToTree(root->left, newNode);
	}
	else {
		root->right = root->right == NULL ? newNode : addNodeToTree(root->right, newNode);
	}

	int diff = getTreeHeight(root->left) - getTreeHeight(root->right);
	if (-1 <= diff && diff <= 1) {
		updateHeight(root);
		return root;
	}
	else {
		if (diff > 0) {
			if (getTreeHeight(root->left->right) > getTreeHeight(root->left->left)) {
				root->left = leftFlip(root->left);
			}
			root = rightFlip(root);
		}
		else {
			if (getTreeHeight(root->right->left) > getTreeHeight(root->right->right)) {
				root->right = rightFlip(root->right);
			}
			root = leftFlip(root);
		}
	}
}

int main() {
	int N;
	scanf("%d", &N);
	if (N == 0) {
		printf("0");
		return 0;
	}
	TTreeNode* nodeArray = (TTreeNode*)malloc(sizeof(TTreeNode)*N);
	if (nodeArray == NULL) {
		printf("out of memory");
		return 1;
	}
	for (int i = 0; i < N; ++i) {
		scanf("%d", &(nodeArray[i].data));
		nodeArray[i].left = NULL;
		nodeArray[i].right = NULL;
		nodeArray[i].height = 1;
	}
	TTreeNode* root = nodeArray;
	for (int i = 1; i < N; ++i) {
		root = addNodeToTree(root, nodeArray + i);
	}
	printf("%d", root->height);
	free(nodeArray);
	return 0;
}