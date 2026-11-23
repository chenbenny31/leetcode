# Cisco — prep guide for agents

Read this before helping Siyue prep for Cisco. It is the only md in this company folder;
don't add others (no per-problem notes).

## What we know

- Full JD and current status: `~/Code/interview/jd/cisco.md`.
- Role: Software Engineer Embedded Systems II (Full Time) - United States, req 2028988,
  posted 2026-11-14. New-grad level (MS with 0 years). Embedded Linux / RTOS, C or C++,
  device drivers and kernel components, Python test automation; preferred: networking
  (TCP/IP, BGP, OSPF), bootloaders, hardware bring-up. The posting does not name a team.
- 2026-11-20: he was invited to interview. There is no OA: the process goes straight to
  live interviews, and the algorithm coding round comes first. Date and number of rounds
  are not known yet. He also has a Moveworks (ServiceNow) interview
  (`company/moveworks/`) and prepares both sets together.
- His own summary of 1point3acres and fastprep.io reports (collected 2026-11-20, ranked by
  how close to new grad): `~/Code/interview/coding/cisco_servicenow_moveworks_mianjing.md`.
  Concept answers, process notes and BQ: `~/Code/interview/coding/cisco_moveworks_round_notes.md`.
- Live rounds, the closest reports to his:
  - Embedded / low-level team, 2026-07: LC 3 returning the substring itself, then C
    pointer questions (array of int pointers vs pointer to an array of ints,
    `*(ptr + 3)`).
  - Phone screen, 2026-11 (full time): a few BQs (how you use AI day to day, how you get
    unstuck, a recent project), then about 4 easy problems: palindrome, values that
    appear once in order, first letter seen twice, middle of a list, reverse a list,
    reverse a string, valid parentheses. Another 2026-11 phone screen: K-step bit flip.
  - Meraki / ThousandEyes phone screens: LC 2034 Stock Price Fluctuation, LFU cache
    (clarify first). Onsite 2026-09: LC 287 without changing the array, O(1) space.
  - Switch team HM: a horses-and-ladders river-crossing puzzle; HR said to prepare
    switching basics.
  - Older and prep-site reports: Webex 45–60 min, project talk then 1–2 easy / medium,
    runnable code; reverse a list in C both ways, bit tricks, LRU, graphs.
- New-grad OA reports (most 1point3acres posts; an OA pool shared by SWE I / II and new
  grads): 90 min, 3 problems, hidden tests, strict output format, Python / Java / C#.
  He has no OA, but the same pool shows what Cisco considers new-grad level. Most
  frequent: Row Max / Column Min (×4), House Robber (×6), Maximum Difference = LC 121
  (×3), Find Last Cell (×4), digit-sum count (×3), `(ab){3}` expansion, look-and-say,
  compress ranges (input may be unsorted: don't sort).
- He codes in C++. Ask at the start of the round whether C++ with the STL is fine; if the
  interviewer wants plain C, `ref/must/02` is written that way.

## Folders

- `prep/` — his own hand-written solutions. Never create, edit or reformat files here.
- `ref/` — AI reference solutions, `NN-slug.cpp`, split into `must/`, `recommended/` and
  `optional/`. The number is the priority order (01 first, across the three folders;
  renumbered 2026-11-22). `must/` = type it cold once; the rest is read. Reported problems
  already solved elsewhere in the repo are copied in, so everything Cisco is here.
  01–14 were written when the round was assumed to be an OA, 15–16 from prep sites,
  17–21 from his 1point3acres summary.

## How to run a session

1. One problem at a time, in the order of "Doing order" below. Start with what to clarify,
   written in English (input ranges, ties, edge cases, what to return), each with the
   answer or the assumption to state.
2. Say the reduction out loud first ("this is fast / slow pointers"), then a very concise
   derivation, T/S, and the full code. It is a live round: talk while typing and dry-run
   one small example before saying "done".
3. "check" = compile his `prep/` file with `g++ -std=c++17 -Wall -Wextra
   -fsanitize=address,undefined`, run its tests, report only errors; never edit his file.

## Code conventions

Same as `select/README.md`: one runnable C++17 file, `using std::x;` (no `std::` in the
body), the textbook solution you can type live, `// clarify:` one point per line,
`// core idea:` and `// T: .., S: ..` above each solution, comments only on key data
structures and easy-to-get-wrong lines, `test(label, ..., want)` printing `label PASS` /
`label FAIL` (`check(ok, label)` for step-by-step files), 2–4 hand-made tests, a closing
`// follow-up:` block of numbered one-liners (`// N. variant -> what changes`). Lines
stay within 90 columns. `ref/must/02` uses `malloc` / `free` and no containers in the
solutions on purpose; `optional/46` is Python because its statement requires Python.

Run a file with `g++ -std=c++17 -Wall -Wextra -g -fsanitize=address,undefined <file> -o a
&& ./a`.

## Tiers

The `prep/` column names his file (— = not written yet).

### Must — type it cold once

| # | File | Source | Difficulty | Core DS / algorithm | What it tests | `prep/` |
|---|---|---|---|---|---|---|
| 01 | `01-phone-screen-easy-set.cpp` | phone screen 2026-11 | Easy | two pointers, count map, fast / slow, stack | six warm-ups in one file | — |
| 02 | `02-linked-list-plain-c.cpp` | phone screen 2026-11, many older reports | Easy | pointer rewiring, fast / slow | reverse both ways, delete, delete given the node, cycle; `malloc` / `free` | — |
| 03 | `03-longest-unique-substring.cpp` | embedded team live 2026-07 (LC 3) | Medium | sliding window + last index | return the substring: track `bestStart` | — |
| 04 | `04-register-bits.cpp` | embedded live | Easy | bit masks | set / clear / toggle / test, fields, `1u << 32`, `volatile` | — |
| 05 | `05-k-step-bit-flip.cpp` | phone screen 2026-11 (statement reconstructed) | Medium | simulate + cycle skip | state = (string, label parity), k up to 1e9 | — |
| 06 | `06-find-duplicate.cpp` | onsite 2026-09 (LC 287) | Medium | Floyd cycle on idx -> value | no change to the array, O(1) space | — |
| 07 | `07-lru-cache.cpp` | ThousandEyes phone 2026-05; graduate 2025 (LC 146) | Medium | hash map + doubly linked list | O(1) get / put, sentinels | — |

### Recommended — read: say the approach and T/S, read the code once

| # | File | Source | Difficulty | Core DS / algorithm | What it tests | `prep/` |
|---|---|---|---|---|---|---|
| 08 | `08-lucky-numbers-matrix.cpp` | OA ×4, new grad | Easy | row maxes + column mins | Cisco's direction is row max, column min; -1 if none | — |
| 09 | `09-house-robber.cpp` | OA ×6, new grad ("max non-adjacent chocolates", LC 198) | Medium | 1D DP, two rolling vars | take or skip | — |
| 10 | `10-best-time-stock.cpp` | OA ×3, new grad (LC 121: max difference, larger after smaller) | Easy | running min | 0 if none | — |
| 11 | `11-stock-price-fluctuation.cpp` | Meraki phone 2026-06 (LC 2034) | Medium | hash map + multiset | correction erases one copy, not all | — |
| 12 | `12-lfu-cache.cpp` | ThousandEyes phone 2026-05 (LC 460) | Hard | per-freq lists + min freq | a tie evicts the least recent | — |
| 13 | `13-maximum-subarray.cpp` | SDE1 OA, guides (LC 53) | Medium | Kadane | all negative | — |
| 14 | `14-find-last-cell.cpp` | OA ×4, new grad / intern (statement reconstructed) | Medium | counter-clockwise spiral | four bounds, last even position | — |
| 15 | `15-digit-sum-count.cpp` | OA ×3, new grad | Medium | brute force, then digit DP | tight flag, memo only non-tight, -1 if none | — |
| 16 | `16-expand-repetitions.cpp` | OA, intern and SWE II | Medium | stack saved at `(` | multi-digit counts, nesting; reports also mention spaces in the input | — |
| 17 | `17-look-and-say.cpp` | OA ×2 (LC 38 step) | Medium | run-length scan | keep it a string, don't convert to int | — |
| 18 | `18-summary-ranges.cpp` | intern OA | Easy | scan runs in input order | "a to b" format, unsorted input, no sort | — |
| 19 | `19-binary-list-to-int.cpp` | OA, 26 new grad (LC 1290) | Easy | `value * 2 + bit` | N up to 64 nodes: `unsigned long long` | — |
| 20 | `20-longest-subarray-k-distinct.cpp` | OA ×2 (prep sites) | Medium | sliding window + count map | erase at zero | — |
| 21 | `21-diagonal-traverse.cpp` | OA "zig-zag" + LC tag (LC 498) | Medium | diagonals by r + c | row bounds per diagonal, parity sets the direction | — |
| 22 | `22-add-two-numbers.cpp` | OA, 26 new grad (LC 2) | Medium | carry through two lists | final carry | — |
| 23 | `23-rotate-image.cpp` | OA, new grad 2025 (LC 48) | Medium | transpose + reverse rows | in place | — |
| 24 | `24-memmove.cpp` | embedded basics list | Easy | byte copy, direction by overlap | copy backward when dst is after src | — |
| 25 | `25-endian-fixed-width.cpp` | embedded basics list | Easy | shifts per byte | big / little endian, host order check | — |
| 26 | `26-string-to-integer-atoi.cpp` | embedded basics list (LC 8) | Medium | char loop | sign, overflow clamp | — |
| 27 | `27-longest-common-subsequence.cpp` | OA, new grad: password insert / delete distance (LC 583) | Medium | LCS DP | answer = len(a) + len(b) - 2 * LCS | — |
| 28 | `28-two-sum.cpp` | OA, new grad: count distinct pairs with sum K | Easy | sort + two pointers | skip duplicates | — |
| 29 | `29-longest-palindromic-substring.cpp` | full-time OA (LC 5) | Medium | expand around the center | odd and even centers | — |
| 30 | `30-count-primes.cpp` | intern OA (prime or composite, count primes) | Medium | sieve | n < 2 | — |

### Optional

| # | File | Source | Difficulty | Core DS / algorithm | `prep/` |
|---|---|---|---|---|---|
| 31 | `31-block-pool.cpp` | embedded basics list | Medium | free list inside the free blocks | — |
| 32 | `32-number-of-islands.cpp` | graduate 2025, guides (LC 200) | Medium | flood fill | — |
| 33 | `33-level-order.cpp` | graduate 2025, guides (LC 102) | Medium | BFS by level | — |
| 34 | `34-network-delay.cpp` | guides: Dijkstra (LC 743) | Medium | Dijkstra with a min-heap | — |
| 35 | `35-course-schedule.cpp` | guides (LC 207) | Medium | Kahn topological sort | — |
| 36 | `36-min-stack.cpp` | guides (LC 155) | Medium | stack of (val, min) | — |
| 37 | `37-kth-largest.cpp` | guides: top K (LC 215) | Medium | min-heap of size k | — |
| 38 | `38-circular-queue.cpp` | guides: ring buffer (LC 622) | Medium | array + head + count | — |
| 39 | `39-power-grid-maintenance.cpp` | LC 3607; same as the OA "Critical Pods" | Medium | union-find + ordered set per component | — |
| 40 | `40-water-jug-min-pours.cpp` | full-time OA (4 jugs) | Medium | BFS over states | — |
| 41 | `41-apple-grouping.cpp` | prep-site OA write-ups | Medium | sort + greedy | — |
| 42 | `42-max-coins-exact-target.cpp` | prep-site OA write-ups | Medium | 0/1 knapsack, max count | — |
| 43 | `43-bst-node-distance.cpp` | prep-site OA write-up | Medium | BST insert + LCA split walk | — |
| 44 | `44-valid-ip-address.cpp` | prep-site OA write-up (LC 468) | Medium | char-loop split | — |
| 45 | `45-implement-router.cpp` | LeetCode tag (LC 3508) | Medium | queue + set + sorted times | — |
| 46 | `46-todo-manager.py` | onsite 2026-10, QA team (Python required) | Easy | dict id → task, status check | — |
| 47 | `47-predict-the-winner.cpp` | LC tag (LC 486) | Medium | interval DP on score difference | — |
| 48 | `48-apples-in-tree.cpp` | LC tag (LC 1443) | Medium | BFS order, then leaves-first | — |
| 49 | `49-autocomplete-system.cpp` | LC tag (LC 642) | Hard | trie, per-node sentence set | — |

40, 41, 42 and 43 are reconstructions from one-line summaries: each file's `// clarify:`
block says which reading it takes. Files whose header says "copy of" mirror a `select/`,
`topic/` or `company/scientech/` file; when the source changes, copy it again.

## Reported, idea only

| Problem | Source | Idea |
|---|---|---|
| FizzBuzz, N < 2·10^6 | OA, new grad / intern | test 15 first, then 3, then 5; print with `"\n"`, not `endl` |
| Alternating string merge (LC 1768) | OA, SWE II new grad | two indexes; take `a[i]` then `b[i]` while either has chars left |
| Min swaps so every even comes before every odd | OA, SWE II new grad | E = num of evens; answer = num of odds among the first E positions |
| Words in rows / columns of a grid, forward or backward | OA, 25 new grad | build each row and column string; a word is found if it is a substring of the line or of its reverse; case-sensitive |
| Pilot drop points: best horizontal or vertical line | OA, 25 new grad | count map of x and of y; the max count, only if it is at least 2 |
| Non-twin person: smallest value that appears once | OA, new grad 2025 | count map, min over values with count 1, -1 if none |
| Valid latitude / longitude | OA, SDE II new grad | char-loop parse of a signed decimal (one optional sign, digits, at most one `.`), then range check; no regex |
| Create new version: `cisco2India` → `ciscociscoIndia` | OA, SDE1 new grad | letter → append; digit d ≥ 2 → result becomes result repeated d times; 0 and 1 do nothing |
| Mean and mode; first out-of-order character | intern pool | floor of `sum / n` in `long long`, mode by count map (ask the tie rule); first i with `s[i] < s[i - 1]`, -1 if none |
| Odd birthday count | SWE II OA | count map date → count, count dates with an odd count |
| Meraki full-stack phone: times with no signal | new grad 2024 | merge the on-intervals (`select/11-intervals-greedy/01-merge-intervals.cpp`), report the gaps between merged ranges; the two follow-ups build on it |
| Auction sniping window minimum | full-time OA 2026-07 | two pointers on time; count map x → count inside the window plus an ordered set of x with count ≥ K; answer = smallest in the set or -1 |
| Drone delivery with battery swap stations | full-time OA 2026-07, Hard | BFS on (row, col, battery left, swaps left); a swap station refills to B and uses one swap; like `select/09-advanced-graphs/03-grid-obstacles-elimination.cpp` |
| Chessboard OOD: place / remove / print, opposite colors never share a row, column or diagonal | 1point3acres question bank | `Board` with an 8×8 grid of pieces and per-color counters for each row, column, diagonal (r - c) and anti-diagonal (r + c), so `place` checks in O(1) |
| Collect Coins in a tree (LC 2603, Hard) | full-time OA | trim leaves without coins, then two layers of leaves; answer = 2 × remaining edges |
| Horses and ladders river crossing | switch team HM | ask the exact rules, then BFS over (horses on each side, where the ladders are) states |
| Find Machine Count, Get Minimum Servers, Shift Machines | OA pools, Hard | statements unclear in the reports; skip |

## LeetCode company tag (Cisco, more than six months)

Pasted 2026-11-20, in LeetCode's order. New files from it: `ref/recommended/21` and
`ref/optional/47`, `48`, `49`; easy ones became follow-up lines in `select/`. `ref/`
paths are inside this folder, the rest from the repo root.

| LC | Problem | Where it lives |
|---|---|---|
| 1 | Two Sum | `ref/recommended/28-two-sum.cpp` |
| 5 | Longest Palindromic Substring | `ref/recommended/29-longest-palindromic-substring.cpp` |
| 20 | Valid Parentheses | `select/05-stack/01-valid-parentheses.cpp`, `ref/must/01` |
| 48 | Rotate Image | `ref/recommended/23-rotate-image.cpp` |
| 53 | Maximum Subarray | `ref/recommended/13-maximum-subarray.cpp` |
| 54 | Spiral Matrix | `select/16-bit-math-matrix/01-spiral-matrix.cpp` (counter-clockwise OA variant: `ref/recommended/14`) |
| 56 | Merge Intervals | `select/11-intervals-greedy/01-merge-intervals.cpp` |
| 2016 | Maximum Difference Between Increasing Elements | follow-up line in `ref/recommended/10-best-time-stock.cpp` |
| 121 | Best Time to Buy and Sell Stock | `ref/recommended/10-best-time-stock.cpp` |
| 141 | Linked List Cycle | `ref/must/02`, `topic/linked-list/ref/141_list-cycle.cpp` |
| 146 | LRU Cache | `ref/must/07-lru-cache.cpp` |
| 149 | Max Points on a Line | `select/optional/16-bit-math-matrix/12-max-points-on-a-line.cpp` |
| 198 | House Robber | `ref/recommended/09-house-robber.cpp` |
| 200 | Number of Islands | `ref/optional/32-number-of-islands.cpp` |
| 202 | Happy Number | follow-up line in `select/06-linked-list/03-list-cycle-ii.cpp` |
| 212 | Word Search II | `select/optional/15-trie/03-word-search-ii.cpp` |
| 283 | Move Zeroes | `select/02-two-pointers/06-move-zeroes.cpp` |
| 412 | Fizz Buzz | idea table below |
| 1380 | Lucky Numbers in a Matrix | `ref/recommended/08` (Cisco's reverse direction; LC 1380 is its follow-up line) |
| 88 | Merge Sorted Array | `select/02-two-pointers/05-merge-sorted-array.cpp` |
| 206 | Reverse Linked List | `ref/must/02`, `select/06-linked-list/01-reverse-list.cpp` |
| 2 | Add Two Numbers | `ref/recommended/22-add-two-numbers.cpp` |
| 49 | Group Anagrams | `select/01-arrays-hashing/03-group-anagrams.cpp` |
| 17 | Letter Combinations of a Phone Number | `topic/backtracking/ref/17_phone-letters.cpp` |
| 151 | Reverse Words in a String | `topic/parser/ref/151_reverse-words.cpp` |
| 125 | Valid Palindrome | `select/02-two-pointers/02-valid-palindrome.cpp` |
| 26 | Remove Duplicates from Sorted Array | follow-up line in `select/02-two-pointers/06-move-zeroes.cpp` |
| 3 | Longest Substring Without Repeating Characters | `ref/must/03`, `select/03-sliding-window/01-longest-substring-no-repeat.cpp` |
| 72 | Edit Distance | `select/optional/13-dp/05-edit-distance.cpp` |
| 567 | Permutation in String | `select/03-sliding-window/05-permutation-in-string.cpp` |
| 287 | Find the Duplicate Number | `ref/must/06-find-duplicate.cpp` |
| 253 | Meeting Rooms II | `select/11-intervals-greedy/02-meeting-rooms-ii.cpp` |
| 486 | Predict the Winner | `ref/optional/47-predict-the-winner.cpp` |
| 3508 | Implement Router | `ref/optional/45-implement-router.cpp` |
| 380 | Insert Delete GetRandom O(1) | `select/14-design/05-insert-delete-getrandom.cpp` |
| 84 | Largest Rectangle in Histogram | `select/optional/05-stack/04-largest-rectangle-histogram.cpp` |
| 3607 | Power Grid Maintenance | `ref/optional/39-power-grid-maintenance.cpp` |
| 1047 | Remove All Adjacent Duplicates In String | follow-up line in `select/05-stack/01-valid-parentheses.cpp` |
| 25 | Reverse Nodes in k-Group | `select/optional/06-linked-list/05-reverse-k-group.cpp` |
| 1443 | Minimum Time to Collect All Apples in a Tree | `ref/optional/48-apples-in-tree.cpp` |
| 2603 | Collect Coins in a Tree | idea table below; follow-up line in `ref/optional/48` |
| 33 | Search in Rotated Sorted Array | `select/04-binary-search/02-search-rotated-array.cpp` |
| 75 | Sort Colors | `select/02-two-pointers/07-sort-colors.cpp` |
| 394 | Decode String | `select/05-stack/05-decode-string.cpp` (count-after variant: `ref/recommended/16`) |
| 50 | Pow(x, n) | `select/16-bit-math-matrix/10-pow-x-n.cpp` |
| 498 | Diagonal Traverse | `ref/recommended/21-diagonal-traverse.cpp` |
| 79 | Word Search | `select/12-backtracking/04-word-search.cpp` |
| 273 | Integer to English Words | `topic/parser/ref/273_integer-to-english.cpp` |
| 642 | Design Search Autocomplete System | `ref/optional/49-autocomplete-system.cpp` |
| 236 | Lowest Common Ancestor of a Binary Tree | `select/07-trees/02-lca.cpp` |
| 1143 | Longest Common Subsequence | `ref/recommended/27-longest-common-subsequence.cpp` |
| 76 | Minimum Window Substring | `select/optional/03-sliding-window/02-min-window-substring.cpp` |
| 312 | Burst Balloons | `select/optional/13-dp/14-burst-balloons.cpp` |
| 242 | Valid Anagram | follow-up line in `select/01-arrays-hashing/03-group-anagrams.cpp` |
| 15 | 3Sum | `select/02-two-pointers/01-three-sum.cpp` |
| 207 | Course Schedule | `ref/optional/35-course-schedule.cpp` |
| 239 | Sliding Window Maximum | `select/03-sliding-window/04-sliding-window-max.cpp` |

## Doing order

Just follow the numbers: `ref/must/` typed cold, then `ref/recommended/` read in order.

1. `must/01`, then `must/02` typed cold (the 2026-11 phone screen shape: several easy
   problems back to back, so speed matters).
2. `must/03`, then the C pointer questions in the round notes.
3. `must/04`, `must/05`, then `must/06`, `must/07`.
4. `recommended/08`–`13` (the most frequent new-grad OA problems and the phone-screen
   designs), then `14`–`30` in order.
5. The idea table, read once; then the LeetCode tag table (already-solved problems
   only need a re-read); `optional/` only if time is left.

Moveworks (Python, 2026-11-30) has its own Doing order.

Fundamentals asked in the same rounds: the round notes above, then
`~/Code/interview/bagu/foundation/` (`networking.md`, `os_processes_scheduling.md`,
`memory.md`, `concurrency.md`), `bagu/extension/internet_routing_edge.md` and
`bagu/extension/cpp.md`.

Sources: his 1point3acres / fastprep.io summary (above); prep-site guides from the first
pass (prachub.com, dataford.io, techprep.app, ophyai.com, interview.norahq.com,
prepfully.com, jobmentis.com, dev.to, codingkaro.in, interviewfox.ai, linkjob.ai);
Glassdoor Cisco graduate reviews; GeeksforGeeks Cisco intern 2021; LeetCode Cisco tag
lists (interviewsolver.com, algo.monster).
