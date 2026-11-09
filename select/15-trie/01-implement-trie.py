# LC 208 Implement Trie (Prefix Tree) (Medium). Pattern: trie with 26 child pointers.
# insert(word); search(word) = word was inserted; starts_with(prefix) = some inserted
# word starts with prefix. Lowercase letters only.


class TrieNode:
    def __init__(self):
        # children[c] = node for letter 'a' + c, None = none
        self.children = [None] * 26
        # some inserted word ends here
        self.is_end = False


# One node per letter along the path; insert creates missing nodes.
# T: O(L) per op, S: O(26 * total letters inserted).
class Trie:
    def __init__(self):
        # node for the empty prefix
        self.root = TrieNode()

    # node reached by spelling s from the root, None if the path breaks
    def walk(self, s):
        cur = self.root
        for ch in s:
            cur = cur.children[ord(ch) - ord("a")]
            if cur is None:
                return None
        return cur

    def insert(self, word):
        cur = self.root
        for ch in word:
            letter = ord(ch) - ord("a")
            if cur.children[letter] is None:
                cur.children[letter] = TrieNode()
            cur = cur.children[letter]
        cur.is_end = True

    def search(self, word):
        node = self.walk(word)
        return node is not None and node.is_end

    def starts_with(self, prefix):
        return self.walk(prefix) is not None


def test(label, got, want):
    print(label, "PASS" if got == want else "FAIL")


def main():
    trie = Trie()
    trie.insert("car")
    trie.insert("cart")
    test("word", trie.search("car"), True)
    test("prefix-not-word", trie.search("ca"), False)
    test("prefix", trie.starts_with("ca"), True)
    test("longer-than-any", trie.starts_with("carts"), False)


main()

# follow-up: delete(word) -> per-node pass count, decrement along the path and drop a
#   child whose count hits 0
# follow-up: autocomplete top-k -> DFS from the prefix node, or cache top-k per node
# follow-up: children as a dict {ch: node} -> any alphabet, less memory when sparse
