#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#define MAX 20


int read_l(FILE *in, char line[], int len_templ);
int hash(const char line[], int st_index, int len_templ);
void search_and_result(FILE *in, FILE *out, const char templ[], int len_templ);

int main() {

	FILE *in = fopen("in.txt", "r");
	FILE *out = fopen("out.txt", "w");

	char templ[MAX];
	fgets(templ, MAX, in);
	templ[strlen(templ) - 1] = '\0';

	unsigned int pow;
	size_t len_templ = strlen(templ);
	unsigned int h_templ = hash(templ, 0, len_templ);
	fprintf(out, "%d ", h_templ);

	search_and_result(in, out, templ, len_templ);

	fclose(in);
	fclose(out);
}

int read_l(FILE *in, char line[], int len_templ) {
	while (len_templ--) {
		int a = fgetc(in);
		if (a == EOF) {
			return 0;
		}
		*line = a;
		*line++;
	}
	return 1;
}
int hash(const char line[], int st_index, int len_templ) {
	int pow = 1;

	unsigned int result = 0;
	for (int i = 0; i < len_templ; i++) {
		result += ((unsigned char)line[(i + st_index) % len_templ] % 3)*pow;
		pow *= 3;
	}
	return result;
}
void search_and_result(FILE *in, FILE *out, const char templ[], int len_templ) {
	char line[MAX];
	unsigned int pow;
	unsigned int h_templ = hash(templ, 0, len_templ);

	int temp, ins_pos = 0, n_line = 0;
	if (!read_l(in, line, len_templ))
		return;

	unsigned int h_line = hash(line, 0, len_templ);
	if (h_line == h_templ)
	{
		for (int i = 0; i < len_templ; i++)
		{
			fprintf(out, "%d ", i + 1);
			if (line[i] != templ[i])
				break;
		}
	}
	while ((temp = fgetc(in)) != EOF) {

		line[ins_pos++] = (char)temp;
		n_line++;
		h_line = h_line / 3;
		pow = 1;
		for (int i = 1; i < len_templ; i++)
			pow *= 3;
		h_line = h_line + ((unsigned char)line[ins_pos - 1] % 3)*pow;
		if (ins_pos == len_templ) ins_pos = 0;

		if (h_line == h_templ) {
			for (int i = 0; i < len_templ; i++)
			{
				fprintf(out, "%d ", i + n_line + 1);
				if (line[(ins_pos + i) % len_templ] != templ[i]) break;
			}
		}
	}

}

