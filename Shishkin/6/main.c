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
	int minus = 1;
	int c = fgetc(in);
	//assert((c-48 >= 0) && (c-48 <= 9));
	while ((c != 32) && (c != 10)) {
		if (c == '-') minus = -1;
		else {
			num = num * 10;
			num = num + (c - 48);
		}
		c = fgetc(in);
		//assert((c >= '0') && (c <= '9'));
	}

	num = minus * num;
	//fprintf(stderr, "%d    ", num);
	return num;
}

int qb(tree *root) {
	int left, right;
	if (root->left == NULL) left = 0;
	else left = root->left->height;
	if (root->right == NULL) right = 0;
	else right = root->right->height;

	if ((left - right >= 2) || (left - right <= -2)) return 1;
	else return 0;
}
tree *fixheight(tree *root) {
	int left, right;
	
	if (root->left == NULL) left = 0;
	else left = root->left->height;
	
	if (root->right == NULL) right = 0;
	else right = root->right->height;
	
	if (left >= right) root->height = left + 1;
	else root->height = right + 1;
	
	return root;
}
tree *balancetree(tree *root) {
	int res = qb(root);
	if (res == 1) {
		int left, right;
		if (root->left == NULL) left = 0;
		else left = root->left->height;
		if (root->right == NULL) right = 0;
		else right = root->right->height;
		if (right < left) {
			//right turn;
			int left1, right1;
			if (root->left->right == NULL) right1 = 0;
			else right1 = root->left->right->height;
			if (root->left->left == NULL) left1 = 0;
			else left1 = root->left->left->height;
			if (right1 < left1) {
				//right small
				tree *tmp = root->left;
				root->left = tmp->right;
				tmp->right = root;
				root = tmp;
				fixheight(root->right);
				//fprintf(stderr, "%d  ", root->right->height);
				fixheight(root);

			}
			else {
				//right big
				tree *tmp = root->left->right;
				root->left->right = tmp->left;
				tmp->left = root->left;
				root->left = tmp;
				fixheight(root->left->left);
				fixheight(root->left);

				tmp = root->left;
				root->left = tmp->right;
				tmp->right = root;
				root = tmp;
				fixheight(root->right);
				//fprintf(stderr, "%d  ", root->left->height);
				fixheight(root);


			}
		}
		else {
			//left turn;
			int left1, right1;
			if (root->right->right == NULL) right1 = 0;
			else right1 = root->right->right->height;
			if (root->right->left == NULL) left1 = 0;
			else left1 = root->right->left->height;
			if (left1 <= right1) {
				//left small
				tree *tmp = root->right;
				root->right = tmp->left;
				tmp->left = root;
				root = tmp;
				fixheight(root->left);
				fixheight(root);

			}
			else {
				//left big
				tree *tmp = root->right->left;
				root->right->left = tmp->right;
				tmp->right = root->right;
				root->right = tmp;
				fixheight(root->right->right);
				fixheight(root->right);

				tmp = root->right;
				root->right = tmp->left;
				tmp->left = root;
				root = tmp;
				fixheight(root->left);
				fixheight(root);


			}
		}
	}
	return root;
}
tree *raitplace(int num, int *height, tree *root) {
	if (root != NULL) {

		if (num >= root->num) {
			root->right = raitplace(num, height, root->right);
			*height = *height + 1;
			if (*height > root->height) root->height = *height;
			root = balancetree(root);
		}
		else {
			//printf("1");
			root->left = raitplace(num, height, root->left);
			//printf("%d   ", root->height);
			*height = *height + 1;
			//printf("%d   ", *height);
			if (*height > root->height) root->height = *height;
			//printf("%d   ", root->height);
			root = balancetree(root);
		}
	}
	else {
		tree *leaf = (tree*)malloc(sizeof(tree));
		leaf->num = num;
		leaf->height = 1;
		*height = 1;
		leaf->left = NULL;
		leaf->right = NULL;
		root = leaf;
	}
	return root;
}
tree *filltree(FILE *in, tree *root) {
	int i = 0;
	int amount = 0;
	int c = fgetc(in);
	while (c != 10){
		//printf("%d  ", c);
		amount = amount * 10 + (c - 48);
		c = fgetc(in);
	}
	//printf("%d", amount);
	int height = 0;
	for (i; i < amount; i++) {

		int num = readnum(in);
		//printf("%d ", num);
		root = raitplace(num, &height, root);
	}
	//printf("   ---%d---  ", root->num);
	return root;
}

int main() {
	FILE *fin = fopen("in.txt", "r");
	FILE *fout = fopen("out.txt", "w");

	if (fin == NULL)
		fprintf(fout, "File could not be opened");
	else {
		tree *root = NULL;
		root = filltree(fin, root);
		if (root == NULL) fprintf(fout, "0");
		else fprintf(fout, "%d", root->height);
	}
	fclose(fin);
	fclose(fout);
	return 0;
}