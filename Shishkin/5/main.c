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
		//printf(" '%c' ", text->root->symbol);
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
void keyencode(tree *root, FILE *out, unsigned char *ch, int *counter) {
	if (*counter > 7){
		//printf(" %d ", *ch);
		fprintf(out, "%c", *ch);
		//printf("   ");
		*ch = 0;
		*counter = 0;
		
	}
	if ((root->left != NULL) || (root->right != NULL)) {
		
		*ch = *ch << 1;
		*ch = *ch + 1;
		//printf("1");
		*counter = *counter + 1;
		//printf(" %d and %d ---> ", *ch, *counter);
		keyencode(root->left, out, ch, counter);
		keyencode(root->right, out, ch, counter);
	}
	else {
		
		*ch = *ch << 1;
		//printf("0");
		*counter = *counter + 1;
		//printf(" %d and %d ---> ", *ch, *counter);
		int i;
		//printf(" '%c' ", root->symbol);
		for (i = 7; i >= 0; i--){
			
			int byte = (root->symbol >> i) & 1;
			if (*counter > 7){
				//printf(" %d ", *ch);
				fprintf(out, "%c", *ch);
				//printf("   ");
				*ch = 0;
				*counter = 0;
			}
			
			//printf("%d", byte);
			*ch = *ch << 1;
			*ch = *ch + byte;
			*counter = *counter + 1;
			//printf(" %d and %d ---> ", *ch, *counter);
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
	//fprintf(stderr, " '%d' ", c);
	c = fgetc(in);
	c = fgetc(in);
	//fprintf(stderr, " '%d' ", c);
	fprintf(stderr, "%d ", root->usage);
	//printnum(root->usage, out);
	fwrite(&root->usage, sizeof(int), 1, out);
	int z = ftell(out);
	fprintf(stderr, "    %d     ", z);
	//fprintf(out, " ");
	unsigned char buf = 0;
	int buf_sym = 0;
	int i;
	//c = fgetc(in);
	//fprintf(stderr, " '%d' ", c);
	//while (c != EOF) {
	for (i = 0; i < root->usage; i++){
		c = fgetc(in);
		//fprintf(stderr, " '%d' ", c);
		//printf("'%c'  ", c);
		int array[256] = { 0 };
		int counter = 0;
		//unsigned char ch = c;
		int res = findsymbol(root, c, &counter, array);
		int i;
		for (i = counter - 1; i >= 0; i--) {
			//fprintf(out, "%d", array[i]);			writting code in bytes
			buf = buf + (array[i]);
			buf_sym++;
			if (buf_sym > 7) {
				//printf("%d ", buf);
				fprintf(out, "%c", buf);
				//int z1 = ftell(out);
				//fprintf(stderr, "    %d     ", z1);
				buf = buf << 8;
				buf_sym = 0;
			}
			else buf = buf << 1;

		}
		//c = fgetc(in);
		//fprintf(stderr, " '%d' ", c);

	}
	if (buf_sym > 0) {
		buf = buf << (7 - buf_sym);
		//printf("%d ", buf);
		fprintf(out, "%c", buf);
	}
	z = ftell(out);
	fprintf(stderr, "    %d     ", z);
}
void zip(FILE *fin, FILE *fout) {
	int c;
	textline *text = NULL;
	int alph[256] = { 0 };
	//int amountofsymbols = 0;
	//reading to array
	c = fgetc(fin);
	if (c != EOF){
		//fprintf(res, " 0 \n");
		fseek(fin, 0L, SEEK_SET);
		c = fgetc(fin);
		//fprintf(stderr, "'%d'", c);
		c = fgetc(fin);
		c = fgetc(fin);
		//fprintf(stderr, "'%d'", c);
		while ((c = fgetc(fin)) != EOF) {
			//fprintf(stderr, "'%d'", c);
			alph[c]++;
			//amountofsymbols++;
			//printf("%c & %d & %d    ", c, c, alph[c]);
		}
		//fprintf(stderr, "'%d'", c);
		//fprintf(stderr, "\n");
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
		unsigned char ch = 0;
		int counter = 0;
		//printf(" '%c' ", treePtr->left->right->symbol);
		int z = ftell(fout);
		fprintf(stderr, "    %d     ", z);
		keyencode(treePtr, fout, &ch, &counter);
		ch = (ch << (8 - counter));
		//printf(" %d ", ch);
		fprintf(fout, "%c", ch);
		z = ftell(fout);
		fprintf(stderr, "    %d     ", z);
		//printf(" '%d' ", treePtr->right->usage);
		//fprintf(fout, "\r\n");
		//text encoding
		fseek(fin, 0L, SEEK_SET);   /* move to start of fin */
		//char h = fgetc(fin);
		//printf("%d and '%c'  ", treePtr->right->right->left->usage, treePtr->right->right->left->symbol);
		//printf("1");
		textencode(fin, fout, treePtr);
		z = ftell(fout);
		fprintf(stderr, "    %d     ", z);
		//printf("text cade is ready");
	}
	//else fprintf(res, " 1 \n");
}
void keydecode(FILE *in, tree *root, unsigned char *ch, int *counter) {
	//unsigned char c = fgetc(in);
	//printf(" %c ", c);
	//if (c == '\n') return c;
	//else {
	//printf(" '%d' ", *ch);
	if (*counter < 0) {
		*ch = fgetc(in);
		*counter = 7;
//		printf("     ");
	}
	if (((*ch >> *counter) & 1) == 0) {
//		printf(" 0 ");
		*counter = *counter - 1;
		int i;
		unsigned char sym = 0;
		for (i = 0; i < 8; i++) {
			if (*counter < 0) {
				*ch = fgetc(in);
				*counter = 7;
//				printf("     ");
			}
			int byte = ((*ch >> *counter) & 1);
//			printf("'%d'", byte);
			sym = sym << 1;
			sym = sym + byte;

			*counter = *counter - 1;
		}
		root->symbol = sym;
//		printf(" '%d' ", sym);
		root->left = NULL;
		root->right = NULL;
	}
	//if (((*ch >> *counter) & 1) == 1) {
	else {
//		printf(" 1 ");
		*counter = *counter - 1;
		tree *tmpl = (tree*)malloc(sizeof(tree));
		tree *tmpr = (tree*)malloc(sizeof(tree));
		assert(tmpl != NULL);
		assert(tmpr != NULL);
		
		
		root->left = tmpl;
		
		root->right = tmpr;
//		printf(" L ");
		keydecode(in, root->left, ch, counter);
//		printf(" R ");
		keydecode(in, root->right, ch, counter);
	}

	//}
	//return c;
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
	//unsigned char c = fgetc(in);
	//printf("'%d' ", c);
	//c = fgetc(in);
	//printf("'%d' ", c);
	//c = fgetc(in);
	//printf("'%d' ", c);
//	int lengthoftext = scannum(in);
	int lengthoftext;
	fread(&lengthoftext, sizeof(int), 1, in);
	//while (c != ' ') {
	//	//printf("'%c' ", c);
	//	int r = c - 48;
	//	lengthoftext = (lengthoftext * 10) + r;
	//	c = fgetc(in);
	//}
	fprintf(stderr, "  %d   ", lengthoftext);

	unsigned char c = fgetc(in);
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
				//printf("%d", c);
			}
			int byte = (c >> counter) & 1;
			//printf(" %d --> ", byte);
			counter--;
			if (byte == 0) tmp = tmp->left;
			if (byte == 1) tmp = tmp->right;
		}
		//fprintf(stderr, " '%d' ", tmp->symbol);
		//fprintf(stderr, "fghj");
		fprintf(out, "%c", tmp->symbol);
	}
}
void unzip(FILE *in, FILE *out) {
	tree *root = (tree*)malloc(sizeof(tree));
	assert(root != NULL);
	int c = fgetc(in);
	if (c != EOF) {
		//fprintf(stderr, " 0 ");
		fseek(in, 0L, SEEK_SET);
		c = fgetc(in);
		c = fgetc(in);
		c = fgetc(in);
		int counter = 7;
		unsigned char ch = fgetc(in);
		//printf("123");
		keydecode(in, root, &ch, &counter);
		//printf("11313435");
		//printf("'%с' \n", root->right->symbol);
		textdecode(root, out, in);
		fprintf(stderr, "\n");
	}
	//else fprintf(res, " 1 \n");
}
int main() {
	//FILE *fout0 = fopen("in.txt", "wb");
	//fprintf(fout0, "c\r\n");
	//int i, j;
	//for (i = 0; i < 256; i++) {
	//	for (j = 0; j < 256; j++) {
	//		fprintf(fout0, "%c", j);
	//	}
	//}
	//fclose(fout0);

	FILE *fout = fopen("out.txt", "wb");
	FILE *fin = fopen("in.txt", "rb");
	if (fin == NULL)
		fprintf(fout, " File could not be opened. \n");
	else {
		//		fprintf(result, " 1 \n");
		//fprintf(fout, "c\n\n\r\n\r");
		char res = wtd(fin);
		//fprintf(fout, "d\r\n");
		if (res == 'c') zip(fin, fout);
		if (res == 'd') unzip(fin, fout);
		//zip(fin, fout);
	}
	fclose(fin);
	fclose(fout);

	//FILE *fout2 = fopen("out2.txt", "wb");
	//FILE *fin2 = fopen("out.txt", "rb");
	//if (fin2 == NULL)
	//	fprintf(fout2, " File could not be opened. \r\n");
	//else {
	//	 printf("\n\n\n\n\n\n\n");
	//	 char res = wtd(fin2);
	//	 unzip(fin2, fout2);
	//}
	//fclose(fin2);
	//fclose(fout2);
	return 0;
}