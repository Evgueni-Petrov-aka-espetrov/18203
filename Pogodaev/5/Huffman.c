#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "huffman.h"
#include "minprqueue.h"
#define FILLER 0
#define INTERNAL -1
#define END -1
#define CODE_MAX_SIZE 65 // 64 + edge
#define ULL_BIT (CHAR_BIT * sizeof(unsigned long long)) 
#define CHAR_AMOUNT 256

typedef struct {
	int buffer[CHAR_BIT];
	int position;
} BitStream;

typedef struct TTree{
	int symbol;
	unsigned long long frequence;
	struct TTree *leftChild;
	struct TTree *rightChild;
} TTree;

void CountFreq(FILE *fileToRead, unsigned long long freq[CHAR_AMOUNT]) {
	int c;
	long curPosInFile = ftell(fileToRead);
	for (int i = 0; i < CHAR_AMOUNT; ++i) {
		freq[i] = 0;
	}
	while ((c = fgetc(fileToRead)) != EOF) {
		++freq[c];
	}
	if (fseek(fileToRead, curPosInFile, SEEK_SET)) {
		fprintf(stderr, "file error");
		exit(0);
	}
	return;
}

TTree *CreateTreeNode(int symbol, unsigned long long frequence, TTree *leftChild, TTree *rightChild) {
	TTree *newNode = (TTree*)malloc(sizeof(TTree));
	newNode->symbol = symbol;
	newNode->frequence = frequence;
	newNode->leftChild = leftChild;
	newNode->rightChild = rightChild;
	return newNode;
}

void FillQueue(PQueue *queue, const unsigned long long freq[CHAR_AMOUNT]) {
	for (int i = 0; i < QMAXSIZE; ++i) {
		if (freq[i] != 0) {
			TTree *charNode = CreateTreeNode(i, freq[i], NULL, NULL);
			Insert(queue, charNode->frequence, (void*)charNode);
		}
	}
}

TTree *BuildCodeTree(PQueue *queue) {
	QueueElement firstMin;
	QueueElement secMin;
	if (ExtractMin(queue, &firstMin) == 0) {
		return NULL;
	}
	if (ExtractMin(queue, &secMin) == 0) {
		return (TTree*)firstMin.data;
	}
	TTree *firstChild = (TTree*)firstMin.data;
	TTree *secChild = (TTree*)secMin.data;
	TTree *newInternalNode = CreateTreeNode(
		INTERNAL, firstChild->frequence + secChild->frequence, firstChild, secChild);
	Insert(queue, newInternalNode->frequence, (void*)newInternalNode);
	return BuildCodeTree(queue);
}

void DeleteTree(TTree *root) {
	TTree *leftChild = root->leftChild;
	TTree *rightChild = root->rightChild;
	free(root);
	if (leftChild != NULL) DeleteTree(leftChild);
	if (rightChild != NULL) DeleteTree(rightChild);
	return;
}

// returns 1 if buffer can be pushed out into file
// returns 0 if not yet
int AddBitToStream(BitStream *bitStream, int bit) {
	if (bit != 1 && bit != 0) {
		return -1;
	}
	bitStream->buffer[bitStream->position] = bit;
	++bitStream->position;
	if (bitStream->position >= CHAR_BIT) {
		bitStream->position = 0;
		return 1;
	}
	return 0;
}

unsigned char ByteToChar(const int byte[CHAR_BIT]) {
	unsigned char result = 0;
	for (int i = 0; i < CHAR_BIT; ++i) {
		result *= 2;
		result += byte[i];
	}
	return result;
}

void CharToByte(unsigned char ch, int byte[CHAR_BIT]) {
	int i = CHAR_BIT - 1;
	while (ch > 0) {
		byte[i] = ch % 2;
		ch /= 2;
		--i;
	}
	while (i >= 0) {
		byte[i] = 0;
		--i;
	}
	return;
}

void LongIntToBytes(unsigned long long longInt, int bytes[ULL_BIT]) {
	int i = ULL_BIT - 1;
	while (longInt > 0) {
		bytes[i] = longInt % 2;
		longInt /= 2;
		--i;
	}
	while (i >= 0) {
		bytes[i] = 0;
		--i;
	}
	return;
}

unsigned long long BytesToLongInt(const int bytes[ULL_BIT]) {
	unsigned long long longInt = 0;
	for (int i = 0; i < ULL_BIT; ++i) {
		longInt *= 2;
		longInt += bytes[i];
	}
	return longInt;
}

void WriteCharToFile(unsigned char ch, BitStream *bitStream, FILE *fileToWrite) {
	int byte[CHAR_BIT];
	CharToByte(ch, byte);
	for (int i = 0; i < CHAR_BIT; ++i) {
		if (AddBitToStream(bitStream, byte[i])) {
			fputc(ByteToChar(bitStream->buffer), fileToWrite);
		}
	}
}

void WriteLongIntToFile(unsigned long long longInt, BitStream *bitStream, FILE *fileToWrite) {
	int bytes[ULL_BIT];
	LongIntToBytes(longInt, bytes);
	for (int i = 0; i < ULL_BIT; ++i) {
		if (AddBitToStream(bitStream, bytes[i])) {
			fputc(ByteToChar(bitStream->buffer), fileToWrite);
		}
	}
	return;
}

int GetBitFromFile(BitStream *bitStream, FILE *fileToRead) {
	if (bitStream->position >= CHAR_BIT) {
		bitStream->position = 0;
	}
	if (bitStream->position == 0) {
		int nextChar = fgetc(fileToRead);
		if (nextChar != EOF) {
			CharToByte(nextChar, bitStream->buffer);
		}
		else return EOF;
	}
	++bitStream->position;
	return bitStream->buffer[bitStream->position - 1];
}

int GetCharFromFile(BitStream *bitStream, FILE *fileToRead) {
	int nextChar[CHAR_BIT];
	for (int i = 0; i < CHAR_BIT; ++i) {
		nextChar[i] = GetBitFromFile(bitStream, fileToRead);
		if (nextChar[i] == EOF) {
			return EOF;
		}
	}
	return ByteToChar(nextChar);
}

unsigned long long GetLongIntFromFile(BitStream *bitStream, FILE *fileToRead) {
	int bytes[ULL_BIT];
	for (int i = 0; i < ULL_BIT; ++i) {
		bytes[i] = GetBitFromFile(bitStream, fileToRead);
	}
	return BytesToLongInt(bytes);
}

void Serialize(const TTree *codeTree, BitStream *bitStream, FILE *fileToWrite) {
	if (codeTree == NULL) {
		return;
	}
	if (codeTree->symbol != INTERNAL) {
		if (AddBitToStream(bitStream, 1)) {
			fputc(ByteToChar(bitStream->buffer), fileToWrite);
		}
		WriteCharToFile(codeTree->symbol, bitStream, fileToWrite);
	}
	else {
		if (AddBitToStream(bitStream, 0)) {
			fputc(ByteToChar(bitStream->buffer), fileToWrite);
		}
		Serialize(codeTree->leftChild, bitStream, fileToWrite);
		Serialize(codeTree->rightChild, bitStream, fileToWrite);
	}
	return;
}

int IsInBinTree(int ch, const TTree *tree) {
	if (tree == NULL) {
		return 0;
	}
	if (tree->symbol == ch) {
		return 1;
	}
	else {
		if (tree->leftChild != NULL) {
			assert(tree->rightChild != NULL);
			return (IsInBinTree(ch, tree->leftChild) || IsInBinTree(ch, tree->rightChild));
		}
		else {
			assert(tree->rightChild == NULL);
			return 0;
		}
	}
}

// returns 0 if there is only one node in code tree
// returns 1 otherwise
int GetCode(int ch, int codeString[CODE_MAX_SIZE], const TTree *codeTree) {
	int i = 0;
	while (codeTree->symbol == INTERNAL) {
		if (IsInBinTree(ch, codeTree->leftChild)) {
			codeString[i++] = 0;
			codeTree = codeTree->leftChild;
		}
		if (IsInBinTree(ch, codeTree->rightChild)) {
			codeString[i++] = 1;
			codeTree = codeTree->rightChild;
		}
	}
	codeString[i] = END;
	if (i == 0) {
		return 0;
	}
	return 1;
}

void Encode(FILE *fileToRead, const TTree *codeTree, BitStream *bitStream, FILE *fileToWrite) {
	WriteLongIntToFile(codeTree->frequence, bitStream, fileToWrite);
	int nextChar;
	int charCode[CODE_MAX_SIZE];
	while ((nextChar = fgetc(fileToRead)) != EOF) {
		if (!GetCode(nextChar, charCode, codeTree)) { // if there is only one symbol in file
			break;
		} 
		for (int i = 0; charCode[i] != END; ++i) {
			if (AddBitToStream(bitStream, charCode[i])) {
				fputc(ByteToChar(bitStream->buffer), fileToWrite);
			}
		}
	}
	if (bitStream->position != 0) {
		for (int i = bitStream->position; i < CHAR_BIT; ++i) {
			bitStream->buffer[i] = FILLER;
		}
		fputc(ByteToChar(bitStream->buffer), fileToWrite);
	}
	return;
}

void Compress(FILE *fileIn, FILE *fileOut) {
	unsigned long long frequences[CHAR_AMOUNT];
	CountFreq(fileIn, frequences);
	PQueue *queue = CreateQueue();
	FillQueue(queue, frequences);
	TTree *codeTree = BuildCodeTree(queue);
	DestroyQueue(queue);
	BitStream bitStream;
	bitStream.position = 0;
	Serialize(codeTree, &bitStream, fileOut);
	Encode(fileIn, codeTree, &bitStream, fileOut);
	DeleteTree(codeTree);
	fflush(fileOut);
	return;
}

TTree *Unserialize(FILE *fileToRead, BitStream *bitStream) {
	TTree *newNode;
	if (GetBitFromFile(bitStream, fileToRead) == 0) {
		newNode = CreateTreeNode(INTERNAL, FILLER, 
			Unserialize(fileToRead, bitStream), 
			Unserialize(fileToRead, bitStream));
	}
	else {
		newNode = CreateTreeNode(GetCharFromFile(bitStream, fileToRead), FILLER, NULL, NULL);
	}
	return newNode;
}

int GetDecodedCharFromFile(FILE *fileToRead, const TTree *codeTree, BitStream *bitStream) {
	while (codeTree->symbol == INTERNAL) {
		int nextBit = GetBitFromFile(bitStream, fileToRead);
		if (nextBit == 1) {
			codeTree = codeTree->rightChild;
		}
		else if (nextBit == 0) {
			codeTree = codeTree->leftChild;
		}
		else {
			return EOF;
		}
	}
	return codeTree->symbol;
}

void Decode(FILE *fileToRead, const TTree *codeTree, BitStream *bitStream, FILE *fileToWrite) {
	unsigned long long fileSize = GetLongIntFromFile(bitStream, fileToRead);
	if (codeTree->symbol != INTERNAL) {
		for (unsigned long long i = 0; i < fileSize; ++i) {
			fputc(codeTree->symbol, fileToWrite);
		}
	}
	else {
		for (unsigned long long i = 0; i < fileSize; ++i) {
			fputc(GetDecodedCharFromFile(fileToRead, codeTree, bitStream), fileToWrite);
		}
	}

}

void Decompress(FILE *fileIn, FILE *fileOut) {
	BitStream bitStream;
	bitStream.position = 0;
	TTree *codeTree = Unserialize(fileIn, &bitStream);
	Decode(fileIn, codeTree, &bitStream, fileOut);
	DeleteTree(codeTree);
	fflush(fileOut);
	return;
}
