#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <limits.h>

#define MAX_NUMBER_OF_VERTICES 5000
#define DISCONNECTED (unsigned int)INT_MAX + 3
#define LONG_WAY (unsigned int)INT_MAX + 2
#define SEVERAL_LONG_WAYS (unsigned int)INT_MAX + 1

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

struct TWayInAndDistance {
	int parent;
	unsigned int edgeLength;
	unsigned int distance;
};

TGraph* readGraph(FILE* file, FILE* errorFile, unsigned int numberOfVertices, unsigned int numberOfEdges);
TWayInAndDistance* findWaysInAndDistances(TGraph* graph, int startVertex);
void printDistances(FILE* file, TWayInAndDistance* waysInAndDistances, int numberOfVertices);
void printShortestPath(FILE* file, int vertex, TWayInAndDistance* waysInAndDistances);
void updateWaysInAndDistances(TWayInAndDistance* waysInAndDistances, bool* ifVertexAdded, int newVertex, TGraph* graph);
unsigned int wayLengthsSum(unsigned int wayLength1, unsigned int wayLength2);
int nearestNewVertex(TWayInAndDistance* waysInAndDistances, bool* ifVertexAdded, int numberOfVertices);

int main() {
	FILE* inputFile = fopen("in.txt", "r");
	FILE* outputFile = fopen("out.txt", "w");
	if (inputFile == NULL || outputFile == NULL)
		return -1;

	int nodeNumber, arcNumber, startNode, finishNode;
	fscanf(inputFile, "%d%d%d%d", &nodeNumber, &startNode, &finishNode, &arcNumber);
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
	if (startNode < 1 || startNode > nodeNumber || finishNode < 1 || finishNode > nodeNumber){
		fprintf(outputFile, "bad vertex");
		fclose(outputFile);
		return 0;
	}
	--startNode; --finishNode;

	TGraph* graph = readGraph(inputFile, outputFile, nodeNumber, arcNumber);
	TWayInAndDistance* waysInAndDistances = findWaysInAndDistances(graph, startNode);
	if (waysInAndDistances == NULL)
		goto end;
	printDistances(outputFile, waysInAndDistances, graph->numberOfVertices);
	fprintf(outputFile, "\n");
	printShortestPath(outputFile, finishNode, waysInAndDistances);
	
end:
	if (graph != NULL)
		graph->freeGraph();
	if (waysInAndDistances != NULL)
		free(waysInAndDistances);
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

TWayInAndDistance* findWaysInAndDistances(TGraph* graph, int startVertex) {
	if (graph == NULL)
		return NULL;
	TWayInAndDistance* waysInAndDistances = (TWayInAndDistance*)malloc(sizeof(TWayInAndDistance) * graph->numberOfVertices);
	bool* ifVertexAdded = (bool*)malloc(sizeof(bool) * graph->numberOfVertices);

	if (waysInAndDistances == NULL || ifVertexAdded == NULL) {
		if (waysInAndDistances != NULL)
			free(waysInAndDistances);
		if (ifVertexAdded != NULL)
			free(ifVertexAdded);
		return NULL;
	}
	for (int i = graph->numberOfVertices - 1; i >= 0; --i) {
		waysInAndDistances[i].distance = DISCONNECTED;
		waysInAndDistances[i].edgeLength = DISCONNECTED;
		waysInAndDistances[i].parent = i;
		ifVertexAdded[i] = false;
	}

	waysInAndDistances[startVertex].edgeLength = 0;
	waysInAndDistances[startVertex].distance = 0;
	ifVertexAdded[startVertex] = true;
	updateWaysInAndDistances(waysInAndDistances, ifVertexAdded, startVertex, graph);
	unsigned int numberOfAddedVertices = 1;
	unsigned int newVertex;
	while (numberOfAddedVertices != graph->numberOfVertices) {
		newVertex = nearestNewVertex(waysInAndDistances, ifVertexAdded, graph->numberOfVertices);
		if (newVertex == -1)
			break;
		ifVertexAdded[newVertex] = true;
		updateWaysInAndDistances(waysInAndDistances, ifVertexAdded, newVertex, graph);
		++numberOfAddedVertices;
	}

	free(ifVertexAdded);
	return waysInAndDistances;
}

int nearestNewVertex(TWayInAndDistance* waysInAndDistances, bool* ifVertexAdded, int numberOfVertices) {
	int newVertex = -1;
	unsigned int newVertexDistance = DISCONNECTED;
	for (int i = 0; i != numberOfVertices; ++i)
		if (!ifVertexAdded[i] && waysInAndDistances[i].distance < newVertexDistance) {
			newVertex = i;
			newVertexDistance = waysInAndDistances[i].distance;
		}
	return newVertex;
}

void updateWaysInAndDistances(TWayInAndDistance* waysInAndDistances, bool* ifVertexAdded, int newVertex, TGraph* graph) {
	unsigned int newDistance;
	for (unsigned int i = 0; i != graph->numberOfVertices; ++i)
		if (!ifVertexAdded[i]){
		newDistance = wayLengthsSum(waysInAndDistances[newVertex].distance, graph->getLength(newVertex, i));
		if (newDistance < waysInAndDistances[i].distance) {
			waysInAndDistances[i].distance = newDistance;
			waysInAndDistances[i].parent = newVertex;
			waysInAndDistances[i].edgeLength = graph->getLength(newVertex, i);
		}
		else if (newDistance == LONG_WAY && waysInAndDistances[i].distance == LONG_WAY)
			waysInAndDistances[i].distance = SEVERAL_LONG_WAYS;
	}
}

unsigned int wayLengthsSum(unsigned int wayLength1, unsigned int wayLength2) {
	if (wayLength1 == DISCONNECTED || wayLength2 == DISCONNECTED)
		return DISCONNECTED;
	if (wayLength1 == SEVERAL_LONG_WAYS || wayLength2 == SEVERAL_LONG_WAYS)
		return SEVERAL_LONG_WAYS;
	if (wayLength1 == LONG_WAY || wayLength2 == LONG_WAY)
		return LONG_WAY;
	return ((wayLength1 + wayLength2 >= SEVERAL_LONG_WAYS) ? LONG_WAY : (wayLength1 + wayLength2));
}

void printDistances(FILE* file, TWayInAndDistance* waysInAndDistances, int numberOfVertices) {
	if (numberOfVertices == 0)
		return;
	for (int i = 0; i != numberOfVertices; ++i)
		switch (waysInAndDistances[i].distance) {
		case DISCONNECTED:
			fprintf(file, "oo ");
			break;
		case LONG_WAY:
		case SEVERAL_LONG_WAYS:
			fprintf(file, "INT_MAX+ ");
			break;
		default:
			fprintf(file, "%d ", waysInAndDistances[i].distance);
			break;
		}
}

void printShortestPath(FILE* file, int vertex, TWayInAndDistance* waysInAndDistances) {
	switch (waysInAndDistances[vertex].distance) {
	case DISCONNECTED:
		fprintf(file, "no path");
		break;
	case SEVERAL_LONG_WAYS:
		fprintf(file, "overflow");
		break;
	default:
		fprintf(file, "%d", vertex + 1);
		while (waysInAndDistances[vertex].parent != vertex) {
			vertex = waysInAndDistances[vertex].parent;
			fprintf(file, " %d", vertex + 1);
		}
	}
}

int KantorNumber(int number1, int number2) {
	return (((number1 + number2) * (number1 + number2) + number1 + 3 * number2) / 2);
}