// ByteDance phone (FT), LC 986: two lists of closed intervals, each sorted by start
// and pairwise disjoint. Return every non-empty intersection, sorted by start; a
// shared endpoint is a one-point intersection [x, x].
//   intersectIntervalLists(firstList, secondList) -> vector<vector<int>>

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;
using std::min;

// Two pointers: overlap = [max start, min end]; advance whichever interval ends first.
// T: O(m + n), S: O(1) besides output.
vector<vector<int>> intersectIntervalLists(const vector<vector<int>>& first,
                                           const vector<vector<int>>& second) {
    vector<vector<int>> result;
    int i = 0;
    int j = 0;
    while (i < (int)first.size() && j < (int)second.size()) {
        int lo = max(first[i][0], second[j][0]);
        int hi = min(first[i][1], second[j][1]);
        if (lo <= hi) {
            result.push_back({lo, hi});
        }

        // the one ending first cannot overlap anything later in the other list
        if (first[i][1] < second[j][1]) {
            i++;
        } else {
            j++;
        }
    }
    return result;
}

void test(const string& label, const vector<vector<int>>& first,
          const vector<vector<int>>& second, const vector<vector<int>>& want) {
    bool ok = intersectIntervalLists(first, second) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("contained", {{0, 10}}, {{2, 3}, {5, 6}}, {{2, 3}, {5, 6}});
    test("touching", {{1, 4}}, {{4, 8}}, {{4, 4}});
    test("disjoint", {{1, 2}}, {{3, 4}}, {});
    test("one-empty", {}, {{1, 5}}, {});
    test("negative", {{-5, -1}, {2, 3}}, {{-3, 2}}, {{-3, -1}, {2, 2}});
}

// follow-up: union of the two lists -> merge by start, extend last if start <= last end,
//   O(m + n)
// follow-up: k lists -> fold cur = intersect(cur, list[i]), O(k * total) worst; or sweep
//   sorted endpoints (starts before ends on ties), keep spans where active == k
// follow-up: unsorted / overlapping input -> sort by start + merge (LC 56) each list,
//   then same two pointers, O((m + n) log(m + n))
