// LC 721 Accounts Merge (Medium) — union-find over accounts sharing an email
// accounts[i] = [name, emails...]; two accounts are one person if they share an email.
// Return each person as [name, sorted distinct emails...], people in any order.
// Clarify: same name without a shared email stays apart; an email listed twice in one
//   account; every account has an email; output order; sizes (~1e3 accounts x 10).

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::sort;

// returns the root account of node, compressing the path
int findRoot(vector<int>& parent, int node) {
    if (parent[node] != node) { parent[node] = findRoot(parent, parent[node]); }
    return parent[node];
}

// Core idea: the first account listing an email owns it; a later account listing it is
// united with the owner. Then each distinct email goes to its owner's root.
// T: O(E log E) for sorting, E = total emails, S: O(E).
vector<vector<string>> accountsMerge(const vector<vector<string>>& accounts) {
    int n = accounts.size();
    // parent[i] = parent account of i in the union-find
    vector<int> parent(n);
    for (int i = 0; i < n; i++) { parent[i] = i; }
    // email -> first account that listed it
    unordered_map<string, int> owner;
    for (int i = 0; i < n; i++) {
        for (int j = 1; j < (int)accounts[i].size(); j++) {
            const string& email = accounts[i][j];
            if (!owner.count(email)) {
                owner[email] = i;
                continue;
            }
            // link the roots: account i may already hang under another root
            int rootCur = findRoot(parent, i);
            int rootOwner = findRoot(parent, owner[email]);
            parent[rootCur] = rootOwner;
        }
    }
    // root account -> its distinct emails
    unordered_map<int, vector<string>> groups;
    for (const auto& [email, id] : owner) {
        groups[findRoot(parent, id)].push_back(email);
    }
    vector<vector<string>> result;
    for (auto& [root, emails] : groups) {
        sort(emails.begin(), emails.end());
        vector<string> merged = {accounts[root][0]};
        for (const string& email : emails) { merged.push_back(email); }
        result.push_back(merged);
    }
    return result;
}

void test(const string& label, const vector<vector<string>>& accounts,
          vector<vector<string>> want) {
    vector<vector<string>> got = accountsMerge(accounts);
    sort(got.begin(), got.end());
    sort(want.begin(), want.end());
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("chain", {{"Ann", "a@x", "b@x"}, {"Ann", "c@x"}, {"Ann", "b@x", "c@x"}},
         {{"Ann", "a@x", "b@x", "c@x"}});
    test("same-name-apart", {{"Bob", "b1@x"}, {"Bob", "b2@x"}},
         {{"Bob", "b1@x"}, {"Bob", "b2@x"}});
    test("dup-email", {{"Cy", "c@x", "c@x"}}, {{"Cy", "c@x"}});
    test("two-people", {{"Ann", "a@x"}, {"Bob", "b@x"}, {"Ann", "c@x", "a@x"}},
         {{"Ann", "a@x", "c@x"}, {"Bob", "b@x"}});
    return 0;
}

// follow-up (ideas only):
// - DFS instead of union-find -> number the distinct emails, link each email of an
//   account to that account's first email (a star, O(E) edges), one DFS per unseen email
// - union by size -> keep size[root], hang the smaller root under the larger; with path
//   compression each operation is ~O(alpha(n))
// - earliest moment everyone is friends (LC 1101) -> sort the logs by time, union a and
//   b; each union of two different roots drops the count, answer when it reaches 1
// - number of provinces (LC 547) -> union i and j when isConnected[i][j] == 1, count the
//   nodes that are still their own root
// - point clusters within distance d (Google) -> union every pair with squared distance
//   <= d * d in long long; a grid of d-sized buckets avoids checking all pairs
// - make the network connected (LC 1319) -> fewer than n - 1 cables is -1, otherwise
//   the answer is components - 1
// - dynamic friendship (Google) -> adds only: union-find online; removals known up
//   front: replay in reverse so each removal is a union; both: rollback union-find
// - redundant connection (LC 684) -> union the edges in order; the answer is the first
//   edge whose endpoints already share a root
// - graph valid tree (LC 261) -> exactly n - 1 edges and no edge inside one component
// - number of connected components (LC 323) -> union every edge, count the roots
