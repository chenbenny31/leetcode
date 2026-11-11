# select — topic-organized core problems

68 core problems (母题) in 16 topic folders, plus 60 harder or niche ones moved to `optional/<topic>/`. Core = the basic algorithm each pattern needs, written so it can be typed bug-free live, plus the follow-up interviewers usually ask (e.g. binary search: lower vs upper bound and the `lo == 0` / `lo == n` checks before using the index). Do the core first; open `optional/` only when a target company is known to ask it. Inside a topic, files are numbered by priority (01 = do first). Every file is one runnable C++17 file in the live-interview style, hand-made tests printing `label PASS/FAIL`, and a `// follow-up:` block at the end.

Every problem is one C++ file `NN-slug.cpp`. Run: `g++ -std=c++17 -Wall -Wextra -fsanitize=address,undefined F.cpp && ./a.out`.

Tags = companies whose reports included the same LeetCode problem (historical; those company folders are gone).

## 01-arrays-hashing

Hash map / set as the index; prefix sums counted in a map; index-as-hash for O(1) space.

| # | File | LC | Difficulty | Pattern | Tags |
|---|---|---|---|---|---|
| 01 | `01-two-sum.cpp` | 1 | Easy | hash map of earlier values | Apple |
| 02 | `02-subarray-sum-k.cpp` | 560 | Medium | prefix sum + hash map count | Apple, ByteDance |
| 03 | `03-group-anagrams.cpp` | 49 | Medium | hash map with sorted / count key | Amazon, Apple, ByteDance |
| 04 | `04-longest-consecutive.cpp` | 128 | Medium | hash set, count from run heads | Amazon |
| 05 | `05-product-except-self.cpp` | 238 | Medium | prefix * suffix products, no division | — |
| 06 | `optional/01-arrays-hashing/06-valid-sudoku.cpp` | 36 | Medium | seen flags per row, column, box | Apple |
| 07 | `optional/01-arrays-hashing/07-first-missing-positive.cpp` | 41 | Hard | cyclic sort, index as hash | Apple |

Apple variants (no separate file): highway toll, flaky tests, lonely numbers -> hash grouping as in 03 group-anagrams.

## 02-two-pointers

Sorted input or two ends: move the pointer whose side cannot improve; write pointer for in-place.

| # | File | LC | Difficulty | Pattern | Tags |
|---|---|---|---|---|---|
| 01 | `01-three-sum.cpp` | 15 | Medium | sort + fix one + two pointers | Apple |
| 02 | `02-valid-palindrome.cpp` | 125 | Easy | two pointers from both ends | Apple |
| 03 | `optional/02-two-pointers/03-trapping-rain-water.cpp` | 42 | Hard | two pointers with running side maxes | Amazon, ByteDance, Snowflake |
| 04 | `04-container-most-water.cpp` | 11 | Medium | two pointers, move shorter side | — |
| 05 | `05-merge-sorted-array.cpp` | 88 | Easy | two pointers filling from the back | Amazon |
| 06 | `06-move-zeroes.cpp` | 283 | Easy | read/write pointers, swap forward | Apple |
| 07 | `07-sort-colors.cpp` | 75 | Medium | Dutch national flag 3-way partition | — |
| 08 | `optional/02-two-pointers/08-rotate-array.cpp` | 189 | Medium | three reversals | Apple |
| 09 | `optional/02-two-pointers/09-compare-versions.cpp` | 165 | Medium | two pointers, parse revisions | Apple |

Apple variants: palindrome score -> 02 valid-palindrome; string compression -> 06 move-zeroes (read/write pointers).

## 03-sliding-window

Grow right, shrink left while invalid, update the answer on a valid window; deque for window max.

| # | File | LC | Difficulty | Pattern | Tags |
|---|---|---|---|---|---|
| 01 | `01-longest-substring-no-repeat.cpp` | 3 | Medium | variable window, jump past last index | Amazon, Apple, ByteDance |
| 02 | `optional/03-sliding-window/02-min-window-substring.cpp` | 76 | Hard | variable window, need counts + missing | ByteDance |
| 03 | `03-longest-repeating-replacement.cpp` | 424 | Medium | variable window, len - maxFreq <= k | ByteDance |
| 04 | `optional/03-sliding-window/04-sliding-window-max.cpp` | 239 | Hard | monotonic decreasing deque | ByteDance |
| 05 | `05-permutation-in-string.cpp` | 567 | Medium | fixed window of letter counts | ByteDance |
| 06 | `06-min-size-subarray-sum.cpp` | 209 | Medium | variable window, shrink while valid | Apple |

Apple variants: subarray product < k (LC 713) -> 06 min-size-subarray-sum; canary rollback -> 14-design hit-counter window.

## 04-binary-search

One template: lower bound (`lo < hi`, `hi = mid` / `lo = mid + 1`); search on the answer when feasibility is monotonic.

| # | File | LC | Difficulty | Pattern | Tags |
|---|---|---|---|---|---|
| 01 | `01-first-last-position.cpp` | 34 | Medium | lower-bound template, twice | Apple, Snowflake |
| 02 | `02-search-rotated-array.cpp` | 33 | Medium | binary search, pick the sorted half | ByteDance |
| 03 | `optional/04-binary-search/03-rotated-array-min.cpp` | 153 | Medium | binary search against right end | Snowflake |
| 04 | `04-koko-bananas.cpp` | 875 | Medium | binary search on the answer | Snowflake |
| 05 | `optional/04-binary-search/05-split-array-largest-sum.cpp` | 410 | Hard | binary search on answer + greedy check | Apple |
| 06 | `optional/04-binary-search/06-median-two-sorted-arrays.cpp` | 4 | Hard | kth smallest, drop k/2 per step | ByteDance |
| 07 | `07-find-peak-element.cpp` | 162 | Medium | binary search on the slope | ByteDance |

Apple variants: last good version / first bad version (LC 278, reported) -> 01 first-last-position lower-bound template on a boolean predicate; test sharding -> 05 split-array.

## 05-stack

Matching with a stack; monotonic stack for next greater / smaller; save state at '(' or '['.

| # | File | LC | Difficulty | Pattern | Tags |
|---|---|---|---|---|---|
| 01 | `01-valid-parentheses.cpp` | 20 | Easy | stack of open brackets | Amazon, ByteDance, Snowflake |
| 02 | `02-basic-calculator-ii.cpp` | 227 | Medium | stack of signed terms | Apple, ByteDance, Snowflake |
| 03 | `03-daily-temperatures.cpp` | 739 | Medium | monotonic decreasing stack | — |
| 04 | `optional/05-stack/04-largest-rectangle-histogram.cpp` | 84 | Hard | monotonic increasing stack, sentinel | ByteDance |
| 05 | `05-decode-string.cpp` | 394 | Medium | two stacks saved at '[' | ByteDance |
| 06 | `optional/05-stack/06-basic-calculator.cpp` | 224 | Hard | stack of (result, sign) per paren | Apple, ByteDance, Snowflake |
| 07 | `optional/05-stack/07-remove-k-digits.cpp` | 402 | Medium | greedy monotonic increasing stack | ByteDance |

Apple variants: parse Lisp expression (LC 736) -> 06 basic-calculator (save state per paren, recurse).

## 06-linked-list

Dummy head, fast/slow pointers, in-place reversal; draw the pointers before rewiring.

| # | File | LC | Difficulty | Pattern | Tags |
|---|---|---|---|---|---|
| 01 | `01-reverse-list.cpp` | 206 | Easy | in-place pointer reversal | Apple |
| 02 | `02-merge-two-lists.cpp` | 21 | Easy | dummy head + tail pointer | — |
| 03 | `03-list-cycle-ii.cpp` | 142 | Medium | Floyd fast/slow pointers | — |
| 04 | `optional/06-linked-list/04-reorder-list.cpp` | 143 | Medium | middle, reverse second half, merge | Apple |
| 05 | `optional/06-linked-list/05-reverse-k-group.cpp` | 25 | Hard | count, reverse group by group | ByteDance |
| 06 | `optional/06-linked-list/06-sort-list.cpp` | 148 | Medium | linked-list merge sort | ByteDance |
| 07 | `07-copy-random-list.cpp` | 138 | Medium | old-to-new node hash map | Snowflake |

## 07-trees

Decide what dfs returns vs. what updates the global answer; level BFS with `sz = q.size()`.

| # | File | LC | Difficulty | Pattern | Tags |
|---|---|---|---|---|---|
| 01 | `01-level-order.cpp` | 102 | Medium | BFS by level size | — |
| 02 | `02-lca.cpp` | 236 | Medium | post-order return of found nodes | Apple |
| 03 | `03-validate-bst.cpp` | 98 | Medium | DFS with (lo, hi) bounds | — |
| 04 | `04-build-tree-pre-in.cpp` | 105 | Medium | recursion with inorder index map | Apple, ByteDance |
| 05 | `optional/07-trees/05-max-path-sum.cpp` | 124 | Hard | post-order gain + global best | ByteDance |
| 06 | `06-kth-smallest-bst.cpp` | 230 | Medium | iterative inorder, stop early | Apple |
| 07 | `optional/07-trees/07-path-sum-iii.cpp` | 437 | Medium | prefix sums on root path, backtracking | — |
| 08 | `optional/07-trees/08-nodes-distance-k.cpp` | 863 | Medium | tree to graph, then BFS | — |
| 09 | `optional/07-trees/09-serialize-tree.cpp` | 297 | Hard | preorder with null markers | — |
| 10 | `optional/07-trees/10-vertical-order-traversal.cpp` | 987 | Hard | collect (col, row, val), sort, group | Amazon |

Apple variants: LCA with parent pointers (LC 1650) -> 02 lca / list intersection; construct from pre + post (LC 889) -> 04 build-tree-pre-in; BST greater sum (LC 1038) -> 06 kth-smallest-bst (reverse inorder).

## 08-graphs

BFS (mark on push) for shortest steps, Kahn for order and cycles, union-find for components.

| # | File | LC | Difficulty | Pattern | Tags |
|---|---|---|---|---|---|
| 01 | `01-number-of-islands.cpp` | 200 | Medium | grid BFS flood fill | Amazon, Apple, ByteDance |
| 02 | `02-course-schedule.cpp` | 207 | Medium | topological sort (Kahn's BFS), LC 210 order | Apple, ByteDance, Snowflake |
| 03 | `03-rotting-oranges.cpp` | 994 | Medium | multi-source BFS by layers | Amazon, ByteDance |
| 04 | `optional/08-graphs/04-word-ladder.cpp` | 127 | Hard | BFS shortest path on implicit graph | Apple, ByteDance |
| 05 | `05-clone-graph.cpp` | 133 | Medium | BFS with old -> new hash map | ByteDance |
| 06 | `optional/08-graphs/06-is-bipartite.cpp` | 785 | Medium | BFS 2-coloring | Apple, Snowflake |
| 07 | `07-accounts-merge.cpp` | 721 | Medium | union-find over shared keys | ByteDance |
| 08 | `optional/08-graphs/08-alien-dictionary.cpp` | 269 | Hard | build edges + Kahn's topological sort | ByteDance |
| 09 | `optional/08-graphs/09-parallel-courses-iii.cpp` | 2050 | Hard | topological order + DAG longest path | Amazon, Apple, Snowflake |

Apple variants: affected targets (reverse graph BFS), parallel build -> 02 course-schedule / 09 parallel-courses-iii; unit conversion (LC 3535) -> 09-advanced-graphs evaluate-division.

## 09-advanced-graphs

Weighted shortest path (Dijkstra / Bellman-Ford), BFS on augmented state, MST.

| # | File | LC | Difficulty | Pattern | Tags |
|---|---|---|---|---|---|
| 01 | `01-network-delay.cpp` | 743 | Medium | Dijkstra with min-heap | — |
| 02 | `optional/09-advanced-graphs/02-cheapest-flights-k-stops.cpp` | 787 | Medium | Bellman-Ford, k+1 rounds with copy | Apple |
| 03 | `optional/09-advanced-graphs/03-grid-obstacles-elimination.cpp` | 1293 | Hard | BFS on (r, c, k) state | — |
| 04 | `optional/09-advanced-graphs/04-swim-rising-water.cpp` | 778 | Hard | Dijkstra on max elevation (bottleneck) | ByteDance |
| 05 | `optional/09-advanced-graphs/05-evaluate-division.cpp` | 399 | Medium | weighted graph DFS, multiply along path | Apple |
| 06 | `optional/09-advanced-graphs/06-min-cost-connect-points.cpp` | 1584 | Medium | MST, Prim O(n^2) | — |

## 10-heap

Size-k heap for top-k (min-heap keeps the largest); two heaps for the median; event simulation.

| # | File | LC | Difficulty | Pattern | Tags |
|---|---|---|---|---|---|
| 01 | `01-kth-largest.cpp` | 215 | Medium | size-k min-heap; randomized 3-way quickselect | ByteDance |
| 02 | `02-top-k-frequent.cpp` | 347 | Medium | count map + size-k min-heap / buckets | Amazon, Apple, ByteDance, Snowflake |
| 03 | `03-merge-k-sorted-lists.cpp` | 23 | Hard | min-heap of current list heads | Amazon, Apple, ByteDance |
| 04 | `04-median-stream.cpp` | 295 | Hard | two heaps: lower max-heap, upper min-heap | Amazon, Apple, Snowflake |
| 05 | `05-k-closest-points.cpp` | 973 | Medium | size-k max-heap on squared distance | — |
| 06 | `optional/10-heap/06-task-scheduler.cpp` | 621 | Medium | max-heap cycles of n+1; counting formula | Apple |
| 07 | `optional/10-heap/07-reorganize-string.cpp` | 767 | Medium | max-heap greedy, place two at a time | Amazon |
| 08 | `optional/10-heap/08-single-threaded-cpu.cpp` | 1834 | Medium | sort by arrival + min-heap simulation | Apple |
| 09 | `optional/10-heap/09-first-fit-scheduler.cpp` | — | Medium | event simulation + global min-heap of end times | Apple |
| 10 | `optional/10-heap/10-min-cost-hire-k-workers.cpp` | 857 | Hard | sort by wage/quality ratio + max-heap | Apple |

Apple variants: meeting rooms III (LC 2402), process tasks using servers (LC 1882) -> 08 single-threaded-cpu / 09 first-fit-scheduler (two heaps, event loop); last stone weight (LC 1046) -> plain max-heap.

## 11-intervals-greedy

Sort by start to merge, by end to keep the most; a min-heap of ends counts overlaps.

| # | File | LC | Difficulty | Pattern | Tags |
|---|---|---|---|---|---|
| 01 | `01-merge-intervals.cpp` | 56 | Medium | sort by start, extend last interval | Amazon, Apple, ByteDance, Snowflake |
| 02 | `02-meeting-rooms-ii.cpp` | 253 | Medium | sort by start + min-heap of ends | Amazon, ByteDance, Snowflake |
| 03 | `03-non-overlapping-intervals.cpp` | 435 | Medium | greedy: sort by end, keep earliest end | ByteDance |
| 04 | `optional/11-intervals-greedy/04-interval-list-intersections.cpp` | 986 | Medium | two pointers, advance earlier-ending interval | ByteDance |
| 05 | `05-jump-game.cpp` | 55 | Medium | greedy farthest reach; LC 45 BFS layers | ByteDance |
| 06 | `optional/11-intervals-greedy/06-partition-labels.cpp` | 763 | Medium | last occurrence + greedy cut | Apple |
| 07 | `optional/11-intervals-greedy/07-gas-station.cpp` | 134 | Medium | one pass, reset start on negative tank | — |
| 08 | `optional/11-intervals-greedy/08-valid-parenthesis-string.cpp` | 678 | Medium | greedy range [lo, hi] of open counts | Apple |

Apple variants: maximum swap (LC 670) -> 06 partition-labels (last occurrence index).

## 12-backtracking

choose -> recurse -> unchoose; `start` index for combinations, `used[]` for permutations.

| # | File | LC | Difficulty | Pattern | Tags |
|---|---|---|---|---|---|
| 01 | `01-subsets.cpp` | 78 | Medium | backtracking, record every node | ByteDance |
| 02 | `02-permutations.cpp` | 46 | Medium | backtracking with used[] array | — |
| 03 | `03-combination-sum.cpp` | 39 | Medium | sorted backtracking, reuse via recurse on i | Apple, ByteDance, Snowflake |
| 04 | `04-word-search.cpp` | 79 | Medium | grid DFS, mark cell in place | — |
| 05 | `optional/12-backtracking/05-restore-ip.cpp` | 93 | Medium | backtracking over 4 parts of 1-3 digits | ByteDance |
| 06 | `optional/12-backtracking/06-palindrome-partitioning.cpp` | 131 | Medium | backtracking on palindromic prefixes | — |
| 07 | `optional/12-backtracking/07-n-queens.cpp` | 51 | Hard | row backtracking with column/diagonal flags | ByteDance |

Apple variants: subsets with sum = target -> 03 combination-sum (each element once, skip duplicates).

## 13-dp

Define the state in one sentence, then the transition and base case; roll arrays when only the last rows matter.

| # | File | LC | Difficulty | Pattern | Tags |
|---|---|---|---|---|---|
| 01 | `01-house-robber.cpp` | 198 | Medium | 1D DP, take or skip | Apple, ByteDance |
| 02 | `02-coin-change.cpp` | 322 | Medium | unbounded knapsack DP | Snowflake |
| 03 | `03-longest-increasing-subsequence.cpp` | 300 | Medium | O(n^2) DP; patience tails binary search | ByteDance |
| 04 | `04-longest-common-subsequence.cpp` | 1143 | Medium | 2D DP over two prefixes | ByteDance |
| 05 | `optional/13-dp/05-edit-distance.cpp` | 72 | Medium | 2D DP, replace/delete/insert | Apple |
| 06 | `06-word-break.cpp` | 139 | Medium | prefix DP over split points | Amazon |
| 07 | `optional/13-dp/07-partition-equal-subset-sum.cpp` | 416 | Medium | 0/1 knapsack subset sum | Apple |
| 08 | `optional/13-dp/08-ones-and-zeroes.cpp` | 474 | Medium | 0/1 knapsack, two capacities | Amazon |
| 09 | `optional/13-dp/09-longest-palindromic-substring.cpp` | 5 | Medium | expand around center | Apple, ByteDance |
| 10 | `optional/13-dp/10-max-product-subarray.cpp` | 152 | Medium | Kadane tracking max and min | — |
| 11 | `11-best-time-stock.cpp` | 121 | Easy | running min price, one pass | Apple |
| 12 | `optional/13-dp/12-stock-cooldown.cpp` | 309 | Medium | three-state machine DP | — |
| 13 | `optional/13-dp/13-regex-matching.cpp` | 10 | Hard | 2D DP, star zero or more | ByteDance |
| 14 | `optional/13-dp/14-burst-balloons.cpp` | 312 | Hard | interval DP on last burst | ByteDance |

Apple variants: subset sum to target, last stone weight II (LC 1049) -> 07 partition-equal-subset-sum (0/1 knapsack); knight probability (LC 688) -> grid DP over steps.

## 14-design

Data-structure design classes (LRU, circular buffer, hit counter, freq stack, ...): pick the combination of structures first (list + map, vector + map, ...), state each operation's cost, then code. Anything that fits another topic lives there instead (median stream -> heap, trie -> trie).

| # | File | LC | Difficulty | Pattern | Tags |
|---|---|---|---|---|---|
| 01 | `01-lru-cache.cpp` | 146 | Medium | doubly linked list + hash map | Amazon, Apple, ByteDance, Snowflake |
| 02 | `02-circular-queue.cpp` | 622 | Medium | ring buffer, head + count | Apple |
| 03 | `03-hit-counter.cpp` | 362 | Medium | sliding window queue; circular buckets | Apple, Snowflake |
| 04 | `optional/14-design/04-freq-stack.cpp` | 895 | Hard | frequency -> stack of values, maxFreq | Amazon |
| 05 | `05-insert-delete-getrandom.cpp` | 380 | Medium | vector + index map, swap-pop | Apple |
| 06 | `06-time-map.cpp` | 981 | Medium | per-key history + binary search | Apple, ByteDance, Snowflake |
| 07 | `optional/14-design/07-first-unique.cpp` | 1429 | Medium | queue + count, lazy cleanup | Amazon |
| 08 | `optional/14-design/08-lfu-cache.cpp` | 460 | Hard | list per frequency + minFreq | Apple, ByteDance |
| 09 | `09-design-hashmap.cpp` | 706 | Easy | separate chaining buckets | ByteDance, Snowflake |
| 10 | `optional/14-design/10-flatten-nested-iterator.cpp` | 341 | Medium | lazy DFS, stack of (list, index) | Apple |
| 11 | `optional/14-design/11-exam-room.cpp` | 855 | Medium | ordered set, scan gaps for max distance | Apple |

Apple variants: snapshot array (LC 1146) -> 06 time-map (per-key history + binary search); memory allocator (LC 2502), task manager (LC 3408) -> ordered set + map as in 11 exam-room.

## 15-trie

26 child pointers + an end flag; DFS on the trie for wildcards and grid search.

| # | File | LC | Difficulty | Pattern | Tags |
|---|---|---|---|---|---|
| 01 | `01-implement-trie.cpp` | 208 | Medium | trie with 26 child pointers | Apple |
| 02 | `optional/15-trie/02-add-search-word.cpp` | 211 | Medium | trie + DFS for wildcard | ByteDance |
| 03 | `optional/15-trie/03-word-search-ii.cpp` | 212 | Hard | trie + grid backtracking | Amazon, Snowflake |

## 16-bit-math-matrix

XOR pairs cancel; boundaries for matrix walks; transpose + reverse for rotation.

| # | File | LC | Difficulty | Pattern | Tags |
|---|---|---|---|---|---|
| 01 | `01-spiral-matrix.cpp` | 54 | Medium | four shrinking bounds | Apple, ByteDance |
| 02 | `02-rotate-image.cpp` | 48 | Medium | transpose then reverse rows | — |
| 03 | `03-single-number.cpp` | 136 | Easy | XOR cancels pairs | — |
| 04 | `04-bit-ops.cpp` | — | Easy | bit masks, x & (x - 1) popcount | Apple |
| 05 | `optional/16-bit-math-matrix/05-counting-bits.cpp` | 338 | Easy | DP on i >> 1 | — |
| 06 | `optional/16-bit-math-matrix/06-total-hamming-distance.cpp` | 477 | Medium | count ones per bit, ones * zeros | Apple |
| 07 | `optional/16-bit-math-matrix/07-string-to-integer-atoi.cpp` | 8 | Medium | char scan with pre-multiply overflow check | Apple |
