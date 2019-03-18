#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>

struct TNode;
struct TTree;
struct TOrderedPNodeStack;
struct TBitStream;
void compress(FILE* in, FILE* out);
void decompress(FILE* in, FILE* out);
void swap(TNode** nodePtr1, TNode** nodePtr2);
void swap(int* value1, int* value2);
bool generateSymbolCodes(TNode* symbolTreeRoot, int* symbolCodesArray);
bool _generateSymbolCodes(TNode* root, int* codesArray, int way, char depth);
void printTree(TBitStream* bitStream, TNode* tree);
void _readNode(TBitStream* bitStream, TTree* tree, TNode** pNextNode);
TTree* readTree(TBitStream* bitStream);
int readSymbol(TBitStream* bitStream, TNode* treeRoot);
unsigned int readInt(FILE* file);
void writeInt(FILE* file, unsigned int value);
void prepareTreeForOutput(TTree* tree, TNode** leafPtrs);
void _prepareTreeForOutput(TNode* node, TNode** leafPtrs);
void writeSymbol(TNode* symbolNode, TBitStream* bitStream);

struct TNode {
	int value;
	TNode* child1;
	TNode* child2;
};

struct TTree {
	TNode* nodes;
	TNode* root;
	int maxNumberOfNodes;
	int numberOfNodes;
	static TTree* createAndInit(int size) {
		if (size < 0)
			return NULL;
		TTree* tree = (TTree*)malloc(sizeof(TTree));
		tree->nodes = (TNode*)malloc(sizeof(TNode) * size);
		if (tree == NULL || tree->nodes == NULL)
			return NULL;
		tree->maxNumberOfNodes = size;
		tree->numberOfNodes = 0;
		for (int i = 0; i != size; ++i) {
			tree->nodes[i].child1 = NULL;
			tree->nodes[i].child2 = NULL;
		}
		tree->root = NULL;
		return tree;
	}
	TNode* addNode(int value) {
		if (numberOfNodes == maxNumberOfNodes)
			return NULL;
		nodes[numberOfNodes++].value = value;
		return (nodes + numberOfNodes - 1);
	}
	TNode* unite(TNode* node1, TNode* node2) {
		if (addNode(0) == NULL)
			return NULL;
		TNode* parent = nodes + numberOfNodes - 1;
		parent->child1 = node1;
		parent->child2 = node2;
		return parent;
	}
	void freeTree() {
		free(nodes);
		free(this);
	}
};

struct TOrderedPNodeStack {
	TNode** pNodes;
	int* keys;
	int maxNumberOfNodes;
	int numberOfNodes;
	static TOrderedPNodeStack* createAndInit(int size) {
		if (size < 0)
			return NULL;
		TOrderedPNodeStack* stack = (TOrderedPNodeStack*)malloc(sizeof(TOrderedPNodeStack));
		stack->pNodes = (TNode**)malloc(sizeof(TNode**) * size);
		stack->keys = (int*)malloc(sizeof(int*) * size);
		if (stack == NULL || stack->pNodes == NULL || stack->keys == NULL)
			return NULL;
		stack->maxNumberOfNodes = size;
		stack->numberOfNodes = 0;
		return stack;
	}
	bool add(TNode* pNode, int key) {
		if (numberOfNodes == maxNumberOfNodes)
			return false;
		pNodes[numberOfNodes] = pNode;
		keys[numberOfNodes] = key;
		for (int i = numberOfNodes; i != 0; --i)
			if (keys[i] > keys[i - 1]) {
				swap(keys + i, keys + i - 1);
				swap(pNodes + i, pNodes + i - 1);
			}
			else {
				break;
			}
		++numberOfNodes;
		return true;
	}
	bool uniteLastTwo(TTree* tree) {
		if (numberOfNodes < 2)
			return false;
		numberOfNodes -= 2;
		add(tree->unite(pNodes[numberOfNodes], pNodes[numberOfNodes + 1]), keys[numberOfNodes] + keys[numberOfNodes + 1]);
		return true;
	}
	TNode* get() {
		if (numberOfNodes == NULL)
			return NULL;
		return pNodes[--numberOfNodes];
	}
	void freeStack() {
		free(pNodes);
		free(keys);
		free(this);
	}
};

struct TBitStream {
	unsigned long long buffer;
	char numberOfFilledBits;
	FILE* targetFile;
	static TBitStream* createAndInit(FILE* targetFile) {
		if (targetFile == NULL)
			return NULL;
		TBitStream* stream = (TBitStream*)malloc(sizeof(TBitStream));
		stream->buffer = 0;
		stream->numberOfFilledBits = 0;
		stream->targetFile = targetFile;
		return stream;
	}
	void write(int value, char numberOfBits) {
		buffer += (unsigned long long)value << (sizeof(unsigned long long) * 8 - numberOfBits) >> numberOfFilledBits;
		numberOfFilledBits += numberOfBits;
		if (numberOfFilledBits >= sizeof(int) * 8) {
			writeInt(targetFile, (unsigned int)(buffer >> (sizeof(unsigned long long) * 4)));
			buffer = buffer << sizeof(unsigned long long) * 4;
			numberOfFilledBits -= sizeof(unsigned long long) * 4;
		}
	}
	void writeTail() {
		writeInt(targetFile, (unsigned int)(buffer >> (sizeof(unsigned long long) * 4)));
		buffer = 0;
		numberOfFilledBits = 0;
	}
	int read(char numberOfBits) {
		unsigned int buf = 0;
		if (numberOfFilledBits < numberOfBits) {
			buf = readInt(targetFile);
			buffer += (unsigned long long)buf << (sizeof(unsigned long long) * 4 - numberOfFilledBits);
			numberOfFilledBits += sizeof(unsigned long long) * 4;
		}
		buf = buffer >> (sizeof(unsigned long long) * 8 - numberOfBits);
		buffer = buffer << numberOfBits;
		numberOfFilledBits -= numberOfBits;
		return buf;
	}
	bool readOneBit() {
		unsigned int buf = 0;
		if (numberOfFilledBits == 0) {
			buf = readInt(targetFile);
			buffer += (unsigned long long)buf << (sizeof(unsigned long long) * 4 - numberOfFilledBits);
			numberOfFilledBits += sizeof(unsigned long long) * 4;
		}
		buf = (int)(buffer >> (sizeof(unsigned long long) * 8 - 1));
		buffer = buffer << 1;
		numberOfFilledBits -= 1;
		return buf;
	}
	void freeStream() {
		free(this);
	}
};

int main() {
	FILE* in = fopen("in.txt", "rb");
	FILE* out = fopen("out.txt", "wb");
	if (in == NULL || out == NULL) {
		FILE* error = fopen("error.txt", "w");
		if (error == NULL)
			return -2;
		fprintf(error, "Can't open files: ");
		if (in == NULL) {
			if (out == NULL) {
				fprintf(error, "in.txt, out.txt.");
			}
			else {
				fprintf(error, "in.txt.");
			}
		}
		else {
			fprintf(error, "out.txt.");
		}
		fclose(error);
		return -1;
	}

	unsigned char mode = fgetc(in);
	fgetc(in); fgetc(in);
	switch (mode) {
	case 'c':
		compress(in, out);
		break;
	case 'd':
		decompress(in, out);
		break;
	case 't':
		compress(in, out);
		fclose(in);
		fclose(out);
		in = fopen("out.txt", "rb");
		out = fopen("out2.txt", "wb");
		decompress(in, out);
	}

	fclose(in);
	fclose(out);
}

void compress(FILE* in, FILE* out) {
	int textSize = 0;
	int numberOfSymbols[512] = { 0 };
	int temp = fgetc(in);			//чтение файла и подсчёт символов
	if (temp == EOF) {
		writeInt(out, 0);
		return;
	}
	while (temp != EOF) {
		++numberOfSymbols[temp];
		temp = fgetc(in);
		++textSize;
	}
	temp = 0;						//построение дерева
	for (int symbol = 0; symbol != 256; ++symbol)
		if (numberOfSymbols[symbol] != 0)
			++temp;
	TTree* symbolTree = TTree::createAndInit(2 * temp - 1);
	TOrderedPNodeStack* stack = TOrderedPNodeStack::createAndInit(temp);
	for (int symbol = 0; symbol != 256; ++symbol)
		if (numberOfSymbols[symbol] != 0)
			stack->add(symbolTree->addNode(symbol), numberOfSymbols[symbol]);
	while (stack->uniteLastTwo(symbolTree));
	symbolTree->root = stack->get();
#define symbolPtrs (TNode**)numberOfSymbols		//вывод
	writeInt(out, textSize);
	fseek(in, 3, SEEK_SET);
	TBitStream* oBitStream = TBitStream::createAndInit(out);
	printTree(oBitStream, symbolTree->root);
	prepareTreeForOutput(symbolTree, symbolPtrs);
	for (int i = 0; i != textSize; ++i)
		writeSymbol(*(symbolPtrs + fgetc(in)), oBitStream);
	oBitStream->writeTail();
#undef symbolPtrs
	symbolTree->freeTree();
	stack->freeStack();
	oBitStream->freeStream();
}

void writeSymbol(TNode* symbolNode, TBitStream* bitStream) {
	if (symbolNode->child1 == NULL)
		return;
	writeSymbol(symbolNode->child1, bitStream);
	bitStream->write(symbolNode->child1->child2 == symbolNode, 1);
}

void _prepareTreeForOutput(TNode* node, TNode** leafPtrs){
	if (node->child1 == NULL) {
		leafPtrs[node->value] = node;
		return;
	}
	_prepareTreeForOutput(node->child1, leafPtrs);
	node->child1->child1 = node;
	_prepareTreeForOutput(node->child2, leafPtrs);
	node->child2->child1 = node;
}

void prepareTreeForOutput(TTree* tree, TNode** leafPtrs) {
	_prepareTreeForOutput(tree->root, leafPtrs);
	tree->root->child1 = NULL;
}

void decompress(FILE* in, FILE* out) {
	unsigned int textSize = readInt(in);
	if (textSize == 0)
		return;
	TBitStream* iBitStream = TBitStream::createAndInit(in);
	TTree* symbolTree = readTree(iBitStream);
	for (int i = 0; i != textSize; ++i)
		fputc(readSymbol(iBitStream, symbolTree->root), out);
	iBitStream->freeStream();
	symbolTree->freeTree();
}

int readSymbol(TBitStream* bitStream, TNode* treeRoot) {
	if (treeRoot->child1 == NULL)
		return treeRoot->value;
	if (bitStream->readOneBit() == 0) {
		return readSymbol(bitStream, treeRoot->child1);
	}
	else {
		return readSymbol(bitStream, treeRoot->child2);
	}
}

void _readNode(TBitStream* bitStream, TTree* tree, TNode** pNextNode) {
	int buffer = bitStream->readOneBit();
	if (buffer == 0) {
		*pNextNode = tree->addNode(bitStream->read(8));
		return;
	}
	*pNextNode = tree->addNode(0);
	_readNode(bitStream, tree, &(*pNextNode)->child1);
	_readNode(bitStream, tree, &(*pNextNode)->child2);
}

TTree* readTree(TBitStream* bitStream) {
	TTree* tree = TTree::createAndInit(256 + 255);
	_readNode(bitStream, tree, &tree->root);
	return tree;
}

void printTree(TBitStream* bitStream, TNode* node) {
	if (node->child1 == NULL) {
		bitStream->write(node->value, 9);
		return;
	}
	bitStream->write(1, 1);
	printTree(bitStream, node->child1);
	printTree(bitStream, node->child2);
}

bool _generateSymbolCodes(TNode* root, int* codesArray, int way, char depth) {
	if (root == NULL)
		return false;
	_generateSymbolCodes(root->child1, codesArray, way << 1, depth + 1);
	if (!_generateSymbolCodes(root->child2, codesArray, (way << 1) + 1, depth + 1))
		codesArray[root->value] = way + (depth << 24);
	return true;
}

bool generateSymbolCodes(TNode* symbolTreeRoot, int* symbolCodesArray) {
	if (symbolTreeRoot == NULL || symbolCodesArray == NULL)
		return false;
	for (int i = 0; i != 256; ++i)
		symbolCodesArray[i] = 0;
	_generateSymbolCodes(symbolTreeRoot, symbolCodesArray, 0, 0);
	return true;
}

unsigned int readInt(FILE* file) {
	unsigned int buffer;
	fread((char*)&buffer, sizeof(buffer), 1, file);
	return buffer;
}

void writeInt(FILE* file, unsigned int value) {
	fwrite((char*)&value, sizeof(value), 1, file);
}

void swap(TNode** nodePtr1, TNode** nodePtr2) {
	TNode* temp;
	temp = *nodePtr1;
	*nodePtr1 = *nodePtr2;
	*nodePtr2 = temp;
}

void swap(int* value1, int* value2) {
	int temp;
	temp = *value1;
	*value1 = *value2;
	*value2 = temp;
}