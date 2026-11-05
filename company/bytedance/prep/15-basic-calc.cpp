// ops?  + - * / space, no parenthese
// epn valid? yes, no div by 0, no consec op
// unary minus? no, all numbers non-neg
// div? int dev truncate, e.g. -7/2 = -3
// overflow

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// collect term with signed, apply */ to last term, T: O(n), S: O(n)
int calculate(const string& s) {
    vector<int> terms; // signed terms, sum to result
    int num = 0;
    char op = '+';
    int n = s.size();
    for (int i = 0; i < n; i++) {
        char c = s[i];
        bool isDigit = (c >= '0' && c <= '9');
        if (isDigit) { num = num * 10 + (c - '0'); }

        if ((!isDigit && c != ' ') || i == n - 1) {
            if (op == '+') {
                terms.push_back(num);
            } else if (op == '-') {
                terms.push_back(-num);
            } else if (op == '*') {
                terms.back() *= num;
            } else {
                terms.back() /= num;
            }
            op = c;
            num = 0;
        }
    }

    int result = 0;
    for (int t : terms) {
        result += t;
    }
    return result;
}

// follow-up: keep only last term over stack, T: O(n), S: O(1)
int calculateO1(const string& s) {
    int result = 0;
    int last = 0;
    int num = 0;
    char op = '+';
    int n = s.size();
    for (int i = 0; i < n; i++) {
        char c = s[i];
        bool isDigit = (c >= '0' && c <= '9');
        if (isDigit) { num = num * 10 + (c - '0'); }
        if ((!isDigit && c != ' ') || i == n - 1) {
            if (op == '+' || op == '-') {
                result += last;
                last = (op == '+' ? num : -num);
            } else if (op == '*') {
                last *= num;
            } else {
                last /= num;
            }
            op = c;
            num = 0;
        }
    }
    return result + last;
}

void test(const string& label, const string& s, int want) {
    bool ok = calculate(s) == want && calculateO1(s) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", "3+2*2", 7);
    test("lc ex2", " 3/2 ", 1);
    test("lc ex3", " 3+5 / 2 ", 5);
    test("minus then divide", "1-7/2", -2);
    test("chain", "2*3*4-10/3", 21);
}
