// LC 986 Interval List Intersections (Medium) — two pointers over two sorted lists
// two lists of closed intervals, each sorted + pairwise disjoint
// return every non-empty intersection in order; shared endpoint gives [x, x]
//
// clarify:
// closed or half-open -> closed
// each list sorted + disjoint -> yes; else merge each first (LC 56)
// single-point intersections -> kept
// an empty list -> {}

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;
using std::min;

// core idea: overlap of the two cur intervals = [max start, min end]; advance earlier end
// T: O(m + n), S: O(1) — besides the output
vector<vector<int>> intervalIntersection(const vector<vector<int>>& first,
                                         const vector<vector<int>>& second) {
    int m = first.size();
    int n = second.size();
    vector<vector<int>> result;
    int i = 0;
    int j = 0;
    while (i < m && j < n) {
        int start = max(first[i][0], second[j][0]);
        int end = min(first[i][1], second[j][1]);
        if (start <= end) { result.push_back({start, end}); }
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
    bool ok = intervalIntersection(first, second) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("contained", {{0, 10}}, {{2, 3}, {5, 6}}, {{2, 3}, {5, 6}});
    test("touching", {{1, 4}}, {{4, 8}}, {{4, 4}});
    test("one-empty", {}, {{1, 5}}, {});
    test("negative", {{-5, -1}, {2, 3}}, {{-3, 2}}, {{-3, -1}, {2, 2}});
}

// follow-up:
// union of the two lists (Google) -> merge by start; extend last output if start <= end
// unsorted / overlapping lists -> merge each (LC 56) first, then same two pointers
// intersection of k lists -> fold: result = intersect(result, list[i])
// union of K lists -> min-heap of (start, list, idx); pop, extend or append, O(N log K)
// union too big for memory -> stream both files, flush last interval once passed
