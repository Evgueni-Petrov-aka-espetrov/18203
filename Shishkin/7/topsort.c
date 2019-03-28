#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <assert.h>
int readamount(FILE *in){
	int c = 0;
	int amount = 0;
	int noteof = 0;
	int minus = 0;
	c = fgetc(in);
	while ((c != '\n')&&(c != EOF)){
		noteof++;
		if (c == '-') minus++;
		else amount = amount * 10 + (c - '0');
		c = fgetc(in);
	}
	if (minus == 1) amount = amount * (-1);
	if (noteof == 0) amount = -1;
	return amount;
}
int getgraph(char *graph, FILE *in, int vert, int *res){
	int minus;
	int lines = 2;
	int errors = 0;
	int c = fgetc(in);
	while (c != EOF) {
		int from = 0;
		int to = 0;
		minus = 0;
		while ((c != ' ') && (c != EOF) && (c != '\n')){
			if (c == '-') minus++;
			else from = from * 10 + (c - '0');
			c = fgetc(in);
		}
		if (minus == 1) from = from * (-1);
		if ((from >= 0)&&(from <= vert)) graph[vert*from + from] = 1;
		//fprintf(stderr, "%d  ", from);
		c = fgetc(in);
		minus = 0;
		while ((c != '\n') && (c != EOF)) {
			if (c == '-') minus++;
			else to = to * 10 + (c - '0');
			c = fgetc(in);
		}
		if (minus == 1) to = to * (-1);
		if ((to > 0)&&(to <= vert)) graph[vert*to + to] = 1;
		//fprintf(stderr, "%d  ", to);
		c = fgetc(in);
		if (from == to) res[0] = -2;
		if ((from > vert) || (to > vert) || (to < 0) || (from < 0)) {
			errors = 1;
			break;
		}
		else {
			graph[from*vert + to] = 1;
			lines++;
		}
	}
	//place 1 on diagonal
	int i;
	for (i = 1; i <= vert; i++){
		graph[vert*i + i] = 1;
	}

	if (errors == 0) return lines;
	else return 0;
}

int findbeg(char *graph, int vert) {
	int i = 1;
	int result = -1;
	int existence = 0;
	for (i; i <= vert; i++) {
		int counter = 0;
		int j = 1;
		for (j; j <= vert; j++) {
			if (graph[j*vert + i] == 1) counter++;
		}
		if (counter == 1) result = i;
		if (counter > 1) existence = counter;
	}
	if ((result == -1) && (existence > 1)) result = -2;
	return result;
}

void topsort(char *graph, int *res, int verticies) {
	int counter = 1;
	int head = findbeg(graph, verticies);
	//fprintf(stderr, "head: %d ", head);
	while (head >= 1) {
		res[counter] = head;
		counter++;
		int i;
		for (i = 1; i <= verticies; i++){
			graph[verticies*head + i] = 0;
		}
		head = findbeg(graph, verticies);
		//fprintf(stderr, "head: %d ", head);
	}
	if (head == -1) res[counter] = -1;
	if (head == -2) res[0] = head;
}
void print(int *res, int vert, FILE *out){
	int i = 1;
	//for (i = 1; i < vert; i++){
	//	fprintf(out, "%d ", res[i]);
	//}
	while (res[i] >= 0) {
		fprintf(out, "%d ", res[i]);
		i++;
	}
	//fprintf(out, "%d", res[vert]);
}
int main() {
	FILE *fin = fopen("in.txt", "r");
	FILE *fout = fopen("out.txt", "w");
	assert(fin != NULL);
	int verticies = readamount(fin);
	fprintf(stderr, "vert: %d  ", verticies);
	if (verticies > 1000) fprintf(fout, "bad number of vertices");
	else {
		int edges = readamount(fin); 
		fprintf(stderr, "edges: %d  ", edges);
		if (edges > (verticies*(verticies - 1) / 2)) fprintf(fout, "bad number of edges");
		else {
			char *graph = (char*)malloc((verticies+1)*(verticies+1)*sizeof(char));
			int *result = (int*)malloc((verticies + 2)*sizeof(int));
			assert(graph != NULL);
			assert(result != NULL);
			int lines = getgraph(graph, fin, verticies, result);
			if (edges == -1) lines = 1;
			fprintf(stderr, "lines after getgraph: %d  ", lines);
			if (lines == 0) {
				fprintf(fout, "bad vertex");
				fprintf(stderr, "bad vertex");
			}
			if (lines != 0) {
				//fprintf(stderr, "fjgjknhkjgfkhj");
				if (edges == -1) edges = 0;
				if (lines < edges + 2) fprintf(fout, "bad number of lines");
				else {
					topsort(graph, result, verticies);
					if (result[0] == -2) fprintf(fout, "impossible to sort");
					else {
						print(result, verticies, fout);
					}
				}
			}
			free(graph);
			free(result);
		}
	}
	return 0;
}