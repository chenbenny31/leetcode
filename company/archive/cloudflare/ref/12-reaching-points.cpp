// LC 780 Reaching Points (Hard). Pattern: work backward, subtraction becomes modulo.
// From (x, y) one move gives (x, x + y) or (x + y, y). Can (sx, sy) reach (tx, ty)?
// All four values are positive integers up to 1e9.

#include <iostream>
#include <string>

using std::cout;
using std::string;

// Backward, the move is unique: the larger coordinate came from adding the smaller one.
// Undoing it one subtraction at a time is too slow (1, 1e9), so subtract in bulk with %
// (like gcd). Once one side is down to its start, only the other side may still shrink:
// it must be start + a multiple of the fixed side.
// T: O(log(max(tx, ty))), S: O(1).
bool reachingPoints(int sx, int sy, int tx, int ty) {
    while (tx >= sx && ty >= sy) {
        // equal sides cannot come from a move (both > 0), so this must be the start
        if (tx == ty) { break; }

        if (tx > ty) {
            // tx was built by adding ty; ty fixed at its start -> check the multiple
            if (ty > sy) { tx %= ty; }
            else { return (tx - sx) % ty == 0; }
        } else {
            if (tx > sx) { ty %= tx; }
            else { return (ty - sy) % tx == 0; }
        }
    }
    return tx == sx && ty == sy;
}

void test(const string& label, int sx, int sy, int tx, int ty, bool want) {
    cout << label << (reachingPoints(sx, sy, tx, ty) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("start", 1, 1, 1, 1, true);
    test("reach", 1, 1, 3, 5, true);
    test("unreach", 1, 1, 2, 2, false);
    test("big-jump", 1, 1, 1, 1000000000, true);
    test("passed-start", 3, 7, 3, 4, false);
}

// follow-up: why not forward BFS -> 2^steps states; backward has exactly one parent.
// follow-up: why % and not one step -> (1, 1e9) needs 1e9 subtractions, % does it in one.
