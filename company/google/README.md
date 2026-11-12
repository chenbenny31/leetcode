# Google — prep guide for agents

Read this before helping Siyue prep for Google. It is the only md in this company folder;
don't add others (no per-problem notes).

## Folders

- `prep/` — his own hand-written solutions (followups as `*-followup.cpp`). Never create, edit
  or reformat files here.
- `ref/` — AI reference solutions, `NN-slug.cpp`, numbered in the tier order below and split
  by tier into `must/` and `recommended/`. A new AI solution goes in its tier's folder with the
  next free number and gets a row in the matching tier table. Optional problems have no file;
  they are listed in the Optional table with the one-line idea.
- How a new report is filed: if it reduces to a `select/` core problem, its variant becomes a
  follow-up function in that `select/` file and its weight goes up in `STUDY_ORDER.md` Part A
  (no file here). If it combines two techniques (e.g. prefix sums + binary search), it gets a
  `recommended/` file. If it is niche, it gets an Optional row.

## What we know about the interview

- NG SWE. Problems come from interview reports he collected (three batches pasted on
  2026-11-11, the FastPrep list with sighting counts, and earlier ones); tiers are a prep
  judgment. The top FastPrep counts: car rental assignment ×14 (meeting rooms II), min chairs
  ×9, restaurant waitlist ×8, file system size ×6, max subarray with equal endpoints ×6.
- The most repeated patterns across reports: top-k frequency, meeting rooms II / III and heap
  scheduling, prefix sums + hash map, shortest subarray with k distinct, BFS (multi-source, state BFS), union-find,
  topological order, tries. These live in `select/`; see `STUDY_ORDER.md` Part A.
- Expect follow-ups that ask for a better complexity than the first working solution
  (e.g. O(n^2) → O(n), brute-force count → backtracking).

## How to run a session

1. Pick the next problem in tier order. Skip rows that already have a `prep/` file unless he
   asks to redo one.
2. Start every problem with what to clarify with the interviewer, written in English (input order and ranges,
   duplicates / overlaps, edge cases, what to return), each with the answer the statement
   gives or the assumption to state.
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
- Header, under the problem statement: `Clarify:` (bullets `- question -> stated answer or
  assumption`: ranges and the complexity they force, duplicates / ties, empty input, open vs
  closed bounds, what to return), `Idea:` (2–4 lines), `Pitfalls:` (the concrete bugs in this
  code), `Tests to write:` (the cases in words). Multi-part files label them per part, or put
  a short block above a function that is its own problem.
- Follow-ups are code: a separate tested function when it is a reported variant or differs by
  more than a few lines (its `// follow-up:` line then points at it), otherwise a snippet of up
  to ~6 lines inside the `// follow-up:` comment.
- Main function = the most straightforward passable solution. Add a second function only for a
  substantially better one, and test both in the same `test`. Follow-ups are separate
  functions.
- Comment above each function: approach + `T: O(...), S: O(...)` (graphs in V+E). Comment what
  each key data structure stores. Adjacency list is `graph`; BFS/DFS use `cur` / `nxt`; DFS
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

The `prep/` column names his file(s) for that problem (— = not written yet). Run a ref file
with `g++ -std=c++17 -Wall -Wextra -g -fsanitize=address,undefined ref/NN-slug.cpp -o a && ./a`.

### High-frequency — must do

| # | `ref/` file | Source | Problem | `prep/` |
|---|---|---|---|---|
| 01 | `01-max-ends-score` | interview report | max of first + last − middle over subarrays (Kadane variant) | `01-max-end-score.cpp` |
| 02 | `02-perfect-wave-number` | interview report | zigzag digits check + count in [1, N] by backtracking | — |
| 03 | `03-roster-merge-shifts` | interview report | merge shift roster into [start, end, names] rows incl. gaps (cut points; sweep) | — |
| 04 | `04-ad-service` | interview report | AdService: highest score, no repeat in a row, score −1 (scan; max-heap) | — |
| 05 | `05-string-substitution` | interview report | replace %KEY% from a dict; follow-up nested values + cycle check (DFS) | — |
| 06 | `06-magical-string-pairs` | interview report | count (i, j, k) with equal letter sets on both sides (O(n²) masks; 26-step chains) | — |
| 07 | `07-piano-hand-moves` | interview report | group notes with max − min ≤ 4, count hand moves + fingers (greedy) | — |
| 08 | `08-tower-profit` | interview report | tower height vs houses covered, max profit (sort distances; integer-height follow-up) | — |

### Recommended — reported, combine two techniques

| # | `ref/` file | Source | Problem | `prep/` |
|---|---|---|---|---|
| 09 | `09-lr-pieces-move` | phone ×4, LC tag #21 | LC 2337 / LC 777: move L / R pieces into blanks (two pointers); walls follow-up | — |
| 10 | `10-meeting-rooms-iii` | onsite ×6 | LC 2402 (free-room heap + busy heap), CPU tasks with k CPUs, min cars for orders | — |
| 11 | `11-server-bfs-series` | phone ×4 | BFS distance, reachability avoiding failed servers, all distances, failure radius | — |
| 12 | `12-grid-shortest-path-one-break` | phone, onsite ×3 | multi-source BFS left→right column with path; maze BFS; one wall break = state BFS | — |
| 13 | `13-train-earliest-arrival` | onsite ×2 | train / flight route feasibility with times: sort by departure, earliest arrival per station | — |
| 14 | `14-nested-function-eval` | onsite ×2, LC tag #73 (LC 3749) | evaluate add(1, mul(2, sub(5, 3))): recursive descent with a shared index | — |
| 15 | `15-boolean-tree-leaf-flip` | onsite ×2 | which single leaf flips change the root (values up, "matters" down); flips in O(height) | — |
| 16 | `16-file-system-size` | phone, onsite ×2 | directory size by DFS; cached sizes updated along parent pointers | — |
| 17 | `17-locker-system` | phone ×2 | smallest free locker: min-heap of free lockers + customer → locker map | — |
| 18 | `18-next-word-predictor` | phone ×2 | bigram counts: most frequent successor; weighted sampling with prefix sums + upper_bound | — |
| 19 | `19-top-k-pair-sums` | onsite | K largest nums[i] + nums[j]: sorted desc, heap expanding (i, j+1), (i+1, j) with visited | — |
| 20 | `20-belt-packer-triples` | onsite ×3 | stream of sizes, remove three with max − min < threshold: multiset, check neighbor windows | — |
| 21 | `21-interval-membership` | phone, LC tag #99 | disjoint intervals, is x covered: scan; map start → end + upper_bound; Range Module (LC 715) | — |
| 22 | `22-max-subarray-equal-endpoints` | onsite, FastPrep ×6, LC tag #94 | max sum of a[i..j] with a[i] == a[j]: prefix sums + hash map of best start per value; LC 3026 (\|a[i] − a[j]\| == k) | — |
| 23 | `23-alternating-parity-queries` | onsite ×3 | does a[l..r] alternate parity: prefix count of same-parity adjacent pairs | — |
| 24 | `24-nondecreasing-one-replacement` | onsite ×2 | longest non-decreasing subarray after one replacement: left / right run arrays | — |
| 25 | `25-validate-tree` | onsite ×3 | tree from a parent array; undirected edges form a binary tree, pick the root | — |
| 26 | `26-people-with-secret` | phone | LC 2092: union-find per timestamp group, reset people not linked to 0 | — |
| 27 | `27-recover-doubled-array` | onsite ×2 | double + shuffle; recover the original: sort by abs, count map, pair x with 2x | — |
| 28 | `28-count-square-submatrices` | onsite | LC 1277 / LC 221: dp = 1 + min(up, left, diag), sum or max | — |
| 29 | `29-rpn-to-infix` | phone ×2 | RPN → infix: stack of strings; minimal parentheses by precedence and associativity | — |
| 30 | `30-restaurant-waitlist` | FastPrep ×8 | join / leave / seat earliest party that fits: list scan; buckets by size of (order, id) sets; exact-size tables |
| 31 | `31-min-root-leaf-cut-cost` | FastPrep ×5 | cut edges so the root reaches no leaf: cost(node) = Σ min(w, cost(child)), leaf = ∞ |
| 32 | `32-logger-dedup` | FastPrep ×4 + ×3 | out-of-order rate limiter (map → set of printed times, check neighbors); drop messages with a copy within the window (two passes; streaming queue) |
| 33 | `33-count-wrapped-lines` | FastPrep ×4 + ×2 | greedy word wrap line count; two columns: try every left width, min of max heights |
| 34 | `34-horizontal-bisect-squares` | FastPrep ×3 | LC 3453: binary search on y over doubles; exact sweep over sorted edges |
| 35 | `35-connected-crop-garden` | FastPrep ×3 | k crops as connected regions: fill along the snake order; stacked squares follow-up |
| 36 | `36-sliding-window-trimmed-average` | FastPrep ×3 + ×2 | window average without the k largest: two multisets + running sum; trim both ends |
| 37 | `37-top-rated-similar-movies` | FastPrep ×3 | BFS over the similarity graph, then size-k heap on (rating, id) |
| 38 | `38-merge-trees-by-name` | FastPrep ×3 | merge two ordered trees: same-name children merge recursively, others copied in order |
| 39 | `39-water-drop-destinations` | FastPrep ×3 | each drop flows to the lowest lower neighbor: simulate; memoized DFS |
| 40 | `40-two-travelers-shared-route` | FastPrep ×3 | LC 2203 style: BFS from a, b and dest, min over meeting city of the three distances |
| 41 | `41-longest-duplicate-substring` | FastPrep ×3 | LC 1044: binary search on length + Rabin-Karp, verify hash hits with compare |
| 42 | `42-count-one-valued-tree-regions` | FastPrep ×3 | count 1-regions in a tree: a 1-node whose parent is 0 (or root) starts a region |
| 43 | `43-directed-reachability-queries` | FastPrep ×3 | BFS per query; precompute reach[u][v] by BFS from every node; SCC condensation note |
| 44 | `44-random-pick-weight` | LC tag #48, FastPrep ×3 | LC 528: prefix sums + upper_bound on a draw in [0, total); recorded-draws variant |
| 45 | `45-safest-path-grid` | LC tag #64, phone | LC 2812: multi-source BFS distances, then maximin Dijkstra; binary search + BFS |
| 46 | `46-count-zero-request-servers` | LC tag #22 | LC 2747: sort logs and queries, slide a time window with a server count map |

### Optional — reported but niche (no file; idea only)

| Problem | Source | Idea |
|---|---|---|
| LR pieces on a circular board (LC 777 follow-up) | phone | cut the circle at an empty cell, then the linear check of rec 09 |
| Org hierarchy: manager / peer / is_manager | onsite | union-find of peer classes, one manager pointer per class, reject conflicts and cycles by walking up |
| Belt packer with 2D sizes | onsite | sort by x, only items with abs(dx) < threshold are candidates (rec 20 follow-up) |
| Data chunking (equal chunks; indivisible header) | onsite | chunks = ceil(data / max); first `data % chunks` get base + 1; header variant fills the header chunk first |
| Sudoku solver (LC 37) | onsite | backtracking with row / column / box used flags |
| Nth license plate (letters then digits) | onsite | mixed radix: convert N − 1 with base 10 for digits, base 26 for letters |
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
| Exam score / book reading schedule | OA | counting; greedy chapter-by-chapter simulation |
| Sum of distances in a (weighted) tree (LC 834) ×2 | onsite | rerooting: subtree sizes + one down pass, ans[child] = ans[parent] + w·(n − 2·size) |
| Mahjong winning hand | phone | counts per tile; pick the pair, then greedily remove triplets / sequences |
| Template search in a tree's leaf string | onsite | in-order leaf concatenation, then KMP / find |
| Problem-to-programmer skill matching | onsite | bipartite matching (augmenting paths) |
| Longest water-flow path with momentum | onsite | DFS with memo over (cell, state) |
| Matrix flower planting | onsite | constraint check per cell; search / matching |
| Uniquely identifiable ranks from match results | phone | transitive closure; rank known iff wins + losses == n − 1 |
| Ways to pick letters forming "GOOGLE" | phone | C(g, 2) · C(o, 2) · l · e |
| Fully dynamic friend / unfriend connectivity | onsite | offline divide and conquer with rollback union-find (mention only) |
| Email anti-spam checksum | OA | per-character lookup modulo a length |
| Game of Life (LC 289) | onsite | in-place two-bit states, then shift |
| Door entry and exit scheduling (LC 2534) | onsite | two queues + who used the door last second |
| Tic-tac-toe with k players, 3 in a row | phone | after each move check 4 directions around it, O(1) per move |
| Detonate the maximum bombs (LC 2101) | onsite | directed graph by distance², BFS from every bomb |
| Longest consecutive path in a binary tree (LC 298 / 549) | phone | DFS returning (inc, dec) lengths |
| Signal matching within a delay window | phone | value → queue of indices in antenna2; match the oldest index within maxDelay |
| Stream word detector (LC 1032) / wildcard rule engine | phone | trie of reversed words over the recent suffix; wildcard → DFS over the pattern trie |
| Two arrays, first k disjoint after deletions | onsite | hash set of list1's first k; keep list2 items not in it |
| Minimum planks covering roof leaks | onsite | bipartite min vertex cover = max matching (König) |
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
| Winning electoral coalitions ×2; task reward in a time budget ×2 | FastPrep | 0/1 knapsack (count subsets with sum > half; max reward), see `select/optional/13-dp/07-partition-equal-subset-sum` |
| Town judge (LC 997) ×2 | FastPrep | trust in / out degree, judge has in = n − 1, out = 0 |
| Fountain safety ×2 | FastPrep | from each fountain walk left / right while heights don't rise; mark covered cells |
| Longest dictionary word from nine letters ×2 | FastPrep | letter counts, check each word's counts ≤ available |
| Match shops and buyers within a distance ×2 | FastPrep | sort both, two pointers: pair if close, else advance the one further left |
| Maximal rectangle in a binary matrix (LC 85) ×2 | FastPrep | per-row histogram + `select/optional/05-stack/04-largest-rectangle-histogram` |
| Minimum direction violations ×2 | FastPrep | 0-1 BFS: forward edge cost 0, reversed edge cost 1, deque |
| Return a book via trains ×2 | FastPrep | multi-source BFS from stations seeded with walk + station-to-library cost (Dijkstra if costs differ) |
| Recover coins from change counts ×2 | FastPrep | ascending a: if ways[a] still > 0 after current coins, a is a coin; run the unbounded-knapsack update for it |
| Select FIRST / LAST / DISTINCT k stream words ×2 | FastPrep | vector + hash set; LAST_DISTINCT scans from the back |
| Grid shortest path with k eliminations (LC 1293) ×2 | FastPrep | state BFS (r, c, used), see `select/optional/09-advanced-graphs/03-grid-obstacles-elimination` |
| Simplify a nested variable expression ×2 | FastPrep | stack of signs for parentheses, accumulate coefficient per variable |
| Sliding window median (LC 480) ×2 | FastPrep | two multisets kept balanced (as in rec 36), erase the outgoing value |
| Sum of unit-step subarrays ×2 | FastPrep | for each end, extend the +1 / −1 run: sum contributed = value × count of starts in the run |
| Text justification (LC 68) ×2 | FastPrep | greedy pack words, spread spaces left-heavy; last line left-aligned |
| Skyline (LC 218) ×2 | FastPrep | sweep x events with a max multiset of active heights |
| Most talkative chat users / most active caller ×2 each | FastPrep | parse with `istringstream`, hash count (word count or distinct-contact set), top k |
| Peak task concurrency intervals ×2 | FastPrep | sweep events, record ranges where the running count equals the max (`select/11-intervals-greedy/02-meeting-rooms-ii` note) |
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
| LC tag easy warm-ups: palindrome number (9), roman to integer (13), longest common prefix (14), max consecutive ones (485), happy number (202), valid anagram (242), intersection of arrays (349), contains duplicate I / II (217, 219), ransom note (383), climbing stairs (70), Fibonacci (509), Pascal's triangle (118), same tree (100), path sum (112), middle of list (876), queue with two stacks (232), graph valid tree (261, rec 25) | LC tag | hash set / counts, two pointers, simple recursion; type each once if time allows |
