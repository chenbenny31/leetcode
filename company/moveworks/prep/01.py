from string import ascii_lowercase

def fits(word, board, guessed):
    if len(word) != len(board):
        return False
    for i in range(len(board)):
        if board[i] != "_" and word[i] != board[i]:
            return False
        if board[i] == "_" and word[i] in guessed:
            return False
    return True

# T: O(26WL), S: O(W), W=dict-size, L=word-len
def next_letter(dictionary, board, guesses):
    guessed = set(guesses)
    cands = [w for w in dictionary if fits(w, board, guessed)]

    best = ""
    best_words = 0  # num of fitting words contains best
    best_total = 0  # best's total count in them for tie-break
    for i in range(26):
        c = chr(ord("a") + i)
        if c in guessed:
            continue
        words = 0
        total = 0
        for word in cands:
            k = word.count(c)
            if k > 0:
                words += 1
                total += k

        if (words, total) > (best_words, best_total):
            best = c
            best_words = words
            best_total = total
    return best

def test(label, dictionary, board, guesses, want):
    ok = next_letter(dictionary, board, guesses) == want
    print(label, "PASS" if ok else "FAIL")

def main():
    test("most-words", ["apple", "ample", "angle", "maple"], "a___e", "ae", "l")
    test("wrong-guess-filters", ["bat", "cat", "hat"], "_at", "atc", "b")
    test("tie-by-occurrences", ["acc", "abd"], "a__", "a", "c")
    test("no-fit", ["cat", "dog"], "_____", "", "")


main()
