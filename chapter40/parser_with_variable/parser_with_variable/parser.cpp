﻿#include "parser.h"

#include <iostream>
#include <cstring>
#include <cstdlib> // isspace - является ли символ пробелом
#include <cctype> // для функции isalpha
//#include <ctype.h>

using namespace std;
// конструктор синтаксического анализатора
parser::parser() {
	exp_ptr=nullptr;
	tok_type=0;
	token[0] = '\0';
	for (int i = 0; i < NUMVARS; i ++) vars[i] = 0.0;
}

double parser::eval_exp(char* exp) {
	double result;
	exp_ptr=exp;

	get_token();
	if (!*token){
		serror(2);   // выражение пусто
		return 0.0;
	}

	eval_exp1(result);

	return result;
}
/* Обработка присваивания. */
void parser::eval_exp1(double& result)
{
	int slot;
	char ttok_type;
	char temp_token[80];

	if (tok_type == VARIABLE) {
		/* сохраниеть старую лексему */
		strcpy_s(temp_token, 80,token);
		ttok_type = tok_type;

		/* вычислить индекс переменной */
		slot = toupper(*token) - 'A';

		get_token();
		if (*token != '=') {
			putback(); /* вернуть текущую переменную */
			/* восстановить старуб лексему - это не присваивание */
			strcpy_s(token, 79,temp_token);
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
void parser::eval_exp2(double& result) {
	register char op;
	double temp;

	eval_exp3(result);
	// result=10
	// token == "+"
	// exp_ptr=  "2*3"
	while ((op = *token) == '+' || op == '-') {
		get_token();
		// token = 2
    	// exp_ptr=  "*3"
		// op = '+'
		//
		eval_exp3(temp);
		switch (op) {
		case '-':
			result = result - temp;
			break;
		case '+':
			result = result + temp;
			break;
		}
	}
}
/* Умножение или деление двух множителей. */
void parser::eval_exp3(double& result)
{
	register char op;
	double temp;
	// token = 10
	// exp_ptr=  " +2*3"

	eval_exp4(result);
	while ((op = *token) == '*' || op == '/' || op == '%') {
		get_token();
		eval_exp4(temp);
		switch (op) {
		case '*':
			result = result * temp;
			break;
		case '/':
			if (temp == 0.0) {
				serror(3); /* деление на нуль */
				result = 0.0;
			}
			else result = result / temp;
			break;
		case '%':
			result = (int)result % (int)temp;
			break;
		}
	}
}
// Возведение в степень
void parser::eval_exp4(double& result)
{
	double temp, ex;
	register int t;

	eval_exp5(result);

	if (*token == '^') {
		get_token();
		eval_exp4(temp);
		ex = result;
		if (temp == 0.0) {
			result = 1.0;
			return;
		}
		for (t = temp - 1; t > 0; --t) result = result * ex;
	}
}
/* Умножение унарных операторов + и -. */
void parser::eval_exp5(double& result)
{
	register char  op;

	op = 0;
	if ((tok_type == DELIMITER) && *token == '+' || *token == '-') {
		op = *token;
		get_token();
	}
	eval_exp6(result);
	if (op == '-') result = -result;
}
/* Вычисление выражения в скобках. */
void parser::eval_exp6(double& result)
{
	if ((*token == '(')) {
		get_token();
		eval_exp2(result);
		if (*token != ')')
			serror(1);
		get_token();
	}
	else
		atom(result);
}
// возвращает число или переменную и считывает следующий символ
void parser::atom(double& result)
{
	if (tok_type == NUMBER) {
		result = atof(token);
		get_token();
		return;
	}
	if (tok_type == VARIABLE) {
		result = find_var(token);
		get_token();
		return;
	}

	serror(0);  /* иначе синтаксическая ошибка в выражении */
}

void parser::serror(int error) {
	static const char* e[] = { 
		"Синтаксическая ошибка","Нарушен баланс скобок","Выражение пусто"
		,"Деление на нуль"};
	cout<< e[error]<<endl;
}
void parser::putback()
{
	char* t;

	t = token;
	for (; *t; t++) exp_ptr--;
}
// извлекает текущую лексему
// и помещает ее в массив token
//
void parser::get_token()
{
	register char* temp;
	tok_type = 0;
	temp= token;
	*temp = '\0';

	if (!*exp_ptr) return; // в конце выражения
	while (isspace(*exp_ptr)) exp_ptr++; // пропуск пробелов, табуляции и пустой строки
		
	if (strchr("+-/*%^=()", *exp_ptr)) {
		tok_type = DELIMITER;
     // переход к следующему символу
		*temp++ = *exp_ptr++;
	}
	else if (isalpha(*exp_ptr)) {
		// имена переменных могут быть любой длины, но значащия только 1 буква
		while (!isdelim(*exp_ptr)) *temp++ = *exp_ptr++;
		tok_type = VARIABLE;
	}
	else if (isdigit(*exp_ptr)) {
		while (!isdelim(*exp_ptr)) *temp++ = *exp_ptr++;
		tok_type = NUMBER;
	}

	*temp = '\0';
}
/* Возвращает значение ИСТИНА, если с является раздилителем. */
int parser::isdelim(char c)
{
	// c= 9 горизонтальная табуляция
	if (strchr(" +-/*%^=()", c) || c == 9 || c == '\r' || c == 0)
		return 1;
	return 0;
}
/* Получение значения переменной. */
double parser::find_var(char* s)
{
	if (!isalpha(*s)) {
		serror(1);
		return 0;
	}
	return vars[(int)toupper(*token) - (int) 'A'];
}