#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#define MAX_SIZE 256
#define FILLER 0

typedef struct TTree {
	unsigned char sym;
	unsigned int freq;
	struct TTree* left;
	struct TTree* right;
} Tree;

typedef struct TStack{
	Tree* root;
	struct TStack* next;
} Stack;

typedef struct TCode {
	unsigned char* sym;
	int size;
} Code;

typedef struct TBitStream {
	unsigned char byte;
	int size;
} BitStream;

Stack* newStack(Tree* node) {
	Stack* tmp = (Stack*)malloc(sizeof(Stack));
	assert(tmp);
	tmp->next = NULL;
	tmp->root = node;
	return tmp;
}

Tree* newTree(unsigned char sym, int freq) {
	Tree* tmp = (Tree*)malloc(sizeof(Tree));
	assert(tmp);
	tmp->freq = freq;
	tmp->sym = sym;
	tmp->left = NULL;
	tmp->right = NULL;
	return tmp;
}

Code* newCode(unsigned char* sym, int size) {
	Code* tmp = (Code*)malloc(sizeof(Code));
	assert(tmp);
	tmp->sym = sym;
	tmp->size = size;
	return tmp;
}

BitStream* newBitStream() {
	BitStream* tmp = (BitStream*)malloc(sizeof(BitStream));
	assert(tmp);
	tmp->byte = 0;
	tmp->size = 0;
	return tmp;
}

void countFreq(int* freqTable, FILE* in) {
	int c;
	while ((c = fgetc(in)) != EOF) {
		freqTable[c]++;
	}
}

void push(Stack** sortedStack, Tree* node) {
	int val = node->freq;
	Stack* tmp = newStack(node);
	if (!(*sortedStack) || (*sortedStack)->root->freq >= val) {
		tmp->next = *sortedStack;
		*sortedStack = tmp;
		return;
	}
	Stack* cur = *sortedStack;
	while (cur->next && val > cur->next->root->freq) {
		cur = cur->next;
	}
	tmp->next = cur->next;
	cur->next = tmp;
}

Tree* pop(Stack** stack) {
	Stack* tmp = *stack;
	Tree* sym = tmp->root;
	*stack = (*stack)->next;
	free(tmp);
	return sym;
}

void buildBase(Stack** stack, int* freqTable) {
	for (int i = 0; i < MAX_SIZE; i++) {
		if (freqTable[i]) {
			push(stack, newTree(i, freqTable[i]));
		}
	}
}

Tree* merge(Tree* first, Tree* second) {
	Tree* tmp = newTree(FILLER, first->freq + second->freq);
	tmp->left = second;
	tmp->right = first;
	return tmp;
}

Tree* buildCodeTree(Stack** stack) {
	if (!(*stack)) {
		return NULL;
	}
	while ((*stack)->next) {
		push(stack, merge(pop(stack), pop(stack)));
	}
	return pop(stack);
}

void resizeBuffer(Code* buffer) {
	buffer->size++;
	int isOverflow = buffer->size % CHAR_BIT;
	if (isOverflow == 1) {
		int bytes = buffer->size / CHAR_BIT + 1;
		buffer->sym = (unsigned char*)realloc(buffer->sym, bytes);
		buffer->sym[bytes - 1] = 0;
	}

}

unsigned char* copyCode(Code* buffer) {
	int bytes = (buffer->size - 1) / CHAR_BIT + 1;
	unsigned char* newSym = (unsigned char*)malloc(bytes);
	assert(newSym);
	for (int i = 0; i < bytes; i++) {
		newSym[i] = buffer->sym[i];
	}
	return newSym;
}

void addBit(Code* buffer, int bit) {
	int pos = (buffer->size - 1) / CHAR_BIT;
	buffer->sym[pos] = (buffer->sym[pos] << 1) + bit;
}

void buildCodeTable(Code** codeTable, Tree* codeTree, Code* buffer) {
	if (!codeTree) {
		return;
	}
	if (!codeTree->left) {
		if (!buffer->sym) {
			resizeBuffer(buffer);
		}
		codeTable[codeTree->sym] = buffer;
	}
	else {
		resizeBuffer(buffer);
		Code* newBuff = newCode(copyCode(buffer), buffer->size);
		addBit(buffer, 0);
		addBit(newBuff, 1);
		buildCodeTable(codeTable, codeTree->left, buffer);
		buildCodeTable(codeTable, codeTree->right, newBuff);
	}
}

void writeBits(unsigned char bits, int count, BitStream* bitStream, FILE* out) {
	if (!count) {
		return;
	}
	int shift = CHAR_BIT - bitStream->size;
	if (shift >= count) {
		bitStream->size += count;
		bitStream->byte <<= count;
		unsigned char mask = (1 << count) - 1;
		bitStream->byte += bits & mask;
		if (bitStream->size == CHAR_BIT) {
			fputc(bitStream->byte, out);
			bitStream->size = 0;
			bitStream->byte = 0;
		}
		return;
	}
	writeBits(bits >> (count - shift), shift, bitStream, out);
	writeBits(bits, count - shift, bitStream, out);
}

void writeTree(Tree* codeTree, BitStream* bitStream, FILE* out) {
	if (!codeTree) {
		return;
	}
	if (!codeTree->left) {
		writeBits(0, 1, bitStream, out);
		writeBits(codeTree->sym, CHAR_BIT, bitStream, out);
		return;
	}
	writeBits(1, 1, bitStream, out);
	writeTree(codeTree->left, bitStream, out);
	writeTree(codeTree->right, bitStream, out);
}

void codeText(FILE* in, FILE* out, Code** codeTable, BitStream* bitStream) {
	int c, i;
	while ((c = fgetc(in)) != EOF) {
		for (i = 0; i < codeTable[c]->size / CHAR_BIT; i++) {
			writeBits(codeTable[c]->sym[i], CHAR_BIT, bitStream, out);
		}
		writeBits(codeTable[c]->sym[i], codeTable[c]->size % CHAR_BIT, bitStream, out);
	}
	if (bitStream->size) {
		fputc(bitStream->byte << (CHAR_BIT - bitStream->size), out);
	}
}

void writeSize(unsigned int size, FILE* out) {
	fwrite(&size, sizeof(unsigned int), 1, out);
}

void freeCodeTable(Code** codeTable) {
	for (int i = 0; i < MAX_SIZE; i++) {
		if (codeTable[i]) {
			free(codeTable[i]->sym);
			free(codeTable[i]);
		}
	}
}

void freeTree(Tree* tree) {
	if (!tree) {
		return;
	}
	if (tree->left) {
		freeTree(tree->left);
	}
	if (tree->right) {
		freeTree(tree->right);
	}
	free(tree);
}

void code(FILE* in, FILE* out) {
	int freqTable[MAX_SIZE];
	Stack* stack = NULL;
	Code* codeTable[MAX_SIZE];
	Tree* codeTree = NULL;	
	BitStream* bitStream = newBitStream();
	memset(freqTable, 0, sizeof(int) * MAX_SIZE);
	memset(codeTable, NULL, sizeof(Code*) * MAX_SIZE);
	countFreq(freqTable, in);
	buildBase(&stack, freqTable);
	codeTree = buildCodeTree(&stack);
	if (codeTree) {
		writeSize(codeTree->freq, out);
	}
	writeTree(codeTree, bitStream, out);
	buildCodeTable(codeTable, codeTree, newCode(NULL, 0));
	fseek(in, 3, SEEK_SET);
	codeText(in, out, codeTable, bitStream);
	free(bitStream);
	freeCodeTable(codeTable);
	freeTree(codeTree);
}

unsigned char getBit(BitStream* bitStream, FILE* in) {
	if (!bitStream->size) {
		bitStream->byte = fgetc(in);
		bitStream->size = CHAR_BIT;
	}
	unsigned char bit = bitStream->byte >> 7;
	bitStream->size--;
	bitStream->byte <<= 1;
	return bit;
}

Tree* getCodeTree(FILE* in, BitStream* bitStream) {
	Tree* codeTree = NULL;
	if (getBit(bitStream, in)) {
		codeTree = newTree(FILLER, FILLER);
		codeTree->left = getCodeTree(in, bitStream);
		codeTree->right = getCodeTree(in, bitStream);
	}
	else {
		unsigned char sym = 0;
		for (int i = 0; i < CHAR_BIT; i++) {
			sym = (sym << 1) + getBit(bitStream, in);
		}
		codeTree = newTree(sym, FILLER);
	}
	return codeTree;
}

int readSize(FILE* in, unsigned int* size) {
	return fread(size, sizeof(unsigned int), 1, in);
}

void decodeText(Tree* codeTree, FILE* in, FILE* out, unsigned int size, BitStream* bitStream) {
	Tree* tmpCodeTree;
	for (unsigned int i = 0; i < size; i++) {
		tmpCodeTree = codeTree;
		while (tmpCodeTree->left) {
			if (getBit(bitStream, in)) {
				tmpCodeTree = tmpCodeTree->right;
			}
			else {
				tmpCodeTree = tmpCodeTree->left;
			}
		}
		fputc(tmpCodeTree->sym, out);
	}
}

void decode(FILE* in, FILE* out) {
	unsigned int size;
	if (!readSize(in, &size)) {
		return;
	}
	BitStream* bitStream = newBitStream();
	Tree* codeTree = getCodeTree(in, bitStream);
	decodeText(codeTree, in, out, size, bitStream);
	free(bitStream);
	freeTree(codeTree);
}

int main() {
	FILE* in = fopen("in.txt", "rb");
	FILE* out = fopen("out.txt", "wb");
	assert(in);
	assert(out);
	unsigned char mode = fgetc(in);
	fseek(in, 3, SEEK_SET);
	if (mode == 'c') {
		code(in, out);
	}
	else if (mode == 'd') {
		decode(in, out);
	}
	fclose(in);
	fclose(out);
	return 0;
}
