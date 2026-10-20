# Study order

Apple first, then the one-trick set, then ByteDance. Duplicates across folders are
marked **skip** at their second appearance (the first copy is the one to do; later copies
may only add a follow-up). Paths are relative to `300blows/company/` except `specialty/`.

How to work each item: timer on (25 min for medium), write it cold in one file with the
`test` function, run it, then diff against the repo file and note what you missed.

Phase 0 — before anything: read `HANDOUT.md` §2–§5 once (round workflow, style, pitfalls,
skeletons).

## Phase 1 — Apple must (ng) (15 to do)

Bit ops first (Silicon Validation role), then the LC list top by frequency, then the hack2hire Apple questions.

| # | File | LC | Note |
|---|---|---|---|
| 1 | `apple/ref/02-bit-ops.cpp` | — | |
| 2 | `apple/ref/07-two-sum.cpp` | 1 | |
| 3 | `apple/ref/08-longest-substring-no-repeat.cpp` | 3 | |
| 4 | `apple/ref/09-merge-intervals.cpp` | 56 | |
| 5 | `apple/ref/10-lru-cache.cpp` | 146 | |
| 6 | `apple/ref/11-number-of-islands.cpp` | 200 | |
| 7 | `apple/ref/12-course-schedule.cpp` | 207 | |
| 8 | `apple/ref/13-group-anagrams.cpp` | 49 | |
| 9 | `apple/ref/15-valid-palindrome.cpp` | 125 | |
| 10 | `apple/ref/14-top-k-frequent.cpp` | 347 | |
| 11 | `apple/ref/01-palindrome-score.cpp` | — | |
| 12 | `apple/ref/03-bipartite.cpp` | 785 | |
| 13 | `apple/ref/04-subset-sum-dp.cpp` | — | |
| 14 | `apple/ref/05-subsets-with-sum.cpp` | — | |
| 15 | `apple/ref/06-highway-toll.cpp` | — | |

## Phase 2 — Apple important (26 to do)

Rest of the Apple LC list, already ordered by frequency.

| # | File | LC | Note |
|---|---|---|---|
| 1 | `apple/ref/16-longest-common-prefix.cpp` | 14 | |
| 2 | `apple/ref/17-merge-k-sorted-lists.cpp` | 23 | |
| 3 | `apple/ref/18-valid-sudoku.cpp` | 36 | |
| 4 | `apple/ref/19-best-time-stock.cpp` | 121 | |
| 5 | `apple/ref/20-reverse-linked-list.cpp` | 206 | |
| 6 | `apple/ref/21-word-ladder.cpp` | 127 | |
| 7 | `apple/ref/22-design-hit-counter.cpp` | 362 | |
| 8 | `apple/ref/23-spiral-matrix.cpp` | 54 | |
| 9 | `apple/ref/24-first-last-position.cpp` | 34 | |
| 10 | `apple/ref/25-time-map.cpp` | 981 | |
| 11 | `apple/ref/26-subarray-product-less-k.cpp` | 713 | |
| 12 | `apple/ref/27-insert-delete-getrandom.cpp` | 380 | |
| 13 | `apple/ref/28-snapshot-array.cpp` | 1146 | |
| 14 | `apple/ref/29-basic-calculator-ii.cpp` | 227 | |
| 15 | `apple/ref/30-three-sum.cpp` | 15 | |
| 16 | `apple/ref/31-longest-palindromic-substring.cpp` | 5 | |
| 17 | `apple/ref/32-subarray-sum-k.cpp` | 560 | |
| 18 | `apple/ref/33-task-scheduler.cpp` | 621 | |
| 19 | `apple/ref/34-edit-distance.cpp` | 72 | |
| 20 | `apple/ref/35-median-stream.cpp` | 295 | |
| 21 | `apple/ref/36-house-robber.cpp` | 198 | |
| 22 | `apple/ref/37-move-zeroes.cpp` | 283 | |
| 23 | `apple/ref/38-longest-common-subsequence.cpp` | 1143 | |
| 24 | `apple/ref/39-jump-game.cpp` | 55 | |
| 25 | `apple/ref/40-find-peak-element.cpp` | 162 | |
| 26 | `apple/ref/41-longest-consecutive.cpp` | 128 | |

## Phase 3 — Specialty tricks (46 to do)

One-trick problems; read the `// Trick:` line first, then type from memory. Bit tricks and index tricks matter most for a low-level C/C++ role.

| # | File | LC | Note |
|---|---|---|---|
| 1 | `specialty/84.cpp` | 84 | |
| 2 | `specialty/739.cpp` | 739 | |
| 3 | `specialty/402.cpp` | 402 | |
| 4 | `specialty/316.cpp` | 316 | |
| 5 | `specialty/32.cpp` | 32 | |
| 6 | `specialty/239.cpp` | 239 | |
| 7 | `specialty/42.cpp` | 42 | |
| 8 | `specialty/41.cpp` | 41 | |
| 9 | `specialty/448.cpp` | 448 | |
| 10 | `specialty/287.cpp` | 287 | |
| 11 | `specialty/142.cpp` | 142 | |
| 12 | `specialty/160.cpp` | 160 | |
| 13 | `specialty/31.cpp` | 31 | |
| 14 | `specialty/169.cpp` | 169 | |
| 15 | `specialty/134.cpp` | 134 | |
| 16 | `specialty/238.cpp` | 238 | |
| 17 | `specialty/75.cpp` | 75 | |
| 18 | `specialty/189.cpp` | 189 | |
| 19 | `specialty/48.cpp` | 48 | |
| 20 | `specialty/11.cpp` | 11 | |
| 21 | `specialty/135.cpp` | 135 | |
| 22 | `specialty/528.cpp` | 528 | |
| – | ~~`specialty/560.cpp`~~ | 560 | skip — same as apple/ref/32-subarray-sum-k.cpp |
| 23 | `specialty/974.cpp` | 974 | |
| – | ~~`specialty/128.cpp`~~ | 128 | skip — same as apple/ref/41-longest-consecutive.cpp |
| 24 | `specialty/410.cpp` | 410 | |
| 25 | `specialty/4.cpp` | 4 | |
| – | ~~`specialty/380.cpp`~~ | 380 | skip — same as apple/ref/27-insert-delete-getrandom.cpp |
| 26 | `specialty/382.cpp` | 382 | |
| 27 | `specialty/384.cpp` | 384 | |
| 28 | `specialty/470.cpp` | 470 | |
| 29 | `specialty/215.cpp` | 215 | |
| 30 | `specialty/136.cpp` | 136 | |
| 31 | `specialty/137.cpp` | 137 | |
| 32 | `specialty/338.cpp` | 338 | |
| – | ~~`specialty/621.cpp`~~ | 621 | skip — same as apple/ref/33-task-scheduler.cpp |
| 33 | `specialty/435.cpp` | 435 | |
| 34 | `specialty/763.cpp` | 763 | |
| 35 | `specialty/snowcal.cpp` | — | |
| 36 | `specialty/394.cpp` | 394 | |
| 37 | `specialty/224.cpp` | 224 | |
| 38 | `specialty/271.cpp` | 271 | |
| 39 | `specialty/297.cpp` | 297 | |
| 40 | `specialty/399.cpp` | 399 | |
| 41 | `specialty/269.cpp` | 269 | |
| 42 | `specialty/332.cpp` | 332 | |
| 43 | `specialty/300.cpp` | 300 | |
| 44 | `specialty/312.cpp` | 312 | |
| 45 | `specialty/416.cpp` | 416 | |
| 46 | `specialty/28.cpp` | 28 | |

## Phase 4 — Apple optional (23 to do)

Lower-frequency / harder Apple LC problems. Skip `66-sv-constraints.sv` (Design Verification role only).

| # | File | LC | Note |
|---|---|---|---|
| – | ~~`apple/ref/42-rotate-array.cpp`~~ | 189 | skip — same as specialty/189.cpp |
| 1 | `apple/ref/43-reorder-list.cpp` | 143 | |
| 2 | `apple/ref/44-string-compression.cpp` | 443 | |
| 3 | `apple/ref/45-last-stone-weight.cpp` | 1046 | |
| 4 | `apple/ref/46-maximum-swap.cpp` | 670 | |
| 5 | `apple/ref/47-valid-parenthesis-string.cpp` | 678 | |
| 6 | `apple/ref/48-string-to-integer-atoi.cpp` | 8 | |
| 7 | `apple/ref/49-lonely-numbers.cpp` | 2150 | |
| 8 | `apple/ref/50-total-hamming-distance.cpp` | 477 | |
| 9 | `apple/ref/51-lca-parent-pointers.cpp` | 1650 | |
| 10 | `apple/ref/52-bst-greater-sum-tree.cpp` | 1038 | |
| 11 | `apple/ref/53-construct-pre-post.cpp` | 889 | |
| 12 | `apple/ref/54-cheapest-flights-k-stops.cpp` | 787 | |
| 13 | `apple/ref/55-last-stone-weight-ii.cpp` | 1049 | |
| 14 | `apple/ref/56-knight-probability.cpp` | 688 | |
| 15 | `apple/ref/57-flatten-nested-iterator.cpp` | 341 | |
| 16 | `apple/ref/58-design-memory-allocator.cpp` | 2502 | |
| 17 | `apple/ref/59-design-task-manager.cpp` | 3408 | |
| 18 | `apple/ref/60-unit-conversion-ii.cpp` | 3535 | |
| 19 | `apple/ref/61-exam-room.cpp` | 855 | |
| 20 | `apple/ref/62-meeting-rooms-iii.cpp` | 2402 | |
| 21 | `apple/ref/63-min-cost-hire-k-workers.cpp` | 857 | |
| 22 | `apple/ref/64-lfu-cache.cpp` | 460 | |
| 23 | `apple/ref/65-parse-lisp-expression.cpp` | 736 | |

## Phase 5 — ByteDance must (ng) (16 to do)

Only the ones not already done above.

| # | File | LC | Note |
|---|---|---|---|
| – | ~~`bytedance/ng/01-lru-cache.cpp`~~ | 146 | skip — same as apple/ref/10-lru-cache.cpp; read its TTL follow-up class |
| – | ~~`bytedance/ng/02-number-of-islands.cpp`~~ | 200 | skip — same as apple/ref/11-number-of-islands.cpp; read its distinct-islands (LC 694) follow-up |
| – | ~~`bytedance/ng/03-merge-intervals.cpp`~~ | 56 | skip — same as apple/ref/09-merge-intervals.cpp |
| – | ~~`bytedance/ng/04-course-schedule.cpp`~~ | 207 | skip — same as apple/ref/12-course-schedule.cpp; read its LC 210 Kahn + 3-color DFS follow-ups |
| – | ~~`bytedance/ng/05-kth-largest-quickselect.cpp`~~ | 215 | skip — same as specialty/215.cpp |
| 1 | `bytedance/ng/06-shortest-bridge.cpp` | 934 | |
| – | ~~`bytedance/ng/07-word-ladder.cpp`~~ | 127 | skip — same as apple/ref/21-word-ladder.cpp |
| – | ~~`bytedance/ng/08-house-robber.cpp`~~ | 198 | skip — same as apple/ref/36-house-robber.cpp |
| – | ~~`bytedance/ng/09-two-sum.cpp`~~ | 1 | skip — same as apple/ref/07-two-sum.cpp |
| 2 | `bytedance/ng/10-add-two-numbers.cpp` | 2 | |
| 3 | `bytedance/ng/11-search-rotated-array.cpp` | 33 | |
| 4 | `bytedance/ng/12-combination-sum.cpp` | 39 | |
| 5 | `bytedance/ng/13-multiply-strings.cpp` | 43 | |
| 6 | `bytedance/ng/14-max-subarray.cpp` | 53 | |
| – | ~~`bytedance/ng/15-best-time-stock.cpp`~~ | 121 | skip — same as apple/ref/19-best-time-stock.cpp |
| 7 | `bytedance/ng/16-maximal-square.cpp` | 221 | |
| – | ~~`bytedance/ng/17-basic-calculator-ii.cpp`~~ | 227 | skip — same as apple/ref/29-basic-calculator-ii.cpp |
| – | ~~`bytedance/ng/18-basic-calculator.cpp`~~ | 224 | skip — same as specialty/224.cpp |
| – | ~~`bytedance/ng/19-top-k-frequent.cpp`~~ | 347 | skip — same as apple/ref/14-top-k-frequent.cpp |
| 8 | `bytedance/ng/20-max-width-binary-tree.cpp` | 662 | |
| 9 | `bytedance/ng/21-rotting-oranges.cpp` | 994 | |
| – | ~~`bytedance/ng/22-longest-common-subsequence.cpp`~~ | 1143 | skip — same as apple/ref/38-longest-common-subsequence.cpp |
| 10 | `bytedance/ng/23-longest-repeating-char-replacement.cpp` | 424 | |
| – | ~~`bytedance/ng/24-longest-substring-no-repeat.cpp`~~ | 3 | skip — same as apple/ref/08-longest-substring-no-repeat.cpp |
| 11 | `bytedance/ng/25-implement-queue.cpp` | — | |
| 12 | `bytedance/ng/26-path-sum-ii.cpp` | 113 | |
| 13 | `bytedance/ng/27-all-paths-directed.cpp` | — | |
| 14 | `bytedance/ng/28-all-cycles-directed.cpp` | — | |
| 15 | `bytedance/ng/29-design-hashmap.cpp` | 706 | |
| 16 | `bytedance/ng/30-reverse-words.cpp` | 151 | |

## Phase 6 — ByteDance important (23 to do)

| # | File | LC | Note |
|---|---|---|---|
| 1 | `bytedance/important/31-subsets.cpp` | 78 | |
| 2 | `bytedance/important/32-add-strings-char-arrays.cpp` | 415 | |
| 3 | `bytedance/important/33-substructure-of-tree.cpp` | — | |
| 4 | `bytedance/important/34-kth-node-from-end.cpp` | — | |
| 5 | `bytedance/important/35-three-sum-closest.cpp` | 16 | |
| – | ~~`bytedance/important/36-merge-k-sorted-lists.cpp`~~ | 23 | skip — same as apple/ref/17-merge-k-sorted-lists.cpp |
| – | ~~`bytedance/important/37-trapping-rain-water.cpp`~~ | 42 | skip — same as specialty/42.cpp |
| – | ~~`bytedance/important/38-jump-game.cpp`~~ | 55 | skip — same as apple/ref/39-jump-game.cpp |
| 6 | `bytedance/important/39-reverse-linked-list-ii.cpp` | 92 | |
| 7 | `bytedance/important/40-restore-ip-addresses.cpp` | 93 | |
| 8 | `bytedance/important/41-zigzag-level-order.cpp` | 103 | |
| 9 | `bytedance/important/42-clone-graph.cpp` | 133 | |
| 10 | `bytedance/important/43-sort-list.cpp` | 148 | |
| – | ~~`bytedance/important/44-find-peak-element.cpp`~~ | 162 | skip — same as apple/ref/40-find-peak-element.cpp |
| – | ~~`bytedance/important/45-sliding-window-max.cpp`~~ | 239 | skip — same as specialty/239.cpp |
| – | ~~`bytedance/important/46-decode-string.cpp`~~ | 394 | skip — same as specialty/394.cpp |
| – | ~~`bytedance/important/47-remove-k-digits.cpp`~~ | 402 | skip — same as specialty/402.cpp |
| 11 | `bytedance/important/48-delete-node-bst.cpp` | 450 | |
| 12 | `bytedance/important/49-single-element-sorted.cpp` | 540 | |
| – | ~~`bytedance/important/50-subarray-sum-k.cpp`~~ | 560 | skip — same as apple/ref/32-subarray-sum-k.cpp |
| 13 | `bytedance/important/51-accounts-merge.cpp` | 721 | |
| 14 | `bytedance/important/52-infected-tree-time.cpp` | 2385 | |
| 15 | `bytedance/important/53-longest-common-prefix-length.cpp` | 3043 | |
| – | ~~`bytedance/important/54-design-memory-allocator.cpp`~~ | 2502 | skip — same as apple/ref/58-design-memory-allocator.cpp |
| 16 | `bytedance/important/55-product-last-k.cpp` | 1352 | |
| 17 | `bytedance/important/56-valid-parentheses.cpp` | 20 | |
| 18 | `bytedance/important/57-top-k-kols.cpp` | — | |
| 19 | `bytedance/important/58-add-search-word.cpp` | 211 | |
| 20 | `bytedance/important/59-sqrt-precision.cpp` | — | |
| 21 | `bytedance/important/60-permutation-in-string.cpp` | 567 | |
| – | ~~`bytedance/important/61-lfu-cache.cpp`~~ | 460 | skip — same as apple/ref/64-lfu-cache.cpp |
| 22 | `bytedance/important/62-max-path-sum.cpp` | 124 | |
| – | ~~`bytedance/important/63-longest-palindromic-substring.cpp`~~ | 5 | skip — same as apple/ref/31-longest-palindromic-substring.cpp |
| – | ~~`bytedance/important/64-group-anagrams.cpp`~~ | 49 | skip — same as apple/ref/13-group-anagrams.cpp |
| 23 | `bytedance/important/65-regex-matching.cpp` | 10 | |
| – | ~~`bytedance/important/66-longest-increasing-subsequence.cpp`~~ | 300 | skip — same as specialty/300.cpp |

## Phase 7 — ByteDance optional (22 to do)

Only if time remains.

| # | File | LC | Note |
|---|---|---|---|
| 1 | `bytedance/optional/67-text-justification.cpp` | 68 | |
| – | ~~`bytedance/optional/68-remove-duplicate-letters.cpp`~~ | 316 | skip — same as specialty/316.cpp |
| 2 | `bytedance/optional/69-shortest-distance-buildings.cpp` | 317 | |
| 3 | `bytedance/optional/70-min-area-rectangle.cpp` | 939 | |
| 4 | `bytedance/optional/71-remove-zero-sum-nodes.cpp` | 1171 | |
| 5 | `bytedance/optional/72-k-length-substrings-no-repeat.cpp` | 1100 | |
| 6 | `bytedance/optional/73-remove-duplicates-ii.cpp` | 80 | |
| 7 | `bytedance/optional/74-missing-ranges.cpp` | 163 | |
| 8 | `bytedance/optional/75-swim-rising-water.cpp` | 778 | |
| 9 | `bytedance/optional/76-combinations.cpp` | 77 | |
| 10 | `bytedance/optional/77-photo-groups.cpp` | 547 | |
| – | ~~`bytedance/optional/78-valid-parenthesis-string.cpp`~~ | 678 | skip — same as apple/ref/47-valid-parenthesis-string.cpp |
| 11 | `bytedance/optional/79-search-insert-position.cpp` | 35 | |
| 12 | `bytedance/optional/80-circular-deque-index.cpp` | — | |
| 13 | `bytedance/optional/81-daily-login-counts.cpp` | — | |
| 14 | `bytedance/optional/82-min-cost-steps.cpp` | — | |
| 15 | `bytedance/optional/83-construct-pre-in.cpp` | 105 | |
| 16 | `bytedance/optional/84-populate-next-right.cpp` | 116 | |
| 17 | `bytedance/optional/85-house-robber-iii.cpp` | 337 | |
| 18 | `bytedance/optional/86-n-queens.cpp` | 51 | |
| – | ~~`bytedance/optional/87-largest-rectangle-histogram.cpp`~~ | 84 | skip — same as specialty/84.cpp |
| 19 | `bytedance/optional/88-kth-smallest-subarray-sum.cpp` | 1918 | |
| 20 | `bytedance/optional/89-camel-case-filter.cpp` | — | |
| – | ~~`bytedance/optional/90-construct-pre-post.cpp`~~ | 889 | skip — same as apple/ref/53-construct-pre-post.cpp |
| 21 | `bytedance/optional/91-max-candies.cpp` | 2226 | |
| 22 | `bytedance/optional/92-min-jumps-home.cpp` | 1654 | |

## Phase 8 — Snowflake (only if it moves to another round)

`company/snowflake/ng/` root 12 + the two real questions (`word-pair-within-k.cpp`,
`command-interpreter.cpp`) → `ng/important/` → `ng/optional/`. Many overlap with the
phases above (LRU, merge intervals, course schedule, top-k, calculator, median, task manager).
