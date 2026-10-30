// Return the indices of comments that contain at least one abusive word. A word in a
// comment (split on spaces) matches an abusive word if they have the same length and each
// character matches: a letter matches itself (case-insensitive), and a character listed
// in the transform map matches any of its mapped letters. Other characters never match.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <cctype>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::tolower;
using std::isalpha;

// split on spaces with a plain character loop
vector<string> words(const string& s) {
    vector<string> result;
    string cur;
    for (char c : s) {
        if (c == ' ') {
            if (!cur.empty()) { result.push_back(cur); }
            cur.clear();
        } else {
            cur += c;
        }
    }
    if (!cur.empty()) { result.push_back(cur); }
    return result;
}

// can character c stand for letter w?
bool charMatches(char c, char w, const unordered_map<char, string>& transform) {
    if (isalpha(c) && tolower(c) == w) { return true; }
    char key = tolower(c);
    if (!transform.count(key)) { return false; }
    return transform.at(key).find(w) != string::npos;
}

// straightforward: compare every comment word with every abusive word of equal length.
// T: O(total comment chars * a) for a abusive words, S: O(map).
vector<int> detect(const vector<string>& comments, const vector<string>& abuse,
                   const unordered_map<char, string>& transform) {
    vector<int> result;
    int n = comments.size();
    for (int i = 0; i < n; i++) {
        bool bad = false;
        for (const string& w : words(comments[i])) {
            for (const string& a : abuse) {
                if (a.size() != w.size()) { continue; }
                bool same = true;
                for (int j = 0; j < (int)w.size() && same; j++) {
                    same = charMatches(w[j], a[j], transform);
                }
                if (same) { bad = true; }
            }
        }
        if (bad) { result.push_back(i); }
    }
    return result;
}

struct TrieNode {
    TrieNode* next[26] = {};
    bool end = false;
};

// does w[i..] spell a whole abusive word from node?
bool dfs(const string& w, int i, TrieNode* node,
         const unordered_map<char, string>& transform) {
    if (i == (int)w.size()) { return node->end; }
    for (int c = 0; c < 26; c++) {
        if (node->next[c] && charMatches(w[i], 'a' + c, transform) &&
            dfs(w, i + 1, node->next[c], transform)) {
            return true;
        }
    }
    return false;
}

// improved: abusive words share prefixes in a trie; each comment word walks it once,
// branching only where the map allows several letters. T: O(total chars * branching).
vector<int> detectTrie(const vector<string>& comments, const vector<string>& abuse,
                       const unordered_map<char, string>& transform) {
    TrieNode root;
    vector<TrieNode*> owned;
    for (const string& a : abuse) {
        TrieNode* node = &root;
        for (char ch : a) {
            int c = ch - 'a';
            if (!node->next[c]) {
                node->next[c] = new TrieNode();
                owned.push_back(node->next[c]);
            }
            node = node->next[c];
        }
        node->end = true;
    }
    vector<int> result;
    int n = comments.size();
    for (int i = 0; i < n; i++) {
        for (const string& w : words(comments[i])) {
            if (dfs(w, 0, &root, transform)) {
                result.push_back(i);
                break;
            }
        }
    }
    for (TrieNode* node : owned) { delete node; }
    return result;
}

void test(const string& label, const vector<string>& comments,
          const vector<string>& abuse, const unordered_map<char, string>& transform,
          const vector<int>& want) {
    bool ok = detect(comments, abuse, transform) == want &&
              detectTrie(comments, abuse, transform) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    unordered_map<char, string> t = {{'@', "a"}, {'x', "uc"}, {'3', "e"}};
    test("symbol", {"you @ss", "nice"}, {"ass"}, t, {0});
    test("multi-map", {"ok", "fxxk3r now"}, {"fucker"}, t, {1});
    test("case", {"ASS here"}, {"ass"}, t, {0});
    test("whole-word", {"asss"}, {"ass"}, t, {});
    test("unmapped", {"a$s"}, {"ass"}, t, {});
}
