#define CRT_NO_SECURE_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>

typedef enum {false,true} bool;

struct treee {
	int height;
	unsigned char symbol;
	struct treee* left;
	struct treee* right;
};
struct list {
	int weight;
	struct treee* tree;
	struct list* next_list;
};
struct list* create_list(unsigned char symbol, int weight) {
	struct treee* new_tree = malloc(sizeof(struct treee));
	struct list* new_list = malloc(sizeof(struct list));
	new_tree->left = NULL;
	new_tree->right = NULL;
	new_tree->symbol = symbol;
	new_tree->height = 1;
	new_list->tree = new_tree;
	new_list->next_list = NULL;
	new_list->weight = weight;
	return new_list;
}

struct list* connect_lists(struct list* first_list, struct list* second_list) {
	struct treee* new_tree = malloc(sizeof(struct treee));
	struct list* new_list = malloc(sizeof(struct list));
	/*if (first_list->weight > second_list->weight) {
		new_tree->right = first_list->tree;
		new_tree->left = second_list->tree;
	}
	else {*/
		new_tree->right = second_list->tree;
		new_tree->left = first_list->tree;
	//}
	new_tree->height = max(first_list->tree->height,second_list->tree->height) + 1;
	new_list->tree = new_tree;
	new_list->next_list = NULL;
	new_list->weight = first_list->weight + second_list->weight;
	free(first_list);
	free(second_list);
	return new_list;
}

struct list* put_list(struct list* main_list, struct list* new_list) {
	if (new_list->weight < main_list->weight) {
		new_list->next_list = main_list;
		return new_list;
	}
	else {
		struct list* buff_list = main_list;
		while (buff_list->next_list!= NULL ){
			if (buff_list->next_list->weight < new_list->weight) {
				break;
			}
			buff_list = buff_list->next_list;
		}
		if (buff_list->next_list == NULL) {
			buff_list->next_list = new_list;
		}
		else {
			new_list->next_list = buff_list->next_list;
			buff_list->next_list = new_list;
		}
		return main_list;
	}
}

bool is_last(struct list* list) {
	if (list->next_list==NULL) {
		return true;
	}
	else {
		return false;
	}
}

struct list* create_starting_lists(int * table_of_frequency) {
	struct list* new_list = NULL;
	struct list* start_of_lists = NULL;
	int i = 0;
	for ( i ; i < 256; i++) {
		if (table_of_frequency[i] != 0) {
			new_list = create_list((char)i, table_of_frequency[i]);
		}
		break;
	}
	start_of_lists = new_list;
	i++;
	for (i; i < 256; i++) {
		if (table_of_frequency[i] != 0) {
			new_list = create_list((char)i, table_of_frequency[i]);
			start_of_lists= put_list(start_of_lists,new_list);
		}	
	}
	return start_of_lists;
}

struct list* pop_list(struct list** lists) {
	struct list* poped_list = (*lists);
	poped_list->next_list = NULL;
	(*lists) = (*lists)->next_list;
	return poped_list;
}

struct treee* create_tree(struct list* lists) {
	struct list* first_list = NULL;
	struct list* second_list = NULL;
	struct list* new_list = NULL;
	while (!is_last(lists)) {
		first_list = pop_list(&lists);
		second_list = pop_list(&lists);
		new_list = connect_lists(first_list, second_list);
		lists = put_list(lists, new_list);
	} 
	struct treee* code_tree = new_list->tree;
	free(lists);
	return code_tree;
}


int* create_table_of_frequency(FILE *fin) {
	int number_of_chars = 256, EOF_catcher=0;
	int *table_of_frequency = malloc(number_of_chars*sizeof(int));
	for (int i = 0; i < 256; i++) {
		table_of_frequency[i] = 0;
	}
	unsigned char symbol = 0;
	EOF_catcher = fscanf(fin, "%c", &symbol);
	while (EOF_catcher != EOF) {
		table_of_frequency[(int)symbol]++;
		EOF_catcher = fscanf(fin, "%c", &symbol);
	}
	fseek(fin, 2, SEEK_SET);
	return table_of_frequency;
}

int* create_code_table(struct treee* code_tree) {
	int * code_table = malloc(256 * 256 * sizeof(char));
	char buff[256];
	for (int i = 0; i < 256; i++) {
		buff[i] = 's';
	}


}

void code(FILE *fin, FILE*fout) {
	int table_of_frequency[256]= create_table_of_frequency(fin);
	struct list* lists = create_starting_lists(table_of_frequency);
	struct treee* code_tree = create_tree(lists);
	int* code_table = create_code_table(code_tree);

}


void decode(FILE *fin, FILE*fout) {

}

int main() {
	FILE *fin,*fout;
	fin = fopen("in.txt","rb");
	fout = fopen("out.txt","wb");
	if (fin == NULL) {
		fprinf(fout, "Error in openning file");
		return 0;
	}
	char mode;
	fscanf(fin, "%c", &mode);
	if (mode == 'c') {
		fscanf(fin, "%c", &mode);
		//code(fin,fout);
	}
	else {
		fscanf(fin, "%c", &mode);
		//decode(fin,fout);
	}


	return 0;
}