#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <stdlib.h>
#include <limits.h>
typedef struct list{
	int to;
	int from;
	int length;
	char usage;
} list;
int readamount(FILE *in){
	int c = 0;
	int amount = 0;
	int noteof = 0;
	int minus = 0;
	c = fgetc(in);
	while ((c != '\n') && (c != EOF)){
		noteof++;
		if (c == '-') minus++;
		else amount = amount * 10 + (c - '0');
		c = fgetc(in);
	}
	if (minus == 1) amount = amount * (-1);
	if (noteof == 0) amount = -1;
	return amount;
}
int getgraph(list *graph, FILE *in, int vert, int *res){
	int minus;
	int lines = 2;
	int errors = 0;
	int c = fgetc(in);
	int counter = 0;
	while (c != EOF) {
		int from = 0;
		int to = 0;
		int len = 0;
		minus = 0;
		while ((c != ' ') && (c != EOF) && (c != '\n')){
			if (c == '-') minus++;
			else from = from * 10 + (c - '0');
			c = fgetc(in);
		}
		if (minus == 1) from = from * (-1);
		c = fgetc(in);
		minus = 0;
		while ((c != ' ') && (c != EOF)) {
			if (c == '-') minus++;
			else to = to * 10 + (c - '0');
			c = fgetc(in);
		}
		if (minus == 1) to = to * (-1);
		c = fgetc(in);
		minus = 0;
		while ((c != '\n') && (c != EOF)) {
			if (c == '-') minus++;
			else len = len * 10 + (c - '0');
			c = fgetc(in);
		}
		if (minus == 1) len = len * (-1);
		c = fgetc(in);
		if ((from <= vert) && (to <= vert) && (to >= 0) && (from >= 0) && (len >= 0) && (len <= INT_MAX)) {
			graph[counter].length = len;
			graph[counter].from = from;
			graph[counter].to = to;
			graph[counter].usage = 0;
			lines++;
			counter++;
		}
		else {
			if ((len < 0) || (len > INT_MAX)) *res = 1;
			if ((from > vert) || (to > vert) || (to < 0) || (from < 0)) *res = 2;
		}
	}
	if (*res == 0) return lines;
	else return 0;
}
int compare(const void *a, const void *b){
	list *c = (list*)a;
	list *d = (list*)b;
	return (c->length - d->length);
}
void kruskal(list *graph, int vert, int edges, int *groups, FILE *fout){
	qsort(graph, edges, sizeof(list), compare);
	int i = 0;
	for (i; i < vert; i++){
		groups[i] = i;
	}
	for (i = 0; i < edges; i++) {
		int to = graph[i].to;
		int from = graph[i].from;
		if (groups[to - 1] != groups[from - 1]) {
			int j = 0;
			int was = groups[to - 1];
			int will = groups[from - 1];
			for (j; j < vert; j++){
				if (groups[j] == was) groups[j] = will;
			}
			graph[i].usage = 1;
		}
	}
	int value = groups[0];
	int difference = 0;
	for (i = 1; i < vert; i++){
		if (groups[i] != value) {
			difference = 1;
			break;
		}
	}
	if (difference == 1) fprintf(fout, "no spanning tree");
	else {
		for (i = 0; i < edges; i++){
			if (graph[i].usage == 1) fprintf(fout, "%d %d\n", graph[i].from, graph[i].to);
		}
	}
}
int findmin(list *graph, int *groups, int edges){
	int result = 0;
	int i = 0;
	int end = 0;
	while ((end != 1)&&(i < edges)) {
		int to = graph[i].to;
		int from = graph[i].from;
		if (groups[to-1] != groups[from-1]) {
			graph[i].usage = 1;
			groups[to-1] = 1;
			groups[from-1] = 1;
			end = 1;
		}
		else i++;
	}
	if (i == edges) result = 1;
	return result;
}
void prim(list *graph, int vert, int edges, int *groups, FILE *fout){
	qsort(graph, edges, sizeof(list), compare);
	int i = 1;
	int usedvert = 1;
	for (i; i < vert; i++){
		groups[i] = 0;
	}
	groups[0] = 1;
	while (findmin(graph, groups, edges) != 1) {
		usedvert++;
	};
	if (usedvert < vert) fprintf(fout, "no spanning tree");
	else {
		for (i = 0; i < edges; i++){
			if (graph[i].usage == 1) fprintf(fout, "%d %d\n", graph[i].from, graph[i].to);
		}
	}
}
int main(){
	FILE *fin = fopen("in.txt", "r");
	FILE *fout = fopen("out.txt", "w");
	assert(fin != NULL);
	int vert = readamount(fin);
	if ((vert < 0) || (vert > 5000)) fprintf(fout, "bad number of vertices");
	else {
		int edges = readamount(fin);
		if ((edges < 0) || (edges > vert*(vert - 1) / 2)) fprintf(fout, "bad number of edges");
		else {
			list *graph = (list*)malloc(sizeof(list) * edges);
			int *groups = (int*)malloc(sizeof(int) * vert);
			assert(graph != NULL);
			assert(groups != NULL);
			int res = 0;
			int lines = getgraph(graph, fin, vert, &res);
			int errors = 0;
			fprintf(stderr, " %d ", lines);
			if ((res == 2) && (errors == 0)) {
				errors++;
				fprintf(fout, "bad vertex");
			}
			if ((res == 1) && (errors == 0)) {
				errors++;
				fprintf(fout, "bad length");
			}
			if ((lines < edges + 2) && (errors == 0)) {
				errors++;
				fprintf(fout, "bad number of lines");
			}
			if ((edges == 0) && (errors == 0) && (vert != 1)) {
				errors++;
				fprintf(fout, "no spanning tree");
			}
			if (errors == 0) {
				prim(graph, vert, edges, groups, fout);
			}
			free(graph);
			free(groups);
		}
	}
	fclose(fin);
	fclose(fout);
	return 0;
}
