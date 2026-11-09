# LC 211 Design Add and Search Words Data Structure (Medium). Pattern: trie + DFS.
# add_word(word); search(word) where '.' matches any one letter. Lowercase letters.


class TrieNode:
    def __init__(self):
        # children[c] = node for letter 'a' + c, None = none
        self.children = [None] * 26
        # some added word ends here
        self.is_end = False


# Trie; search is a DFS where '.' tries every child.
# T: add O(L), search O(L) without dots, O(trie size) worst case; S: O(26 * letters).
class WordDictionary:
    def __init__(self):
        # node for the empty prefix
        self.root = TrieNode()

    # does word[i..] match some path below node
    def dfs(self, node, word, i):
        if i == len(word):
            return node.is_end
        if word[i] == ".":
            for child in node.children:
                if child is not None and self.dfs(child, word, i + 1):
                    return True
            return False
        nxt = node.children[ord(word[i]) - ord("a")]
        return nxt is not None and self.dfs(nxt, word, i + 1)

    def add_word(self, word):
        cur = self.root
        for ch in word:
            letter = ord(ch) - ord("a")
            if cur.children[letter] is None:
                cur.children[letter] = TrieNode()
            cur = cur.children[letter]
        cur.is_end = True

    def search(self, word):
        return self.dfs(self.root, word, 0)


def test(label, got, want):
    print(label, "PASS" if got == want else "FAIL")


def main():
    words = WordDictionary()
    words.add_word("cat")
    words.add_word("car")
    words.add_word("dog")
    test("middle-dot", words.search("c.r"), True)
    test("all-dots", words.search("..."), True)
    test("dot-miss", words.search("d.t"), False)
    test("prefix-only", words.search("ca"), False)


main()

# follow-up: '*' = any sequence -> at '*' recurse on (node, i + 1) to match nothing,
#   or on (child, i) for every child to eat one letter; memo on (id(node), i)
# follow-up: many searches of one length -> also bucket words by length, skip the trie
#   when the bucket is empty
