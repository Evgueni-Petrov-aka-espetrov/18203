#include <stdio.h>
#include <string.h>

void make_stops(int stops[], int stops_len, char templ[], int l);
void make_suffixes(int suffixes[], char templ[], int l);
int read_n_shift(FILE *in, char line[], int l, int n);
void search(FILE *in, FILE *out, char templ[], int len_templ, int *stops, int *suffixes);

int main() {

	FILE *in = fopen("in.txt", "r");
	FILE *out = fopen("out.txt", "w");

	char templ[255];
	fgets(templ, 255, in);
	templ[strlen(templ) - 1] = 0;

	int stops[256];
	int suffixes[255];
	make_stops(stops, 256, templ, strlen(templ));
	make_suffixes(suffixes, templ, strlen(templ));

	search(in, out, templ, strlen(templ), stops, suffixes);

	fclose(in);
	fclose(out);
}
void make_stops(int stops[], int stops_len, char templ[], int l) {
	int i;
	for (i = 0; i < 256; stops[i++] = 0);
	for (i = 0; i < l - 1; stops[(unsigned char)templ[i]] = i + 1, i++);
}

void make_suffixes(int suffixes[], char templ[], int l) {
	for (int s = 0; s <= l; s++) {
		int shift = 0;
		while (shift < l) {
			shift++;
			if (s + shift >= l) {
				if (strncmp(templ, &(templ[shift]), l - shift) == 0)
					break;
			}
			else {
				if (templ[l - shift - s - 1] != templ[l - s - 1] && strncmp(&(templ[l - shift - s]), &(templ[l - s]), s) == 0)
					break;
			}
		}
		suffixes[s] = shift;
	}
}

int read_n_shift(FILE *in, char line[], int l, int n) {

	for (int i = n; i < l; i++) {
		line[i - n] = line[i];
	}

	for (int i = l - n; i < l; i++) {
		int a = fgetc(in);
		if (a == EOF) {
			return 0;
		}
		line[i] = a;
	}
	return 1;
}

void search(FILE *in, FILE *out, char templ[], int len_templ, int *stops, int *suffixes) {

	char line[17];
	int shift = len_templ, n_line = 1;


	while (read_n_shift(in, line, len_templ, shift)) {

		n_line = n_line + shift;
		shift = 0;
		for (int i = len_templ - 1; i >= 0; i--) {
			fprintf(out, "%d ", n_line - len_templ + i);
			if (line[i] != templ[i]) {
				if ((i - stops[(unsigned char)line[i]] + 1) > suffixes[len_templ - i - 1])
					shift = i - stops[(unsigned char)line[i]] + 1;
				else
					shift = suffixes[len_templ - i - 1];
				break;
			}
		}
		if (shift == 0) {
			shift = suffixes[len_templ];
		}
	}
}
