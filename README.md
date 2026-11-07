# 300blows

300 LeetCode problems in C++17, one per technique family. Selected by diffing five company lists (Google, Snowflake, Databricks, Bloomberg, and a mixed 30-day set) against a technique taxonomy, then cutting everything that duplicated a shape already represented.

The premise: coverage is about **techniques, not problem count**. Of 328 distinct problems across those lists, 166 were absent from an earlier draft — but only 57 represented a technique with no existing representative, and only two of those appeared on more than one list. Past ~300 the marginal problem is a re-run of something already here.

---

## Layout

```
arrays-hashing/     two-pointers/       sliding-window/
stack/              binary-search/      linked-list/
trees/              trie-heap/          backtracking/
graphs/             advanced-graphs/    dp/
greedy-intervals/   bit-math-matrix/    string-matching/
knapsack/           design/             parser/
scheduling/         hard/               monotonic-sweep/
optional/
```

One file per problem, named by number: `128.cpp`, `1044.cpp`. Each carries a header comment with the approach and its time/space bounds.

---

## Foundation — 259

### Arrays & Hashing (24)
`1 36 41 49 128 169 217 238 242 268 271 274 287 290 303 304 347 380 398 442 448 528 645 974`

Hash-map complement lookup, flat-array-over-bounded-domain, cyclic sort, sign marking, prefix/suffix products, prefix sum (1D, 2D, modulo-counting), Boyer–Moore voting, quickselect and bucket sort for top-k, reservoir sampling, weighted random pick, XOR pairing, index-as-hash-table.

### Two Pointers (14)
`11 15 16 18 31 42 75 80 88 125 167 189 912 986`

Converging pointers on sorted input, Dutch-flag three-way partition, in-place read/write compaction, merge backward to avoid overwriting, next-permutation pivot-and-reverse, dominance arguments for discarding a pointer, merge sort / three-way quicksort / heap sort written from scratch, rotation by three reversals.

### Sliding Window (14)
`3 76 121 209 219 220 239 424 438 523 560 567 904 1004`

Variable window with a frequency table (`have`/`need` deficit counting), fixed window as exact-match, at-most-K-distinct, monotonic deque for window extrema, high-water-mark tricks that stay correct while stale, nearest-value queries over the window (ordered set or width-(t+1) buckets).

### Stack & Monotonic Stack (16)
`20 22 32 84 85 150 155 316 394 402 456 496 503 735 739 853`

LIFO matching, monotonic stack for next-greater and span-finding, cascading pops that settle multiple pending items in one step, encoded auxiliary state (min-stack difference trick), stack simulation with annihilation, recursive descent via an explicit stack, an index stack seeded with a base for the longest balanced run.

### Binary Search (13)
`4 33 34 74 153 154 162 378 410 540 704 875 981`

The two templates and when each applies — closed-interval exact match (`lo <= hi`, `hi = mid - 1`) vs. half-open boundary finding (`lo < hi`, `hi = mid`). Search on the *answer* with a monotonic feasibility predicate, search on a rotated array, search on index parity, search on a value range with matrix counting, `lower_bound`/`upper_bound` and the floor idiom.

### Linked List (19)
`2 19 21 23 25 61 82 92 138 141 142 143 146 148 160 206 234 328 382`

Dummy-head construction, fast/slow pointers, Floyd cycle detection with the entry-point proof, in-place reversal in k-groups, interleaving for O(1)-space deep copy, bottom-up merge sort with stride doubling, LRU via list + iterator map.

### Trees (24)
`98 102 103 104 105 108 114 116 117 124 173 199 222 226 230 235 236 297 437 450 543 687 863 2096`

Traversal in all four orders, iterative inorder with an explicit stack, bounds propagation for global invariants, level-order with a frozen level boundary, O(1)-space level linking, LCA, path encoding, binary search inside a complete tree, BST insert/delete, tree-as-undirected-graph with a parent map, serialize/deserialize, lazy iterator design, Morris-style in-place rewiring in O(1) space.

### Trie & Heap (12)
`208 211 212 215 295 355 502 621 703 767 973 1046`

Prefix trie with a fixed 26-slot child array, wildcard matching, trie-guided grid DFS, top-k via size-k heap (min-heap for largest, max-heap for smallest — and why the inversion matters), two-heap median, streaming heap, k-way merge, greedy scheduling by frequency.

### Backtracking (13)
`17 37 39 40 46 47 51 78 79 90 93 131 698`

Choose/explore/unchoose with strict `push_back`/`pop_back` bracketing, `start`-index pruning for combinations vs. a `used` array for permutations, duplicate skipping on sorted input, constraint tables for N-Queens and Sudoku, subset enumeration by bitmask, pruning under a sum budget.

### Graphs (24)
`127 130 133 200 207 210 261 269 323 332 417 490 542 547 684 695 765 785 815 994 1094 1136 1197 2050`

Grid and adjacency-list DFS/BFS, multi-source BFS, level-counted BFS, BFS with momentum and on unbounded grids, three-color cycle detection (and why two colors is the classic wrong answer), Kahn's algorithm, topological levels and longest-path-in-DAG, union-find with path halving + union by size, bipartite 2-coloring, Eulerian path, difference arrays, minimum swaps as permutation cycles.

### Advanced Graphs (10)
`399 743 787 947 990 1091 1293 1584 1631 2316`

Dijkstra, Bellman-Ford with a hop limit, minimax path (minimize the maximum edge), **weighted** union-find over ratios, Kruskal/Prim MST, state-augmented BFS.

### DP (31)
`5 10 62 63 64 70 72 91 122 123 139 140 152 174 188 198 213 221 279 300 309 312 322 337 416 494 516 714 746 1048 1143`

1D and 2D tables, rolling-array compression with the saved-diagonal trick, state machines (hold/cash, cooldown, k-transaction), interval DP with a chosen-last pivot, DP computed **backward** from the target, LIS in O(n log n) via patience piles, edit distance, tree DP, subset-sum, regex matching, DP in a hash map keyed by a predecessor rule.

### Greedy & Intervals (18)
`45 53 55 56 57 134 135 252 253 435 452 763 846 1029 1353 2251 2333 2402`

Exchange arguments, sort-by-end vs. sort-by-start and why each licenses its recurrence, sweep with a heap, two-directional passes combined with `max`, greedy pairing by sorted delta, implicit-BFS jump counting, two heaps for room assignment (free by number, busy by end time), levelling the largest values down.

### Bit / Math / Matrix (18)
`29 43 48 50 54 73 136 137 149 166 190 191 204 260 338 384 462 470`

XOR pairing and parity, bit counting mod 3, mask/shift idioms and the signed-shift UB trap, fast exponentiation, division without `/` or `*`, sieve of Eratosthenes, in-place matrix rotation and spiral traversal, Fisher–Yates shuffle, cycle detection in long division, the median as the point of least total distance, rejection sampling (rand10 from rand7).

### String Matching (4)
`28 459 686 1044`

KMP failure function, the rotation trick, **rolling hash / Rabin–Karp** with binary search on length.

### Knapsack & Sequences (5)
`354 377 474 518 673`

The loop-order table made concrete — 0/1 vs. unbounded, combinations vs. permutations. Same skeleton, four different semantics purely from nesting and direction.

---

## Target-directed — 45

### M1 · Design & DS Hard (15)
`232 341 348 359 362 460 588 706 715 729 933 1146 1166 1396 1570`

Composite structures where the interface constrains the implementation: hash map from scratch, LFU, time-bucketed counters, in-memory filesystems, snapshot arrays, versioned stores, sparse vectors, nested iterators, an ordered-map interval set.

### M2 · Parser / Simulation / String Engineering (10)
`8 68 151 165 224 227 273 415 443 772`

Tokenizing without a lexer, operator precedence with and without parentheses, full recursive-descent evaluation, big-integer arithmetic on strings, text justification, in-place compaction.

### M3 · Scheduling / Heap-Greedy / Hard DP (6)
`630 871 1235 1335 1345 1751`

Three distinct uses of the same max-heap: **grow** a resource to unlock options, **evict** a bad past commitment to preserve feasibility, **defer** everything until forced then retroactively commit. Plus weighted interval scheduling.

### M4 · Hard block (7)
`6 179 301 315 480 773 992`

45 min/problem, think-aloud throughout. Custom comparators with transitivity proofs, BFS over state spaces, inversion counting, two-heap window median.

### M5 · Monotonic Structures / Sweep Line / Range Queries (7)
`218 307 759 862 907 1944 2104`

Sweep line with a heap, **segment tree / Fenwick (BIT)**, contribution counting over subarrays, prefix-sum + deque for negative-tolerant windows.

---

## Conditional

Activated only on a confirmed loop.

- **Speed pack** — `71 314 408 680 938 1249` + timed sprints (2 mediums / 35 min)
- **Systems-adjacent** — `277 1236`
- **C-track (non-LC)** — hand-written `memcpy` (overlap-safe), `strtok`, `atoi`, `itoa`, byte-order reversal
- **Company tag pull** — two weeks out, sort by 6-month frequency, take the top 30

---

## Optional — 9
`65 421 685 726 1192 1242 1494 2035 2276`

Unlikely in a 45-minute round: Tarjan bridges, directed redundant edge, bit trie for max-XOR, bitmask DP over subsets, meet-in-the-middle, a concurrent crawler, a dynamic interval-count structure, and spec-heavy parsers (valid number, number of atoms). Read only if time remains.

---

## Conventions

C++17 strictly. Explicit includes with a trailing comment naming what each provides; no `bits/stdc++.h`, no `using namespace std`, `std::` prefixes throughout. camelCase locals. Named constants over magic numbers. No side effects inside expressions.

Container choices follow measurement, not habit:

- **Fixed, compile-time-known, ≤512 bytes** → raw stack array (`int freq[26]`). Measured 15–30% faster than `std::vector` on the access itself; zero allocation.
- **Runtime-sized** → `std::vector`, reserved up front.
- **Booleans** → `std::uint8_t`, never `std::vector<bool>` (bit-packed, returns proxies).
- **Stack** → `std::stack<T, std::vector<T>>`. Push/pop at one end means the vector reclaims space; measured 10–25% faster than the deque default.
- **Both ends touched** → `std::deque`. Its chunk recycling beats a `vector` + head index that never reclaims the front — measured 20% faster on sliding-window maximum at n=10⁶.
- **BFS frontier** → `std::vector` + head index. Append-only, no reclamation needed, and the O(n) retention is already paid elsewhere.

Complexity is stated at the end of each solution with variables defined.

---

## Method

Every solution here was compiled under `-fsanitize=address,undefined` and cross-checked against a brute-force reference on randomized inputs before being committed. Several bugs found that way were invisible to the official examples:

- A Sudoku validator that never wrote to its box table — passed both official cases, wrong on 11,540 of 200,000 random boards.
- A three-color DFS missing the "already fully explored" skip — still correct, but 4.2M recursive calls instead of 61 on a 61-node graph.
- A `max_load_factor(0.25f)` habit that cost 36% on hash-set insertion at n=10⁶ by inflating the bucket array past L3.
- A merge-intervals *oracle* that modeled intervals as covering integer points, reporting 6,842 false failures — the test was wrong, not the code.

The name is from Truffaut, and from what the second pass does to a problem you thought you'd finished.
