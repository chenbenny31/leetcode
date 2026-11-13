// LC 636 Exclusive Time of Functions (Medium) — simulate the call stack
// logs are "id:start:t" / "id:end:t" in time order on a single-threaded CPU; an end at t
// covers the whole unit t. Return each function's time excluding the calls it makes.
// Clarify: logs sorted and well formed; recursion (same id nested); end is inclusive;
//   ids are 0..n-1.

#include <iostream>
#include <vector>
#include <string>
#include <stack>

using std::cout;
using std::string;
using std::vector;
using std::stack;

// Core idea: the top of the stack is the function running now; every log closes the
// slice since prevTime and credits it to the top. A start pushes, an end pops and its
// slice includes the unit t itself. T: O(total log length), S: O(n + depth).
vector<int> exclusiveTime(int n, const vector<string>& logs) {
    vector<int> result(n, 0);
    // ids of the functions that started and have not ended; the top is running
    stack<int> running;
    // start of the time slice that is not credited yet
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
            // the caller ran until now and is paused
            if (!running.empty()) { result[running.top()] += time - prevTime; }
            running.push(id);
            prevTime = time;
        } else {
            // + 1: the end timestamp is the end of unit `time`
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

// follow-up (ideas only):
// - evaluate a nested call expression add(1, sub(2, 3)) -> one frame per open call
//   holding its name and the args parsed so far; ')' pops the frame, applies it and
//   passes the value to the new top as its next arg
// - recursion -> explicit stack in general (iterative DFS / inorder) -> push the state
//   you would have kept in locals (node, next child index), loop while the stack is not
//   empty
// - inclusive time (with callees) -> stack of (id, start time); on end add
//   t - start + 1; for recursion count only the outermost frame of an id
// - detect unmatched start / end -> an end whose id differs from the top or that finds an
//   empty stack is invalid; so is a non-empty stack after the last log
// - multi-threaded logs "thread:id:start:t" -> one stack and one prevTime per thread id
//   (map thread -> state), same rule inside each thread
