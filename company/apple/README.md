# Apple — prep guide for agents

Read this before helping Siyue prep for Apple. It is the only md in this company folder; don't
add others (no per-problem notes).

## Folders

- `prep/` — his own hand-written solutions. Never create, edit or reformat files here.
- `ref/` — AI reference solutions, `NN-slug.cpp`, numbered in the tier order below and split
  by tier into `must/`, `recommended/`, `optional/`. A new AI solution goes in its tier's
  folder with the next free number and gets a row in the matching tier table.
- `ref/ase/` — business-flavored problems for the ASE Compute role (see "ASE Compute" below),
  numbered `01-` on their own. The ASE reading order mixes these with the tier files.

## What we know about the interview

- Role: Silicon Validation Software Engineer – GPU IP Validation and Integration (Cupertino,
  role 200660621). Work: writing shaders, firmware/driver code, telling hardware from software bugs.
- First round (2026-10-19) was expected to be algorithms but was OS / architecture fundamentals:
  memory hierarchy, TLB and page tables, cache, why virtual memory. Prep later rounds for OS and
  device topics too (MMIO, interrupts, DMA, memory barriers, cache coherence), next to coding.

## ASE Compute (role 200674593, Software Development Engineer, Developer Experience and Platform Tools)

- Team: Apple Services Engineering (ASE) Compute. Owns the platform qualification and test
  automation service (decides if a hardware / OS release is fit for production) and CI/CD +
  release engineering for internal compute platforms (x86 and Apple silicon servers).
- JD themes to map problems onto: test automation and release signal, build / release
  pipelines with promotion gates, canary + progressive rollout + automated rollback,
  qualification of new server generations, distributed systems and OS fundamentals, K8s.
  Go / Python preferred, but coding is in C++ here.
- What real rounds look like (Apple GPU IP validation: a circular queue + binary search for
  the last good snapshot; ByteDance compute scheduling: first-fit job scheduler): either a
  pure algorithm you can test with an array, or a business story that reduces to one data
  structure (the scheduler = a global min-heap of end times driving an event loop). Say the
  reduction out loud before coding.

ASE reading order (do top to bottom; `prep/` column = his file):

| # | File | JD theme | Data structure / algorithm | `prep/` |
|---|---|---|---|---|
| 1 | `ref/must/00-ring-buffer.cpp` | reported (Apple validation round) | circular array, head + size | — |
| 2 | `ref/must/00-last-good-version.cpp` | reported (Apple); bisect a bad release | binary search on a monotone predicate | — |
| 3 | `ref/ase/01-first-fit-scheduler.cpp` | compute platform scheduling (reported at ByteDance) | event simulation, global min-heap of end times | — |
| 4 | `ref/ase/02-process-tasks-servers.cpp` | assign work to machines (LC 1882) | two heaps: idle by weight, busy by free time | — |
| 5 | `ref/ase/03-parallel-build.cpp` | CI pipeline rounds, critical path (LC 1136 / 2050) | Kahn's topological sort by layers | — |
| 6 | `ref/must/12-course-schedule.cpp` | build dependency cycle check | Kahn / 3-color DFS | `course-schedule.cpp` |
| 7 | `ref/ase/04-affected-targets.cpp` | what to rebuild / retest for a commit | BFS on the reversed dependency graph | — |
| 8 | `ref/ase/07-test-sharding.cpp` | split a test suite across machines (LC 410) | binary search on the answer + greedy check | — |
| 9 | `ref/ase/08-canary-rollback.cpp` | canary error-rate gate, automated rollback | sliding window over time (deque) | — |
| 10 | `ref/ase/05-compare-versions.cpp` | artifact versioning (LC 165) | two pointers, char-loop parsing | — |
| 11 | `ref/ase/06-flaky-tests.cpp` | trustworthy release signal | hash grouping by (test, commit) | — |
| 12 | `ref/must/10-lru-cache.cpp` | build artifact cache | doubly linked list + hash map | `lru-cache.cpp` |
| 13 | `ref/recommended/22-design-hit-counter.cpp` | release metrics / rate limiting | queue of timestamps (follow-up: 300 circular buckets) | `design-hit-counter.cpp` |
| 14 | `ref/recommended/25-time-map.cpp` | versioned config store | per-key sorted list + binary search | `time-map.cpp` |
| 15 | `ref/recommended/28-snapshot-array.cpp` | versioned state per release | per-index history + binary search | `snapshot-array.cpp` |
| 16 | `ref/must/09-merge-intervals.cpp` | maintenance / rollout windows | sort by start, merge | `merge-intervals.cpp` |
| 17 | `ref/recommended/35-median-stream.cpp` | latency percentiles | two heaps | `median-stream.cpp` |
| 18 | `ref/optional/62-meeting-rooms-iii.cpp` | allocate servers / rooms (LC 2402) | two heaps, same shape as #4 | — |
| 19 | `ref/recommended/33-task-scheduler.cpp` | job cooldowns (LC 621) | counting / greedy formula | `task-scheduler.cpp` |
| 20 | `ref/optional/58-design-memory-allocator.cpp` | allocator at the OS boundary (LC 2502) | array scan for a free run | — |
| 21 | `ref/optional/59-design-task-manager.cpp` | priority job queue with updates (LC 3408) | ordered set of (priority, id) + hash map | — |
| 22 | `ref/must/14-top-k-frequent.cpp` | top failing tests | count + heap / bucket sort | `top-k-frequent.cpp` |
| 23 | `ref/recommended/17-merge-k-sorted-lists.cpp` | merge sorted logs from many hosts | k-way merge with a min-heap | `merge-k-sorted-lists.cpp` |

Fundamentals for ASE (next to coding): processes vs threads, scheduling, virtual memory,
file systems and I/O, cgroups / namespaces / containers, K8s Pods / Deployments / rollout,
TCP and HTTP basics, consistency and replication, CI/CD concepts (artifacts, promotion
gates, canary, rollback), x86_64 vs arm64 (endianness is the same; memory ordering differs:
x86 TSO vs weaker ARM ordering, so missing barriers show up as arm-only bugs).

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

- One runnable file: header comment stating the problem, includes, `using std::x;` for every
  type and function used (no `std::` in the body), solution, `test(label, input..., want)`, `main`.
- Main function = the most straightforward passable solution. Add a second function only for a
  substantially better one, and test both in the same `test`.
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
  words on whitespace; use a character loop when the format has other delimiters. Keep lines short (move trailing comments
  above the line).
- Minimal: implement only what the statement requires. Ignore hack2hire "insider tips".

## Tiers

Sources: hack2hire Apple questions plus the LeetCode Apple 6-month list, ordered by frequency
and NG relevance. Follow-ups are separate functions without tests. Skipped: LC 2633 (JS-only).
`66-sv-constraints.sv` is for the Design Verification role and was not simulated. The `prep/`
column names his file for that problem (— = not written yet).

### High-frequency — must do (hack2hire + top LC)

| # | `ref/` file | Source | Problem | `prep/` |
|---|---|---|---|---|
| 00 | `00-last-good-version.cpp` | Apple | last good version (binary search) | — |
| 00 | `00-ring-buffer.cpp` | Apple | fixed-capacity ring buffer | — |
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
