# Cisco — prep guide for agents

Read this before helping Siyue prep for Cisco. It is the only md in this company folder;
don't add others (no per-problem notes).

## What we know

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
  interviewer wants plain C, `ref/must/15` is written that way.

## Folders

- `prep/` — his own hand-written solutions. Never create, edit or reformat files here.
- `ref/` — AI reference solutions, `NN-slug.cpp`, split into `must/`, `recommended/` and
  `optional/`. The number is a stable id (never renumber); the tier gives the priority.
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
`label FAIL` (`check(ok, label)` for step-by-step files), 3–4 hand-made tests, a closing
`// follow-up:` block of one-line ideas. Lines stay within 90 columns. `ref/must/15` uses
`malloc` / `free` and no containers in the solutions on purpose; `16` is Python because
its statement requires Python.

Run a file with `g++ -std=c++17 -Wall -Wextra -g -fsanitize=address,undefined <file> -o a
&& ./a`.

## Tiers

The `prep/` column names his file (— = not written yet).

### Must — live-round reports and the top new-grad OA problems

| # | `ref/must/` file | Source | Difficulty | Core DS / algorithm | What it tests | `prep/` |
|---|---|---|---|---|---|---|
| 18 | `18-phone-screen-easy-set.cpp` | phone screen 2026-11 | Easy | two pointers, count map, fast / slow, stack | six warm-ups in one file | — |
| 15 | `15-linked-list-plain-c.cpp` | phone screen 2026-11, many older reports | Easy | pointer rewiring, fast / slow | reverse both ways, delete, delete given the node, cycle; `malloc` / `free` | — |
| 17 | `17-longest-unique-substring.cpp` | embedded team live 2026-07 (LC 3) | Medium | sliding window + last index | return the substring: track `bestStart` | — |
| 19 | `19-k-step-bit-flip.cpp` | phone screen 2026-11 (statement reconstructed) | Medium | simulate + cycle skip | state = (string, label parity), k up to 1e9 | — |
| 13 | `13-lucky-numbers-matrix.cpp` | OA ×4, new grad | Easy | row maxes + column mins | Cisco's direction is row max, column min; -1 if none | — |
| 07 | `07-register-bits.cpp` | embedded live | Easy | bit masks | set / clear / toggle / test, fields, `1u << 32`, `volatile` | — |

Already in the repo (paths relative to the repo root):

| Problem | Source | Where it lives |
|---|---|---|
| House Robber (LC 198, "max non-adjacent chocolates") | OA ×6, new grad | `select/13-dp/01-house-robber.cpp` |
| Maximum Difference, larger after smaller, 0 if none (LC 121) | OA ×3, new grad | `select/13-dp/11-best-time-stock.cpp` |
| Stock Price Fluctuation (LC 2034) | Meraki phone 2026-06 | `company/moveworks/ref/recommended/04-stock-price-fluctuation.cpp` |
| LFU cache (LC 460), LRU cache (LC 146) | ThousandEyes phone 2026-05; graduate 2025 | `select/optional/14-design/08-lfu-cache.cpp`, `select/14-design/01-lru-cache.cpp` |
| Find the Duplicate Number (LC 287), no change to the array, O(1) space | onsite 2026-09 | `topic/arrays-hashing/ref/287_find-duplicate.cpp` |
| Maximum subarray (LC 53) | SDE1 OA, guides | `select/13-dp/16-maximum-subarray.cpp` |

### Recommended — new-grad OA pool and older live reports

| # | `ref/recommended/` file | Source | Difficulty | Core DS / algorithm | What it tests | `prep/` |
|---|---|---|---|---|---|---|
| 20 | `20-find-last-cell.cpp` | OA ×4, new grad / intern (statement reconstructed) | Medium | counter-clockwise spiral | four bounds, last even position | — |
| 21 | `21-digit-sum-count.cpp` | OA ×3, new grad | Medium | brute force, then digit DP | tight flag, memo only non-tight, -1 if none | — |
| 10 | `10-expand-repetitions.cpp` | OA, intern and SWE II | Medium | stack saved at `(` | multi-digit counts, nesting; reports also mention spaces in the input | — |
| 06 | `06-look-and-say.cpp` | OA ×2 (LC 38 step) | Medium | run-length scan | keep it a string, don't convert to int | — |
| 11 | `11-summary-ranges.cpp` | intern OA | Easy | scan runs in input order | "a to b" format, unsorted input, no sort | — |
| 05 | `05-binary-list-to-int.cpp` | OA, 26 new grad (LC 1290) | Easy | `value * 2 + bit` | N up to 64 nodes: `unsigned long long` | — |
| 03 | `03-longest-subarray-k-distinct.cpp` | OA ×2 (prep sites) | Medium | sliding window + count map | erase at zero | — |

Already in the repo:

| Problem | Source | Where it lives |
|---|---|---|
| Add Two Numbers, reversed digits (LC 2) | OA, 26 new grad | `topic/linked-list/ref/2_add-two-numbers.cpp` |
| Rotate a matrix 90° (LC 48) | OA, new grad 2025 | `select/16-bit-math-matrix/02-rotate-image.cpp` |
| Password insert / delete distance (LC 583) | OA, new grad | `select/13-dp/04-longest-common-subsequence.cpp`: answer = `len(a) + len(b) - 2 * LCS` |
| Count distinct pairs with sum K | OA, new grad | `select/01-arrays-hashing/01-two-sum.cpp` (sort + two pointers, skip duplicates) |
| Longest palindromic substring (LC 5) | full-time OA | `select/optional/13-dp/09-longest-palindromic-substring.cpp` |
| Prime or composite, count primes | OA, intern | `select/16-bit-math-matrix/08-count-primes.cpp` |
| Graphs: islands, level order, Dijkstra, course schedule | graduate 2025, guides | `select/08-graphs/01-number-of-islands.cpp`, `select/07-trees/01-level-order.cpp`, `select/09-advanced-graphs/01-network-delay.cpp`, `select/08-graphs/02-course-schedule.cpp` |
| Min stack, top K, ring buffer | guides | `select/05-stack/09-min-stack.cpp`, `select/10-heap/01-kth-largest.cpp`, `select/14-design/02-circular-queue.cpp` |
| `memcpy` / `memmove`, endianness, fixed-block pool, `atoi` | generic embedded list | `company/scientech/ref/recommended/09-memmove.cpp`, `07-endian-fixed-width.cpp`, `19-block-pool.cpp`, `select/optional/16-bit-math-matrix/07-string-to-integer-atoi.cpp` |

### Optional — full-time OA, prep-site guesses, QA role

| # | `ref/optional/` file | Source | Difficulty | Core DS / algorithm | `prep/` |
|---|---|---|---|---|---|
| 14 | `14-power-grid-maintenance.cpp` | LC 3607; same as the OA "Critical Pods" | Medium | union-find + ordered set per component | — |
| 02 | `02-water-jug-min-pours.cpp` | full-time OA (4 jugs) | Medium | BFS over states | — |
| 01 | `01-apple-grouping.cpp` | prep-site OA write-ups | Medium | sort + greedy | — |
| 04 | `04-max-coins-exact-target.cpp` | prep-site OA write-ups | Medium | 0/1 knapsack, max count | — |
| 08 | `08-bst-node-distance.cpp` | prep-site OA write-up | Medium | BST insert + LCA split walk | — |
| 09 | `09-valid-ip-address.cpp` | prep-site OA write-up (LC 468) | Medium | char-loop split | — |
| 12 | `12-implement-router.cpp` | LeetCode tag (LC 3508) | Medium | queue + set + sorted times | — |
| 16 | `16-todo-manager.py` | onsite 2026-10, QA team (Python required) | Easy | dict id → task, status check | — |

01, 02, 04 and 08 are reconstructions from one-line summaries: each file's `// clarify:`
block says which reading it takes.

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
| Matrix zig-zag diagonal (LC 498) | SDE II OA | diagonal d = r + c; walk it up-right on even d, down-left on odd d |
| Meraki full-stack phone: times with no signal | new grad 2024 | merge the on-intervals (`select/11-intervals-greedy/01-merge-intervals.cpp`), report the gaps between merged ranges; the two follow-ups build on it |
| Auction sniping window minimum | full-time OA 2026-07 | two pointers on time; count map x → count inside the window plus an ordered set of x with count ≥ K; answer = smallest in the set or -1 |
| Drone delivery with battery swap stations | full-time OA 2026-07, Hard | BFS on (row, col, battery left, swaps left); a swap station refills to B and uses one swap; like `select/09-advanced-graphs/03-grid-obstacles-elimination.cpp` |
| Chessboard OOD: place / remove / print, opposite colors never share a row, column or diagonal | 1point3acres question bank | `Board` with an 8×8 grid of pieces and per-color counters for each row, column, diagonal (r - c) and anti-diagonal (r + c), so `place` checks in O(1) |
| Collect Coins in a tree (LC 2603, Hard) | full-time OA | trim leaves without coins, then two layers of leaves; answer = 2 × remaining edges |
| Horses and ladders river crossing | switch team HM | ask the exact rules, then BFS over (horses on each side, where the ladders are) states |
| Find Machine Count, Get Minimum Servers, Shift Machines | OA pools, Hard | statements unclear in the reports; skip |

## Doing order

1. `ref/must/18`, then `ref/must/15` typed cold (the 2026-11 phone screen shape: several
   easy problems back to back, so speed matters).
2. `ref/must/17`, then the C pointer questions in the round notes.
3. `ref/must/13`, House Robber, LC 121 (the three most frequent new-grad OA problems).
4. `ref/must/19`, `ref/must/07`.
5. LC 2034, LFU, LRU, LC 287.
6. `ref/recommended/` in table order (20, 21, 10, 06, 11, 05, 03), then its
   "already in the repo" table.
7. The idea table, read once; `ref/optional/` only if time is left.

Moveworks `ref/` runs in parallel (its own Doing order).

Fundamentals asked in the same rounds: the round notes above, then
`~/Code/interview/bagu/foundation/` (`networking.md`, `os_processes_scheduling.md`,
`memory.md`, `concurrency.md`), `bagu/extension/internet_routing_edge.md` and
`bagu/extension/cpp.md`.

Sources: his 1point3acres / fastprep.io summary (above); prep-site guides from the first
pass (prachub.com, dataford.io, techprep.app, ophyai.com, interview.norahq.com,
prepfully.com, jobmentis.com, dev.to, codingkaro.in, interviewfox.ai, linkjob.ai);
Glassdoor Cisco graduate reviews; GeeksforGeeks Cisco intern 2021; LeetCode Cisco tag
lists (interviewsolver.com, algo.monster).
