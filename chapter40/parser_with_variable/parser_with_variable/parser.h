//#pragma once
#ifndef PARSER_MIKE 
#define PARSER_MIKE


enum types {DELIMITER=1,VARIABLE,NUMBER};

const int NUMVARS=26; // переменная - буква латинского алфавита

class parser {
	char* exp_ptr; // ccылка на выражение
	char token[80]; // хранит текущюю лексему
	char tok_type; // тип лексемы
	double vars[NUMVARS]; // хранит значения переменных

	void eval_exp1(double& result);
	void eval_exp2(double& result);
	void eval_exp3(double& result);
	void eval_exp4(double& result);
	void eval_exp5(double& result);
	void eval_exp6(double& result);
	void atom(double& result);
	void get_token();
	void putback();
	void serror(int error);
	double find_var(char* s);
	int isdelim(char c);
public:
		parser();
		double eval_exp(char* exp);
};
#endif // 

