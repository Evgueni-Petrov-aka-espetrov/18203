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
	char* storage;
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
	newBitArray->storage = (char*)malloc(sizeof(char)*byteCount);
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
		Array->storage = (char*)realloc(Array->storage, Array->bitCount / 8 + 1);
	}
	char mask = ((char)1) << (7 - (Array->bitCount % 8));
	if (toPush == 0) {
		Array->storage[Array->bitCount / 8] &= (~mask);
	}
	else {
		Array->storage[Array->bitCount / 8] |= mask;
	}
	++(Array->bitCount);
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