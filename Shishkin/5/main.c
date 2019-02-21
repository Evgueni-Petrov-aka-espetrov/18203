#include <stdio.h>
#include <malloc.h>
#include <io.h>
#include <assert.h>
#include <string.h>
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
typedef struct stack {
	struct table *next;
	int num;
} table;
char wtd(FILE *fin) {
	char b = fgetc(fin);
	char c = fgetc(fin);
	return b;
}
int searchformax(int array[], int num, int *maxchar) {
	int i, max = 0;
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
textline *push(textline *head, int num, char letter) {
	textline *tmp = (textline*)malloc(sizeof(textline));
	tree *tr = (tree*)malloc(sizeof(tree));
	assert(tmp != NULL);
	assert(tr != NULL);
	tr->left = NULL;
	tr->right = NULL;
	tr->symbol = letter;
	tr->usage = num;
	tmp->root = tr;
	tmp->next = head;
	//free(tr);
	//printf("%d %c", tmp->root->usage, tmp->root->symbol);
	return tmp;
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
void rightplace(textline *text) {
	if (text->next != NULL) {
		if (text->next->root->usage < text->root->usage) {
			//printf("   %d --> %d   ", text->root->usage, text->next->root->usage);
			tree *ptr = NULL;
			ptr = text->next->root;
			text->next->root = text->root;
			text->root = ptr;
			rightplace(text->next);
		}
	}
}
tree *treecreate(textline *text) {
	while (issolotext(text) == 1) {
		//printf("F");
		tree *tmp = (tree*)malloc(sizeof(tree));
		textline *text1 = (textline*)malloc(sizeof(textline));
		assert(tmp != NULL);
		assert(text1 != NULL);
		//printf(" %c ", text->root->symbol);
		if ((text)->root->usage >= ((text)->next)->root->usage) {
			tmp->left = (text)->root;
			tmp->right = (text)->next->root;
		}
		else {
			tmp->left = (text)->next->root;
			tmp->right = text->root;
		}
		tmp->usage = text->root->usage + text->next->root->usage;
		tmp->symbol = 0;
		text1->next = text->next->next;
		text1->root = tmp;
		//printf(" '%d' ", text1->root->right->usage);
		free(text->next);
		free(text);
		text = text1;
		//printf(" %c ", text->next->root->symbol);
		rightplace(text);
		//printf("  %d  ", text->root->usage);
	}
	return (text->root);
}
int findsymbol(tree *root, char sym, int *counter, int array[]) {
	//printf("%d  ", *counter);
	int l_res = 1, r_res = 1;
	//while ((r_res == 1) && (l_res == 1)) {
	if (root->left != NULL) {
		l_res = findsymbol(root->left, sym, counter, array);
		if (l_res == 0) {
			array[*counter] = 0;
			*counter = *counter + 1;
		}
	}
	if (root->right != NULL) {
		r_res = findsymbol(root->right, sym, counter, array);
		if (r_res == 0) {
			array[*counter] = 1;
			//printf(" %d ", array[*counter]);
			*counter = *counter + 1;
		}
	}
	if ((root->left == NULL) && (root->right == NULL)) {
		if (root->symbol == sym) {
			l_res = 0;
			r_res = 0;
		}
		else {
			l_res = 1;
			r_res = 1;
		}
	}
	//}
	//printf("%d ", array[*counter]);
	if ((l_res == 0) || (r_res == 0)) return 0;
	else return 1;
}
void keyencode(tree *root, FILE *out) {
	if ((root->left != NULL) || (root->right != NULL)) {
		fprintf(out, "1");
		keyencode(root->left, out);
		keyencode(root->right, out);
	}
	else {
		fprintf(out, "0%c", root->symbol);
	}
}
void textencode(FILE *in, FILE *out, tree *root) {
	char c = fgetc(in);
	c = fgetc(in);
	fprintf(out, "%d ", root->usage);
	unsigned char buf = 0;
	int buf_sym = 0;
	while ((c = fgetc(in)) != EOF) {
		//printf("'%c'  ", c);
		int array[256] = { 0 };
		int counter = 0;
		int res = findsymbol(root, c, &counter, array);
		int i;
		for (i = counter - 1; i >= 0; i--) {
			//fprintf(out, "%d", array[i]);			writting code in bytes
			buf = buf + (array[i]);
			buf_sym++;
			if (buf_sym > 7) {
				//printf("%d ", buf);
				fprintf(out, "%c", buf);
				buf = buf << 8;
				buf_sym = 0;
			}
			else buf = buf << 1;

		}
	}
	buf = buf << (7 - buf_sym);
	//printf("%d ", buf);
	fprintf(out, "%c", buf);
}
void zip(FILE *fin, FILE *fout) {
	char c;
	textline *text = NULL;
	int alph[256] = { 0 };
	//int amountofsymbols = 0;
	//reading to array
	c = fgetc(fin);
	if (c != EOF){
		fprintf(stderr, " 0 ");
		fseek(fin, 0L, SEEK_SET);
		c = fgetc(fin);
		c = fgetc(fin);
		while ((c = fgetc(fin)) != EOF) {
			alph[c]++;
			//amountofsymbols++;
			//printf("%c & %d & %d    ", c, c, alph[c]);
		}
		//printf("%d ", alph[97]);
		//organizing stack from array
		int max;
		int maxchar = 0;
		max = searchformax(alph, 256, &maxchar);
		//printf("%d %c   ", max, maxchar);
		while (max > 0) {
			char ch = maxchar;
			text = push(text, max, ch);
			//printf("%d %c   ", max, maxchar);
			max = searchformax(alph, 256, &maxchar);
		}
		//printf("'%c' 123 '%c'", text->root->symbol, text->next->root->symbol);
		//tree generate
		tree *treePtr = treecreate(text);
		//printf(" %d ");
		//tree encoding
		//fprintf(fout, "%d ", treePtr->usage);
		keyencode(treePtr, fout);
		//printf("tree code is ready");
		fprintf(fout, "\n");
		//text encoding
		fseek(fin, 0L, SEEK_SET);   /* move to start of fin */
		//char h = fgetc(fin);
		//printf("%d and '%c'  ", treePtr->right->right->left->usage, treePtr->right->right->left->symbol);
		textencode(fin, fout, treePtr);
	}
	else fprintf(stderr, " 1 ");
}
void keydecode(FILE *in, tree *root) {
	unsigned char c = fgetc(in);
	//printf(" %c ", c);
	//if (c == '\n') return c;
	//else {
	if (c == '1') {
		tree *tmpl = (tree*)malloc(sizeof(tree));
		tree *tmpr = (tree*)malloc(sizeof(tree));
		assert(tmpl != NULL);
		assert(tmpr != NULL);
		root->left = tmpl;
		root->right = tmpr;
		keydecode(in, root->left);
		keydecode(in, root->right);
	}
	if (c == '0') {
		c = fgetc(in);
		root->symbol = c;
		//printf(" '%c' ", c);
		root->left = NULL;
		root->right = NULL;
	}
	//}
	//return c;
}
void textdecode(tree *root, FILE *out, FILE *in) {
	unsigned char c = fgetc(in);
	c = fgetc(in);
	//printf("'%c' ", c);
	int lengthoftext = 0;
	while (c != ' ') {
		//printf("'%c' ", c);
		int r = c - 48;
		lengthoftext = (lengthoftext * 10) + r;
		c = fgetc(in);
	}
	//printf("%d", lengthoftext);
	c = fgetc(in);
	int i = 0;
	int counter = 7;
	//printf("%d", c);
	tree *tmp = root;
	//printf("%c", root->left->right->symbol);
	for (i; i < lengthoftext; i++) {
		tree *tmp = root;
		while ((tmp->left != NULL) && (tmp->right != NULL)) {
			if (counter < 0) {
				counter = 7;

				c = fgetc(in);
				printf("%d", c);
			}
			int byte = (c >> counter) & 1;
			printf(" %d --> ", byte);
			counter--;
			if (byte == 0) tmp = tmp->left;
			if (byte == 1) tmp = tmp->right;
		}
		printf("'%c'", tmp->symbol);
		fprintf(out, "%c", tmp->symbol);
	}
}
void unzip(FILE *in, FILE *out) {
	tree *root = (tree*)malloc(sizeof(tree));
	assert(root != NULL);
	char c = fgetc(in);
	if (c != EOF) {
		fprintf(stderr, " 0 ");
		fseek(in, 0L, SEEK_SET);
		c = fgetc(in);
		c = fgetc(in);
		keydecode(in, root);
		//printf("'%c'", root->left->right->symbol);
		textdecode(root, out, in);
	}
	else fprintf(stderr, " 1 ");
}
int main() {
	FILE *fout = fopen("out.txt", "w");
	FILE *fin = fopen("in.txt", "r+");
	//fprintf(stderr, " 2 ");
	if (fin == NULL)
		fprintf(fout, "File could not be opened.");
	else {
		fprintf(stderr, " 1 \n");
		char res = wtd(fin);
		if (res == 'c') zip(fin, fout);
		if (res == 'd') unzip(fin, fout);
	}
	return 0;
}