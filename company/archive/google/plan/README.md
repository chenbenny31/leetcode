# Google plan — the files in the order to do them

Next interview: Google SWE new grad, next round not scheduled (round 1 was 2026-11-12).
This folder is the working list for it: every solution to study, copied here and numbered
001–155 in the order to do them. Start at 001 and go down.

- The copies come from `../ref/` (reported Google problems and the LeetCode families) and
  from `select/` (technique mother files). A copy differs from its source only in the number
  in its title line and in cross-references such as `-> 025`, which use the numbers of this
  folder. The "Copied from" column names the source.
- Order = how often Google reports hit the problem (FastPrep counts, the LeetCode Google
  tag, both 1point3acres batches), most reported first. The counts and the reasoning are in
  `STUDY_ORDER.md` ("What Google asks most", Part A); the session log is there too.
- Run one: `g++ -std=c++17 -Wall -Wextra -g -fsanitize=address,undefined plan/NNN-slug.cpp -o a && ./a`.

## Stages

| Stage | Files | What to do |
|---|---|---|
| A — most reported | 001–038 | type each one cold in a plain document (no compiler, as in the Google Doc), then compile and run it |
| B — reported 2–3 times | 039–079 | type the main function, read the follow-ups |
| C — rare but must-know techniques | 080–106 | one cold write of the core of each |
| D — coverage | 107–155 | read only |

For every problem in A and B, practise the round, not only the code: restate the problem,
ask the `clarify:` points, say the approach and its complexity per variable, write, dry-run
one small example by hand, then go through the `follow-up:` lines out loud.

Already typed (his own files in `../prep/`): 001 meeting rooms, 008 islands part 1, 012 max
ends score; 082 is round 1 itself. Still worth typing from those: **008 part 2** (LC 305,
land added one by one, union-find) — one new-grad onsite was lost on exactly that follow-up.

Not copied here: the remaining `select/` basics (read them in place after D), the idea-only
rows in `../README.md` ("Optional reports"), and anything from L5+ / MLE / research loops
("Optional — not now"). BQ and system design notes are in `~/Code/interview/`.

## When stuck on first sight

Moves to try, in the order of the three questions below. Add a row whenever a problem
exposes a new one.

| # | Signal | Move | Examples |
|---|---|---|---|
| 1 | brute force is two nested loops | fix one loop; the value the other loop looks for can be computed → hash lookup | two sum; 010 prefix sums; 010 part 2 (remove one, then split) |
| 2 | two nested loops, and the inner bound only moves one way | use monotonicity: two pointers, sliding window, monotonic stack / deque | 009; 035 |
| 3 | values are small, or exactly 0..n | use the value as an index: counting array, buckets, `nxt[i]` / `last[value]` | 004 buckets; 020 `nxt[i]` |
| 4 | tree / recursion: trying to picture the whole path at once | look at one node only: what it needs from its children, what it hands to its parent | 022; tree diameter; DP states |
| 5 | going forward needs "undo", or a search from every point | reverse it: run time backwards, start from the target or the border, push all sources at once | 007 unfriend; 008 inland lakes; 005 |
| 6 | "smallest maximum", "earliest time when", "largest possible" | binary search on the answer with a monotonic `ok(x)` | 034; 007 part 2 |
| 7 | the same cell / node allows different moves in different situations | add a dimension to the state: (position, extra info) | 031 one wall break; 023 entry move |
| 8 | no choice of graph node feels natural | change what a node is: not always the most visible object | 006 nodes are accounts, not emails; 038 nodes are flights, not airports |
| 9 | many queries, each recomputed from scratch | precompute once, queries only read: prefix sums, suffix min / max, difference array | 020; 010 part 3 |
| 10 | a heap or union-find cannot delete | lazy deletion (mark, drop when it reaches the top), or go offline in reverse | 027; 007 |
| 11 | "exactly k" is hard to count directly | atMost(k) − atMost(k − 1), or count the complement | 009 |
| 12 | tiny limits (n ≤ 20, at most 9 digits) | it is a hint: enumerate, backtrack or use a bitmask | 043 |

Three questions that cover most of the table:

1. What is the brute force, and which part of it is recomputed? (1, 2, 9: look it up, skip
   it by monotonicity, or precompute it.)
2. Is my definition of the state / node rich enough? (4, 7, 8: add a dimension, change the
   object, or look at one node only.)
3. Can I change direction? (5, 6, 10, 11: go backwards, binary search the answer, count
   the complement.)

## A — most reported: type cold (001–038)

| # | File | Problem | Reported | Copied from | `prep/` |
|---|---|---|---|---|---|
| 001 | `001-meeting-rooms-ii.cpp` | meeting rooms II family: min rooms, car rental assignment, smallest chair | family ~45 reports, 1p3a Q4 #1, car ids ×4 · LC 253, 1942, 2406 | `ref/must/08-meeting-rooms-ii.cpp` | `01-meeting-rooms.cpp` |
| 002 | `002-meeting-rooms-iii.cpp` | LC 2402 (free-room heap + busy heap), CPU tasks with k CPUs, min cars for orders | family ×6+, CPU tasks ×2 · onsite ×6, CPU tasks L4 ×2 | `ref/must/09-meeting-rooms-iii.cpp` | — |
| 003 | `003-topological-sort.cpp` | topological sort: course schedule, order, recipes, unique reconstruction | family ~19 reports · LC 207, 210, 2115, 444 | `ref/must/04-topological-sort.cpp` | — |
| 004 | `004-top-k-frequent.cpp` | top-k frequent (count map + heap / bucket) | family ~18 reports · LC 347, 692 | `ref/must/12-top-k-frequent.cpp` | — |
| 005 | `005-multi-source-bfs.cpp` | multi-source BFS: rotting oranges, walls and gates, shortest path in a binary matrix | family ~14 reports · LC 994, 286, 1091 | `ref/must/02-multi-source-bfs.cpp` | — |
| 006 | `006-union-find.cpp` | union-find: provinces, accounts merge, cables (earliest all friends → 06) | LC 547, 721, 1101, 1319 | `ref/must/05-union-find.cpp` | — |
| 007 | `007-friend-graph-timeline.cpp` | friend logs: earliest time all connected (union-find); + every pair within k hops (binary search on the log prefix, BFS from each person); max hops and unfriend (run time backwards) as ideas | family LC 1101, 1p3a ×6 · 1p3a ×6 (LC 1101; Q4 list ×3, onsite ×3) | `ref/must/06-friend-graph-timeline.cpp` | — |
| 008 | `008-number-of-islands.cpp` | number of islands family: count, add land (union-find), enclosed, max area, battleships | family 1p3a Q4 ×4 · LC 200, 305, 1254, 695, 419 | `ref/must/01-number-of-islands.cpp` | `num-islands.cpp` (part 1) |
| 009 | `009-sliding-window-distinct.cpp` | variable window + counts: no repeat, at most k distinct, exactly k distinct | family ×9 · LC 3, 340, 904, 992 | `ref/must/20-sliding-window-distinct.cpp` | — |
| 010 | `010-prefix-sum-hash.cpp` | prefix sum + hash map family | family ~20 reports · LC 560, 523, 974, 1590, 325, 525, 1109 | `ref/must/17-prefix-sum-hash.cpp` | — |
| 011 | `011-max-subarray-equal-endpoints.cpp` | == k) | onsite, FastPrep ×6, LC tag #94 | max sum of a[i..j] with a[i] == a[j]: prefix sums + hash map of best start per value; LC 3026 (\|a[i] − a[j]\ | `ref/must/18-max-subarray-equal-endpoints.cpp` | — |
| 012 | `012-max-ends-score.cpp` | max of first + last − middle over subarrays (Kadane variant) | interview report | `ref/must/19-max-ends-score.cpp` | `max-end-score.cpp` |
| 013 | `013-decode-string.cpp` | decode string, nested list weight sum from a raw string | family ~10 reports · LC 394, 339 | `ref/must/24-decode-string.cpp` | — |
| 014 | `014-restaurant-waitlist.cpp` | join / leave / seat earliest party that fits: list scan; buckets by size of (order, id) sets; exact-size tables | family ×8 · FastPrep ×8 | `ref/must/11-restaurant-waitlist.cpp` | — |
| 015 | `015-level-order.cpp` | level order, right side view | LC 102, 199 | `ref/must/28-level-order.cpp` | — |
| 016 | `016-validate-tree.cpp` | tree from a parent array; undirected edges form a binary tree, pick the root | family ×7 · onsite ×3 | `ref/must/29-validate-tree.cpp` | — |
| 017 | `017-file-system-size.cpp` | directory size by DFS; cached sizes updated along parent pointers | family ×6 · phone, onsite ×2 | `ref/must/30-file-system-size.cpp` | — |
| 018 | `018-logger-dedup.cpp` | out-of-order rate limiter (map → set of printed times, check neighbors); drop messages with a copy within the window (two passes; streaming queue) | family ×7 · FastPrep ×4 + ×3 | `ref/recommended/42-logger-dedup.cpp` | — |
| 019 | `019-count-wrapped-lines.cpp` | greedy word wrap line count; two columns: try every left width, min of max heights | family ×6 · FastPrep ×4 + ×2 | `ref/recommended/47-count-wrapped-lines.cpp` | — |
| 020 | `020-range-duplicate-check.cpp` | are nums[l..r] all distinct, O(1) per query: next-same-value index + suffix min | family 1p3a ×3 · 1p3a Q4 ×3, L4 onsite | `ref/must/21-range-duplicate-check.cpp` | — |
| 021 | `021-merge-trees-by-name.cpp` | merge two ordered trees: same-name children merge recursively, others copied in order | family ×6 · FastPrep ×3, 1p3a ×3 (Q4 list ×2, L4 onsite) | `ref/must/31-merge-trees-by-name.cpp` | — |
| 022 | `022-boolean-tree-leaf-flip.cpp` | which single leaf flips change the root (values up, "matters" down); flips in O(height) | family onsite ×3 · onsite ×3 (early-career NG, L4 ×2), FastPrep | `ref/recommended/50-boolean-tree-leaf-flip.cpp` | — |
| 023 | `023-longest-water-flow-path.cpp` | longest path where water goes downhill, or uphill when the cell before is higher still: DFS + memo over (cell, entry move); strict comparisons assumed | family onsite ×3 · onsite ×3 (L3 early career, L4, L5) | `ref/recommended/134-longest-water-flow-path.cpp` | — |
| 024 | `024-piano-hand-moves.cpp` | group notes with max − min ≤ 4, count hand moves + fingers (greedy) | family ×3 · interview report | `ref/must/32-piano-hand-moves.cpp` | — |
| 025 | `025-roster-merge-shifts.cpp` | merge shift roster into [start, end, names] rows incl. gaps (cut points; sweep) | family ×3 · onsite ×3 (L3, L4, PhD NG) | `ref/must/10-roster-merge-shifts.cpp` | — |
| 026 | `026-min-root-leaf-cut-cost.cpp` | cut edges so the root reaches no leaf: cost(node) = Σ min(w, cost(child)), leaf = ∞ | family ×5 · FastPrep ×5 | `ref/recommended/49-min-root-leaf-cut-cost.cpp` | — |
| 027 | `027-sliding-window-trimmed-average.cpp` | window average without the k largest: two multisets + running sum; trim both ends | family ×5 · FastPrep ×3 + ×2 | `ref/recommended/45-sliding-window-trimmed-average.cpp` | — |
| 028 | `028-calculator.cpp` | calculator II / III, RPN evaluation | family ×3 + LC tag · LC 227, 772, 150 | `ref/must/22-calculator.cpp` | — |
| 029 | `029-nested-function-eval.cpp` | evaluate add(1, mul(2, sub(5, 3))): recursive descent with a shared index | family ×3 · onsite ×2, LC tag #73 (LC 3749) | `ref/must/23-nested-function-eval.cpp` | — |
| 030 | `030-server-bfs-series.cpp` | BFS distance, reachability avoiding failed servers, all distances, failure radius | family ×5 · phone ×4 | `ref/must/03-server-bfs-series.cpp` | — |
| 031 | `031-grid-shortest-path-one-break.cpp` | multi-source BFS left→right column with path; maze BFS; one wall break = state BFS | family ×4 · phone, onsite ×3 | `ref/must/07-grid-shortest-path-one-break.cpp` | — |
| 032 | `032-lr-pieces-move.cpp` | LC 2337 / LC 777: move L / R pieces into blanks (two pointers); walls follow-up | family ×4 + LC tag · phone ×4, LC tag #21 | `ref/recommended/48-lr-pieces-move.cpp` | — |
| 033 | `033-binary-search-bounds.cpp` | binary search bounds: first / last position, insert position, sqrt, first bad version | LC 34, 35, 69, 278 | `ref/must/14-binary-search-bounds.cpp` | — |
| 034 | `034-binary-search-on-answer.cpp` | binary search on the answer: koko, ship capacity, cutting ribbons | LC 875, 1011, 1891 | `ref/must/15-binary-search-on-answer.cpp` | — |
| 035 | `035-monotonic-stack.cpp` | monotonic stack: next greater, daily temperatures, visible people | LC 739, 496, 503, 1944 | `ref/must/25-monotonic-stack.cpp` | — |
| 036 | `036-string-substitution.cpp` | replace %KEY% from a dict; follow-up nested values + cycle check (DFS); agree on literal % and `%%X%%` first | family ×3 · 1p3a ×3 (Q4 list ×2, L4 phone, L4 onsite), L5 onsite | `ref/must/26-string-substitution.cpp` | — |
| 037 | `037-url-router.cpp` | register `/users/{id}` patterns: trie over segments with a wildcard child, DFS literal first | family ×3 · 1p3a ×3 (Q4 list ×2, L4 onsite R2) | `ref/must/27-url-router.cpp` | — |
| 038 | `038-train-earliest-arrival.cpp` | train / flight route feasibility with times: sort by departure, earliest arrival per station; least total layover (Dijkstra over trips) | family ×4 · onsite ×2, 2027 intern, L4 onsite | `ref/recommended/53-train-earliest-arrival.cpp` | — |

## B — reported 2–3 times: main function + follow-ups (039–079)

| # | File | Problem | Reported | Copied from | `prep/` |
|---|---|---|---|---|---|
| 039 | `039-house-flower-planting.cpp` | houses and flowers: one flower next to each house, ≤ 1 flower per row / column, no flower shared: backtracking house by house; one planting and all plantings | onsite ×2 (early-career NG, L4) | `ref/recommended/135-house-flower-planting.cpp` | — |
| 040 | `040-unique-rank-players.cpp` | players whose rank is fixed by match results: BFS on the win graph and on the reversed graph, below + above == n − 1 | phone, L4 onsite, L5 onsite | `ref/recommended/136-unique-rank-players.cpp` | — |
| 041 | `041-employee-hierarchy.cpp` | org chart add / remove: reports move up to the manager; or the most senior report (level, then joined first) takes the seat | early-career onsite R2, 1p3a | `ref/recommended/137-employee-hierarchy.cpp` | — |
| 042 | `042-max-area-rectangle-points.cpp` | max-area rectangle from points, any orientation: bucket point pairs by (midpoint, length), two pairs in a bucket are the diagonals | L4 onsite ×2 | `ref/recommended/138-max-area-rectangle-points.cpp` | — |
| 043 | `043-perfect-wave-number.cpp` | zigzag digits check + count in [1, N] by backtracking; good number variant (no digit below both neighbors) | L5 R1 (wave), L3 early-career onsite (good number) | `ref/must/34-perfect-wave-number.cpp` | — |
| 044 | `044-ad-service.cpp` | AdService: highest score, no repeat in a row, score −1 (scan; max-heap) | interview report | `ref/must/13-ad-service.cpp` | — |
| 045 | `045-magical-string-pairs.cpp` | count (i, j, k) with equal letter sets on both sides (O(n²) masks; 26-step chains) | interview report | `ref/must/33-magical-string-pairs.cpp` | — |
| 046 | `046-random-pick-weight.cpp` | LC 528: prefix sums + upper_bound on a draw in [0, total); recorded-draws variant | LC tag #48, FastPrep ×3 | `ref/must/16-random-pick-weight.cpp` | — |
| 047 | `047-group-anagrams.cpp` | group anagrams / shifted strings | LC 49, 249, 242 | `ref/recommended/36-group-anagrams.cpp` | — |
| 048 | `048-two-travelers-shared-route.cpp` | LC 2203 style: BFS from a, b and dest, min over meeting city of the three distances | FastPrep ×3 | `ref/recommended/41-two-travelers-shared-route.cpp` | — |
| 049 | `049-next-word-predictor.cpp` | bigram counts: most frequent successor; weighted sampling with prefix sums + upper_bound | phone ×2 | `ref/recommended/51-next-word-predictor.cpp` | — |
| 050 | `050-alternating-parity-queries.cpp` | does a[l..r] alternate parity: prefix count of same-parity adjacent pairs | onsite ×3 | `ref/recommended/54-alternating-parity-queries.cpp` | — |
| 051 | `051-lis.cpp` | LIS, Russian doll envelopes, string chain | LC 300, 354, 1048 | `ref/recommended/63-lis.cpp` | — |
| 052 | `052-grid-paths-checkpoints.cpp` | paths moving one column right with row −1 / 0 / +1: rolling column DP; checkpoints in column order | 1p3a, FastPrep ×2 | `ref/recommended/82-grid-paths-checkpoints.cpp` | — |
| 053 | `053-lca.cpp` | LCA: two nodes, k nodes, delete nodes into a forest | LC 236, 1676, 1110 | `ref/recommended/58-lca.cpp` | — |
| 054 | `054-word-break.cpp` | word break, decode ways | LC 139, 91 | `ref/recommended/64-word-break.cpp` | — |
| 055 | `055-horizontal-bisect-squares.cpp` | LC 3453: binary search on y over doubles; exact sweep over sorted edges | FastPrep ×3 | `ref/recommended/65-horizontal-bisect-squares.cpp` | — |
| 056 | `056-connected-crop-garden.cpp` | k crops as connected regions: fill along the snake order; stacked squares follow-up | FastPrep ×3 | `ref/recommended/66-connected-crop-garden.cpp` | — |
| 057 | `057-belt-packer-triples.cpp` | stream of sizes, remove three with max − min < threshold: multiset, check neighbor windows | onsite ×3 | `ref/recommended/67-belt-packer-triples.cpp` | — |
| 058 | `058-top-rated-similar-movies.cpp` | BFS over the similarity graph, then size-k heap on (rating, id) | FastPrep ×3 | `ref/recommended/68-top-rated-similar-movies.cpp` | — |
| 059 | `059-water-drop-destinations.cpp` | each drop flows to the lowest lower neighbor: simulate; memoized DFS | FastPrep ×3 | `ref/recommended/69-water-drop-destinations.cpp` | — |
| 060 | `060-count-one-valued-tree-regions.cpp` | count 1-regions in a tree: a 1-node whose parent is 0 (or root) starts a region | FastPrep ×3 | `ref/recommended/70-count-one-valued-tree-regions.cpp` | — |
| 061 | `061-directed-reachability-queries.cpp` | BFS per query; precompute reach[u][v] by BFS from every node; SCC condensation note | FastPrep ×3 | `ref/recommended/71-directed-reachability-queries.cpp` | — |
| 062 | `062-longest-duplicate-substring.cpp` | LC 1044: binary search on length + Rabin-Karp, verify hash hits with compare | FastPrep ×3 | `ref/recommended/73-longest-duplicate-substring.cpp` | — |
| 063 | `063-locker-system.cpp` | smallest free locker: min-heap of free lockers + customer → locker map | phone ×2 | `ref/recommended/52-locker-system.cpp` | — |
| 064 | `064-sum-of-distances-in-tree.cpp` | sum of distances in a tree by rerooting; weighted edges (the reported version) | LC 834, NG onsite ×2 | `ref/recommended/55-sum-of-distances-in-tree.cpp` | — |
| 065 | `065-nth-license-plate.cpp` | n-th plate of 00000 … 99999, 0000A … ZZZZZ: skip whole blocks, then mixed radix (format assumed from older reports) | NG onsite ×2 | `ref/recommended/56-nth-license-plate.cpp` | — |
| 066 | `066-nondecreasing-one-replacement.cpp` | longest non-decreasing subarray after one replacement: left / right run arrays | onsite ×2 | `ref/recommended/83-nondecreasing-one-replacement.cpp` | — |
| 067 | `067-recover-doubled-array.cpp` | double + shuffle; recover the original: sort by abs, count map, pair x with 2x | onsite ×2 | `ref/recommended/84-recover-doubled-array.cpp` | — |
| 068 | `068-rpn-to-infix.cpp` | RPN → infix: stack of strings; minimal parentheses by precedence and associativity | phone ×2 | `ref/recommended/85-rpn-to-infix.cpp` | — |
| 069 | `069-tower-profit.cpp` | tower height vs houses covered, max profit (sort distances; integer-height follow-up) | interview report | `ref/must/35-tower-profit.cpp` | — |
| 070 | `070-subtree-pair-sum-product.cpp` | count nodes where every (left-subtree value, right-subtree value) pair has sum ≤ node ≤ product: post-order (min, max), 4 corner products (comparison assumed) | intern phone | `ref/recommended/57-subtree-pair-sum-product.cpp` | — |
| 071 | `071-people-with-secret.cpp` | LC 2092: union-find per timestamp group, reset people not linked to 0 | phone | `ref/recommended/40-people-with-secret.cpp` | — |
| 072 | `072-safest-path-grid.cpp` | LC 2812: multi-source BFS distances, then maximin Dijkstra; binary search + BFS | LC tag #64, phone | `ref/recommended/39-safest-path-grid.cpp` | — |
| 073 | `073-count-zero-request-servers.cpp` | LC 2747: sort logs and queries, slide a time window with a server count map | LC tag #22 | `ref/recommended/72-count-zero-request-servers.cpp` | — |
| 074 | `074-interval-membership.cpp` | disjoint intervals, is x covered: scan; map start → end + upper_bound; Range Module (LC 715) | phone, LC tag #99 | `ref/recommended/46-interval-membership.cpp` | — |
| 075 | `075-merge-intervals.cpp` | merge / insert intervals, employee free time, meeting scheduler | LC 56, 57, 759, 1229 | `ref/recommended/43-merge-intervals.cpp` | — |
| 076 | `076-median-stream.cpp` | median of a stream, sliding window median | LC 295, 480 | `ref/recommended/44-median-stream.cpp` | — |
| 077 | `077-top-k-pair-sums.cpp` | K largest nums[i] + nums[j]: sorted desc, heap expanding (i, j+1), (i+1, j) with visited | onsite | `ref/recommended/86-top-k-pair-sums.cpp` | — |
| 078 | `078-temperature-monitor.cpp` | last-k readings: queue + running sum for the average, monotonic deque for the max | 1p3a | `ref/recommended/87-temperature-monitor.cpp` | — |
| 079 | `079-max-profit-job-scheduling.cpp` | LC 1235: sort by end, dp + upper_bound for the last compatible job | 1p3a | `ref/recommended/88-max-profit-job-scheduling.cpp` | — |

## C — rare but must-know techniques (080–106)

| # | File | Problem | Reported | Copied from | `prep/` |
|---|---|---|---|---|---|
| 080 | `080-dijkstra.cpp` | Dijkstra: network delay, max probability, labels with run limit, 0-1 BFS | LC 743, 1514, 3970, 1368 | `ref/recommended/37-dijkstra.cpp` | — |
| 081 | `081-minimax-path.cpp` | minimax path: swim in rising water, min effort path | LC 778, 1631 | `ref/recommended/38-minimax-path.cpp` | — |
| 082 | `082-round1-super-primes.cpp` | isPrime → super (right-truncatable) prime → all of them up to n: √n trial division, sieve + dp[i / 10], grow from 2 / 3 / 5 / 7 by digits | his round 1, 2026-11-12 | `ref/must/00-round1-super-primes.cpp` | done in the interview |
| 083 | `083-count-primes.cpp` | LC 204 Count Primes (Medium) — trial division + sieve of Eratosthenes | — | `select/16-bit-math-matrix/08-count-primes.cpp` | — |
| 084 | `084-numbers-same-consecutive-differences.cpp` | LC 967 Numbers With Same Consecutive Differences (Medium) — digit-by-digit BFS / DFS | — | `select/optional/16-bit-math-matrix/09-numbers-same-consecutive-differences.cpp` | — |
| 085 | `085-trie.cpp` | implement trie | LC 208 | `ref/recommended/89-trie.cpp` | — |
| 086 | `086-lru-cache.cpp` | LRU cache | LC 146 | `ref/recommended/90-lru-cache.cpp` | — |
| 087 | `087-sliding-window-max.cpp` | LC 239 Sliding Window Maximum (Hard) — monotonic decreasing deque | — | `select/optional/03-sliding-window/04-sliding-window-max.cpp` | — |
| 088 | `088-kth-largest.cpp` | kth largest, min moves to median, sort an array | LC 215, 462, 912 | `ref/recommended/96-kth-largest.cpp` | — |
| 089 | `089-sort-an-array.cpp` | LC 912 Sort an Array (Medium) — merge sort / quicksort with a 3-way partition | — | `select/02-two-pointers/10-sort-an-array.cpp` | — |
| 090 | `090-fast-power.cpp` | pow(x, n), count good numbers | LC 50, 1922 | `ref/recommended/97-fast-power.cpp` | — |
| 091 | `091-floyd-cycle.cpp` | linked list cycle II, find the duplicate number | LC 142, 287 | `ref/recommended/93-floyd-cycle.cpp` | — |
| 092 | `092-partition-equal-subset-sum.cpp` | LC 416 Partition Equal Subset Sum (Medium) — 0/1 knapsack, subset sum | — | `select/13-dp/07-partition-equal-subset-sum.cpp` | — |
| 093 | `093-longest-increasing-path-matrix.cpp` | LC 329 Longest Increasing Path in a Matrix (Hard) — grid DFS + memo (implicit DAG) | — | `select/optional/13-dp/17-longest-increasing-path-matrix.cpp` | — |
| 094 | `094-car-pooling.cpp` | LC 1094 Car Pooling (Medium) — difference array / sweep line | — | `select/11-intervals-greedy/09-car-pooling.cpp` | — |
| 095 | `095-shuffle-an-array.cpp` | LC 384 Shuffle an Array (Medium) — randomness: Fisher–Yates | — | `select/14-design/12-shuffle-an-array.cpp` | — |
| 096 | `096-find-first-occurrence.cpp` | LC 28 Find First Occurrence in a String (Easy) — string matching: rolling hash / KMP | — | `select/optional/01-arrays-hashing/08-find-first-occurrence.cpp` | — |
| 097 | `097-range-sum-query-mutable.cpp` | LC 307 Range Sum Query - Mutable (Medium) — Fenwick tree (binary indexed tree) | — | `select/optional/14-design/13-range-sum-query-mutable.cpp` | — |
| 098 | `098-min-cost-connect-points.cpp` | LC 1584 Min Cost to Connect All Points (Medium). Pattern: MST, Prim O(n^2). | — | `select/optional/09-advanced-graphs/06-min-cost-connect-points.cpp` | — |
| 099 | `099-reconstruct-itinerary.cpp` | LC 332 Reconstruct Itinerary (Hard) — Eulerian path (Hierholzer) | — | `select/optional/09-advanced-graphs/07-reconstruct-itinerary.cpp` | — |
| 100 | `100-is-bipartite.cpp` | LC 785 Is Graph Bipartite? (Medium). Pattern: BFS 2-coloring. | — | `select/optional/08-graphs/06-is-bipartite.cpp` | — |
| 101 | `101-word-ladder.cpp` | LC 127 Word Ladder (Hard) — BFS shortest path on an implicit graph | — | `select/optional/08-graphs/04-word-ladder.cpp` | — |
| 102 | `102-pair-up-no-rematch.cpp` | Pair Up Players Without Rematches (interview report) — backtracking perfect matching | — | `select/optional/12-backtracking/08-pair-up-no-rematch.cpp` | — |
| 103 | `103-partition-k-equal-subsets.cpp` | LC 698 Partition to K Equal Sum Subsets (Medium) — backtracking into k buckets | — | `select/optional/12-backtracking/09-partition-k-equal-subsets.cpp` | — |
| 104 | `104-min-stack.cpp` | LC 155 Min Stack (Medium) — stack of (value, min so far) pairs | — | `select/05-stack/09-min-stack.cpp` | — |
| 105 | `105-flatten-nested-iterator.cpp` | LC 341 Flatten Nested List Iterator (Medium) — stack of (list, index) | — | `select/14-design/10-flatten-nested-iterator.cpp` | — |
| 106 | `106-multiply-strings.cpp` | LC 43 Multiply Strings (Medium) — big-number arithmetic on digit strings | — | `select/16-bit-math-matrix/11-multiply-strings.cpp` | — |

## D — coverage, read only (107–155)

| # | File | Problem | Reported | Copied from | `prep/` |
|---|---|---|---|---|---|
| 107 | `107-bst-ops.cpp` | BST validate / insert / delete | LC 98, 701, 450 | `ref/recommended/59-bst-ops.cpp` | — |
| 108 | `108-two-sum.cpp` | two sum, contains duplicate I / II | LC 1, 217, 219 | `ref/recommended/60-two-sum.cpp` | — |
| 109 | `109-three-sum.cpp` | 3Sum, 3Sum smaller | LC 15, 259 | `ref/recommended/61-three-sum.cpp` | — |
| 110 | `110-kadane-stock.cpp` | stock I, Kadane, max alternating sum | LC 121, 53, 1911 | `ref/recommended/62-kadane-stock.cpp` | — |
| 111 | `111-house-robber.cpp` | house robber I / II, climbing stairs | LC 198, 213, 70 | `ref/recommended/74-house-robber.cpp` | — |
| 112 | `112-coin-change.cpp` | coin change | LC 322 | `ref/recommended/75-coin-change.cpp` | — |
| 113 | `113-subsets.cpp` | subsets I / II, generate parentheses | LC 78, 90, 22 | `ref/recommended/76-subsets.cpp` | — |
| 114 | `114-permutations.cpp` | permutations I / II | LC 46, 47 | `ref/recommended/77-permutations.cpp` | — |
| 115 | `115-combination-sum.cpp` | combination sum | LC 39 | `ref/recommended/78-combination-sum.cpp` | — |
| 116 | `116-word-search.cpp` | word search | LC 79 | `ref/recommended/79-word-search.cpp` | — |
| 117 | `117-lcs.cpp` | longest common subsequence | LC 1143 | `ref/recommended/80-lcs.cpp` | — |
| 118 | `118-count-square-submatrices.cpp` | LC 1277 / LC 221: dp = 1 + min(up, left, diag), sum or max | onsite | `ref/recommended/81-count-square-submatrices.cpp` | — |
| 119 | `119-longest-consecutive.cpp` | longest consecutive sequence | LC 128 | `ref/recommended/91-longest-consecutive.cpp` | — |
| 120 | `120-container-most-water.cpp` | container with most water | LC 11 | `ref/recommended/92-container-most-water.cpp` | — |
| 121 | `121-merge-two-lists.cpp` | merge two lists, add two numbers | LC 21, 2 | `ref/recommended/94-merge-two-lists.cpp` | — |
| 122 | `122-valid-parentheses.cpp` | valid parentheses | LC 20 | `ref/recommended/95-valid-parentheses.cpp` | — |
| 123 | `123-interval-list-intersections.cpp` | interval list intersections; union of the two lists (K lists / out of memory as ideas) | LC 986, L4 / L5 loop | `ref/recommended/98-interval-list-intersections.cpp` | — |
| 124 | `124-median-two-sorted-arrays.cpp` | median two sorted arrays | LC 4 | `ref/optional/99-median-two-sorted-arrays.cpp` | — |
| 125 | `125-longest-palindromic-substring.cpp` | longest palindromic substring | LC 5 | `ref/optional/100-longest-palindromic-substring.cpp` | — |
| 126 | `126-split-array-largest-sum.cpp` | split array largest sum | LC 410 | `ref/optional/101-split-array-largest-sum.cpp` | — |
| 127 | `127-min-window-substring.cpp` | min window substring | LC 76 | `ref/optional/102-min-window-substring.cpp` | — |
| 128 | `128-largest-rectangle-histogram.cpp` | largest rectangle histogram | LC 84 | `ref/optional/104-largest-rectangle-histogram.cpp` | — |
| 129 | `129-basic-calculator.cpp` | basic calculator | LC 224 | `ref/optional/105-basic-calculator.cpp` | — |
| 130 | `130-serialize-tree.cpp` | serialize tree | LC 297 | `ref/optional/106-serialize-tree.cpp` | — |
| 131 | `131-alien-dictionary.cpp` | alien dictionary | LC 269 | `ref/optional/107-alien-dictionary.cpp` | — |
| 132 | `132-grid-obstacles-elimination.cpp` | grid obstacles elimination | LC 1293 | `ref/optional/108-grid-obstacles-elimination.cpp` | — |
| 133 | `133-single-threaded-cpu.cpp` | single threaded cpu | LC 1834 | `ref/optional/109-single-threaded-cpu.cpp` | — |
| 134 | `134-task-scheduler.cpp` | task scheduler | LC 621 | `ref/optional/110-task-scheduler.cpp` | — |
| 135 | `135-edit-distance.cpp` | edit distance | LC 72 | `ref/optional/112-edit-distance.cpp` | — |
| 136 | `136-string-to-integer-atoi.cpp` | string to integer atoi | LC 8 | `ref/optional/113-string-to-integer-atoi.cpp` | — |
| 137 | `137-first-missing-positive.cpp` | first missing positive | LC 41 | `ref/optional/114-first-missing-positive.cpp` | — |
| 138 | `138-nodes-distance-k.cpp` | nodes distance k | LC 863 | `ref/optional/115-nodes-distance-k.cpp` | — |
| 139 | `139-vertical-order-traversal.cpp` | vertical order traversal | LC 314 | `ref/optional/116-vertical-order-traversal.cpp` | — |
| 140 | `140-lfu-cache.cpp` | lfu cache | LC 460 | `ref/optional/117-lfu-cache.cpp` | — |
| 141 | `141-cheapest-flights-k-stops.cpp` | cheapest flights k stops | LC 787 | `ref/optional/118-cheapest-flights-k-stops.cpp` | — |
| 142 | `142-clone-graph.cpp` | clone graph | LC 133 | `ref/optional/119-clone-graph.cpp` | — |
| 143 | `143-non-overlapping-intervals.cpp` | non overlapping intervals | LC 435, 452 | `ref/optional/120-non-overlapping-intervals.cpp` | — |
| 144 | `144-insert-delete-getrandom.cpp` | insert delete getrandom | LC 380 | `ref/optional/121-insert-delete-getrandom.cpp` | — |
| 145 | `145-time-map.cpp` | time map | LC 981 | `ref/optional/122-time-map.cpp` | — |
| 146 | `146-hit-counter.cpp` | hit counter | LC 362 | `ref/optional/123-hit-counter.cpp` | — |
| 147 | `147-shortest-distance-all-buildings.cpp` | shortest distance all buildings | LC 317 | `ref/optional/124-shortest-distance-all-buildings.cpp` | — |
| 148 | `148-shortest-bridge.cpp` | shortest bridge | LC 934 | `ref/optional/125-shortest-bridge.cpp` | — |
| 149 | `149-process-tasks-using-servers.cpp` | process tasks using servers | LC 1882 | `ref/optional/127-process-tasks-using-servers.cpp` | — |
| 150 | `150-data-stream-disjoint-intervals.cpp` | data stream disjoint intervals | LC 352 | `ref/optional/128-data-stream-disjoint-intervals.cpp` | — |
| 151 | `151-range-addition-2d-prefix.cpp` | range addition 2d prefix | LC 370, 304 | `ref/optional/129-range-addition-2d-prefix.cpp` | — |
| 152 | `152-binary-subarrays-with-sum.cpp` | binary subarrays with sum | LC 930 | `ref/optional/130-binary-subarrays-with-sum.cpp` | — |
| 153 | `153-tree-longest-consecutive.cpp` | tree longest consecutive | LC 298, 549 | `ref/optional/131-tree-longest-consecutive.cpp` | — |
| 154 | `154-word-break-ii.cpp` | word break ii | LC 140 | `ref/optional/132-word-break-ii.cpp` | — |
| 155 | `155-expression-add-operators.cpp` | expression add operators | LC 282 | `ref/optional/133-expression-add-operators.cpp` | — |

## Keeping this folder in sync (for AI assistants)

- `../ref/` and `select/` stay the sources. After changing a source file that has a copy
  here, apply the same change to the copy (same text; only the title number and the
  cross-reference numbers differ).
- The numbers here are the doing order, so they do change: when a problem is added or moved,
  rename the copies after it, then fix the tables above and the cross-references inside
  the copies. `ref/` numbers are stable ids and never change.
- Never edit `../prep/`.
