// LC 990 Satisfiability of Equality Equations: each equation is "a==b" or "a!=b" over
// lowercase variables. Return true if integers can be assigned to satisfy all of them.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

int findRoot(vector<int>& uf, int x) {
    while (uf[x] != x) {
        uf[x] = uf[uf[x]];
        x = uf[x];
    }
    return x;
}

// Union all "==" variables first, then no "!=" pair may share a group.
// T: O(N * alpha), S: O(1) (26 letters).
bool equationsPossible(vector<string>& equations) {
    vector<int> uf(26);   // uf[x] = parent of letter x in its equality group
    for (int i = 0; i < 26; i++) { uf[i] = i; }
    for (const auto& eq : equations) {
        if (eq[1] == '=') { uf[findRoot(uf, eq[0] - 'a')] = findRoot(uf, eq[3] - 'a'); }
    }
    for (const auto& eq : equations) {
        if (eq[1] == '!' && findRoot(uf, eq[0] - 'a') == findRoot(uf, eq[3] - 'a')) {
            return false;
        }
    }
    return true;
}

void test(const string& label, vector<string> equations, bool want) {
    cout << label << (equationsPossible(equations) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("transitive-clash", {"a==b", "b==c", "a!=c"}, false);
    test("separate-groups", {"a==b", "c==d", "a!=c"}, true);
    test("self-differs", {"a!=a"}, false);
    test("order-independent", {"a!=c", "a==b", "b==c"}, false);
}
