# Google — prep guide for agents

Read this before helping Siyue prep for Google. It is the only md in this company folder; don't
add others (no per-problem notes).

## Folders

- `prep/` — his own hand-written solutions (followups as `*-followup.cpp`). Never create, edit
  or reformat files here.
- `ref/` — the study list `NN-slug.*`, numbered 00–133 in priority order and split into
  `must/`, `recommended/` and `optional/` (see Tiers). all files are runnable `.cpp`; an LC
  family file is a lean version of its `select/` file (main solution + follow-ups that need
  different code; other variants as one-line ideas). A new file takes the number of its priority slot: renumber the files after it
  (and the Tiers tables, `STUDY_ORDER.md`) rather than appending out of order.
- How a new report is filed: if it reduces to a `select/` core problem, it becomes a
  one-line follow-up idea in that `select/` file, its weight goes up in `STUDY_ORDER.md`
  Part A, and the lean `ref/` copy gets a Follow-ups entry, or code only if it needs a
  different algorithm (no new file). If it is a new pattern, it gets its own mother file in
  `select/`. If it combines two techniques (e.g. prefix sums + binary search), it gets a
  `.cpp` in `must/` or `recommended/` by weight. If it is niche, it gets an Optional reports
  row. Scope check first: a report from an L5+, MLE / AI-ML or research (PhD / RS) loop is
  not covered now and goes to the last table ("Optional — not now"), whatever its pattern.

## What we know about the interview

- NG SWE. Round 1 (2026-11-12, his own): isPrime → right-truncatable "super" prime check →
  print all up to n (see `select/16-bit-math-matrix/08-count-primes.cpp` and
  `09-numbers-same-consecutive-differences.cpp`). Problems come from interview reports he collected (three batches pasted on
  2026-11-11, the FastPrep list with sighting counts, and earlier ones); tiers are a prep
  judgment. The top FastPrep counts: car rental assignment ×14 (meeting rooms II), min chairs
  ×9, restaurant waitlist ×8, file system size ×6, max subarray with equal endpoints ×6.
- The most repeated patterns across reports: top-k frequency, meeting rooms II / III and heap
  scheduling, prefix sums + hash map, shortest subarray with k distinct, BFS (multi-source, state BFS), union-find,
  topological order, tries. These live in `select/`; see `STUDY_ORDER.md` Part A.
- Expect follow-ups that ask for a better complexity than the first working solution
  (e.g. O(n^2) → O(n), brute-force count → backtracking).
- 1point3acres official 2026 Q4 summary (reports from 08-05 to 11-03, pasted 2026-11-13).
  Process: recruiter → OA or GHA (work-style survey, no code) → R1 (45 min coding + 45 min
  BQ, typed in a Google Doc, nothing runs) → R2 onsite → HC → team match → HM call. Intern
  onsite: usually two coding rounds back to back, no separate BQ (one report: 3 problems in
  one round; another: 20 minutes just to read the statement). NG OA: HackerRank, 90 min, 2
  problems (a small implementation + a larger graph / dependency / cycle one). L3 / L4:
  usually 2 coding rounds; L4 can get a design round. No AI in most technical rounds.
- Communication is scored on its own: one candidate had two optimal solutions and was
  rejected for "lack of communication". Difficulty is LC medium, but follow-ups stack up
  (bigger input, add deletion, more queries, does not fit in memory). In the Google Doc:
  talk while typing, dry-run an example by hand, name the edge cases before being asked.
- Its coding top list: meeting rooms / interval scheduling ×8 (ref 08–10), number of
  islands ×4 (ref 01), course schedule / dependencies ×3 (ref 04), friend graph with
  timeline ×3 (ref 06), range duplicate check with O(1) queries ×3 (ref 21), template
  variable expansion ×2 (ref 26), URL request router ×2 (ref 27), merge named tree nodes ×2
  (ref 31). All eight are in `must/`.
- Thread 1196220 (L3 onsite, fail) = ref 10 roster merge, ref 13 AdService, ref 33 magical
  string pairs + a Googlyness round. Three of the source posts (author "PineVOOA")
  advertise a proxy-interview service; the notes do not say which threads, so treat every
  posted analysis as unverified and keep only the problem statements.
- System design and BQ notes are not in this repo: `~/Code/interview/system-design/google/`
  and `~/Code/interview/bq/google/`.

## How to run a session

1. Pick the next problem in tier order. Skip rows that already have a `prep/` file unless he
   asks to redo one.
2. Start every problem with what to clarify with the interviewer, written in English (input
   order and ranges, duplicates / overlaps, edge cases, what to return), as a plain list.
3. One problem at a time: a very concise derivation (which idea, why it is efficient) → T/S
   for the crucial functions → the full code. Prefer the version that is easiest
   to follow line by line (e.g. prefix sums + binary search over a two-pointer with special
   cases) when both meet the constraints.
4. "check" = review his file in `prep/`: compile with `g++ -std=c++17 -Wall -Wextra`, run its
   own tests, and report only errors (say whether it is a compile failure or a test failure).
   No praise, no summaries. Never edit his file — show the fix in chat and he types it.
5. Don't run tests for solutions you only show in chat; he tests them himself.

## Code conventions (`ref/` files and any code shown in chat)

- One runnable file: header comment stating the problem, includes, one blank line,
  `using std::x;` for every type and function used (no `std::` in the body), solution,
  `test(label, input..., want)`, `main`. C++17: use `auto` / structured bindings where the
  type is obvious (copy, never `auto&`, before a `pop()`).
- Comments are terse notes, one note per line: never wrap a sentence onto a second comment
  line, and use abbreviations (dup, num, prereq, idx, len, `->`). Lowercase labels.
- Header: title line, the problem in 1–2 lines (`prereq[i] = {a, b} means b -> a; return if
  can finish all`; one line per reported part), an empty `//` line, then
  `// clarify:` followed by one point per line (`dup pairs -> ok`, `input is a batch -> a
  stream is the follow-up`).
- Above each solution: `// solution k: <name>; <when to use>` (only when the same problem
  has 2+ solutions), `// core idea: <one line>`, `// T: O(..), S: O(..)`. Bare bounds; add a
  few words of why only for unusual ones (n * n!, n * 4^n, amortized).
- Code is kept for every part or variant that was actually reported; every other follow-up is
  one line in the closing `// follow-up:` block (`<variant> (LC n) -> <idea>`). A snippet
  (≤ 6 lines) goes in a `/* */` block after that list, only where code is clearer than words.
- One solution per part. Add a second function only when the improvement is the reported
  follow-up or a standard named technique, and test both in the same `test`.
- Complexity in V+E for graphs. Comment what each key data structure stores and only the
  crucial lines. Adjacency list is `graph`; BFS/DFS use `cur` / `nxt`; DFS
  helper is `dfs`; matrix size is `m` / `n`; class data members go at the top.
- Tests: few, hand-made, high coverage, never LeetCode's examples. Labels short, words joined
  with `-`. No comments on test cases. Print only `label PASS` / `label FAIL`:
  `cout << label << (ok ? " PASS\n" : " FAIL\n");`.
- No lambdas except a `sort` comparator: a helper is a free function with explicit
  parameters (pass the containers it reads by `const&`).
- Names say what a value is: prefix sums are `prefix...` (`segPrefix`, `prefixCoins`), not
  `pre` / `upTo`. No one-line wrapper helpers; write the expression where it is used.
- No mutation inside a condition (`if (--indeg[v] == 0)`, `if (s.erase(x))`); the only
  exception is `insert(...).second`. `<sstream>` is fine: `ostringstream` for formatted output, `istringstream` for splitting
  words on whitespace; use a character loop when the format has other delimiters. Lines ≤ 90 columns (move trailing
  comments above the line).
- Minimal: implement only what the statement requires.

## Tiers

`ref/` is one study list numbered 00–133 in priority order (weight from the reports, the
LeetCode Google tag and 1point3acres, not strictly by topic), split into `must/`,
`recommended/` and `optional/`. Every file is a runnable `.cpp` in the same lean format as `select/` (see Code
conventions). Unlike `select/`, a file here keeps the code of every part
or variant that was actually reported. An LC-family file names its mother file in `select/` on
line 2. `00` is his own round 1. The `prep/` column names his file(s) (— = not written yet). Run a
`.cpp` with `g++ -std=c++17 -Wall -Wextra -g -fsanitize=address,undefined ref/<tier>/NN-slug.cpp -o a && ./a`.

### Must (00–35), grouped by topic

| # | `ref/` file | Source | Problem | `prep/` |
|---|---|---|---|---|
| 00 | `00-round1-super-primes.cpp` | his round 1, 2026-11-12 | isPrime → super (right-truncatable) prime → all of them up to n: √n trial division, sieve + dp[i / 10], grow from 2 / 3 / 5 / 7 by digits | done in the interview |
| | **Graphs: BFS, topological sort, union-find** | | | |
| 01 | `01-number-of-islands.cpp` | LC 200, 305, 1254, 695, 419 | number of islands family: count, add land (union-find), enclosed, max area, battleships → `select/08-graphs/01-number-of-islands.cpp` | `num-islands.cpp` |
| 02 | `02-multi-source-bfs.cpp` | LC 994, 286, 1091 | multi-source BFS: rotting oranges, walls and gates, shortest path in a binary matrix → `select/08-graphs/03-rotting-oranges.cpp` | — |
| 03 | `03-server-bfs-series.cpp` | phone ×4 | BFS distance, reachability avoiding failed servers, all distances, failure radius | — |
| 04 | `04-topological-sort.cpp` | LC 207, 210, 2115, 444 | topological sort: course schedule, order, recipes, unique reconstruction → `select/08-graphs/02-course-schedule.cpp` | — |
| 05 | `05-union-find.cpp` | LC 547, 721, 1101, 1319 | union-find: provinces, accounts merge, cables (earliest all friends → 06) → `select/08-graphs/07-accounts-merge.cpp` | — |
| 06 | `06-friend-graph-timeline.cpp` | 1p3a Q4 ×3, L4 onsite | friend logs: earliest time all connected (union-find); + every pair within k hops (binary search on the log prefix, BFS from each person); unfriend as ideas | — |
| 07 | `07-grid-shortest-path-one-break.cpp` | phone, onsite ×3 | multi-source BFS left→right column with path; maze BFS; one wall break = state BFS | — |
| | **Scheduling: intervals + heaps** | | | |
| 08 | `08-meeting-rooms-ii.cpp` | LC 253, 1942, 2406 | meeting rooms II family: min rooms, car rental assignment, smallest chair → `select/11-intervals-greedy/02-meeting-rooms-ii.cpp` | — |
| 09 | `09-meeting-rooms-iii.cpp` | onsite ×6 | LC 2402 (free-room heap + busy heap), CPU tasks with k CPUs, min cars for orders | — |
| 10 | `10-roster-merge-shifts.cpp` | interview report | merge shift roster into [start, end, names] rows incl. gaps (cut points; sweep) | — |
| 11 | `11-restaurant-waitlist.cpp` | FastPrep ×8 | join / leave / seat earliest party that fits: list scan; buckets by size of (order, id) sets; exact-size tables | — |
| | **Heap / top-k** | | | |
| 12 | `12-top-k-frequent.cpp` | LC 347, 692 | top-k frequent (count map + heap / bucket) → `select/10-heap/02-top-k-frequent.cpp` | — |
| 13 | `13-ad-service.cpp` | interview report | AdService: highest score, no repeat in a row, score −1 (scan; max-heap) | — |
| | **Binary search** | | | |
| 14 | `14-binary-search-bounds.cpp` | LC 34, 35, 69, 278 | binary search bounds: first / last position, insert position, sqrt, first bad version → `select/04-binary-search/01-first-last-position.cpp` | — |
| 15 | `15-binary-search-on-answer.cpp` | LC 875, 1011, 1891 | binary search on the answer: koko, ship capacity, cutting ribbons → `select/04-binary-search/04-koko-bananas.cpp` | — |
| 16 | `16-random-pick-weight.cpp` | LC tag #48, FastPrep ×3 | LC 528: prefix sums + upper_bound on a draw in [0, total); recorded-draws variant | — |
| | **Hash + prefix sums, sliding window** | | | |
| 17 | `17-prefix-sum-hash.cpp` | LC 560, 523, 974, 1590, 325, 525, 1109 | prefix sum + hash map family → `select/01-arrays-hashing/02-subarray-sum-k.cpp` | — |
| 18 | `18-max-subarray-equal-endpoints.cpp` | onsite, FastPrep ×6, LC tag #94 | max sum of a[i..j] with a[i] == a[j]: prefix sums + hash map of best start per value; LC 3026 (\|a[i] − a[j]\| == k) | — |
| 19 | `19-max-ends-score.cpp` | interview report | max of first + last − middle over subarrays (Kadane variant) | `max-end-score.cpp` |
| 20 | `20-sliding-window-distinct.cpp` | LC 3, 340, 904, 992 | variable window + counts: no repeat, at most k distinct, exactly k distinct → `select/03-sliding-window/01-longest-substring-no-repeat.cpp` | — |
| 21 | `21-range-duplicate-check.cpp` | 1p3a Q4 ×3, L4 onsite | are nums[l..r] all distinct, O(1) per query: next-same-value index + suffix min | — |
| | **Stack and parsing** | | | |
| 22 | `22-calculator.cpp` | LC 227, 772, 150 | calculator II / III, RPN evaluation → `select/05-stack/02-basic-calculator-ii.cpp` | — |
| 23 | `23-nested-function-eval.cpp` | onsite ×2, LC tag #73 (LC 3749) | evaluate add(1, mul(2, sub(5, 3))): recursive descent with a shared index | — |
| 24 | `24-decode-string.cpp` | LC 394, 339 | decode string, nested list weight sum from a raw string → `select/05-stack/05-decode-string.cpp` | — |
| 25 | `25-monotonic-stack.cpp` | LC 739, 496, 503, 1944 | monotonic stack: next greater, daily temperatures, visible people → `select/05-stack/03-daily-temperatures.cpp` | — |
| 26 | `26-string-substitution.cpp` | 1p3a Q4 ×2, L4 phone, L5 onsite | replace %KEY% from a dict; follow-up nested values + cycle check (DFS); agree on literal % and `%%X%%` first | — |
| 27 | `27-url-router.cpp` | 1p3a Q4 ×2 | register `/users/{id}` patterns: trie over segments with a wildcard child, DFS literal first | — |
| | **Trees** | | | |
| 28 | `28-level-order.cpp` | LC 102, 199 | level order, right side view → `select/07-trees/01-level-order.cpp` | — |
| 29 | `29-validate-tree.cpp` | onsite ×3 | tree from a parent array; undirected edges form a binary tree, pick the root | — |
| 30 | `30-file-system-size.cpp` | phone, onsite ×2 | directory size by DFS; cached sizes updated along parent pointers | — |
| 31 | `31-merge-trees-by-name.cpp` | FastPrep ×3, 1p3a Q4 ×2 | merge two ordered trees: same-name children merge recursively, others copied in order | — |
| | **Simulation / other reported** | | | |
| 32 | `32-piano-hand-moves.cpp` | interview report | group notes with max − min ≤ 4, count hand moves + fingers (greedy) | — |
| 33 | `33-magical-string-pairs.cpp` | interview report | count (i, j, k) with equal letter sets on both sides (O(n²) masks; 26-step chains) | — |
| 34 | `34-perfect-wave-number.cpp` | interview report | zigzag digits check + count in [1, N] by backtracking | — |
| 35 | `35-tower-profit.cpp` | interview report | tower height vs houses covered, max profit (sort distances; integer-height follow-up) | — |
| | **Graphs: BFS, topological sort, union-find** | | | |
| | **Scheduling: intervals + heaps** | | | |
| | **Heap / top-k** | | | |
| | **Binary search** | | | |
| | **Hash + prefix sums, sliding window** | | | |
| | **Stack and parsing** | | | |
| | **Trees** | | | |
| | **Simulation / other reported** | | | |

### Recommended (36–98)

| # | `ref/` file | Source | Problem | `prep/` |
|---|---|---|---|---|
| 36 | `36-group-anagrams.cpp` | LC 49, 249, 242 | group anagrams / shifted strings → `select/01-arrays-hashing/03-group-anagrams.cpp` | — |
| 37 | `37-dijkstra.cpp` | LC 743, 1514, 3970, 1368 | Dijkstra: network delay, max probability, labels with run limit, 0-1 BFS → `select/09-advanced-graphs/01-network-delay.cpp` | — |
| 38 | `38-minimax-path.cpp` | LC 778, 1631 | minimax path: swim in rising water, min effort path → `select/09-advanced-graphs/04-swim-rising-water.cpp` | — |
| 39 | `39-safest-path-grid.cpp` | LC tag #64, phone | LC 2812: multi-source BFS distances, then maximin Dijkstra; binary search + BFS | — |
| 40 | `40-people-with-secret.cpp` | phone | LC 2092: union-find per timestamp group, reset people not linked to 0 | — |
| 41 | `41-two-travelers-shared-route.cpp` | FastPrep ×3 | LC 2203 style: BFS from a, b and dest, min over meeting city of the three distances | — |
| 42 | `42-logger-dedup.cpp` | FastPrep ×4 + ×3 | out-of-order rate limiter (map → set of printed times, check neighbors); drop messages with a copy within the window (two passes; streaming queue) | — |
| 43 | `43-merge-intervals.cpp` | LC 56, 57, 759, 1229 | merge / insert intervals, employee free time, meeting scheduler → `select/11-intervals-greedy/01-merge-intervals.cpp` | — |
| 44 | `44-median-stream.cpp` | LC 295, 480 | median of a stream, sliding window median → `select/10-heap/04-median-stream.cpp` | — |
| 45 | `45-sliding-window-trimmed-average.cpp` | FastPrep ×3 + ×2 | window average without the k largest: two multisets + running sum; trim both ends | — |
| 46 | `46-interval-membership.cpp` | phone, LC tag #99 | disjoint intervals, is x covered: scan; map start → end + upper_bound; Range Module (LC 715) | — |
| 47 | `47-count-wrapped-lines.cpp` | FastPrep ×4 + ×2 | greedy word wrap line count; two columns: try every left width, min of max heights | — |
| 48 | `48-lr-pieces-move.cpp` | phone ×4, LC tag #21 | LC 2337 / LC 777: move L / R pieces into blanks (two pointers); walls follow-up | — |
| 49 | `49-min-root-leaf-cut-cost.cpp` | FastPrep ×5 | cut edges so the root reaches no leaf: cost(node) = Σ min(w, cost(child)), leaf = ∞ | — |
| 50 | `50-boolean-tree-leaf-flip.cpp` | onsite ×2 | which single leaf flips change the root (values up, "matters" down); flips in O(height) | — |
| 51 | `51-next-word-predictor.cpp` | phone ×2 | bigram counts: most frequent successor; weighted sampling with prefix sums + upper_bound | — |
| 52 | `52-locker-system.cpp` | phone ×2 | smallest free locker: min-heap of free lockers + customer → locker map | — |
| 53 | `53-train-earliest-arrival.cpp` | onsite ×2 | train / flight route feasibility with times: sort by departure, earliest arrival per station | — |
| 54 | `54-alternating-parity-queries.cpp` | onsite ×3 | does a[l..r] alternate parity: prefix count of same-parity adjacent pairs | — |
| 55 | `55-sum-of-distances-in-tree.cpp` | LC 834, NG onsite ×2 | sum of distances in a tree by rerooting; weighted edges (the reported version) | — |
| 56 | `56-nth-license-plate.cpp` | NG onsite ×2 | n-th plate of 00000 … 99999, 0000A … ZZZZZ: skip whole blocks, then mixed radix (format assumed from older reports) | — |
| 57 | `57-subtree-pair-sum-product.cpp` | intern phone | count nodes where every (left-subtree value, right-subtree value) pair has sum ≤ node ≤ product: post-order (min, max), 4 corner products (comparison assumed) | — |
| 58 | `58-lca.cpp` | LC 236, 1676, 1110 | LCA: two nodes, k nodes, delete nodes into a forest → `select/07-trees/02-lca.cpp` | — |
| 59 | `59-bst-ops.cpp` | LC 98, 701, 450 | BST validate / insert / delete → `select/07-trees/03-validate-bst.cpp` | — |
| 60 | `60-two-sum.cpp` | LC 1, 217, 219 | two sum, contains duplicate I / II → `select/01-arrays-hashing/01-two-sum.cpp` | — |
| 61 | `61-three-sum.cpp` | LC 15, 259 | 3Sum, 3Sum smaller → `select/02-two-pointers/01-three-sum.cpp` | — |
| 62 | `62-kadane-stock.cpp` | LC 121, 53, 1911 | stock I, Kadane, max alternating sum → `select/13-dp/11-best-time-stock.cpp` | — |
| 63 | `63-lis.cpp` | LC 300, 354, 1048 | LIS, Russian doll envelopes, string chain → `select/13-dp/03-longest-increasing-subsequence.cpp` | — |
| 64 | `64-word-break.cpp` | LC 139, 91 | word break, decode ways → `select/13-dp/06-word-break.cpp` | — |
| 65 | `65-horizontal-bisect-squares.cpp` | FastPrep ×3 | LC 3453: binary search on y over doubles; exact sweep over sorted edges | — |
| 66 | `66-connected-crop-garden.cpp` | FastPrep ×3 | k crops as connected regions: fill along the snake order; stacked squares follow-up | — |
| 67 | `67-belt-packer-triples.cpp` | onsite ×3 | stream of sizes, remove three with max − min < threshold: multiset, check neighbor windows | — |
| 68 | `68-top-rated-similar-movies.cpp` | FastPrep ×3 | BFS over the similarity graph, then size-k heap on (rating, id) | — |
| 69 | `69-water-drop-destinations.cpp` | FastPrep ×3 | each drop flows to the lowest lower neighbor: simulate; memoized DFS | — |
| 70 | `70-count-one-valued-tree-regions.cpp` | FastPrep ×3 | count 1-regions in a tree: a 1-node whose parent is 0 (or root) starts a region | — |
| 71 | `71-directed-reachability-queries.cpp` | FastPrep ×3 | BFS per query; precompute reach[u][v] by BFS from every node; SCC condensation note | — |
| 72 | `72-count-zero-request-servers.cpp` | LC tag #22 | LC 2747: sort logs and queries, slide a time window with a server count map | — |
| 73 | `73-longest-duplicate-substring.cpp` | FastPrep ×3 | LC 1044: binary search on length + Rabin-Karp, verify hash hits with compare | — |
| 74 | `74-house-robber.cpp` | LC 198, 213, 70 | house robber I / II, climbing stairs → `select/13-dp/01-house-robber.cpp` | — |
| 75 | `75-coin-change.cpp` | LC 322 | coin change → `select/13-dp/02-coin-change.cpp` | — |
| 76 | `76-subsets.cpp` | LC 78, 90, 22 | subsets I / II, generate parentheses → `select/12-backtracking/01-subsets.cpp` | — |
| 77 | `77-permutations.cpp` | LC 46, 47 | permutations I / II → `select/12-backtracking/02-permutations.cpp` | — |
| 78 | `78-combination-sum.cpp` | LC 39 | combination sum → `select/12-backtracking/03-combination-sum.cpp` | — |
| 79 | `79-word-search.cpp` | LC 79 | word search → `select/12-backtracking/04-word-search.cpp` | — |
| 80 | `80-lcs.cpp` | LC 1143 | longest common subsequence → `select/13-dp/04-longest-common-subsequence.cpp` | — |
| 81 | `81-count-square-submatrices.cpp` | onsite | LC 1277 / LC 221: dp = 1 + min(up, left, diag), sum or max | — |
| 82 | `82-grid-paths-checkpoints.cpp` | 1p3a, FastPrep ×2 | paths moving one column right with row −1 / 0 / +1: rolling column DP; checkpoints in column order | — |
| 83 | `83-nondecreasing-one-replacement.cpp` | onsite ×2 | longest non-decreasing subarray after one replacement: left / right run arrays | — |
| 84 | `84-recover-doubled-array.cpp` | onsite ×2 | double + shuffle; recover the original: sort by abs, count map, pair x with 2x | — |
| 85 | `85-rpn-to-infix.cpp` | phone ×2 | RPN → infix: stack of strings; minimal parentheses by precedence and associativity | — |
| 86 | `86-top-k-pair-sums.cpp` | onsite | K largest nums[i] + nums[j]: sorted desc, heap expanding (i, j+1), (i+1, j) with visited | — |
| 87 | `87-temperature-monitor.cpp` | 1p3a | last-k readings: queue + running sum for the average, monotonic deque for the max | — |
| 88 | `88-max-profit-job-scheduling.cpp` | 1p3a | LC 1235: sort by end, dp + upper_bound for the last compatible job | — |
| 89 | `89-trie.cpp` | LC 208 | implement trie → `select/15-trie/01-implement-trie.cpp` | — |
| 90 | `90-lru-cache.cpp` | LC 146 | LRU cache → `select/14-design/01-lru-cache.cpp` | — |
| 91 | `91-longest-consecutive.cpp` | LC 128 | longest consecutive sequence → `select/01-arrays-hashing/04-longest-consecutive.cpp` | — |
| 92 | `92-container-most-water.cpp` | LC 11 | container with most water → `select/02-two-pointers/04-container-most-water.cpp` | — |
| 93 | `93-floyd-cycle.cpp` | LC 142, 287 | linked list cycle II, find the duplicate number → `select/06-linked-list/03-list-cycle-ii.cpp` | — |
| 94 | `94-merge-two-lists.cpp` | LC 21, 2 | merge two lists, add two numbers → `select/06-linked-list/02-merge-two-lists.cpp` | — |
| 95 | `95-valid-parentheses.cpp` | LC 20 | valid parentheses → `select/05-stack/01-valid-parentheses.cpp` | — |
| 96 | `96-kth-largest.cpp` | LC 215, 462, 912 | kth largest, min moves to median, sort an array → `select/10-heap/01-kth-largest.cpp` | — |
| 97 | `97-fast-power.cpp` | LC 50, 1922 | pow(x, n), count good numbers → `select/16-bit-math-matrix/04-bit-ops.cpp` | — |
| 98 | `98-interval-list-intersections.cpp` | LC 986, L4 / L5 loop | interval list intersections; union of the two lists (K lists / out of memory as ideas) → `select/11-intervals-greedy/04-interval-list-intersections.cpp` | — |

### Optional (99–133): extra LeetCode practice

| # | `ref/` file | Source | Problem | `prep/` |
|---|---|---|---|---|
| 99 | `99-median-two-sorted-arrays.cpp` | LC 4 | median two sorted arrays → `select/optional/04-binary-search/06-median-two-sorted-arrays.cpp` | — |
| 100 | `100-longest-palindromic-substring.cpp` | LC 5 | longest palindromic substring → `select/optional/13-dp/09-longest-palindromic-substring.cpp` | — |
| 101 | `101-split-array-largest-sum.cpp` | LC 410 | split array largest sum → `select/optional/04-binary-search/05-split-array-largest-sum.cpp` | — |
| 102 | `102-min-window-substring.cpp` | LC 76 | min window substring → `select/optional/03-sliding-window/02-min-window-substring.cpp` | — |
| 103 | `103-sliding-window-max.cpp` | LC 239 | sliding window max → `select/03-sliding-window/04-sliding-window-max.cpp` | — |
| 104 | `104-largest-rectangle-histogram.cpp` | LC 84 | largest rectangle histogram → `select/optional/05-stack/04-largest-rectangle-histogram.cpp` | — |
| 105 | `105-basic-calculator.cpp` | LC 224 | basic calculator → `select/05-stack/06-basic-calculator.cpp` | — |
| 106 | `106-serialize-tree.cpp` | LC 297 | serialize tree → `select/optional/07-trees/09-serialize-tree.cpp` | — |
| 107 | `107-alien-dictionary.cpp` | LC 269 | alien dictionary → `select/optional/08-graphs/08-alien-dictionary.cpp` | — |
| 108 | `108-grid-obstacles-elimination.cpp` | LC 1293 | grid obstacles elimination → `select/09-advanced-graphs/03-grid-obstacles-elimination.cpp` | — |
| 109 | `109-single-threaded-cpu.cpp` | LC 1834 | single threaded cpu → `select/optional/10-heap/08-single-threaded-cpu.cpp` | — |
| 110 | `110-task-scheduler.cpp` | LC 621 | task scheduler → `select/optional/10-heap/06-task-scheduler.cpp` | — |
| 111 | `111-partition-equal-subset-sum.cpp` | LC 416 | partition equal subset sum → `select/13-dp/07-partition-equal-subset-sum.cpp` | — |
| 112 | `112-edit-distance.cpp` | LC 72 | edit distance → `select/optional/13-dp/05-edit-distance.cpp` | — |
| 113 | `113-string-to-integer-atoi.cpp` | LC 8 | string to integer atoi → `select/optional/16-bit-math-matrix/07-string-to-integer-atoi.cpp` | — |
| 114 | `114-first-missing-positive.cpp` | LC 41 | first missing positive → `select/optional/01-arrays-hashing/07-first-missing-positive.cpp` | — |
| 115 | `115-nodes-distance-k.cpp` | LC 863 | nodes distance k → `select/optional/07-trees/08-nodes-distance-k.cpp` | — |
| 116 | `116-vertical-order-traversal.cpp` | LC 314 | vertical order traversal → `select/optional/07-trees/10-vertical-order-traversal.cpp` | — |
| 117 | `117-lfu-cache.cpp` | LC 460 | lfu cache → `select/optional/14-design/08-lfu-cache.cpp` | — |
| 118 | `118-cheapest-flights-k-stops.cpp` | LC 787 | cheapest flights k stops → `select/09-advanced-graphs/02-cheapest-flights-k-stops.cpp` | — |
| 119 | `119-clone-graph.cpp` | LC 133 | clone graph → `select/08-graphs/05-clone-graph.cpp` | — |
| 120 | `120-non-overlapping-intervals.cpp` | LC 435, 452 | non overlapping intervals → `select/11-intervals-greedy/03-non-overlapping-intervals.cpp` | — |
| 121 | `121-insert-delete-getrandom.cpp` | LC 380 | insert delete getrandom → `select/14-design/05-insert-delete-getrandom.cpp` | — |
| 122 | `122-time-map.cpp` | LC 981 | time map → `select/14-design/06-time-map.cpp` | — |
| 123 | `123-hit-counter.cpp` | LC 362 | hit counter → `select/14-design/03-hit-counter.cpp` | — |
| 124 | `124-shortest-distance-all-buildings.cpp` | LC 317 | shortest distance all buildings | — |
| 125 | `125-shortest-bridge.cpp` | LC 934 | shortest bridge | — |
| 126 | `126-car-pooling.cpp` | LC 1094 | car pooling | — |
| 127 | `127-process-tasks-using-servers.cpp` | LC 1882 | process tasks using servers | — |
| 128 | `128-data-stream-disjoint-intervals.cpp` | LC 352 | data stream disjoint intervals | — |
| 129 | `129-range-addition-2d-prefix.cpp` | LC 370, 304 | range addition 2d prefix | — |
| 130 | `130-binary-subarrays-with-sum.cpp` | LC 930 | binary subarrays with sum | — |
| 131 | `131-tree-longest-consecutive.cpp` | LC 298, 549 | tree longest consecutive | — |
| 132 | `132-word-break-ii.cpp` | LC 140 | word break ii | — |
| 133 | `133-expression-add-operators.cpp` | LC 282 | expression add operators | — |

### Optional reports — niche (no file; idea only)

| Problem | Source | Idea |
|---|---|---|
| LR pieces on a circular board (LC 777 follow-up) | phone | cut the circle at an empty cell, then the linear check of ref 48 |
| Org hierarchy: manager / peer / is_manager | onsite | union-find of peer classes, one manager pointer per class, reject conflicts and cycles by walking up |
| Belt packer with 2D sizes | onsite | sort by x, only items with abs(dx) < threshold are candidates (ref 67 follow-up) |
| Data chunking (equal chunks; indivisible header) | onsite | chunks = ceil(data / max); first `data % chunks` get base + 1; header variant fills the header chunk first |
| Sudoku solver (LC 37) | onsite | backtracking with row / column / box used flags |
| Android unlock patterns (LC 351) | onsite | DFS with a "skip over" table; count from 1, 2, 5 and use symmetry |
| Two-digit numbers sharing a digit (max subset; largest group) | OA | count per digit 0–9; group version = union-find over digit nodes |
| Tokens jumping 3 to the right collecting coins | OA | residue class mod 3; with blocking, simulate / search |
| Minimum racks for two-resource machines | phone | bin packing, bitmask DP over subsets (n ≤ 20) |
| All length ≥ 3 subsequences are words / anagrams | phone | enumerate 2^n subsequences; anagram version keys by sorted letters |
| FizzBuzz market order matching | phone | FIFO queue per side, scan for compatible price, aggregate by (buyer, seller) with VWAP |
| First occurrence of a target across a list of strings | onsite | treat as one stream; KMP or a rolling window across string boundaries |
| Multiply with only +, >>, % (and LC 397) | onsite | Russian peasant multiplication; LC 397: n % 4 == 1 or n == 3 → n − 1, else n + 1 |
| Die-roll walk ends in [mn, mx] (LC 837 style) | onsite | backward DP with a sliding window sum of the next K probabilities |
| Shortest cycle through a node | onsite | BFS from src without marking src; first time back at src |
| Intern OA (2 easy): grade buckets `[<50, 50–80, >80]`; chapters in order with `evening[]` time per night, whole chapters only, unused time does not carry over, min nights or −1 (`[3,2,1]` + `[2,5,3,3]` → 3; `[5,8,5]` + `[9,4,5,3,8]` → −1) | OA ×2 | one counting pass; pointer on chapters, per night read while the next chapter fits |
| Mahjong winning hand | phone | counts per tile; pick the pair, then greedily remove triplets / sequences |
| Template search in a tree's leaf string | onsite | in-order leaf concatenation, then KMP / find |
| Longest water-flow path with momentum | onsite | DFS with memo over (cell, state) |
| Matrix flower planting | onsite | constraint check per cell; search / matching |
| Uniquely identifiable ranks from match results (determine player rankings) ×2 | phone, L5 onsite | transitive closure (Floyd, or BFS on the win graph and on the reversed one per player); rank known iff wins + losses == n − 1 |
| Ways to pick letters forming "GOOGLE" | phone | C(g, 2) · C(o, 2) · l · e |
| Email anti-spam checksum | OA | per-character lookup modulo a length |
| Game of Life (LC 289) | onsite | in-place two-bit states, then shift |
| Door entry and exit scheduling (LC 2534) | onsite | two queues + who used the door last second |
| Tic-tac-toe with k players, 3 in a row | phone | after each move check 4 directions around it, O(1) per move |
| Detonate the maximum bombs (LC 2101) | onsite | directed graph by distance², BFS from every bomb |
| Longest consecutive path in a binary tree (LC 298 / 549) | phone | DFS returning (inc, dec) lengths |
| Signal matching within a delay window | phone | value → queue of indices in antenna2; match the oldest index within maxDelay |
| Stream word detector (LC 1032) / wildcard rule engine | phone | trie of reversed words over the recent suffix; wildcard → DFS over the pattern trie |
| Two arrays, first k disjoint after deletions | onsite | hash set of list1's first k; keep list2 items not in it |
| Insert + find k-th largest | onsite | sorted vector with binary-search insert, or an order-statistics tree |
| Server class: print requests by end time | onsite | min-heap by end time |
| Guess the word (LC 843) | onsite | pick the guess that minimizes the worst bucket of match counts |
| Cheapest common ancestor in a priced tree | onsite | mark ancestors of one node, walk the other up, track the min price among common ones |
| Dynamic taxi network | onsite | rerun multi-source BFS on change, or BFS from the changed taxi only |
| 3×3 block minimum (skip blocks containing 1) | onsite | loop over block corners, min per block |
| Iterator merging favorites + photos with blocked IDs | onsite | two cursors, skip blocked and seen IDs in hasNext |
| Median of two run-length-encoded arrays | onsite | walk both by runs to the middle count (or binary search on value) |
| String warm-ups: snake_case → camelCase, thousand separators, numbers up to 1000 digits, OCR 1–200 rotation, π self-locating indices, slice by signed offset | various | character loops |
| Deduplicate card-pile prefixes ×3 | FastPrep | hash map value → last pile index; a pile's top k must not hit a value seen in the previous d piles |
| Microwave optimal input ×2 | FastPrep | try all 0–9999 inputs, convert mm:ss (seconds may be ≥ 60), score by error then key count |
| Collinear points ×2 | FastPrep | cross product against the first point distinct from p0; skip duplicates |
| BST to sorted circular doubly linked list (LC 426) ×2 | FastPrep | in-order DFS with `prev` and `head`, link head and tail at the end |
| Balanced parentheses with left deletions ×2 | FastPrep | stack of kept chars; digit d pops d chars, then check balance |
| Rightward grid paths (row −1 / 0 / +1 per column) ×2 | FastPrep | column-by-column DP, ways[c][r] = sum of three neighbors in c − 1 |
| Winning electoral coalitions ×2; task reward in a time budget ×2 | FastPrep | 0/1 knapsack (count subsets with sum > half; max reward), see `select/13-dp/07-partition-equal-subset-sum` |
| Town judge (LC 997) ×2 | FastPrep | trust in / out degree, judge has in = n − 1, out = 0 |
| Fountain safety ×2 | FastPrep | from each fountain walk left / right while heights don't rise; mark covered cells |
| Longest dictionary word from nine letters ×2 | FastPrep | letter counts, check each word's counts ≤ available |
| Match shops and buyers within a distance ×2 | FastPrep | sort both, two pointers: pair if close, else advance the one further left |
| Maximal rectangle in a binary matrix (LC 85) ×2 | FastPrep | per-row histogram + `select/optional/05-stack/04-largest-rectangle-histogram` |
| Minimum direction violations ×2 | FastPrep | 0-1 BFS: forward edge cost 0, reversed edge cost 1, deque |
| Return a book via trains ×2 | FastPrep | multi-source BFS from stations seeded with walk + station-to-library cost (Dijkstra if costs differ) |
| Recover coins from change counts ×2 | FastPrep | ascending a: if ways[a] still > 0 after current coins, a is a coin; run the unbounded-knapsack update for it |
| Select FIRST / LAST / DISTINCT k stream words ×2 | FastPrep | vector + hash set; LAST_DISTINCT scans from the back |
| Grid shortest path with k eliminations (LC 1293) ×2 | FastPrep | state BFS (r, c, used), see `select/09-advanced-graphs/03-grid-obstacles-elimination` |
| Simplify a nested variable expression ×2 | FastPrep | stack of signs for parentheses, accumulate coefficient per variable |
| Sliding window median (LC 480) ×2 | FastPrep | two multisets kept balanced (as in ref 45), erase the outgoing value |
| Text justification (LC 68) ×2 | FastPrep | greedy pack words, spread spaces left-heavy; last line left-aligned |
| Skyline (LC 218) ×2 | FastPrep | sweep x events with a max multiset of active heights |
| Most talkative chat users / most active caller ×2 each | FastPrep | parse with `istringstream`, hash count (word count or distinct-contact set), top k |
| Peak task concurrency intervals ×2; ranges where exactly X run (L5 onsite) | FastPrep, 1p3a | sweep events (ends before starts at a tie), record ranges where the running count equals the max / X (`select/11-intervals-greedy/02-meeting-rooms-ii` note) |
| Ad scheduler with delay ×2 | FastPrep | heap by (score) of ads whose ready time ≤ now; delayed ads in a second heap by ready time |
| Median of two sorted arrays (LC 4), LC tag #3 | LC tag | binary search on the cut of the shorter array, see `select/optional/04-binary-search/06-median-two-sorted-arrays` |
| Longest palindromic substring (LC 5), LC tag #7 | LC tag | expand around each center, see `select/optional/13-dp/09-longest-palindromic-substring` |
| Split array largest sum (LC 410), LC tag #19 | LC tag | binary search on the answer, see `select/optional/04-binary-search/05-split-array-largest-sum` |
| atoi (LC 8) #34, partition equal subset (LC 416) #45, task scheduler (LC 621) #50, edit distance (LC 72) #58, first missing positive (LC 41) #68, largest rectangle (LC 84) #84 | LC tag | already in `select/optional/` |
| Largest number (LC 179), LC tag #43 | LC tag | sort strings with comparator a + b > b + a; all zeros → "0" |
| Substring with concatenation of all words (LC 30), LC tag #53 | LC tag | for each offset in [0, wordLen), sliding window over words with a count map |
| Smallest rectangle enclosing black pixels (LC 302), LC tag #54 | LC tag | binary search on the row / column projections from the given black pixel (or BFS bounding box) |
| Zigzag conversion (LC 6), LC tag #66 | LC tag | one string per row, bounce the row index |
| Longest happy string (LC 1405), LC tag #80 | LC tag | max-heap of (count, char); if the top would make 3 in a row, use the second |
| Maximum score of a good subarray (LC 1793), LC tag #81 | LC tag | two pointers expand from k toward the larger neighbor (same answer as LC 84 with i ≤ k ≤ j) |
| Minimum operations to sort a string (LC 3863), LC tag #89 | LC tag | case analysis: 0 if sorted; −1 if n = 2; 1 if s[0] is min or s[n−1] is max; 2 if min / max appear inside; else 3 |
| Sum of sortable integers (LC 3886), LC tag #96 | LC tag | for each divisor k of n: each block has ≤ 1 circular descent and prefix max ≤ suffix min at block borders |
| LC tag easy warm-ups: palindrome number (9), roman to integer (13), longest common prefix (14), max consecutive ones (485), happy number (202), valid anagram (242), intersection of arrays (349), contains duplicate I / II (217, 219), ransom note (383), climbing stairs (70), Fibonacci (509), Pascal's triangle (118), same tree (100), path sum (112), middle of list (876), queue with two stacks (232), graph valid tree (261, ref 29) | LC tag | hash set / counts, two pointers, simple recursion; type each once if time allows |
| Unit-weight items with limited counts, capacity W (bounded knapsack variant) | 1p3a | every item weighs 1 → take the W most valuable units: sort by value (or quickselect) and take greedily |
| Longest same-color line in a grid (row, column, diagonals) ×2 | 1p3a | four direction DPs: run[r][c][d] = run of the previous cell in direction d + 1 if same color |
| Target expressions from ordered digits with +, * and parentheses; expression add operators (LC 282) | 1p3a ×2 | interval DP: set of values for each digit range, combine left / right sets by + and * |
| Employee hierarchy add / delete (delete manager, CEO, replacement by seniority) | 1p3a | id → node with parent and children set; on delete, re-parent children to the chosen replacement |
| Fence painting with vertical / horizontal strokes | 1p3a | divide and conquer: min(width, minHeight − base + recurse on the parts above the min) |
| Babylon tile-stack game | 1p3a | simulate stacks; win / lose by DFS + memo over the sorted state |
| Jump game with profit (take profit[i] and jump i + 1 + arr[i], or skip) | 1p3a | DP from the right: dp[i] = max(dp[i + 1], profit[i] + dp[i + 1 + arr[i]]) |
| Max-area rectangle from points (any orientation, LC 963 turned into max) ×3 | 1p3a, L4 onsite | group point pairs as diagonals by (midpoint, length); any two diagonals in a group form a rectangle |
| Highway toll from checkpoint logs | 1p3a | sort by (car, time), pair consecutive entry / exit records, fee by distance |
| Tournament champion probability | 1p3a | DP by round: p[r][i] = p[r−1][i] · Σ over possible opponents p[r−1][j] · win(i, j) |
| Monotone increasing digits (LC 738) | 1p3a | from the right, on a drop decrement the left digit and set the rest to 9 |
| Blind maze escape with one fixed command sequence | 1p3a | BFS over the set of possible positions (bitmask), apply each command to all |
| Deliver to all grid locations and return home | 1p3a | BFS distances between targets, then bitmask DP (TSP) over targets |
| deleteDirectory with getChildrenPaths / deleteFile | 1p3a | post-order DFS: delete children first, then the directory; iterative with a stack for deep trees |
| OOD warm-ups: multi-elevator controller, poker game, timer, bookshelf, CompletableFuture | 1p3a | clarify the API first; classes with data members at the top, one method per operation |
| Big-number strings: add / subtract | 1p3a ×2 | digit loop from the right with carry / borrow; compare magnitudes first for subtraction |
| Split into consecutive runs of length 5 (LC 846 style) | 1p3a | count map, start from the smallest value, consume value..value+4 |
| OA: count 4-digit codes with digit sum S ×8 | 1p3a | four nested loops over 0–9 (or digit DP); clarify leading zeros |
| OA: score with points and consecutive 'T' bonuses ×3 | 1p3a | one pass with a running streak counter |
| Shuffle a deck (Fisher–Yates) | 1p3a | for i from n − 1 down to 1, swap a[i] with a[rand() % (i + 1)] |
| Easy warm-ups from 1p3a: shortest word distance (243), custom sort string (791), squares of a sorted array (977), linked list components (817), encode / decode strings (271), even before odd | 1p3a | one pass / two pointers / length-prefix encoding |
| Print zero / even / odd with 3 threads (LC 1116) | 1p3a Q4 ×1 | one mutex + condition variable + a `turn` state; each thread waits until it is its turn, prints, sets the next turn, `notify_all` |
| Lowest-bid auction | 1p3a Q4 ×1 (title only) | likely "lowest unique bid wins": count map + ordered map of bids with count 1; clarify the rule first |
| Path with the minimum product of weights | 1p3a Q4 ×1 | Dijkstra with multiplication when all weights ≥ 1 (or sum of logs), see ref 37 |
| Jump game variant (L4 onsite, "hard", no statement) | 1p3a | start from `select/11-intervals-greedy/05-jump-game`; BFS / DP if jumps have costs |
| Run-length encoded vectors: storage + dot product | 1p3a, L4 (AI infra) | runs (value, count); two pointers take min(remaining counts) per step |
| Wordle oracle (is letter c at position i) | 1p3a, phone | information-gain greedy: pick the query that splits the remaining candidates most evenly |

### Optional — not now: outside NG / intern SWE scope (skip unless everything above is done)

Rows here were reported only in L5+ loops, in MLE / AI-ML loops or in research (PhD / RS)
intern loops on 1point3acres (labels checked 2026-11-11), or need tools outside the usual NG set
(bipartite matching, dynamic connectivity). SWE L3 / L4 / NG / intern reports stay in the table
above. A new report from such a loop goes here, not into `select/` or `ref/`. Idea only.

| Problem | Source | Idea |
|---|---|---|
| Problem-to-programmer skill matching (also FastPrep ×2) | onsite | bipartite matching (augmenting paths) — beyond the usual NG toolkit |
| Fully dynamic friend / unfriend connectivity | onsite, 1p3a (L4 follow-up of ref 06) | simple answer: edge set + BFS per log; fast: offline divide and conquer with rollback union-find, mention only |
| Minimum planks covering roof leaks | onsite | bipartite min vertex cover = max matching (König) — beyond the usual NG toolkit |
| Sum of unit-step subarrays ×2 | 1p3a, MLE L5 round; FastPrep ×2 | for each end, extend the +1 / −1 run: sum contributed = value × count of starts in the run |
| Median of K sorted arrays (LC 4 follow-up) | 1p3a, L5 | binary search on the value, count ≤ x with upper_bound in each array |
| Huffman tree construction | 1p3a, L6 ×3 (L4 ×1) | min-heap of (freq, node), merge the two smallest until one remains |
| Python `itertools.tee` | 1p3a, L5 | shared deque buffer + per-iterator absolute index; drop the buffer prefix all iterators have passed |
| Grouping equal values gives distinct group sizes (LC 1207); consecutive-run follow-up | 1p3a, L5 | count map, then a set of counts; clarify the run rule with examples |
| Necklace of D / R cut into two equal halves with ≤ 2 cuts | 1p3a, L4 AI/ML (MLE) loop | the half-length window over the circular string: D count changes by ≤ 1 per shift, so some window has exactly half the D's |
| Find all bad pairs with runTest(S) | 1p3a, PhD research intern (RS) | divide and conquer: test halves, recurse only into failing parts and cross pairs |
| Two-day log intersection under a memory limit | 1p3a, PhD research intern | hash set per day in memory; otherwise external sort both files by obj_id, merge with two pointers |
| Spreadsheet `set_cell` / `get_cell` with formulas (3 requirements at once) | 1p3a, Applied AI senior | cell → value or formula; `get` = DFS with memo over references, cycle check with an on-path set; `set` invalidates dependents (reverse edges) — ask which part must be finished |
| Swipe typing: words hidden in a swiped string (`"cxsasert"` → cat, sat; `"gokgle"` → google) | 1p3a, onsite in the L5 section | trie + DFS over the input: at each char either match and go down or skip it; a doubled letter may reuse the same char; return a set (no shared `found` member) |
| Longest subarray with max − min ≤ limit (LC 1438) | 1p3a, same loop | sliding window + max deque + min deque, see `select/03-sliding-window/04-sliding-window-max` |
| Matrix path where the next cell is smaller than the current or the previous cell | 1p3a, L5 onsite | DFS + memo; the state is (cell, previous value), not the cell alone |
| Find the edge cases a given piece of code misses, then LC 528 | 1p3a, L5 onsite | read for empty input, one element, duplicates, overflow, off-by-one at both ends; LC 528 = ref 16 |
| Car rental with car ids; Russian doll envelopes (LC 354); meeting rooms III variant; LC 1109-style difference array | 1p3a, L5 / FDE loops | already covered: ref 08, ref 63, ref 09, `select/11-intervals-greedy/09-car-pooling` |
| System design questions (token rate limiter, web crawler, offline-first sync, news aggregator, BigQuery slowest query, customer-service agent, ML SD) | 1p3a, L4+ | not an NG round; notes in `~/Code/interview/system-design/google/README.md` |
