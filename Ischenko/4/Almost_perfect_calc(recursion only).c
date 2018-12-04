#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

typedef enum{false,true} bool;
typedef enum{syntax_error,division_by_zero, no_error}errors_catcher;

void error_processer(errors_catcher catch_error) {
	if (catch_error == syntax_error) {
		printf("syntax error");
		exit(0);
	}
	if (catch_error == division_by_zero) {
		printf("division by zero");
		exit(0);
	}

}

errors_catcher check_the_following_element(const char *string_for_calculate,int lenth, int position_of_simbol) {
	char simbol = ' ';
	if (position_of_simbol <= lenth - 1) {
		simbol = string_for_calculate[position_of_simbol];
		if ((position_of_simbol == lenth - 1 || position_of_simbol == 0) && (simbol == '*' || simbol == '-' || simbol == '+' || simbol == '/')) {
			return syntax_error;
		}
	}
	if (position_of_simbol<lenth-1) {
		char next_simbol = string_for_calculate[position_of_simbol + 1];
		switch (simbol) {
		case '(':
		case '*':
		case '/':
		case '-':
		case '+':
			switch (next_simbol) {
			case '*':
			case '-':
			case '+':
			case '/':
			case ')':
				return syntax_error;
			}
			break;
		case ')':
			switch (next_simbol) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '(':
				return syntax_error;
			}
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			if (next_simbol == '(') {
				return syntax_error;
			}
			break;

		}
	}
	return no_error;
}

int brackets_balance(int old_position_of_last_bracket,char *brackets,char bracket) {
	int new_position_of_last_bracket = old_position_of_last_bracket;
	switch (bracket) {
	case '(': 
		brackets[old_position_of_last_bracket + 1] = '('; 
		new_position_of_last_bracket++; 
		break;
	case ')': 
		if (brackets[old_position_of_last_bracket] == '(') {
			brackets[old_position_of_last_bracket] = ' ';
			new_position_of_last_bracket--;
		}
		else {
			brackets[old_position_of_last_bracket+1] = ')';
			new_position_of_last_bracket++;
		}
		break;
	}
	return new_position_of_last_bracket;
}

errors_catcher syntax_error_checker(char simbol) {
	errors_catcher is_syntax_error= syntax_error;
	switch (simbol){
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case '(':
	case ')':
	case '/':
	case '*':
	case '-':
	case '+': is_syntax_error = no_error;
	}
	return is_syntax_error;
}

int find_closer_bracket(const char *string_for_calculate, int position_of_opening_bracket, int end) {
	int position_of_closing_bracket = position_of_opening_bracket+1, amount_of_brakets=1;
	for (int i = position_of_opening_bracket; i <= end; i++) {
		if (string_for_calculate[i] == '('){
			amount_of_brakets++;
		}
		if (string_for_calculate[i] == ')'){
			amount_of_brakets--;
		}
		if (amount_of_brakets == 0){
			position_of_closing_bracket = i;
			break;
		}
	}
		return position_of_closing_bracket;
}

int create_number(const char *string_for_calculate, int *start_of_number, int end){
	double number = 0;
	int lenth_of_number = 0, i = 0;
	char string_number[11];
	for ( i = *start_of_number; i <= end; i++){
		if (isdigit(string_for_calculate[i])){
			string_number[lenth_of_number] = string_for_calculate[i];
			lenth_of_number++;
			//number += ((int)string_for_calculate[i] - 48) / (pow(10,lenth_of_number));
		}
		else{
			break;
		}
	}
	string_number[lenth_of_number] = '\0';
	number = atoi(string_number);
	//number = number* pow(10, lenth_of_number);
	*start_of_number = i;
	return (int)number;
}

int find_position_of_weak_operation(const char *string_for_calculate, int *position_of_operation,int end){
	
	int i=0,ammount_of_brackets=0;
	for (i = *position_of_operation+1; i <= end; i++){
		if (string_for_calculate[i] == '(') {
			ammount_of_brackets++;
		}
		if (string_for_calculate[i] == ')') {
			ammount_of_brackets--;
		}
		if ((string_for_calculate[i] == '+' || string_for_calculate[i] == '-') && ammount_of_brackets==0){
			break;
		}
	}
	return i;
}

int find_position_of_next_operation(const char *string_for_calculate, int *position_of_operation, int end) {
	int i = 0, ammount_of_brackets = 0;
	for (i = *position_of_operation + 1; i <= end; i++) {
		if (string_for_calculate[i] == '(') {
			ammount_of_brackets++;
		}
		if (string_for_calculate[i] == ')') {
			ammount_of_brackets--;
		}
		if ((string_for_calculate[i] == '+' || string_for_calculate[i] == '-' || string_for_calculate[i] == '*' || string_for_calculate[i] == '/') && ammount_of_brackets == 0) {
			break;
		}
	}
	return i;
}


int process_operations(const char *string_for_calculate, char operation, int sub_result, int *position_of_operation,int end, errors_catcher *catch_error, int lenth){
	int result=0, position_of_weak_operation=0;
	switch (operation){
	case '+':
		*position_of_operation = *position_of_operation + 1;
		result = sub_result + calculate(string_for_calculate, position_of_operation, end, catch_error, lenth );
		break;
	case '*':
		position_of_weak_operation = find_position_of_next_operation(string_for_calculate, position_of_operation, end);
		*position_of_operation = *position_of_operation + 1;
		result = sub_result * calculate(string_for_calculate, position_of_operation, position_of_weak_operation-1, catch_error, lenth);
		break;
	case '-':
		position_of_weak_operation = find_position_of_weak_operation(string_for_calculate, position_of_operation, end);
		*position_of_operation = *position_of_operation + 1;
		result = sub_result - calculate(string_for_calculate, position_of_operation, position_of_weak_operation - 1, catch_error, lenth);
		break;
	case '/':
		position_of_weak_operation = find_position_of_next_operation(string_for_calculate, position_of_operation, end);
		*position_of_operation = *position_of_operation + 1;
		int new_sub_resalt =calculate(string_for_calculate, position_of_operation, position_of_weak_operation - 1, catch_error, lenth);
		if (new_sub_resalt != 0) {
			result = sub_result / new_sub_resalt;
		}
		else {
			*catch_error = division_by_zero;
		}
		break;
	}
	return result;
}

int calculate (const char *string_for_calculate, int *position_on_string, int end, errors_catcher *catch_error,int lenth){
	int result = 0, sub_result = 0;
	*catch_error = check_the_following_element(string_for_calculate, lenth, *position_on_string);
		while (*position_on_string <= end && *catch_error == no_error) {
			if (string_for_calculate[*position_on_string] == '(') {
				*position_on_string = *position_on_string + 1;
				int position_of_closing_bracket = find_closer_bracket(string_for_calculate, *position_on_string, end);
				sub_result = calculate(string_for_calculate, position_on_string, position_of_closing_bracket - 1, catch_error, lenth);
				*position_on_string = position_of_closing_bracket + 1;
			}
			else {
				if (isdigit(string_for_calculate[*position_on_string])) {
					sub_result = create_number(string_for_calculate, position_on_string, end);
				}
				else {
					sub_result = process_operations(string_for_calculate, string_for_calculate[*position_on_string], sub_result, position_on_string, end, catch_error, lenth);
				}
			}
			if (*catch_error == no_error) {
				*catch_error = check_the_following_element(string_for_calculate, lenth, *position_on_string);
			}
		}
		result = sub_result;
	return result;
}

int main() {
	char string_for_calculate[1000], brackets[1001];
	int balance_of_brackets = 0, EOF_checker=0, lenth_of_string = 0, position_of_last_bracket=0;
	char simbol = ' ';
	bool is_input_bad = false;
	errors_catcher catch_error = no_error;
	EOF_checker = scanf("%c", &simbol);
	while (EOF_checker != EOF && simbol!='\n') {
		string_for_calculate[lenth_of_string] = simbol;
		if (syntax_error_checker(simbol) == syntax_error) {
			catch_error = syntax_error;
		}
		position_of_last_bracket = brackets_balance(position_of_last_bracket, brackets, simbol);
		lenth_of_string++;
		EOF_checker = scanf("%c", &simbol);
	}
	if (lenth_of_string == 0) {
		error_processer(syntax_error);
	}
	if (position_of_last_bracket != 0) {
		catch_error = syntax_error;
	}
	int result = 0,start=0;
	if (catch_error == no_error) {
		
		result = calculate(string_for_calculate,&start, lenth_of_string - 1, &catch_error, lenth_of_string);
	}
	error_processer(catch_error);
	printf("%d", result);
	return 0;
}