// start is "0000"
// if "0000" is dead-end -> return -1
// target be dead-end -> un-reachable -> return -1
// wheels wrap (9 -> 0, 0 -> 9)
// return the move count

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

// BFS over 10^4 states, by level, each state has 8 neighbors
// T: O(10^4 * 8), S: O(10^4)
int openLock(const vector<string>& deadends, const string& target) {
    unordered_set<string> deads(deadends.begin(), deadends.end());
    if (deads.count("0000")) { return -1; }
    unordered_set<string> seen{"0000"};
    queue<string> q;
    q.push("0000");

    int steps = 0;
    while (!q.empty()) {
        int sz = q.size();
        for (int i = 0; i < sz; i++) {
            string cur = q.front();
            q.pop();

            if (cur == target) { return steps; }
            for (int wheel = 0; wheel < 4; wheel++) {
                for (int delta : {+1, -1}) {
                    int digit = cur[wheel] - '0' + delta;
                    if (digit > 9) { digit = 0; }
                    if (digit < 0) { digit = 9; }
                    string nxt = cur;
                    nxt[wheel] = '0' + digit;
                    if (deads.count(nxt)) { continue; }
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

