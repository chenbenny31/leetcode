// ByteDance OA: houses holds distinct integer positions; queries holds distinct
// positions that are all in houses, destroyed one by one in order. A segment is a
// maximal group of remaining houses at consecutive positions. After each destruction
// record the number of segments; return the counts in query order.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>

using std::cout;
using std::string;
using std::vector;
using std::unordered_set;

// Keep the alive positions in a set. Removing x only touches its two neighbors:
// both alive -> one segment splits in two (+1); neither alive -> x was a segment of
// its own (-1); exactly one alive -> x was an end of a segment (+0).
// T: O(n + q) average, S: O(n).
vector<int> countHouseSegmentsAfterDestruction(const vector<int>& houses,
                                               const vector<int>& queries) {
    // positions of houses still standing
    unordered_set<int> alive(houses.begin(), houses.end());

    // every segment has exactly one house whose left neighbor is missing
    int segments = 0;
    for (int x : houses) {
        if (alive.count(x - 1) == 0) {
            segments++;
        }
    }

    vector<int> result;
    for (int x : queries) {
        alive.erase(x);
        bool left = alive.count(x - 1) > 0;
        bool right = alive.count(x + 1) > 0;
        if (left && right) {
            segments++;
        } else if (!left && !right) {
            segments--;
        }
        result.push_back(segments);
    }
    return result;
}

void test(const string& label, const vector<int>& houses, const vector<int>& queries,
          const vector<int>& want) {
    bool ok = countHouseSegmentsAfterDestruction(houses, queries) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("split-then-shrink", {5, 6, 7}, {6, 5, 7}, {2, 1, 0});
    test("middle-of-gaps", {1, 3, 5}, {3}, {2});
    test("negatives", {-2, -1, 0}, {-1}, {2});
}
