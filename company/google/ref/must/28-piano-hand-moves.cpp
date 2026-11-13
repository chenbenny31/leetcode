// Play notes in order with one hand. Split the notes into consecutive groups so that in
// each group max - min <= 4 (five fingers cover five neighboring keys); every new group
// after the first is one hand move. In a group, a note's finger is note - group min
// (0 = thumb, 4 = little finger). Return the fewest moves and each note's finger.
//
// Clarify:
// - notes are key numbers (piano: 88 keys), in playing order; repeats allowed
// - "max - min <= 4" over the whole group, not distance to the group's first note
// - the first hand placement is free; each later group is one move, whatever the
//   distance (distance cost is a follow-up)
// - finger = note - group min, so each group's lowest note gets the thumb
// - empty -> 0 moves, no fingers
// Idea: greedy: extend the group while max - min stays <= 4, else start a new one.
//   Exchange argument: the greedy first group ends no earlier than any plan's first
//   group, and what is left is a suffix that plan must cover too. O(n).
// Pitfalls:
// - test the range with the new note included (newHigh - newLow), then commit low /
//   high; comparing each note with the first note only lets 3, 0, 6 into one group
// - set fingers only after the group closes: a lower note later shifts all of them
// - moves = groups - 1: no move for the first group, none for empty input
// Tests to write: drift up one key per note; lowest note comes last; jump away and
//   back; repeated note; notes on both sides of the first; empty.

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

// Greedy: extend the current group while max - min stays <= 4, else start a new group.
// Taking as many notes as possible never pushes later groups later, so the group count is
// minimal. Fingers are set once a group is closed, since its min may come late.
// T: O(n), S: O(n) for the output.
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
        for (int idx = groupStart; idx < groupEnd; idx++) {
            plan.fingers[idx] = notes[idx] - low;
        }
        if (groupStart > 0) { plan.moves++; }
        groupStart = groupEnd;
    }
    return plan;
}

// Follow-up: a move costs the number of keys the hand shifts (first placement free);
// return the least total shift. Thumb at p covers p..p+4, so note x needs p in
// [x - 4, x]. Lazy greedy: [lo, hi] = thumb positions that reach the least cost so
// far. If the next note's range overlaps it, intersect; else shift to the NEAREST end
// of that range and pay the gap. Shifting farther never pays back: a later saving is
// at most the extra keys moved now. T: O(n), S: O(1).
// Pitfalls: keep an interval, not one position (any fixed choice for the first note
//   fails 4 8 0 or 4 0 8); pay the gap to the nearest end only.
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

// Follow-up: two hands, five keys each; a note may go to either hand; a move = one
// hand jumping (any distance); both first placements are free. Return the fewest
// moves. Assumes few keys K (piano: 88); hands may overlap or cross.
// DP over both thumb positions: best[a][b] = fewest moves so far with the thumbs at a
// and b, inf if the current note is under neither. Between notes each hand stays
// (free) or jumps anywhere (1), so new[a][b] = min(best[a][b], rowMin[a] + 1,
// colMin[b] + 1, allMin + 2) for states covering the note. T: O(n * K^2), S: O(K^2).
// Pitfalls: take rowMin / colMin / allMin from the previous note's table before
//   overwriting it; reset states that miss the note to inf; empty input -> 0.
int minMovesTwoHands(const vector<int>& notes) {
    int n = notes.size();
    if (n == 0) { return 0; }
    int low = *min_element(notes.begin(), notes.end());
    int high = *max_element(notes.begin(), notes.end());
    // thumb positions low - 4 .. high, stored as index position - base
    int base = low - 4;
    int k = high - base + 1;
    // more moves than any plan needs: marks a state that cannot play the notes so far
    int inf = n + 1;
    // before the first note every placement is free
    vector<vector<int>> best(k, vector<int>(k, 0));
    for (int note : notes) {
        // fewest moves with thumb A at a / thumb B at b / anywhere, before this note
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
    test("same", {7, 7}, 0, {0, 0});
    test("spread-both-sides", {3, 0, 6}, 1, {3, 0, 0});
    test("empty", {}, 0, {});
    testDistance("dist-nearest-edge", {0, 10, 6}, 6);
    testDistance("dist-high-first", {4, 8, 0}, 4);
    testDistance("dist-low-first", {4, 0, 8}, 4);
    testDistance("dist-back-and-forth", {1, 9, 1}, 8);
    testDistance("dist-empty", {}, 0);
    testTwoHands("two-alternate", {1, 20, 1, 20}, 0);
    testTwoHands("two-three-spots", {1, 6, 11, 1, 6, 11}, 2);
    testTwoHands("two-one-group", {3, 5, 7}, 0);
    testTwoHands("two-empty", {}, 0);
}

// follow-up: a move costs the distance shifted -> the group greedy is no longer
//   optimal (4 0 8: thumb 0 then 8 shifts 8 keys, 4 is enough); minMoveDistance above
//   (a DP over (note, thumb position) also works, O(n * K^2))
// follow-up: two hands -> minMovesTwoHands above
// follow-up: the hand spans k keys -> pass k and replace the 4:
//     if (newHigh - newLow > k - 1) { break; }
