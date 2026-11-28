# 08 · Synonymous Sentences (Moveworks onsite, Medium) — LC 1258, union-find + backtrack
# synonym pairs (transitive) and a sentence of single-space words; return every
# sentence made by swapping words for synonyms, sorted lexicographically
#
# clarify:
# synonyms transitive (a~b, b~c -> a~c) -> yes
# words split by one space, lowercase -> yes (LC)
# word without synonyms -> stays as it is
# output size -> product of group sizes; small on LC (<= 10 pairs)


# path-halving find. T: ~O(1) amortized
def find_root(parent, w):
    while parent[w] != w:
        parent[w] = parent[parent[w]]
        w = parent[w]
    return w


# choose a word for position pos, recurse; at the end join the chosen words
def build(options, pos, chosen, out):
    if pos == len(options):
        out.append(" ".join(chosen))
        return
    for word in options[pos]:
        chosen.append(word)
        build(options, pos + 1, chosen, out)
        chosen.pop()


# core idea: union-find groups the synonyms; each word's options are its group, sorted;
# backtracking over sorted options already yields sorted sentences
# T: O(P log P + product of group sizes * len), S: same as the output
def generate_sentences(synonyms, text):
    # word -> its parent in the union-find
    parent = {}
    for pair in synonyms:
        for w in pair:
            if w not in parent:
                parent[w] = w
    for a, b in synonyms:
        root_a = find_root(parent, a)
        root_b = find_root(parent, b)
        if root_a != root_b:
            parent[root_a] = root_b

    # groups[root] = every word in that synonym group, sorted
    groups = {}
    for w in parent:
        root = find_root(parent, w)
        if root not in groups:
            groups[root] = []
        groups[root].append(w)
    for root in groups:
        groups[root].sort()

    # options[i] = words that can stand at position i, sorted
    options = []
    for token in text.split(" "):
        if token in parent:
            options.append(groups[find_root(parent, token)])
        else:
            options.append([token])
    out = []
    build(options, 0, [], out)
    return out


def test(label, synonyms, text, want):
    ok = generate_sentences(synonyms, text) == want
    print(label, "PASS" if ok else "FAIL")


def main():
    test("transitive-sorted", [["big", "large"], ["large", "huge"]], "a big dog",
         ["a big dog", "a huge dog", "a large dog"])
    test("no-synonyms", [], "hello world", ["hello world"])


main()

# follow-up:
# 1. no final sort -> sorted options filled left to right give sorted output
# 2. phrase synonyms -> match the longest phrase at each position (trie), backtrack
# 3. only the count -> product of the group sizes of the words in the sentence
# 4. query rewrite for search (reported) -> same groups; expand the query into an OR
