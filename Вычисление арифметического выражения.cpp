#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <locale>
#include <math.h>
#include <cstdio>

using namespace std;

int prior(char c) {
	//Функция определяет приоритет операции

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

bool isFunction(string& str) {
	//функция проверяет является ли выражение функцией от другого выражения

	int balance = 0;

	for (int i = 0; i < int(str.size()); ++i) {
		if (str[i] == '(') {
			++balance;
		}
		if (str[i] == ')') {
			--balance;
			if (balance == 0 && i < int(str.size()) - 1) {
				return false;
			}
		}
	}
	return true;
}

double parseToDouble(string str) {
	//Функция переводит строку str в вещественное число

	double res = 0; /*К результату будем добавляет соответствующий разряд*/
	int point = str.size(); //индекс разделителя

	for (int i = 0; i < int(str.size()); ++i) {
		if (str[i] == '.') {
			/*сначала находим где разделитель, чтобы знать сколько разрядов всего*/
			point = i;
		}
	}

	double k = pow(10.0, point - 1); /*10 в степени максимального разряда
									 (по сути это кол-во символов до разделителя)*/


	for (int i = 0; i < int(str.size()); ++i) {
		if (isdigit(str[i])) {
			res += k * int(char(str[i]) - '0');/*прибавляем соответствующий разряд*/
			k /= 10;/*уменьшаем разряд*/
		}
		else if (str[i] != '.') {/*если встретили сторонний символ значит
								 выражение не корректно*/
			cout << "Не удалось распознать выражение: " << str << '\n';
			return res;
		}
	}

	/*если всё ок возвращаем значение*/
	return res;
}

double calc(string str, string& signs, double& x) {
	/*функция вычисляет выражение str, используя
	операции signs и подставляя вместо x значение &x*/

	/*убираем все лишние пробелы*/
	for (int i = 0; i < int(str.size()); ++i) {
		if (str[i] == ' ') {
			str.erase(str.begin() + i);
			--i;
		}
	}

	/*убираем все крайние скобки*/
	while (str[0] == '(' && str[int(str.size()) - 1] == ')') {
		str.erase(str.begin());
		str.erase(str.end() - 1);
	}

	/*если получили пустое выражение возвращаем 0*/
	if (str == "") {
		return 0;
	}

	/*вместо x возвращаем соответствующее значение*/
	if (str == "x") {
		return x;
	}

	/*p-дополнительный приоритет (для скобок)
	mn-миниимальный приоритет операции
	mxind- индекс операции с минимальным приоритетом
	cur- итератор для цикла*/
	int p = 0, mn = 3, mnind = -1, cur = 0;

	for (; cur < int(str.size()); ++cur) {

		/*если заходим в скобку повышаем приоритет*/
		if (str[cur] == '(') {
			p += 100;
		}

		/*соответственно отнимаем приоритет*/
		if (str[cur] == ')') {
			p -= 100;
		}

		/*ищем операцию, которую будем выполняь в последнюю очередь
		т.е. операцию с минимальным приоритетом и самую правую из минимальных*/
		if (signs.find(str[cur]) != -1 && prior(str[cur]) + p <= mn) {
			mn = prior(str[cur]) + p;
			mnind = cur;
		}
	}

	/*строки содержащие левую и правую части операции*/
	string left = "";
	string right = "";

	/*заполняем их соответственно*/
	for (int i = 0; i < mnind; ++i) {
		left += str[i];
	}
	
	for (int i = mnind + 1; i < int(str.size()); ++i) {
		right += str[i];
	}

	/*случаи, когда наше выражение представляет из себя функции (sin, cos, tg, ...)*/
	if (isFunction(str)) {
		string unFunc = str.substr(str.find("(") + 1, int(str.size()) - int(str.find("(")) - 2);
		double unFuncCalc = calc(unFunc, signs, x);

		if (str.find("sin(") == 0) {
			return sin(unFuncCalc);
		}
		if (str.find("arcsin(") == 0) {
			return asin(unFuncCalc);
		}
		if (str.find("cos(") == 0) {
			return cos(unFuncCalc);
		}
		if (str.find("arccos(") == 0) {
			return acos(unFuncCalc);
		}
		if (str.find("tg(") == 0) {
			return tan(unFuncCalc);
		}
		if (str.find("arctg(") == 0) {
			return atan(unFuncCalc);
		}
		if (str.find("ctg(") == 0) {
			return 1.0 / tan(unFuncCalc);
		}
		if (str.find("arcctg(") == 0) {
			return atan(1.0 / unFuncCalc);
		}
		if (str.find("abs(") == 0) {
			return abs(unFuncCalc);
		}
		if (str.find("sign(") == 0) {
			return unFuncCalc/abs(unFuncCalc);
		}
	}

	/*если у нас нет операций в выражении и это не вышеописанные функции, то возвращаем
	вещественное число от этого выражения*/
	if (mnind == -1) {
		return parseToDouble(str);
	}
	else {
		/*если нужно совершить операцию*/

		/*считаем левую и правую части*/
		double calcL = calc(left, signs, x);
		double calcR = calc(right, signs, x);

		/*возвращаем соответствующее значение*/
		if (str[mnind] == '+') {
			return calcL + calcR;
		}
		else if (str[mnind] == '-') {
			return calcL - calcR;
		}
		else if (str[mnind] == '*') {
			return calcL * calcR;
		}
		else if (str[mnind] == '/') {
			return calcL / calcR;
		}
		else if (str[mnind] == '^') {
			return pow(calcL, calcR);
		}
	}
	return 0;
}

int main()
{
	setlocale(LC_ALL, "Russian");

	string signs = "+-/*^";/*строка содержащая операции которые нам доступны*/
	string f;/*строка самого выражения*/
	string fileName;
	double a, b, h;/*соответствующие переменные*/

	cout << "Введите выражение: "; getline(cin, f);

	cout << "Введите a: "; cin >> a;
	cout << "Введите b: "; cin >> b;
	cout << "Введите h: "; cin >> h;
	cout << "Введите имя файла: "; cin >> fileName;

	FILE* fout = fopen(fileName.c_str(), "w");

	printf("|%12s|%12s|\n", "x", "f(x)");
	fprintf(fout, "|%12s|%12s|\n", "x", "f(x)");
	for (double x = a; x <= b; x += h) {
		printf("|%12lf|%12lf|\n", x, calc(f, signs, x));
		fprintf(fout, "|%12lf|%12lf|\n", x, calc(f, signs, x));
	}
	fclose(fout);

}
