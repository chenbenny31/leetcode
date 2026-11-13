// 93 · Interval List Intersections — LC 986
// Mother problem: select/11-intervals-greedy/04-interval-list-intersections.cpp
//
// LC 986 (Medium), two pointers over sorted lists: two lists of closed intervals, each
// sorted and pairwise disjoint; return every non-empty intersection in order.
// Google reports: LC 986, union of two interval lists
//
// Clarify
// - shared endpoint → gives [x, x]; either list empty → empty result
//
// Core idea
// - Overlap = [max start, min end], kept if start <= end.
// - Advance whichever interval ends first; it cannot overlap anything later.
// - T O(m + n), S O(1) besides the output.
//
// Tests: one wide containing two, touching, one empty, negatives
//
// Follow-ups
// - union of the two lists → merge by start like merge sort, extend the last output
//   interval as in LC 56 (start <= last end merges, so touching ones merge)
// - unsorted / overlapping lists → merge each first (LC 56), then the same pointers
// - k lists → fold result = intersect(result, list[i])
// - union of K lists → min-heap of (start, list, index), extend or append, O(N log K)
// - union too big for memory → stream both files the same way, write out the last
//   interval once the next start passes it
//
//////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;
using std::min;

// Overlap = [max start, min end]; advance whichever interval ends first.
// T: O(m + n), S: O(1) besides the output.
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
