# LC 394 Decode String (Medium). Pattern: two stacks saved at each '['.
# Decode k[encoded] = encoded repeated k times; brackets may nest. Input is valid and
# digits appear only as repeat counts.


# At '[' save (count, text so far) and start fresh; at ']' repeat and append to the saved
# text. Text is kept as char lists (strings are immutable). T: O(output length),
# S: O(output length).
def decode_string(s):
    # repeat count of each open bracket
    counts = []
    # text built before each open bracket, as a char list
    prevs = []
    # chars inside the innermost open bracket so far
    cur = []
    num = 0
    for c in s:
        if "0" <= c <= "9":
            num = num * 10 + int(c)
        elif c == "[":
            counts.append(num)
            prevs.append(cur)
            num = 0
            cur = []
        elif c == "]":
            outer = prevs.pop()
            count = counts.pop()
            for _ in range(count):
                outer.extend(cur)
            cur = outer
        else:
            cur.append(c)
    return "".join(cur)


def test(label, s, want):
    got = decode_string(s)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("flat", "2[ab]c", "ababc")
    test("nested", "2[a2[b]]", "abbabb")
    test("multi-digit", "10[a]", "aaaaaaaaaa")
    test("text-around", "x2[y]z", "xyyz")


main()

# follow-up: recursive version -> decode(s, i) reads until ']' or the end; on k[ recurse
#   and append the result k times; return (text, next index) since ints can't be
#   passed by reference
# follow-up: k-th char without expanding (LC 880) -> decoded size, scan back k %= size
