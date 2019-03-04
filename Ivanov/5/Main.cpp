#include <stdio.h>
#include <malloc.h>

struct TTreeNode {
	int weight;
	char symbol;
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

void pushTreeNodeToQueue(TTreeNode* toPush, TTreeNodeQueue* queue){
	TTreeNodePtrListNode* newNode = (TTreeNodePtrListNode*)malloc(sizeof(TTreeNodePtrListNode));
	newNode->next = NULL;
	if (queue->head == NULL){
		queue->head = newNode;
		queue->tail = newNode;
	}
	else{
		queue->tail->next = newNode;
		queue->tail = newNode;
	}
	newNode->TTreeNodePtr = toPush;
}

TTreeNode* popTreeNodeFromQueue(TTreeNodeQueue* queue){
	if (queue->head == NULL){
		return NULL;
	}
	TTreeNodePtrListNode* oldNode = queue->head;
	TTreeNode* toReturn = oldNode->TTreeNodePtr;
	if (queue->head->next == NULL){

	}
	queue->head = queue->head->next;
	free(oldNode);
	return toReturn;
}

TTreeNodePtrListNode* getTreeNodeListPtrFromStat(int charStat[256]) {
	TTreeNodePtrListNode* firstListNodePtr = NULL;
	for (int i = 0; i <= 255; ++i) {
		if (charStat[i] != 0) {
			TTreeNode* newNodePtr = (TTreeNode*)malloc(sizeof(TTreeNode));  //this would be ergonomic in c++
			newNodePtr->weight = charStat[i];
			newNodePtr->symbol = (char)i;
			newNodePtr->left = NULL;
			newNodePtr->right = NULL;

			TTreeNodePtrListNode* newListNode = (TTreeNodePtrListNode*)malloc(sizeof(TTreeNodePtrListNode));
			newListNode->next = NULL;
			newListNode->TTreeNodePtr = newNodePtr;

			if (firstListNodePtr == NULL) {
				firstListNodePtr = newListNode;
			}
			else {
				TTreeNodePtrListNode* currentListNodePtr = firstListNodePtr;
				for (; currentListNodePtr->next != NULL; currentListNodePtr = currentListNodePtr->next);
				currentListNodePtr->next = newListNode;
			}
		}
	}
	return firstListNodePtr;
}

TTreeNodePtrListNode* findMinWeightTreeNodePtrListNodePtr(TTreeNodePtrListNode* firstListNodePtr) {
	TTreeNodePtrListNode* result = firstListNodePtr;
	for (TTreeNodePtrListNode* listIterator = result->next; listIterator != NULL; listIterator = listIterator->next) {
		if (listIterator->TTreeNodePtr->weight < result->TTreeNodePtr->weight) {
			result = listIterator;
		}
	}
	return result;
}

TTreeNode* getTreeRootPtr(int charStat[256]) {
	TTreeNodePtrListNode* firstListNodePtr = getTreeNodeListPtrFromStat(charStat); //need checking
	for (; firstListNodePtr->next != NULL;) {
		TTreeNodePtrListNode* min1 = findMinWeightTreeNodePtrListNodePtr(firstListNodePtr);

		for (TTreeNodePtrListNode** ptrIterator = &firstListNodePtr;; ptrIterator = &((*ptrIterator)->next)) {
			if (*ptrIterator == min1) {
				(*ptrIterator) = min1->next;
				min1->next = NULL;
				break;
			}
		}

		TTreeNodePtrListNode* min2 = findMinWeightTreeNodePtrListNodePtr(firstListNodePtr);

		TTreeNode* newTreeNodePtr = (TTreeNode*)malloc(sizeof(TTreeNode));
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

struct TBitArray {
	unsigned char* storage;
	int bitCount;
};

TBitArray* cloneBitArray(const TBitArray* original) {
	TBitArray* newBitArray = (TBitArray*)malloc(sizeof(TBitArray));
	newBitArray->bitCount = original->bitCount;
	if (original->bitCount == 0) {
		newBitArray->storage = NULL;
		return newBitArray;
	}
	int byteCount = ((original->bitCount - 1) / 8) + 1;
	newBitArray->storage = (unsigned char*)malloc(sizeof(char)*byteCount);
	for (int i = 0; i < byteCount; ++i) {
		newBitArray->storage[i] = original->storage[i];
	}
	return newBitArray;
}
void pushBitToBitArray(TBitArray* Array, int toPush);

void buildBitMapFromTree(TBitArray** storagePtr, TTreeNode* currentRootPtr, TBitArray* currentCode) {
	if (currentRootPtr->left != NULL) {
		TBitArray* newBitArray = cloneBitArray(currentCode);
		pushBitToBitArray(currentCode, 1);
		pushBitToBitArray(newBitArray, 0);
		buildBitMapFromTree(storagePtr, currentRootPtr->left, currentCode);
		buildBitMapFromTree(storagePtr, currentRootPtr->right, newBitArray);
	}
	else {
		storagePtr[currentRootPtr->symbol] = currentCode;
	}
}



void pushBitToBitArray(TBitArray* Array, int toPush) {
	if (Array->bitCount % 8 == 0) {
		Array->storage = (unsigned char*)realloc(Array->storage, Array->bitCount / 8 + 1);
	}
	char mask = ((unsigned char)1) << (Array->bitCount % 8);
	if (toPush == 0) {
		Array->storage[Array->bitCount / 8] &= (~mask);
	}
	else {
		Array->storage[Array->bitCount / 8] |= mask;
	}
	++(Array->bitCount);
}

void pushBitsToOutputStream(FILE* fileStream, TBitArray* buffer, TBitArray* toPush){
	*(buffer->storage) &= (~((unsigned char)0)) >> (8 - buffer->bitCount);
	int writeCount = (buffer->bitCount + toPush->bitCount) / 8;
	for (int i = 0; i < writeCount; ++i){
		*(buffer->storage) |= toPush->storage[i] << (buffer->bitCount);
		fputc(*(buffer->storage), fileStream);
		*(buffer->storage) &= (unsigned char)0;
		*(buffer->storage) |= toPush->storage[i] >> (8 - buffer->bitCount);
	}
	if (writeCount * 8<(toPush->bitCount)){
		*(buffer->storage) |= (toPush->storage[writeCount] << buffer->bitCount);
	}
	buffer->bitCount = (buffer->bitCount + toPush->bitCount) % 8;
}

void encode(TTreeNode* root){
	TBitArray* buffer = (TBitArray*)malloc(sizeof(TBitArray));
	buffer->storage = (unsigned char*)malloc(sizeof(unsigned char));
	buffer->bitCount = 3;


}

int main() {
	/*FILE* fileInput = fopen("in.txt", "r");
	*/int charStat[256] = { 0 };/*
	for (int currentChar = fgetc(fileInput); currentChar != EOF; currentChar = fgetc(fileInput)) {
	++(charStat[currentChar]);
	}*/
char debugInput[40];
scanf("%s", debugInput);

for (int i = 0; i<39; ++i) {
	++(charStat[debugInput[i]]);
}
TTreeNode* treeRootPtr = getTreeRootPtr(charStat);

TBitArray** bitMapPtrArray = (TBitArray**)calloc(256, sizeof(TBitArray*));
TBitArray* initialCode = (TBitArray*)malloc(sizeof(TBitArray));
initialCode->bitCount = 0;
initialCode->storage = NULL;
buildBitMapFromTree(bitMapPtrArray, treeRootPtr, initialCode);



return 0;
}