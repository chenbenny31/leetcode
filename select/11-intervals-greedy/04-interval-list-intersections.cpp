// LC 986 Interval List Intersections (Medium). Pattern: two pointers over sorted lists.
// Two lists of closed intervals, each sorted and pairwise disjoint. Return every
// non-empty intersection in order; a shared endpoint gives [x, x].

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

// follow-up: unsorted / overlapping lists -> merge each (LC 56) first, then the same
//   two pointers, O((m + n) log(m + n))
// follow-up: k lists -> fold result = intersect(result, list[i])
