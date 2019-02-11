#include <stdio.h>
#include <assert.h>
#include <malloc.h>
struct tree {
	char symbol;
	int usage;
	struct tree* left;
	struct tree* right;
};
typedef struct tree tree;

struct zipline {
	char buffer;
	FILE *fout;
	int bytescount;
};
typedef struct zipline zipline;

typedef struct textline {
	struct textline *next;
	tree *root;
} textline;

void read(FILE *fin, int alph[]){
	unsigned char symbol;
	while ((symbol = getc(fin)) != EOF){
		alph[symbol]++;
	}
}

void zip(FILE *fin, FILE *fout, int alph) {
	read(fin, alph);
	qsort(alph, 255, 0);
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
//void swap(textline *t1, textline *t2) {
//	tree tmp;
//	tmp = *t1->root;
//	t1->root = t2->root;     //??????????
//	*t2->root = tmp;
// }
//int treecreate(textline **text) {
//	int result;
//	if (isemptytext(*text) == 0) {
//		result = 1; //no elements in text
//	}
//	else{
//		if (issolotext(*text) == 0) {
//			result = 0; //1 element in text so tree is done 
//		}
//		else {
//			while (issolotext(*text) != 0) { //tree can be done
//				textline *tmp = (textline*)malloc(sizeof(textline));
//				assert(tmp != NULL);
//				if ((*text)->usage >= ((*text)->next)->usage) {
//					tmp->right = *text;
//					tmp->left = (*text)->next;
//				}
//				else {
//					tmp->right = (*text)->next;
//					tmp->left = *text;
//				}
//				tmp->symbol = (*text)->symbol;
//				tmp->next = (*text)->next->next;
//				free(*text);
//				free((*text)->next);
//				*text = tmp;
//				while (tmp->next != NULL) { // ???????????
//					if (&tmp->usage >= tmp->next->usage) swap(tmp, tmp->next);
//				}
//				free(tmp);
//			}
//			result = 0;
//		}
//	}
//	return result;
//}
void rightplace(textline *t) {
	if ((t->next != NULL)&(t->next->root->usage < t->root->usage)) {
		tree ptr;
		ptr = *t->next ->root;
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
int main() {
	FILE *fout = fopen("out.txt", "w");
	FILE *fin = fopen("in.txt", "r");
	if (fin == NULL)
		fprintf(fout, "File could not be opened.");
	else {
		int alph[256];
		char purp = fgetc(fin);
		if (purp == 'c') zip;
		if (purp == 'd') unzip;

	}
	return 0;
}