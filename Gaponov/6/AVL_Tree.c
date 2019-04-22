#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>


struct tree {
	int element;
	int height;
	struct tree* left;
	struct tree* right;
};

struct tree* create_root(int key) {
	struct tree* root = malloc(sizeof(struct tree));
	root->element = key;
	root->height = 1;
	root->left = NULL;
	root->right = NULL;
	return root;
}

int get_height(struct tree *root) {
	if (root != NULL)
		return root->height;
	else
		return 0;
}

void fix_height(struct tree *root) {
	int height_left = get_height(root->left);
	int height_right = get_height(root->right);
	root->height = (height_left>height_right?height_left:height_right) + 1;
}

int balance_factor(struct tree *root) {
	if (root != NULL)
		return get_height(root->right) - get_height(root->left);
	else
		return 0;
}

struct tree* rotate_left(struct tree* root) {
	struct tree *new_root = root->right;
	root->right = new_root->left;
	new_root->left = root;
	fix_height(root);
	fix_height(new_root);
	return new_root;
}

struct tree* rotate_right(struct tree* root) {
	struct	tree *new_root = root->left;
	root->left = new_root->right;
	new_root->right = root;
	fix_height(root);
	fix_height(new_root);
	return new_root;
}

struct tree* balance_tree(struct tree* root) {
	fix_height(root);
	if (balance_factor(root) == 2) {
		if (balance_factor(root->right) < 0) 
			root->right = rotate_right(root->right);
		return rotate_left(root);
	}
	if (balance_factor(root) == -2) {
		if (balance_factor(root->left) > 0) 
			root->left = rotate_left(root->left);
		return rotate_right(root);
	}
	return root;
}

struct tree* insert(struct tree* root, int key) {
	if (root == NULL) 
		return create_root(key);
	if (key < root->element) 
		root->left = insert(root->left, key);
	else 
		root->right = insert(root->right, key);
	return balance_tree(root);
}

void free_tree(struct tree* root) {
	if (root->left != NULL)
		free_tree(root->left);
	if (root->right != NULL)
		free_tree(root->right);
	free(root);
}

int main() {
	FILE *input, *output;
	input = fopen("in.txt", "r");
	output = fopen("out.txt", "w");
	int tree_height = 0;
	int amount_of_vertices = 0;
	int element = 0;
	struct tree* tree = NULL;
	fscanf(input, "%d", &amount_of_vertices);
	for (int i = 0; i < amount_of_vertices; i++) {
		fscanf(input, "%d", &element);
		tree = insert(tree, element);
	}
	tree_height = get_height(tree);
	if (amount_of_vertices != 0) {
		free_tree(tree);
	}
	fprintf(output, "%d", tree_height);
	fclose(input);
	fclose(output);
}
