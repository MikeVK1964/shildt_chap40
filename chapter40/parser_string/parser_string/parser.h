//#pragma once
#ifndef PARSER_MIKE 
#define PARSER_MIKE

#include <iostream>
#include <cstring>
#include <cstdlib> // isspace - �������� �� ������ ��������
#include <cctype> // ��� ������� isalpha
#include <typeinfo> // typeid

using namespace std;
enum types { DELIMITER = 1, VARIABLE, STRING };
const int NUMVARS = 26; // ���������� - ����� ���������� ��������

class parser {
	char* exp_ptr; // cc���� �� ���������
	char token[80]; // ������ ������� �������
	char tok_type; // ��� �������
	string vars[NUMVARS]; // ������ �������� ����������

	void get_token() {
		register char* temp;
		tok_type = 0;
		temp = token;
		*temp = '\0';
		if (!*exp_ptr) return; // � ����� ���������
		while (isspace(*exp_ptr)) exp_ptr++; // ������� ��������, ��������� � ������ ������
		
		if (strchr("+=", *exp_ptr)) {
			tok_type = DELIMITER;
			*temp++ = *exp_ptr++; // ������� � ���������� �������
		}
		else if (isalpha(*exp_ptr)) {
			// ����� ���������� ����� ���� ����� �����, �� �������� ������ 1 �����
			while (!isdelim(*exp_ptr)) *temp++ = *exp_ptr++;
			tok_type = VARIABLE;
		}
		else if (*exp_ptr=='"') {
			exp_ptr++;
			// ���� ������ ������ ��� ����� ������
			while ((*exp_ptr != '"') && (*exp_ptr)) 	*temp++ = *exp_ptr++;   // ��������� token 
			if (!*exp_ptr)
			{
				serror(3); // ��� ������ "
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
			"�������������� ������","������� ������ ������","��������� �����"
			,"��� ������ \"" };
		cout << e[error] << endl;
	}
	/* ���������� �������� ������, ���� � �������� ������������. */
	// �� ���������� � ����� ����������
	int isdelim(char c)
	{
		if (strchr(" +=", c) ||  c == '\"' || c == 0)
			return 1;
		return 0;
	}
	/* ��������� ������������. */
	void eval_exp1(string& result)
	{
		int slot;
		char ttok_type;
		char temp_token[80];
		if (tok_type == VARIABLE) {
			/* ���������� ������ ������� */
			strcpy_s(temp_token, 80, token);
			ttok_type = tok_type;

			/* ��������� ������ ���������� */
			slot = toupper(*token) - 'A';
			get_token();
			if (*token != '=') {
				putback(); /* ������� ������� ���������� */
				/* ������������ ������ ������� - ��� �� ������������ */
				strcpy_s(token, 79, temp_token);
				tok_type = ttok_type;
			}
			else {
				get_token(); /* �������� ��������� ����� ��������� */
				eval_exp2(result);
				vars[slot] = result;
				return;
			}

		}
		eval_exp2(result);
	}
	/* �������� ��� ��������� ���� ���������. */
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
			get_token();  // ??? ��������� ������ ������ ���� ���� ��� ����� ������
			return;
		}
		if (tok_type == VARIABLE) {
    		result = find_var(token);
			get_token();
			return;
		}

		serror(0);
	}
	/* ��������� �������� ����������. */
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
			serror(2);   // ��������� �����
			return "\n";
		}
		eval_exp1(result);

		return result;
	}
};
////#include "parser.cpp""
#endif  

