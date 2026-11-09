# LC 986 Interval List Intersections (Medium). Pattern: two pointers over sorted lists.
# Two lists of closed intervals, each sorted and pairwise disjoint. Return every
# non-empty intersection in order; a shared endpoint gives [x, x].


# Overlap = [max start, min end]; advance whichever interval ends first.
# T: O(m + n), S: O(1) besides the output.
def interval_intersection(first, second):
    m = len(first)
    n = len(second)
    result = []
    i = 0
    j = 0
    while i < m and j < n:
        start = max(first[i][0], second[j][0])
        end = min(first[i][1], second[j][1])
        if start <= end:
            result.append([start, end])
        # the one ending first cannot overlap anything later in the other list
        if first[i][1] < second[j][1]:
            i += 1
        else:
            j += 1
    return result


def test(label, first, second, want):
    ok = interval_intersection(first, second) == want
    print(label, "PASS" if ok else "FAIL")


def main():
    test("contained", [[0, 10]], [[2, 3], [5, 6]], [[2, 3], [5, 6]])
    test("touching", [[1, 4]], [[4, 8]], [[4, 4]])
    test("one-empty", [], [[1, 5]], [])
    test("negative", [[-5, -1], [2, 3]], [[-3, 2]], [[-3, -1], [2, 2]])


main()

# follow-up: unsorted / overlapping lists -> merge each (LC 56) first, then the same
#   two pointers, O((m + n) log(m + n))
# follow-up: k lists -> fold result = intersect(result, lists[i])
