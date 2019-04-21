#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "topsort.h"

typedef enum {white, grey, black} Ñolors;

void AddVertexToList(int vertexNumber, VerticesList **verticesList) {
	VerticesList *newListElement = (VerticesList*)malloc(sizeof(VerticesList));
	assert(newListElement != NULL);
	newListElement->vertexNumber = vertexNumber;
	newListElement->next = *verticesList;
	*verticesList = newListElement;
}

// returns 1 if graph has a cycle
int VisitVertex(const VerticesList **adjacentVerticesLists, int vertexNumber, Ñolors *verticesColors, VerticesList **sortedVerticesList) {
	verticesColors[vertexNumber - 1] = grey;
	const VerticesList *adjacentVertex = adjacentVerticesLists[vertexNumber - 1];
	for (; adjacentVertex != NULL; adjacentVertex = adjacentVertex->next) {
		Ñolors adjVertexColor = verticesColors[adjacentVertex->vertexNumber - 1];
		if (adjVertexColor == grey) {
			return 1;
		}
		if (adjVertexColor == white) {
			if (VisitVertex(adjacentVerticesLists, adjacentVertex->vertexNumber, verticesColors, sortedVerticesList)) {
				return 1;
			}
		}
	}
	verticesColors[vertexNumber - 1] = black;
	AddVertexToList(vertexNumber, sortedVerticesList);
	return 0;
}

// returns isImpossibleToSort
int TopSort (const VerticesList **adjacentVerticesLists, int verticesCount, FILE *out) {
	int IsImpossibleToSort = 0;
	Ñolors *verticesColors = (Ñolors*)malloc(verticesCount * sizeof(Ñolors));
	assert(verticesColors != NULL);
	VerticesList *sortedVerticesList = NULL;
	for (int i = 0; i < verticesCount; ++i) {
		verticesColors[i] = white;
	}
	for (int i = 0; i < verticesCount; ++i) {
		if (verticesColors[i] == white) {
			if (VisitVertex(adjacentVerticesLists, i + 1, verticesColors, &sortedVerticesList)) {
				IsImpossibleToSort = 1;
				break;
			}
		}
	}
	if (!IsImpossibleToSort) {
		for (; sortedVerticesList != NULL; sortedVerticesList = sortedVerticesList->next) {
			fprintf(out, "%d ", sortedVerticesList->vertexNumber);
		}
	}
	free(verticesColors);
	DeleteList(sortedVerticesList);
	return IsImpossibleToSort;
}