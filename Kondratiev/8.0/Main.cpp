#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

enum { MAX_NUMBER_OF_VERTICES = 5000 };

struct TArc;
struct TArcList;
struct TNumberUDSNode;
struct TNumberUDS;
int max(int number1, int number2);
TArcList* findGraphBase(TArcList* arcList, int nodeNumber);

struct TArc {
	short vertex1;
	short vertex2;
	unsigned int length;
};

struct TArcList {
	TArc* arcArray;
	unsigned int numberOfArcs;
	unsigned int currentPositionInList;
	static TArcList* readAndCheckArcs(FILE* file, int numberOfArcs, int numberOfVertices, FILE* errorFile) {
		TArcList* arcList = TArcList::createAndInit(numberOfArcs);
		if (arcList == NULL) {
			printf("<ERROR: can not allocate memory for arcList>\n");
			return NULL;
		}
		TArc* arcs = arcList->arcArray;
		long long length;
		int vertex1, vertex2;
		for (int i = 0; i != numberOfArcs; ++i) {
			if (fscanf(file, "%d%d%lld", &vertex1, &vertex2, &length) != 3) {
				fprintf(errorFile, "bad number of lines");
				goto badInput;
			}
			if (vertex1 < 1 || vertex2 < 1 || vertex1 > numberOfVertices || vertex2 > numberOfVertices) {
				fprintf(errorFile, "bad vertex");
				goto badInput;
			}
			if (length < 0 || length > INT_MAX) {
				fprintf(errorFile, "bad length");
				goto badInput;
			}
			arcs[i].vertex1 = vertex1 - 1;
			arcs[i].vertex2 = vertex2 - 1;
			arcs[i].length = length;
		}
		return arcList;
	badInput:
		free(arcs);
		free(arcList);
		return NULL;
	}
	static TArcList* createAndInit(int numberOfArcs) {
		if (numberOfArcs <= 0)
			return NULL;
		TArcList* arcList = (TArcList*)malloc(sizeof(TArcList));
		if (arcList == NULL)
			return NULL;
		TArc* arcs = (TArc*)malloc(sizeof(TArc) * numberOfArcs);
		if (arcs == NULL) {
			free(arcList);
			return NULL;
		}
		arcList->arcArray = arcs;
		arcList->numberOfArcs = numberOfArcs;
		arcList->currentPositionInList = 0;
		return arcList;
	}
	void copyCurrentArcOfAnotherList(TArcList* anotherArcList) {
		arcArray[currentPositionInList] = anotherArcList->arcArray[anotherArcList->currentPositionInList];
	}
	static int arcCompareFunction(const void* arc1, const void* arc2) {
		return (((const TArc*)arc1)->length - ((const TArc*)arc2)->length);
	}
	void sort() {
		qsort(arcArray, numberOfArcs, sizeof(TArc), arcCompareFunction);
	}
	int readVertex1() {
		return arcArray[currentPositionInList].vertex1;
	}
	int readVertex2() {
		return arcArray[currentPositionInList].vertex2;
	}
	void moveToNextArc() {
		++currentPositionInList;
	}
	bool endOfList() {
		return (currentPositionInList >= numberOfArcs);
	}
	void writeList(FILE* file) {
		for (int i = 0; i != numberOfArcs; ++i)
			fprintf(file, "%d %d\n", arcArray[i].vertex1 + 1, arcArray[i].vertex2 + 1);
	}
	void freeMem() {
		if (arcArray != NULL)
			free(arcArray);
		free(this);
	}
};

struct TNumberUDSNode {
	int nodeHeight;
	TNumberUDSNode* parent;
};

struct TNumberUDS {
	TNumberUDSNode* nodes;
	unsigned int nodeNumber;
	unsigned int numberOfGroups;
	static TNumberUDS* createAndInit(int numberOfNodes) {
		if (numberOfNodes < 0)
			return NULL;
		TNumberUDS* uds = (TNumberUDS*)malloc(sizeof(TNumberUDS));
		if (uds == NULL)
			return NULL;
		TNumberUDSNode* udsNodes = (TNumberUDSNode*)malloc(sizeof(TNumberUDSNode) * numberOfNodes);
		if (udsNodes == NULL) {
			free(uds);
			return NULL;
		}
		uds->nodes = udsNodes;
		uds->nodeNumber = numberOfNodes;
		uds->numberOfGroups = numberOfNodes;
		for (int i = 0; i != numberOfNodes; ++i) {
			udsNodes[i].nodeHeight = 1;
			udsNodes[i].parent = udsNodes + i;
		}
		return uds;
	}
	bool mergeGroups(int number1, int number2) {
		TNumberUDSNode* node1 = nodes + number1;
		TNumberUDSNode* node2 = nodes + number2;
		while (node1->parent != node1)
			node1 = node1->parent;
		while (node2->parent != node2)
			node2 = node2->parent;
		if (node1 == node2) {
			nodes[number1].parent = node1;
			nodes[number2].parent = node1;
			return false;
		}
		if (node2->nodeHeight < node1->nodeHeight) {
			node2->parent = node1;
			nodes[number1].parent = node1;
			nodes[number2].parent = node1;
		}
		else {
			node1->parent = node2;
			node2->nodeHeight = max(node2->nodeHeight, node1->nodeHeight + 1);
			nodes[number1].parent = node2;
			nodes[number2].parent = node2;
		}
		--numberOfGroups;
		return true;
	}
	bool isMergedInOneGroup() {
		return (numberOfGroups == 1);
	}
	void freeMem() {
		if (nodes != NULL)
			free(nodes);
		free(this);
	}
};

int main() {
	FILE* inputFile = fopen("in.txt", "r");
	FILE* outputFile = fopen("out.txt", "w");
	if (inputFile == NULL || outputFile == NULL)
		return -1;

	int nodeNumber, arcNumber;
	fscanf(inputFile, "%d%d", &nodeNumber, &arcNumber);
	if (nodeNumber < 0 || nodeNumber > MAX_NUMBER_OF_VERTICES) {
		fprintf(outputFile, "bad number of vertices");
		fclose(inputFile);
		fclose(outputFile);
		return 0;
	}
	if (arcNumber < 0 || arcNumber > nodeNumber * (nodeNumber - 1) / 2) {
		fprintf(outputFile, "bad number of edges");
		fclose(inputFile);
		fclose(outputFile);
		return 0;
	}
	if (nodeNumber == 0 || arcNumber < nodeNumber - 1) {
		fprintf(outputFile, "no spanning tree");
		fclose(outputFile);
		return 0;
	}
	if (nodeNumber == 1) {
		fclose(outputFile);
		return 0;
	}
	TArcList* arcList = TArcList::readAndCheckArcs(inputFile, arcNumber, nodeNumber, outputFile);
	if (arcList == NULL) {
		fclose(inputFile);
		fclose(outputFile);
		return 0;
	}
	TArcList* graphBaseArcList = findGraphBase(arcList, nodeNumber);
	if (graphBaseArcList == NULL) {
		fprintf(outputFile, "no spanning tree");
		fclose(inputFile);
		fclose(outputFile);
		return 0;
	}
	graphBaseArcList->writeList(outputFile);
	graphBaseArcList->freeMem();
	arcList->freeMem();
	fclose(inputFile);
	fclose(outputFile);
	return 0;
}

TArcList* findGraphBase(TArcList* arcList, int nodeNumber) {
	arcList->sort();
	TNumberUDS* vertexUDS = TNumberUDS::createAndInit(nodeNumber);
	if (vertexUDS == NULL) {
		printf("<ERROR: can not allocate memory for vertexUDS>\n");
		return NULL;
	}
	TArcList* baseArcs = TArcList::createAndInit(nodeNumber - 1);
	if (baseArcs == NULL) {
		free(vertexUDS);
		printf("<ERROR: can not allocate memory for baseArcs>\n");
		return NULL;
	}
	bool merged;
	while (!arcList->endOfList() && !vertexUDS->isMergedInOneGroup()) {
		merged = vertexUDS->mergeGroups(arcList->readVertex1(), arcList->readVertex2());
		if (merged) {
			baseArcs->copyCurrentArcOfAnotherList(arcList);
			baseArcs->moveToNextArc();
		}
		arcList->moveToNextArc();
	}
	if (!vertexUDS->isMergedInOneGroup())
		return NULL;
	return baseArcs;
}

int max(int number1, int number2) {
	if (number1 > number2) {
		return number1;
	}
	else {
		return number2;
	}
}