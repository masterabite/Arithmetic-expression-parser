#include <iostream>
#include <string>
#include <locale>
#include <fstream>

using namespace std;

int prior(char c) {
	//������� ���������� ��������� ��������

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

bool isFunction(string &str) {
	//������� ��������� �������� �� ��������� �������� �� ������� ���������

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
	//������� ��������� ������ str � ������������ �����

	double res = 0; /*� ���������� ����� ��������� ��������������� ������*/
	int point = str.size(); //������ �����������

	for (int i = 0; i < int(str.size()); ++i) {
		if (str[i] == '.') {
			/*������� ������� ��� �����������, ����� ����� ������� �������� �����*/
			point = i;
		}
	}
	
	double k = pow(10.0, point - 1); /*10 � ������� ������������� ������� 
									 (�� ���� ��� ���-�� �������� �� �����������)*/
	

	for (int i = 0; i < int(str.size()); ++i) {
		if (isdigit(str[i])) {
			res += k * int(char(str[i]) - '0');/*���������� ��������������� ������*/
			k /= 10;/*��������� ������*/
		}
		else if (str[i] != '.') {/*���� ��������� ��������� ������ ������ 
								 ��������� �� ���������*/
			cout << "�� ������� ���������� ���������: " << str << '\n';
			return res;
		}
	}

	/*���� �� �� ���������� ��������*/
	return res;
}

double calc(string str, string& signs, double& x) {
	/*������� ��������� ��������� str, ��������� 
	�������� signs � ���������� ������ x �������� &x*/
	
	/*������� ��� ������� ������*/
	while (str[0] == '(' && str[int(str.size()) - 1] == ')') {
		str.erase(str.begin());
		str.erase(str.end()-1);
	}
	
	/*������� ��� ������ �������*/
	for (int i = 0; i < int(str.size()); ++i) {
		while (i < int(str.size()) && str[i] == ' ') {
			str.erase(str.begin() + i);
		}
	}

	/*���� �������� ������ ��������� ���������� 0*/
	if (str == "") {
		return 0;
	}

	/*������ x ���������� ��������������� ��������*/
	if (str == "x") {
		return x;
	}

	/*p-�������������� ��������� (��� ������)
	mn-������������ ��������� ��������
	mxind- ������ �������� � ����������� �����������
	cur- �������� ��� �����*/
	int p = 0, mn = 3, mnind = -1, cur = 0;

	for (; cur < int(str.size()); ++cur) {
		
		/*���� ������� � ������ �������� ���������*/
		if (str[cur] == '(') {
			p += 4;
		}

		/*�������������� �������� ���������*/
		if (str[cur] == ')') {
			p -= 4;
		}

		/*���� ��������, ������� ����� �������� � ��������� �������
		�.�. �������� � ����������� ����������� � ����� ������ �� �����������*/
		if (signs.find(str[cur]) != -1 && prior(str[cur]) + p <= mn) {
			mn = prior(str[cur]) + p;
			mnind = cur;
		}
	}

	/*������ ���������� ����� � ������ ����� ��������*/
	string left = "";
	string right = "";

	/*��������� �� ��������������*/
	for (int i = 0; i < mnind; ++i) {
		left += str[i];
	}
	for (int i = mnind+1; i < int(str.size()); ++i) {
		right += str[i];
	}

	/*����������� ��� �������, ����� ������*/
	//cout << str << "= " << left << "?" << right << '\n';

	/*������, ����� ���� ��������� ������������ �� ���� ������� (sin, cos, tg ...)*/
	if (str.find("sin(") == 0 && isFunction(str)) {
		string unSin = "";
		for (int i = 4; i < int(str.size()) - 1; ++i) {
			unSin += str[i];
		}
		return sin(calc(unSin, signs, x));
	}
	if (str.find("cos(") == 0 && isFunction(str)) {
		string unSin = "";
		for (int i = 4; i < int(str.size()) - 1; ++i) {
			unSin += str[i];
		}
		return cos(calc(unSin, signs, x));
	}
	if (str.find("tg(") == 0 && isFunction(str)) {
		string unSin = "";
		for (int i = 3; i < int(str.size()) - 1; ++i) {
			unSin += str[i];
		}
		return tan(calc(unSin, signs, x));
	}
	if (str.find("ctg(") == 0 && isFunction(str)) {
		string unSin = "";
		for (int i = 4; i < int(str.size()) - 1; ++i) {
			unSin += str[i];
		}
		return 1.0/tan(calc(unSin, signs, x));
	}
	if (str.find("abs(") == 0 && isFunction(str)) {
		string unSin = "";
		for (int i = 4; i < int(str.size()) - 1; ++i) {
			unSin += str[i];
		}
		return abs(calc(unSin, signs, x));
	}


	/*���� � ��� ��� �������� � ��������� � ��� �� ������������� �������, �� ����������
	������������ ����� �� ����� ���������*/
	if (mnind == -1) {
		return parseToDouble(str);
	}
	else {
		/*���� ����� ��������� ��������*/

		/*������� ����� � ������ �����*/
		double calcL = calc(left, signs, x);
		double calcR = calc(right, signs, x);

		/*���������� ��������������� ��������*/
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
			return pow(calcL,calcR);
		}
	}
}

int main()
{
	setlocale(LC_ALL, "Russian");

	string signs = "+-/*^";/*������ ���������� �������� ������� ��� ��������*/
	string head = "x	f(x)\n";
	string f;/*������ ������ ���������*/
	string fileName;
	double a, b, h;/*��������������� ����������*/
	
	cout << "������� ���������: "; getline(cin, f);
	
	cout << "������� a: "; cin >> a;
	cout << "������� b: "; cin >> b;
	cout << "������� h: "; cin >> h;
	cout << "������� ��� �����: "; cin >> fileName;

	ofstream fout(fileName);

	fout << head;
	cout << head;
	for (double x = a; x <= b; x += h) {
		cout << x << "	" << calc(f, signs, x) << '\n';
		fout << x << "	" << calc(f, signs, x) << '\n';
	}
	fout.close();
	
}
