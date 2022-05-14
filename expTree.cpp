#include <iostream>
#include <string>

using namespace std;

//структура узла дерева
struct node {
    double res; //результат вычислений
    node* left;
    node* right;
};

//структура дерева
struct tree {
    node* root;
    string ops; //допустимые операции
    string tabs; //игнорируемые символы

    tree() {
        root = new node();
        ops = "-+*/";
        tabs = "\t ";
    }

    ~tree() {
        clean();
    }

    void clean() {
        cleanNode(root);
    }

    void cleanNode(node* cur) {
        if (cur == nullptr) {return;}
        cleanNode(cur->left);
        cleanNode(cur->right);
        delete(cur);
    }

    //вычисляем приоритет операции
    static int prior(char op) {
        switch(op){
            case '+':
                return 1;
            case '-':
                return 1;
            case '*':
                return 2;
            case '/':
                return 2;
            default:
                return 0;
        }
    }

    //совершаем операцию op с числами a, b
    static double doOp(char op, double a, double b) {
        switch(op){
            case '+':
                return a+b;
            case '-':
                return a-b;
            case '*':
                return a*b;
            case '/':
                return a/b;
            default:
                return 0;
        }
    }

    //считаем выражение exp начиная от узла cur
    double calculate(node* cur, string exp) {

        //находим позицию разделения
        int b = -1;
        for (int i = 0; i < (int)exp.size(); ++i) {
            if (ops.find(exp[i]) != string::npos) {
                if (b == -1 || prior(exp[i]) <= prior(exp[b])) {
                    b = i;
                }
            }
        }

        if (b == -1) {                        //если операций нет
            //парсим строку в число
            cur->res = atof(exp.c_str());

        } else {                            //если есть хотя бы одна операция

            if (cur->left == nullptr) {
                cur->left = new node();
            }
            if (cur->right == nullptr) {
                cur->right = new node();
            }

            //деление на правую и левую часть
            string exp_left = exp.substr(0, b);
            string exp_right = exp.substr(b+1, exp.size()-b-1);

            cur->res = doOp(exp[b], calculate(cur->left, exp_left), calculate(cur->right, exp_right)) ;
        }

        return cur->res;
    }
};

int main() {

    string exp, cleanExp;
    tree t;

    while (true) {
        cout << "Enter expression: ";
        getline(cin, exp);

        cleanExp = "";
        for (char c: exp) {
            if (t.tabs.find(c) == string::npos) {
                cleanExp += c;
            }
        }

        cout << exp << " = " << t.calculate(t.root, cleanExp) << '\n';

        int cmd;
        cout << "continue? 0/1: "; cin >> cmd;
        getline(cin, exp);

        if (cmd == 0) {
            break;
        }
    }
    return 0;
}
