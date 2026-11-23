# 01 · Hangman, Next Letter (Moveworks phone screen, Medium) — filter + count
# given a dictionary, the board ("a__le", '_' = unknown) and every letter guessed so far,
# return the next letter to guess: the unguessed letter found in the most dictionary
# words that still fit the board
#
# clarify:
# statement known only from short reports -> this is the usual version; confirm it
# guessed includes the revealed letters and the wrong guesses -> yes
# a word fits -> same len, revealed spots equal, and no guessed letter at a '_' spot
#   (a guessed letter would have been revealed there)
# count words or occurrences -> words (a letter twice in a word still reveals in 1 guess)
# tie on words -> more total occurrences in the fitting words, then smallest letter
# no word fits -> return ""


# T: O(len), S: O(1)
def fits(word, board, guessed):
    if len(word) != len(board):
        return False
    for i in range(len(board)):
        if board[i] != "_" and word[i] != board[i]:
            return False
        if board[i] == "_" and word[i] in guessed:
            return False
    return True


# core idea: keep the dictionary words that fit the board; for each unguessed letter
# count the fitting words that contain it; the letter in the most words wins
# T: O(26 * W * L), S: O(W) — W = dictionary size, L = word len
def next_letter(dictionary, board, guesses):
    # letters already guessed, right or wrong
    guessed = set(guesses)

    # dictionary words that still fit the board
    cands = [w for w in dictionary if fits(w, board, guessed)]

    best = ""
    # num of fitting words containing best, and its total count in them (tie-break)
    best_words = 0
    best_total = 0
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
        # tuples compare words first, then total; scanning a..z with strict >,
        # the smaller letter wins a full tie
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

# follow-up:
# 1. letter for a given '_' -> one more dimension: pos_cnt[i][word[i]] += 1
# 2. board after guessing c -> dict pattern ("_pp__") -> num of fitting words
# 3. min worst-case candidates -> minimax: per letter, pattern -> count; min of max group
# 4. play a whole game -> loop guess / reveal until no '_' or out of misses
# 5. guess the word with match counts (LC 843) -> same minimax, group by match count
# 6. many games, big dict -> bucket words by len once
