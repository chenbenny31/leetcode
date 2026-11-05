// ByteDance OA: houses = distinct integer positions; queries = distinct positions from
// houses, destroyed in order. A segment is a maximal run of remaining houses at
// consecutive positions. Return the segment count after each destruction.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>

using std::cout;
using std::string;
using std::vector;
using std::unordered_set;

// Hash set of alive positions; removing x depends only on x - 1 and x + 1.
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
        // both alive: a split (+1); neither: x was a whole segment (-1); one: an end (0)
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

// follow-up: houses added instead -> mirror: both neighbors alive -> -1, neither -> +1,
//   one -> 0
// follow-up: largest segment after each removal -> reverse queries, add back with DSU
//   (union x with x-1, x+1), track size[root], max only grows, O((n+q) a(n))
// follow-up: no hash set / worst-case bound -> sort houses, alive[] by index; neighbor
//   idx +- 1 counts only if its position differs by 1, O(n log n)
