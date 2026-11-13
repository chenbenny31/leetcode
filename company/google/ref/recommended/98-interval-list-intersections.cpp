// 98 · Interval List Intersections — LC 986
// Mother: select/11-intervals-greedy/04-interval-list-intersections.cpp
// two sorted, disjoint lists of closed intervals
// (1) LC 986: every intersection, in order
// (2) union of the two lists, in order
// reported: LC 986; (2) in an L4 / L5 loop w/ K-list and out-of-memory follow-ups
//
// clarify:
// closed intervals -> a shared endpoint gives [x, x]
// each list -> sorted and disjoint
// either list empty -> (1) empty, (2) the other list
// (2) touching [1, 4] [4, 8] -> one interval; [1, 2] [3, 4] stay apart

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;
using std::min;

// (1) intersections
// core idea: overlap = [max start, min end], kept if start <= end
// then advance whichever interval ends first; it cannot overlap anything later
// T: O(m + n), S: O(1) + output
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
        if (first[i][1] < second[j][1]) {
            i++;
        } else {
            j++;
        }
    }
    return result;
}

// (2) union
// core idea: merge-sort step: take the interval w/ the smaller start
// extend the last output if it overlaps, else append
// T: O(m + n), S: O(1) + output
vector<vector<int>> intervalUnion(const vector<vector<int>>& first,
                                  const vector<vector<int>>& second) {
    int m = first.size();
    int n = second.size();
    vector<vector<int>> result;
    int i = 0;
    int j = 0;
    while (i < m || j < n) {
        bool takeFirst = j == n || (i < m && first[i][0] <= second[j][0]);
        vector<int> cur;
        if (takeFirst) {
            cur = first[i];
            i++;
        } else {
            cur = second[j];
            j++;
        }
        if (!result.empty() && cur[0] <= result.back()[1]) {
            result.back()[1] = max(result.back()[1], cur[1]);
        } else {
            result.push_back(cur);
        }
    }
    return result;
}

void test(const string& label, const vector<vector<int>>& first,
          const vector<vector<int>>& second, const vector<vector<int>>& want) {
    bool ok = intervalIntersection(first, second) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testUnion(const string& label, const vector<vector<int>>& first,
               const vector<vector<int>>& second, const vector<vector<int>>& want) {
    cout << label << (intervalUnion(first, second) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("contained", {{0, 10}}, {{2, 3}, {5, 6}}, {{2, 3}, {5, 6}});
    test("touching", {{1, 4}}, {{4, 8}}, {{4, 4}});
    test("one-empty", {}, {{1, 5}}, {});
    test("negative", {{-5, -1}, {2, 3}}, {{-3, 2}}, {{-3, -1}, {2, 2}});

    testUnion("union-overlap", {{1, 3}, {6, 9}}, {{2, 5}, {8, 10}, {12, 13}},
              {{1, 5}, {6, 10}, {12, 13}});
    testUnion("union-touching", {{1, 4}}, {{4, 8}}, {{1, 8}});
    testUnion("union-contained", {{0, 10}}, {{2, 3}, {5, 6}}, {{0, 10}});
    testUnion("union-one-empty", {}, {{1, 5}, {7, 9}}, {{1, 5}, {7, 9}});
}

// follow-up:
// (2) why not concat + sort (LC 56) -> O(N log N), wastes the sorted input
// union of K lists -> min-heap of (start, list, idx); extend or append, O(N log K)
// union too big for memory -> stream the lists; write an interval once a start passes it
// K huge files -> external merge: K-way heap over file cursors, or merge pairs in rounds
// lists w/ overlaps inside -> merge-by-start still works (extend handles it); else LC 56
// intersection of k lists -> fold: result = intersect(result, next list)
