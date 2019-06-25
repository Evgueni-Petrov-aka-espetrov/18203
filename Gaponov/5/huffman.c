#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char bigbuffer[9];
int bufpos = 0;

struct node {
	struct node *left;
	struct node *right;
	struct node *parent;
	int flag;
	char data;
	int count;
};

void MakeTable(FILE *huffmanfile, int chartable[]) {

	memset(chartable, 0, sizeof(int) * 256);
	unsigned char currchar = 0;

	while (!feof(huffmanfile)) {
		fscanf(huffmanfile, "%c", &currchar);
		chartable[currchar]++;
	}
}

int InitTreeArr(struct node *treearr, int huffmantable[]) {

	int end = 0;
	for (int i = 0; i < 256; ++i)
		if (huffmantable[i]) {
			treearr[end].left = NULL;
			treearr[end].right = NULL;
			treearr[end].parent = NULL;
			treearr[end].flag = 1;
			treearr[end].count = huffmantable[i];
			treearr[end].data = (char)i;

			++end;
		}
	return end;
}

int FindMinimumElement(struct node *treearr, int end) {

	int currmin = 0;
	while (treearr[currmin].flag == 0)
		currmin++;

	for (int i = 0; i < end; i++)
		if ((treearr[i].count < treearr[currmin].count) && treearr[i].flag == 1)
			currmin = i;

	treearr[currmin].flag = 0;

	return currmin;
}

int BuildTree(struct node *treearr, int end) {

	int symbolcount = end;
	for (int i = 0; i < symbolcount - 1; i++) {

		int min1, min2;
		min1 = FindMinimumElement(treearr, end);
		min2 = FindMinimumElement(treearr, end);
		treearr[min1].parent = &treearr[end];
		treearr[min2].parent = &treearr[end];
		treearr[end].left = &treearr[min1];
		treearr[end].right = &treearr[min2];
		treearr[end].parent = NULL;
		treearr[end].count = treearr[min1].count + treearr[min2].count;
		treearr[end].flag = 1;

		end++;
	}
	return end;
}

void Encode(char charachter, char *buffer, struct node *treearr) {

	int i = 0;
	while (treearr[i].data != charachter)
		i++;

	struct node *ptr = &treearr[i];

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

	for (int j = 0; j < currpos / 2; j++) {
		char temp = buffer[currpos - j - 1];
		buffer[currpos - j - 1] = buffer[j];
		buffer[j] = temp;
	}
}

void Unpack(char symbol, char *buffer) {

	unsigned char in = symbol, temp;
	for (int i = 0; i < 8; i++)
	{
		temp = in % 2;
		if (temp == 0)
			buffer[i] = '0';

		if (temp == 1)
			buffer[i] = '1';

		in = in / 2;
	}
	buffer[8] = '\0';
}

char Pack(char bigbuffer[]) {

	unsigned char x = 0;
	for (int i = 0; i < 8; i++) {
		if (bigbuffer[7 - i] == '1') 
			x++;
		
		if (i < 7) 
			x = x << 1;
		
	}
	bigbuffer[8] = '\0';
	return x;
}

void Submit(FILE* zippedfile, char *buffer) {
	for (int i = 0; i < (int)strlen(buffer); i++) {
		bigbuffer[bufpos] = buffer[i];
		bufpos++;
		bigbuffer[bufpos] = '\0';
		if (bufpos == 8) {
			char packedbyte = Pack(bigbuffer);
			fwrite(&packedbyte, 1, 1, zippedfile);
			bufpos = 0;
		}
	}
}

void PackTree(FILE* zippedfile, struct node *treearr, struct node *n) {

	char buffer[9];

	if (n->left != NULL) {
		Submit(zippedfile, "0");
		PackTree(zippedfile, treearr, n->left);
	}
	else
	{
		Submit(zippedfile, "1");
		Unpack(n->data, buffer);
		Submit(zippedfile, buffer);
	}

	if (n->right != NULL)
		PackTree(zippedfile, treearr, n->right);
}

char FetchOne(FILE* archived) {

	char tmp;
	if (bufpos >= 8) {
		fread(&tmp, 1, 1, archived);
		Unpack(tmp, bigbuffer);
		bufpos = 0;
	}
	return bigbuffer[bufpos++];
}

char Decode(FILE *archive, struct node *root) {

	while (root->left != NULL) 
		if (FetchOne(archive) == '0')
			root = root->left;
		else
			root = root->right;
	return root->data;
}

void FreeMemory(struct node *root) {
	if (root != NULL) {
		FreeMemory(root->right);
		FreeMemory(root->left);
		free(root);
	}
}

struct node* UnpackTree(FILE* archived) {

	char buf[9];
	if (FetchOne(archived) == '0') {

		struct node *node = (struct node*)malloc(sizeof(struct node));

		node->left = UnpackTree(archived);
		node->right = UnpackTree(archived);

		return node;
	}
	else
	{
		for (int i = 0; i < 8; i++)
			buf[i] = FetchOne(archived);

		buf[8] = '\0';
		struct node *leaf = (struct node*)malloc(sizeof(struct node));
		leaf->data = Pack(buf);
		leaf->left = NULL;
		leaf->right = NULL;
		return leaf;
	}
}

int DecodeFile(FILE *archived, FILE *unarchived) {

	if (archived == NULL)
		return 1;

	size_t datasize;
	fread(&datasize, sizeof(size_t), 1, archived);

	bufpos = 8;

	struct node *root = UnpackTree(archived);

	char symbol;

	for (int i = 0; i < (int)datasize; i++) {
		symbol = Decode(archived, root);
		fwrite(&symbol, 1, 1, unarchived);
	}

	FreeMemory(root);
	return 0;
}

int CodeFile(FILE *raw, FILE *zipped) {

	int chartable[256];

	MakeTable(raw, chartable);

	int N = 0;
	for (int i = 0; i < 256; i++)
		if (chartable[i] != 0)
			N++;

	N = 2 * N;

	struct node *treearr = (struct node *)malloc(sizeof(struct node) * N);
	int end = InitTreeArr(treearr, chartable);
	int symbolcount = end;

	end = BuildTree(treearr, end);

	char *buffer = (char*)malloc(symbolcount);

	fseek(raw, 0, SEEK_END);
	size_t datasize = ftell(raw) - 3;
	fwrite(&datasize, sizeof(size_t), 1, zipped);
	fseek(raw, 3, SEEK_SET);

	PackTree(zipped, treearr, &treearr[end - 1]);

	char character;
	while (fscanf(raw, "%c", &character) > 0) {
		Encode(character, buffer, treearr);
		Submit(zipped, buffer);
	}

	if (bufpos != 0) {
		unsigned char flushbyte = Pack(bigbuffer);
		fwrite(&flushbyte, 1, 1, zipped);
	}

	free(treearr);
	return 0;
}

int main() {
	FILE *raw;
	raw = fopen("in.txt", "rb");

	FILE *zipped;
	zipped = fopen("out.txt", "wb");

	char buffer[3];
	fread(&buffer, 1, 3, raw);

	if (buffer[1] != '\r')
		return 1;

	if (buffer[2] != '\n')
		return 1;

	if (buffer[0] == 'c')
		CodeFile(raw, zipped);

	if (buffer[0] == 'd')
		DecodeFile(raw, zipped);

	fclose(raw);
	fclose(zipped);
	return 0;
}
