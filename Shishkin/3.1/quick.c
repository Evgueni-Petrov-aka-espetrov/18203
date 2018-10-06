#include <stdio.h>
#include <string.h>
#include <malloc.h>
void qsort (int array[], int finish, int start) {
	int firstpos, lastpos, pivot, t; 		
	firstpos = start;
	lastpos = finish;
	pivot = array[(finish+start+1)/2];		
	do {
		while (array[firstpos] < pivot) firstpos++;
		while (array[lastpos] > pivot) lastpos--;
		if (firstpos <= lastpos) {
    		t = array[firstpos];
			array[firstpos] = array[lastpos]; 						// ïðîöåäóðà ñîðòèðîâêè îòíîñèòåëüíî îïîðíîãî ýëåìåíòà
			array[lastpos] = t;
			firstpos++; 
			lastpos--;
    	}
	} while ( firstpos<=lastpos );
	if ( lastpos > start ){
		qsort(array, lastpos, start);									// ðåêóðñèâíûå âûçîâû
	}
	if ( finish > firstpos ){
		qsort(array, finish, firstpos);
	}
}

int main () {
	FILE *fin, *fout;
	fout = fopen("out.txt", "w");
	fin=fopen("in.txt", "r");
	int length, i, firstnum;
	firstnum = 0;
	if (fin == NULL) 
		fprintf(fout, "File could not be opened.");
	else {
		fscanf(fin, "%d", &length);
		int *array;
		array = (int*)malloc(length * sizeof(int));
		if (array == NULL) printf ("out of memory");
			else {
			for (i=firstnum; i<length; i++)
				fscanf(fin, "%d ", &array[i]);
			qsort(array, length-1, firstnum);
			for (i = firstnum; i<length; i++)
				fprintf(fout, "%d ", array[i]);
			}
		free(array);
	}
	fclose(fin);
	fclose(fout);
	return 0; 
}
