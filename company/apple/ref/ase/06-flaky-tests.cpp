// Flaky test detection (ASE test automation: "tests passed" vs "the release is good").
// runs[i] = {test, commit, result} with result "pass" or "fail". A test is flaky if, on
// the same commit, it has at least one pass and at least one fail. Return the flaky
// test names sorted, each once.

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::map;
using std::set;
using std::pair;

// Group runs by (test, commit) and remember which outcomes were seen.
// T: O(n log n), S: O(n).
vector<string> flakyTests(const vector<vector<string>>& runs) {
    // seen[{test, commit}] = bit 1 if it passed there, bit 2 if it failed there
    map<pair<string, string>, int> seen;
    for (const vector<string>& run : runs) {
        if (run[2] == "pass") {
            seen[{run[0], run[1]}] |= 1;
        } else {
            seen[{run[0], run[1]}] |= 2;
        }
    }
    // a set keeps names sorted and unique (one test can be flaky on many commits)
    set<string> flaky;
    for (const auto& [key, outcomes] : seen) {
        if (outcomes == 3) {
            flaky.insert(key.first);
        }
    }
    return vector<string>(flaky.begin(), flaky.end());
}

void test(const string& label, const vector<vector<string>>& runs,
          const vector<string>& want) {
    cout << label << (flakyTests(runs) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("same-commit-mixed", {{"boot", "c1", "pass"}, {"boot", "c1", "fail"},
                               {"net", "c1", "pass"}}, {"boot"});
    test("different-commits", {{"boot", "c1", "pass"}, {"boot", "c2", "fail"}}, {});
    test("flaky-twice-listed-once", {{"io", "c1", "fail"}, {"io", "c1", "pass"},
                                     {"io", "c2", "pass"}, {"io", "c2", "fail"}}, {"io"});
}

// follow-up: rank by flakiness -> per test, count commits with mixed results / commits
//   run; sort by that rate
// follow-up: fail -> pass on a later commit is not flaky but a fix; fail on c2 after pass
//   on c1 is a regression: bisect it (see last-good-version)
// follow-up: quarantine -> a flaky test's failures don't block the release gate; the
//   gate = all non-quarantined tests pass on the candidate commit
