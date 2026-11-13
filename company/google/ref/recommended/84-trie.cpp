// 84 · Implement Trie (Prefix Tree) — LC 208
// Mother problem: select/15-trie/01-implement-trie.cpp
//
// LC 208 (Medium), trie with 26 child pointers: insert(word); search(word) = word was
// inserted; startsWith(prefix) = some inserted word starts with prefix. Lowercase only.
// Google reports: prefix words ×2, tokenizer, stream detector, URL router (82) ×2
//
// Clarify
// - search("ca") after insert("car") → false (prefix, not a word); duplicates harmless
// - who frees nodes → `TrieNode` destructor deletes its children
//
// Core idea
// - One node per letter; insert creates missing children and sets `isEnd` on the last.
// - Shared `walk(s)` returns the node for s or nullptr. T O(L) per op, S O(26 * letters).
//
// Tests: word, prefix not a word, prefix, longer than any word
//
// Follow-ups
// - count words with a prefix in a sorted list, no trie → lower_bound(prefix) to
//   lower_bound(prefix + '{') is one contiguous range ('{' follows 'z'), O(L log n)
// - exact lookup in a sorted list → binary_search
// - delete(word) → per-node pass count, decrement along the path, free a child at 0
// - autocomplete top-k → DFS from the prefix node, or cache top-k per node
//
//////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>

using std::cout;
using std::string;

struct TrieNode {
    // children[c] = node for letter 'a' + c, nullptr = none
    TrieNode* children[26] = {};
    // some inserted word ends here
    bool isEnd = false;

    ~TrieNode() {
        for (TrieNode* child : children) {
            delete child;
        }
    }
};

// One node per letter along the path; insert creates missing nodes.
// T: O(L) per op, S: O(26 * total letters inserted).
class Trie {
    // node for the empty prefix
    TrieNode root;

    // node reached by spelling s from the root, nullptr if the path breaks
    TrieNode* walk(const string& s) {
        TrieNode* cur = &root;
        for (char ch : s) {
            cur = cur->children[ch - 'a'];
            if (cur == nullptr) { return nullptr; }
        }
        return cur;
    }

public:
    void insert(const string& word) {
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

    bool search(const string& word) {
        TrieNode* node = walk(word);
        return node != nullptr && node->isEnd;
    }

    bool startsWith(const string& prefix) { return walk(prefix) != nullptr; }
};

void test(const string& label, bool got, bool want) {
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    Trie trie;
    trie.insert("car");
    trie.insert("cart");
    test("word", trie.search("car"), true);
    test("prefix-not-word", trie.search("ca"), false);
    test("prefix", trie.startsWith("ca"), true);
    test("longer-than-any", trie.startsWith("carts"), false);
}
