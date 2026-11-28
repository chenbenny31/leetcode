# 24 · Text Justification (Moveworks ×4, Hard) — LC 68
# pack words greedily into lines of exactly max_width chars, spaces between words
# leftover spaces spread over the gaps, left gaps get the extra ones
# last line and one-word lines are left-justified (pad the right end)
#
# clarify:
# word longer than max_width -> never
# empty word list -> not given (LC: at least one word)
# uneven spaces -> gaps on the left get one more
# last line -> single spaces, pad the right end
# line with one word -> left-justified (no gap to stretch)


# core idea: collect words until the next one won't fit, then justify those words;
# the last line is left-justified
# T: O(total chars), S: O(max_width) besides the output
def full_justify(words, max_width):
    result = []
    # words of the line being filled
    cur_words = []
    # letters only; len(cur_words) = min spaces needed once the next word joins
    cur_len = 0

    for word in words:
        if cur_len + len(word) + len(cur_words) > max_width:
            total_spaces = max_width - cur_len
            if len(cur_words) == 1:
                # one-word line: left-justified
                result.append(cur_words[0] + " " * total_spaces)
            else:
                gaps = len(cur_words) - 1
                sep_spaces = total_spaces // gaps
                ext_spaces = total_spaces % gaps
                line = ""
                for i in range(len(cur_words)):
                    line += cur_words[i]
                    if i < gaps:
                        line += " " * sep_spaces
                        # extra spaces go to the left gaps
                        if i < ext_spaces:
                            line += " "
                result.append(line)
            cur_words = []
            cur_len = 0
        cur_words.append(word)
        cur_len += len(word)

    # last line: left-justified
    last_line = " ".join(cur_words)
    last_line += " " * (max_width - len(last_line))
    result.append(last_line)
    return result


def test(label, words, max_width, want):
    ok = full_justify(words, max_width) == want
    print(label, "PASS" if ok else "FAIL")


def main():
    test("extra-goes-left", ["a", "b", "c", "d"], 6, ["a  b c", "d     "])
    test("even-gaps", ["to", "be", "or", "not", "to"], 10, ["to  be  or", "not to    "])
    test("one-word-line", ["data", "structures", "ok"], 10,
         ["data      ", "structures", "ok        "])


main()

# follow-up:
# 1. right-justify -> same packing; all spaces go before the first word
# 2. center -> same packing; (max_width - line len) // 2 spaces left, the rest right
# 3. word longer than max_width -> cut it into max_width-sized pieces before packing
# 4. words arrive as a stream -> keep cur_words; emit a line when the next won't fit
# 5. rearrange spaces in one line (LC 1592) -> the same total // gaps + extra split
