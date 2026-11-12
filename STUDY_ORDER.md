# Study order

Google new grad is the next target, so this file ranks the basic algorithms in `select/` by how
often Google interview reports hit their pattern, then points at the Google and Cloudflare company
sets. Paths are relative to the repo root.

How to work each item: timer on (25 min for a medium), write it cold in one file with the `test`
function, run it, then diff against the repo file and note what you missed. Reading mode: read
the header (Clarify, Idea, Pitfalls, Tests to write), name the approach and T/S out loud, then
read the body and the `// follow-up:` block.

Reference standard: every file in the plan has, under the problem statement, **Clarify** (questions
for the interviewer, each with the stated answer or the assumption), **Idea**, **Pitfalls** and
**Tests to write**; its follow-ups are code (a tested function, or a short snippet in the
`// follow-up:` block).

## Now — plan and progress (agents: read first, update the log)

- **Next interview:** Google SWE new grad, date not set. Cloudflare Argo HM call was 2026-11-11.
  Amazon (11-09) and PlusAI (11-10) are done; their folders were removed.
- **Plan** (by topic, in the order of "What Google asks most"; inside a topic: `select/` core
  with its follow-up functions, then Google must, then recommended by sighting count):
  1. Graphs: `select/08-graphs/` 01 islands, 03 rotting oranges, 02 course schedule, 07
     accounts merge, `select/09-advanced-graphs/01-network-delay` (Dijkstra, needed for 13 /
     45); must 05; recommended 11, 12, 40, 43, 37, 35, 39, 13, 45, 26.
  2. Intervals and heap scheduling (incl. the scheduling-style designs): `select/11-intervals-
     greedy/` 02 meeting rooms II, 01 merge intervals, `select/10-heap/04-median-stream`; must
     03, 04; recommended 10, 30, 32, 36, 20, 17, 46, 21, 19.
  3. Hash counting and prefix sums: `select/01-arrays-hashing/02-subarray-sum-k`,
     `select/10-heap/02-top-k-frequent`, `select/01-arrays-hashing/03-group-anagrams`; must 01
     (done), 06; recommended 22, 23, 44, 18, 27.
  4. Trees: `select/07-trees/01-level-order`; recommended 16, 31, 25, 38, 42, 15.
  5. Sliding window, binary search, stack: `select/` 03-sliding-window/01, 04-binary-search/01
     and 04, 05-stack/02 and 03; must 07, 08; recommended 33, 09, 34, 41, 14, 29, 24.
  6. DP and backtracking: must 02; recommended 28.
  7. The other Part A ★★ rows, the `select/optional/` files the LC tag ranks high, then ★ and
     the rest; skim the Optional table in `company/google/README.md` (idea only, no files).
- **Order for 2026-11-11, 7 pm – 12 am** (type = write cold in `company/google/prep/`, run,
  diff; meeting rooms II / III come before the rest of step 1 because of their counts):
  1. 7:15 type `numIslands`, 7:35 `numIslandsAfterAdds` (`select/08-graphs/01`); 8:00
     `orangesRotting` (`03`); 8:20 `findOrder` (`02`); 8:40 `networkDelayTime`
     (`select/09-advanced-graphs/01`).
  2. 9:05 read (+ break): islands follow-ups (`maxAreaOfIsland`, `countEnclosedRegions`,
     `countBattleships`, diagonal), `canFinishDfs`, `smallestOrder`, `minSemesters`,
     `masterSequence`, `select/08-graphs/07-accounts-merge`.
  3. 9:45 type `minMeetingRooms`, 10:00 `assignRooms` (`select/11-intervals-greedy/02`);
     10:25 `mostBooked` (recommended 10).
  4. 10:55 read recommended 11, 40, 43.
  5. 11:15 if time, type recommended 12 `shortestPathOneBreak`, then must 05 `expand`.
- **Lessons from the last interviews:** basics decide it (binary-search bounds and the
  `lo == 0` / `lo == n` checks, smart pointers / `make_shared`); write the straightforward
  correct solution first, state its complexity, dry-run one small case before saying "done".
- **Log** (newest last; one line per session: date, rows covered, problems to revisit):
  - 2026-11-11: plan rebuilt for Google; three batches of Google reports mapped onto `select/`
    (weights below), follow-ups added to the mother files, recommended 09–29 written.
  - 2026-11-11: FastPrep list (359 problems with sighting counts) mapped: weights re-ranked,
    more follow-up functions in `select/`, recommended 30–43 written, Optional table extended.
  - 2026-11-11: LeetCode Google tag (30-day frequency, top 100) folded into the weights; tag
    problems added as follow-up functions in `select/`, recommended 44–46, Optional rows.
  - 2026-11-11: plan reordered by topic (graphs, then interval / heap scheduling, then hash +
    prefix sums, trees, windows / search / stack, DP); must 02 moved to the DP step.

## What Google asks most (analysis, 2026-11-11)

Sources: the FastPrep list (359 problems, 554 sightings; topics tagged by keywords in each
summary, one problem can count for several topics, so the shares add up to more than 100%), the
earlier report batches, and the LeetCode Google tag.

| Rank | Topic | Share of FastPrep sightings | Typical reports |
|---|---|---|---|
| 1 | Graph traversal: BFS + union-find + topological sort | ~18% | broken-node shortest path, multi-source BFS, islands series, union-find, Kahn |
| 2 | Intervals / sweep line + heap scheduling | intervals 14%, heap 8% | car rental ×14, min chairs ×9, meeting rooms III, CPU scheduling, combined schedule |
| 3 | Hash counting + prefix sums | hash 16%, prefix 5% | equal-endpoint max subarray ×6, target remainder, top-k, group shifted |
| 4 | Trees (DFS / recursion) | ~11% | file system size ×6, min cut cost, boolean leaf flips, merge trees, validate tree |
| 5 | Design / simulation | ~7% | restaurant waitlist ×8, lockers, logger, AdService |
| 6 | Sliding window, binary search, stack parsing | ~5% each | at least k distinct ×9, calculator, nested function eval, weighted pick |
| 7 | DP, backtracking | DP 9%, backtracking 2% | mostly basic 1D / 2D DP (Kadane, LIS, knapsack); hard DP is rare |

What follows from it:

1. Graphs and interval scheduling carry about half of the frequent reports. The single most
   frequent pattern is meeting rooms II (car rental ×14, chairs ×9).
2. Most problems wrap a basic algorithm in a real-life story (piano hand moves, waitlist, crop
   garden, water drops). The hard part is translating the story, not the algorithm; frequent
   Hard problems are rare.
3. Follow-ups are almost always asked and point the same ways: brute force → optimal
   (O(n^2) → O(n)), streaming input or dynamic updates, return the construction (path,
   assignment, indices), one more constraint (broken nodes, wrap past midnight, weights). The
   follow-up functions in the mother files are worth more than new problems.
4. DP and backtracking weigh less than the LeetCode community suggests.
5. The LC tag leans to array / hash / two-pointer easy and medium problems (partly general
   popularity): basics are checked, but the reports are more specific.

Priority: graphs first, then interval / heap scheduling, then hash + prefix sums (the Plan in
"Now" follows this order). Know every follow-up function in the ★★★ files `number-of-islands`,
`rotting-oranges`, `meeting-rooms-ii`, `subarray-sum-k` and `top-k-frequent` by heart.

## Part A — `select/` core by Google weight

Weight = Google interview reports that reduce to the file's pattern or its follow-ups (2025–2026
onsite / phone lists plus the FastPrep list, `×n` = sightings), plus the LeetCode Google tag
ranked by 30-day frequency (2026-11-11 snapshot): a problem in the tag's top 30 counts as 3, one in
31–100 as 1. ★★★ = 6+, ★★ = 3–5, ★ = 1–2, — = none (kept for pattern coverage). "LC tag" lists
`problem number #rank`; SQL / JavaScript problems in the tag are ignored. The reports column
names the variants, which are follow-up functions in that file or a `company/google/ref/` file
(`G09` = recommended 09).

### ★★★

| File | Pattern | LC tag | Google reports |
|---|---|---|---|
| `select/11-intervals-greedy/02-meeting-rooms-ii.cpp` | sort + min-heap of ends; sweep | — | car rental assignment ×14 (`assignRooms`), min chairs ×9, LC 253 ×3, combined schedule ×4 and roster merge (must 03), min CPUs ×2, cyclic rooms ×2 (`minRoomsCyclic`), peak concurrency ×2, meeting rooms III ×4 (G10) |
| `select/01-arrays-hashing/02-subarray-sum-k.cpp` | prefix sum + hash map | 560 #28 | equal endpoints ×6 (G22, LC 3026), split after one removal ×3 (`canSplitAfterRemoval`), target remainder ×3 (`shortestWithRemainder`), max ends score ×3 (must 01), parity queries ×3 (G23) |
| `select/08-graphs/01-number-of-islands.cpp` | grid BFS flood fill | 200 #15, 419 #70 | LC 200 ×3, land added one by one ×3 (`numIslandsAfterAdds`), enclosed regions ×3 (`countEnclosedRegions`), max area ×2 (`maxAreaOfIsland`), battleships (`countBattleships`), diagonal ×2, crop garden ×3 (G35) |
| `select/03-sliding-window/01-longest-substring-no-repeat.cpp` | variable window + counts | 3 #11, 904 #33 | shortest with at least k distinct ×9 (`shortestAtLeastKDistinct`), at most 2 distinct (LC 904) |
| `select/08-graphs/03-rotting-oranges.cpp` | multi-source BFS by layers | 2812 #64 (G45) | broken-node path ×4 and server BFS (G11), hop path ×3, land path ×3, one wall break (G12), return a book ×2, safest path (G45) |
| `select/10-heap/02-top-k-frequent.cpp` | count map + size-k heap / sort | 347 #18 | top-k IPs ×3, LC 347 ×4, largest files ×2, talkative users ×2, stream (`TopKStream`), first / last k words ×2, next word ×3 (G18) |
| `select/05-stack/02-basic-calculator-ii.cpp` | stack of signed terms | 772 #71, 3749 #73 (G14) | parentheses ×3 (`calculateWithParens`), nested calls ×2 (G14), variable expression ×2, RPN to infix ×2 (G29) |
| `select/04-binary-search/01-first-last-position.cpp` | lower / upper bound | 34 #67, 69 #41, 278 #44, 528 #48 (G44) | `mySqrt`, prefix count ×2, prefix words ×2, point-in-range ×2 and Range Module (G21), weighted pick ×3 (G44) |
| `select/04-binary-search/04-koko-bananas.cpp` | binary search on the answer | 410 #19, 875 #79 | bisecting square area ×3 (G34), minimax path |
| `select/05-stack/03-daily-temperatures.cpp` | monotonic stack | 1944 #30, 739 #86 | visible people to the left ×3 (`countVisibleLeft`) |
| `select/01-arrays-hashing/03-group-anagrams.cpp` | hash map with a canonical key | 49 #39, 242 #26 | group shifted ×3 (`groupShifted`), string bag, anagram subsequences |
| `select/08-graphs/02-course-schedule.cpp` | Kahn topological sort | 207 #75 | LC 207 ×3, smallest order ×3 (`smallestOrder`), merge subsequences ×2, master sequence ×2 |
| `select/08-graphs/07-accounts-merge.cpp` | union-find | 547 #52 | `findCircleNum`, `earliestAllFriends`, secret (G26), point clusters ×2, dynamic friendship ×2, org hierarchy |
| `select/07-trees/01-level-order.cpp` | BFS by level | — | LC 102 ×2, right side view ×2, alternating-color roots ×4 and tree roots (G25), same color per depth |

### ★★

| File | Pattern | LC tag | Google reports |
|---|---|---|---|
| `select/01-arrays-hashing/01-two-sum.cpp` | hash map of earlier values | 1 #1, 217 #59, 219 #60 | LC 1 ×2 |
| `select/02-two-pointers/01-three-sum.cpp` | sort + two pointers | 15 #9 | sum below target ×2 (`countTripletsBelow`) |
| `select/13-dp/11-best-time-stock.cpp` | running min / two-state DP | 121 #14, 53 #40, 1911 #87 | Kadane (`maxSubArray`), alternating sum (`maxAlternatingSum`), max ends score (must 01) |
| `select/01-arrays-hashing/04-longest-consecutive.cpp` | hash set, start of run | 128 #12 | — |
| `select/02-two-pointers/04-container-most-water.cpp` | two pointers, move the shorter | 11 #13 | — |
| `select/06-linked-list/03-list-cycle-ii.cpp` | Floyd cycle entrance | 287 #16 | find the duplicate (`findDuplicate`) |
| `select/06-linked-list/02-merge-two-lists.cpp` | dummy head | 2 #2, 21 #35 | add two numbers (`addTwoNumbers`) |
| `select/05-stack/01-valid-parentheses.cpp` | stack | 20 #25 | parentheses with deletions ×2 (Optional) |
| `select/10-heap/01-kth-largest.cpp` | heap / quickselect | 215 #77, 462 #5, 912 #63, 1046 #92 | median moves (`minMovesToEqual`), merge sort (`sortArray`), insert + k-th largest |
| `select/16-bit-math-matrix/04-bit-ops.cpp` | bits, fast power | 50 #36, 1922 #82, 3644 #95 | `myPow`, `countGoodNumbers` |
| `select/05-stack/05-decode-string.cpp` | two stacks / recursion | — | LC 394 ×4, `{k}` decompression |
| `select/12-backtracking/03-combination-sum.cpp` | backtracking on sums | — | 1s and 2s summing to N ×4 (`oneTwoStrings`) |
| `select/12-backtracking/01-subsets.cpp` | backtracking / bitmask | 78 #97, 22 #90 | `generateParenthesis`, LC 90, subsequences are words |
| `select/13-dp/03-longest-increasing-subsequence.cpp` | tails + lower_bound | 354 #98, 1048 #100 | envelopes ×3 (`maxEnvelopes`), `longestStrChain` |
| `select/09-advanced-graphs/01-network-delay.cpp` | Dijkstra | 1514 #57, 3970 #31 | `maxProbability`, run-limited labels (`shortestPathRunLimit`), flights ×2 and trains (G13) |
| `select/11-intervals-greedy/01-merge-intervals.cpp` | sort + merge | 56 #32, 759 #62 | `employeeFreeTime`, LC 56 ×2, roster merge (must 03) |
| `select/15-trie/01-implement-trie.cpp` | trie | — | prefix words ×2 (`countWithPrefix`), tokenizer, stream detector |
| `select/09-advanced-graphs/04-swim-rising-water.cpp` | Dijkstra on max cell (bottleneck) | — | LC 778, minimax path (`minimaxBinarySearch`), safest path (G45) |
| `select/14-design/01-lru-cache.cpp` | hash map + linked list | — | LC 146 ×3, `setCapacity` |
| `select/10-heap/04-median-stream.cpp` | two heaps | — | LC 295, sliding window median ×2, trimmed averages ×5 (G36) |
| `select/16-bit-math-matrix/02-rotate-image.cpp` | transpose + reverse | — | `rotateRect`, `isSymmetric` |

### ★

| File | Pattern | LC tag | Google reports |
|---|---|---|---|
| `select/16-bit-math-matrix/03-single-number.cpp` | XOR / voting | 136 #85, 169 #42 | majority element (`majorityElement`) |
| `select/16-bit-math-matrix/01-spiral-matrix.cpp` | boundary walk | 54 #83 | — |
| `select/03-sliding-window/06-min-size-subarray-sum.cpp` | shrink while valid | 209 #76 | — |
| `select/13-dp/04-longest-common-subsequence.cpp` | 2D DP | 1143 #93 | common parent sequence (`canShareParent`) |
| `select/07-trees/02-lca.cpp` | post-order / parent pointers | — | cheapest common ancestor, blood relation |
| `select/07-trees/06-kth-smallest-bst.cpp` | order statistics | — | insert + k-th largest |
| `select/10-heap/05-k-closest-points.cpp` | size-k max-heap | — | k closest stars ×2 |
| `select/13-dp/01-house-robber.cpp` | 1D DP | 70 #49, 509 #56 | `robWithHouses` |
| `select/13-dp/06-word-break.cpp` | DP over prefixes | — | LC 139 ×2 |
| `select/14-design/03-hit-counter.cpp` | queue / ring buffer | — | five-minute hit counter ×2 |
| `select/14-design/06-time-map.cpp` | per-key history + floor search | — | interval membership (G21) |
| `select/05-stack/07-remove-k-digits.cpp` | monotonic stack, greedy | — | `largestAfterRemoving` |
| `select/11-intervals-greedy/04-interval-list-intersections.cpp` | two pointers on intervals | — | LC 986 |

### `select/optional/` files the LC tag ranks high

Open these after the ★★ rows: `04-binary-search/06-median-two-sorted-arrays` (LC 4, #3),
`13-dp/09-longest-palindromic-substring` (LC 5, #7), `04-binary-search/05-split-array-largest-sum`
(LC 410, #19), `16-bit-math-matrix/07-string-to-integer-atoi` (LC 8, #34),
`13-dp/07-partition-equal-subset-sum` (LC 416, #45), `10-heap/06-task-scheduler` (LC 621, #50),
`13-dp/05-edit-distance` (LC 72, #58), `01-arrays-hashing/07-first-missing-positive` (LC 41,
#68), `05-stack/04-largest-rectangle-histogram` (LC 84, #84).

### No Google signal yet (pattern coverage)

`select/01-arrays-hashing/05-product-except-self`; `select/02-two-pointers/02-valid-palindrome`,
`05-merge-sorted-array`, `06-move-zeroes`, `07-sort-colors`;
`select/03-sliding-window/03-longest-repeating-replacement`, `05-permutation-in-string`;
`select/04-binary-search/02-search-rotated-array`, `07-find-peak-element`;
`select/06-linked-list/01-reverse-list`, `07-copy-random-list`; `select/07-trees/03-validate-bst`,
`04-build-tree-pre-in`; `select/08-graphs/05-clone-graph`; `select/10-heap/03-merge-k-sorted-lists`;
`select/11-intervals-greedy/03-non-overlapping-intervals`, `05-jump-game`;
`select/12-backtracking/02-permutations`, `04-word-search`; `select/13-dp/02-coin-change`;
`select/14-design/02-circular-queue`, `05-insert-delete-getrandom`, `09-design-hashmap`.

## Part B — company sets

- Google: `company/google/README.md` (must 01–08, recommended 09–46, Optional table).
- Cloudflare: `company/cloudflare/README.md` (ref 01–12).

## Part C — reference only

`select/optional/<topic>/` (harder or niche versions of each pattern) and `topic/` (the older
300-problem bank with the user's early drills). Open them only for a specific reported problem.
