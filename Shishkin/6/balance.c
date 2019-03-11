#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <assert.h>
struct tree {
	struct tree *left;
	struct tree *right;
	int num;
	int height;
};
typedef struct tree tree;
int readnum(FILE *in) {
	int num = 0;
	char c = fgetc(in);
	assert((c >= '0') && (c <= '9'));
	while (c != ' ') {
		num = num * 10;
		num = num + (c - '0');
		char c = fgetc(in);
		assert((c >= '0') && (c <= '9'));
	}
	return num;
}
void filltree(FILE *in, tree *root) {
	int i = 0;
	int amount;
	fgets(amount, 20, in);
	for (i; i < amount; i++) {
		int num = readnum(in);
		raitplace(num, root);
	}
}
int raitplace(int num, tree *root) {
	if (root != NULL) {
		int height;
		if (num >= root->num) {
			height = raitplace(num, root->right);
			height++;
			if (height > root->height) root->height = height;
			root = balancetree(root);
		}
		else {
			height = raitplace(num, root->left);
			height++;
			if (height > root->height) root->height = height;
			root = balancetree(root);
		}
	}
	else {
		tree *leaf = (tree*)malloc(sizeof(tree));
		leaf->num = num;
		leaf->height = 0;
		leaf->left = NULL;
		leaf->right = NULL;
		root = leaf;
	}
	return root->height;
}
int qb(tree *root) {
	if ((root->left->height - root->right->height) >= 2) return 1;
	if ((root->left->height - root->right->height) <= -2) return 1;
	else return 0;
}
tree *fixheight(tree *root) {
	if (root->left->height >= root->right->height) root->height = root->left->height + 1;
	else root->height = root->right->height + 1;
	return root;
}
tree *balancetree(tree *root) {
	int res = qb(root);
	if (res == 1) {
		if (root->right->height > root->left->height) {
			//right turn;
			if (root->right->right->height > root->right->left->height) {
				//right small
				tree *tmp = root->left;
				root->left = tmp->right;
				tmp->right = root;
				root = tmp;
				fixheight(root->left);
				fixheight(root);
				return root;
			}
			else {
				//right big
				tree *tmp = root->left->right;
				root->left->right = tmp->left;
				tmp->left = root->left;
				root->left = tmp;
				fixheight(root->left->right);
				fixheight(root->left);

				tree *tmp = root->left;
				root->left = tmp->right;
				tmp->right = root;
				root = tmp;
				fixheight(root->left);
				fixheight(root);

				return root;
			}
		}
		else {
			//left turn;
			if (root->left->left->height > root->left->right->height) {
				//left small
				tree *tmp = root->right;
				root->right = tmp->left;
				tmp->left = root;
				root = tmp;
				fixheight(root->right);
				fixheight(root);
				return root;
			}
			else {
				//left big
				tree *tmp = root->right->left;
				root->right->left = tmp->right;
				tmp->right = root->right;
				root->right = tmp;
				fixheight(root->right->left);
				fixheight(root->right);

				tree *tmp = root->right;
				root->right = tmp->left;
				tmp->left = root;
				root = tmp;
				fixheight(root->right);
				fixheight(root);

				return root;
			}
		}
	}
}
int main() {
	FILE *fin = fopen("in.txt", "r");
	FILE *fout = fopen("out.txt", "w");

	if (fin == NULL) 
		fprintf(fout, "File could not be opened");
	else {
		tree *root = NULL;
	
	}
	fclose(fin);
	fclose(fout);
	return 0;
}