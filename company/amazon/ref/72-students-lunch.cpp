// LC 1700: students (queue) and sandwiches (stack) are 0/1 preferences. The front student
// takes the top sandwich if it matches, otherwise goes to the back; stop when nobody in
// line wants the top sandwich. Return how many students can't eat.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Order in line doesn't matter: the top sandwich is taken as long as someone wants it.
// Count preferences and serve sandwiches in order. T: O(n), S: O(1).
int countStudents(const vector<int>& students, const vector<int>& sandwiches) {
    // want[t] = students who want sandwich type t
    int want[2] = {0, 0};
    for (int s : students) { want[s]++; }
    int n = sandwiches.size();
    for (int i = 0; i < n; i++) {
        if (want[sandwiches[i]] == 0) { return n - i; }
        want[sandwiches[i]]--;
    }
    return 0;
}

void test(const string& label, const vector<int>& students, const vector<int>& sandwiches,
          int want) {
    bool ok = countStudents(students, sandwiches) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("all-eat", {1, 0}, {0, 1}, 0);
    test("stuck", {1, 1}, {0, 1}, 2);
    test("partial", {0, 1, 1}, {1, 0, 0}, 1);
}
