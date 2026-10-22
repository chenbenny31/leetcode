// LC 1944 Number of Visible People in a Queue: people stand left to right with distinct
// heights. Person i sees person j > i if everyone between them is shorter than both.
// Return, for each i, how many people to the right i can see.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// scan from the right with a monotonic stack of the heights still visible from the left;
// the people popped by i are the shorter ones it sees, plus the first taller one left
// on the stack.
// T: O(n), S: O(n).
vector<int> canSeePersonsCount(const vector<int>& heights) {
    int n = heights.size();
    vector<int> seen(n, 0);
    vector<int> stk;   // heights to the right of i that are not hidden, nearest on top
    for (int i = n - 1; i >= 0; i--) {
        while (!stk.empty() && stk.back() < heights[i]) {
            stk.pop_back();
            seen[i]++;
        }
        // the taller one that stopped the popping is still visible
        if (!stk.empty()) { seen[i]++; }
        stk.push_back(heights[i]);
    }
    return seen;
}

void test(const string& label, const vector<int>& heights, const vector<int>& want) {
    if (canSeePersonsCount(heights) == want) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    test("descending", {3, 2, 1}, {1, 1, 0});
    test("valley", {5, 1, 2, 6}, {3, 1, 1, 0});
    test("peak", {4, 2, 3, 1}, {2, 1, 1, 0});
    test("single", {4}, {0});
}
