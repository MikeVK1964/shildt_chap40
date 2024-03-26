//#pragma once
#ifndef PARSER_MIKE 
#define PARSER_MIKE

#include <iostream>
#include <cstring>
#include <cstdlib> // isspace - является ли символ пробелом
#include <cctype> // для функции isalpha
#include <typeinfo> // typeid

using namespace std;
enum types { DELIMITER = 1, VARIABLE, STRING };
const int NUMVARS = 26; // переменная - буква латинского алфавита

class parser {
	char* exp_ptr; // ccылка на выражение
	char token[80]; // хранит текущюю лексему
	char tok_type; // тип лексемы
	string vars[NUMVARS]; // хранит значения переменных

	void get_token() {
		register char* temp;
		tok_type = 0;
		temp = token;
		*temp = '\0';
		if (!*exp_ptr) return; // в конце выражения
		while (isspace(*exp_ptr)) exp_ptr++; // пропуск пробелов, табуляции и пустой строки
		
		if (strchr("+=", *exp_ptr)) {
			tok_type = DELIMITER;
			*temp++ = *exp_ptr++; // переход к следующему символу
		}
		else if (isalpha(*exp_ptr)) {
			// имена переменных могут быть любой длины, но значащия только 1 буква
			while (!isdelim(*exp_ptr)) *temp++ = *exp_ptr++;
			tok_type = VARIABLE;
		}
		else if (*exp_ptr=='"') {
			exp_ptr++;
			// ищем вторую скобку или конец строки
			while ((*exp_ptr != '"') && (*exp_ptr)) 	*temp++ = *exp_ptr++;   // заполняем token 
			if (!*exp_ptr)
			{
				serror(3); // нет второй "
				exit(2);
			}
		////	while (!isdelim(*exp_ptr)) *temp++ = *exp_ptr++;
			if (*exp_ptr == '"') exp_ptr++;


			tok_type = STRING;
		}
		*temp = '\0';

	}
	void serror(int error) {
		static const char* e[] = {
			"Синтаксическая ошибка","Нарушен баланс скобок","Выражение пусто"
			,"Нет второй \"" };
		cout << e[error] << endl;
	}
	/* Возвращает значение ИСТИНА, если с является раздилителем. */
	// не допустимом в имени переменной
	int isdelim(char c)
	{
		if (strchr(" +=", c) ||  c == '\"' || c == 0)
			return 1;
		return 0;
	}
	/* Обработка присваивания. */
	void eval_exp1(string& result)
	{
		int slot;
		char ttok_type;
		char temp_token[80];
		if (tok_type == VARIABLE) {
			/* сохраниеть старую лексему */
			strcpy_s(temp_token, 80, token);
			ttok_type = tok_type;

			/* вычислить индекс переменной */
			slot = toupper(*token) - 'A';
			get_token();
			if (*token != '=') {
				putback(); /* вернуть текущую переменную */
				/* восстановить старуб лексему - это не присваивание */
				strcpy_s(token, 79, temp_token);
				tok_type = ttok_type;
			}
			else {
				get_token(); /* получить следующую часть выражения */
				eval_exp2(result);
				vars[slot] = result;
				return;
			}

		}
		eval_exp2(result);
	}
	/* Сложение или вычитание двух слагаемых. */
	void eval_exp2(string& result) {
		register char op;
		string temp;
		
		atom(result);

	////////////	eval_exp3(result);
		while ((op = *token) == '+' ) {
			get_token();
			atom(temp);

			switch (op) {
			case '+':
				result = result + temp;
				break;
			}
		}
	/////	atom(result);
	}
	void putback()
	{
		char* t;

		t = token;
		for (; *t; t++) exp_ptr--;
	}
	void atom(string& result) {
		if (tok_type == STRING) {
			result = token;
			get_token();  // ??? сдедующий символ должен быть плюс или конец строки
			return;
		}
		if (tok_type == VARIABLE) {
    		result = find_var(token);
			get_token();
			return;
		}

		serror(0);
	}
	/* Получение значения переменной. */
	string find_var(char* s) {
		if (!isalpha(*s)) {
			serror(1);
			exit(0);
		}
		return vars[(unsigned int)toupper(*token) - (unsigned int)'A'];

	}

public:
	parser() 
	{
		exp_ptr = nullptr;
		tok_type = 0;
		token[0] = '\0';
	};
	string eval_exp(char* exp) {
		string result="";
		exp_ptr = exp;
		get_token();
		if (!*token) {
			serror(2);   // выражение пусто
			return "\n";
		}
		eval_exp1(result);

		return result;
	}
};
////#include "parser.cpp""
#endif  

