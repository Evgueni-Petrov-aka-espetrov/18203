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
	char symbol;
	int usage;
	struct textline *next;
	struct textline *right;
	struct textline *left;
} textline;

void read(FILE *fin, int alph[]){
	unsigned char symbol;
	while ((symbol = getc(fin)) != EOF){
		alph[symbol]++;
	}
}

void qsort(int array[], int finish, int start) {
	int firstpos, lastpos, pivot, t;
	firstpos = start;
	lastpos = finish;
	pivot = array[(finish + start + 1) / 2];
	do {
		while (array[firstpos] < pivot) firstpos++;
		while (array[lastpos] > pivot) lastpos--;
		if (firstpos <= lastpos) {
			t = array[firstpos];
			array[firstpos] = array[lastpos]; 						
			array[lastpos] = t;
			firstpos++;
			lastpos--;
		}
	} while (firstpos <= lastpos);
	if (lastpos > start){
		qsort(array, lastpos, start);									
	}
	if (finish > firstpos){
		qsort(array, finish, firstpos);
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
void swap(textline *t1, textline **t2) {
	textline *tmp = (textline*)malloc(sizeof(textline));
	assert(tmp != NULL);
	tmp = t1;
	t1 = *t2;     //??????????
	*t2 = tmp;
	free(tmp); 
}
int treecreate(textline **text) {
	int result;
	if (isemptytext(*text) == 0) {
		result = 1; //no elements in text
	}
	else{
		if (issolotext(*text) == 0) {
			result = 0; //1 element in text so tree is done 
		}
		else {
			while (issolotext(*text) != 0) { //tree can be done
				textline *tmp = (textline*)malloc(sizeof(textline));
				assert(tmp != NULL);
				if ((*text)->usage >= ((*text)->next)->usage) {
					tmp->right = *text;
					tmp->left = (*text)->next;
				}
				else {
					tmp->right = (*text)->next;
					tmp->left = *text;
				}
				tmp->symbol = (*text)->symbol;
				tmp->next = (*text)->next->next;
				free(*text);
				free((*text)->next);
				*text = tmp;
				while (tmp->next != NULL) { // ???????????
					if (&tmp->usage >= tmp->next->usage) swap(tmp, tmp->next);
				}
				free(tmp);
			}
			result = 0;
		}
	}
	return result;
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