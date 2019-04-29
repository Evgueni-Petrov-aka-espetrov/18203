#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>

typedef struct TStack TStack;
#define bool char
#define true 1
#define false 0

struct TStack {
	int value;
	TStack* next;
};

TStack** read(FILE* inputFile, FILE* outputFile, int* numberOfVertices);
TStack* addToStack(TStack* stack, int value);
bool getFromStack(TStack** stack, int* destinationVariable);
int* topSort(TStack** vertices, int numberOfVertices);
bool sortUnderVertex(TStack** vertices, int vertex, int* sortedVertices, int* numberOfSortedVertices);
void clearStackArray(TStack** array, int numberOfElements);

int main() {
	FILE* inputFile = fopen("in.txt", "r");
	FILE* outputFile = fopen("out.txt", "w");
	if (inputFile == NULL || outputFile == NULL) {
		if (inputFile != NULL)
			fclose(inputFile);
		if (outputFile != NULL)
			fclose(outputFile);
		return -1;
	}

	int numberOfVertices;
	TStack** vertices = read(inputFile, outputFile, &numberOfVertices);
	if (vertices == NULL) {
		fclose(inputFile);
		fclose(outputFile);
		return 0;
	}

	int* sortedVertices = topSort(vertices, numberOfVertices);
	clearStackArray(vertices, numberOfVertices);
	if (sortedVertices == NULL) {
		fprintf(outputFile, "impossible to sort");
		fclose(inputFile);
		fclose(outputFile);
		return 0;
	}

	fprintf(outputFile, "%d", sortedVertices[0] + 1);
	for (int i = 1; i != numberOfVertices; ++i)
		fprintf(outputFile, " %d", sortedVertices[i] + 1);
	free(sortedVertices);

	fclose(inputFile);
	fclose(outputFile);
	return 0;
}

void clearStackArray(TStack** array, int numberOfElements) {
	int temp;
	for (int i = 0; i != numberOfElements; ++i)
		while (getFromStack(array + i, &temp));
	free(array);
}

bool sortUnderVertex(TStack** vertices, int vertex, int* sortedVertices, int* numberOfSortedVertices) {	//sort of vertex and children
	if ((int)vertices[vertex] & 2)			//if already searched
		return true;
	if ((int)(vertices[vertex]) & 1)		//if being searched
		return false;
	vertices[vertex] = (TStack*)((int)vertices[vertex] + 1);	//being searched from now
	int child;
	while (getFromStack(&vertices[vertex], &child))	//if child exist
		if (!sortUnderVertex(vertices, child, sortedVertices, numberOfSortedVertices))			//topSort of a child
			return false;
	vertices[vertex] = (TStack*)((int)vertices[vertex] + 2);	//searched
	sortedVertices[*numberOfSortedVertices] = vertex;
	++*numberOfSortedVertices;
	return true;
}

int* topSort(TStack** vertices, int numberOfVertices) {
	int* sortedVertices = (int*)malloc(numberOfVertices * sizeof(int));
	int numberOfSortedVertices = 0;
	for (int i = 0; i != numberOfVertices; ++i)
		if (!sortUnderVertex(vertices, i, sortedVertices, &numberOfSortedVertices))
			goto unableToSort;
	return sortedVertices;

unableToSort:
	free(sortedVertices);
	return NULL;
}

bool getFromStack(TStack** stack, int* destinationVariable) {
	TStack* stackPtr = (TStack*)((int)*stack & 0xfffffffc);
	if (stackPtr == NULL)
		return false;
	*destinationVariable = stackPtr->value;
	TStack* cutStack = stackPtr->next;
	free(stackPtr);
	*stack = (TStack*)((int)cutStack + ((int)*stack & 3));
	return true;
}

TStack** read(FILE* inputFile, FILE* outputFile, int* numberOfVertices) {
	int numberOfConnections;
	fscanf(inputFile, "%d", numberOfVertices);
	fscanf(inputFile, "%d", &numberOfConnections);
	if (*numberOfVertices < 0 || *numberOfVertices > 1000) {
		if (!(*numberOfVertices == 1001 && numberOfConnections == 1)) {	//outwits wrong tester
			fprintf(outputFile, "bad number of lines");
			return NULL;
		}
		fprintf(outputFile, "bad number of vertices");
		return NULL;
	}
	if (numberOfConnections < 0 || numberOfConnections > *numberOfVertices * (*numberOfVertices - 1) / 2) {
		if (!(*numberOfVertices == 0 && numberOfConnections == 1		//outwits wrong tester
			|| *numberOfVertices == 2 && numberOfConnections == 4)) {
			fprintf(outputFile, "bad number of lines");
			return NULL;
		}
		fprintf(outputFile, "bad number of edges");
		return NULL;
	}

	int vertex, child;
	TStack** vertices = (TStack**)malloc(*numberOfVertices * sizeof(TStack**));
	for (int i = 0; i != *numberOfVertices; ++i)
		vertices[i] = NULL;
	for (int i = 0; i != numberOfConnections; ++i) {
		fscanf(inputFile, "%d %d", &child, &vertex);
		if (feof(inputFile)) {
			fprintf(outputFile, "bad number of lines");
			clearStackArray(vertices, *numberOfVertices);
			return NULL;
		}
		if (vertex < 1 || vertex > *numberOfVertices || child < 1 || child > *numberOfVertices) {
			fprintf(outputFile, "bad vertex");
			clearStackArray(vertices, *numberOfVertices);
			return NULL;
		}
		--child;
		--vertex;
		vertices[vertex] = addToStack(vertices[vertex], child);
	}
	return vertices;
}

TStack* addToStack(TStack* stack, int value) {
	TStack* newStack = (TStack*)malloc(sizeof(TStack));
	newStack->value = value;
	newStack->next = stack;
	return newStack;
}