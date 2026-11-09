// LC 721 Accounts Merge (Medium). Pattern: union-find over shared keys.
// accounts[i] = [name, emails...]. Two accounts belong to the same person if they share
// an email. Merge them into [name, sorted distinct emails...], in any order.

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

// Union-find over account indices, joined through shared emails.
// T: O(E log E) for sorting, E = total emails; S: O(E).
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
}

// follow-up: DFS instead of union-find -> graph[email] = emails in the same account (link
//   the first email to the rest), DFS each component, sort; O(E log E)
// follow-up: union by size -> with path compression ~O(alpha(n)) per op
