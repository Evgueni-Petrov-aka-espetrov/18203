#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>

struct TNode;
struct TTree;
struct TNodePtrStack64;
int max(int a, int b);
TTree* createTree(int numberOfValues);

struct TNode {
	int value;
	int height;
	TNode* left;
	TNode* right;
};

struct TTree {
	TNode* nodeArray;
	int numberOfNodes = 0;
	int maxNumberOfNodes;
	TNode* root = NULL;
	void addToTree(int value);
	void addToNotEmptyTree(int value);
	char height(TNode* node) {
		if (node == NULL) {
			return 0;
		}
		else {
			return node->height;
		}
	}
	void balanceTree(TNode** treePtr);
	void updateHeight(TNode* node) {
		if (node != NULL)
			node->height = max(height(node->left), height(node->right)) + 1;
	}
	void freeTree() {
		free(nodeArray);
		free(this);
	}
};

struct TNodePtrStack64 {
	TNode* nodePtrArray[64];
	char numberOfElements = 0;
	char readingIndex = -1;
	void addPtr(TNode* ptr) {
		if (numberOfElements == 64) {
			printf("Unable to add: stack is full.");
			return;
		}
		readingIndex = numberOfElements;
		nodePtrArray[numberOfElements++] = ptr;
	}
	TNode* read() {
		if (readingIndex == -1) {
			printf("Unable to read: stack is empty.");
			return NULL;
		}
		return nodePtrArray[readingIndex--];
	}
	bool isRead() {
		if (readingIndex == -1) {
			return true;
		}
		else {
			return false;
		}
	}
};

int main()
{
	FILE* inputFile = fopen("in.txt", "r");
	FILE* outputFile = fopen("out.txt", "w");
	if (inputFile == NULL || outputFile == NULL) {
		printf("Can't open file.");
		return -1;
	}

	int numberOfNumbers;
	fscanf(inputFile, "%d", &numberOfNumbers);
	int tmp;
	TTree* tree = createTree(numberOfNumbers);
	for (int i = 0; i != numberOfNumbers; ++i) {
		fscanf(inputFile, "%d", &tmp);
		tree->addToTree(tmp);
	}
	fprintf(outputFile, "%d", tree->height(tree->root));
	tree->freeTree();

	fclose(inputFile);
	fclose(outputFile);
}

void TTree::balanceTree(TNode** treePtr) {
	char balanceType = 0;
	TNode* temp;
	if (height((*treePtr)->right) > height((*treePtr)->left)) {	//определение типа балансировки
		temp = (*treePtr)->right;
		if (height(temp->right) > height(temp->left)) {
			balanceType = 'r';
		}
		else {
			balanceType = 'R';
		}
	}
	else {
		temp = (*treePtr)->left;
		if (height(temp->left) > height(temp->right)) {
			balanceType = 'l';
		}
		else {
			balanceType = 'L';
		}
	}
	switch (balanceType) {
	case 'r':
		(*treePtr)->right = temp->left;	//балансировка
		temp->left = *treePtr;
		*treePtr = temp;
		updateHeight((*treePtr)->left);	//изменение высот
		updateHeight(*treePtr);
		break;
	case 'l':
		(*treePtr)->left = temp->right;	//балансировка
		temp->right = *treePtr;
		*treePtr = temp;
		updateHeight((*treePtr)->right);//изменение высот
		updateHeight(*treePtr);
		break;
	case 'R':
		temp = *treePtr;
		*treePtr = temp->right->left;	//балансировка
		temp->right->left = (*treePtr)->right;
		(*treePtr)->right = temp->right;
		temp->right = (*treePtr)->left;
		(*treePtr)->left = temp;
		updateHeight((*treePtr)->left);	//изменение высот
		updateHeight((*treePtr)->right);
		updateHeight(*treePtr);
		break;
	case 'L':
		temp = *treePtr;
		*treePtr = temp->left->right;	//балансировка
		temp->left->right = (*treePtr)->left;
		(*treePtr)->left = temp->left;
		temp->left = (*treePtr)->right;
		(*treePtr)->right = temp;
		updateHeight((*treePtr)->right);//изменение высот
		updateHeight((*treePtr)->left);
		updateHeight(*treePtr);
	}
}

int max(int a, int b) {
	if (a >= b) {
		return a;
	}
	else {
		return b;
	}
}

void TTree::addToNotEmptyTree(int value) {
	TNode* newNode = nodeArray + numberOfNodes;
	nodeArray[numberOfNodes++].value = value;
	TNodePtrStack64 path;
	TNode* currentNode = root;
	while (true) {
		path.addPtr(currentNode);
		if (value <= currentNode->value) {
			if (currentNode->left == NULL) {
				currentNode->left = newNode;
				break;
			}
			currentNode = currentNode->left;
		}
		else {
			if (currentNode->right == NULL) {
				currentNode->right = newNode;
				break;
			}
			currentNode = currentNode->right;
		}
	}
	char heightDifference;
	while (!path.isRead()) {
		currentNode = path.read();
		heightDifference = height(currentNode->left) - height(currentNode->right);
		if (heightDifference > 1 || heightDifference < -1) {
			if (path.isRead()) {			//вызов функции балансировки
				balanceTree(&root);
			}
			else {
				newNode = path.read();
				if (newNode->left == currentNode) {
					balanceTree(&newNode->left);
				}
				else {
					balanceTree(&newNode->right);
				}
			}
			break;		//upper nodes saves their heights
		}
		updateHeight(currentNode);
	}
}

void TTree::addToTree(int value) {
	if (numberOfNodes == maxNumberOfNodes) {
		printf("Tree ended.");
		return;
	}

	if (numberOfNodes == 0) {
		root = nodeArray;
		nodeArray[numberOfNodes++].value = value;
	}
	else {
		addToNotEmptyTree(value);
	}
}

TTree* createTree(int numberOfValues) {
	if (numberOfValues < 0)
		return NULL;
	TTree* tree = (TTree*)malloc(sizeof(TTree));
	tree->maxNumberOfNodes = numberOfValues;
	tree->numberOfNodes = 0;
	tree->root = NULL;
	tree->nodeArray = (TNode*)malloc(sizeof(TNode) * numberOfValues);
	for (int i = 0; i != numberOfValues; ++i) {
		tree->nodeArray[i].left = NULL;
		tree->nodeArray[i].right = NULL;
		tree->nodeArray[i].height = 1;
	}
	return tree;
}
