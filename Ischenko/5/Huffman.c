#pragma warning( push, 4 )
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<assert.h> 
#include<math.h>

typedef enum { false, true } bool;
typedef enum { file_ended, file_not_ended} file_end;

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
struct bits_buff {
	unsigned char buff;
	int used_bits;
};
struct treee* create_tree(unsigned char symbol) {
	struct treee* new_tree = malloc(sizeof(struct treee));
	new_tree->left = NULL;
	new_tree->right = NULL;
	new_tree->symbol = symbol;
	new_tree->height = 1;
	return new_tree;
}

void delete_tree(struct treee* tree){
	if (tree->left == NULL) {
		return;
	}
	else {
		delete_tree(tree->left);
		free(tree->left);
		delete_tree(tree->right);
		free(tree->right);
	}
}
////////////////
struct list* create_list(unsigned char symbol, int weight) {
	struct treee* new_tree = create_tree(symbol);
	struct list* new_list = malloc(sizeof(struct list));
	new_list->tree = new_tree;
	new_list->next_list = NULL;
	new_list->weight = weight;
	return new_list;
}
void push_in_bits_buff(struct bits_buff* buff_of_bits,char pushed_bit, FILE* fout) {
	assert(pushed_bit=='0' ||pushed_bit=='1');
	char prepared_bit = (pushed_bit - '0') << (7 - buff_of_bits->used_bits);
	buff_of_bits->buff = buff_of_bits->buff | prepared_bit;
	buff_of_bits->used_bits++;
	if (buff_of_bits->used_bits == 8) {
		fprintf(fout, "%c", buff_of_bits->buff);
		buff_of_bits->used_bits = 0;
		buff_of_bits->buff = 0; 
	}
}

struct list* connect_lists(struct list* first_list, struct list* second_list) {
	struct treee* new_tree = malloc(sizeof(struct treee));
	struct list* new_list = malloc(sizeof(struct list));
	new_tree->right = second_list->tree;
	new_tree->left = first_list->tree;
	new_tree->height = max(first_list->tree->height, second_list->tree->height) + 1;
	new_list->tree = new_tree;
	new_list->next_list = NULL;
	new_list->weight = first_list->weight + second_list->weight;
	free(first_list);
	free(second_list);
	return new_list;
}
struct list* put_list(struct list* main_list, struct list* new_list) {
	if (main_list == NULL) {
		return new_list;
	}
	if (new_list->weight < main_list->weight) {
		new_list->next_list = main_list;
		return new_list;
	}
	else {
		struct list* buff_list = main_list;
		while (buff_list->next_list != NULL) {
			if (buff_list->next_list->weight > new_list->weight) {
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
struct list* create_starting_lists(int * table_of_frequency) {
	struct list* new_list = NULL;
	struct list* start_of_lists = NULL;
	int i = 0;
	for (; i < 256; i++) {
		if (table_of_frequency[i] != 0) {
			new_list = create_list((char)i, table_of_frequency[i]);
			break;
		}
		
	}
	start_of_lists = new_list;
	i++;
	for (; i < 256; i++) {
		if (table_of_frequency[i] != 0) {
			new_list = create_list((char)i, table_of_frequency[i]);
			start_of_lists = put_list(start_of_lists, new_list);
		}
	}
	return start_of_lists;
}
struct list* pop_list(struct list** lists) {
	struct list* poped_list = (*lists);
	(*lists) = (*lists)->next_list;
	poped_list->next_list = NULL;
	return poped_list;
}
bool is_last(struct list* list) {
	if (list->next_list == NULL) {
		return true;
	}
	else {
		return false;
	}
}
struct treee* create_code_tree(struct list* lists) {
	struct list* first_list = NULL;
	struct list* second_list = NULL;
	struct list* new_list = lists;
	while (is_last(lists)!=true) {
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
	int number_of_chars = 256, EOF_catcher = 0;
	int *table_of_frequency = malloc(number_of_chars * sizeof(int));
	for (int i = 0; i < 256; i++) {
		table_of_frequency[i] = 0;
	}
	unsigned char symbol = 0;
	EOF_catcher = fscanf(fin, "%c", &symbol);
	while (EOF_catcher != EOF) {
		table_of_frequency[(int)symbol]++;
		EOF_catcher = fscanf(fin, "%c", &symbol);
	}
	fseek(fin, 3, SEEK_SET);
	return table_of_frequency;
}

void copy_to_table(struct treee* tree, unsigned char* code_buff,unsigned char* code_table) {
	int symbol_code = (int)tree->symbol;
	for (int i = 0; i < 256; i++) {
		if (code_buff[i] == 's') {
			code_table[symbol_code * 256 + i] = code_buff[i];
			return;
		}
		else {
			code_table[symbol_code * 256 + i] = code_buff[i];
		}
	}
	return;
}
void from_tree_to_table(struct treee* code_tree, int pos_in_buff,unsigned char* code_buff,unsigned char* code_table) {
	if (code_tree->left==NULL || code_tree->right==NULL) {
		copy_to_table(code_tree,code_buff,code_table);
	}
	else {
		code_buff[pos_in_buff] = '0';
		from_tree_to_table(code_tree->left, pos_in_buff + 1, code_buff, code_table);

		code_buff[pos_in_buff] = '1';
		from_tree_to_table(code_tree->right, pos_in_buff + 1, code_buff, code_table);
	}
	code_buff[pos_in_buff] = 's';
	return;
}
unsigned char* create_code_table(struct treee* code_tree) {
	unsigned char* code_table = malloc(256 * 256 * sizeof(unsigned char));
	unsigned char* code_buff = malloc(256 * sizeof(unsigned char));
	for (int i = 0; i < 256; i++) {
			code_buff[i] = 's';
		}
	from_tree_to_table(code_tree,0,code_buff,code_table);
	free(code_buff);
	return code_table;
}

void serialize_tree(struct treee* tree, FILE* fout,struct bits_buff* buff_of_bits) {
	if (tree->left == NULL) {
		push_in_bits_buff(buff_of_bits, '0', fout);
		int symbol = (int)(tree->symbol);
		for (int i = 0; i < 8; i++) {
			unsigned char push_bit = (((unsigned char)symbol)>>7) + 48;
			push_in_bits_buff(buff_of_bits,push_bit,fout);
			symbol = symbol << 1;
		}
	}
	else {
		push_in_bits_buff(buff_of_bits, '1', fout);
		serialize_tree(tree->left, fout, buff_of_bits);
		serialize_tree(tree->right, fout, buff_of_bits);
	}
	return;
}

void code_symbol(FILE* fout, struct bits_buff* buff_of_bits, unsigned char* code_table, int char_code) {
	unsigned char bit = ' ';
	for (int i = 0; i < 256; i++) {
		bit = code_table[char_code * 256 + i];
		if (bit=='s') {
			return;
		}
		else {
			push_in_bits_buff(buff_of_bits,bit,fout);
		}
	}
	return;
}
void code_data(FILE* fin, FILE* fout,struct bits_buff* buff_of_bits,unsigned char* code_table){
	int EOF_catcher = 0;
	unsigned char symbol = ' ';
	EOF_catcher = fscanf(fin,"%c",&symbol);
	while (EOF_catcher!= EOF) {
		code_symbol(fout, buff_of_bits, code_table, (int)symbol);
		EOF_catcher = fscanf(fin, "%c", &symbol);
	}
	return;
}

void code_if_unique(FILE* fin, FILE* fout,struct bits_buff* buff_of_bits){
	int EOF_catcher = 0;
	unsigned char symbol;
	EOF_catcher = fscanf(fin, "%c", &symbol);
	while (EOF_catcher != EOF) {
		push_in_bits_buff(buff_of_bits, '1', fout);
		EOF_catcher = fscanf(fin, "%c", &symbol);
	}
	return;
}
void direct_coding(FILE* fin, FILE* fout) {
	int EOF_catcher = 0;
	unsigned char symbol;
	EOF_catcher = fscanf(fin, "%c", &symbol);
	while (EOF_catcher != EOF) {
		fprintf(fout, "%c", symbol);
		EOF_catcher = fscanf(fin, "%c", &symbol);
	}
	return;
}

void code(FILE *fin, FILE*fout) {
	unsigned char extra_bits_and_scanning_mode_code = (char)0; //@@@##*** @@@ - ammount of extra bits for tree, #-mode code, *** - ammount of extra bits for code
	unsigned char extra_bits_for_serialized_tree = 0, extra_bits_for_code = 0, mode = 0;
	fprintf(fout, "%c", extra_bits_and_scanning_mode_code);
	struct bits_buff *buff_of_bits = malloc(sizeof(struct bits_buff));
	buff_of_bits->buff = 0;
	buff_of_bits->used_bits = 0;
	int *table_of_frequency = create_table_of_frequency(fin);
	int ammount_of_symbols = 0;
	for (int i = 0; i < 256; i++) {
		if (table_of_frequency[i] != 0) {
			ammount_of_symbols++;
		}
	}
	if (ammount_of_symbols == 0) {
		mode = 3 << 3;
		goto if_mode_not_normal;
	}
	struct list* lists = create_starting_lists(table_of_frequency);
	free(table_of_frequency);
	struct treee* code_tree = create_code_tree(lists);
	if (code_tree->height == 1) {
		fprintf(fout, "%c", code_tree->symbol);
		code_if_unique(fin, fout, buff_of_bits);
		mode = 1 << 3;
		goto if_mode_not_normal;
	}
	if (code_tree->height == 9 && ammount_of_symbols == 256) {
		direct_coding(fin, fout);
		mode = 1 << 4;
		goto if_mode_not_normal;
	}
	unsigned char* code_table = create_code_table(code_tree);
	serialize_tree(code_tree, fout, buff_of_bits);
	delete_tree(code_tree);
	free(code_tree);
	extra_bits_for_serialized_tree = (8 - buff_of_bits->used_bits) % 8;
	for (int i = 0; i < extra_bits_for_serialized_tree; i++) {
		push_in_bits_buff(buff_of_bits, '0', fout);
	}
	code_data(fin, fout, buff_of_bits, code_table);
	free(code_table);
if_mode_not_normal:
	extra_bits_for_code = (8 - buff_of_bits->used_bits) % 8;
	for (int i = 0; i < extra_bits_for_code; i++) {
		push_in_bits_buff(buff_of_bits, '0', fout);
	}
	extra_bits_and_scanning_mode_code = mode | (unsigned char)extra_bits_for_serialized_tree << 5 | (unsigned char)extra_bits_for_code;
	fseek(fout, 0, SEEK_SET);
	fprintf(fout, "%c", extra_bits_and_scanning_mode_code);
	free(buff_of_bits);
	return;
}
///////////////
file_end fill_bits_buffer(struct bits_buff* buff_of_bits, FILE* fin) {
	int EOF_catcher = 0;
	unsigned char symbol=' ';
	EOF_catcher = fscanf(fin, "%c", &symbol);
	if (EOF_catcher != EOF) {
		buff_of_bits->buff = symbol;
		return file_not_ended;
	}
	else {
		return file_ended;
	}
}
file_end controlled_get_bit_from_file(struct bits_buff* buff_of_bits, struct bits_buff* control_buff_of_bits, FILE* fin, int* bit, file_end did_file_ended) {
	*bit = (buff_of_bits->buff) >> 7;
	buff_of_bits->buff = (buff_of_bits->buff) << 1;
	buff_of_bits->used_bits++;
	if (buff_of_bits->used_bits == 8) {
		buff_of_bits->used_bits = 0;
		buff_of_bits->buff = control_buff_of_bits->buff;
		file_end is_file_ended = fill_bits_buffer(control_buff_of_bits, fin);
		return is_file_ended;
	}
	return did_file_ended;
}
unsigned char get_bit_from_file(struct bits_buff* buff_of_bits,FILE* fin ) {
	if (buff_of_bits->used_bits == 8) {
		buff_of_bits->used_bits = 0;
		fill_bits_buffer(buff_of_bits, fin);
	}
	unsigned char bit = (buff_of_bits->buff) >> 7;
	buff_of_bits->buff = (buff_of_bits->buff) << 1;
	buff_of_bits->used_bits++;
	return bit;
}

void deserialization(FILE*fin, struct bits_buff* buff_of_bits, struct treee* tree){
	for (int i = 0; i < 2; i++) {
		int bit = get_bit_from_file(buff_of_bits, fin);
		if (bit == 1) {
			struct treee* new_tree = create_tree('r');
			if (tree->left == NULL) {
				tree->left = new_tree;
				deserialization(fin, buff_of_bits, tree->left);
			}
			else {
				if (tree->right == NULL) {
					tree->right = new_tree;
					deserialization(fin, buff_of_bits, tree->right);
				}
			}
		}
		else {
			unsigned char symbol=0;
			unsigned char buff_bit = 0;
			for (int j = 0; j < 8; j++) {
				symbol = symbol << 1;
				buff_bit = get_bit_from_file(buff_of_bits, fin);
				symbol = symbol | buff_bit;
			}
			struct treee* new_tree = create_tree(symbol);
			if (tree->left == NULL) {
				tree->left = new_tree;
			}
			else {
				if (tree->right == NULL) {
					tree->right = new_tree;
					return;
				}
				else {
					assert(0);
				}
			}
		}
	}
	return;
}
struct treee* deserialize_tree(FILE* fin){
	struct bits_buff* buff_of_bits = malloc(sizeof(struct bits_buff));
	buff_of_bits->buff = 0;
	buff_of_bits->used_bits = 0;
	struct treee* tree = create_tree('r');
	fill_bits_buffer(buff_of_bits, fin);
	get_bit_from_file(buff_of_bits, fin);
	deserialization(fin, buff_of_bits, tree);
	free(buff_of_bits);
	return tree;
}

void normal_decode(FILE* fin, FILE* fout, int extra_bits) {
	struct treee* code_tree = deserialize_tree(fin);
	struct bits_buff* buff_of_bits = malloc(sizeof(struct bits_buff));
	buff_of_bits->used_bits = 0;
	struct bits_buff* controlled_buff_of_bits = malloc(sizeof(struct bits_buff));
	controlled_buff_of_bits->used_bits = 0;
	file_end is_file_ended = file_not_ended;
	is_file_ended = fill_bits_buffer(buff_of_bits, fin);
	is_file_ended = fill_bits_buffer(controlled_buff_of_bits, fin);
	struct treee* current_tree = NULL;
	int bit = 0;
	while (is_file_ended != file_ended) {
		current_tree = code_tree;
		while (1) {
			bit = 0;
			is_file_ended = controlled_get_bit_from_file(buff_of_bits, controlled_buff_of_bits, fin, &bit,is_file_ended);
			if (bit == 0) {
				current_tree = current_tree->left;
			}
			if (bit == 1) {
				current_tree = current_tree->right;
			}
			if (current_tree->left == NULL) {
				break;
			}
		}
		fprintf(fout,"%c" ,current_tree->symbol);
	}
	current_tree = code_tree;
	int used_bits = buff_of_bits->used_bits;
	for (int i = 0; i < abs( (8 - used_bits) /*% 8*/-extra_bits); i++) {
		bit= get_bit_from_file(buff_of_bits, fin);
		if (bit == 0) {
			current_tree = current_tree->left;
		}
		if (bit == 1) {
			current_tree = current_tree->right;
		}
		if (current_tree->left == NULL) {
			fprintf(fout, "%c", current_tree->symbol);
			current_tree = code_tree;
		}
	}
	free(buff_of_bits);
	free(controlled_buff_of_bits);
	delete_tree(code_tree);
	free(code_tree);
}
void decode_for_unique(FILE* fin,FILE* fout,int extra_bits) {
	struct bits_buff* buff_of_bits = malloc(sizeof(struct bits_buff));
	struct bits_buff* controlled_buff_of_bits = malloc(sizeof(struct bits_buff));
	unsigned char symbol=' ';
	fscanf(fin,"%c",&symbol);
	file_end is_file_ended = file_not_ended;
	is_file_ended = fill_bits_buffer(buff_of_bits,fin);
	is_file_ended = fill_bits_buffer(controlled_buff_of_bits, fin);
	while (is_file_ended != file_ended) {
		for (int i = 0; i < 8; i++) {
			fprintf(fout, "%c", symbol);
		}
		is_file_ended = fill_bits_buffer(controlled_buff_of_bits,fin);
	}
	for (int i = 0; i < (8 - extra_bits)/* % 8*/; i++) {
		fprintf(fout, "%c", symbol);
	}
	free(buff_of_bits);
	free(controlled_buff_of_bits);
	return;
}
void direct_decoding(FILE* fin, FILE* fout) {
	unsigned char symbol;
	int EOF_catcher = 0;
	EOF_catcher = fscanf(fin, "%c", &symbol);
	while (EOF_catcher != EOF) {
		fprintf(fout,"%c",symbol);
		EOF_catcher = fscanf(fin, "%c", &symbol);
	}
}

void decode(FILE *fin, FILE*fout) {
	unsigned char extra_bits_and_scanning_mode_code = 0;
	fscanf(fin, "%c", &extra_bits_and_scanning_mode_code);
	unsigned char extra_bits_for_serialized_tree=0, extra_bits_for_code=0,mode=0;
	extra_bits_for_serialized_tree = extra_bits_and_scanning_mode_code >> 5;
	mode = (extra_bits_and_scanning_mode_code << 3);
	mode = mode >> 6;
	extra_bits_for_code = (extra_bits_and_scanning_mode_code << 5) ;
	extra_bits_for_code = extra_bits_for_code >> 5;
	if (mode == 3) {
		return;
	}
	if (mode == 1) {
		decode_for_unique(fin,fout,extra_bits_for_code);
	}
	if (mode == 2) {
		direct_decoding(fin, fout);
	}
	if (mode == 0) {
		normal_decode(fin, fout,extra_bits_for_code);
	}
	return;
}

int main() {
	FILE *fin, *fout;
	fin = fopen("in.txt", "rb");
	fout = fopen("out.txt", "wb");
	unsigned char mode;
	fscanf(fin, "%c", &mode);
	if (mode == 'c') {
		fscanf(fin, "%c", &mode);
		fscanf(fin, "%c", &mode);
		code(fin,fout);
	}
	else {
		if (mode == 'd') {
			fscanf(fin, "%c", &mode);
			fscanf(fin, "%c", &mode);
			decode(fin, fout);
		}
	}
	fclose(fin);
	fclose(fout);
	return 0;
}
