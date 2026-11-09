// LC 341 Flatten Nested List Iterator (Medium). Pattern: stack of (list, index).
// Iterate a nested list of integers in order: next() returns the next integer,
// hasNext() says whether one is left. Lists may be empty or deeply nested.

#include <iostream>
#include <vector>
#include <string>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::pair;

// minimal stand-in for LC's NestedInteger interface
struct NestedInteger {
    bool isInt;
    int val;
    vector<NestedInteger> list;

    bool isInteger() const { return isInt; }
    int getInteger() const { return val; }
    const vector<NestedInteger>& getList() const { return list; }
};

// Lazy DFS: hasNext descends into lists and pops finished ones until an integer is on
// top. T: O(1) amortized per call, S: O(depth).
class NestedIterator {
    // (list, next index in it); back = innermost list being walked
    vector<pair<const vector<NestedInteger>*, int>> stack;

public:
    NestedIterator(const vector<NestedInteger>& nestedList) {
        stack.push_back({&nestedList, 0});
    }

    int next() {
        // makes sure an integer is on top
        hasNext();
        auto& [items, index] = stack.back();
        int val = (*items)[index].getInteger();
        index++;
        return val;
    }

    bool hasNext() {
        while (!stack.empty()) {
            auto& [items, index] = stack.back();
            if (index == (int)items->size()) {
                stack.pop_back();
                continue;
            }
            const NestedInteger& cur = (*items)[index];
            if (cur.isInteger()) { return true; }
            // step past the sublist first so the parent resumes after it
            index++;
            stack.push_back({&cur.getList(), 0});
        }
        return false;
    }
};

NestedInteger num(int value) { return {true, value, {}}; }
NestedInteger nest(const vector<NestedInteger>& items) { return {false, 0, items}; }

void test(const string& label, const vector<NestedInteger>& nestedList,
          const vector<int>& want) {
    NestedIterator it(nestedList);
    vector<int> got;
    while (it.hasNext()) {
        got.push_back(it.next());
    }
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("mixed", {num(1), nest({num(2), num(3)}), num(4)}, {1, 2, 3, 4});
    test("deep", {nest({nest({nest({num(5)})})}), num(6)}, {5, 6});
    test("empty-lists", {nest({}), nest({nest({})})}, {});
}

// follow-up: simplest -> dfs-flatten everything into a vector in the constructor,
//   then walk an index; O(total) memory up front instead of O(depth)
