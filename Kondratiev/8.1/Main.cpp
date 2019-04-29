#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <limits.h>

#define MAX_NUMBER_OF_VERTICES 5000
#define DISCONNECTED (unsigned int)INT_MAX + 1

int KantorNumber(int number1, int number2);

struct TGraph {
	unsigned int numberOfVertices;
	unsigned int* edgeLengths;
	static TGraph* createAndInit(unsigned int numberOfVertices, unsigned int startEdgeLength) {
		if (numberOfVertices == 0)
			return NULL;
		TGraph* graph = (TGraph*)malloc(sizeof(TGraph));
		if (graph == NULL)
			return NULL;
		graph->numberOfVertices = numberOfVertices;
		unsigned int edgeNumber = (numberOfVertices * (numberOfVertices - 1) / 2);
		graph->edgeLengths = (unsigned int*)malloc(sizeof(unsigned int) * edgeNumber);
		unsigned int* edges = graph->edgeLengths;
		if (graph->edgeLengths == NULL && numberOfVertices != 1)
			goto error;
		for (unsigned int i = 0; i != edgeNumber; ++i)
			edges[i] = startEdgeLength;
		return graph;
	error:
		if (graph != NULL) {
			if (graph->edgeLengths != NULL)
				free(graph->edgeLengths);
			free(graph);
		}
		return NULL;
	}
	void freeGraph() {
		if (edgeLengths != NULL)
			free(edgeLengths);
		free(this);
	}
	void setLength(unsigned int vertex1, unsigned int vertex2, unsigned int length) {
		if (vertex1 == vertex2)
			return;
		if (vertex1 > vertex2) {
			int tmp = vertex1;
			vertex1 = vertex2;
			vertex2 = tmp;
		}
		edgeLengths[KantorNumber(vertex1, numberOfVertices - 1 - vertex2)] = length;
	}
	unsigned int getLength(unsigned int vertex1, unsigned int vertex2) {
		if (vertex1 == vertex2)
			return 0;
		if (vertex1 > vertex2) {
			int tmp = vertex1;
			vertex1 = vertex2;
			vertex2 = tmp;
		}
		return edgeLengths[KantorNumber(vertex1, numberOfVertices - 1 - vertex2)];
	}
};

struct TEdge {
	unsigned int length;
	unsigned int vertex1;
	unsigned int vertex2;
};

TGraph* readGraph(FILE* file, FILE* errorFile, unsigned int numberOfVertices, unsigned int numberOfEdges);
TEdge* findBase(TGraph* graph, FILE* errorFile);
void printEdgeArray(FILE* file, TEdge* edges, int arrayLength);
void updateDistancesAndParentVertices(unsigned int* distances, int* parentVertices, int newVertex, TGraph* graph);
int nearestNewVertex(unsigned int* distances, bool* ifVertexAdded, int numberOfVertices);

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

	TGraph* graph = readGraph(inputFile, outputFile, nodeNumber, arcNumber);
	TEdge* base = findBase(graph, outputFile);
	if (base == NULL)
		goto end;
	printEdgeArray(outputFile, base, graph->numberOfVertices - 1);

end:
	if (graph != NULL)
		graph->freeGraph();
	if (base != NULL)
		free(base);
	fclose(inputFile);
	fclose(outputFile);
	return 0;
}

TGraph* readGraph(FILE* file, FILE* errorFile, unsigned int numberOfVertices, unsigned int numberOfEdges) {
	if (file == NULL)
		return NULL;
	TGraph* graph = TGraph::createAndInit(numberOfVertices, DISCONNECTED);	//set vertices disconnected
	if (graph == NULL)
		return NULL;

	unsigned long long length;
	unsigned int vertex1, vertex2;
	for (unsigned int i = 0; i != numberOfEdges; ++i) {
		if (fscanf(file, "%d%d%llu", &vertex1, &vertex2, &length) < 3) {
			fprintf(errorFile, "bad number of lines");
			goto error;
		}
		if (vertex1 < 1 || vertex1 > numberOfVertices || vertex2 < 1 || vertex2 > numberOfVertices) {
			fprintf(errorFile, "bad vertex");
			goto error;
		}
		if (length < 0 || length > INT_MAX) {
			fprintf(errorFile, "bad length");
			goto error;
		}
		graph->setLength(vertex1 - 1, vertex2 - 1, (unsigned int)length);
	}
	return graph;

error:
	graph->freeGraph();
	return NULL;
}

TEdge* findBase(TGraph* graph, FILE* errorFile) {
	if (graph == NULL)
		return NULL;
	TEdge* baseEdges = (TEdge*)malloc(sizeof(TEdge) * graph->numberOfVertices - 1);
	unsigned int* distances = (unsigned int*)malloc(sizeof(unsigned int) * graph->numberOfVertices);
	int* parentVertices = (int*)malloc(sizeof(int) * graph->numberOfVertices);
	bool* ifVertexAdded = (bool*)malloc(sizeof(bool*) * graph->numberOfVertices);

	if (baseEdges == NULL || distances == NULL || parentVertices == NULL || ifVertexAdded == NULL) {
		if (baseEdges != NULL)
			free(baseEdges);
		if (distances != NULL)
			free(distances);
		if (parentVertices != NULL)
			free(parentVertices);
		if (ifVertexAdded != NULL)
			free(ifVertexAdded);
		return NULL;
	}
	for (int i = graph->numberOfVertices - 1; i >= 0; --i) {
		distances[i] = DISCONNECTED;
		ifVertexAdded[i] = false;
	}
	unsigned int numberOfAddedEdges = 0;

	distances[0] = 0;
	ifVertexAdded[0] = true;
	updateDistancesAndParentVertices(distances, parentVertices, 0, graph);
	unsigned int newVertex;
	while (numberOfAddedEdges != graph->numberOfVertices - 1) {
		newVertex = nearestNewVertex(distances, ifVertexAdded, graph->numberOfVertices);
		if (newVertex == -1) {
			fprintf(errorFile, "no spanning tree");
			free(distances);
			free(parentVertices);
			free(ifVertexAdded);
			free(baseEdges);
			return NULL;
		}
		baseEdges[numberOfAddedEdges].vertex1 = parentVertices[newVertex];
		baseEdges[numberOfAddedEdges].length = distances[newVertex];
		baseEdges[numberOfAddedEdges].vertex2 = newVertex;
		ifVertexAdded[newVertex] = true;
		updateDistancesAndParentVertices(distances, parentVertices, newVertex, graph);
		++numberOfAddedEdges;
	}

	free(distances);
	free(parentVertices);
	free(ifVertexAdded);
	return baseEdges;
}

int nearestNewVertex(unsigned int* distances, bool* ifVertexAdded, int numberOfVertices) {
	int newVertex = -1;
	unsigned int newVertexDistance = DISCONNECTED;
	for (int i = 0; i != numberOfVertices; ++i)
		if (!ifVertexAdded[i] && distances[i] < newVertexDistance) {
			newVertex = i;
			newVertexDistance = distances[i];
		}
	return newVertex;
}

void updateDistancesAndParentVertices(unsigned int* distances, int* parentVertices, int newVertex, TGraph* graph) {
	for (unsigned int i = 0; i != graph->numberOfVertices; ++i)
		if (graph->getLength(newVertex, i) < distances[i]) {
			distances[i] = graph->getLength(newVertex, i);
			parentVertices[i] = newVertex;
		}
}

void printEdgeArray(FILE* file, TEdge* edges, int arrayLength) {
	if (edges == NULL)
		return;
	for (int i = 0; i != arrayLength; ++i) {
		fprintf(file, "%d %d\n", edges[i].vertex1 + 1, edges[i].vertex2 + 1);
	}
}

int KantorNumber(int number1, int number2) {
	return (((number1 + number2) * (number1 + number2) + number1 + 3 * number2) / 2);
}