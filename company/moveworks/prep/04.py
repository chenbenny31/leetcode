# collect words until exceed width, then justify by line

def full_justify(words, max_width):
    result = []
    cur_words = []
    cur_len = 0

    for w in words:
        if cur_len + len(w) + len(cur_words) > max_width:
            total = max_width - cur_len

            if len(cur_words) == 1:  # one-word line left justify
                result.append(cur_words[0] + " " * total)
            else:
                gaps = len(cur_words) - 1
                sep = total // gaps
                ext = total % gaps
                line = ""
                for i in range(len(cur_words)):
                    line += cur_words[i]
                    if i < gaps:
                        line += " " * sep
                        if i < ext:  # extra space go to left gaps
                            line += " "
                result.append(line)

            cur_words = []
            cur_len = 0

        # not exceed max-width, keep load
        cur_words.append(w)
        cur_len += len(w)

    # last_line is left justify
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
