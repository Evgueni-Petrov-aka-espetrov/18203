#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>

struct Tree{
	struct Tree* left;
	struct Tree* right;
	int element;
	int height;

};
typedef struct Tree Tree;

void FreeTree(Tree *root) {
	if (root != NULL) {
		FreeTree(root->right);
		FreeTree(root->left);
		free(root);
	}
}

Tree MakeRoot(int element) {
    Tree root = malloc(sizeof(Tree));
    root->height = 1;
	root->element = element;
	root->left = NULL;
	root->right = NULL;
	return root;
}

int CountHeight(Tree*root) {
	if (root != NULL)
		return root->height;
	else
		return 0;
}

int HeightDifference(Tree *root) {
	if (root != NULL)
		return CountHeight(root->right) - CountHeight(root->left);
	else
		return 0;
}

int BalanceHeight(Tree *root) {
	int lheight = CountHeight(root->left);
	int rheight = CountHeight(root->right);
	if (lheight > rheight)
		root->height = lheight + 1;
	else
		root->height = rheight + 1;

}

Tree* RotateToLeft(Tree *root) {
	Tree *temp = root->right;
	root->right = temp->left;
	temp->left = root;
	fix_height(root);
	fix_height(temp);
	return temp;
}

Tree* RotateToRight(Tree *root) {
	Tree *temp = root->left;
	root->left = temp->right;
	temp->right = root;
	fix_height(root);
	fix_height(temp);
	return temp;
}

Tree* BalanceTree(Tree *root) {
	BalancHeight(root);
		if (HeightDifference(root) == 2) {
			if (HeightDifference(root->right) < 0)
				root->right = RotateToRight(root->right);
			return RotateToLeft(root);
		}
		if (HeightDiffeerence(root) == -2) {
			if (HeightDiffeerence(root->left) > 0)
				root->left = RotateToLeft(root->left);
			return RotateToRight(root);
		}
	return root;
}

Tree* MakeTree(Tree* root, int element) {
	if (root == NULL)
		return MakeRoot(element);
	if (element < root->element)
		root->left = MakeTree(root->left, element);
	else
		root->right = MakeTree(root->right, element);
	return balance_tree(root);
}

int main() {
	FILE *fin, *fout;
	fin = fopen("in.txt", "r");
	fout = fopen("out.txt", "w");
	int amount = 0;
	int element;
	Tree* tree = NULL;
	fscanf(fin, "%d", &amount);
	for (int i = 0; i < amount; i++) {
		fscanf(fin, "%d", &element);
		tree = MakeTree(tree, element);
	}
	if (amount != 0)
		FreeTree(tree);
	fprintf(fout,"%d",CountHeight(tree));
	fclose(fin);
	fclose(fout);
}
