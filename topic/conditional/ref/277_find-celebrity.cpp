// LC 277 Find the Celebrity: among n people labeled 0..n-1, a celebrity is known by
// everyone else and knows no one. Given knows(a, b), return the celebrity's label or
// -1, using as few knows calls as possible.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// test-local fake of the judge's API; know[a][b] = a knows b
vector<vector<int>> know;

bool knows(int a, int b) {
    return know[a][b] == 1;
}

// Elimination: if cand knows i, cand is not a celebrity; otherwise i is not. One pass
// leaves a single candidate, a second pass verifies it. T: O(n) knows calls, S: O(1).
int findCelebrity(int n) {
    int cand = 0;
    for (int i = 1; i < n; i++) {
        if (knows(cand, i)) { cand = i; }
    }
    for (int i = 0; i < n; i++) {
        if (i == cand) { continue; }
        if (knows(cand, i) || !knows(i, cand)) { return -1; }
    }
    return cand;
}

void test(const string& label, const vector<vector<int>>& relation, int want) {
    know = relation;
    cout << label << (findCelebrity(relation.size()) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("celebrity-last", {{1, 1, 1}, {1, 1, 1}, {0, 0, 1}}, 2);
    test("mutual-friends", {{1, 1}, {1, 1}}, -1);
    test("unknown-by-one", {{1, 1, 1}, {0, 1, 0}, {0, 0, 1}}, -1);
    test("alone", {{1}}, 0);
}
