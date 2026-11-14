# Study order

Google new grad is the next target. The study list is `company/google/ref/`, numbered 00–133 in
priority order (must → recommended → optional); Part A below ranks the `select/` core files it
points at by how often Google reports hit their pattern. Paths are relative to the repo root.

How to work each item: timer on (25 min for a medium), write it cold in one file with the `test`
function, run it, then diff against the repo file and note what you missed. Reading mode: read
the header, name the approach and T/S out loud, then read the body and the follow-up block.

File standard: a `select/` file is one mother problem with terse one-line comments — header
with `// clarify:` (one point per line), `// core idea:` and `// T: .., S: ..` above each
solution, follow-ups as one-line ideas (no code; a rare snippet sits in `/* */`).
A second solution appears only when the mother problem has several standard named
techniques. `company/google/ref/` uses the same lean format but keeps the code of every part or
variant that was actually reported (an LC-family file there = the mother solution plus the
reported variants that need different code); `must/00` is his own round 1.

## Now — plan and progress (agents: read first, update the log)

- **Next interview:** Google SWE new grad, next round not scheduled (round 1 was 2026-11-12:
  primes, see the log). Cloudflare Argo HM call was 2026-11-11.
  Amazon (11-09) and PlusAI (11-10) are done; their folders were removed.
- **Plan** (2026-11-13, by Google frequency; `ref NN` = `company/google/ref/*/NN-*`; family
  counts in brackets; skip the ones with a `prep/` file: 01, 19):
  A. Most reported, type each cold:
     08 meeting rooms II [~45, 1p3a Q4 #1] → 09 meeting rooms III [×6+] → 04 topological
     sort [~19] → 12 top-k [~18] → 02 multi-source BFS [~14] → 20 window distinct [×9] →
     17 prefix sum + hash [~20] → 18 equal endpoints [×6] → 24 decode string [~10] → 11
     waitlist [×8] → 05 union-find → 06 friend timeline [1p3a ×3] → 28 level order → 29
     validate tree [×7] → 30 file system size [×6] → 42 logger dedup [×7] → 47 wrapped
     lines [×6] → 21 range duplicate check [1p3a ×3] → 31 merge trees [×5] → 49 cut cost
     [×5] → 45 trimmed average [×5] → 22 calculator [×3 + tag] → 03 server BFS [×4] → 07
     one wall break [×4] → 48 L/R pieces [×4 + tag] → 14 binary-search bounds → 15 search
     on the answer → 25 monotonic stack → 26 template expansion → 27 URL router.
  B. Reported 2–3 times, type the main function, read the follow-ups:
     10 roster merge, 13 AdService, 33 magical pairs (10 / 13 / 33 = one L3 onsite), 23
     nested eval, 16 weighted pick, 36, 41, 51, 54, 63, 82, 58, 64, 65–71, 73, 52, 53, 55,
     56, 50, 83–85, 32, 34, 35, 57, 40, 39, 72, 46, 43, 44, 86–88.
  C. Rare but must-know techniques (one cold write of the core each):
     Dijkstra + 0-1 BFS (ref 37), bottleneck path (ref 38); sieve + numbers built digit by
     digit (ref 00, `select/16-bit-math-matrix/08`, `09`); trie (ref 89), LRU (ref 90), two
     heaps (ref 44), monotonic deque (`select/03-sliding-window/04`); quickselect / merge
     sort / quicksort (ref 96, `select/02-two-pointers/10`), fast power (ref 97), Floyd
     cycle (ref 93); 0/1 knapsack (`select/13-dp/07`), DFS + memo on a grid
     (`select/13-dp/17`), job scheduling DP + upper_bound (ref 88), difference array
     (`select/11-intervals-greedy/09`), Fisher–Yates (`select/14-design/12`), rerooting
     (ref 55); Rabin–Karp (ref 73) / KMP (`select/optional/01-arrays-hashing/08`), Fenwick
     tree (`select/optional/14-design/13`), MST (`select/optional/09-advanced-graphs/06`),
     Eulerian path (`.../07`), bipartite (`select/optional/08-graphs/06`), BFS on implicit
     states (`select/08-graphs/04`), perfect matching / k equal subsets
     (`select/12-backtracking/08`, `09`), min stack (`select/05-stack/09`), lazy iterator
     (`select/14-design/10`), big-number strings (`select/16-bit-math-matrix/11`).
  D. Coverage, read only: the rest of `recommended/` (60–62, 74–81, 91–95, 98), the other
     `select/` basics, then `optional/` 99–133 and the README Optional reports table (its
     last table, "Optional — not now", is out of scope).
- **Scope:** SWE new grad / intern level. Reports from L5+, MLE / AI-ML or research (PhD / RS)
  loops are not covered now: they go to the "Optional — not now" table, never into `select/`,
  `ref/` or the weights below.
- **Format (1point3acres 2026 Q4 summary):** R1 = 45 min coding + 45 min BQ, typed in a
  Google Doc, nothing runs. Communication is scored on its own (two optimal solutions were
  still rejected for "lack of communication"): talk while typing, dry-run one example by
  hand, name the edge cases first. Follow-ups stack up: bigger input, add deletion, more
  queries, does not fit in memory. Practice at least the `must/` files in a plain doc
  without compiling. BQ: 6–7 STAR stories with numbers (`~/Code/interview/bq/google/`).
- **Lessons from the last interviews:** basics decide it (binary-search bounds and the
  `lo == 0` / `lo == n` checks, smart pointers / `make_shared`); write the straightforward
  correct solution first, state its complexity, dry-run one small case before saying "done".
  From Google round 1: primality loops to `i * i <= n`; for "print every number up to n with
  property P", think sieve / precompute first, then generate the valid numbers digit by digit
  instead of testing each one.
- **Log** (newest last; one line per session: date, rows covered, problems to revisit; ref
  numbers in lines before 2026-11-13 are the old ones):
  - 2026-11-11: plan rebuilt for Google; three batches of Google reports mapped onto `select/`
    (weights below), follow-ups added to the mother files, recommended 09–29 written.
  - 2026-11-11: FastPrep list (359 problems with sighting counts) mapped: weights re-ranked,
    more follow-up functions in `select/`, recommended 30–43 written, Optional table extended.
  - 2026-11-11: LeetCode Google tag (30-day frequency, top 100) folded into the weights; tag
    problems added as follow-up functions in `select/`, recommended 44–46, Optional rows.
  - 2026-11-11: 1point3acres Google bank checked for gaps: more follow-up functions in
    `select/`, recommended 47–50, Optional rows (labels checked: L5+, MLE and research-loop
    reports moved to "Optional — not now").
  - 2026-11-11: plan reordered by topic (graphs, then interval / heap scheduling, then hash +
    prefix sums, trees, windows / search / stack, DP); evening session typed islands
    (`prep/num-islands.cpp`), then rotting oranges, course schedule, network delay, meeting rooms.
  - 2026-11-11: `company/google/ref/` renumbered into one priority list 01–129 (must 01–31,
    recommended 32–93, optional 94–129) mixing the reported problems with the prioritized
    LeetCode numbers; the `select/` families are added as lean `.cpp` files with a short header; the 1point3acres round
    above was merged back after a parallel edit had dropped it.
  - 2026-11-12: Google round 1 done (isPrime → right-truncatable "super" primes → print all up
    to n; solved with dp[i] = prime && dp[i / 10], O(n²) primality; better: √n trial division,
    sieve, or grow from 2 / 3 / 5 / 7 by appending digits). `select/` rebuilt as a lean
    mother-problem bank (variants are idea lines only) and 12 mother files added: primes,
    numbers by digits, pow, sort, Kadane, grid DP, diameter, difference array, call-stack
    simulation, shuffle, pairing backtracking, k equal subsets; 6 promoted from `optional/`.
    Cross-checked against NeetCode 150: 5 more core mothers and 4 optional ones (see plan
    step 0); the remaining NeetCode problems are follow-up idea lines. `company/google/ref/`
    passed through the same lean format and re-checked (all files compile and pass); round 1
    added as `must/00-round1-super-primes.cpp`.
  - 2026-11-13: 1point3acres notes (2026-08 → 11, with the official Q4 frequency list)
    integrated. `ref/` renumbered to 00–133 (must 00–35, recommended 36–98, optional
    99–133): new must 06 friend graph with timeline and 21 range duplicate check; URL
    router (27) and merge trees by name (31) moved up to must; new recommended 55 (weighted
    sum of distances, was optional 127), 56 n-th license plate, 57 subtree pair sum /
    product; union of interval lists coded in 98. Follow-up lines added in `select/` and
    the ref copies (ribbons, nested list from a string, ordered checkpoints, concurrency
    == X, min product path). L5+ / senior / FDE coding reports went to "Optional — not
    now"; SD and BQ notes went to `~/Code/interview/`.

## What Google asks most (analysis, 2026-11-11)

Sources: the FastPrep list (359 problems, 554 sightings; topics tagged by keywords in each
summary, one problem can count for several topics, so the shares add up to more than 100%), the
earlier report batches, the LeetCode Google tag, and the 1point3acres Google bank (87 curated
questions with frequency labels + 411 reported coding problems, read 2026-11-11). 1point3acres
agrees: its only "very high" coding question is the meeting rooms family; then course
scheduling, islands, friend graph (union-find), top-k logs and trie autocomplete; its coding tags
rank array, hash map, DP, string, graph / DFS / tree, simulation, data structure, grid, heap.

| Rank | Topic | Share of FastPrep sightings | Typical reports |
|---|---|---|---|
| 1 | Graph traversal: BFS + union-find + topological sort | ~18% | broken-node shortest path, multi-source BFS, islands series, union-find, Kahn |
| 2 | Intervals / sweep line + heap scheduling | intervals 14%, heap 8% | car rental ×14, min chairs ×9, meeting rooms III, CPU scheduling, combined schedule |
| 3 | Hash counting + prefix sums | hash 16%, prefix 5% | equal-endpoint max subarray ×6, target remainder, top-k, group shifted |
| 4 | Trees (DFS / recursion) | ~11% | file system size ×6, min cut cost, boolean leaf flips, merge trees, validate tree |
| 5 | Design / simulation | ~7% | restaurant waitlist ×8, lockers, logger, AdService |
| 6 | Sliding window, binary search, stack parsing | ~5% each | at least k distinct ×9, calculator, nested function eval, weighted pick |
| 7 | DP, backtracking | DP 9%, backtracking 2% | mostly basic 1D / 2D DP (Kadane, LIS, knapsack); hard DP is rare |

1point3acres official 2026 Q4 summary (reports from 08-05 to 11-03, added 2026-11-13) — its
coding top list and where each one lives:

| Count | Problem | Where |
|---|---|---|
| ×8 | meeting rooms / interval scheduling variants | ref 08, 09, 10 |
| ×4 | number of islands | ref 01 |
| ×3 | course schedule / dependency variants | ref 04 |
| ×3 | friend graph with timeline | ref 06 |
| ×3 | range duplicate check, O(1) per query | ref 21 |
| ×2 | template variable expansion | ref 26 |
| ×2 | URL request handler router | ref 27 |
| ×2 | merge named tree nodes | ref 31 |

Seen once there: spreadsheet set / get with dependencies, calculator with parentheses
(ref 22), `itertools.tee`, LC 1116, swim in rising water (ref 38), min-product path
(ref 37 follow-up), lowest-bid auction, lockers (ref 52), highway toll, LC 1044 (ref 73),
an LC 4 variant (ref 99), Russian doll (ref 63), visible people (ref 25), tree distance sum
(ref 55). It confirms the ranking above: intervals first, then graphs (islands, topological
sort, union-find); the three new names (range duplicate check, template expansion, URL
router) are parsing / precompute problems where the candidate has to set the rules.

What follows from it:

1. Graphs and interval scheduling carry about half of the frequent reports. The single most
   frequent pattern is meeting rooms II (car rental ×14, chairs ×9).
2. Most problems wrap a basic algorithm in a real-life story (piano hand moves, waitlist, crop
   garden, water drops). The hard part is translating the story, not the algorithm; frequent
   Hard problems are rare.
3. Follow-ups are almost always asked and point the same ways: brute force → optimal
   (O(n^2) → O(n)), streaming input or dynamic updates, return the construction (path,
   assignment, indices), one more constraint (broken nodes, wrap past midnight, weights). The
   follow-ups of the mother files are worth more than new problems.
4. DP and backtracking weigh less than the LeetCode community suggests.
5. The LC tag leans to array / hash / two-pointer easy and medium problems (partly general
   popularity): basics are checked, but the reports are more specific.

Priority: graphs, interval / heap scheduling and hash + prefix sums carry the first 30 numbers of
`company/google/ref/`. Know the follow-up ideas of the ★★★ files `number-of-islands`,
`rotting-oranges`, `meeting-rooms-ii`, `subarray-sum-k` and `top-k-frequent` by heart.

## Part A — `select/` core by Google weight

Weight = Google interview reports that reduce to the file's pattern or its follow-ups (2025–2026
onsite / phone lists plus the FastPrep list, `×n` = sightings), plus the LeetCode Google tag
ranked by 30-day frequency (2026-11-11 snapshot): a problem in the tag's top 30 counts as 3, one in
31–100 as 1. ★★★ = 6+, ★★ = 3–5, ★ = 1–2, — = none (kept for pattern coverage). "LC tag" lists
`problem number #rank`; SQL / JavaScript problems in the tag are ignored. The reports column
names the variants: each is a follow-up idea line in that file; a name in backticks is a
function in the Google copy of that file under `company/google/ref/`, and `ref 48` =
`company/google/ref/*/48-*`.

### ★★★

| File | Pattern | LC tag | Google reports |
|---|---|---|---|
| `select/05-stack/05-decode-string.cpp` | two stacks / recursion | — | LC 394 ×4, `{k}` decompression, nested list weight sum ×3 (`depthSum`, from a raw string), 1p3a decode ×3 |
| `select/11-intervals-greedy/02-meeting-rooms-ii.cpp` | sort + min-heap of ends; sweep | — | car rental assignment ×14 (`assignRooms`), min chairs ×9, LC 253 ×3, combined schedule ×4 and roster merge (ref 10), min CPUs ×2, cyclic rooms ×2, peak concurrency ×2, meeting rooms III ×4 (ref 09), smallest unoccupied chair (`smallestChair`, LC 1942), 1p3a Q4 list ×8 for the family, ranges where exactly X run |
| `select/01-arrays-hashing/02-subarray-sum-k.cpp` | prefix sum + hash map | 560 #28 | equal endpoints ×6 (ref 18, LC 3026), split after one removal ×3 (`canSplitAfterRemoval`), target remainder ×3, max ends score ×3 (ref 19), parity queries ×3 (ref 54), flight bookings ×2, rectangle coverage ×2 (`maxRectangleOverlap`), subarray with a given mean, LC 523, LC 525, LC 974 |
| `select/08-graphs/01-number-of-islands.cpp` | grid BFS flood fill | 200 #15, 419 #70 | LC 200 ×3, land added one by one ×3 (`numIslandsAfterAdds`), enclosed regions ×3, max area ×2, battleships, diagonal ×2, crop garden ×3 (ref 66), 1p3a Q4 list ×4 |
| `select/03-sliding-window/01-longest-substring-no-repeat.cpp` | variable window + counts | 3 #11, 904 #33 | shortest with at least k distinct ×9 (`shortestAtLeastKDistinct`), at most 2 distinct (LC 904), range duplicate check ×3 (ref 21, 1p3a Q4 list), exactly k distinct (LC 992) |
| `select/08-graphs/03-rotting-oranges.cpp` | multi-source BFS by layers | 2812 #64 (ref 39) | broken-node path ×4 and server BFS (ref 03), hop path ×3, land path ×3, one wall break (ref 07), return a book ×2, safest path (ref 39), count shortest paths / nodes on any shortest path / escape a pursuer (ref 03), walls and gates (LC 286), binary-matrix path (LC 1091) |
| `select/10-heap/02-top-k-frequent.cpp` | count map + size-k heap / sort | 347 #18 | top-k IPs ×3, LC 347 ×4, largest files ×2, talkative users ×2, stream (`TopKStream`), first / last k words ×2, next word ×3 (ref 51), most active users ×2 |
| `select/05-stack/02-basic-calculator-ii.cpp` | stack of signed terms | 772 #71, 3749 #73 (ref 23) | parentheses ×3 (`calculateWithParens`), nested calls ×2 (ref 23), variable expression ×2, RPN to infix ×2 (ref 85), evaluate RPN (LC 150) |
| `select/04-binary-search/01-first-last-position.cpp` | lower / upper bound | 34 #67, 69 #41, 278 #44, 528 #48 (ref 16) | my sqrt, prefix count ×2, prefix words ×2, point-in-range ×2 and Range Module (ref 46), weighted pick ×3 (ref 16), job scheduling DP + upper_bound (ref 88) |
| `select/04-binary-search/04-koko-bananas.cpp` | binary search on the answer | 410 #19, 875 #79 | bisecting square area ×3 (ref 65), minimax path, cutting ribbons ×2 (`maxRibbonLength`, float lengths as a follow-up), min workers for a target |
| `select/05-stack/03-daily-temperatures.cpp` | monotonic stack | 1944 #30, 739 #86 | visible people to the left ×3 (`countVisibleLeft`), temperature monitor sliding max (ref 87), next greater element (LC 496) |
| `select/01-arrays-hashing/03-group-anagrams.cpp` | hash map with a canonical key | 49 #39, 242 #26 | group shifted ×3, string bag, anagram subsequences |
| `select/08-graphs/02-course-schedule.cpp` | Kahn topological sort | 207 #75 | LC 207 ×3, 1p3a Q4 list ×3, smallest order ×4, merge subsequences ×3 (two sequences, intern phone), master sequence ×2, feature rollout naming the cycle ×2 (`findCycle`), unique reconstruction ×2 (LC 444), recipes (LC 2115) |
| `select/08-graphs/07-accounts-merge.cpp` | union-find | 547 #52 | find circle num, friend graph with timeline ×3 (ref 06: earliest all friends, + every pair within k hops, unfriend), secret (ref 40), point clusters ×2, dynamic friendship ×2, org hierarchy, min cable moves |
| `select/07-trees/01-level-order.cpp` | BFS by level | — | LC 102 ×2, right side view ×2, alternating-color roots ×4 and tree roots (ref 29), same color per depth |

### ★★

| File | Pattern | LC tag | Google reports |
|---|---|---|---|
| `select/13-dp/06-word-break.cpp` | DP over prefixes | — | LC 139 ×2, element-symbol spellings ×2, Morse decodings, decode ways (LC 91) |
| `select/07-trees/02-lca.cpp` | post-order / parent pointers | — | cheapest common ancestor, blood relation, lowest common manager of k ×2, delete nodes into a forest ×2 (`delNodes`), pedigree ×2 |
| `select/01-arrays-hashing/01-two-sum.cpp` | hash map of earlier values | 1 #1, 217 #59, 219 #60 | LC 1 ×2 |
| `select/02-two-pointers/01-three-sum.cpp` | sort + two pointers | 15 #9 | sum below target ×2 |
| `select/13-dp/11-best-time-stock.cpp` | running min, one pass | 121 #14 | max ends score (ref 19) |
| `select/01-arrays-hashing/04-longest-consecutive.cpp` | hash set, start of run | 128 #12 | — |
| `select/02-two-pointers/04-container-most-water.cpp` | two pointers, move the shorter | 11 #13 | — |
| `select/06-linked-list/03-list-cycle-ii.cpp` | Floyd cycle entrance | 287 #16 | find the duplicate |
| `select/06-linked-list/02-merge-two-lists.cpp` | dummy head | 2 #2, 21 #35 | add two numbers |
| `select/05-stack/01-valid-parentheses.cpp` | stack | 20 #25 | parentheses with deletions ×2 (Optional) |
| `select/10-heap/01-kth-largest.cpp` | heap / quickselect | 215 #77, 462 #5, 1046 #92 | median moves, insert + k-th largest |
| `select/16-bit-math-matrix/04-bit-ops.cpp` | bit tricks | 3644 #95 | — |
| `select/12-backtracking/03-combination-sum.cpp` | backtracking on sums | — | 1s and 2s summing to N ×4 |
| `select/12-backtracking/01-subsets.cpp` | backtracking / bitmask | 78 #97, 22 #90 | generate parenthesis, LC 90, subsequences are words, subsets with / without duplicates ×2 |
| `select/13-dp/03-longest-increasing-subsequence.cpp` | tails + lower_bound | 354 #98, 1048 #100 | envelopes ×3, longest str chain |
| `select/09-advanced-graphs/01-network-delay.cpp` | Dijkstra | 1514 #57, 3970 #31 | max probability, run-limited labels, flights ×2 and trains (ref 53), direction violations ×3 (`minReversals`, 0-1 BFS), min-product path |
| `select/11-intervals-greedy/01-merge-intervals.cpp` | sort + merge | 56 #32, 759 #62 | employee free time, LC 56 ×2, roster merge (ref 10), meeting scheduler (LC 1229) |
| `select/15-trie/01-implement-trie.cpp` | trie | — | prefix words ×2, tokenizer, stream detector, URL router with `{param}` segments ×2 (ref 27, 1p3a Q4 list) |
| `select/09-advanced-graphs/04-swim-rising-water.cpp` | Dijkstra on max cell (bottleneck) | — | LC 778, minimax path (`minimaxBinarySearch`), safest path (ref 39), min effort path (LC 1631) |
| `select/14-design/01-lru-cache.cpp` | hash map + linked list | — | LC 146 ×3, `setCapacity`, LRU ×3 (1p3a) |
| `select/10-heap/04-median-stream.cpp` | two heaps | — | LC 295, sliding window median ×2, trimmed averages ×5 (ref 45) |
| `select/16-bit-math-matrix/02-rotate-image.cpp` | transpose + reverse | — | rotate rect, is symmetric |
| `select/16-bit-math-matrix/08-count-primes.cpp` | trial division to √n, sieve | — | his round 1 (2026-11-12): isPrime → right-truncatable primes → print all up to n |
| `select/16-bit-math-matrix/09-numbers-same-consecutive-differences.cpp` | build numbers digit by digit | — | round 1 last part (grow primes by appending digits), perfect wave / zero-free counts (ref 34) |
| `select/11-intervals-greedy/09-car-pooling.cpp` | difference array / sweep line | — | flight bookings ×2, rectangle coverage ×2, peak concurrency ×2 |
| `select/09-advanced-graphs/03-grid-obstacles-elimination.cpp` | BFS on (r, c, used) state | — | LC 1293 ×2, one wall break (ref 07) |
| `select/13-dp/07-partition-equal-subset-sum.cpp` | 0/1 knapsack | 416 #45 | electoral coalitions ×2, task reward in a time budget ×2 |
| `select/13-dp/15-unique-paths.cpp` | grid DP | — | rightward grid paths ×3 and checkpoints, also in a given order (ref 82), same-color squares (ref 81) |

### ★

| File | Pattern | LC tag | Google reports |
|---|---|---|---|
| `select/07-trees/03-validate-bst.cpp` | in-order bounds | — | implement a BST (`insertIntoBST`, `deleteNode`), subtree pair sum / product count (ref 57) |
| `select/14-design/05-insert-delete-getrandom.cpp` | vector + index map | — | FancySet with O(1) add / delete / getRandom |
| `select/08-graphs/05-clone-graph.cpp` | DFS + old → new map | — | clone an undirected graph (1p3a) |
| `select/11-intervals-greedy/05-jump-game.cpp` | greedy farthest reach | — | jump game (1p3a), jump game with profit (Optional) |
| `select/02-two-pointers/02-valid-palindrome.cpp` | two pointers, skip non-alphanumerics | — | valid palindrome (1p3a) |
| `select/12-backtracking/02-permutations.cpp` | backtracking with used flags | — | permutations II (1p3a) |
| `select/12-backtracking/04-word-search.cpp` | grid DFS with backtracking | — | word search (1p3a) |
| `select/16-bit-math-matrix/03-single-number.cpp` | XOR / voting | 136 #85, 169 #42 | majority element |
| `select/16-bit-math-matrix/01-spiral-matrix.cpp` | boundary walk | 54 #83 | — |
| `select/03-sliding-window/06-min-size-subarray-sum.cpp` | shrink while valid | 209 #76 | — |
| `select/13-dp/04-longest-common-subsequence.cpp` | 2D DP | 1143 #93 | common parent sequence |
| `select/07-trees/06-kth-smallest-bst.cpp` | order statistics | — | insert + k-th largest |
| `select/10-heap/05-k-closest-points.cpp` | size-k max-heap | — | k closest stars ×2 |
| `select/13-dp/01-house-robber.cpp` | 1D DP | 70 #49, 509 #56 | rob with houses |
| `select/14-design/03-hit-counter.cpp` | queue / ring buffer | — | five-minute hit counter ×2 |
| `select/14-design/06-time-map.cpp` | per-key history + floor search | — | interval membership (ref 46) |
| `select/05-stack/07-remove-k-digits.cpp` | monotonic stack, greedy | — | largest after removing |
| `select/11-intervals-greedy/04-interval-list-intersections.cpp` | two pointers on intervals | — | LC 986, union of two interval lists (`intervalUnion` in ref 98; K lists, external merge) |
| `select/02-two-pointers/10-sort-an-array.cpp` | merge sort, quicksort | 912 #63 | implement quicksort (1p3a) |
| `select/13-dp/16-maximum-subarray.cpp` | Kadane | 53 #40, 1911 #87 | — |
| `select/16-bit-math-matrix/10-pow-x-n.cpp` | exponentiation by squaring | 50 #36, 1922 #82 | — |
| `select/05-stack/06-basic-calculator.cpp` | stack of (result, sign) per parenthesis | 772 #71 | calculator with parentheses ×3 (ref 22) |
| `select/05-stack/08-exclusive-time-of-functions.cpp` | simulate the call stack | — | nested function calls, recursive and explicit-stack versions ×2 (ref 23) |
| `select/03-sliding-window/04-sliding-window-max.cpp` | monotonic deque | — | temperature monitor (ref 87) |
| `select/14-design/10-flatten-nested-iterator.cpp` | lazy iterator with a stack | — | merged photo iterator, level-order stream iterator (1p3a) |
| `select/14-design/12-shuffle-an-array.cpp` | Fisher–Yates | — | shuffle a deck (1p3a), weighted pick ×3 (ref 16) |
| `select/12-backtracking/08-pair-up-no-rematch.cpp` | backtracking perfect matching | — | Amazon report; mahjong winning hand ×3 has the same shape |
| `select/11-intervals-greedy/10-hand-of-straights.cpp` | greedy with an ordered count map | — | split into consecutive runs of five (1p3a), mahjong hand ×3 |
| `select/16-bit-math-matrix/11-multiply-strings.cpp` | big-number digit strings | — | add / subtract big numbers ×2 (1p3a) |
| `select/13-dp/17-longest-increasing-path-matrix.cpp` | DFS + memo on a grid | — | water drop destinations ×3 (ref 69), non-increasing grid path |

### `select/optional/` files the LC tag ranks high

Open these after the ★★ rows: `04-binary-search/06-median-two-sorted-arrays` (LC 4, #3; 1p3a ×2),
`07-trees/09-serialize-tree` (1p3a), `08-graphs/09-parallel-courses-iii`, `14-design/08-lfu-cache` (1p3a),
`13-dp/09-longest-palindromic-substring` (LC 5, #7), `04-binary-search/05-split-array-largest-sum`
(LC 410, #19), `16-bit-math-matrix/07-string-to-integer-atoi` (LC 8, #34),
`10-heap/06-task-scheduler` (LC 621, #50),
`13-dp/05-edit-distance` (LC 72, #58), `01-arrays-hashing/07-first-missing-positive` (LC 41,
#68), `05-stack/04-largest-rectangle-histogram` (LC 84, #84).

### No Google signal yet (pattern coverage)

`select/01-arrays-hashing/05-product-except-self`; `select/02-two-pointers/05-merge-sorted-array`,
`06-move-zeroes`, `07-sort-colors`; `select/03-sliding-window/03-longest-repeating-replacement`,
`05-permutation-in-string`; `select/04-binary-search/02-search-rotated-array`,
`07-find-peak-element`; `select/06-linked-list/01-reverse-list`, `07-copy-random-list`;
`select/07-trees/04-build-tree-pre-in`, `11-diameter-of-binary-tree`;
`select/09-advanced-graphs/02-cheapest-flights-k-stops`;
`select/10-heap/03-merge-k-sorted-lists`; `select/11-intervals-greedy/03-non-overlapping-intervals`;
`select/12-backtracking/09-partition-k-equal-subsets`; `select/13-dp/02-coin-change`;
`select/14-design/02-circular-queue`, `09-design-hashmap`; `select/05-stack/09-min-stack`;
`select/08-graphs/04-word-ladder`.

## Part B — company sets

- Google: `company/google/README.md` (study list `ref/` 00–133 in must / recommended /
  optional, Optional reports table).
- Cloudflare: `company/cloudflare/README.md` (ref 01–12).

## Part C — reference only

`select/optional/<topic>/` (harder or niche versions of each pattern) and `topic/` (the older
300-problem bank with the user's early drills). Open them only for a specific reported problem.
