#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>

typedef enum{false,true} bool;

int lenth_of_template = 0;

int make_shab_hash(const int *hash_3, char *template_, FILE fin) {
	unsigned char simbol;
	int i = 0;
	int hash = 0;
	fscanf(&fin,"%c", &simbol);
	while (simbol != '\n')
	{
		template_[i] = simbol;
		hash += ((int)simbol % 3) * hash_3[i];
		i++;
		fscanf(&fin,"%c", &simbol);
	}
	lenth_of_template = i;
	return hash;
}

bool change_hash( int *current_hash, char *str, int *position,const int *hash_3, FILE *fin) {
	unsigned char simbol;
	bool is_lenth_of_input_enough = true;
	int EOF_catcher = 0;
	EOF_catcher= fscanf(fin,"%c", &simbol);
	if (EOF_catcher!=EOF) {
		*current_hash = (*current_hash - (int)str[*position  % lenth_of_template] % 3) / 3 + (int)simbol % 3 * hash_3[lenth_of_template - 1];
		str[*position % lenth_of_template] = simbol;
		*position = *position + 1;
	}
	else {
		is_lenth_of_input_enough = false;
	}

	return is_lenth_of_input_enough;
}

bool make_first_hash(char *str,const int *hash_3,int *current_hash,FILE *fin){
	unsigned char simbol;
	int EOF_catcher = 0;
	bool is_lenth_of_input_enough = true;
	for (int i = 0; i < lenth_of_template; i++) {
			EOF_catcher= fscanf(fin,"%c", &simbol);
			if (EOF_catcher != EOF){
				str[i] = simbol;
				*current_hash = *current_hash+((int)simbol % 3) * hash_3[i];
			}
			else{
				is_lenth_of_input_enough = false;
				break;
			}
		}
	return is_lenth_of_input_enough;
}

void compare_template_and_string(const char *str,const char *template_,int position, FILE *fout){
	
	for (int i = 0; i <= lenth_of_template - 1; i++){
		fprintf(fout,"%d ", position - lenth_of_template + i+1);
		if (str[(position-lenth_of_template + i) % lenth_of_template] != template_ [i]){
			break;
		}
	}
}

int main() {
	FILE *fin,*fout;
	fin = fopen("in.txt","r");
	fout = fopen("out.txt", "w");
	const int hash_3[16]= { 1,3,9,27,81,243,729,2187,6561,19683,59049,177147,531441,1594323,4782969,14348907 };
	char template_[16];
	int  shab_hash=0, current_hash = 0;
	shab_hash= make_shab_hash(hash_3, template_,*fin);
	fprintf(fout,"%d ", shab_hash);
	char simbol =' ';
	char *str = (char*)malloc(lenth_of_template * sizeof(char));
	int position = lenth_of_template;
	bool is_lenth_of_input_enough = true;
	is_lenth_of_input_enough = make_first_hash(str, hash_3, &current_hash, &fin);
	
	while (is_lenth_of_input_enough==true) {
		if (current_hash == shab_hash) {
			compare_template_and_string(str, template_, position, &fout);
		}
		is_lenth_of_input_enough = change_hash(&current_hash, str, &position, hash_3, &fin);
	}
	fclose(fin);
	fclose(fout);
	return 0;
}