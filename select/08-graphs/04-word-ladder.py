# LC 127 Word Ladder (Hard). Pattern: BFS shortest path on implicit graph.
# Transform beginWord into endWord one letter at a time; every intermediate word must be
# in wordList. Return the number of words in the shortest sequence, or 0 if impossible.

import string
from collections import deque


# BFS by layers; neighbors = one-letter edits found in the dictionary.
# T: O(N * L^2 * 26), S: O(N * L); N = words, L = word length (slicing / hashing cost L).
def ladder_length(begin_word, end_word, word_list):
    # dictionary words not visited yet
    words = set(word_list)
    if end_word not in words:
        return 0
    # words reached in the current layer
    q = deque([begin_word])
    words.discard(begin_word)
    steps = 1
    while q:
        level_size = len(q)
        for _ in range(level_size):
            cur = q.popleft()
            if cur == end_word:
                return steps
            for pos in range(len(cur)):
                for letter in string.ascii_lowercase:
                    # strings are immutable: build the edited word by slicing
                    nxt = cur[:pos] + letter + cur[pos + 1:]
                    # remove on push so a word enters the queue only once
                    if nxt in words:
                        words.remove(nxt)
                        q.append(nxt)
        steps += 1
    return 0


def test(label, begin_word, end_word, word_list, want):
    got = ladder_length(begin_word, end_word, word_list)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("cat-to-dog", "cat", "dog", ["cot", "cog", "dog"], 4)
    test("one-step", "cat", "cot", ["cot"], 2)
    test("end-missing", "cat", "dog", ["cot", "cog"], 0)
    test("unreachable", "cat", "dog", ["cot", "dog"], 0)


main()

# follow-up: all shortest paths (LC 126) -> parents[nxt] collects every cur of the layer;
#   remove visited words per layer, not per push; DFS back from end_word
# follow-up: faster -> bidirectional BFS, always expand the smaller frontier set
