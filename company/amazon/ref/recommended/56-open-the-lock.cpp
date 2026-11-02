// LC 752 Open the Lock: 4 wheels "0000", one move turns one wheel +1 or -1 (wrapping).
// Never step on a deadend. Return the fewest moves to reach target, or -1.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_set>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::unordered_set;

// BFS over the 10^4 states level by level; each state has 8 neighbors.
// T: O(10^4 * 8), S: O(10^4).
int openLock(const vector<string>& deadends, const string& target) {
    unordered_set<string> dead(deadends.begin(), deadends.end());
    if (dead.count("0000")) { return -1; }
    unordered_set<string> seen{"0000"};
    queue<string> q;
    q.push("0000");
    int steps = 0;
    while (!q.empty()) {
        int size = q.size();
        for (int k = 0; k < size; k++) {
            string cur = q.front();
            q.pop();
            if (cur == target) { return steps; }
            for (int i = 0; i < 4; i++) {
                // +1 and -1 (as +9 mod 10)
                for (int delta : {1, 9}) {
                    string nxt = cur;
                    nxt[i] = '0' + (cur[i] - '0' + delta) % 10;
                    if (dead.count(nxt)) { continue; }
                    if (seen.insert(nxt).second) { q.push(nxt); }
                }
            }
        }
        steps++;
    }
    return -1;
}

void test(const string& label, const vector<string>& deadends, const string& target,
          int want) {
    cout << label << (openLock(deadends, target) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("wrap", {}, "0009", 1);
    test("detour", {"0001"}, "0002", 4);
    test("start-dead", {"0000"}, "0001", -1);
    test("boxed-in", {"0001", "0009", "0010", "0090", "0100", "0900", "1000", "9000"},
         "0002", -1);
}
