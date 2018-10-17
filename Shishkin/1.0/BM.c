#include <stdio.h>
#include <string.h>
#include <malloc.h>
int booyer (char haystack[], char needle[], FILE *fout, int lengthneedle, int lengthhay) {
	int i, j, l, var, ascichar1, count;
	int table[256];
	//printf("%s ", needle);
	//lengthhay = strlen(haystack);
	//lengthneedle = strlen(needle);
//	printf("%d ", lengthhay);
	if ((lengthhay >= lengthneedle) && (lengthneedle > 0) && (lengthhay > 2)) {
		for (i=0; i<256; i++) 
			table[i] = lengthneedle;
			
		for(i=lengthneedle-2; i>=0; i--){
			ascichar1 = needle[i];
			if (table[ascichar1] == lengthneedle) table[ascichar1] = lengthneedle-1-i;
			//printf("%d ", needle[i]);
			//printf("%d ", table[needle[i]]);
		}
		//table[needle[lengthneedle-1]] = lengthneedle;
		//printf("%d ", table[needle[8]]);
		j = lengthneedle - 1;
		i = j;
		l = j;
		while (j <= lengthhay-1) {
			//printf("111111");
			count = 0;
			do {
				fprintf(fout, "%d ", j+1);
				//printf("%d ", j+1);
				i--;
				j--;
				count++;
				
			} while ((i >= 0) && (haystack[j+1] == needle[i+1]));
			
			if (i < 0) var =1;
			if (haystack[j+1] != needle[i+1]) var = 2;
			
			if (var==1) {
				i = lengthneedle - 1;
				j = l + lengthneedle;
				l = j;
				//printf(" 11 ");
			}
			if (var==2) {
				
				i = lengthneedle - 1;
				if (count == 1) j = l + table[haystack[j+1]];
				if (count > 1) j = l + table[needle[i]];
				l = j;
			} 
			//printf("%d ", j);
		}
	}
	//else fprintf(fout, "bad input");
}
int main () {
	FILE *fin, *fout;
	char *haystack;
	char needle[16];
	char f, symbol;
	int c, d, i, lengthhay, lengthneedle;
	fout = fopen("out.txt", "w");
	fin=fopen("in.txt", "r");

	if (fin == NULL) 
		fprintf(fout, "File could not be opened.");
	else {
		haystack = (char*)malloc(sizeof(char));
		i = 0;
		while ((symbol=getc(fin)) != '\n'){
			needle[i] = symbol;
 			i++;
				
		}
		lengthneedle = i;
		//printf("%s  ", needle);
		//printf("  %d   ", strlen(needle));
		c = 0;
		while ((f=getc(fin)) != EOF) {
			haystack[c] = f;
			c++;
			realloc(haystack, (c+1)*sizeof(char));	
		}
		lengthhay = c;
		
		//printf("%s  ", haystack);
		//printf("  %d   ", strlen(haystack));
		
	
		booyer(haystack, needle, fout, lengthneedle, lengthhay);

		//fgets(f, 1, fout);
		
	}
	fclose(fin);
	fclose(fout);
	return 0; 
}
