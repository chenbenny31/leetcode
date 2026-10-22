// LC 341 Flatten Nested List Iterator: nested list of integers (lists may be empty or
// deeply nested). next() returns the next integer in order; hasNext() says if one is
// left.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::pair;
using std::string;
using std::vector;

struct NestedInteger {
    bool isInt;
    int val;                       // valid when isInt
    vector<NestedInteger> list;    // valid when !isInt
    bool isInteger() const { return isInt; }
    int getInteger() const { return val; }
    const vector<NestedInteger>& getList() const { return list; }
};

class NestedIterator {
    // (list, next index in it); the back is the innermost list being walked
    vector<pair<const vector<NestedInteger>*, int>> stk;

public:
    NestedIterator(const vector<NestedInteger>& nestedList) {
        stk.push_back({&nestedList, 0});
    }

    // hasNext leaves an integer under the top index. T: O(1) amortized.
    int next() {
        hasNext();
        auto& top = stk.back();
        return (*top.first)[top.second++].getInteger();
    }

    // lazy DFS with an explicit stack: pop finished lists, descend into sublists until
    // an integer is next. T: O(1) amortized, S: O(depth).
    bool hasNext() {
        while (!stk.empty()) {
            auto& top = stk.back();
            const vector<NestedInteger>& lst = *top.first;
            if (top.second == (int)lst.size()) {
                stk.pop_back();
                continue;
            }
            const NestedInteger& cur = lst[top.second];
            if (cur.isInteger()) { return true; }
            // step past the sublist first so the parent resumes after it
            top.second++;
            stk.push_back({&cur.getList(), 0});
        }
        return false;
    }
};

NestedInteger num(int v) { return {true, v, {}}; }
NestedInteger nest(const vector<NestedInteger>& l) { return {false, 0, l}; }

void test(const string& label, const vector<NestedInteger>& nestedList,
          const vector<int>& want) {
    NestedIterator it(nestedList);
    vector<int> got;
    while (it.hasNext()) { got.push_back(it.next()); }
    if (got == want) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    test("flat", {num(1), num(2), num(3)}, {1, 2, 3});
    test("deep", {num(1), nest({num(4), nest({num(6)}), num(5)}), num(7)},
         {1, 4, 6, 5, 7});
    test("empty-lists", {nest({}), nest({nest({})}), num(9)}, {9});
    test("empty", {}, {});
}
