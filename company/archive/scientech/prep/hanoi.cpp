// return the moves, count is 2^n - 1
// n is small
// peg ids -> 0, 1, 2: 0 -> 2
// n == 0, no moves

#include <iostream>
#include <vector>
#include <string>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::pair;

// return the moves take n disks from peg 0 to peg 2

// park the top n-1 on spare peg, move the largest, bring n-1 back
// T: O(2^n), S: O(n) call stack
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

// without recursion

struct Frame {
    int n;
    int from;
    int to;
    int via;
};

// stack of pending calls, push 3 steps inreverse so pop in order, T: O(2^n), S: O(n)
vector<pair<int, int>> hanoiIterative(int n) {
    vector<pair<int, int>> moves;
    vector<Frame> pending; // calls not run yet, next one on top
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
