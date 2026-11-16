// LC 2337 Move Pieces to Obtain a String (Google, interview report).
// (1) same-len strings of 'L', 'R', '_'; L moves left into '_', R right, no passing
// (2) '|' is a wall: never moves, cannot be crossed
// return whether start can become target
//
// clarify:
// same len -> yes
// moves -> any number of one-cell moves
// n up to 1e5 -> no simulation
// no pieces at all -> true

#include <iostream>
#include <string>

using std::cout;
using std::string;

// (1) no walls
// core idea: no passing -> same letters in same order w/o '_'; two pointers match pieces
// each L ends at or left of its start, each R at or right
// T: O(n), S: O(1)
bool canChange(const string& start, const string& target) {
    int n = start.size();
    // from = idx in start, to = idx in target
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

// (2) walls
// core idea: same walk; '|' is a piece that must stay at the same idx (never skipped)
// order kept -> every piece stays between the same two walls
// T: O(n), S: O(1)
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

    testWalls("inside", "R_|_L", "_R|L_", true);
    testWalls("cross", "_|L", "L|_", false);
    testWalls("wall-moved", "|_", "_|", false);
}

// follow-up:
// LC 777 ("XL" -> "LX", "RX" -> "XR") -> same check w/ 'X' as the empty cell
// fewest moves -> sum |to - from| over matched pieces (long long); -1 instead of false
// circular board -> try each rotation of the piece list that lines up; O(n * k)
// equal cnts of L and R enough? -> no: "R_L" -> "L_R" needs pieces to pass
