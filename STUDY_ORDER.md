# Study order

ByteDance first, then Amazon, then the earlier Apple / specialty / Snowflake sets. Each problem
appears once, at its first (highest-priority) appearance; later copies are marked **skip**. All
paths are relative to the repo root. `topic/<category>/ref/<lc>_<name>.cpp` is the AI reference in the
live-interview style (one runnable file with a `test` function); `topic/<category>/<lc>_<name>.cpp` is
your own technique drill; `company/` and `specialty/` hold the older sets. A `—` file means no
solution exists in the repo yet (the problem is not in the 300-problem README).

How to work each item: timer on (25 min for medium), write it cold in one file with the
`test` function, run it, then diff against the repo file and note what you missed.

Phase 0 — before anything: read `HANDOUT.md` §2–§5 once (round workflow, style, pitfalls,
skeletons).

## Phase 1 — ByteDance — contest-point problems (18 to do, 2 without a file)

The ByteDance LeetCode list, the 18 problems that carry a contest point, highest first.

| # | File | LC | Problem | Note |
|---|---|---|---|---|
| 1 | `company/bytedance/ng/10-add-two-numbers.cpp` | 2 | Add Two Numbers | also `topic/linked-list/ref/2_add-two-numbers.cpp` |
| 2 | `company/bytedance/important/50-subarray-sum-k.cpp` | 560 | Subarray Sum Equals K | also `company/apple/ref/32-subarray-sum-k.cpp`, `specialty/560_subarray-sum-k.cpp`, `topic/sliding-window/560_subarray-sum-k.cpp` |
| 3 | `company/bytedance/ng/20-max-width-binary-tree.cpp` | 662 | Maximum Width of Binary Tree |  |
| 4 | `company/bytedance/important/51-accounts-merge.cpp` | 721 | Accounts Merge |  |
| 5 | `company/bytedance/ng/06-shortest-bridge.cpp` | 934 | Shortest Bridge |  |
| 6 | — | 1081 | Smallest Subsequence of Distinct Characters |  |
| 7 | `topic/scheduling/ref/1235_max-profit-job-scheduling.cpp` | 1235 | Maximum Profit in Job Scheduling |  |
| 8 | `company/bytedance/ng/21-rotting-oranges.cpp` | 994 | Rotting Oranges | also `topic/graphs/ref/994_rotting-oranges.cpp` |
| 9 | `company/bytedance/optional/71-remove-zero-sum-nodes.cpp` | 1171 | Remove Zero Sum Consecutive Nodes from Linked List |  |
| 10 | `company/bytedance/important/55-product-last-k.cpp` | 1352 | Product of the Last K Numbers |  |
| 11 | — | 1997 | First Day Where You Have Been in All the Rooms |  |
| 12 | `company/bytedance/important/52-infected-tree-time.cpp` | 2385 | Amount of Time for Binary Tree to Be Infected |  |
| 13 | `company/bytedance/important/54-design-memory-allocator.cpp` | 2502 | Design Memory Allocator | also `company/apple/ref/58-design-memory-allocator.cpp` |
| 14 | `company/bytedance/important/53-longest-common-prefix-length.cpp` | 3043 | Find the Length of the Longest Common Prefix |  |
| 15 | `company/bytedance/ng/09-two-sum.cpp` | 1 | Two Sum | also `company/apple/ref/07-two-sum.cpp`, `topic/arrays-hashing/1_two-sum.cpp` |
| 16 | `company/bytedance/important/46-decode-string.cpp` | 394 | Decode String | also `topic/stack/ref/394_decode-string.cpp`, `specialty/394_decode-string.cpp` |
| 17 | `company/bytedance/important/47-remove-k-digits.cpp` | 402 | Remove K Digits | also `topic/stack/ref/402_remove-k-digits.cpp`, `specialty/402_remove-k-digits.cpp` |
| 18 | `company/bytedance/ng/23-longest-repeating-char-replacement.cpp` | 424 | Longest Repeating Character Replacement | also `topic/sliding-window/424_longest-repeating-char-replace.cpp` |

## Phase 2 — ByteDance — rest of the LeetCode list (41 to do)

The remaining ByteDance LeetCode problems, in the list order.

| # | File | LC | Problem | Note |
|---|---|---|---|---|
| 1 | `company/bytedance/important/65-regex-matching.cpp` | 10 | Regular Expression Matching | also `topic/dp/ref/10_regex-matching.cpp` |
| 2 | `company/apple/ref/30-three-sum.cpp` | 15 | 3Sum | also `topic/two-pointers/15_three-sum.cpp` |
| 3 | `company/bytedance/important/35-three-sum-closest.cpp` | 16 | 3Sum Closest | also `topic/two-pointers/16_three-sum-closest.cpp` |
| 4 | `topic/stack/22_generate-parentheses.cpp` | 22 | Generate Parentheses |  |
| 5 | `company/bytedance/important/36-merge-k-sorted-lists.cpp` | 23 | Merge k Sorted Lists | also `topic/linked-list/ref/23_merge-k-lists.cpp`, `company/apple/ref/17-merge-k-sorted-lists.cpp` |
| 6 | `company/bytedance/ng/11-search-rotated-array.cpp` | 33 | Search in Rotated Sorted Array | also `topic/binary-search/ref/33_search-rotated-array.cpp` |
| 7 | `company/bytedance/ng/12-combination-sum.cpp` | 39 | Combination Sum | also `topic/backtracking/ref/39_combination-sum.cpp` |
| 8 | `company/bytedance/important/37-trapping-rain-water.cpp` | 42 | Trapping Rain Water | also `specialty/42_trapping-rain-water.cpp`, `company/snowflake/ref/56-trapping-rain-water.cpp`, `topic/two-pointers/42_trapping-rain-water.cpp` |
| 9 | `company/bytedance/ng/13-multiply-strings.cpp` | 43 | Multiply Strings | also `topic/bit-math-matrix/ref/43_multiply-strings.cpp` |
| 10 | `topic/bit-math-matrix/ref/48_rotate-image.cpp` | 48 | Rotate Image | also `specialty/48_rotate-image.cpp` |
| 11 | `company/bytedance/ng/14-max-subarray.cpp` | 53 | Maximum Subarray | also `topic/greedy-intervals/ref/53_max-subarray.cpp` |
| 12 | `company/bytedance/important/38-jump-game.cpp` | 55 | Jump Game | also `topic/greedy-intervals/ref/55_jump-game.cpp`, `company/apple/ref/39-jump-game.cpp` |
| 13 | `company/bytedance/ng/03-merge-intervals.cpp` | 56 | Merge Intervals | also `topic/greedy-intervals/ref/56_merge-intervals.cpp`, `company/apple/ref/09-merge-intervals.cpp`, `company/snowflake/ref/29-merge-intervals.cpp` |
| 14 | `topic/dp/ref/62_unique-paths.cpp` | 62 | Unique Paths |  |
| 15 | `company/bytedance/optional/67-text-justification.cpp` | 68 | Text Justification | also `topic/parser/ref/68_text-justification.cpp` |
| 16 | `company/bytedance/important/39-reverse-linked-list-ii.cpp` | 92 | Reverse Linked List II | also `topic/linked-list/ref/92_reverse-list-ii.cpp` |
| 17 | `company/bytedance/important/40-restore-ip-addresses.cpp` | 93 | Restore IP Addresses | also `topic/backtracking/ref/93_restore-ip.cpp` |
| 18 | `company/bytedance/important/41-zigzag-level-order.cpp` | 103 | Binary Tree Zigzag Level Order Traversal | also `topic/trees/ref/103_zigzag-level-order.cpp` |
| 19 | `company/bytedance/ng/15-best-time-stock.cpp` | 121 | Best Time to Buy and Sell Stock | also `company/apple/ref/19-best-time-stock.cpp`, `topic/sliding-window/121_best-time-stock.cpp` |
| 20 | `company/bytedance/ng/07-word-ladder.cpp` | 127 | Word Ladder | also `topic/graphs/ref/127_word-ladder.cpp`, `company/apple/ref/21-word-ladder.cpp` |
| 21 | `company/bytedance/important/42-clone-graph.cpp` | 133 | Clone Graph | also `topic/graphs/ref/133_clone-graph.cpp` |
| 22 | `topic/linked-list/ref/142_list-cycle-ii.cpp` | 142 | Linked List Cycle II | also `specialty/142_list-cycle-ii.cpp` |
| 23 | `company/bytedance/ng/01-lru-cache.cpp` | 146 | LRU Cache | also `topic/linked-list/ref/146_lru-cache.cpp`, `company/apple/ref/10-lru-cache.cpp`, `company/snowflake/ref/22-lru-cache-resize.cpp` |
| 24 | `company/bytedance/important/43-sort-list.cpp` | 148 | Sort List | also `topic/linked-list/ref/148_sort-list.cpp` |
| 25 | `topic/dp/ref/152_max-product-subarray.cpp` | 152 | Maximum Product Subarray |  |
| 26 | `company/bytedance/important/44-find-peak-element.cpp` | 162 | Find Peak Element | also `topic/binary-search/ref/162_find-peak.cpp`, `company/apple/ref/40-find-peak-element.cpp` |
| 27 | `company/bytedance/ng/08-house-robber.cpp` | 198 | House Robber | also `topic/dp/ref/198_house-robber.cpp`, `company/apple/ref/36-house-robber.cpp` |
| 28 | `topic/trees/ref/199_right-side-view.cpp` | 199 | Binary Tree Right Side View |  |
| 29 | `company/bytedance/ng/02-number-of-islands.cpp` | 200 | Number of Islands | also `topic/graphs/ref/200_number-of-islands.cpp`, `company/apple/ref/11-number-of-islands.cpp` |
| 30 | `company/bytedance/ng/04-course-schedule.cpp` | 207 | Course Schedule | also `topic/graphs/ref/207_course-schedule.cpp`, `company/apple/ref/12-course-schedule.cpp` |
| 31 | `company/bytedance/ng/16-maximal-square.cpp` | 221 | Maximal Square | also `topic/dp/ref/221_maximal-square.cpp` |
| 32 | `company/bytedance/ng/18-basic-calculator.cpp` | 224 | Basic Calculator | also `topic/parser/ref/224_basic-calculator.cpp`, `specialty/224_basic-calculator.cpp`, `company/snowflake/ref/48-basic-calculator.cpp` |
| 33 | `company/bytedance/ng/17-basic-calculator-ii.cpp` | 227 | Basic Calculator II | also `topic/parser/ref/227_basic-calculator-ii.cpp`, `company/apple/ref/29-basic-calculator-ii.cpp`, `company/snowflake/ref/48-basic-calculator.cpp` |
| 34 | `company/bytedance/important/45-sliding-window-max.cpp` | 239 | Sliding Window Maximum | also `specialty/239_sliding-window-max.cpp`, `topic/sliding-window/239_sliding-window-max.cpp` |
| 35 | `company/bytedance/important/66-longest-increasing-subsequence.cpp` | 300 | Longest Increasing Subsequence | also `topic/dp/ref/300_longest-increasing-subseq.cpp`, `specialty/300_longest-increasing-subseq.cpp` |
| 36 | `company/bytedance/optional/68-remove-duplicate-letters.cpp` | 316 | Remove Duplicate Letters | also `specialty/316_remove-duplicate-letters.cpp`, `topic/stack/316_remove-duplicate-letters.cpp` |
| 37 | `company/bytedance/optional/69-shortest-distance-buildings.cpp` | 317 | Shortest Distance from All Buildings |  |
| 38 | `company/bytedance/ng/19-top-k-frequent.cpp` | 347 | Top K Frequent Elements | also `company/apple/ref/14-top-k-frequent.cpp`, `company/snowflake/ref/52-top-k-frequent.cpp`, `topic/arrays-hashing/347_top-k-frequent.cpp` |
| 39 | `company/bytedance/important/48-delete-node-bst.cpp` | 450 | Delete Node in a BST | also `topic/trees/ref/450_delete-node-bst.cpp` |
| 40 | `company/bytedance/important/49-single-element-sorted.cpp` | 540 | Single Element in a Sorted Array | also `topic/binary-search/ref/540_single-element-sorted.cpp` |
| 41 | `company/bytedance/ng/22-longest-common-subsequence.cpp` | 1143 | Longest Common Subsequence | also `topic/dp/ref/1143_longest-common-subseq.cpp`, `company/apple/ref/38-longest-common-subsequence.cpp` |

## Phase 3 — ByteDance — hack2hire extras (43 to do)

Questions from the hack2hire ByteDance sets (`company/bytedance/`) that are not on the LeetCode list above. Files without an LC number are interview-style questions (no LeetCode link).

| # | File | LC | Problem | Note |
|---|---|---|---|---|
| 1 | `company/bytedance/ng/05-kth-largest-quickselect.cpp` | 215 | Kth largest quickselect |  |
| 2 | `company/bytedance/ng/24-longest-substring-no-repeat.cpp` | 3 | Longest substring no repeat |  |
| 3 | `company/bytedance/ng/25-implement-queue.cpp` | — | Implement queue |  |
| 4 | `company/bytedance/ng/26-path-sum-ii.cpp` | 113 | Path sum ii |  |
| 5 | `company/bytedance/ng/27-all-paths-directed.cpp` | — | All paths directed |  |
| 6 | `company/bytedance/ng/28-all-cycles-directed.cpp` | — | All cycles directed |  |
| 7 | `company/bytedance/ng/29-design-hashmap.cpp` | 706 | Design hashmap |  |
| 8 | `company/bytedance/ng/30-reverse-words.cpp` | 151 | Reverse words |  |
| 9 | `company/bytedance/important/31-subsets.cpp` | 78 | Subsets |  |
| 10 | `company/bytedance/important/32-add-strings-char-arrays.cpp` | 415 | Add strings char arrays |  |
| 11 | `company/bytedance/important/33-substructure-of-tree.cpp` | — | Substructure of tree |  |
| 12 | `company/bytedance/important/34-kth-node-from-end.cpp` | — | Kth node from end |  |
| 13 | `company/bytedance/important/56-valid-parentheses.cpp` | 20 | Valid parentheses |  |
| 14 | `company/bytedance/important/57-top-k-kols.cpp` | — | Top k kols |  |
| 15 | `company/bytedance/important/58-add-search-word.cpp` | 211 | Add search word |  |
| 16 | `company/bytedance/important/59-sqrt-precision.cpp` | — | Sqrt precision |  |
| 17 | `company/bytedance/important/60-permutation-in-string.cpp` | 567 | Permutation in string |  |
| 18 | `company/bytedance/important/61-lfu-cache.cpp` | 460 | Lfu cache |  |
| 19 | `company/bytedance/important/62-max-path-sum.cpp` | 124 | Max path sum |  |
| 20 | `company/bytedance/important/63-longest-palindromic-substring.cpp` | 5 | Longest palindromic substring |  |
| 21 | `company/bytedance/important/64-group-anagrams.cpp` | 49 | Group anagrams |  |
| 22 | `company/bytedance/optional/70-min-area-rectangle.cpp` | 939 | Min area rectangle |  |
| 23 | `company/bytedance/optional/72-k-length-substrings-no-repeat.cpp` | 1100 | K length substrings no repeat |  |
| 24 | `company/bytedance/optional/73-remove-duplicates-ii.cpp` | 80 | Remove duplicates ii |  |
| 25 | `company/bytedance/optional/74-missing-ranges.cpp` | 163 | Missing ranges |  |
| 26 | `company/bytedance/optional/75-swim-rising-water.cpp` | 778 | Swim rising water |  |
| 27 | `company/bytedance/optional/76-combinations.cpp` | 77 | Combinations |  |
| 28 | `company/bytedance/optional/77-photo-groups.cpp` | 547 | Photo groups |  |
| 29 | `company/bytedance/optional/78-valid-parenthesis-string.cpp` | 678 | Valid parenthesis string |  |
| 30 | `company/bytedance/optional/79-search-insert-position.cpp` | 35 | Search insert position |  |
| 31 | `company/bytedance/optional/80-circular-deque-index.cpp` | — | Circular deque index |  |
| 32 | `company/bytedance/optional/81-daily-login-counts.cpp` | — | Daily login counts |  |
| 33 | `company/bytedance/optional/82-min-cost-steps.cpp` | — | Min cost steps |  |
| 34 | `company/bytedance/optional/83-construct-pre-in.cpp` | 105 | Construct pre in |  |
| 35 | `company/bytedance/optional/84-populate-next-right.cpp` | 116 | Populate next right |  |
| 36 | `company/bytedance/optional/85-house-robber-iii.cpp` | 337 | House robber iii |  |
| 37 | `company/bytedance/optional/86-n-queens.cpp` | 51 | N queens |  |
| 38 | `company/bytedance/optional/87-largest-rectangle-histogram.cpp` | 84 | Largest rectangle histogram |  |
| 39 | `company/bytedance/optional/88-kth-smallest-subarray-sum.cpp` | 1918 | Kth smallest subarray sum |  |
| 40 | `company/bytedance/optional/89-camel-case-filter.cpp` | — | Camel case filter |  |
| 41 | `company/bytedance/optional/90-construct-pre-post.cpp` | 889 | Construct pre post |  |
| 42 | `company/bytedance/optional/91-max-candies.cpp` | 2226 | Max candies |  |
| 43 | `company/bytedance/optional/92-min-jumps-home.cpp` | 1654 | Min jumps home |  |

## Phase 4 — Amazon — most frequent (42 to do, 12 without a file)

The first 70 of the Amazon list (3-month frequency order).

| # | File | LC | Problem | Note |
|---|---|---|---|---|
| – | ~~`company/apple/ref/07-two-sum.cpp`~~ | 1 | Two Sum | skip — done in Phase 1 |
| 1 | — | 3413 | Maximum Coins From K Consecutive Bags |  |
| – | ~~`company/apple/ref/08-longest-substring-no-repeat.cpp`~~ | 3 | Longest Substring Without Repeating Characters | skip — done in Phase 3 |
| – | ~~`specialty/42_trapping-rain-water.cpp`~~ | 42 | Trapping Rain Water | skip — done in Phase 2 |
| – | ~~`topic/linked-list/ref/146_lru-cache.cpp`~~ | 146 | LRU Cache | skip — done in Phase 2 |
| – | ~~`company/apple/ref/13-group-anagrams.cpp`~~ | 49 | Group Anagrams | skip — done in Phase 3 |
| – | ~~`company/apple/ref/19-best-time-stock.cpp`~~ | 121 | Best Time to Buy and Sell Stock | skip — done in Phase 2 |
| 2 | `topic/binary-search/ref/4_median-two-sorted-arrays.cpp` | 4 | Median of Two Sorted Arrays | also `specialty/4_median-two-sorted-arrays.cpp` |
| – | ~~`topic/dp/ref/5_longest-palindromic-substring.cpp`~~ | 5 | Longest Palindromic Substring | skip — done in Phase 3 |
| 3 | `specialty/11_container-with-most-water.cpp` | 11 | Container With Most Water | also `topic/two-pointers/11_container-with-most-water.cpp` |
| – | ~~`topic/greedy-intervals/ref/56_merge-intervals.cpp`~~ | 56 | Merge Intervals | skip — done in Phase 2 |
| 4 | `company/apple/ref/37-move-zeroes.cpp` | 283 | Move Zeroes |  |
| – | ~~`company/apple/ref/14-top-k-frequent.cpp`~~ | 347 | Top K Frequent Elements | skip — done in Phase 2 |
| 5 | `topic/binary-search/ref/875_koko-bananas.cpp` | 875 | Koko Eating Bananas |  |
| – | ~~`topic/linked-list/ref/2_add-two-numbers.cpp`~~ | 2 | Add Two Numbers | skip — done in Phase 1 |
| – | ~~`company/apple/ref/30-three-sum.cpp`~~ | 15 | 3Sum | skip — done in Phase 2 |
| – | ~~`topic/greedy-intervals/ref/53_max-subarray.cpp`~~ | 53 | Maximum Subarray | skip — done in Phase 2 |
| – | ~~`company/apple/ref/32-subarray-sum-k.cpp`~~ | 560 | Subarray Sum Equals K | skip — done in Phase 1 |
| 6 | `company/apple/ref/16-longest-common-prefix.cpp` | 14 | Longest Common Prefix |  |
| – | ~~`topic/graphs/ref/200_number-of-islands.cpp`~~ | 200 | Number of Islands | skip — done in Phase 2 |
| 7 | `topic/trees/ref/236_lca-binary-tree.cpp` | 236 | Lowest Common Ancestor of a Binary Tree |  |
| 8 | `topic/trie-heap/ref/767_reorganize-string.cpp` | 767 | Reorganize String |  |
| – | ~~`topic/graphs/ref/994_rotting-oranges.cpp`~~ | 994 | Rotting Oranges | skip — done in Phase 1 |
| – | ~~`company/snowflake/ref/30-valid-parentheses.cpp`~~ | 20 | Valid Parentheses | skip — done in Phase 3 |
| – | ~~`topic/stack/22_generate-parentheses.cpp`~~ | 22 | Generate Parentheses | skip — done in Phase 2 |
| – | ~~`topic/graphs/ref/207_course-schedule.cpp`~~ | 207 | Course Schedule | skip — done in Phase 2 |
| 9 | `specialty/238_product-except-self.cpp` | 238 | Product of Array Except Self | also `topic/arrays-hashing/238_product-except-self.cpp` |
| 10 | — | 9 | Palindrome Number |  |
| 11 | `company/apple/ref/18-valid-sudoku.cpp` | 36 | Valid Sudoku | also `topic/arrays-hashing/36_valid-sudoku.cpp` |
| 12 | `company/apple/ref/41-longest-consecutive.cpp` | 128 | Longest Consecutive Sequence | also `specialty/128_longest-consecutive.cpp`, `topic/arrays-hashing/128_longest-consecutive.cpp` |
| 13 | — | 7 | Reverse Integer |  |
| – | ~~`topic/binary-search/ref/33_search-rotated-array.cpp`~~ | 33 | Search in Rotated Sorted Array | skip — done in Phase 2 |
| 14 | — | 69 | Sqrt(x) |  |
| 15 | `specialty/169_majority-element.cpp` | 169 | Majority Element | also `topic/arrays-hashing/169_majority-element.cpp` |
| 16 | `topic/trie-heap/ref/621_task-scheduler.cpp` | 621 | Task Scheduler | also `company/apple/ref/33-task-scheduler.cpp`, `specialty/621_task-scheduler.cpp` |
| 17 | — | 1011 | Capacity To Ship Packages Within D Days |  |
| – | ~~`topic/linked-list/ref/23_merge-k-lists.cpp`~~ | 23 | Merge k Sorted Lists | skip — done in Phase 2 |
| 18 | — | 26 | Remove Duplicates from Sorted Array |  |
| 19 | `topic/linked-list/ref/138_copy-random-list.cpp` | 138 | Copy List with Random Pointer | also `company/snowflake/ref/26-copy-random-list.cpp` |
| 20 | `topic/linked-list/ref/141_list-cycle.cpp` | 141 | Linked List Cycle |  |
| 21 | `topic/arrays-hashing/242_valid-anagram.cpp` | 242 | Valid Anagram |  |
| 22 | `topic/stack/ref/735_asteroid-collision.cpp` | 735 | Asteroid Collision |  |
| 23 | `topic/sliding-window/ref/904_fruit-into-baskets.cpp` | 904 | Fruit Into Baskets |  |
| 24 | — | 13 | Roman to Integer |  |
| 25 | `specialty/31_next-permutation.cpp` | 31 | Next Permutation | also `topic/two-pointers/31_next-permutation.cpp` |
| 26 | `topic/dp/ref/70_climbing-stairs.cpp` | 70 | Climbing Stairs |  |
| – | ~~`specialty/239_sliding-window-max.cpp`~~ | 239 | Sliding Window Maximum | skip — done in Phase 2 |
| 27 | `topic/binary-search/ref/410_split-array-largest-sum.cpp` | 410 | Split Array Largest Sum | also `specialty/410_split-array-largest-sum.cpp` |
| – | ~~`topic/binary-search/ref/540_single-element-sorted.cpp`~~ | 540 | Single Element in a Sorted Array | skip — done in Phase 2 |
| – | ~~`topic/backtracking/ref/78_subsets.cpp`~~ | 78 | Subsets | skip — done in Phase 3 |
| 28 | `topic/two-pointers/88_merge-sorted-array.cpp` | 88 | Merge Sorted Array |  |
| – | ~~`topic/trees/ref/124_max-path-sum.cpp`~~ | 124 | Binary Tree Maximum Path Sum | skip — done in Phase 3 |
| – | ~~`topic/dp/ref/198_house-robber.cpp`~~ | 198 | House Robber | skip — done in Phase 2 |
| 29 | `topic/greedy-intervals/ref/253_meeting-rooms-ii.cpp` | 253 | Meeting Rooms II | also `company/snowflake/ref/25-meeting-rooms-bus.cpp` |
| 30 | `topic/trie-heap/ref/295_median-stream.cpp` | 295 | Find Median from Data Stream | also `company/apple/ref/35-median-stream.cpp`, `company/snowflake/ref/51-median-stream.cpp` |
| – | ~~`topic/stack/ref/402_remove-k-digits.cpp`~~ | 402 | Remove K Digits | skip — done in Phase 1 |
| 31 | `topic/sliding-window/ref/1004_max-consecutive-ones-iii.cpp` | 1004 | Max Consecutive Ones III |  |
| 32 | — | 1929 | Concatenation of Array |  |
| 33 | — | 2938 | Separate Black and White Balls |  |
| 34 | — | 3914 | Minimum Operations to Make Array Non Decreasing |  |
| 35 | `topic/binary-search/ref/34_first-last-position.cpp` | 34 | Find First and Last Position of Element in Sorted Array | also `company/apple/ref/24-first-last-position.cpp`, `company/snowflake/ref/19-binary-search-queries.cpp` |
| 36 | `topic/sliding-window/76_min-window-substring.cpp` | 76 | Minimum Window Substring |  |
| 37 | — | 97 | Interleaving String |  |
| – | ~~`topic/graphs/ref/127_word-ladder.cpp`~~ | 127 | Word Ladder | skip — done in Phase 2 |
| – | ~~`topic/trie-heap/ref/215_kth-largest.cpp`~~ | 215 | Kth Largest Element in an Array | skip — done in Phase 3 |
| 38 | `topic/dp/ref/322_coin-change.cpp` | 322 | Coin Change |  |
| 39 | — | 643 | Maximum Average Subarray I |  |
| 40 | `topic/binary-search/ref/704_binary-search.cpp` | 704 | Binary Search | also `company/snowflake/ref/19-binary-search-queries.cpp` |
| 41 | `topic/stack/ref/739_daily-temperatures.cpp` | 739 | Daily Temperatures | also `specialty/739_daily-temperatures.cpp` |
| 42 | `topic/two-pointers/18_four-sum.cpp` | 18 | 4Sum |  |

## Phase 5 — Amazon — frequent (67 to do, 24 without a file)

List positions 71–150.

| # | File | LC | Problem | Note |
|---|---|---|---|---|
| 1 | `topic/linked-list/ref/21_merge-two-lists.cpp` | 21 | Merge Two Sorted Lists |  |
| 2 | `topic/string-matching/ref/28_find-first-occurrence.cpp` | 28 | Find the Index of the First Occurrence in a String | also `specialty/28_find-first-occurrence.cpp` |
| 3 | `specialty/32_longest-valid-parentheses.cpp` | 32 | Longest Valid Parentheses |  |
| – | ~~`company/bytedance/optional/79-search-insert-position.cpp`~~ | 35 | Search Insert Position | skip — done in Phase 3 |
| – | ~~`topic/bit-math-matrix/ref/48_rotate-image.cpp`~~ | 48 | Rotate Image | skip — done in Phase 2 |
| 4 | `topic/trees/ref/98_validate-bst.cpp` | 98 | Validate Binary Search Tree |  |
| 5 | — | 118 | Pascal's Triangle |  |
| 6 | `topic/dp/ref/139_word-break.cpp` | 139 | Word Break |  |
| 7 | `topic/graphs/ref/210_course-schedule-ii.cpp` | 210 | Course Schedule II | also `company/snowflake/ref/05-course-schedule-ii.cpp` |
| 8 | `topic/arrays-hashing/217_contains-duplicate.cpp` | 217 | Contains Duplicate |  |
| 9 | — | 240 | Search a 2D Matrix II |  |
| 10 | `topic/stack/ref/503_next-greater-ii.cpp` | 503 | Next Greater Element II |  |
| 11 | — | 1111 | Maximum Nesting Depth of Two Valid Parentheses Strings |  |
| 12 | — | 3806 | Maximum Bitwise AND After Increment Operations |  |
| 13 | `topic/parser/ref/8_atoi.cpp` | 8 | String to Integer (atoi) | also `company/apple/ref/48-string-to-integer-atoi.cpp` |
| 14 | — | 12 | Integer to Roman |  |
| 15 | `topic/linked-list/ref/25_reverse-k-group.cpp` | 25 | Reverse Nodes in k-Group |  |
| 16 | `topic/bit-math-matrix/ref/29_divide-two-integers.cpp` | 29 | Divide Two Integers |  |
| 17 | `topic/backtracking/ref/46_permutations.cpp` | 46 | Permutations |  |
| – | ~~`topic/backtracking/ref/51_n-queens.cpp`~~ | 51 | N-Queens | skip — done in Phase 3 |
| 18 | — | 58 | Length of Last Word |  |
| – | ~~`specialty/84_largest-rectangle.cpp`~~ | 84 | Largest Rectangle in Histogram | skip — done in Phase 3 |
| 19 | `topic/greedy-intervals/ref/135_candy.cpp` | 135 | Candy | also `specialty/135_candy.cpp` |
| 20 | `topic/bit-math-matrix/ref/136_single-number.cpp` | 136 | Single Number | also `specialty/136_single-number.cpp` |
| – | ~~`topic/dp/ref/152_max-product-subarray.cpp`~~ | 152 | Maximum Product Subarray | skip — done in Phase 2 |
| 21 | `topic/binary-search/ref/153_rotated-array-min.cpp` | 153 | Find Minimum in Rotated Sorted Array | also `company/snowflake/ref/32-rotated-array-min.cpp` |
| 22 | `topic/two-pointers/167_two-sum-ii.cpp` | 167 | Two Sum II - Input Array Is Sorted |  |
| 23 | — | 175 | Combine Two Tables |  |
| 24 | — | 197 | Rising Temperature |  |
| 25 | `topic/arrays-hashing/303_range-sum-query.cpp` | 303 | Range Sum Query - Immutable |  |
| 26 | `company/apple/ref/27-insert-delete-getrandom.cpp` | 380 | Insert Delete GetRandom O(1) | also `specialty/380_insert-delete-getrandom.cpp`, `topic/arrays-hashing/380_insert-delete-getrandom.cpp` |
| 27 | `topic/advanced-graphs/ref/399_evaluate-division.cpp` | 399 | Evaluate Division | also `specialty/399_evaluate-division.cpp` |
| 28 | — | 525 | Contiguous Array |  |
| 29 | `topic/trie-heap/ref/973_k-closest-points.cpp` | 973 | K Closest Points to Origin |  |
| 30 | — | 2271 | Maximum White Tiles Covered by a Carpet |  |
| 31 | — | 3720 | Lexicographically Smallest Permutation Greater Than Target |  |
| 32 | `topic/hard/ref/6_zigzag-conversion.cpp` | 6 | Zigzag Conversion |  |
| 33 | `topic/backtracking/ref/17_phone-letters.cpp` | 17 | Letter Combinations of a Phone Number |  |
| 34 | — | 27 | Remove Element |  |
| 35 | `topic/greedy-intervals/ref/45_jump-game-ii.cpp` | 45 | Jump Game II |  |
| 36 | `topic/bit-math-matrix/ref/54_spiral-matrix.cpp` | 54 | Spiral Matrix | also `company/apple/ref/23-spiral-matrix.cpp` |
| 37 | `topic/linked-list/ref/61_rotate-list.cpp` | 61 | Rotate List |  |
| 38 | `topic/dp/ref/122_stock-ii.cpp` | 122 | Best Time to Buy and Sell Stock II |  |
| 39 | `company/apple/ref/15-valid-palindrome.cpp` | 125 | Valid Palindrome | also `topic/two-pointers/125_valid-palindrome.cpp` |
| 40 | `topic/linked-list/ref/143_reorder-list.cpp` | 143 | Reorder List | also `company/apple/ref/43-reorder-list.cpp` |
| 41 | `topic/stack/ref/155_min-stack.cpp` | 155 | Min Stack | also `company/snowflake/ref/53-min-stack.cpp` |
| 42 | `topic/hard/ref/179_largest-number.cpp` | 179 | Largest Number |  |
| 43 | `specialty/189_rotate-array.cpp` | 189 | Rotate Array | also `company/apple/ref/42-rotate-array.cpp` |
| 44 | — | 205 | Isomorphic Strings |  |
| 45 | `topic/linked-list/ref/206_reverse-list.cpp` | 206 | Reverse Linked List | also `company/apple/ref/20-reverse-linked-list.cpp` |
| – | ~~`topic/parser/ref/224_basic-calculator.cpp`~~ | 224 | Basic Calculator | skip — done in Phase 2 |
| 46 | `topic/parser/ref/273_integer-to-english.cpp` | 273 | Integer to English Words |  |
| 47 | `specialty/287_find-duplicate.cpp` | 287 | Find the Duplicate Number | also `topic/arrays-hashing/287_find-duplicate.cpp` |
| 48 | `topic/linked-list/ref/328_odd-even-list.cpp` | 328 | Odd Even Linked List |  |
| 49 | — | 344 | Reverse String |  |
| 50 | — | 486 | Predict the Winner |  |
| 51 | `topic/stack/ref/496_next-greater-i.cpp` | 496 | Next Greater Element I |  |
| – | ~~`topic/sliding-window/567_permutation-in-string.cpp`~~ | 567 | Permutation in String | skip — done in Phase 3 |
| 52 | `topic/conditional/ref/680_valid-palindrome-ii.cpp` | 680 | Valid Palindrome II |  |
| 53 | — | 724 | Find Pivot Index |  |
| 54 | `topic/advanced-graphs/ref/787_cheapest-flights-k-stops.cpp` | 787 | Cheapest Flights Within K Stops | also `company/apple/ref/54-cheapest-flights-k-stops.cpp` |
| 55 | `topic/graphs/ref/815_bus-routes.cpp` | 815 | Bus Routes |  |
| 56 | `topic/trees/ref/863_nodes-distance-k.cpp` | 863 | All Nodes Distance K in Binary Tree |  |
| 57 | — | 876 | Middle of the Linked List |  |
| 58 | — | 1148 | Article Views I |  |
| 59 | — | 1248 | Count Number of Nice Subarrays |  |
| 60 | — | 1757 | Recyclable and Low Fat Products |  |
| 61 | — | 2448 | Minimum Cost to Make Array Equal |  |
| 62 | — | 2916 | Subarrays Distinct Element Sum of Squares II |  |
| – | ~~`topic/two-pointers/16_three-sum-closest.cpp`~~ | 16 | 3Sum Closest | skip — done in Phase 2 |
| 63 | `specialty/41_first-missing-positive.cpp` | 41 | First Missing Positive | also `topic/arrays-hashing/41_first-missing-positive.cpp` |
| – | ~~`topic/bit-math-matrix/ref/43_multiply-strings.cpp`~~ | 43 | Multiply Strings | skip — done in Phase 2 |
| – | ~~`topic/greedy-intervals/ref/55_jump-game.cpp`~~ | 55 | Jump Game | skip — done in Phase 2 |
| 64 | `specialty/75_sort-colors.cpp` | 75 | Sort Colors | also `topic/two-pointers/75_sort-colors.cpp` |
| – | ~~`company/bytedance/optional/76-combinations.cpp`~~ | 77 | Combinations | skip — done in Phase 3 |
| – | ~~`topic/two-pointers/80_remove-duplicates-ii.cpp`~~ | 80 | Remove Duplicates from Sorted Array II | skip — done in Phase 3 |
| 65 | — | 81 | Search in Rotated Sorted Array II |  |
| – | ~~`topic/linked-list/ref/92_reverse-list-ii.cpp`~~ | 92 | Reverse Linked List II | skip — done in Phase 2 |
| 66 | — | 100 | Same Tree |  |
| 67 | `topic/trees/ref/102_level-order.cpp` | 102 | Binary Tree Level Order Traversal |  |

## Phase 6 — Amazon — rest (73 to do, 37 without a file)

List positions 151 to the end of the pasted list (it was cut at 437).

| # | File | LC | Problem | Note |
|---|---|---|---|---|
| 1 | `topic/trees/ref/108_sorted-array-to-bst.cpp` | 108 | Convert Sorted Array to Binary Search Tree |  |
| 2 | — | 110 | Balanced Binary Tree |  |
| 3 | `topic/backtracking/ref/131_palindrome-partition.cpp` | 131 | Palindrome Partitioning |  |
| 4 | `topic/greedy-intervals/ref/134_gas-station.cpp` | 134 | Gas Station | also `specialty/134_gas-station.cpp` |
| – | ~~`topic/trees/ref/199_right-side-view.cpp`~~ | 199 | Binary Tree Right Side View | skip — done in Phase 2 |
| 5 | `company/snowflake/ref/59-happy-number.cpp` | 202 | Happy Number |  |
| 6 | `topic/trie-heap/ref/212_word-search-ii.cpp` | 212 | Word Search II | also `company/snowflake/ref/12-word-search-ii.cpp` |
| 7 | `topic/sliding-window/219_contains-duplicate-ii.cpp` | 219 | Contains Duplicate II |  |
| – | ~~`topic/parser/ref/227_basic-calculator-ii.cpp`~~ | 227 | Basic Calculator II | skip — done in Phase 2 |
| 8 | — | 229 | Majority Element II |  |
| 9 | `topic/linked-list/ref/234_palindrome-list.cpp` | 234 | Palindrome Linked List |  |
| 10 | `topic/greedy-intervals/ref/252_meeting-rooms.cpp` | 252 | Meeting Rooms | also `company/snowflake/ref/25-meeting-rooms-bus.cpp` |
| – | ~~`specialty/316_remove-duplicate-letters.cpp`~~ | 316 | Remove Duplicate Letters | skip — done in Phase 2 |
| 11 | — | 349 | Intersection of Two Arrays |  |
| 12 | — | 387 | First Unique Character in a String |  |
| 13 | — | 412 | Fizz Buzz |  |
| 14 | `topic/dp/ref/416_partition-equal-subset-sum.cpp` | 416 | Partition Equal Subset Sum | also `specialty/416_partition-equal-subset-sum.cpp` |
| 15 | `topic/graphs/ref/417_pacific-atlantic.cpp` | 417 | Pacific Atlantic Water Flow |  |
| 16 | `topic/greedy-intervals/ref/435_non-overlapping-intervals.cpp` | 435 | Non-overlapping Intervals | also `specialty/435_non-overlapping-intervals.cpp` |
| 17 | `company/snowflake/ref/31-find-anagrams.cpp` | 438 | Find All Anagrams in a String | also `topic/sliding-window/438_find-anagrams.cpp` |
| 18 | `topic/parser/ref/443_string-compression.cpp` | 443 | String Compression | also `company/apple/ref/44-string-compression.cpp` |
| – | ~~`topic/trees/ref/450_delete-node-bst.cpp`~~ | 450 | Delete Node in a BST | skip — done in Phase 2 |
| 19 | — | 451 | Sort Characters By Frequency |  |
| 20 | `topic/stack/ref/456_pattern-132.cpp` | 456 | 132 Pattern |  |
| – | ~~`topic/design/ref/460_lfu-cache.cpp`~~ | 460 | LFU Cache | skip — done in Phase 3 |
| 21 | — | 493 | Reverse Pairs |  |
| 22 | — | 517 | Super Washing Machines |  |
| 23 | `topic/sliding-window/523_continuous-subarray-sum.cpp` | 523 | Continuous Subarray Sum |  |
| 24 | `specialty/528_random-pick-weight.cpp` | 528 | Random Pick with Weight | also `topic/arrays-hashing/528_random-pick-weight.cpp` |
| 25 | `topic/trees/ref/543_diameter-of-tree.cpp` | 543 | Diameter of Binary Tree |  |
| – | ~~`topic/graphs/ref/547_provinces.cpp`~~ | 547 | Number of Provinces | skip — done in Phase 3 |
| 26 | — | 577 | Employee Bonus |  |
| 27 | — | 628 | Maximum Product of Three Numbers |  |
| 28 | `topic/arrays-hashing/645_set-mismatch.cpp` | 645 | Set Mismatch |  |
| 29 | `company/apple/ref/26-subarray-product-less-k.cpp` | 713 | Subarray Product Less Than K |  |
| 30 | — | 733 | Flood Fill |  |
| 31 | — | 836 | Rectangle Overlap |  |
| 32 | `topic/monotonic-sweep/ref/862_shortest-subarray-sum-k.cpp` | 862 | Shortest Subarray with Sum at Least K |  |
| 33 | `topic/advanced-graphs/ref/990_equality-equations.cpp` | 990 | Satisfiability of Equality Equations |  |
| 34 | — | 1021 | Remove Outermost Parentheses |  |
| 35 | `topic/graphs/ref/1094_car-pooling.cpp` | 1094 | Car Pooling |  |
| 36 | — | 1140 | Stone Game II |  |
| 37 | — | 1423 | Maximum Points You Can Obtain from Cards |  |
| 38 | — | 1429 | First Unique Number |  |
| 39 | — | 1552 | Magnetic Force Between Two Balls |  |
| 40 | — | 1581 | Customer Who Visited but Did Not Make Any Transactions |  |
| 41 | — | 1752 | Check if Array Is Sorted and Rotated |  |
| 42 | — | 1834 | Single-Threaded CPU |  |
| 43 | — | 1838 | Frequency of the Most Frequent Element |  |
| 44 | — | 2029 | Stone Game IX |  |
| 45 | — | 2134 | Minimum Swaps to Group All 1's Together II |  |
| 46 | — | 2235 | Add Two Integers |  |
| 47 | — | 3414 | Maximum Score of Non-overlapping Intervals |  |
| 48 | — | 4007 | Widest Possible Fence |  |
| – | ~~`topic/dp/ref/10_regex-matching.cpp`~~ | 10 | Regular Expression Matching | skip — done in Phase 2 |
| 49 | `topic/linked-list/ref/19_remove-nth-from-end.cpp` | 19 | Remove Nth Node From End of List |  |
| 50 | — | 44 | Wildcard Matching |  |
| 51 | `topic/bit-math-matrix/ref/50_pow.cpp` | 50 | Pow(x, n) |  |
| 52 | `topic/greedy-intervals/ref/57_insert-interval.cpp` | 57 | Insert Interval |  |
| – | ~~`topic/dp/ref/62_unique-paths.cpp`~~ | 62 | Unique Paths | skip — done in Phase 2 |
| 53 | `topic/backtracking/ref/79_word-search.cpp` | 79 | Word Search |  |
| 54 | `topic/stack/85_maximal-rectangle.cpp` | 85 | Maximal Rectangle |  |
| 55 | — | 101 | Symmetric Tree |  |
| 56 | `topic/trees/ref/104_max-depth.cpp` | 104 | Maximum Depth of Binary Tree |  |
| – | ~~`company/bytedance/ng/26-path-sum-ii.cpp`~~ | 113 | Path Sum II | skip — done in Phase 3 |
| 57 | — | 115 | Distinct Subsequences |  |
| 58 | `topic/dp/ref/140_word-break-ii.cpp` | 140 | Word Break II |  |
| – | ~~`topic/linked-list/ref/142_list-cycle-ii.cpp`~~ | 142 | Linked List Cycle II | skip — done in Phase 2 |
| 59 | — | 144 | Binary Tree Preorder Traversal |  |
| 60 | `topic/stack/150_eval-rpn.cpp` | 150 | Evaluate Reverse Polish Notation |  |
| 61 | — | 177 | Nth Highest Salary |  |
| 62 | — | 184 | Department Highest Salary |  |
| 63 | — | 187 | Repeated DNA Sequences |  |
| 64 | `topic/bit-math-matrix/ref/204_count-primes.cpp` | 204 | Count Primes |  |
| 65 | `topic/sliding-window/209_min-size-subarray-sum.cpp` | 209 | Minimum Size Subarray Sum |  |
| 66 | — | 216 | Combination Sum III |  |
| 67 | — | 231 | Power of Two |  |
| 68 | `topic/graphs/ref/269_alien-dictionary.cpp` | 269 | Alien Dictionary | also `specialty/269_alien-dictionary.cpp` |
| 69 | `topic/trees/ref/297_serialize-tree.cpp` | 297 | Serialize and Deserialize Binary Tree | also `specialty/297_serialize-tree.cpp` |
| – | ~~`topic/dp/ref/300_longest-increasing-subseq.cpp`~~ | 300 | Longest Increasing Subsequence | skip — done in Phase 2 |
| 70 | `topic/dp/ref/312_burst-balloons.cpp` | 312 | Burst Balloons | also `specialty/312_burst-balloons.cpp` |
| – | ~~`topic/dp/ref/337_house-robber-iii.cpp`~~ | 337 | House Robber III | skip — done in Phase 3 |
| 71 | — | 383 | Ransom Note |  |
| 72 | — | 389 | Find the Difference |  |
| 73 | `topic/trees/ref/437_path-sum-iii.cpp` | 437 | Path Sum III |  |

## Phase 7 — Apple must (ng) (6 to do)

Bit ops first (Silicon Validation role), then the LC list top by frequency, then the hack2hire Apple questions.

| # | File | LC | Problem | Note |
|---|---|---|---|---|
| 1 | `company/apple/ref/02-bit-ops.cpp` | — | Bit ops |  |
| – | ~~`company/apple/ref/07-two-sum.cpp`~~ | 1 | Two Sum | skip — done in Phase 1 |
| – | ~~`company/apple/ref/08-longest-substring-no-repeat.cpp`~~ | 3 | Longest Substring Without Repeating Characters | skip — done in Phase 3 |
| – | ~~`company/apple/ref/09-merge-intervals.cpp`~~ | 56 | Merge Intervals | skip — done in Phase 2 |
| – | ~~`company/apple/ref/10-lru-cache.cpp`~~ | 146 | LRU Cache | skip — done in Phase 2 |
| – | ~~`company/apple/ref/11-number-of-islands.cpp`~~ | 200 | Number of Islands | skip — done in Phase 2 |
| – | ~~`company/apple/ref/12-course-schedule.cpp`~~ | 207 | Course Schedule | skip — done in Phase 2 |
| – | ~~`company/apple/ref/13-group-anagrams.cpp`~~ | 49 | Group Anagrams | skip — done in Phase 3 |
| – | ~~`company/apple/ref/15-valid-palindrome.cpp`~~ | 125 | Valid Palindrome | skip — done in Phase 5 |
| – | ~~`company/apple/ref/14-top-k-frequent.cpp`~~ | 347 | Top K Frequent Elements | skip — done in Phase 2 |
| 2 | `company/apple/ref/01-palindrome-score.cpp` | — | Palindrome score |  |
| 3 | `company/apple/ref/03-bipartite.cpp` | 785 | Bipartite | also `topic/graphs/ref/785_is-bipartite.cpp`, `company/snowflake/ref/47-bipartition.cpp` |
| 4 | `company/apple/ref/04-subset-sum-dp.cpp` | — | Subset sum dp |  |
| 5 | `company/apple/ref/05-subsets-with-sum.cpp` | — | Subsets with sum |  |
| 6 | `company/apple/ref/06-highway-toll.cpp` | — | Highway toll |  |

## Phase 8 — Apple important (4 to do)

Rest of the Apple LC list, already ordered by frequency.

| # | File | LC | Problem | Note |
|---|---|---|---|---|
| – | ~~`company/apple/ref/16-longest-common-prefix.cpp`~~ | 14 | Longest Common Prefix | skip — done in Phase 4 |
| – | ~~`company/apple/ref/17-merge-k-sorted-lists.cpp`~~ | 23 | Merge k Sorted Lists | skip — done in Phase 2 |
| – | ~~`company/apple/ref/18-valid-sudoku.cpp`~~ | 36 | Valid Sudoku | skip — done in Phase 4 |
| – | ~~`company/apple/ref/19-best-time-stock.cpp`~~ | 121 | Best Time to Buy and Sell Stock | skip — done in Phase 2 |
| – | ~~`company/apple/ref/20-reverse-linked-list.cpp`~~ | 206 | Reverse Linked List | skip — done in Phase 5 |
| – | ~~`company/apple/ref/21-word-ladder.cpp`~~ | 127 | Word Ladder | skip — done in Phase 2 |
| 1 | `company/apple/ref/22-design-hit-counter.cpp` | 362 | Design hit counter | also `topic/design/ref/362_hit-counter.cpp`, `company/snowflake/ref/44-hit-counter.cpp` |
| – | ~~`company/apple/ref/23-spiral-matrix.cpp`~~ | 54 | Spiral Matrix | skip — done in Phase 5 |
| – | ~~`company/apple/ref/24-first-last-position.cpp`~~ | 34 | Find First and Last Position of Element in Sorted Array | skip — done in Phase 4 |
| 2 | `company/apple/ref/25-time-map.cpp` | 981 | Time map | also `topic/binary-search/ref/981_time-map.cpp`, `company/snowflake/ref/27-time-map-out-of-order.cpp` |
| – | ~~`company/apple/ref/26-subarray-product-less-k.cpp`~~ | 713 | Subarray Product Less Than K | skip — done in Phase 6 |
| – | ~~`company/apple/ref/27-insert-delete-getrandom.cpp`~~ | 380 | Insert Delete GetRandom O(1) | skip — done in Phase 5 |
| 3 | `company/apple/ref/28-snapshot-array.cpp` | 1146 | Snapshot array | also `topic/design/ref/1146_snapshot-array.cpp` |
| – | ~~`company/apple/ref/29-basic-calculator-ii.cpp`~~ | 227 | Basic Calculator II | skip — done in Phase 2 |
| – | ~~`company/apple/ref/30-three-sum.cpp`~~ | 15 | 3Sum | skip — done in Phase 2 |
| – | ~~`company/apple/ref/31-longest-palindromic-substring.cpp`~~ | 5 | Longest Palindromic Substring | skip — done in Phase 3 |
| – | ~~`company/apple/ref/32-subarray-sum-k.cpp`~~ | 560 | Subarray Sum Equals K | skip — done in Phase 1 |
| – | ~~`company/apple/ref/33-task-scheduler.cpp`~~ | 621 | Task Scheduler | skip — done in Phase 4 |
| 4 | `company/apple/ref/34-edit-distance.cpp` | 72 | Edit distance | also `topic/dp/ref/72_edit-distance.cpp` |
| – | ~~`company/apple/ref/35-median-stream.cpp`~~ | 295 | Find Median from Data Stream | skip — done in Phase 4 |
| – | ~~`company/apple/ref/36-house-robber.cpp`~~ | 198 | House Robber | skip — done in Phase 2 |
| – | ~~`company/apple/ref/37-move-zeroes.cpp`~~ | 283 | Move Zeroes | skip — done in Phase 4 |
| – | ~~`company/apple/ref/38-longest-common-subsequence.cpp`~~ | 1143 | Longest Common Subsequence | skip — done in Phase 2 |
| – | ~~`company/apple/ref/39-jump-game.cpp`~~ | 55 | Jump Game | skip — done in Phase 2 |
| – | ~~`company/apple/ref/40-find-peak-element.cpp`~~ | 162 | Find Peak Element | skip — done in Phase 2 |
| – | ~~`company/apple/ref/41-longest-consecutive.cpp`~~ | 128 | Longest Consecutive Sequence | skip — done in Phase 4 |

## Phase 9 — Specialty tricks (12 to do)

One-trick problems; read the `// Trick:` line first, then type from memory. Bit tricks and index tricks matter most for a low-level C/C++ role.

| # | File | LC | Problem | Note |
|---|---|---|---|---|
| – | ~~`specialty/84_largest-rectangle.cpp`~~ | 84 | Largest Rectangle in Histogram | skip — done in Phase 3 |
| – | ~~`specialty/739_daily-temperatures.cpp`~~ | 739 | Daily Temperatures | skip — done in Phase 4 |
| – | ~~`specialty/402_remove-k-digits.cpp`~~ | 402 | Remove K Digits | skip — done in Phase 1 |
| – | ~~`specialty/316_remove-duplicate-letters.cpp`~~ | 316 | Remove Duplicate Letters | skip — done in Phase 2 |
| – | ~~`specialty/32_longest-valid-parentheses.cpp`~~ | 32 | Longest Valid Parentheses | skip — done in Phase 5 |
| – | ~~`specialty/239_sliding-window-max.cpp`~~ | 239 | Sliding Window Maximum | skip — done in Phase 2 |
| – | ~~`specialty/42_trapping-rain-water.cpp`~~ | 42 | Trapping Rain Water | skip — done in Phase 2 |
| – | ~~`specialty/41_first-missing-positive.cpp`~~ | 41 | First Missing Positive | skip — done in Phase 5 |
| 1 | `specialty/448_find-disappeared-numbers.cpp` | 448 | 448_find disappeared numbers | also `topic/arrays-hashing/448_find-disappeared-numbers.cpp` |
| – | ~~`specialty/287_find-duplicate.cpp`~~ | 287 | Find the Duplicate Number | skip — done in Phase 5 |
| – | ~~`specialty/142_list-cycle-ii.cpp`~~ | 142 | Linked List Cycle II | skip — done in Phase 2 |
| 2 | `specialty/160_intersection-of-lists.cpp` | 160 | 160_intersection of lists | also `topic/linked-list/ref/160_intersection-of-lists.cpp` |
| – | ~~`specialty/31_next-permutation.cpp`~~ | 31 | Next Permutation | skip — done in Phase 4 |
| – | ~~`specialty/169_majority-element.cpp`~~ | 169 | Majority Element | skip — done in Phase 4 |
| – | ~~`specialty/134_gas-station.cpp`~~ | 134 | Gas Station | skip — done in Phase 6 |
| – | ~~`specialty/238_product-except-self.cpp`~~ | 238 | Product of Array Except Self | skip — done in Phase 4 |
| – | ~~`specialty/75_sort-colors.cpp`~~ | 75 | Sort Colors | skip — done in Phase 5 |
| – | ~~`specialty/189_rotate-array.cpp`~~ | 189 | Rotate Array | skip — done in Phase 5 |
| – | ~~`specialty/48_rotate-image.cpp`~~ | 48 | Rotate Image | skip — done in Phase 2 |
| – | ~~`specialty/11_container-with-most-water.cpp`~~ | 11 | Container With Most Water | skip — done in Phase 4 |
| – | ~~`specialty/135_candy.cpp`~~ | 135 | Candy | skip — done in Phase 5 |
| – | ~~`specialty/528_random-pick-weight.cpp`~~ | 528 | Random Pick with Weight | skip — done in Phase 6 |
| – | ~~`specialty/560_subarray-sum-k.cpp`~~ | 560 | Subarray Sum Equals K | skip — done in Phase 1 |
| 3 | `specialty/974_subarray-sums-div-k.cpp` | 974 | 974_subarray sums div k | also `topic/arrays-hashing/974_subarray-sums-div-k.cpp` |
| – | ~~`specialty/128_longest-consecutive.cpp`~~ | 128 | Longest Consecutive Sequence | skip — done in Phase 4 |
| – | ~~`specialty/410_split-array-largest-sum.cpp`~~ | 410 | Split Array Largest Sum | skip — done in Phase 4 |
| – | ~~`specialty/4_median-two-sorted-arrays.cpp`~~ | 4 | Median of Two Sorted Arrays | skip — done in Phase 4 |
| – | ~~`specialty/380_insert-delete-getrandom.cpp`~~ | 380 | Insert Delete GetRandom O(1) | skip — done in Phase 5 |
| 4 | `specialty/382_random-list-node.cpp` | 382 | 382_random list node | also `topic/linked-list/ref/382_random-list-node.cpp` |
| 5 | `specialty/384_shuffle-array.cpp` | 384 | 384_shuffle array | also `topic/bit-math-matrix/ref/384_shuffle-array.cpp` |
| 6 | `specialty/470_rand10-using-rand7.cpp` | 470 | 470_rand10 using rand7 |  |
| – | ~~`specialty/215_kth-largest.cpp`~~ | 215 | Kth Largest Element in an Array | skip — done in Phase 3 |
| – | ~~`specialty/136_single-number.cpp`~~ | 136 | Single Number | skip — done in Phase 5 |
| 7 | `specialty/137_single-number-ii.cpp` | 137 | 137_single number ii | also `topic/bit-math-matrix/ref/137_single-number-ii.cpp` |
| 8 | `specialty/338_counting-bits.cpp` | 338 | 338_counting bits | also `topic/bit-math-matrix/ref/338_counting-bits.cpp` |
| – | ~~`specialty/621_task-scheduler.cpp`~~ | 621 | Task Scheduler | skip — done in Phase 4 |
| – | ~~`specialty/435_non-overlapping-intervals.cpp`~~ | 435 | Non-overlapping Intervals | skip — done in Phase 6 |
| 9 | `specialty/763_partition-labels.cpp` | 763 | 763_partition labels | also `topic/greedy-intervals/ref/763_partition-labels.cpp` |
| 10 | `specialty/snowcal.cpp` | — | Snowcal |  |
| – | ~~`specialty/394_decode-string.cpp`~~ | 394 | Decode String | skip — done in Phase 1 |
| – | ~~`specialty/224_basic-calculator.cpp`~~ | 224 | Basic Calculator | skip — done in Phase 2 |
| 11 | `specialty/271_encode-decode-strings.cpp` | 271 | 271_encode decode strings | also `company/snowflake/ref/10-encode-decode-strings.cpp`, `topic/arrays-hashing/271_encode-decode-strings.cpp` |
| – | ~~`specialty/297_serialize-tree.cpp`~~ | 297 | Serialize and Deserialize Binary Tree | skip — done in Phase 6 |
| – | ~~`specialty/399_evaluate-division.cpp`~~ | 399 | Evaluate Division | skip — done in Phase 5 |
| – | ~~`specialty/269_alien-dictionary.cpp`~~ | 269 | Alien Dictionary | skip — done in Phase 6 |
| 12 | `specialty/332_reconstruct-itinerary.cpp` | 332 | 332_reconstruct itinerary | also `topic/graphs/ref/332_reconstruct-itinerary.cpp` |
| – | ~~`specialty/300_longest-increasing-subseq.cpp`~~ | 300 | Longest Increasing Subsequence | skip — done in Phase 2 |
| – | ~~`specialty/312_burst-balloons.cpp`~~ | 312 | Burst Balloons | skip — done in Phase 6 |
| – | ~~`specialty/416_partition-equal-subset-sum.cpp`~~ | 416 | Partition Equal Subset Sum | skip — done in Phase 6 |
| – | ~~`specialty/28_find-first-occurrence.cpp`~~ | 28 | Find the Index of the First Occurrence in a String | skip — done in Phase 5 |

## Phase 10 — Apple optional (15 to do)

Lower-frequency / harder Apple LC problems. Skip `66-sv-constraints.sv` (Design Verification role only).

| # | File | LC | Problem | Note |
|---|---|---|---|---|
| – | ~~`company/apple/ref/42-rotate-array.cpp`~~ | 189 | Rotate Array | skip — done in Phase 5 |
| – | ~~`company/apple/ref/43-reorder-list.cpp`~~ | 143 | Reorder List | skip — done in Phase 5 |
| – | ~~`company/apple/ref/44-string-compression.cpp`~~ | 443 | String Compression | skip — done in Phase 6 |
| 1 | `company/apple/ref/45-last-stone-weight.cpp` | 1046 | Last stone weight | also `topic/trie-heap/ref/1046_last-stone-weight.cpp` |
| 2 | `company/apple/ref/46-maximum-swap.cpp` | 670 | Maximum swap |  |
| – | ~~`company/apple/ref/47-valid-parenthesis-string.cpp`~~ | 678 | Valid parenthesis string | skip — done in Phase 3 |
| – | ~~`company/apple/ref/48-string-to-integer-atoi.cpp`~~ | 8 | String to Integer (atoi) | skip — done in Phase 5 |
| 3 | `company/apple/ref/49-lonely-numbers.cpp` | 2150 | Lonely numbers |  |
| 4 | `company/apple/ref/50-total-hamming-distance.cpp` | 477 | Total hamming distance |  |
| 5 | `company/apple/ref/51-lca-parent-pointers.cpp` | 1650 | Lca parent pointers |  |
| 6 | `company/apple/ref/52-bst-greater-sum-tree.cpp` | 1038 | Bst greater sum tree |  |
| – | ~~`company/apple/ref/53-construct-pre-post.cpp`~~ | 889 | Construct pre post | skip — done in Phase 3 |
| – | ~~`company/apple/ref/54-cheapest-flights-k-stops.cpp`~~ | 787 | Cheapest Flights Within K Stops | skip — done in Phase 5 |
| 7 | `company/apple/ref/55-last-stone-weight-ii.cpp` | 1049 | Last stone weight ii |  |
| 8 | `company/apple/ref/56-knight-probability.cpp` | 688 | Knight probability |  |
| 9 | `company/apple/ref/57-flatten-nested-iterator.cpp` | 341 | Flatten nested iterator | also `topic/design/ref/341_flatten-nested-iterator.cpp` |
| – | ~~`company/apple/ref/58-design-memory-allocator.cpp`~~ | 2502 | Design Memory Allocator | skip — done in Phase 1 |
| 10 | `company/apple/ref/59-design-task-manager.cpp` | 3408 | Design task manager | also `company/snowflake/ref/16-task-executor-dedup.cpp`, `company/snowflake/ref/50-task-manager.cpp` |
| 11 | `company/apple/ref/60-unit-conversion-ii.cpp` | 3535 | Unit conversion ii |  |
| 12 | `company/apple/ref/61-exam-room.cpp` | 855 | Exam room |  |
| 13 | `company/apple/ref/62-meeting-rooms-iii.cpp` | 2402 | Meeting rooms iii |  |
| 14 | `company/apple/ref/63-min-cost-hire-k-workers.cpp` | 857 | Min cost hire k workers |  |
| – | ~~`company/apple/ref/64-lfu-cache.cpp`~~ | 460 | LFU Cache | skip — done in Phase 3 |
| 15 | `company/apple/ref/65-parse-lisp-expression.cpp` | 736 | Parse lisp expression |  |

## Phase 11 — Snowflake (only if it moves to another round)

`company/snowflake/prep/` is your own work; the AI references are `company/snowflake/ref/`. Many overlap with the phases above (LRU, merge intervals, course schedule, top-k, calculator, median, task manager).

`company/snowflake/ref/` is numbered `01`–`59` in tier order; its README is the order of attack. `company/snowflake/prep/word-pair-within-k.cpp` and `command-interpreter.cpp` are the two questions actually asked.
