// LC 2337 Move Pieces to Obtain a String (Google, interview report).
// start and target have the same length and hold 'L', 'R' and '_' (empty). An 'L' may
// move left into an empty cell, an 'R' right; pieces never pass each other. Return
// whether start can become target. Follow-up: '|' is a wall that never moves and cannot
// be crossed by any piece.
//
// Clarify:
// - same length (stated); chars only 'L', 'R', '_' ('|' in the follow-up)
// - any number of moves, one cell each -> yes; fewest moves is a follow-up
// - n up to ~1e5 -> O(n) two pointers; simulating moves is O(n^2)
// - no pieces at all -> true iff target has none either
// Idea: pieces never pass each other, so after dropping the '_' both strings must list
//   the same letters in the same order, and the k-th piece of start becomes the k-th of
//   target. Each L may only go left (to <= from), each R only right (to >= from); then
//   moving the Ls left to right and the Rs right to left never blocks, so it is enough.
// Pitfalls:
// - test from < n before reading start[from] (same for to) while skipping '_'
// - when one side runs out of pieces, the other must run out too (extra piece -> false)
// - the direction signs: L fails when to > from, R fails when to < from
// - equal counts of L and R are not enough: "R_L" -> "L_R" needs the pieces to pass
// Tests to write: an R and an L move toward each other; L would move right; pieces would
//   pass; extra piece in target; no pieces. Walls: moves between walls; L crosses a
//   wall; a wall moved.

#include <iostream>
#include <string>

using std::cout;
using std::string;

// two pointers over the pieces only: both strings must list the same letters in the same
// order, each 'L' may only end at or left of where it starts, each 'R' at or right.
// T: O(n), S: O(1).
bool canChange(const string& start, const string& target) {
    int n = start.size();
    // from = position in start, to = position in target
    int from = 0;
    int to = 0;
    while (from < n || to < n) {
        while (from < n && start[from] == '_') { from++; }
        while (to < n && target[to] == '_') { to++; }
        if (from == n || to == n) { return from == n && to == n; }
        if (start[from] != target[to]) { return false; }
        if (start[from] == 'L' && to > from) { return false; }
        if (start[from] == 'R' && to < from) { return false; }
        from++;
        to++;
    }
    return true;
}

// follow-up walls: same walk, a '|' is a piece that must stay at the same index. Order
// is kept, so every piece stays between the same two walls and cannot cross one.
// Pitfall: never skip '|' along with '_'; a skipped wall lets an L cross it.
// T: O(n), S: O(1).
bool canChangeWalls(const string& start, const string& target) {
    int n = start.size();
    int from = 0;
    int to = 0;
    while (from < n || to < n) {
        while (from < n && start[from] == '_') { from++; }
        while (to < n && target[to] == '_') { to++; }
        if (from == n || to == n) { return from == n && to == n; }
        if (start[from] != target[to]) { return false; }
        if (start[from] == 'L' && to > from) { return false; }
        if (start[from] == 'R' && to < from) { return false; }
        if (start[from] == '|' && to != from) { return false; }
        from++;
        to++;
    }
    return true;
}

void test(const string& label, const string& start, const string& target, bool want) {
    cout << label << (canChange(start, target) == want ? " PASS\n" : " FAIL\n");
}

void testWalls(const string& label, const string& start, const string& target,
               bool want) {
    cout << label << (canChangeWalls(start, target) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("both-move", "R__L", "_RL_", true);
    test("wrong-way", "L_", "_L", false);
    test("no-pass", "R_L", "L_R", false);
    test("extra-piece", "L_", "LL", false);
    test("all-empty", "__", "__", true);

    testWalls("inside", "R_|_L", "_R|L_", true);
    testWalls("cross", "_|L", "L|_", false);
    testWalls("wall-moved", "|_", "_|", false);
}

// follow-up: LC 777 ("XL" -> "LX", "RX" -> "XR") -> same check with 'X' as the empty
//   cell: while (from < n && start[from] == 'X') { from++; } (same for to)
// follow-up: fewest moves (one cell per move) -> each matched piece travels |to - from|
//   and nothing more is needed: add moves += abs(to - from) before from++, to++ (long
//   long for n up to 1e5), return -1 instead of false
// follow-up: circular board -> cutting at an empty cell is NOT enough: a piece may move
//   across the cut ("R__" -> "__R" on a ring, cut at index 1). Pieces keep their cyclic
//   order, so the matching is one of the k rotations of the piece list: for each one
//   whose letters line up, unwrap each move (R forward, L backward) and check
//   neighboring pieces still do not cross. O(n * k).
