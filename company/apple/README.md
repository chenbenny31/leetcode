# Apple — prep guide for agents

Read this before helping Siyue prep for Apple. It is the only md in this company folder; don't
add others (no per-problem notes).

## Folders

- `prep/` — his own hand-written solutions. Never create, edit or reformat files here.
- `ref/` — AI reference solutions, one flat folder, `NN-slug.cpp`, numbered in the tier order
  below. A new AI solution goes here with the next free number and gets a row in the matching
  tier table. No subfolders.

## What we know about the interview

- Role: Silicon Validation Software Engineer – GPU IP Validation and Integration (Cupertino,
  role 200660621). Work: writing shaders, firmware/driver code, telling hardware from software bugs.
- First round (2026-10-19) was expected to be algorithms but was OS / architecture fundamentals:
  memory hierarchy, TLB and page tables, cache, why virtual memory. Prep later rounds for OS and
  device topics too (MMIO, interrupts, DMA, memory barriers, cache coherence), next to coding.
- `ref/00-actual-*` are coding questions actually asked.

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

- One runnable file: header comment stating the problem, includes, `using std::x;` for every
  type and function used (no `std::` in the body), solution, `test(label, input..., want)`, `main`.
- Main function = the most straightforward passable solution. Add a second function only for a
  substantially better one, and test both in the same `test`.
- Comment above each function: approach + `T: O(...), S: O(...)` (graphs in V+E). Comment what
  each key data structure stores. Adjacency list is `graph`; BFS/DFS use `cur` / `nxt`; DFS
  helper is `dfs`; matrix size is `m` / `n`; class data members go at the top.
- Tests: few, hand-made, high coverage, never LeetCode's examples. Labels short, words joined
  with `-`. No comments on test cases. Print only `label PASS` / `label FAIL` with if/else
  (no `?:` inside `cout`).
- No mutation inside a condition (`if (--indeg[v] == 0)`, `if (s.erase(x))`); the only
  exception is `insert(...).second`. No `<sstream>`. Keep lines short (move trailing comments
  above the line).
- Minimal: implement only what the statement requires. Ignore hack2hire "insider tips".

## Tiers

Sources: hack2hire Apple questions plus the LeetCode Apple 6-month list, ordered by frequency
and NG relevance. Follow-ups are separate functions without tests. Skipped: LC 2633 (JS-only).
`66-sv-constraints.sv` is for the Design Verification role and was not simulated. The `prep/`
column names his file for that problem (— = not written yet).

### High-frequency — must do (actual interview + hack2hire real questions + top LC)

| # | `ref/` file | Source | Problem | `prep/` |
|---|---|---|---|---|
| 00 | `00-actual-last-good-version.cpp` | actual interview (2026-10) | last good version (binary search) | — |
| 00 | `00-actual-ring-buffer.cpp` | actual interview (2026-10) | fixed-capacity ring buffer | — |
| 01 | `01-palindrome-score.cpp` | Apple (hack2hire) | palindrome score | `palindrome-score.cpp` |
| 02 | `02-bit-ops.cpp` | Apple DV (hack2hire) | bit ops | `count-bits.cpp` |
| 03 | `03-bipartite.cpp` | Apple (hack2hire) / LC 785 | bipartite | `bi-partite.cpp` |
| 04 | `04-subset-sum-dp.cpp` | Apple (hack2hire) | subset sum dp | `subset-sum.cpp` |
| 05 | `05-subsets-with-sum.cpp` | Apple (hack2hire) | subsets with sum | `subsets-with-sum.cpp` |
| 06 | `06-highway-toll.cpp` | Apple (hack2hire) | highway toll | `highway-toll.cpp` |
| 07 | `07-two-sum.cpp` | LC 1 | two sum | `two-sum.cpp` |
| 08 | `08-longest-substring-no-repeat.cpp` | LC 3 | longest substring no repeat | `longest-substring-no-repeat.cpp` |
| 09 | `09-merge-intervals.cpp` | LC 56 | merge intervals | `merge-intervals.cpp` |
| 10 | `10-lru-cache.cpp` | LC 146 | lru cache | `lru-cache.cpp` |
| 11 | `11-number-of-islands.cpp` | LC 200 | number of islands | `number-islands.cpp` |
| 12 | `12-course-schedule.cpp` | LC 207 | course schedule | `course-schedule.cpp` |
| 13 | `13-group-anagrams.cpp` | LC 49 | group anagrams | `group-anagrams.cpp` |
| 14 | `14-top-k-frequent.cpp` | LC 347 | top k frequent | `top-k-frequent.cpp` |
| 15 | `15-valid-palindrome.cpp` | LC 125 | valid palindrome | `valid-palindrome.cpp` |

### Recommended — likely, worth a pass

| # | `ref/` file | Source | Problem | `prep/` |
|---|---|---|---|---|
| 16 | `16-longest-common-prefix.cpp` | LC 14 | longest common prefix | `longest-common-prefix.cpp` |
| 17 | `17-merge-k-sorted-lists.cpp` | LC 23 | merge k sorted lists | `merge-k-sorted-lists.cpp` |
| 18 | `18-valid-sudoku.cpp` | LC 36 | valid sudoku | `valid-sudoku.cpp` |
| 19 | `19-best-time-stock.cpp` | LC 121 | best time stock | `best-time-stock.cpp` |
| 20 | `20-reverse-linked-list.cpp` | LC 206 | reverse linked list | `reverse-linked-list.cpp` |
| 21 | `21-word-ladder.cpp` | LC 127 | word ladder | `word-ladder.cpp` |
| 22 | `22-design-hit-counter.cpp` | LC 362 | design hit counter | `design-hit-counter.cpp` |
| 23 | `23-spiral-matrix.cpp` | LC 54 | spiral matrix | `spiral-matrix.cpp` |
| 24 | `24-first-last-position.cpp` | LC 34 | first last position | `first-last-position.cpp` |
| 25 | `25-time-map.cpp` | LC 981 | time map | `time-map.cpp` |
| 26 | `26-subarray-product-less-k.cpp` | LC 713 | subarray product less k | `subarray-product-less-k.cpp` |
| 27 | `27-insert-delete-getrandom.cpp` | LC 380 | insert delete getrandom | `insert-deletion-getrandom.cpp` |
| 28 | `28-snapshot-array.cpp` | LC 1146 | snapshot array | `snapshot-array.cpp` |
| 29 | `29-basic-calculator-ii.cpp` | LC 227 | basic calculator ii | `basic-calculator-ii.cpp` |
| 30 | `30-three-sum.cpp` | LC 15 | three sum | `three-sum.cpp` |
| 31 | `31-longest-palindromic-substring.cpp` | LC 5 | longest palindromic substring | `longest-palindromic-substring.cpp` |
| 32 | `32-subarray-sum-k.cpp` | LC 560 | subarray sum k | `subarray-sum-eq-k.cpp` |
| 33 | `33-task-scheduler.cpp` | LC 621 | task scheduler | `task-scheduler.cpp` |
| 34 | `34-edit-distance.cpp` | LC 72 | edit distance | `edit-distance.cpp` |
| 35 | `35-median-stream.cpp` | LC 295 | median stream | `median-stream.cpp` |
| 36 | `36-house-robber.cpp` | LC 198 | house robber | `house-robber.cpp` |
| 37 | `37-move-zeroes.cpp` | LC 283 | move zeroes | — |
| 38 | `38-longest-common-subsequence.cpp` | LC 1143 | longest common subsequence | — |
| 39 | `39-jump-game.cpp` | LC 55 (+ LC 45 follow-up) | jump game | — |
| 40 | `40-find-peak-element.cpp` | LC 162 | find peak element | — |
| 41 | `41-longest-consecutive.cpp` | LC 128 | longest consecutive | — |

### Optional — lower frequency, harder, or role-specific

| # | `ref/` file | Source | Problem | `prep/` |
|---|---|---|---|---|
| 42 | `42-rotate-array.cpp` | LC 189 | rotate array | — |
| 43 | `43-reorder-list.cpp` | LC 143 | reorder list | — |
| 44 | `44-string-compression.cpp` | LC 443 | string compression | — |
| 45 | `45-last-stone-weight.cpp` | LC 1046 | last stone weight | — |
| 46 | `46-maximum-swap.cpp` | LC 670 | maximum swap | — |
| 47 | `47-valid-parenthesis-string.cpp` | LC 678 | valid parenthesis string | — |
| 48 | `48-string-to-integer-atoi.cpp` | LC 8 | string to integer atoi | — |
| 49 | `49-lonely-numbers.cpp` | LC 2150 | lonely numbers | — |
| 50 | `50-total-hamming-distance.cpp` | LC 477 | total hamming distance | — |
| 51 | `51-lca-parent-pointers.cpp` | LC 1650 | lca parent pointers | — |
| 52 | `52-bst-greater-sum-tree.cpp` | LC 1038 | bst greater sum tree | — |
| 53 | `53-construct-pre-post.cpp` | LC 889 | construct pre post | — |
| 54 | `54-cheapest-flights-k-stops.cpp` | LC 787 | cheapest flights k stops | — |
| 55 | `55-last-stone-weight-ii.cpp` | LC 1049 | last stone weight ii | — |
| 56 | `56-knight-probability.cpp` | LC 688 | knight probability | — |
| 57 | `57-flatten-nested-iterator.cpp` | LC 341 | flatten nested iterator | — |
| 58 | `58-design-memory-allocator.cpp` | LC 2502 | design memory allocator | — |
| 59 | `59-design-task-manager.cpp` | LC 3408 | design task manager | — |
| 60 | `60-unit-conversion-ii.cpp` | LC 3535 | unit conversion ii | — |
| 61 | `61-exam-room.cpp` | LC 855 | exam room | — |
| 62 | `62-meeting-rooms-iii.cpp` | LC 2402 | meeting rooms iii | — |
| 63 | `63-min-cost-hire-k-workers.cpp` | LC 857 | min cost hire k workers | — |
| 64 | `64-lfu-cache.cpp` | LC 460 | lfu cache | — |
| 65 | `65-parse-lisp-expression.cpp` | LC 736 | parse lisp expression | — |
| 66 | `66-sv-constraints.sv` | Apple DV (hack2hire) | sv constraints | — |
