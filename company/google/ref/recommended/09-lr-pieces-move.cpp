// LC 2337 Move Pieces to Obtain a String (Google, interview report).
// start and target have the same length and hold 'L', 'R' and '_' (empty). An 'L' may
// move left into an empty cell, an 'R' right; pieces never pass each other. Return
// whether start can become target. Follow-up: '|' is a wall that never moves and cannot
// be crossed by any piece.

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

// follow-up: LC 777 ("XL" -> "LX", "RX" -> "XR") -> same check with 'X' as the empty cell
// follow-up: circular board -> cut the circle at an empty cell and run the linear check
