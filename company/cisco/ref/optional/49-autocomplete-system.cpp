// 49 · Design Search Autocomplete System (Cisco LC tag, Hard) — LC 642, trie
// AutocompleteSystem(sentences, times); input(c): c == '#' ends the cur sentence (count
// it once more, return {}); else append c and return the top 3 past sentences starting
// with the typed prefix: higher count first, then ASCII order
//
// clarify:
// chars -> lowercase letters and ' '
// a new sentence typed with '#' -> becomes history with count 1 (or + 1 if known)
// fewer than 3 matches -> return all of them
// no match for the prefix -> {} until '#'

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <unordered_map>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::map;
using std::set;
using std::unordered_map;
using std::sort;

// core idea: trie whose node keeps every sentence passing through it; typing walks one
// node per char; a query sorts that node's sentences by (count desc, text asc)
// T: input O(k log k), k = sentences under the node; '#' O(L log n); S: O(total L^2)
class AutocompleteSystem {
    struct Node {
        // char -> child idx in nodes
        map<char, int> next;
        // sentences in this subtree
        set<string> passing;
    };
    // nodes[0] = root; children by idx, so push_back never leaves a dangling pointer
    vector<Node> nodes;
    // sentence -> times typed
    unordered_map<string, int> count;
    // what has been typed since the last '#'
    string typed;
    // node of `typed`, -1 once no sentence matches
    int cur = 0;

    void insert(const string& s) {
        int node = 0;
        nodes[node].passing.insert(s);
        for (char c : s) {
            auto it = nodes[node].next.find(c);
            int child = 0;
            if (it == nodes[node].next.end()) {
                child = nodes.size();
                nodes[node].next[c] = child;
                nodes.push_back(Node());
            } else {
                child = it->second;
            }
            node = child;
            nodes[node].passing.insert(s);
        }
    }

public:
    AutocompleteSystem(const vector<string>& sentences, const vector<int>& times) {
        nodes.push_back(Node());
        int n = sentences.size();
        for (int i = 0; i < n; i++) {
            count[sentences[i]] += times[i];
            insert(sentences[i]);
        }
    }

    vector<string> input(char c) {
        if (c == '#') {
            count[typed]++;
            insert(typed);
            typed.clear();
            cur = 0;
            return {};
        }
        typed += c;
        if (cur != -1) {
            auto it = nodes[cur].next.find(c);
            cur = it == nodes[cur].next.end() ? -1 : it->second;
        }
        if (cur == -1) { return {}; }
        vector<string> found(nodes[cur].passing.begin(), nodes[cur].passing.end());
        sort(found.begin(), found.end(), [this](const string& a, const string& b) {
            if (count[a] != count[b]) { return count[a] > count[b]; }
            return a < b;
        });
        if (found.size() > 3) { found.resize(3); }
        return found;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    AutocompleteSystem ac({"i love you", "island", "iroman", "i love leetcode"},
                          {5, 3, 2, 2});
    vector<string> afterI = {"i love you", "island", "i love leetcode"};
    check(ac.input('i') == afterI, "top-three");
    ac.input(' ');
    check(ac.input('a').empty(), "no-match");
    ac.input('#');

    ac.input('i');
    vector<string> withNew = {"i love you", "i love leetcode", "i a"};
    check(ac.input(' ') == withNew, "new-sentence-ranked");
    check(ac.input('a') == vector<string>({"i a"}), "new-sentence-found");
    return 0;
}

// follow-up:
// 1. faster queries -> each node caches its own top 3; '#' updates the caches on the path
// 2. huge history -> keep only the top k per node
// 3. ranking by recency too -> score = count decayed by age; same top-k per node
// 4. typo tolerance -> search nodes within edit distance 1 (Levenshtein automaton)
