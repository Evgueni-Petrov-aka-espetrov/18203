#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
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
void readpath(FILE *in, int *start, int *finish){
	int c = fgetc(in);
	while (c != ' '){
		*start = *start * 10 + c - '0';
		c = fgetc(in);
	}
	c = fgetc(in);

	while ((c != '\n') && (c != EOF)) {
		*finish = *finish * 10 + c - '0';
		c = fgetc(in);
	}
}
int getgraph(int *graph, FILE *in, int vert, int *res){
	int minus;
	int lines = 3;
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
			graph[(from - 1) * vert + to - 1] = len;
			lines++;
			counter++;
		}
		else {
			if ((len < 0) || (len > INT_MAX)) *res = 1;
			if ((from > vert) || (to > vert) || (to <= 0) || (from <= 0)) *res = 2;
		}
	}
	if (*res == 0) return lines;
	else return 0;
}
int findmin(int *path, char *crossed, int vert){
	int i = 0;
	int min = -1;
	int existance = 0;
	for (i; i < vert; i++){
		if ((path[i] >= 0) && (crossed[i] == 0)) {
			if (existance == 0) {
				min = i;
				existance = 1;
			}
			else {
				if (path[i] < path[min]) min = i;
			}
		}
	}
	return min + 1;
}
void deikstra(int *graph, int *path, char *crossed, int edges, int vert, int start, int finish, FILE *out){
	char *int_max = (char*)malloc(sizeof(char) * vert);
	assert(int_max != NULL);
	int i = 0;
	for (i; i < vert; i++){
		path[i] = -1;
		crossed[i] = 0;
		int_max[i] = 0;
	}
	path[start - 1] = 0;
	int currvert = start;

	do {
		for (i = 0; i < vert; i++){
			if ((graph[i * vert + currvert - 1] > 0) && (crossed[i] != 1)) {
				if (path[i] == -1) {
					path[i] = (path[currvert - 1] + graph[i * vert + currvert - 1]) % INT_MAX;
					int_max[i] = (path[currvert - 1] + graph[i * vert + currvert - 1]) / INT_MAX + int_max[currvert - 1];
				}
				else {
					if (path[i] > path[currvert - 1] + graph[i * vert + currvert - 1]) {
						path[i] = (path[currvert - 1] + graph[i * vert + currvert - 1]) % INT_MAX;
						int_max[i] = (path[currvert - 1] + graph[i * vert + currvert - 1]) / INT_MAX + int_max[currvert - 1];
					}
				}
			}
			if ((graph[(currvert - 1)*vert + i] > 0) && (crossed[i] != 1)) {
				if (path[i] == -1) {
					path[i] = (path[currvert - 1] + graph[(currvert - 1)*vert + i]) % INT_MAX;
					int_max[i] = (path[currvert - 1] + graph[(currvert - 1)*vert + i]) / INT_MAX + int_max[currvert - 1];
				}
				else {
					if (path[i] > path[currvert - 1] + graph[(currvert - 1)*vert + i]) {
						path[i] = (path[currvert - 1] + graph[(currvert - 1)*vert + i]) % INT_MAX;
						int_max[i] = (path[currvert - 1] + graph[(currvert - 1)*vert + i]) / INT_MAX + int_max[currvert - 1];
					}
				}
			}
		}
		crossed[currvert - 1] = 1;
		currvert = findmin(path, crossed, vert);
	} while (currvert > 0);
	for (i = 0; i < vert; i++){
		if ((path[i] == 0) && (int_max[i] > 0)) {
			path[i] = INT_MAX;
			int_max[i]--;
		}
	}
	for (i = 0; i < vert; i++) {
		if ((path[i] >= 0) && (int_max[i] == 0)) fprintf(out, "%d ", path[i]);
		if (path[i] == -1) fprintf(out, "oo ");
		if (int_max[i] >= 1) fprintf(out, "INT_MAX+ ");
	}
	fprintf(out, "\n");
	if (path[finish - 1] == -1) fprintf(out, "no path");
	else {
		int *used = (int*)malloc(sizeof(int) * vert);
		assert(used != NULL);
		for (i = 0; i < vert; i++){
			used[i] = 0;
		}
		int j = 0;
		int midvert = finish;
		int midvert1 = midvert;
		int errors = 0;
		used[j] = finish;
		while (midvert != start) {
			int counter = 0;
			for (i = 0; i < vert; i++){
				if ((path[i] + graph[i * vert + midvert - 1] - path[midvert - 1] + INT_MAX * (int_max[i] - int_max[midvert - 1]) == 0) && (graph[i * vert + midvert - 1] != 0)) {
					midvert1 = i + 1;
					if (int_max[finish - 1] > 0) counter++;
				}
				if ((path[i] + graph[(midvert - 1) * vert + i] - path[midvert - 1] + INT_MAX * (int_max[i] - int_max[midvert - 1]) == 0) && (graph[(midvert - 1) * vert + i] != 0)) {
					midvert1 = i + 1;
					if (int_max[finish - 1] > 0) counter++;
				}
			}
			midvert = midvert1;
			if (counter > 1) errors = 1;
			j++;
			used[j] = midvert;
		}
		if (errors == 1) fprintf(out, "overflow");
		else {
			i = 0;
			for (i; i <= j; i++){
				fprintf(out, "%d ", used[i]);
			}
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
		int start = 0;
		int finish = 0;
		readpath(fin, &start, &finish);
		int edges = readamount(fin);
		if ((edges < 0) || (edges > vert*(vert - 1) / 2)) fprintf(fout, "bad number of edges");
		else {
			int *graph = (int*)malloc(sizeof(int) * vert * vert);
			int *path = (int*)malloc(sizeof(int) * vert);
			char *crossed = (char*)malloc(sizeof(char) * vert);
			assert(graph != NULL);
			assert(path != NULL);
			assert(crossed != NULL);
			memset(graph, 0, sizeof(int) * vert * vert);
			int res = 0;
			int lines = getgraph(graph, fin, vert, &res);
			int errors = 0;
			if (((res == 2) && (errors == 0)) || (start <= 0) || (finish <= 0) || (start > vert) || (finish > vert)) {
				errors++;
				fprintf(fout, "bad vertex");
			}
			if ((res == 1) && (errors == 0)) {
				errors++;
				fprintf(fout, "bad length");
			}
			if ((lines < edges + 3) && (errors == 0)) {
				errors++;
				fprintf(fout, "bad number of lines");
			}
			if (errors == 0) {
				deikstra(graph, path, crossed, edges, vert, start, finish, fout);
			}
			free(graph);
			free(path);
			free(crossed);
		}
	}
	fclose(fin);
	fclose(fout);
	return 0;
}