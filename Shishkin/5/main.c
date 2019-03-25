#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC  
#include <crtdbg.h>
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
void freetree(tree *root){
	if (root != NULL){
		freetree(root->right);
		freetree(root->left);
		free(root);
	}
}
char wtd(FILE *fin) {
	char b = fgetc(fin);
	char c = fgetc(fin);
	c = fgetc(fin);
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
	return tmp;
}

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
		tree *tmp = (tree*)malloc(sizeof(tree));
		textline *text1 = (textline*)malloc(sizeof(textline));
		assert(tmp != NULL);
		assert(text1 != NULL);
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
		free(text->next);
		free(text);
		text = text1;
		rightplace(text);
	}
	tree *ptr = text->root;
	free(text);
	return (ptr);
}
int findsymbol(tree *root, char sym, int *counter, int array[]) {
	int l_res = 1, r_res = 1;
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
	if ((l_res == 0) || (r_res == 0)) return 0;
	else return 1;
}
void keyencode(tree *root, FILE *out, unsigned char *ch, int *counter) {
	if (*counter > 7){
		fprintf(out, "%c", *ch);
		*ch = 0;
		*counter = 0;
	}
	if ((root->left != NULL) || (root->right != NULL)) {
		*ch = *ch << 1;
		*ch = *ch + 1;
		*counter = *counter + 1;
		keyencode(root->left, out, ch, counter);
		keyencode(root->right, out, ch, counter);
	}
	else {
		*ch = *ch << 1;
		*counter = *counter + 1;
		int i;
		for (i = 7; i >= 0; i--){
			int byte = (root->symbol >> i) & 1;
			if (*counter > 7){
				fprintf(out, "%c", *ch);
				*ch = 0;
				*counter = 0;
			}
			*ch = *ch << 1;
			*ch = *ch + byte;
			*counter = *counter + 1;
		}
	}
}
void printnum(int num, FILE *out){
	unsigned char c = 0;
	int i = 0;
	do {
		int d = num % 2;
		num = (num - d) / 2;
		c = c + d;
		i++;
		if (i > 7){
			fprintf(out, "%c", c);
			c = 0;
			i = 0;
		}
		else c = c << 1;
	} while (num > 0);
	c = c << (7 - i);
	fprintf(out, "%c", c);
	fprintf(stderr, "%d", c);
}
void textencode(FILE *in, FILE *out, tree *root) {
	char c = fgetc(in);
	c = fgetc(in);
	c = fgetc(in);
	fprintf(stderr, "%d ", root->usage);
	fwrite(&root->usage, sizeof(int), 1, out);
	int z = ftell(out);
	fprintf(stderr, "    %d     ", z);
	unsigned char buf = 0;
	int buf_sym = 0;
	int i;
	for (i = 0; i < root->usage; i++){
		c = fgetc(in);
		int array[256] = { 0 };
		int counter = 0;
		int res = findsymbol(root, c, &counter, array);
		int i;
		for (i = counter - 1; i >= 0; i--) {
			buf = buf + (array[i]);
			buf_sym++;
			if (buf_sym > 7) {
				fprintf(out, "%c", buf);
				buf = buf << 8;
				buf_sym = 0;
			}
			else buf = buf << 1;
		}
	}
	if (buf_sym > 0) {
		buf = buf << (7 - buf_sym);
		fprintf(out, "%c", buf);
	}
	z = ftell(out);
	fprintf(stderr, "    %d     ", z);
}
void zip(FILE *fin, FILE *fout) {
	int c;
	textline *text = NULL;
	int alph[256] = { 0 };
	c = fgetc(fin);
	if (c != EOF){
		fseek(fin, 0L, SEEK_SET);
		c = fgetc(fin);
		c = fgetc(fin);
		c = fgetc(fin);
		while ((c = fgetc(fin)) != EOF) {
			alph[c]++;
		}
		int max;
		int maxchar = 0;
		max = searchformax(alph, 256, &maxchar);
		while (max > 0) {
			char ch = maxchar;
			text = push(text, max, ch);
			max = searchformax(alph, 256, &maxchar);
		}
		tree *treePtr = treecreate(text);
		unsigned char ch = 0;
		int counter = 0;
		int z = ftell(fout);
		fprintf(stderr, "    %d     ", z);
		keyencode(treePtr, fout, &ch, &counter);
		ch = (ch << (8 - counter));
		fprintf(fout, "%c", ch);
		z = ftell(fout);
		fprintf(stderr, "    %d     ", z);
		fseek(fin, 0L, SEEK_SET);   /* move to start of fin */
		textencode(fin, fout, treePtr);
		z = ftell(fout);
		fprintf(stderr, "    %d     ", z);
		freetree(treePtr);
	}

}
void keydecode(FILE *in, tree *root, unsigned char *ch, int *counter) {
	if (*counter < 0) {
		*ch = fgetc(in);
		*counter = 7;
	}
	if (((*ch >> *counter) & 1) == 0) {
		*counter = *counter - 1;
		int i;
		unsigned char sym = 0;
		for (i = 0; i < 8; i++) {
			if (*counter < 0) {
				*ch = fgetc(in);
				*counter = 7;
			}
			int byte = ((*ch >> *counter) & 1);
			sym = sym << 1;
			sym = sym + byte;
			*counter = *counter - 1;
		}
		root->symbol = sym;
		root->left = NULL;
		root->right = NULL;
	}
	else {
		*counter = *counter - 1;
		tree *tmpl = (tree*)malloc(sizeof(tree));
		tree *tmpr = (tree*)malloc(sizeof(tree));
		assert(tmpl != NULL);
		assert(tmpr != NULL);
		root->left = tmpl;
		root->right = tmpr;
		keydecode(in, root->left, ch, counter);
		keydecode(in, root->right, ch, counter);
	}
}
int pow2(int deg){
	int res = 1;
	int i;
	for (i = 1; i <= deg; i++){
		res = res * 2;
	}
	return res;
}
int scannum(FILE *in){
	unsigned char ch = 0;
	int res = 0;
	int pow = 1;
	ch = fgetc(in);
	int i;
	for (i = 0; i <= 7; i++){
		res = res + ((ch >> (7 - i)) & 1)*pow;
		fprintf(stderr, "%d  ", res);
		pow = pow * 2;
	}
	ch = fgetc(in);
	while (ch != ' ') {
		i;
		for (i = 0; i <= 7; i++){
			res = res + ((ch >> (7 - i)) & 1)*pow;
			fprintf(stderr, "%d  ", res);
			pow = pow * 2;
		}
		ch = fgetc(in);
	}
	return res;
}
void textdecode(tree *root, FILE *out, FILE *in) {
	int lengthoftext;
	fread(&lengthoftext, sizeof(int), 1, in);
	fprintf(stderr, "  %d   ", lengthoftext);

	unsigned char c = fgetc(in);
	int i = 0;
	int counter = 7;
	tree *tmp = root;
	for (i; i < lengthoftext; i++) {
		tree *tmp = root;
		while ((tmp->left != NULL) && (tmp->right != NULL)) {
			if (counter < 0) {
				counter = 7;

				c = fgetc(in);
			}
			int byte = (c >> counter) & 1;
			counter--;
			if (byte == 0) tmp = tmp->left;
			if (byte == 1) tmp = tmp->right;
		}
		fprintf(out, "%c", tmp->symbol);
	}
}
void unzip(FILE *in, FILE *out) {
	tree *root = (tree*)malloc(sizeof(tree));
	assert(root != NULL);
	int c = fgetc(in);
	if (c != EOF) {
		fseek(in, 0L, SEEK_SET);
		c = fgetc(in);
		c = fgetc(in);
		c = fgetc(in);
		int counter = 7;
		unsigned char ch = fgetc(in);
		keydecode(in, root, &ch, &counter);
		textdecode(root, out, in);
		fprintf(stderr, "\n");
		freetree(root);
	}
	else free(root);
}
int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);

	FILE *fout = fopen("out.txt", "wb");
	FILE *fin = fopen("in.txt", "rb");
	if (fin == NULL)
		fprintf(fout, " File could not be opened. \n");
	else {
		char res = wtd(fin);
		if (res == 'c') zip(fin, fout);
		if (res == 'd') unzip(fin, fout);
	}
	fclose(fin);
	fclose(fout);
	return 0;
}
