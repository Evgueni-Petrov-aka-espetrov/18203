#include <stdio.h>
#include <malloc.h>
#include <io.h>
#include <assert.h>
struct tree {
	struct tree *left;
	struct tree *right;
	char symbol;
	int usage;
};
typedef struct tree tree;
typedef struct textline {
	struct textline *next;
	tree *root;
} textline;
typedef struct table {
	struct table *next;
	char symbol;
	int bytes[256];
} table;
char wtd(FILE *fin) {
	char b = fgetc(fin);
	char c = fgetc(fin);
	return b;
}
int searchformax(int array[], int num, int *maxchar) {
	int i, max=0;
	*maxchar = 0;
	for (i = 0; i < num; i++) {
		if (array[i] > max) {
			max = array[i];
			*maxchar = i;
		}
	}
	array[*maxchar] = 0;
	return max;
}
void push(textline **head, int num, char letter) {
	textline *tmp = (textline*)malloc(sizeof(textline));
	tree *tr = (tree*)malloc(sizeof(tree));
	assert(tmp != NULL);
	assert(tr != NULL);
	tr->left = NULL;
	tr->right = NULL;
	tr->symbol = letter;
	tr->usage = num;
	tmp->root = tr;
	tmp->next = *head;
	*head = tmp;
	free(tr);
}
//char pop(textline **head) {
//	assert(*head != NULL);
//	char data = (*head)->root->symbol;
//	textline *tmp = (*head)->next;
//	free(*head);
//	*head = tmp;
//	return data;
//}
int isemptytext(textline *text) {
	if (text == NULL) return 0;
	else return 1;
}
int issolotext(textline *text) {
	int result;
	if (text->next == NULL) result = 0;
	else result = 1;
	return result;
}
void rightplace(textline *t) {
	if ((t->next != NULL)&(t->next->root->usage < t->root->usage)) {
		tree ptr;
		ptr = *t->next->root;
		*t->next->root = *t->root;
		*t->root = ptr;
		rightplace(t->next);
	}
}
void treecreate(textline *text) {
	while (issolotext(text) == 1) {
		tree tmp;
		if ((text)->root->usage >= ((text)->next)->root->usage) {
			tmp.right = (text)->root;
			tmp.left = (text)->next->root;
		}
		else {
			tmp.right = (text)->next->root;
			tmp.left = text->root;
		}
		tmp.usage = text->root->usage + text->next->root->usage;
		tmp.symbol = 0;
		text = text->next->next;
		*text->root = tmp;
		free(text->next);
		rightplace(text);
	}
}
int findsymbol(tree *root, char sym, int *counter, int array[]) {
	if (root->left != NULL) {
		array[*counter] = 0;
		*counter++;
		findsymbol(root->left, sym, counter, array);
	}
	if (root->right != NULL) {
		array[*counter] = 1;
		*counter++;
		findsymbol(root->right, sym, counter, array);
	}
	if ((root->left == NULL)&(root->right != NULL)) {
		if (root->symbol == sym) return 0;
	}
}
void keyencode(tree *root) {
	if ((root->left != NULL) | (root->right != NULL)) {
		printf("%d", 1);
		keyencode(root->left);
		keyencode(root->right);
	}
	else {
		printf("%d%c", 0, root->symbol);
	}
}
//void textencode(FILE *in, FILE *out, tree *root, int amount) {
//	char c = fgetc(in);
//	c = fgetc(in);
//	printf(out, "%d ", amount);
//	while ((c = fgetc(in)) != EOF) {
//
//	}
//
//}
void zip(FILE *fin, FILE *fout) {
	char c;
	textline *text = NULL;
	int alph[256] = {0};
	int amountofsymbols = 0;
	//reading to array
	while ((c = fgetc(fin)) != EOF) {
		alph[c]++;
		amountofsymbols++;
		//printf("%c & %d & %d    ", c, c, alph[c]);
	}
	//printf("%d ", alph[97]);
	//organizing stack from array
	int max;
	int maxchar = 0;
	max = searchformax(alph, 256, &maxchar);
	printf("%d %c   ", max, maxchar);
	while (max > 0) {
		char ch = maxchar;
		push(&text, max, ch);
		printf("%d %c   ", max, maxchar);
		max = searchformax(alph, 256, &maxchar);
	}
	printf("%c %c", text->next->root->symbol, text->root->symbol);
	//tree generate
	treecreate(text);
	printf("hfj");
	//tree encoding
	keyencode(text->root);
	printf("\n");
	//text encoding
	//fseek(fin, 0L, SEEK_SET);   /* Перейти в начало файла */
	//textencode(fin, fout, text->root, amountofsymbols);
}
int main() {
	FILE *fout = fopen("out.txt", "w");
	FILE *fin = fopen("in.txt", "r+");
	if (fin == NULL)
		fprintf(fout, "File could not be opened.");
	else {
		char res = wtd(fin);
		if (res == 'c') zip(fin, fout);
		//if (res == 'd') unzip(fin, fout);
	}
	return 0;
}