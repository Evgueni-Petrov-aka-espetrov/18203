#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

struct TTreeNode {
	int weight;
	unsigned char symbol;
	TTreeNode* left;
	TTreeNode* right;
};

struct TTreeNodePtrListNode {
	TTreeNode* TTreeNodePtr;
	TTreeNodePtrListNode* next;
};

struct TTreeNodeQueue {
	TTreeNodePtrListNode* head;
	TTreeNodePtrListNode* tail;
};

struct TBitArray {
	unsigned char* storage;
	int bitCount;
};

void pushTreeNodeToQueue(TTreeNode* toPush, TTreeNodeQueue* queue);
TTreeNode* popTreeNodeFromQueue(TTreeNodeQueue* queue);

void pushBitToBitArray(TBitArray* Array, int toPush);
int popBitFromBitArray(TBitArray* array);
TBitArray* cloneBitArray(const TBitArray* original);

TTreeNodePtrListNode* buildTreeNodeList(int charStat[256]);
TTreeNodePtrListNode* findMinWeight(TTreeNodePtrListNode* firstListNodePtr);
TTreeNode* buildTree(int charStat[256]);
void buildBitMap(TBitArray** storagePtr, TTreeNode* currentRootPtr, TBitArray* currentCode);

TBitArray* encodeTree(TTreeNode* rootPtr);
TTreeNode* decodeTree(FILE* inputStream, TBitArray* currentBuffer);

void pushBitsToOutputStream(FILE* fileOutput, TBitArray* buffer, TBitArray* toPush);
int takeBitFromInputStream(FILE* fileInput, TBitArray* buffer);
void flushOutputBitStream(FILE* fileOutput, TBitArray* buffer);
unsigned char takeByteFromInputStream(FILE* inputStream, TBitArray* buffer);
unsigned char decodeCharFromInputStream(FILE* fileInput, TBitArray* buffer, TTreeNode* root);

void compress(TTreeNode* rootPtr, TBitArray** bitMapPtrArray, FILE* fileInput, FILE* fileOutput);
void decompress(FILE* fileInput, FILE* fileOutput);

void getFileSize(FILE* stream);
int myFeof(FILE* stream);
void* safeMalloc(size_t byteCount);
void* safeRealloc(void* ptr, size_t newSize);

const unsigned char uCharZeroes = 0;
const unsigned char uCharOnes = (unsigned char)(~((unsigned char)0));
long bytesLeft;

void getFileSize(FILE* stream){
	long currentPos = ftell(stream);
	fseek(stream, 0, SEEK_END);
	bytesLeft = ftell(stream);
	fseek(stream, currentPos, SEEK_SET);
}

int myFeof(FILE* stream) { ////////////////////////////////
	return !bytesLeft;
}

void* safeMalloc(size_t byteCount){
	void* result = malloc(byteCount);
	if (result == NULL){
		printf("out of memory");
		exit(1);
	}
	else{
		return result;
	}
}

void* safeRealloc(void* ptr, size_t newSize){
	void* result = realloc(ptr, newSize);
	if (newSize != 0 && result == NULL){
		printf("out of memory");
		exit(1);
	}
	else{
		return result;
	}
}

void pushTreeNodeToQueue(TTreeNode* toPush, TTreeNodeQueue* queue) {
	TTreeNodePtrListNode* newNode = (TTreeNodePtrListNode*)safeMalloc(sizeof(TTreeNodePtrListNode));
	newNode->next = NULL;
	if (queue->head == NULL) {
		queue->head = newNode;
		queue->tail = newNode;
	}
	else {
		queue->tail->next = newNode;
		queue->tail = newNode;
	}
	newNode->TTreeNodePtr = toPush;
}

TTreeNode* popTreeNodeFromQueue(TTreeNodeQueue* queue) {
	if (queue->head == NULL) {
		return NULL;
	}
	TTreeNodePtrListNode* oldNode = queue->head;
	TTreeNode* result = oldNode->TTreeNodePtr;
	if (oldNode->next == NULL) {
		queue->tail = NULL;
	}
	queue->head = oldNode->next;
	free(oldNode);
	return result;
}

TTreeNodePtrListNode* buildTreeNodeList(int charStat[256]) {
	TTreeNodePtrListNode* firstListNodePtr = NULL;
	TTreeNodePtrListNode* lastListNodePtr = NULL;
	for (int i = 0; i <= 255; ++i) {
		if (charStat[i] != 0) {
			TTreeNode* newNodePtr = (TTreeNode*)safeMalloc(sizeof(TTreeNode));
			newNodePtr->weight = charStat[i];
			newNodePtr->symbol = (unsigned char)i;
			newNodePtr->left = NULL;
			newNodePtr->right = NULL;

			TTreeNodePtrListNode* newListNode = (TTreeNodePtrListNode*)safeMalloc(sizeof(TTreeNodePtrListNode));
			newListNode->next = NULL;
			newListNode->TTreeNodePtr = newNodePtr;

			if (firstListNodePtr == NULL) {
				firstListNodePtr = newListNode;
				lastListNodePtr = newListNode;
			}
			else {
				lastListNodePtr->next = newListNode;
				lastListNodePtr = newListNode;
			}
		}
	}
	return firstListNodePtr;
}

TTreeNodePtrListNode* findMinWeight(TTreeNodePtrListNode* firstListNodePtr) {
	TTreeNodePtrListNode* result = firstListNodePtr;
	for (TTreeNodePtrListNode* listIterator = result->next; listIterator != NULL; listIterator = listIterator->next) {
		if (listIterator->TTreeNodePtr->weight < result->TTreeNodePtr->weight) {
			result = listIterator;
		}
	}
	return result;
}

TTreeNode* buildTree(int charStat[256]) {
	TTreeNodePtrListNode* firstListNodePtr = buildTreeNodeList(charStat); //need checking
	for (; firstListNodePtr->next != NULL;) {
		TTreeNodePtrListNode* min1 = findMinWeight(firstListNodePtr);

		for (TTreeNodePtrListNode** ptrIterator = &firstListNodePtr;; ptrIterator = &((*ptrIterator)->next)) {
			if (*ptrIterator == min1) {
				(*ptrIterator) = min1->next;
				min1->next = NULL;
				break;
			}
		}

		TTreeNodePtrListNode* min2 = findMinWeight(firstListNodePtr);

		TTreeNode* newTreeNodePtr = (TTreeNode*)safeMalloc(sizeof(TTreeNode));
		newTreeNodePtr->left = min1->TTreeNodePtr;
		newTreeNodePtr->right = min2->TTreeNodePtr;
		newTreeNodePtr->weight = min1->TTreeNodePtr->weight + min2->TTreeNodePtr->weight;

		free(min1);
		min2->TTreeNodePtr = newTreeNodePtr;
	}
	TTreeNode* result = firstListNodePtr->TTreeNodePtr;
	free(firstListNodePtr);
	return result;
}

void buildBitMap(TBitArray** storagePtr, TTreeNode* currentRootPtr, TBitArray* currentCode) {
	if (currentRootPtr->left != NULL) {
		TBitArray* newBitArray = cloneBitArray(currentCode);
		pushBitToBitArray(currentCode, 1);
		pushBitToBitArray(newBitArray, 0);
		buildBitMap(storagePtr, currentRootPtr->left, currentCode);
		buildBitMap(storagePtr, currentRootPtr->right, newBitArray);
	}
	else {
		storagePtr[currentRootPtr->symbol] = currentCode;
	}
}

TBitArray* encodeTree(TTreeNode* rootPtr) {
	TBitArray* result = (TBitArray*)safeMalloc(sizeof(TBitArray));
	result->bitCount = 0;
	result->storage = NULL;

	TTreeNodeQueue* queue = (TTreeNodeQueue*)safeMalloc(sizeof(TTreeNodeQueue));
	queue->head = NULL;
	queue->tail = NULL;
	pushTreeNodeToQueue(rootPtr, queue);

	for (TTreeNode* currentNodePtr; queue->head != NULL;) {
		currentNodePtr = popTreeNodeFromQueue(queue);
		if (currentNodePtr->left != NULL) {
			pushBitToBitArray(result, 1);
			pushTreeNodeToQueue(currentNodePtr->left, queue);
			pushTreeNodeToQueue(currentNodePtr->right, queue);
		}
		else {
			pushBitToBitArray(result, 0);
			for (int i = 0; i < 8; ++i) {
				unsigned char mask = (unsigned char)1 << i;
				pushBitToBitArray(result, (currentNodePtr->symbol & mask) ? 1 : 0);
			}
		}
	}

	free(queue);
	return result;
}

TTreeNode* decodeTree(FILE* inputStream, TBitArray* currentBuffer) {
	TTreeNode* root = (TTreeNode*)safeMalloc(sizeof(TTreeNode));
	root->left = NULL;
	root->right = NULL;

	TTreeNodeQueue* queue = (TTreeNodeQueue*)safeMalloc(sizeof(TTreeNodeQueue));
	queue->head = NULL;
	queue->tail = NULL;

	pushTreeNodeToQueue(root, queue);
	for (TTreeNode* currentTreeNodePtr; queue->head != NULL;) {
		currentTreeNodePtr = popTreeNodeFromQueue(queue);
		if (takeBitFromInputStream(inputStream, currentBuffer) == 1) {
			currentTreeNodePtr->left = (TTreeNode*)safeMalloc(sizeof(TTreeNode));
			currentTreeNodePtr->left->left = NULL;
			currentTreeNodePtr->left->right = NULL;
			currentTreeNodePtr->right = (TTreeNode*)safeMalloc(sizeof(TTreeNode));
			currentTreeNodePtr->right->left = NULL;
			currentTreeNodePtr->right->right = NULL;
			pushTreeNodeToQueue(currentTreeNodePtr->left, queue);
			pushTreeNodeToQueue(currentTreeNodePtr->right, queue);
		}
		else {
			currentTreeNodePtr->symbol = takeByteFromInputStream(inputStream, currentBuffer);
		}
	}

	free(queue);
	return root;
}

TBitArray* fb = NULL;
int ctrl = 0;

TBitArray* cloneBitArray(const TBitArray* original) {
	TBitArray* newBitArray = (TBitArray*)safeMalloc(sizeof(TBitArray));
	newBitArray->bitCount = original->bitCount;
	if (original->bitCount == 0) {
		newBitArray->storage = NULL;
		return newBitArray;
	}
	int byteCount = ((original->bitCount + 7) / 8);
	newBitArray->storage = (unsigned char*)safeMalloc(sizeof(unsigned char)*byteCount);
	for (int i = 0; i < byteCount; ++i) {
		newBitArray->storage[i] = original->storage[i];
	}
	return newBitArray;
}

void pushBitToBitArray(TBitArray* Array, int toPush) {
	if (Array->bitCount % 8 == 0) {
		Array->storage = (unsigned char*)safeRealloc(Array->storage, Array->bitCount / 8 + 1);
	}
	unsigned char mask = ((unsigned char)1) << (Array->bitCount % 8);
	if (toPush == 0) {
		Array->storage[Array->bitCount / 8] &= (~mask);
	}
	else {
		Array->storage[Array->bitCount / 8] |= mask;
	}
	++(Array->bitCount);
}

int popBitFromBitArray(TBitArray* array) {
	if (array->bitCount == 0) {
		return 0;
	}
	--(array->bitCount);
	unsigned char mask = (unsigned char)1 << (7 - (array->bitCount % 8));
	int result = (array->storage[array->bitCount / 8] & mask) ? 1 : 0;
	if (array->bitCount % 8 == 0) {
		array->storage = (unsigned char*)safeRealloc(array->storage, array->bitCount / 8);
	}
	return result;
}

void pushBitsToOutputStream(FILE* fileOutput, TBitArray* buffer, TBitArray* toPush) {
	*(buffer->storage) &= uCharOnes >> (8 - buffer->bitCount);
	int writeCount = (buffer->bitCount + toPush->bitCount) / 8;
	for (int i = 0; i < writeCount; ++i) {
		*(buffer->storage) |= toPush->storage[i] << (buffer->bitCount);
		fwrite(buffer->storage, sizeof(unsigned char), 1, fileOutput);
		if (ctrl == 0) {
			ctrl = 1;
			fb = cloneBitArray(buffer);
		}
		*(buffer->storage) &= uCharZeroes;
		*(buffer->storage) |= toPush->storage[i] >> (8 - buffer->bitCount);
	}
	if (writeCount * 8<(toPush->bitCount)) {
		*(buffer->storage) |= (toPush->storage[writeCount] << buffer->bitCount);
	}
	buffer->bitCount = (buffer->bitCount + toPush->bitCount) % 8;
}

int takeBitFromInputStream(FILE* fileInput, TBitArray* buffer) {
	if (buffer->bitCount == 0) {
		buffer->storage = (unsigned char*)safeRealloc(buffer->storage, sizeof(unsigned char));
		fread(buffer->storage, sizeof(unsigned char), 1, fileInput);
		--bytesLeft;
		buffer->bitCount = 8;
	}
	return popBitFromBitArray(buffer);
}

void flushOutputBitStream(FILE* fileOutput, TBitArray* buffer) {
	unsigned char paddingSize;
	if (buffer->bitCount == 0) {
		paddingSize = 0;
	}
	else {
		paddingSize = 8 - buffer->bitCount;
		fwrite(buffer->storage, sizeof(unsigned char), 1, fileOutput);
	}

	rewind(fileOutput);
	if (!fb) return;
	unsigned char firstByte = *(fb->storage);
	firstByte &= ((~((unsigned char)0)) << 3);
	firstByte |= paddingSize;
	rewind(fileOutput);
	fwrite(&firstByte, sizeof(unsigned char), 1, fileOutput);
}

unsigned char takeByteFromInputStream(FILE* inputStream, TBitArray* buffer) {
	unsigned char result = 0;
	for (int i = 0; i < 8; ++i) {
		if (takeBitFromInputStream(inputStream, buffer)) {
			result |= ((unsigned char)1 << i);
		}
	}
	return result;
}

unsigned char decodeCharFromInputStream(FILE* fileInput, TBitArray* buffer, TTreeNode* root) {
	if (root->left == NULL) {
		takeBitFromInputStream(fileInput, buffer);
		return root->symbol;
	}

	TTreeNode* currentNode = root;
	while (currentNode->left != NULL) {
		if (takeBitFromInputStream(fileInput, buffer) != 0) {
			currentNode = currentNode->left;
		}
		else {
			currentNode = currentNode->right;
		}
	}
	return currentNode->symbol;
}

void compress(TTreeNode* rootPtr, TBitArray** bitMapPtrArray, FILE* fileInput, FILE* fileOutput) {
	TBitArray* buffer = (TBitArray*)malloc(sizeof(TBitArray));
	buffer->storage = (unsigned char*)malloc(sizeof(unsigned char));
	buffer->bitCount = 3;

	TBitArray* encodedTree = encodeTree(rootPtr);
	pushBitsToOutputStream(fileOutput, buffer, encodedTree);
	free(encodedTree);

	rewind(fileInput);
	char buf[5];
	fgets(buf, 5, fileInput);
	while (!feof(fileInput)) {
		unsigned char currentSymbol;
		if (!fread(&currentSymbol, sizeof(unsigned char), 1, fileInput)) break;
		pushBitsToOutputStream(fileOutput, buffer, bitMapPtrArray[currentSymbol]);
	}
	flushOutputBitStream(fileOutput, buffer);
	free(buffer->storage);
	free(buffer);
}

void decompress(FILE* fileInput, FILE* fileOutput) {
	TBitArray* buffer = (TBitArray*)malloc(sizeof(TBitArray));
	buffer->bitCount = 5;
	buffer->storage = (unsigned char*)malloc(sizeof(unsigned char));
	fread(buffer->storage, sizeof(unsigned char), 1, fileInput);
	bytesLeft--;
	unsigned char paddingSize = (*(buffer->storage) & (((unsigned char)(~((unsigned char)0))) >> 5)); /////////////////
	TTreeNode* root = decodeTree(fileInput, buffer);
	while (!(myFeof(fileInput) && (buffer->bitCount == paddingSize))) {
		unsigned char inp = decodeCharFromInputStream(fileInput, buffer, root);
		fwrite(&inp, sizeof(unsigned char), 1, fileOutput);
	}
}

int main() {
	FILE* fileInput = fopen("in.txt", "rb");
	FILE* fileOutput = fopen("out.txt", "wb");
	if (fileInput == NULL || fileOutput == NULL) {
		return 1;
	}

	char buf[5];
	if (!fgets(buf, 5, fileInput)) return 1;
	if (buf[0] == 'c') {
		int charStat[256] = { 0 };
		while (!feof(fileInput)) {
			unsigned char currentSymbol;
			fread(&currentSymbol, sizeof(unsigned char), 1, fileInput);
			++(charStat[currentSymbol]);
		}

		TTreeNode* treeRootPtr = buildTree(charStat);
		TBitArray** bitMapPtrArray = (TBitArray**)calloc(256, sizeof(TBitArray*));
		TBitArray* initialCode = (TBitArray*)malloc(sizeof(TBitArray));
		initialCode->bitCount = 0;
		initialCode->storage = NULL;
		if (treeRootPtr->left != NULL) {
			buildBitMap(bitMapPtrArray, treeRootPtr, initialCode);
		}
		else {
			pushBitToBitArray(initialCode, 1);
			bitMapPtrArray[treeRootPtr->symbol] = initialCode;
		}
		compress(treeRootPtr, bitMapPtrArray, fileInput, fileOutput);
	}
	else {
		getFileSize(fileInput);
		bytesLeft -= 3;
		decompress(fileInput, fileOutput);
	}

	fclose(fileInput);
	fclose(fileOutput);

	return 0;
}