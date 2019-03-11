#define _CRT_SECURE_NO_WARNINGS


#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>

 struct treee{
	int element;
	int height;
	struct treee* left;
	struct treee* right;
};

 struct treee* create_root(int key) {
	 
	struct treee* root=malloc(sizeof(struct treee));
	root->element = key;
	root->height = 1;
	root->left = NULL;
	root->right = NULL;
	return root;
}

int get_height(struct treee *root) {
	if (root != NULL) {
		return root->height;
	}
	else {
		return 0;

	}
}

int find_balance_factor(struct treee *root) {
	if (root != NULL){
		return get_height(root->right) - get_height(root->left);
	}
	else{
		return 0;
	}
}

int fix_height(struct treee *root) {
	int left_height = get_height(root->left);
	int right_height = get_height(root->right);
	if (left_height > right_height) {
		root->height = left_height + 1;
	}
	else {
		root->height = right_height + 1;
	}
}

struct treee* rotate_to_right(struct treee *root) {
	struct	treee *new_root = root->left;
	root->left = new_root->right;
	new_root->right = root;
	fix_height(root);
	fix_height(new_root);
	return new_root;
}

struct treee* rotate_to_left(struct treee *root) {
	struct treee *new_root = root->right;
	root->right= new_root->left;
	new_root->left = root;
	fix_height(root);
	fix_height(new_root);
	return new_root;
}

struct treee* balance_tree(struct treee* root) {
	fix_height(root);
	
		if (find_balance_factor(root) == 2) {
			if (find_balance_factor(root->right) < 0) {
				root->right = rotate_to_right(root->right);
			}
			return rotate_to_left(root);
		}
		if (find_balance_factor(root) == -2) {
			if (find_balance_factor(root->left) > 0) {
				root->left = rotate_to_left(root->left);
			}
			return rotate_to_right(root);
		}
	
	return root;
}

struct treee* insert(struct treee* root, int key) {
	if (root == NULL) {
		return create_root(key);
	}
	if (key < root->element) {
		root->left = insert(root->left, key);
	}
	else {
		root->right = insert(root->right, key);
	}
	return balance_tree(root);
}

void delete_tree(struct treee* root) {
	if (root->left != NULL) {
		delete_tree(root->left);
	}
	if (root->right != NULL) {
		delete_tree(root->right);
	}
	free(root);
}

int main() {
	FILE *fin, *fout;
	fin = fopen("in.txt", "r");
	fout = fopen("out.txt", "w");
	int tree_height = 0, number_of_elements = 0, element = 0;
	struct treee* tree = NULL;
	fscanf(fin, "%d", &number_of_elements);
	for (int i = 0; i < number_of_elements; i++) {
		fscanf(fin, "%d", &element);
		tree = insert(tree, element);
	}
	tree_height = get_height(tree);
	delete_tree(tree);
	fprintf(fout,"%d",tree_height);
	tree_height++;
	fclose(fin);
	fclose(fout);
}