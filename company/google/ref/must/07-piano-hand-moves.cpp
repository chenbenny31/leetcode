// Play notes in order with one hand. Split the notes into consecutive groups so that in
// each group max - min <= 4 (five fingers cover five neighboring keys); every new group
// after the first is one hand move. In a group, a note's finger is note - group min
// (0 = thumb, 4 = little finger). Return the fewest moves and each note's finger.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::min;
using std::max;

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

void test(const string& label, const vector<int>& notes, int wantMoves,
          const vector<int>& wantFingers) {
    Plan plan = planHands(notes);
    bool ok = plan.moves == wantMoves && plan.fingers == wantFingers;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("drift", {1, 2, 3, 4, 5, 6}, 1, {0, 1, 2, 3, 4, 0});
    test("late-min", {5, 3, 1}, 0, {4, 2, 0});
    test("jump", {1, 9, 1}, 2, {0, 0, 0});
    test("same", {7, 7}, 0, {0, 0});
    test("empty", {}, 0, {});
}

// follow-up: cost of a move = distance between hand positions -> greedy no longer
//   optimal; DP over (note index, hand position)
// follow-up: two hands -> each note goes to either hand; DP over both hand positions
