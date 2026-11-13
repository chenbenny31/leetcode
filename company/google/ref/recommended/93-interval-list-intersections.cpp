// 93 · Interval List Intersections — LC 986
// Mother: select/11-intervals-greedy/04-interval-list-intersections.cpp
// two sorted, disjoint lists of closed intervals; return every intersection, in order
// reported: LC 986, union of two interval lists
//
// clarify:
// closed intervals -> a shared endpoint gives [x, x]
// each list -> sorted and disjoint
// either list empty -> empty result
// union instead -> follow-up

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;
using std::min;

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
// union of the two lists (Google) -> merge by start like merge sort, extend last output
// union detail -> as in LC 56, touching intervals merge
// unsorted / overlapping lists -> merge each first (LC 56), then the same pointers
// k lists -> fold: result = intersect(result, next list)
// union of K lists -> min-heap of (start, list, idx); extend or append, O(N log K)
// union too big for memory -> stream both files; flush last interval once passed
