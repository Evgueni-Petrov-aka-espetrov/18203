#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node{
	struct node *left;
	struct node *right;
	struct node *parent;
	int active;
	char charachter;
	int count;
};

void MakeTable(FILE*, int[]);
void Print(int[]);
int ArrOptimalSize(int[]);
int InitTreeArr(struct node *, int []);
int BuildTree(struct node *treearr, int);
int FindMinEl(struct node[], int);
void encode(char charachter, char *buffer, struct node *treearr, int end);

void encode(char symbol, char code[], struct node[], int end); // e.g. encode('a', code) -> code==("1101")
char decode(const char code[], struct node[], int end); // decode("1101") -> 'a'

char pack(const char buf[8]); // pack("00111010") -> 'G'
void unpack(char symbol, char *buf); // unpack('G', buf) -> buf=="00111010"

void submit(FILE *out_file, const char *code);



int main() {
	FILE *huffmanfile;
	huffmanfile = fopen("test.txt", "rb"); //b for binary

	/*замечание : чтобы fopen смогла прочитать файл нужно его класть в одну директорию с main.c */
	/*замечание : чтобы использовать абсолютный путь к файлу нужно добавить двойные слеши (не сработало)*/
	if (huffmanfile == NULL) {
		printf("Unable to open file :(\n");
		return 0;
	}

	int huffmantable[256];

	MakeTable(huffmanfile, huffmantable);
	Print(huffmantable);


	int N = ArrOptimalSize(huffmantable);

	struct node *treearr = (struct node *)malloc(sizeof(struct node) * N);

	int end = InitTreeArr(treearr, huffmantable);

	int symbolcount = end;
	
	end = BuildTree(treearr, end);

	char *buffer = (char *)malloc(symbolcount); // промежуточный буфер для хранения 1!!! кода хаффмана // минимальный необходимый размер буфера расчитывается исходя из наихудшего случая в дереве хаффмана (кол-во символов - 1)

	// fgetpos
	fseek(huffmanfile, 0, SEEK_SET);

	FILE *zippedfile;
	zippedfile = fopen("testout.huf", "wb");
	unsigned char recordscount = symbolcount;
	fwrite(&recordscount, sizeof(unsigned char), 1, zippedfile);

	for (int i = 0; i < symbolcount; i++) {

		char symbol = treearr[i].charachter;
		char buffer[1000]; //

		unpack(symbol, buffer);
		submit(zippedfile, buffer);

		encode(symbol, buffer, treearr, end);
		char size = strlen(buffer);
		unpack(size, buffer);
		submit(zippedfile, buffer);

		encode(symbol, buffer, treearr, end);
		submit(zippedfile, buffer);
	}

	while (!feof(huffmanfile)) { // главная часть программы кодирования. вызываются функции которые в совокупности кодируют файл
		
		char charachter;
		fscanf(huffmanfile ,"%c", &charachter);  
		encode(charachter, buffer, treearr, end); // получает на вход чаровый символ, строит для него двоичный код проходя по дереву и кладет его в buffer
		//submit(zippedfile, buffer, bigbuffer); // первые 8 позиций в buffer'е переводит в char и записыват в файл zippedfile

		/*printf("E %c = %s\n", charachter, buffer);
		rencode(charachter, buffer, &treearr[end-1]);
		printf("R %c = %s\n", charachter, buffer);
*/
	}

	bigbuffer_flush(); // write rest of bigbuffer to file

	//void huffman_node_encodings(const huffman_node *node, unsigned int *arr, unsigned int pos, huffmanfn fun) // pos подается нулевым извне 
	//{
	//	if (node->left) {
	//		arr[pos] = 0;
	//		huffman_node_encodings(node->left, arr, pos + 1, fun);
	//	}
	//	if (node->right) {
	//		arr[pos] = 1;
	//		huffman_node_encodings(node->right, arr, pos + 1, fun);
	//	}
	//	if (!(node->left || node->right)) {
	//		fun(node->data, arr, pos);
	//	}
	//}


	/*void encode(char charachter, char *buffer, struct node *treearr, int end) {

		int i = 0;
		while (treearr[i].charachter != charachter) {
			i++;
		}

		struct node * ptr = &treearr[i];

		int currpos = 0;
		while (ptr->parent != NULL) {

			if (ptr == ptr->parent->left) {
				buffer[currpos] = '0';
				currpos++;
			}
			if (ptr == ptr->parent->right) {
				buffer[currpos] = '1';
				currpos++;
			}

			ptr = ptr->parent;
		}
		buffer[currpos] = '\0';


		for (int i = 0; i < currpos / 2; i++) {
			char tmp;
			tmp = buffer[i];
			buffer[i] = buffer[currpos - 1 - i];
			buffer[currpos - 1 - i] = tmp;
		}
	}*/

	char bigbuffer[1000];
	int bbend = 0;

	void submit(FILE* zippedfile, char *buffer) {
		// append buffer to the end of bigbuffer

		while (bbend >= 8) {
			// pack first 8 bits
			// fwrite to zippedfile
		}





		/*
		int in, out = 0, tmp = 0, base = 1;
		while (in > 0)
		{
			tmp = in % 10;
			out = out + tmp * base;
			in = in / 10;
			base = base * 2;
		}
		*/
	}




	/*
	
	struct node *cur=get_node_by_character(treearr, end, 'G');
	while (cur->parent != NULL) {
		struct node *parent=cur->parent;
		if (parent->left==cur) {
			// next code bit is "1"
		} else {
			// next code bit is "0"
		}
		cur=parent;
	}
	*/
}

void MakeTable(FILE *huffmanfile, int huffmantable[]) { // [] ~ *
	
	memset(huffmantable, 0, sizeof(int)*256);
	unsigned char currchar = 0;

	while (!feof(huffmanfile)) {
		fscanf(huffmanfile, "%c", &currchar);
		huffmantable[currchar]++;
	}
}

void Print(int huffmantable[256]) {

	for (int i = 0; i < 256; i++) {
		printf("%d := %d\n", i, huffmantable[i]);
	}
}

int ArrOptimalSize(int huffmantable[256]){
	int N = 0;
	for (int i = 0; i < 256; i++) {
		if (huffmantable[i] != 0) {
			N++;
		}
	}
	N = 2 * N - 1;
	return N;
}

int InitTreeArr(struct node *treearr, int huffmantable[]) {

	int end = 0;
	for (int i = 0; i < 256; i++) {
		if (huffmantable[i] != 0) { // инициализируем все поля структуры struct node
			treearr[end].count = huffmantable[i];
			treearr[end].charachter = (char)i;
			treearr[end].left = NULL;
			treearr[end].right = NULL;
			treearr[end].parent = NULL;
			treearr[end].active = 1;

			end++; // переходим на следующую ячейку
		}
	}

	return end; 
}

int BuildTree(struct node *treearr, int end) {

	int symbolcount = end;
	for (int i = 0; i < symbolcount - 1; i++) { // N - 1 раз мы создаем новый узел

		int min1, min2;
		min1 = FindMinEl(treearr, end);
		min2 = FindMinEl(treearr, end);
		treearr[min1].parent = &treearr[end];
		treearr[min2].parent = &treearr[end];
		treearr[end].left = &treearr[min1];
		treearr[end].right = &treearr[min2];
		treearr[end].parent = NULL;
		treearr[end].count = treearr[min1].count + treearr[min2].count;
		treearr[end].active = 1;

		end++;
	}
	return end;
}


int FindMinEl(struct node *treearr, int end) {

	int currmin = 0;
	while (treearr[currmin].active == 0) {
		currmin++;
	}
	for (int i = 0; i < end; i++) {
		if ((treearr[i].count < treearr[currmin].count) && treearr[i].active == 1) {
			currmin = i;
		}
	}
	treearr[currmin].active = 0;

	return currmin;
}

int rencode(char charachter, char *buffer, struct node *n) {

	if (n->charachter == charachter) {
		*buffer = '\0';
		return 1;
	}
	if (n->left == NULL) { // we are on a wrong leaf
		//*buffer = 0;
		return 0;
	}

	// not a leaf
	if (rencode(charachter, buffer+1, n->left)) {
		*buffer = '0';
		return 1;
	}
	if (rencode(charachter, buffer + 1, n->right)) {
		buffer[0] = '1';
		return 1;
	}
	return 0;
	//fprintf(stderr, "ERROR, symbol not found %c\n", charachter);
}

void encode(char charachter, char *buffer, struct node *treearr, int end) {

	int i = 0;
	while (treearr[i].charachter != charachter) {
		i++;
	}

	struct node * ptr = &treearr[i];

	int currpos = 0;
	while (ptr->parent != NULL) {

		if (ptr == ptr->parent->left) {
			buffer[currpos] = '0';
			currpos++;
		}
		if (ptr == ptr->parent->right) {
			buffer[currpos] = '1';
			currpos++;
		}

		ptr = ptr->parent;
	}
	buffer[currpos] = '\0';


	for (int i = 0; i < currpos / 2; i++) {
		char tmp;
		tmp = buffer[i];
		buffer[i] = buffer[currpos - 1 - i];
		buffer[currpos - 1 - i] = tmp;
	}
}

void unpack(char symbol, char *buffer) {

	unsigned char in = symbol, tmp;
	for (int i = 0; i < 8; i++)
	{
		tmp = in % 2;
		if (tmp == 0) {
			buffer[i] = '0';
		}
		if (tmp == 1) {
			buffer[i] = '1';
		}
		in = in / 2;
	}
	buffer[8] = '\0';
}
