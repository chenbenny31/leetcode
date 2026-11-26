// same name, no shared mail -> stay apart
// email twice in one account -> dedup
// assume every account has an email
// order -> any people, sorted email
// size up to 1e3 accounts * 10 emails

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::sort;
using std::swap;

int find(vector<int>& parent, int x) {
    while (parent[x] != x) {
        parent[x] = parent[parent[x]];
        x = parent[x];
    }
    return x;
}

bool unite(vector<int>& parent, vector<int>& sz, int a, int b) {
    int rootA = find(parent, a);
    int rootB = find(parent, b);
    if (rootA == rootB) { return false; }
    if (sz[rootA] < sz[rootB]) { swap(rootA, rootB); }
    parent[rootB] = rootA;
    sz[rootA] += sz[rootB];
    return true;
}

// first account with email owns it, later account unite with owner
// each distinct email goes to owner's root, T: O(eloge), S: O(e), e=emails, sort dominates
vector<vector<string>> accountsMerge(const vector<vector<string>>& accounts) {
    int n = accounts.size();
    vector<int> parent(n);
    for (int i = 0; i < n; i++) { parent[i] = i; }
    vector<int> sz(n, 1);
    unordered_map<string, int> owner; // email -> owner account
    for (int i = 0; i < n; i++) {
        for (int j = 1; j < (int)accounts[i].size(); j++) {
            const string& email = accounts[i][j];
            if (!owner.count(email)) { // email as key for O(1) lookup
                owner[email] = i;
                continue;
            }

            unite(parent, sz, i, owner[email]);
        }
    }

    unordered_map<int, vector<string>> groups; // root-account -> distinct emails
    for (const auto& [email, id] : owner) {
        groups[find(parent, id)].push_back(email);
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

// chain, same-name-apart, dup-email, two-people

// DFS instead of union-find -> link each email to account's first email
