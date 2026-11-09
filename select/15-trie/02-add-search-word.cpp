// LC 211 Design Add and Search Words Data Structure (Medium). Pattern: trie + DFS.
// addWord(word); search(word) where '.' matches any one letter. Lowercase letters.

#include <iostream>
#include <string>

using std::cout;
using std::string;

struct TrieNode {
    // children[c] = node for letter 'a' + c, nullptr = none
    TrieNode* children[26] = {};
    // some added word ends here
    bool isEnd = false;

    ~TrieNode() {
        for (TrieNode* child : children) {
            delete child;
        }
    }
};

// Trie; search is a DFS where '.' tries every child.
// T: add O(L), search O(L) without dots, O(trie size) worst case; S: O(26 * letters).
class WordDictionary {
    // node for the empty prefix
    TrieNode root;

    // does word[i..] match some path below node
    bool dfs(TrieNode* node, const string& word, int i) {
        if (i == (int)word.size()) { return node->isEnd; }
        if (word[i] == '.') {
            for (TrieNode* child : node->children) {
                if (child != nullptr && dfs(child, word, i + 1)) { return true; }
            }
            return false;
        }
        TrieNode* nxt = node->children[word[i] - 'a'];
        return nxt != nullptr && dfs(nxt, word, i + 1);
    }

public:
    void addWord(const string& word) {
        TrieNode* cur = &root;
        for (char ch : word) {
            int letter = ch - 'a';
            if (cur->children[letter] == nullptr) {
                cur->children[letter] = new TrieNode();
            }
            cur = cur->children[letter];
        }
        cur->isEnd = true;
    }

    bool search(const string& word) { return dfs(&root, word, 0); }
};

void test(const string& label, bool got, bool want) {
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    WordDictionary dict;
    dict.addWord("cat");
    dict.addWord("car");
    dict.addWord("dog");
    test("middle-dot", dict.search("c.r"), true);
    test("all-dots", dict.search("..."), true);
    test("dot-miss", dict.search("d.t"), false);
    test("prefix-only", dict.search("ca"), false);
}

// follow-up: '*' = any sequence -> at '*' recurse on (node, i + 1) to match nothing,
//   or on (child, i) for every child to eat one letter; memo on (node, i)
// follow-up: many searches of one length -> also bucket words by length, skip the trie
//   when the bucket is empty
