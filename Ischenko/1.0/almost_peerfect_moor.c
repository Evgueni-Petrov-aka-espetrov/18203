#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<malloc.h>

typedef enum { true, false } bool;


bool change_stroka(int lenth_of_shifting, int *position, char *stroka, int lenth){
	char simbol = ' ';
	while (lenth_of_shifting != 0) {
		*position = *position + 1;
		int EOF_checker = 0;
		EOF_checker = scanf("%c", &simbol);
		if (EOF_checker == EOF){
			return true;
		}

		stroka[(*position) % lenth] = simbol;
		lenth_of_shifting--;
	}
	return false;
}
int find_lenth_of_shifting(const int *stoptable,const char *stroka,int position,int lenth_of_template, bool is_stroka_correct) {
	int lenth_of_shifting = 0;
	if (is_stroka_correct == true) {
		lenth_of_shifting = lenth_of_template - stoptable[(int)(stroka[position%lenth_of_template]) + 34] - 1;
	}
	else {
		int position_of_interaption = stoptable[(int)stroka[position%lenth_of_template] + 34];
		if (position_of_interaption == -1) {
			lenth_of_shifting = lenth_of_template;
		}
		else {
			lenth_of_shifting = lenth_of_template - position_of_interaption - 1;
		}
	}
	return lenth_of_shifting;
}
int check(const char *stroka,const int *stoptable,const char *template_, int lenth_of_template, int position, int adding_if_lenth_1){
	int position_at_template = lenth_of_template - 1;
	int lenth_of_shifting = 0;
	bool is_stroka_correct = true;
	for (int pos = position; pos > position - lenth_of_template; pos--) {
		printf( "%d ", pos + 1);
		if (stroka[pos%lenth_of_template] != template_[position_at_template]) {
			is_stroka_correct = false;
			break;
		}
		position_at_template--;
	}
	lenth_of_shifting=find_lenth_of_shifting(stoptable, stroka, position, lenth_of_template, is_stroka_correct);
	if (is_stroka_correct == true && adding_if_lenth_1) {
		lenth_of_shifting++;
	}

	return lenth_of_shifting;
}

int make_template(char *template_) {
	char simbol;
	int lenth_of_template = 0;
	scanf( "%c", &simbol);
	while (simbol != '\n')
	{

		template_[lenth_of_template] = simbol;
		lenth_of_template++;
		scanf( "%c", &simbol);
	}
	return lenth_of_template;
}

void make_stoptable(int *stoptable, char *template_, int lenth_of_template) {
	for (int i = 0; i < 300; i++) {
		stoptable[i] = -1;
	}
	for (int i = 0; i < lenth_of_template - 1; i++) {
		stoptable[(int)template_[i] + 34] = i;
	}
}

int main() {
	char shab[16], simbol = ' ';
	int stoptable[300], adding_if_lenth_1 = 0, number = 0;
	bool is_end_of_input = false;
	int lenth_of_template = 0;
	lenth_of_template=make_template(shab);
	make_stoptable(stoptable, shab, lenth_of_template);
	if (lenth_of_template == 1) {
		adding_if_lenth_1 = 1;
	}
	int lenth_of_shifting = lenth_of_template;
	char *stroka = (char*)malloc(lenth_of_template * sizeof(char));
	int b =scanf( "%c", &simbol);
	stroka[number] = simbol;
	lenth_of_shifting--;
	while (!(b == 0 || b == EOF)) {

		is_end_of_input = change_stroka(lenth_of_shifting, &number, stroka, lenth_of_template);

		if (is_end_of_input == false){
			lenth_of_shifting = check(stroka, stoptable, shab, lenth_of_template, number, adding_if_lenth_1);
		}
		else{
			return 0;
		}
	}

}