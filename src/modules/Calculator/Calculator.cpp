#include <math.h>

#include <modules.h>

using namespace std;

Calculator::Calculator() {
	signs = "+-/*^";

	functions[	"sin"	] = &sin;
	functions[	"cos"	] = &cos;
	functions[	"tg"	] = &tan;
	functions[	"asin"	] = &asin;
	functions[	"acos"	] = &acos;
	functions[	"atg"	] = &atan;
	functions[	"abs"	] = &abs;
	functions[	"ctg"	] = [](double x) -> double {	return 1.0/tan(x);	};
	functions[	"actg"	] = [](double x) -> double {	return atan(1.0/x);	};
	functions[	"sign"	] = [](double x) -> double {	return x/abs(x);	};
	functions[	"sqrt"	] = [](double x) -> double {	return sqrt(x);		};

	operationFunctions['*'] = [](double x, double y) -> double {	return x * y;		};
	operationFunctions['/'] = [](double x, double y) -> double {	return x / y;		};
	operationFunctions['+'] = [](double x, double y) -> double {	return x + y;		};
	operationFunctions['-'] = [](double x, double y) -> double {	return x - y;		};
	operationFunctions['^'] = [](double x, double y) -> double {	return pow(x, y);	};

	
	for (int i = 0; i < 256; ++i) {
		allowedChars[i] = false;
	}

	std::string allowedString = ".0123456789+-*/^()QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm";

	for (char c: allowedString) {
		allowedChars[c] = true;
	}
}

//функция определяет приоритет операции
int Calculator::prior(char c) {

	if (c == '+' || c == '-') {
		return 1;
	}
	if (c == '*' || c == '/') {
		return 2;
	}
	if (c == '^') {
		return 3;
	}
	return 0;
}

//функция проверяет является ли выражение функцией от другого выражения
bool Calculator::isFunction(const std::string& str) {

	int balance = 0;
	bool check = false;
	for (int i = 0; i < int(str.size()); ++i) {
		if (str[i] == '(') {
			check = true;
			++balance;
		}
		if (str[i] == ')') {
			--balance;
			if (balance == 0 && i+1 < str.size()) {
				return false;
			}
		}
	}

	return check;
}

//Функция переводит строку str в вещественное число
double Calculator::parseToDouble(const std::string& str) {

	double res = 0; 			//	к результату будем добавлять соответствующий разряд
	int point = str.size(); 	//	индекс разделителя

	for (int i = 0; i < int(str.size()); ++i) {
		if (str[i] == '.') {
			//	сначала находим где разделитель, чтобы знать сколько разрядов всего
			point = i;
		}
	}

	/*
		10 в степени максимального разряда
		(по сути это кол-во символов до разделителя)
	*/
	double k = pow(10.0, point - 1);


	for (int i = 0; i < int(str.size()); ++i) {
		if (isdigit(str[i])) {
			res += k * int(char(str[i]) - '0');		//	прибавляем соответствующий разряд
			k /= 10;								// 	уменьшаем разряд
		}
		else if (str[i] != '.') {					//	если встретили сторонний символ значит выражение не корректно
			return res;
		}
	}

	//	если всё ок возвращаем значение
	return res;
}

//	Функция очищает строку str от ненужных символов
std::string Calculator::cleanExp(std::string str) {
	std::string res;

	for (char c: str) {
		if (allowedChars[c]) {
			res += tolower(c);
		}
	}

	return res;
}

//	функция вычисляет выражение str, используя
//	операции signs и подставляя вместо x значение &x
double Calculator::calc(std::string str) {

	str = cleanExp(str);

	//	если получили пустое выражение возвращаем 0
	if (str == "") {
		return 0;
	}

	/*
		p-дополнительный приоритет (для скобок)
		mn-миниимальный приоритет операции
		mxind- индекс операции с минимальным приоритетом
		cur- итератор для цикла
	*/
	int p = 0, mn = 3, mnind = -1, cur = 0;

	for (; cur < int(str.size()); ++cur) {

		//	если заходим в скобку повышаем приоритет
		if (str[cur] == '(') {
			p += 100;
		}

		//	соответственно отнимаем приоритет
		if (str[cur] == ')') {
			p -= 100;
		}

		/*
			ищем операцию, которую будем выполняь в последнюю очередь т.е. 
			операцию с минимальным приоритетом и самую правую из минимальных
		*/
		if (signs.find(str[cur]) != -1 && prior(str[cur]) + p <= mn) {
			mn = prior(str[cur]) + p;
			mnind = cur;
		}
	}

	//	строки содержащие левое и правое подвыражения относительно операции
	std::string left = "";
	std::string right = "";

	//	заполняем их соответственно
	for (int i = 0; i < mnind; ++i) {
		left += str[i];
	}

	for (int i = mnind + 1; i < int(str.size()); ++i) {
		right += str[i];
	}

	//	случаи, когда наше выражение представляет из себя функции (sin, cos, tg, ...)
	if (isFunction(str)) {
		size_t bracketPos = str.find("(");
		std::string unFunc = str.substr(bracketPos+1,  -2-bracketPos+str.size());
		double unFuncCalc = calc(unFunc);

		std::string functionName = str.substr(0, bracketPos); 
		if (functions.count(functionName)) {
			return functions[functionName](unFuncCalc);
		} else 
		if (functionName.empty()) {
			return unFuncCalc;
		}
	}

	//	если у нас нет операций в выражении и это не функция,
	//	то возвращаем вещественное число от этого выражения
	if (mnind == -1) {
		return parseToDouble(str);
	}
	else {
		//	если нужно совершить операцию
		//	считаем левое и правое подвыражения
		double calcL = calc(left);
		double calcR = calc(right);

		//	возвращаем соответствующее значение
		double res = operationFunctions[str[mnind]](calcL, calcR);
		// printf("\t[%s]\t%c\t[%s]\t=\t%.2lf\n", left.c_str(), str[mnind], right.c_str(), res);
		// printf("\t%s = %.2lf\n", str.c_str(), res);
		return res;
	}
	return NAN;
}
