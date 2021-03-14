#include <stack>
#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

int calc(char c, int n1, int n2)
{
    // cout << c << "-->" << n1 << " and " << n2 << endl;
    if (c == '+') return n1+n2;
    else if (c == '-') return n1-n2;
    else if (c == '*') return n1*n2;
    else throw runtime_error("bad operator");
}

void update(stack<int>& numbers, stack<char>& operators)
{
    if (operators.size() + 1 != numbers.size()) throw runtime_error("bad");
    char op = operators.top();
    operators.pop();

    int n2 = numbers.top();
    numbers.pop();

    int n1 = numbers.top();
    numbers.pop();

    numbers.push(calc(op, n1, n2));
}

int processMath(const string& input) {
    int num = 0;

    stack<int> numbers;
    stack<char> operators;

    for (char c : input) {
        if (c == '+' || c == '-' || c == '*') {
            numbers.push(num);
            num = 0; // reset number
            if (c == '*' && !operators.empty() && operators.top() == '*') {
                update(numbers, operators);
            } else if (c == '+' || c == '-') { // c is + or -
                while (!operators.empty()) update(numbers, operators);
            }
            operators.push(c);
        } else {
            num = num*10+(c-'0');
            // cout << "num=" << num << endl;
        }
    }
    numbers.push(num);
    while (!operators.empty()) update(numbers, operators);
    return numbers.top();
}

// To execute C++, please define "int main()"
int main() {

    string exp1 = "13+15";
    string exp2 = "3+25*4";
    string exp3 = "1-2*2*2+7";

    cout << exp1 << endl << processMath(exp1) << endl << endl;
    cout << exp2 << endl << processMath(exp2) << endl << endl;
    cout << exp3 << endl << processMath(exp3) << endl << endl;

    return 0;
}
