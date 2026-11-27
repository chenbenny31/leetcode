// 03 · Tower of Hanoi — classic recursion ("a Hanoi-like problem" was reported)
// n disks on peg 0, largest at the bottom; move all of them to peg 2
// one disk per move, never a larger disk on a smaller one; return the moves (from, to)
//
// clarify:
// return -> the moves, or only how many? -> moves here; the count is 2^n - 1
// n -> small (<= 20): the output alone is 2^n - 1 moves
// peg ids -> 0, 1, 2; start 0, target 2
// n == 0 -> no moves

#include <iostream>
#include <vector>
#include <string>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::pair;

// ==================== (1) ====================
// return the moves that take n disks from peg 0 to peg 2

// core idea: park the top n - 1 on the spare peg, move the largest, bring n - 1 back
// T: O(2^n), S: O(n) call stack; moves(n) = 2 * moves(n - 1) + 1 = 2^n - 1
void moveDisks(int n, int from, int to, int via, vector<pair<int, int>>& moves) {
    if (n == 0) { return; }
    moveDisks(n - 1, from, via, to, moves);
    moves.push_back({from, to});
    moveDisks(n - 1, via, to, from, moves);
}

vector<pair<int, int>> hanoi(int n) {
    vector<pair<int, int>> moves;
    moveDisks(n, 0, 2, 1, moves);
    return moves;
}

// ==================== (2) ====================
// the same moves without recursion

// one pending call of moveDisks
struct Frame {
    int n;
    int from;
    int to;
    int via;
};

// core idea: a stack of pending calls; push the 3 steps in reverse so they pop in order
// T: O(2^n), S: O(n)
vector<pair<int, int>> hanoiIterative(int n) {
    vector<pair<int, int>> moves;
    vector<Frame> pending; // calls not run yet, the next one on top
    if (n > 0) { pending.push_back({n, 0, 2, 1}); }
    while (!pending.empty()) {
        Frame cur = pending.back();
        pending.pop_back();
        if (cur.n == 1) {
            moves.push_back({cur.from, cur.to});
            continue;
        }
        pending.push_back({cur.n - 1, cur.via, cur.to, cur.from});
        pending.push_back({1, cur.from, cur.to, cur.via});
        pending.push_back({cur.n - 1, cur.from, cur.via, cur.to});
    }
    return moves;
}

// ==================== tests ====================
void test(const string& label, int n, const vector<pair<int, int>>& want) {
    bool ok = hanoi(n) == want && hanoiIterative(n) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("no-disks", 0, {});
    test("one-disk", 1, {{0, 2}});
    test("two-disks", 2, {{0, 1}, {0, 2}, {1, 2}});
    test("three-disks", 3, {{0, 2}, {0, 1}, {2, 1}, {0, 2}, {1, 0}, {1, 2}, {0, 2}});
}

// follow-up:
// 1. only the num of moves -> 2^n - 1, from T(n) = 2 * T(n - 1) + 1
// 2. which disk moves at step k (1-based) -> lowest set bit of k (bit 0 = smallest disk)
// 3. moves only between adjacent pegs -> T(n) = 3 * T(n - 1) + 2 = 3^n - 1
// 4. 4 pegs (Frame-Stewart) -> top k via 4 pegs, the rest via 3, top k again; try every k
// 5. start from any legal state -> largest first: if off target, park the rest on spare
