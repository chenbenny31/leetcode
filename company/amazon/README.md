# Amazon — prep guide for agents

Read this before helping Siyue prep for Amazon. It is the only md in this company folder;
don't add others (no per-problem notes).

## Folders

- `prep/` — his own hand-written solutions (create it when he writes the first one). Never
  create, edit or reformat files there.
- `ref/` — AI reference solutions, one flat folder, `NN-slug.cpp`, `01`–`97`, numbered in
  the tier order below. A new AI solution goes here with the next free number and gets a
  row in the matching tier table. No subfolders.
- Non-coding material lives in `~/Code/interview`: `AMAZON_NG_PREP.md` (LP stories, OOD,
  plan) and `amazon/system-design/` (the 10 hack2hire Amazon design write-ups).

## What we know about the interview

- Amazon SDE new grad loop on 2026-11-09. Rounds are usually coding plus 15–20 minutes of
  Leadership Principles; there may be an OOD round.
- Sources: hack2hire Amazon questions (site export and pasted pages) and the LeetCode
  Amazon 30-day frequency list (Oct 2026). Tier order is a prep judgment: onsite / phone
  problems and the top of the 30-day list come first; OA-only problems are in the last
  tier. The zip-export problems (`h2h` with no round) didn't say which round they came from.
- Several hack2hire examples are wrong (they contradict their own statement). The refs
  follow the statement and use hand-made tests.

## How to run a session

1. Pick the next problem in tier order (high-frequency → recommended → optional). Skip rows
   that already have a `prep/` file unless he asks to redo one.
2. One problem at a time: a very concise derivation (which data structure, why it is
   efficient) → T/S for the crucial functions → the full code.
3. "check" = review his file in `prep/`: compile with `g++ -std=c++17 -Wall -Wextra`, run its
   own tests, and report only errors (say whether it is a compile failure or a test failure).
   No praise, no summaries. Never edit his file — show the fix in chat and he types it.
4. Don't run tests for solutions you only show in chat; he tests them himself.

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
- No mutation inside a condition (`if (--indeg[v] == 0)`, `if (s.erase(x))`); the only
  exception is `insert(...).second`. No `<sstream>`. Keep lines ≤ 90 columns (move trailing
  comments above the line).
- C++17: use `auto` and structured bindings where the type is obvious, e.g.
  `auto [r, c] = q.front();` (a copy, never `auto&` before `pop()`), `for (auto& [k, v] : m)`.
- Minimal: implement only what the statement requires. Ignore hack2hire "insider tips".

## Tiers

`h2h` = hack2hire Amazon report (×n = reported n times), `LC 30d` = LeetCode Amazon 30-day
list. The `prep/` column names his file for that problem (— = not written yet).

### High-frequency — must do

| # | `ref/` file | Source | Problem | `prep/` |
|---|---|---|---|---|
| 01 | `01-oldest-one-time-visitor` | h2h ×2, LC 1429 | earliest user with exactly one login (+ X oldest) | — |
| 02 | `02-sliding-window-max-ds` | h2h onsite | last-k max data structure (monotonic deque) | — |
| 03 | `03-freq-stack` | h2h phone ×2, LC 895 | maximum frequency stack | — |
| 04 | `04-max-coins-k-bags` | LC 30d #2 (3413) | max coins in k consecutive bags | — |
| 05 | `05-max-k-sort-permutation` | LC 30d #4 (3644) | max k to sort a permutation by AND swaps | — |
| 06 | `06-maximum-interval-overlap` | h2h | max closed intervals sharing a point | — |
| 07 | `07-max-consecutive-on-servers` | h2h | ≤ k range flips, longest run of 1s | — |
| 08 | `08-organize-books-to-shelves` | h2h | least-loaded shelf in a prefix (segment tree) | — |
| 09 | `09-find-smallest-hub` | h2h | smallest-capacity free hub | — |
| 10 | `10-detect-abusive-words` | h2h | obfuscated word matching (trie) | — |
| 11 | `11-connect-points-2d` | h2h | points connected by row/column moves (BFS) | — |
| 12 | `12-block-string-assembly` | h2h | spell target from distinct blocks (matching) | — |
| 13 | `13-smallest-lex-palindrome` | h2h | smallest palindrome from all characters | — |
| 14 | `14-string-reorganization-all` | h2h (LC 767 variant) | all rearrangements with no equal neighbors | — |
| 15 | `15-file-search-api` | h2h onsite (OOD) | `find` with composable filters | — |
| 16 | `16-linked-list-queue` | h2h onsite | linked-list queue (+ delete, dedupe) | — |
| 17 | `17-top-k-slowest-logs` | h2h onsite | top-k durations, stable ties (heap) | — |
| 18 | `18-stock-variants` | h2h onsite | buy/sell stock: single, unlimited, fee in one function | — |
| 19 | `19-service-shutdown-impact` | h2h onsite | affected services in a dependency DAG (BFS) | — |
| 20 | `20-music-player` | h2h onsite | most frequent unplayed song with cycle reset | — |
| 21 | `21-sliding-median-reviews` | h2h onsite | median of the last N ratings | — |
| 22 | `22-log-parser` | h2h phone | parse logs, filter / count / top-k | — |

Already in the repo (same tier — review, don't redo):

| Problem | Source | File |
|---|---|---|
| LC 1 Two Sum (+ follow-ups) | LC 30d #1, h2h ×2 | `company/apple/ref/07-two-sum.cpp` |
| LC 42 Trapping Rain Water | LC 30d | `company/snowflake/ref/56-trapping-rain-water.cpp` |
| LC 146 LRU Cache | LC 30d, h2h | `company/apple/ref/10-lru-cache.cpp` |
| LC 200 Number of Islands (+ LC 695 max area) | LC 30d, h2h | `company/apple/ref/11-number-of-islands.cpp`, `topic/graphs/ref/695_max-area-island.cpp` |
| LC 3 Longest Substring Without Repeating | LC 30d, h2h ×2 | `company/apple/ref/08-longest-substring-no-repeat.cpp` |
| LC 121 Best Time to Buy and Sell Stock | LC 30d | `company/apple/ref/19-best-time-stock.cpp` |
| LC 236 LCA of a Binary Tree | LC 30d, h2h ×2 | `topic/trees/ref/236_lca-binary-tree.cpp` |
| LC 560 Subarray Sum Equals K | LC 30d | `company/apple/ref/32-subarray-sum-k.cpp` |
| LC 49 Group Anagrams (count key) | LC 30d, h2h ×2 | `company/apple/ref/13-group-anagrams.cpp` |
| LC 347 Top K Frequent | LC 30d, h2h ×2 | `company/apple/ref/14-top-k-frequent.cpp` |
| LC 252 / 253 Meeting Rooms | LC 30d, h2h ×2 | `company/pointone/ref/03-meeting-rooms.cpp` |
| LC 295 Median from Data Stream | h2h onsite | `company/apple/ref/35-median-stream.cpp` |
| LC 767 Reorganize String | LC 30d, h2h | `topic/trie-heap/ref/767_reorganize-string.cpp` |
| LC 994 Rotting Oranges | LC 30d, h2h | `topic/graphs/ref/994_rotting-oranges.cpp` |
| LC 138 Copy List with Random Pointer | LC 30d, h2h | `company/snowflake/ref/26-copy-random-list.cpp` |
| LC 56 Merge Intervals | LC 30d, h2h | `company/apple/ref/09-merge-intervals.cpp` |
| LC 20 Valid Parentheses | LC 30d, h2h | `company/snowflake/ref/30-valid-parentheses.cpp` |
| LC 198 House Robber | LC 30d, h2h | `company/apple/ref/36-house-robber.cpp` |
| LC 72 Edit Distance (reverse follow-up: discuss only) | h2h phone | `company/apple/ref/34-edit-distance.cpp` |
| LC 212 Word Search II | h2h onsite | `company/snowflake/ref/12-word-search-ii.cpp` |
| LC 210 / 207 Topological sort | LC 30d, h2h | `topic/graphs/ref/210_course-schedule-ii.cpp` |

### Recommended

| # | `ref/` file | Source | Problem | `prep/` |
|---|---|---|---|---|
| 23 | `23-robot-charging-monitor` | h2h phone | assign / release charging stations each tick | — |
| 24 | `24-package-weights-adjacent-sum` | h2h phone | order digits so neighbors sum < 10 (+ O(n)) | — |
| 25 | `25-hierarchical-comments` | h2h onsite | print a comment tree with indentation | — |
| 26 | `26-point-payment-min` | h2h phone | bounded knapsack, min points for exact payment | — |
| 27 | `27-tree-from-descriptions` | h2h onsite, LC 2196 | build a binary tree from parent/child rows | — |
| 28 | `28-complete-components` | h2h phone, LC 2685 | count complete connected components | — |
| 29 | `29-array-continuous-min-ops` | h2h onsite, LC 2009 | min replacements to make an array continuous | — |
| 30 | `30-domain-score` | h2h onsite | leaf domain score = own + ancestors | — |
| 31 | `31-make-change` | h2h onsite | greedy change in cents (+ limited drawer DP) | — |
| 32 | `32-caesar-cipher` | h2h onsite | Caesar shift (+ lookup table) | — |
| 33 | `33-permutations-circular` | h2h onsite | all line orders (+ circular, (n-1)!) | — |
| 34 | `34-node-relationship` | h2h onsite | siblings / cousins / others | — |
| 35 | `35-consecutive-like-k` | h2h onsite | longest chain with gaps < k (LC 128 general) | — |
| 36 | `36-in-place-sort` | h2h onsite | insertion sort vs quicksort | — |
| 37 | `37-merge-dicts` | h2h onsite | merge two / k dictionaries | — |
| 38 | `38-islands-streaming` | h2h follow-up | island count with rows streamed (union-find) | — |
| 39 | `39-dedup-events` | h2h onsite | latest per id / per-id history | — |
| 40 | `40-log-field-groups` | h2h phone | parse `ts\|k=v;...` lines into field groups | — |
| 41 | `41-aggregate-log-counts` | h2h phone | actions per user, sorted | — |
| 42 | `42-merge-sorted-arrays-variants` | h2h phone ×3, LC 88 | merge in place (+ descending, unsorted) | — |
| 43 | `43-weighted-lottery` | h2h onsite | k weighted winners without replacement | — |
| 44 | `44-package-installer` | h2h onsite | install with dependencies, cycle check, payload | — |
| 45 | `45-delivery-route-planning` | h2h onsite | pickups before drop-offs, capacity (BFS + DP) | — |
| 46 | `46-min-cost-equal-chars` | LC 30d (2712) | prefix/suffix flips to equalize a binary string | — |
| 47 | `47-max-white-tiles` | LC 30d (2271) | carpet covering most white tiles | — |
| 48 | `48-peak-element-ii` | LC 30d (1901) | 2D peak by column binary search | — |
| 49 | `49-closed-islands` | LC 30d (1254) | islands not touching the border | — |
| 50 | `50-search-2d-matrix-ii` | LC 30d (240) | staircase search | — |
| 51 | `51-interleaving-string` | LC 30d (97) | interleaving string DP | — |
| 52 | `52-longest-subarray-negation` | LC 30d (4063) | divisible by k with one negation | — |
| 53 | `53-equal-adjacent-pairs` | LC 30d (4066) | one value replacement, max equal neighbors | — |
| 54 | `54-meeting-earnings-gaps` | LC 30d (4068) | weighted interval scheduling + gap bonus | — |
| 55 | `55-sortable-integers` | LC 30d (3886) | block sizes that sort by rotations | — |
| 56 | `56-max-and-increments` | LC 30d (3806) | max AND of m elements after k increments (statement paraphrased) | — |
| 57 | `57-integer-to-roman` | LC 30d (12) | integer to Roman | — |
| 58 | `58-reverse-integer` | LC 30d (7) | reverse digits with overflow check | — |

Already in the repo (review, don't redo):

| Problem | File |
|---|---|
| LC 4 | `topic/binary-search/ref/4_median-two-sorted-arrays.cpp` |
| LC 2 | `topic/linked-list/ref/2_add-two-numbers.cpp` |
| LC 5 | `company/apple/ref/31-longest-palindromic-substring.cpp` |
| LC 11 | `topic/two-pointers/11_container-with-most-water.cpp` |
| LC 15 | `company/apple/ref/30-three-sum.cpp` |
| LC 22 | `topic/stack/22_generate-parentheses.cpp` |
| LC 23 | `company/apple/ref/17-merge-k-sorted-lists.cpp` |
| LC 36 | `company/apple/ref/18-valid-sudoku.cpp` |
| LC 128 | `company/apple/ref/41-longest-consecutive.cpp` |
| LC 238 | `topic/arrays-hashing/238_product-except-self.cpp` |
| LC 704 | `topic/binary-search/ref/704_binary-search.cpp` |
| LC 875 | `topic/binary-search/ref/875_koko-bananas.cpp` |
| LC 1004 | `topic/sliding-window/ref/1004_max-consecutive-ones-iii.cpp` |
| LC 8 | `company/apple/ref/48-string-to-integer-atoi.cpp` |
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
| LC 74 | `topic/binary-search/ref/74_search-2d-matrix.cpp` |
| LC 122 | `topic/dp/ref/122_stock-ii.cpp` |
| LC 133 | `topic/graphs/ref/133_clone-graph.cpp` |
| LC 136 | `topic/bit-math-matrix/ref/136_single-number.cpp` |
| LC 151 | `topic/parser/ref/151_reverse-words.cpp` |
| LC 169 | `topic/arrays-hashing/169_majority-element.cpp` |
| LC 206 | `company/apple/ref/20-reverse-linked-list.cpp` |
| LC 303 | `topic/arrays-hashing/303_range-sum-query.cpp` |
| LC 399 | `topic/advanced-graphs/ref/399_evaluate-division.cpp` |
| LC 402 | `topic/stack/ref/402_remove-k-digits.cpp` |
| LC 410 | `topic/binary-search/ref/410_split-array-largest-sum.cpp` |
| LC 424 | `topic/sliding-window/424_longest-repeating-char-replace.cpp` |
| LC 621 | `company/apple/ref/33-task-scheduler.cpp` |
| LC 863 | `topic/trees/ref/863_nodes-distance-k.cpp` |
| LC 14 | `company/apple/ref/16-longest-common-prefix.cpp` |
| LC 283 | `company/apple/ref/37-move-zeroes.cpp` |
| LC 235 | `topic/trees/ref/235_lca-bst.cpp` |
| LC 274 (h-index = "Maximum k Instances") | `topic/arrays-hashing/274_h-index.cpp` |
| LC 474 | `topic/knapsack/ref/474_ones-and-zeroes.cpp` |
| LC 323 | `topic/graphs/ref/323_connected-components.cpp` |
| LC 394 | `topic/stack/ref/394_decode-string.cpp` |
| LC 135 | `topic/greedy-intervals/ref/135_candy.cpp` |
| LC 217 | `topic/arrays-hashing/217_contains-duplicate.cpp` |
| LC 904 | `topic/sliding-window/ref/904_fruit-into-baskets.cpp` |
| LC 735 | `topic/stack/ref/735_asteroid-collision.cpp` |
| LC 417 (one-ocean variant) | `topic/graphs/ref/417_pacific-atlantic.cpp` |

### Optional

| # | `ref/` file | Source | Problem | `prep/` |
|---|---|---|---|---|
| 59 | `59-bucket-batching` | h2h (Applied Scientist) | partition into G batches, min padding | — |
| 60 | `60-nested-get-path` | h2h onsite | `get(obj, "a[0].b")` on nested maps / lists | — |
| 61 | `61-distinct-squares-sum` | LC 30d (2916) | sum of squared distinct counts (segment tree) | — |
| 62 | `62-morse-code-words` | h2h onsite, LC 804 | unique Morse transformations | — |
| 63 | `63-search-insert` | LC 30d (35) | search insert position | — |
| 64 | `64-length-last-word` | LC 30d (58) | length of last word | — |
| 65 | `65-sqrt-x` | LC 30d (69) | integer square root | — |
| 66 | `66-reverse-string` | LC 30d (344) | reverse string in place | — |
| 67 | `67-intersection-arrays` | LC 30d (349) | intersection of two arrays | — |
| 68 | `68-jewels-stones` | LC 30d (771) | jewels and stones | — |
| 69 | `69-sort-parity-ii` | LC 30d (922) | sort array by parity II | — |
| 70 | `70-remove-outer-parens` | LC 30d (1021) | remove outermost parentheses | — |
| 71 | `71-kth-missing-positive` | LC 30d (1539) | k-th missing positive | — |
| 72 | `72-students-lunch` | LC 30d (1700) | students unable to eat lunch | — |

### OA only — do last

These were reported only from online assessments (HackerRank-style, timed, partial credit).
His Nov 9 loop is onsite, so do them after everything above.

| # | `ref/` file | Source | Problem | `prep/` |
|---|---|---|---|---|
| 73 | `73-delivery-center-inconvenience` | h2h OA ×3 | add one center to minimize max Chebyshev distance (BFS + binary search) | `delivery-center.cpp` |
| 74 | `74-min-increments-non-decreasing` | h2h OA ×3 | min total range-add to make non-decreasing = sum of drops | — |
| 75 | `75-drone-ring` | h2h OA ×2 | shortest total travel on a unit ring | — |
| 76 | `76-drone-ring-weighted` | h2h OA | ring with per-edge costs (prefix sums) | — |
| 77 | `77-min-adjacent-swaps-binary` | h2h OA, LC 2938 | min adjacent swaps to group 0s/1s | — |
| 78 | `78-contiguous-values-min-cost` | h2h OA | merge overlapping value spans, min changed elements | — |
| 79 | `79-contiguous-values-min-ops` | h2h OA | same spans, min number of replace operations | — |
| 80 | `80-inventory-allocation` | h2h OA | bid groups, round-robin allocation | — |
| 81 | `81-restock-days` | h2h OA (= Codeforces 893D) | min restock days with checkpoints and a cap | — |
| 82 | `82-convert-products-cost` | h2h OA (fastprep "Variant A") | window-sum cost to clear all 1s = m + c(c-1)/2 | — |
| 83 | `83-maximize-shipment-order` | h2h OA | sort one window, lexicographically largest | — |
| 84 | `84-max-alternating-subseq` | h2h OA, LC 1911 | max alternating subsequence sum | — |
| 85 | `85-protected-city-population` | h2h OA | units stay or move left, max protected population | — |
| 86 | `86-min-removal-adjacent-deletion` | h2h OA | take min, delete neighbors (heap + linked list) | — |
| 87 | `87-sort-by-frequency-desc` | h2h OA | sort by frequency desc, value asc | — |
| 88 | `88-sort-errors-by-frequency` | h2h OA | sort by frequency asc, value asc | — |
| 89 | `89-max-min-sum-requests` | h2h OA | max + min per request, decrement max (multiset) | — |
| 90 | `90-prefix-frequency-division` | h2h OA | prefixes whose blocks all share its letter counts | — |
| 91 | `91-smallest-box-ids` | h2h OA | remove digit, insert min(d+1, 9), smallest string | — |
| 92 | `92-server-pairing` | h2h OA | primary/backup pairing, max primary memory | — |
| 93 | `93-replace-question-overhead` | h2h OA | independent min per `?` | — |
| 94 | `94-package-tracking-ledger` | h2h OA (project-style) | latest location + ON_TIME / LATE / PENDING | — |
| 95 | `95-first-unique-char` | h2h OA, LC 387 | first unique character | — |
| 96 | `96-max-median-transfers` | h2h OA (example inconsistent) | max median after +1/-1 transfers | — |
| 97 | `97-grid-one-conversion` | h2h OA (statement unclear) | shortest path with one wall removal (LC 1293, k = 1) | — |

### Skipped

- SQL on the 30-day list: LC 175, 1141, 1158. One-liners: LC 2235, 2413.
