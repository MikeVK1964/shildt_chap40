////#include "parser.h"
template <class T>
void parser< T>::mk_serror(int error) {
	static const char* e[] = {
		"Синтаксическая ошибка","Нарушен баланс скобок","Выражение пусто"
		,"Деление на нуль" };
	cout << e[error] << endl;

}
