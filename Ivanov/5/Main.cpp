#include <stdio.h>
#include <malloc.h>

struct TTreeNode{
	int weight;
	char symbol;
	TTreeNode* left;
	TTreeNode* right;
};

struct TTreeNodePtrListNode{
	TTreeNode* TTreeNodePtr;
	TTreeNodePtrListNode* next;
};

TTreeNodePtrListNode* getTreeNodeListPtrFromStat(int charStat[256]){
	TTreeNodePtrListNode* firstListNodePtr = NULL;
	for (char i = 0; i <= 255; ++i){
		if (charStat[i] != 0){
			TTreeNode* newNodePtr = (TTreeNode*)malloc(sizeof(TTreeNode));
			newNodePtr->weight = charStat[i];
			newNodePtr->symbol = i;
			newNodePtr->left = NULL;
			newNodePtr->right = NULL;

			TTreeNodePtrListNode* newListNode = (TTreeNodePtrListNode) // ... ...
		}
	}
}

TTreeNode* getTreeRootPtr(int charStat[256]){
	TTreeNode* result = NULL;

}

int main(){
	FILE* fileInput = fopen("in.txt", "r");
	int charStat[256] = { 0 };
	for (int currentChar = fgetc(fileInput); currentChar != EOF; currentChar = fgetc(fileInput)){
		++(charStat[currentChar]);
	}
	TTreeNode* treeRootPtr = getTreeRootPtr(charStat);
}