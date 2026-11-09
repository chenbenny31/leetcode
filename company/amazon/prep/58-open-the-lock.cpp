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

