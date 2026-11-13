// LC 636 Exclusive Time of Functions (Medium) — simulate the call stack
// logs "id:start:t" / "id:end:t" in time order, 1 CPU; end at t covers whole unit t
// return each fn's time w/o the calls it makes
//
// clarify:
// logs sorted, well formed -> assume yes
// recursion (same id nested) -> ok, stack handles it
// end inclusive -> + 1 on end
// ids -> 0..n-1

#include <iostream>
#include <vector>
#include <string>
#include <stack>

using std::cout;
using std::string;
using std::vector;
using std::stack;

// core idea: stack top = fn running now; each log credits slice since prevTime to top
// T: O(total log len), S: O(n + depth)
vector<int> exclusiveTime(int n, const vector<string>& logs) {
    vector<int> result(n, 0);
    // ids of fns started, not ended; top is running
    stack<int> running;
    // start of the slice not credited yet
    int prevTime = 0;
    for (const string& log : logs) {
        int pos = 0;
        int id = 0;
        while (log[pos] != ':') {
            id = id * 10 + (log[pos] - '0');
            pos++;
        }
        pos++;
        bool isStart = log[pos] == 's';
        while (log[pos] != ':') { pos++; }
        pos++;
        int time = 0;
        while (pos < (int)log.size()) {
            time = time * 10 + (log[pos] - '0');
            pos++;
        }
        if (isStart) {
            // caller ran until now, then paused
            if (!running.empty()) { result[running.top()] += time - prevTime; }
            running.push(id);
            prevTime = time;
        } else {
            // + 1: end timestamp = end of unit `time`
            result[running.top()] += time - prevTime + 1;
            running.pop();
            prevTime = time + 1;
        }
    }
    return result;
}

void test(const string& label, int n, const vector<string>& logs,
          const vector<int>& want) {
    cout << label << (exclusiveTime(n, logs) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("nested-call", 2, {"0:start:10", "1:start:12", "1:end:20", "0:end:25"}, {7, 9});
    test("recursion", 1, {"0:start:0", "0:start:1", "0:end:2", "0:end:3"}, {4});
    test("two-children", 3,
         {"0:start:0", "1:start:1", "1:end:1", "2:start:2", "2:end:3", "0:end:4"},
         {2, 1, 2});
    return 0;
}

// follow-up:
// eval nested call add(1, sub(2, 3)) -> frame per open call (name, args); ')' pops it
// recursion -> explicit stack -> push (node, next child idx), loop until empty
// inclusive time (w/ callees) -> stack of (id, start); on end add t - start + 1
// unmatched start / end -> end id != top, end on empty stack, or non-empty at finish
// multi-thread logs "thread:id:start:t" -> one stack + prevTime per thread (map)
