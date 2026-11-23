// 41 · Apple Grouping (Cisco OA, Medium) — sort + greedy
// (1) min num of groups so that max - min weight inside every group <= gap
// (2) max num of full boxes of exactly k apples, max - min inside a box <= gap
//
// clarify:
// which variant -> reports differ; (1) is the usual one, (2) is the box variant
// (2) leftover apples -> allowed, just not counted
// gap >= 0, weights are ints -> yes
// empty input -> 0

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;

// ==================== (1) ====================
// min num of groups with max - min <= gap in each

// core idea: sort; the lightest unassigned apple opens a group and takes all within gap
// T: O(n log n), S: O(n) — sorted copy
int minGroups(vector<int> weights, int gap) {
    sort(weights.begin(), weights.end());
    int groups = 0;
    // lightest apple of the cur group
    int groupMin = 0;
    for (int w : weights) {
        if (groups == 0 || w - groupMin > gap) {
            groups++;
            groupMin = w;
        }
    }
    return groups;
}

// ==================== (2) ====================
// max num of boxes of exactly k apples with max - min <= gap

// core idea: sort; a box is k adjacent apples; take the earliest window that fits
// T: O(n log n), S: O(n)
int maxFullBoxes(vector<int> weights, int k, int gap) {
    sort(weights.begin(), weights.end());
    int n = weights.size();
    int boxes = 0;
    int i = 0;
    while (i + k <= n) {
        if (weights[i + k - 1] - weights[i] <= gap) {
            boxes++;
            i += k;
        } else {
            // its k - 1 closest heavier apples are too far, so i fits in no box
            i++;
        }
    }
    return boxes;
}

// ==================== tests ====================

void test(const string& label, int got, int want) {
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("groups", minGroups({1, 5, 4, 6, 8, 9, 2}, 3), 3);
    test("groups-empty", minGroups({}, 2), 0);
    test("boxes", maxFullBoxes({1, 2, 3, 10, 11, 12, 30}, 3, 2), 2);
    test("boxes-skip", maxFullBoxes({1, 5, 6, 7}, 3, 2), 1);
    return 0;
}

// follow-up:
// 1. why greedy works -> the lightest apple needs a group anyway; make that group widest
// 2. print the groups -> start a new vector<int> at each cut
// 3. group size also capped at k -> new group when w - groupMin > gap or size == k
// 4. weights as a stream -> can't sort; keep a multiset, re-run the scan per query
// 5. min groups with max - min <= gap after sorting by value = LC 2294 (partition array)
