////#include "parser.h"
template <class T>
void parser< T>::mk_serror(int error) {
	static const char* e[] = {
		"�������������� ������","������� ������ ������","��������� �����"
		,"������� �� ����" };
	cout << e[error] << endl;

}
