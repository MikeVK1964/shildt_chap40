//#pragma once
#ifndef PARSER_MIKE 
#define PARSER_MIKE

#include <iostream>
#include <cstring>
#include <cstdlib> // isspace - �������� �� ������ ��������
#include <cctype> // ��� ������� isalpha


using namespace std;

enum types {DELIMITER=1,VARIABLE,NUMBER};

const int NUMVARS=26; // ���������� - ����� ���������� ��������
template <typename T> class parser {
	char* exp_ptr; // cc���� �� ���������
	char token[80]; // ������ ������� �������
	char tok_type; // ��� �������
	T vars[NUMVARS]; // ������ �������� ����������

#if 0
	void eval_exp1(T& result);
	void eval_exp2(T& result);
	void eval_exp3(T& result);
	void eval_exp4(T& result);
	void eval_exp5(T& result);
	void eval_exp6(T& result);
	void atom(T& result);
	void get_token();
	void putback();
	void serror(int error);
	T find_var(char* s);
	int isdelim(char c);

#endif



	void mk_get_token() {
		char* temp;
		tok_type = 0;
		temp = token;
		*temp = '\0';

		if (!*exp_ptr) return; // � ����� ���������
		while (isspace(*exp_ptr)) exp_ptr++; // ������� ��������, ��������� � ������ ������

		if (strchr("+-/*%^=()", *exp_ptr)) {
			tok_type = DELIMITER;
			// ������� � ���������� �������
			*temp++ = *exp_ptr++;
		}
		else if (isalpha(*exp_ptr)) {
			// ����� ���������� ����� ���� ����� �����, �� �������� ������ 1 �����
			while (!mk_isdelim(*exp_ptr)) *temp++ = *exp_ptr++;
			tok_type = VARIABLE;
		}
		else if (isdigit(*exp_ptr)) {
			while (!mk_isdelim(*exp_ptr)) *temp++ = *exp_ptr++;
			tok_type = NUMBER;
		}

		*temp = '\0';

	};

	int mk_isdelim(char c) {
		if (strchr(" +-/*%^=()", c) || c == 9 || c == '\r' || c == 0)
			return 1;
		return 0;
	};
	void mk_serror(int error);
#if 0	
	void mk_serror(int error) {
		static const char* e[] = {
			"�������������� ������","������� ������ ������","��������� �����"
			,"������� �� ����" };
		cout << e[error] << endl;

	}
#endif

	/* ��������� ������������. */
	void mk_eval_exp1(T& result) {
		int slot;
		char ttok_type;
		char temp_token[80];

		if (tok_type == VARIABLE) {
			/* ���������� ������ ������� */
			strcpy_s(temp_token, 80, token);
			ttok_type = tok_type;

			/* ��������� ������ ���������� */
			slot = toupper(*token) - 'A';

			mk_get_token();
			if (*token != '=') {
				mk_putback(); /* ������� ������� ���������� */
				/* ������������ ������ ������� - ��� �� ������������ */
				strcpy_s(token, 79, temp_token);
				tok_type = ttok_type;
			}
			else {
				mk_get_token(); /* �������� ��������� ����� ��������� */
				mk_eval_exp2(result);
				vars[slot] = result;
				return;
			}
		}

		mk_eval_exp2(result);

	}
	/* �������� ��� ��������� ���� ���������. */
	void mk_eval_exp2(T& result) {
		char op;
		T temp;

		mk_eval_exp3(result);
		while ((op = *token) == '+' || op == '-') {
			mk_get_token();
			mk_eval_exp3(temp);
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
	/* ��������� ��� ������� ���� ����������. */
	void mk_eval_exp3(T& result) {
		char op;
		T temp;
		// token = 10
		// exp_ptr=  " +2*3"

		mk_eval_exp4(result);
		while ((op = *token) == '*' || op == '/' || op == '%') {
			mk_get_token();
			mk_eval_exp4(temp);
			switch (op) {
			case '*':
				result = result * temp;
				break;
			case '/':
				if (temp == 0.0) {
					mk_serror(3); /* ������� �� ���� */
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
	// ���������� � �������
	void mk_eval_exp4(T& result) {
		T temp, ex;
		int t;

		mk_eval_exp5(result);

		if (*token == '^') {
			mk_get_token();
			mk_eval_exp4(temp);
			ex = result;
			if (temp == 0.0) {
				result = 1.0;
				return;
			}
			for (t = (int)temp - 1; t > 0; --t) result = result * ex;
		}

	}
	/* ��������� ������� ���������� + � -. */
	void mk_eval_exp5(T& result) {
		char  op;

		op = 0;
		if ((tok_type == DELIMITER) && *token == '+' || *token == '-') {
			op = *token;
			mk_get_token();
		}
		mk_eval_exp6(result);
		if (op == '-') result = -result;

	}
	/* ���������� ��������� � �������. */
	void mk_eval_exp6(T& result)
	{
		if ((*token == '(')) {
			mk_get_token();
			mk_eval_exp2(result);
			if (*token != ')')
				mk_serror(1);
			mk_get_token();
		}
		else
			mk_atom(result);
	}
	void mk_atom(T& result)
	{
		if (tok_type == NUMBER) {
			result = atof(token);
			mk_get_token();
			return;
		}
		if (tok_type == VARIABLE) {
			result = mk_find_var(token);
			mk_get_token();
			return;
		}

		mk_serror(0);  /* ����� �������������� ������ � ��������� */
	}

	void mk_putback() {
		char* t;

		t = token;
		for (; *t; t++) exp_ptr--;

	}
	/* ��������� �������� ����������. */
	T mk_find_var(char* s)
	{
		if (!isalpha(*s)) {
			mk_serror(1);
			return 0;
		}
		return vars[(unsigned int)toupper(*token) - 'A'];
	}

public:
	parser<typename T>()
	{
		exp_ptr = nullptr;
		tok_type = 0;
		token[0] = '\0';
		for (int i = 0; i < NUMVARS; i++) vars[i] = (T)0;

	};
/////	double eval_exp(char* exp);
	T mk_eval_exp(char* exp)
	{
		T result=0.0;   // ��� ���������� result
		exp_ptr = exp;

		mk_get_token();
		if (!*token) {
			mk_serror(2);   // ��������� �����
			return result;
		}
		mk_eval_exp1(result);

		return result;
	}
};
#if 1
template <class T> // ���������� ��� ������ ������
void parser< T>::mk_serror(int error) {
	static const char* e[] = {
		"�������������� ������","������� ������ ������","��������� �����"
		,"������� �� ����" };
	cout << e[error] << endl;

}
/////#else
template <>   // ���������� ��� double
void parser<double>::mk_serror(int error) {
	static const char* e[] = {
		"�������������� ������","������� ������ ������","��������� �����"
		,"������� �� ����" };
	cout << e[error] << endl;

}

#endif




//////////////////#include "parser.cpp""
#endif // 

