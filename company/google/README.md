# Google — prep guide for agents

Read this before helping Siyue prep for Google. It is the only md in this company folder;
don't add others (no per-problem notes).

## Folders

- `prep/` — his own hand-written solutions (followups as `*-followup.cpp`). Never create, edit
  or reformat files here.
- `ref/` — AI reference solutions, `NN-slug.cpp`, numbered in the tier order below and split
  by tier into `must/`, `recommended/`, `optional/`. A new AI solution goes in its tier's
  folder with the next free number and gets a row in the matching tier table.

## What we know about the interview

- NG SWE. Problems so far come from interview reports he collected; tiers are a prep
  judgment, not a measured frequency. Treat every row as an ordinary bank problem.
- Expect follow-ups that ask for a better complexity than the first working solution
  (e.g. O(n^2) → O(n), brute-force count → backtracking).

## How to run a session

1. Pick the next problem in tier order. Skip rows that already have a `prep/` file unless he
   asks to redo one.
2. Start every problem with what to clarify with the interviewer, written in English (input order and ranges,
   duplicates / overlaps, edge cases, what to return), each with the answer the statement
   gives or the assumption to state.
3. One problem at a time: a very concise derivation (which idea, why it is efficient) → T/S
   for the crucial functions → the full code. Prefer the version that is easiest
   to follow line by line (e.g. prefix sums + binary search over a two-pointer with special
   cases) when both meet the constraints.
4. "check" = review his file in `prep/`: compile with `g++ -std=c++17 -Wall -Wextra`, run its
   own tests, and report only errors (say whether it is a compile failure or a test failure).
   No praise, no summaries. Never edit his file — show the fix in chat and he types it.
5. Don't run tests for solutions you only show in chat; he tests them himself.

## Code conventions (`ref/` files and any code shown in chat)

- One runnable file: header comment stating the problem, includes, one blank line,
  `using std::x;` for every type and function used (no `std::` in the body), solution,
  `test(label, input..., want)`, `main`. C++17: use `auto` / structured bindings where the
  type is obvious (copy, never `auto&`, before a `pop()`).
- Main function = the most straightforward passable solution. Add a second function only for a
  substantially better one, and test both in the same `test`. Follow-ups are separate
  functions.
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
  words on whitespace; use a character loop when the format has other delimiters. Lines ≤ 90 columns (move trailing
  comments above the line).
- Minimal: implement only what the statement requires.

## Tiers

The `prep/` column names his file(s) for that problem (— = not written yet). Run a ref file
with `g++ -std=c++17 -Wall -Wextra -g -fsanitize=address,undefined ref/NN-slug.cpp -o a && ./a`.

### High-frequency — must do

| # | `ref/` file | Source | Problem | `prep/` |
|---|---|---|---|---|
| 01 | `01-max-ends-score` | interview report | max of first + last − middle over subarrays (Kadane variant) | `01-max-end-score.cpp` |
| 02 | `02-perfect-wave-number` | interview report | zigzag digits check + count in [1, N] by backtracking | — |
| 03 | `03-roster-merge-shifts` | interview report | merge shift roster into [start, end, names] rows incl. gaps (cut points; sweep) | — |
| 04 | `04-ad-service` | interview report | AdService: highest score, no repeat in a row, score −1 (scan; max-heap) | — |
| 05 | `05-string-substitution` | interview report | replace %KEY% from a dict; follow-up nested values + cycle check (DFS) | — |
| 06 | `06-magical-string-pairs` | interview report | count (i, j, k) with equal letter sets on both sides (O(n²) masks; 26-step chains) | — |
| 07 | `07-piano-hand-moves` | interview report | group notes with max − min ≤ 4, count hand moves + fingers (greedy) | — |
| 08 | `08-tower-profit` | interview report | tower height vs houses covered, max profit (sort distances; integer-height follow-up) | — |
