// LC 341 Flatten Nested List Iterator (Medium) — stack of (list, index)
// iterate a nested list of ints in order: next() = next int, hasNext() = one is left
// lists may be empty or deeply nested
//
// clarify:
// empty sublists -> hasNext must skip them
// hasNext called several times in a row -> must be idempotent
// next w/o hasNext before it -> next calls hasNext itself
// input outlives the iterator -> store pointers, no copy

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

// core idea: lazy DFS; hasNext descends into lists, pops finished ones until int on top
// T: O(1) amortized per call, S: O(depth) — each list is pushed and popped once
class NestedIterator {
    // (list, next idx in it); back = innermost list being walked
    vector<pair<const vector<NestedInteger>*, int>> stack;

public:
    NestedIterator(const vector<NestedInteger>& nestedList) {
        stack.push_back({&nestedList, 0});
    }

    int next() {
        // makes sure an int is on top
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
    return 0;
}

// follow-up:
// simplest version -> dfs-flatten into a vector in the constructor; O(total) memory
