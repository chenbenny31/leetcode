// LC 726 Number of Atoms: given a chemical formula such as "Mg(OH)2", return the count
// of each atom as a string: names in sorted order, each followed by its count if > 1.
// Names are an uppercase letter plus lowercase letters; groups in parentheses can be
// followed by a multiplier.
#include <iostream>
#include <vector>
#include <string>
#include <map>
using std::cout;
using std::map;
using std::string;
using std::to_string;
using std::vector;

// Reads the digits at s[i..] and advances i; returns 1 when there are none.
int readCount(const string& s, int& i) {
    int n = s.size(), count = 0;
    bool any = false;
    while (i < n && s[i] >= '0' && s[i] <= '9') {
        count = count * 10 + (s[i] - '0');
        any = true;
        i++;
    }
    return any ? count : 1;
}

// One pass with a stack of atom counts, one map per open paren; on ')' merge the group,
// scaled by its multiplier, into the enclosing map. T: O(n * k), S: O(n) for k atoms.
string countOfAtoms(const string& formula) {
    int n = formula.size(), i = 0;
    // atom name -> count inside the current open group; stk[0] is the whole formula
    vector<map<string, int>> stk(1);
    while (i < n) {
        char c = formula[i];
        if (c == '(') {
            stk.push_back({});
            i++;
        } else if (c == ')') {
            i++;
            int mult = readCount(formula, i);
            map<string, int> group = stk.back();
            stk.pop_back();
            for (const auto& [name, count] : group) {
                stk.back()[name] += count * mult;
            }
        } else {
            int j = i + 1;
            while (j < n && formula[j] >= 'a' && formula[j] <= 'z') {
                j++;
            }
            string name = formula.substr(i, j - i);
            i = j;
            stk.back()[name] += readCount(formula, i);
        }
    }
    string res;
    for (const auto& [name, count] : stk[0]) {
        res += name;
        if (count > 1) { res += to_string(count); }
    }
    return res;
}

void test(const string& label, const string& formula, const string& want) {
    cout << label << (countOfAtoms(formula) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("sorted-counts", "H2O", "H2O");
    test("group", "Mg(OH)2", "H2MgO2");
    test("nested-group", "(Be(OH)2)3", "Be3H6O6");
    test("two-letter-atom", "Fe", "Fe");
}
