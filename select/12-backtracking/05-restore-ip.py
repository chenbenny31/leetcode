# LC 93 Restore IP Addresses (Medium). Pattern: backtracking over 4 parts of 1-3 digits.
# s holds only digits; return every valid IPv4 address made by inserting three dots.
# Each part is 0..255 with no leading zero ("0" itself is fine).


# path = parts taken from s[0..start), joined with dots once 4 parts cover all of s
def dfs(s, start, path, result):
    n = len(s)
    if len(path) == 4:
        if start == n:
            result.append(".".join(path))
        return
    # numeric value of the part s[start:start + length]
    part_value = 0
    for length in range(1, 4):
        if start + length > n:
            break
        digit = s[start + length - 1]
        part_value = part_value * 10 + int(digit)
        if part_value > 255:
            return
        # "0x" / "0xx": leading zero
        if length > 1 and s[start] == "0":
            return
        path.append(s[start:start + length])
        dfs(s, start + length, path, result)
        # undo this part
        path.pop()


# At most 3^4 branches, each joining an O(n) string. T: O(3^4 * n), S: O(n).
def restore_ip_addresses(s):
    result = []
    path = []
    dfs(s, 0, path, result)
    return result


def test(label, s, want):
    print(label, "PASS" if restore_ip_addresses(s) == want else "FAIL")


def main():
    test("ones", "1111", ["1.1.1.1"])
    test("leading-zero", "010010", ["0.10.0.10", "0.100.1.0"])
    test("over-255", "256256256256", [])
    test("too-short", "123", [])


main()

# follow-up: prune early -> return if the digits left are < 4 - len(path) or
#   > 3 * (4 - len(path))
# follow-up: validate one IP (LC 468) -> char-loop split on '.', same per-part checks
