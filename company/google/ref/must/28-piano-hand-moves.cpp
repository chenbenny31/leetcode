// Piano Hand Moves (Google, interview report)
// (1) one hand, notes in order: split into consecutive groups w/ max - min <= 4
// (1) group after the 1st = 1 move; finger = note - group min; return moves + fingers
// (2) a move costs the num of keys the hand shifts; return least total shift
// (3) two hands, a note may go to either, move = one hand jumping; return fewest moves
//
// clarify:
// notes -> key numbers (88 keys), repeats allowed
// range -> over the whole group, not vs its first note
// first placement -> free
// empty input -> 0
// hands may cross in (3)? -> yes

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::min;
using std::max;
using std::min_element;
using std::max_element;

struct Plan {
    int moves;
    // fingers[i] = finger for notes[i]
    vector<int> fingers;
};

// (1) fewest moves, one hand
// core idea: greedy; extend the group while max - min <= 4, else start a new one
// taking as many notes as possible never pushes later groups later
// T: O(n), S: O(n)
Plan planHands(const vector<int>& notes) {
    int n = notes.size();
    Plan plan{0, vector<int>(n)};
    int groupStart = 0;
    while (groupStart < n) {
        int low = notes[groupStart];
        int high = notes[groupStart];
        // notes[groupStart..groupEnd) form the current group
        int groupEnd = groupStart + 1;
        while (groupEnd < n) {
            int newLow = min(low, notes[groupEnd]);
            int newHigh = max(high, notes[groupEnd]);
            if (newHigh - newLow > 4) { break; }
            low = newLow;
            high = newHigh;
            groupEnd++;
        }
        // fingers only once the group is closed: its min may come late
        for (int idx = groupStart; idx < groupEnd; idx++) {
            plan.fingers[idx] = notes[idx] - low;
        }
        if (groupStart > 0) { plan.moves++; }
        groupStart = groupEnd;
    }
    return plan;
}

// (2) least total shift
// core idea: thumb at p covers p..p+4, so note x needs p in [x - 4, x]
// [lo, hi] = cheapest thumb positions; overlap -> intersect, else shift to nearest end
// T: O(n), S: O(1)
int minMoveDistance(const vector<int>& notes) {
    int n = notes.size();
    if (n == 0) { return 0; }
    // cheapest thumb positions so far
    int lo = notes[0] - 4;
    int hi = notes[0];
    int distance = 0;
    for (int i = 1; i < n; i++) {
        int needLo = notes[i] - 4;
        int needHi = notes[i];
        if (hi < needLo) {
            distance += needLo - hi;
            lo = needLo;
            hi = needLo;
        } else if (lo > needHi) {
            distance += lo - needHi;
            lo = needHi;
            hi = needHi;
        } else {
            lo = max(lo, needLo);
            hi = min(hi, needHi);
        }
    }
    return distance;
}

// (3) two hands, fewest moves
// core idea: best[a][b] = fewest moves w/ thumbs at a, b; inf if note is under neither
// a hand stays (0) or jumps (1): min(best, rowMin[a] + 1, colMin[b] + 1, allMin + 2)
// T: O(n * K^2), S: O(K^2) — K = num of thumb positions
int minMovesTwoHands(const vector<int>& notes) {
    int n = notes.size();
    if (n == 0) { return 0; }
    int low = *min_element(notes.begin(), notes.end());
    int high = *max_element(notes.begin(), notes.end());
    // thumb positions low - 4 .. high, stored as idx = position - base
    int base = low - 4;
    int k = high - base + 1;
    // inf = more moves than any plan needs: state cannot play the notes so far
    int inf = n + 1;
    // before the first note every placement is free
    vector<vector<int>> best(k, vector<int>(k, 0));
    for (int note : notes) {
        // fewest moves w/ thumb A at a / thumb B at b / anywhere, before this note
        vector<int> rowMin(k, inf);
        vector<int> colMin(k, inf);
        int allMin = inf;
        for (int a = 0; a < k; a++) {
            for (int b = 0; b < k; b++) {
                rowMin[a] = min(rowMin[a], best[a][b]);
                colMin[b] = min(colMin[b], best[a][b]);
                allMin = min(allMin, best[a][b]);
            }
        }
        for (int a = 0; a < k; a++) {
            for (int b = 0; b < k; b++) {
                bool underA = base + a <= note && note <= base + a + 4;
                bool underB = base + b <= note && note <= base + b + 4;
                if (!underA && !underB) {
                    best[a][b] = inf;
                    continue;
                }
                int moveOne = min(rowMin[a], colMin[b]) + 1;
                best[a][b] = min(min(best[a][b], moveOne), allMin + 2);
            }
        }
    }
    int fewest = inf;
    for (int a = 0; a < k; a++) {
        for (int b = 0; b < k; b++) { fewest = min(fewest, best[a][b]); }
    }
    return fewest;
}

void test(const string& label, const vector<int>& notes, int wantMoves,
          const vector<int>& wantFingers) {
    Plan plan = planHands(notes);
    bool ok = plan.moves == wantMoves && plan.fingers == wantFingers;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testDistance(const string& label, const vector<int>& notes, int want) {
    cout << label << (minMoveDistance(notes) == want ? " PASS\n" : " FAIL\n");
}

void testTwoHands(const string& label, const vector<int>& notes, int want) {
    cout << label << (minMovesTwoHands(notes) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("drift", {1, 2, 3, 4, 5, 6}, 1, {0, 1, 2, 3, 4, 0});
    test("late-min", {5, 3, 1}, 0, {4, 2, 0});
    test("jump", {1, 9, 1}, 2, {0, 0, 0});
    test("spread-both-sides", {3, 0, 6}, 1, {3, 0, 0});
    testDistance("dist-nearest-edge", {0, 10, 6}, 6);
    testDistance("dist-high-first", {4, 8, 0}, 4);
    testDistance("dist-low-first", {4, 0, 8}, 4);
    testDistance("dist-back-and-forth", {1, 9, 1}, 8);
    testTwoHands("two-alternate", {1, 20, 1, 20}, 0);
    testTwoHands("two-three-spots", {1, 6, 11, 1, 6, 11}, 2);
    testTwoHands("two-one-group", {3, 5, 7}, 0);
    testTwoHands("two-empty", {}, 0);
}

// follow-up:
// hand spans k keys -> pass k, compare group range w/ k - 1 instead of 4
// (2) why group greedy fails -> 4 0 8: thumb at 0 then 8 shifts 8 keys, 4 is enough
// (2) as a DP -> best[thumb pos] per note, O(n * K^2); interval greedy = its O(n) form
// (3) why O(K^2) per note, not O(K^4) -> row / col / all mins come from the prev table
