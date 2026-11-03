# Amazon — prep guide for agents

Read this before helping Siyue prep for Amazon. It is the only md in this company folder;
don't add others (no per-problem notes).

## Folders

- `prep/` — his own hand-written solutions (create it when he writes the first one). Never
  create, edit or reformat files there.
- `ref/` — AI reference solutions for interview (phone / onsite) problems, `NN-slug.cpp`,
  `01`–`101`, numbered in the tier order below and split by tier: `must/` 01–09,
  `recommended/` 10–66, `optional/` 67–101.
- `oa/` — AI reference solutions for OA-only problems, `01`–`24`, numbered separately (see
  the `oa/` section at the end). His own OA attempts stay in `prep/oa/`. A new AI solution goes in its tier's
  folder at its priority position (renumber the files after it) and gets a row in the
  matching tier table.
- Non-coding material lives in `~/Code/interview`: `AMAZON_NG_PREP.md` (LP stories, OOD,
  plan) and `amazon/system-design/` (the 10 hack2hire Amazon design write-ups).

## What we know about the interview

- Amazon SDE new grad loop on 2026-11-09. Rounds are usually coding plus 15–20 minutes of
  Leadership Principles; there may be an OOD round.
- Sources: hack2hire Amazon questions and jobright.ai pastes (both tagged `h2h`), fastprep.io
  sighting lists with counts (`fp ×n`, Nov 2026), and the LeetCode Amazon 30-day frequency
  list (`LC 30d`, Oct 2026). Tiers: must = onsite / phone with 3+ sightings; recommended =
  other onsite / phone reports; optional = no-round reports and the 30-day list; OA-only problems live in `oa/`.
- Several hack2hire examples are wrong (they contradict their own statement). The refs
  follow the statement and use hand-made tests.

## How to run a session

1. Pick the next problem in tier order (high-frequency → recommended → optional). Skip rows
   that already have a `prep/` file unless he asks to redo one.
2. Start every problem with what to clarify with the interviewer, written in English (input order and ranges,
   duplicates / overlaps, edge cases, what to return), each with the answer the statement
   gives or the assumption to state.
3. One problem at a time: a very concise derivation (which data structure, why it is
   efficient) → T/S for the crucial functions → the full code. Prefer the version that is easiest
   to follow line by line (e.g. prefix sums + binary search over a two-pointer with special
   cases) when both meet the constraints.
4. "check" = review his file in `prep/`: compile with `g++ -std=c++17 -Wall -Wextra`, run its
   own tests, and report only errors (say whether it is a compile failure or a test failure).
   No praise, no summaries. Never edit his file — show the fix in chat and he types it.
5. Don't run tests for solutions you only show in chat; he tests them himself.

## Code conventions (`ref/` files and any code shown in chat)

- One runnable file: header comment stating the problem, includes, one blank line,
  `using std::x;` for every type and function used (no `std::` in the body), solution,
  `test(label, input..., want)` (or `check(ok, label)` for design problems), `main`.
- Main function = the most straightforward passable solution. Add a second function only for a
  substantially better one, and test both in the same `test`. Follow-ups are separate
  functions without tests.
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
  words on whitespace; use a character loop when the format has other delimiters. Keep lines ≤ 90 columns (move trailing
  comments above the line).
- C++17: use `auto` and structured bindings where the type is obvious, e.g.
  `auto [r, c] = q.front();` (a copy, never `auto&` before `pop()`), `for (auto& [k, v] : m)`.
- Minimal: implement only what the statement requires. Ignore hack2hire "insider tips".

## Tiers

`h2h` = hack2hire / jobright.ai report (×n = reported n times), `fp` = fastprep.io sightings,
`LC 30d` = LeetCode Amazon 30-day list. The `prep/` column names his file for that problem (— = not written yet).

### Must do — onsite / phone, 3+ sightings

| # | `ref/` file | Source | Problem | `prep/` |
|---|---|---|---|---|
| 01 | `01-max-coins-k-bags` | fp ×22 (OA · phone), LC 3413 | "Find Maximum Total Amount": max total in k consecutive bags, mod 1e9+7 | `01-find-max-total.cpp` |
| 02 | `02-currency-conversion` | fp ×5 | multi-hop exchange rate with reverse edges, 2 decimals (BFS) | `02-currency-conv.cpp` |
| 03 | `03-oldest-one-time-visitor` | h2h ×2, fp ×1, LC 1429 | earliest user with exactly one login (+ X oldest) | `03-oldest-visitor.cpp` |
| 04 | `04-longest-waiting-unique-task` | fp ×2 (phone) | first id seen exactly once (+ offline time queries); same family as 03 | `04-longest-waiting-uniq-task.cpp` |
| 05 | `05-sliding-window-max-ds` | h2h onsite, fp ×3 (LC 239) | last-k max data structure (monotonic deque) | `05-sliding-window-max.cpp` |
| 06 | `06-merge-sorted-arrays-variants` | h2h phone ×3, fp ×1, LC 88 | merge in place (+ descending, unsorted) | `06-merge-sorted-array.cpp` |
| 07 | `07-streaming-top-k` | fp ×3 | online add(v) / top() class, ties by smaller value (ordered set) | `07-stream-top-k-freq.cpp` |
| 08 | `08-package-dependency-order` | fp ×3 | install order for one target, [] on a cycle (post-order DFS) | `08-package-dep-order.cpp` |
| 09 | `09-node-relationship` | h2h onsite, fp ×2 | siblings / cousins / others | `node-relation.cpp` |

Already in the repo (same tier — review, don't redo):

| Problem | Source | File |
|---|---|---|
| LC 56 Merge Intervals | fp ×12 (OA · phone · onsite), LC 30d, h2h | `company/apple/ref/must/09-merge-intervals.cpp` |
| LC 994 Rotting Oranges ("Minimum Time to Spread Through a Grid") | fp ×7, LC 30d, h2h | `company/bytedance/ref/optional/118-rotting-oranges.cpp` |
| LC 767 Reorganize String | fp ×6, LC 30d, h2h | `topic/trie-heap/ref/767_reorganize-string.cpp` |
| LC 146 LRU Cache ("LRU Cache for Query Results") | fp ×6, LC 30d, h2h | `company/apple/ref/must/10-lru-cache.cpp` |
| LC 200 Number of Islands (+ LC 695 max area) | fp ×4 (+ ×1 max area), LC 30d, h2h | `company/apple/ref/must/11-number-of-islands.cpp`, `topic/graphs/ref/695_max-area-island.cpp` |
| LC 49 Group Anagrams (count key) | fp ×4, LC 30d, h2h ×2 | `company/apple/ref/must/13-group-anagrams.cpp` |
| LC 295 Median from Data Stream | fp ×4, h2h onsite | `company/apple/ref/recommended/35-median-stream.cpp` |
| LC 252 / 253 Meeting Rooms | fp ×4, LC 30d, h2h ×2 | `company/bytedance/ref/must/08-meeting-rooms.cpp` |
| LC 3 Longest Substring Without Repeating | fp ×3, LC 30d, h2h ×2 | `company/apple/ref/must/08-longest-substring-no-repeat.cpp` |
| LC 347 Top K Frequent | fp ×2 (OA · phone), LC 30d, h2h ×2 | `company/apple/ref/must/14-top-k-frequent.cpp` |
| LC 139 Word Break | fp ×3 | `topic/dp/ref/139_word-break.cpp` |
| LC 42 Trapping Rain Water | fp ×3, LC 30d | `company/snowflake/ref/recommended/56-trapping-rain-water.cpp` |
| LC 23 Merge k Sorted Lists | fp ×3 | `company/apple/ref/recommended/17-merge-k-sorted-lists.cpp` |
| LC 474 Ones and Zeroes ("Largest Binary-String Subset") | fp ×2, h2h phone | `topic/knapsack/ref/474_ones-and-zeroes.cpp` |
| LC 20 Valid Parentheses | fp ×2, LC 30d, h2h | `company/snowflake/ref/optional/30-valid-parentheses.cpp` |
| LC 212 Word Search II | fp ×2, h2h onsite | `company/snowflake/ref/must/12-word-search-ii.cpp` |
| LC 1 Two Sum (+ follow-ups) | fp ×1, LC 30d, h2h ×2 | `company/apple/ref/must/07-two-sum.cpp` |

### Recommended — other onsite / phone reports (1–2 sightings)

Order: 2 sightings first, then 1 (hack2hire onsite / phone, then fastprep, 2025 reports last).

| # | `ref/` file | Source | Problem | `prep/` |
|---|---|---|---|---|
| 10 | `10-freq-stack` | h2h phone ×2, LC 895 | maximum frequency stack | — |
| 11 | `11-file-search-api` | h2h onsite (OOD), fp ×1 | `find` with composable filters | — |
| 12 | `12-linked-list-queue` | h2h onsite, fp ×1 | linked-list queue (+ delete, dedupe) | — |
| 13 | `13-robot-charging-monitor` | h2h phone, fp ×1 | assign / release charging stations each tick | — |
| 14 | `14-hierarchical-comments` | h2h onsite, fp ×1 | print a comment tree with indentation | — |
| 15 | `15-point-payment-min` | h2h phone, fp ×1 | bounded knapsack, min points for exact payment | — |
| 16 | `16-tree-from-descriptions` | h2h onsite, fp ×1, LC 2196 | build a binary tree from parent/child rows | — |
| 17 | `17-complete-components` | h2h phone, fp ×1, LC 2685 | count complete connected components | — |
| 18 | `18-array-continuous-min-ops` | h2h onsite, fp ×1, LC 2009 | min replacements to make an array continuous | — |
| 19 | `19-make-change` | h2h onsite, fp ×1 | greedy change in cents (+ limited drawer DP) | — |
| 20 | `20-caesar-cipher` | h2h onsite, fp ×1 | Caesar shift (+ lookup table) | — |
| 21 | `21-permutations-circular` | h2h onsite, fp ×1 | all line orders (+ circular, (n-1)!) | — |
| 22 | `22-islands-streaming` | h2h follow-up, fp ×1 | island count with rows streamed (union-find) | — |
| 23 | `23-sliding-window-rate-limiter` | fp ×2 (onsite) | 100 requests per user per 60 s (deque per user) | — |
| 24 | `24-page-referrer-reachability` | fp ×2 (onsite) | target reachable from start via referrer edges (BFS) | — |
| 25 | `25-dag-completion-time` | fp ×2, LC 2050 | min finish time, unlimited parallel tasks (Kahn) | — |
| 26 | `26-lca-stack` | fp ×2 (onsite), LC 236 | LCA iteratively: stack DFS parent map + ancestor set | — |
| 27 | `27-cousins-in-binary-tree-ii` | fp ×2 (onsite), LC 2641 | replace each value with its cousins' sum (level sums) | — |
| 28 | `28-website-visit-pattern` | fp ×2, LC 1152 | 3-site pattern visited by the most users | — |
| 29 | `29-lca-inserted-bst` | fp ×1, h2h phone, LC 235 | BST from insertion order, then LCA (+ O(n) first-in-range) | — |
| 30 | `30-top-k-slowest-logs` | h2h onsite | top-k durations, stable ties (heap) | — |
| 31 | `31-stock-variants` | h2h onsite | buy/sell stock: single, unlimited, fee in one function | — |
| 32 | `32-service-shutdown-impact` | h2h onsite | affected services in a dependency DAG (BFS) | — |
| 33 | `33-music-player` | h2h onsite | most frequent unplayed song with cycle reset | — |
| 34 | `34-sliding-median-reviews` | h2h onsite | median of the last N ratings | — |
| 35 | `35-log-parser` | h2h phone | parse logs, filter / count / top-k | — |
| 36 | `36-package-weights-adjacent-sum` | h2h phone | order digits so neighbors sum < 10 (+ O(n)) | — |
| 37 | `37-domain-score` | h2h onsite | leaf domain score = own + ancestors | — |
| 38 | `38-consecutive-like-k` | h2h onsite | longest chain with gaps < k (LC 128 general) | — |
| 39 | `39-in-place-sort` | h2h onsite | insertion sort vs quicksort | — |
| 40 | `40-merge-dicts` | h2h onsite | merge two / k dictionaries | — |
| 41 | `41-dedup-events` | h2h onsite | latest per id / per-id history | — |
| 42 | `42-log-field-groups` | h2h phone | parse `ts\|k=v;...` lines into field groups | — |
| 43 | `43-aggregate-log-counts` | h2h phone | actions per user, sorted | — |
| 44 | `44-weighted-lottery` | h2h onsite | k weighted winners without replacement | — |
| 45 | `45-package-installer` | h2h onsite | install with dependencies, cycle check, payload | — |
| 46 | `46-delivery-route-planning` | h2h onsite | pickups before drop-offs, capacity (BFS + DP) | — |
| 47 | `47-nested-get-path` | h2h onsite | `get(obj, "a[0].b")` on nested maps / lists | — |
| 48 | `48-morse-code-words` | h2h onsite, LC 804 | unique Morse transformations | — |
| 49 | `49-first-unique-char` | fp ×1 (onsite), h2h OA, LC 387 | first unique character | — |
| 50 | `50-one-package-all-products` | fp ×1 (phone) | one package that fits every product (per-dimension max) | — |
| 51 | `51-max-move-score` | fp ×1 (phone) | split into moves / 1-point whiffs, max score (DP) | — |
| 52 | `52-active-graph-connectivity` | fp ×1 (onsite) | start's component + edges that connect the rest | — |
| 53 | `53-assign-partitions-least-loaded` | fp ×1 (onsite) | each partition to the least-loaded server (min-heap) | — |
| 54 | `54-loyal-customers` | fp ×1 (onsite) | seen both days with ≥ 2 distinct products (sets) | — |
| 55 | `55-largest-stepping-number` | fp ×1 (onsite) | largest number ≤ limit with adjacent digits ±1 (BFS) | — |
| 56 | `56-open-the-lock` | fp ×1 (phone), LC 752 | min wheel turns avoiding deadends (BFS) | — |
| 57 | `57-longest-subarray-k-distinct` | fp ×1 (onsite), LC 340 | longest subarray with ≤ k distinct (sliding window) | — |
| 58 | `58-stock-span` | fp ×1 (onsite), LC 901 | span of each day's price (monotonic stack) | — |
| 59 | `59-max-chunks-sorted-ii` | fp ×1 (phone), LC 768 | most chunks that sort independently (stack of maxima) | — |
| 60 | `60-nary-tree-max-depth` | fp ×1 (onsite), LC 559 | depth of an n-ary tree (BFS levels) | — |
| 61 | `61-pascal-triangle` | fp ×1 (onsite), LC 118 | first n rows of Pascal's triangle | — |
| 62 | `62-lca-parent-pointers` | fp ×1 (onsite), LC 1650 | LCA with parent links (+ O(1) two pointers) | — |
| 63 | `63-vertical-order-traversal` | fp ×1 (onsite), LC 987 | columns, sorted by row then value | — |
| 64 | `64-csv-ascii-table` | fp ×1 (phone, 2025) | CSV rows as a padded pipe table | — |
| 65 | `65-last-seen-anagram` | fp ×1 (onsite, 2025) | newest earlier anagram per word (letter-count key) | — |
| 66 | `66-expiring-key-set` | fp ×1 (onsite, 2025) | add / contains / remove with TTL (map + min-heap) | — |

Also reported (fastprep / h2h) and already in the repo — review, don't redo:

| Problem | Source | File |
|---|---|---|
| LC 127 Word Ladder | fp ×2 (onsite) | `company/bytedance/ref/recommended/18-word-ladder.cpp` |
| LC 277 Find the Celebrity | fp ×2 (onsite) | `topic/conditional/ref/277_find-celebrity.cpp` |
| LC 72 Edit Distance (reverse follow-up: discuss only) | fp ×1, h2h phone | `company/apple/ref/recommended/34-edit-distance.cpp` |
| LC 210 / 207 Topological sort | LC 30d, h2h | `topic/graphs/ref/210_course-schedule-ii.cpp` |
| LC 138 Copy List with Random Pointer | LC 30d, h2h | `company/snowflake/ref/optional/26-copy-random-list.cpp` |
| LC 198 House Robber | LC 30d, h2h | `company/apple/ref/recommended/36-house-robber.cpp` |
| LC 232 Queue Using Two Stacks | fp ×1 | `topic/design/ref/232_queue-using-stacks.cpp` |
| LC 2385 Time to Infect a Binary Tree | fp ×1 | `company/bytedance/ref/optional/134-infected-tree-time.cpp` |
| LC 113 Path Sum II | fp ×1 | `company/bytedance/ref/must/11-path-sum-ii.cpp` |
| LC 22 Generate Parentheses | fp ×1 | `topic/stack/22_generate-parentheses.cpp` |
| LC 64 Minimum Path Sum | fp ×1 | `topic/dp/ref/64_min-path-sum.cpp` |
| LC 142 Linked List Cycle II (to break it, set the last node's next to null) | fp ×1 | `topic/linked-list/ref/142_list-cycle-ii.cpp` |
| LC 62 Unique Paths | fp ×1 | `topic/dp/ref/62_unique-paths.cpp` |
| LC 242 Valid Anagram (count array, no sort) | fp ×1 | `topic/arrays-hashing/242_valid-anagram.cpp` |
| LC 54 Spiral Matrix | fp ×1 | `topic/bit-math-matrix/ref/54_spiral-matrix.cpp` |
| LC 3 on an int array ("Longest Subarray Without Repeated Values") | fp ×1 | `topic/sliding-window/3_longest-substring-no-repeat.cpp` |

Already in the repo (LC 30-day list and older reports — review, don't redo):

| Problem | File |
|---|---|
| LC 4 | `topic/binary-search/ref/4_median-two-sorted-arrays.cpp` |
| LC 2 | `topic/linked-list/ref/2_add-two-numbers.cpp` |
| LC 5 | `company/apple/ref/recommended/31-longest-palindromic-substring.cpp` |
| LC 11 | `topic/two-pointers/11_container-with-most-water.cpp` |
| LC 15 | `company/apple/ref/recommended/30-three-sum.cpp` |
| LC 22 | `topic/stack/22_generate-parentheses.cpp` |
| LC 23 | `company/apple/ref/recommended/17-merge-k-sorted-lists.cpp` |
| LC 36 | `company/apple/ref/recommended/18-valid-sudoku.cpp` |
| LC 128 | `company/apple/ref/recommended/41-longest-consecutive.cpp` |
| LC 238 | `topic/arrays-hashing/238_product-except-self.cpp` |
| LC 704 | `topic/binary-search/ref/704_binary-search.cpp` |
| LC 875 | `topic/binary-search/ref/875_koko-bananas.cpp` |
| LC 1004 | `topic/sliding-window/ref/1004_max-consecutive-ones-iii.cpp` |
| LC 8 | `company/apple/ref/optional/48-string-to-integer-atoi.cpp` |
| LC 32 | `specialty/32_longest-valid-parentheses.cpp` |
| LC 53 | `topic/greedy-intervals/ref/53_max-subarray.cpp` |
| LC 139 / 140 | `topic/dp/ref/139_word-break.cpp`, `topic/dp/ref/140_word-break-ii.cpp` |
| LC 141 | `topic/linked-list/ref/141_list-cycle.cpp` |
| LC 152 | `topic/dp/ref/152_max-product-subarray.cpp` |
| LC 219 | `topic/sliding-window/219_contains-duplicate-ii.cpp` |
| LC 273 | `topic/parser/ref/273_integer-to-english.cpp` |
| LC 316 | `topic/stack/316_remove-duplicate-letters.cpp` |
| LC 540 | `topic/binary-search/ref/540_single-element-sorted.cpp` |
| LC 18 | `topic/two-pointers/18_four-sum.cpp` |
| LC 19 | `topic/linked-list/ref/19_remove-nth-from-end.cpp` |
| LC 21 | `topic/linked-list/ref/21_merge-two-lists.cpp` |
| LC 29 | `topic/bit-math-matrix/ref/29_divide-two-integers.cpp` |
| LC 33 | `topic/binary-search/ref/33_search-rotated-array.cpp` |
| LC 46 | `topic/backtracking/ref/46_permutations.cpp` |
| LC 51 | `topic/backtracking/ref/51_n-queens.cpp` |
| LC 70 | `topic/dp/ref/70_climbing-stairs.cpp` |
| LC 74 (fp ×1) | `topic/binary-search/ref/74_search-2d-matrix.cpp` |
| LC 122 | `topic/dp/ref/122_stock-ii.cpp` |
| LC 133 | `topic/graphs/ref/133_clone-graph.cpp` |
| LC 136 | `topic/bit-math-matrix/ref/136_single-number.cpp` |
| LC 151 | `topic/parser/ref/151_reverse-words.cpp` |
| LC 169 | `topic/arrays-hashing/169_majority-element.cpp` |
| LC 206 | `company/apple/ref/recommended/20-reverse-linked-list.cpp` |
| LC 303 | `topic/arrays-hashing/303_range-sum-query.cpp` |
| LC 399 | `topic/advanced-graphs/ref/399_evaluate-division.cpp` |
| LC 402 | `topic/stack/ref/402_remove-k-digits.cpp` |
| LC 410 (fp ×1, "Allocate Minimum Pages") | `topic/binary-search/ref/410_split-array-largest-sum.cpp` |
| LC 424 | `topic/sliding-window/424_longest-repeating-char-replace.cpp` |
| LC 621 | `company/apple/ref/recommended/33-task-scheduler.cpp` |
| LC 863 | `topic/trees/ref/863_nodes-distance-k.cpp` |
| LC 14 | `company/apple/ref/recommended/16-longest-common-prefix.cpp` |
| LC 283 | `company/apple/ref/recommended/37-move-zeroes.cpp` |
| LC 235 (see `lca-inserted-bst`) | `topic/trees/ref/235_lca-bst.cpp` |
| LC 323 (fp ×1) | `topic/graphs/ref/323_connected-components.cpp` |
| LC 394 | `topic/stack/ref/394_decode-string.cpp` |
| LC 135 | `topic/greedy-intervals/ref/135_candy.cpp` |
| LC 217 | `topic/arrays-hashing/217_contains-duplicate.cpp` |
| LC 904 | `topic/sliding-window/ref/904_fruit-into-baskets.cpp` |
| LC 735 | `topic/stack/ref/735_asteroid-collision.cpp` |
| LC 417 (one-ocean variant) | `topic/graphs/ref/417_pacific-atlantic.cpp` |

### Optional

Do these only after must + recommended. Order: hack2hire with no round stated, then the
LeetCode 30-day list (often contest / OA problems). OA-only reports are in `oa/`.

#### hack2hire, round not stated

| # | `ref/` file | Source | Problem | `prep/` |
|---|---|---|---|---|
| 67 | `67-maximum-interval-overlap` | h2h | max closed intervals sharing a point | — |
| 68 | `68-max-consecutive-on-servers` | h2h | ≤ k range flips, longest run of 1s | — |
| 69 | `69-organize-books-to-shelves` | h2h | least-loaded shelf in a prefix (segment tree) | — |
| 70 | `70-find-smallest-hub` | h2h | smallest-capacity free hub | — |
| 71 | `71-detect-abusive-words` | h2h | obfuscated word matching (trie) | — |
| 72 | `72-connect-points-2d` | h2h | points connected by row/column moves (BFS) | — |
| 73 | `73-block-string-assembly` | h2h | spell target from distinct blocks (matching) | — |
| 74 | `74-smallest-lex-palindrome` | h2h | smallest palindrome from all characters | — |
| 75 | `75-string-reorganization-all` | h2h (LC 767 variant) | all rearrangements with no equal neighbors | — |

#### LeetCode 30-day list

| # | `ref/` file | Source | Problem | `prep/` |
|---|---|---|---|---|
| 76 | `76-max-k-sort-permutation` | LC 30d #4 (3644) | max k to sort a permutation by AND swaps | — |
| 77 | `77-min-cost-equal-chars` | LC 30d (2712) | prefix/suffix flips to equalize a binary string | — |
| 78 | `78-max-white-tiles` | LC 30d (2271) | carpet covering most white tiles | — |
| 79 | `79-peak-element-ii` | LC 30d (1901) | 2D peak by column binary search | — |
| 80 | `80-closed-islands` | LC 30d (1254) | islands not touching the border | — |
| 81 | `81-search-2d-matrix-ii` | LC 30d (240) | staircase search | — |
| 82 | `82-interleaving-string` | LC 30d (97) | interleaving string DP | — |
| 83 | `83-longest-subarray-negation` | LC 30d (4063) | divisible by k with one negation | — |
| 84 | `84-equal-adjacent-pairs` | LC 30d (4066) | one value replacement, max equal neighbors | — |
| 85 | `85-meeting-earnings-gaps` | LC 30d (4068) | weighted interval scheduling + gap bonus | — |
| 86 | `86-sortable-integers` | LC 30d (3886) | block sizes that sort by rotations | — |
| 87 | `87-max-and-increments` | LC 30d (3806) | max AND of m elements after k increments (statement paraphrased) | — |
| 88 | `88-integer-to-roman` | LC 30d (12) | integer to Roman | — |
| 89 | `89-reverse-integer` | LC 30d (7) | reverse digits with overflow check | — |
| 90 | `90-distinct-squares-sum` | LC 30d (2916) | sum of squared distinct counts (segment tree) | — |
| 91 | `91-search-insert` | LC 30d (35) | search insert position | — |
| 92 | `92-length-last-word` | LC 30d (58) | length of last word | — |
| 93 | `93-sqrt-x` | LC 30d (69) | integer square root | — |
| 94 | `94-reverse-string` | LC 30d (344) | reverse string in place | — |
| 95 | `95-intersection-arrays` | LC 30d (349) | intersection of two arrays | — |
| 96 | `96-jewels-stones` | LC 30d (771) | jewels and stones | — |
| 97 | `97-sort-parity-ii` | LC 30d (922) | sort array by parity II | — |
| 98 | `98-remove-outer-parens` | LC 30d (1021) | remove outermost parentheses | — |
| 99 | `99-kth-missing-positive` | LC 30d (1539) | k-th missing positive | — |
| 100 | `100-students-lunch` | LC 30d (1700) | students unable to eat lunch | — |
| 101 | `101-bucket-batching` | h2h (Applied Scientist) | partition into G batches, min padding | — |

Already in the repo (LC 30-day list only):

| Problem | Source | File |
|---|---|---|
| LC 121 Best Time to Buy and Sell Stock | LC 30d | `company/apple/ref/recommended/19-best-time-stock.cpp` |
| LC 560 Subarray Sum Equals K | LC 30d | `company/apple/ref/recommended/32-subarray-sum-k.cpp` |

### Skipped

- Reverse an Array Iteratively (fp ×1, 2025): one loop with two pointers.
- SQL on the 30-day list: LC 175, 1141, 1158. One-liners: LC 2235, 2413.

## oa/ — online assessments only (outside ref/; do after everything in ref/)

These were reported only from online assessments (HackerRank-style, timed, partial credit).
His Nov 9 loop is onsite, so do them after everything above.

| # | `oa/` file | Source | Problem | `prep/` |
|---|---|---|---|---|
| 01 | `01-delivery-center-inconvenience` | h2h OA ×3 | add one center to minimize max Chebyshev distance (BFS + binary search) | — |
| 02 | `02-min-increments-non-decreasing` | h2h OA ×3 | min total range-add to make non-decreasing = sum of drops | — |
| 03 | `03-drone-ring` | h2h OA ×2 | shortest total travel on a unit ring | — |
| 04 | `04-drone-ring-weighted` | h2h OA | ring with per-edge costs (prefix sums) | — |
| 05 | `05-min-adjacent-swaps-binary` | h2h OA, LC 2938 | min adjacent swaps to group 0s/1s | — |
| 06 | `06-contiguous-values-min-cost` | h2h OA | merge overlapping value spans, min changed elements | — |
| 07 | `07-contiguous-values-min-ops` | h2h OA | same spans, min number of replace operations | — |
| 08 | `08-inventory-allocation` | h2h OA | bid groups, round-robin allocation | — |
| 09 | `09-restock-days` | h2h OA (= Codeforces 893D) | min restock days with checkpoints and a cap | — |
| 10 | `10-convert-products-cost` | h2h OA (fastprep "Variant A") | window-sum cost to clear all 1s = m + c(c-1)/2 | — |
| 11 | `11-maximize-shipment-order` | h2h OA | sort one window, lexicographically largest | — |
| 12 | `12-max-alternating-subseq` | h2h OA, LC 1911 | max alternating subsequence sum | — |
| 13 | `13-protected-city-population` | h2h OA | units stay or move left, max protected population | — |
| 14 | `14-min-removal-adjacent-deletion` | h2h OA | take min, delete neighbors (heap + linked list) | — |
| 15 | `15-sort-by-frequency-desc` | h2h OA | sort by frequency desc, value asc | — |
| 16 | `16-sort-errors-by-frequency` | h2h OA | sort by frequency asc, value asc | — |
| 17 | `17-max-min-sum-requests` | h2h OA | max + min per request, decrement max (multiset) | — |
| 18 | `18-prefix-frequency-division` | h2h OA | prefixes whose blocks all share its letter counts | — |
| 19 | `19-smallest-box-ids` | h2h OA | remove digit, insert min(d+1, 9), smallest string | — |
| 20 | `20-server-pairing` | h2h OA | primary/backup pairing, max primary memory | — |
| 21 | `21-replace-question-overhead` | h2h OA | independent min per `?` | — |
| 22 | `22-package-tracking-ledger` | h2h OA (project-style) | latest location + ON_TIME / LATE / PENDING | — |
| 23 | `23-max-median-transfers` | h2h OA (example inconsistent) | max median after +1/-1 transfers | — |
| 24 | `24-grid-one-conversion` | h2h OA (statement unclear) | shortest path with one wall removal (LC 1293, k = 1) | — |

Already in the repo (OA sightings — review last):

| Problem | Source | File |
|---|---|---|
| LC 274 H-Index ("Maximum k Instances") | h2h OA | `topic/arrays-hashing/274_h-index.cpp` |
