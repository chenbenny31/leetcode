// ByteDance / LC 224: evaluate a valid expression of non-negative integers, '+', '-',
// '(', ')' and spaces. '-' may be unary, e.g. "-(2+3)"; no eval().
#include <iostream>
#include <vector>
#include <string>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::pair;

// One pass; at '(' save (result so far, sign before the paren) and start fresh.
// T: O(n), S: O(n).
int calculate(const string& s) {
    // sum of finished terms at the current depth
    long long result = 0;
    // number being read
    long long num = 0;
    // sign before num
    int sign = 1;
    // saved[d] = (outer result, sign before '(') for each open paren
    vector<pair<long long, int>> saved;
    for (char c : s) {
        if (c >= '0' && c <= '9') {
            num = num * 10 + (c - '0');
        } else if (c == '+' || c == '-') {
            result += sign * num;
            num = 0;
            sign = c == '+' ? 1 : -1;
        } else if (c == '(') {
            saved.push_back({result, sign});
            result = 0;
            sign = 1;
        } else if (c == ')') {
            result += sign * num;
            num = 0;
            // the paren's value becomes one signed term of the outer sum
            result = saved.back().first + saved.back().second * result;
            saved.pop_back();
        }
    }
    return result + sign * num;
}

void test(const string& label, const string& s, int want) {
    cout << label << (calculate(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("plus", "1 + 1", 2);
    test("spaces", " 2-1 + 2 ", 3);
    test("nested", "(1+(4+5+2)-3)+(6+8)", 23);
    test("unary-minus", "-(2+3)", -5);
}

// follow-up: '*' and '/' (LC 227) -> stack of terms; at each operator apply the previous
//   op: +/- push +-num, * / update top (C++ / truncates toward 0); answer = sum
// follow-up: + - * / ( ) (LC 772) -> recursion: parse(i) runs the LC 227 loop until ')',
//   '(' recurses and its return value becomes num
// follow-up: general precedence -> shunting-yard (nums + ops stacks, pop while
//   prec(top) >= prec(cur)) or recursive descent expr / term / factor
