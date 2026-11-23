# Moveworks (ServiceNow) — prep guide for agents

Read this before helping Siyue prep for Moveworks. It is the only md in this company
folder; don't add others (no per-problem notes). ServiceNow (the parent company since
2025) reports live here too.

## What we know

- Full JD and current status: `~/Code/interview/jd/moveworks.md`.
- Role: Software Engineer Graduate (AI/ML) - Moveworks, a ServiceNow company. Mountain
  View, CA (1400 Terra Bella Ave), full time, posted 2026-10-26; graduation August 2027
  or earlier. General engineering track, then matched to one of five teams: GenAI Search
  Platform (retrieval, vector search, LLMs), Agentic System (LLM planning and multi-step
  tools), Agentic Harness & Quality (guardrails, runtime harness), Agentic Evaluation
  (metrics, synthetic data, benchmarks), Core Infrastructure (high-throughput low-latency
  ML pipelines, inference engines, vector databases). Minimum: ML / NLP / DSA basics,
  Python, C++ or Go, project or internship experience with LLMs, RAG or ML infra.
- 2026-11-20: he got an interview, at the same time as Cisco (`company/cisco/`). He
  prepares both sets together, so a session may mix the two folders.
- His own summary of 1point3acres and fastprep.io reports (2026-11-20):
  `~/Code/interview/coding/cisco_servicenow_moveworks_mianjing.md`. JD match, process,
  concept answers, OOD and system design: `~/Code/interview/coding/cisco_moveworks_round_notes.md`.
- Moveworks reports are almost all experienced hires (backend / full stack); one
  new-grad full-stack post (2023): LC 1258 with phrase synonyms as the follow-up, a React
  table (then transpose it), LC 3076, and a product-design round (components for a UI,
  not system design). Experienced-hire coding: Hangman (×6, 2024–2025), the
  lowest-proportion problem in three parts (×4, up to 2026-03), LC 68 (×4), citation
  parsing + merge intervals (×2, 2025-07), LC 3076 with `<u></u>` (phone screens 2026),
  Word Break (onsite 2026-09). System design shows up in almost every onsite.
- ServiceNow new-grad / intern reports: 2026 intern: phone screen LC 1249 in O(n), Java
  basics (abstract class vs interface, GC), onsite LRU, BQ (a project and its biggest
  challenge, a technical disagreement). Older new-grad / intern finals (2021–2022) were
  OOD-heavy and had no LeetCode: an elevator, a deck of cards (draw / print), OOP,
  design patterns. He codes these rounds in Python, so language questions may come in
  Python terms (Java-version trivia is out of scope). IC2 2026-11: SCCs ("trapped zones"). Phone screen 2025-12:
  LC 2697 with the count.
- He codes Moveworks rounds in Python (decided 2026-11-22; the JD lists Python, C++ and
  Go). Cisco and Scientech stay C++. Every reported problem has a Python file in `ref/`;
  the LeetCode tag tables still point at C++ files in `select/` and `topic/`.

## Folders

- `prep/` — his own hand-written solutions. Never create, edit or reformat files here.
- `ref/` — AI reference solutions, `NN-slug.py`, split into `must/`, `recommended/` and
  `optional/`. The number is the priority order (01 first, across the three folders).

## How to run a session

Same as `company/cisco/README.md`: one problem at a time in "Doing order", clarify list in
English first, then the reduction, a short derivation, T/S and the full code; "check" =
run his `prep/` file with `python3` and its tests, report only errors, never edit his file.

## Code conventions

The python-interview-style skill: one runnable Python file, `# clarify:`, `# core idea:`,
`# T: .., S: ..`, `test(label, ..., want)` or `check(label, ok)`, `main()` called at the
bottom, then a closing `# follow-up:` block of numbered one-liners
(`# N. variant -> what changes`), lines within 90 columns.

## Tiers

Every reported problem has its file in `ref/`, in Python, numbered in priority order
(01 first, across all three folders); the tier folder groups them. Moveworks reports
come before ServiceNow ones. The `prep/` column names his file (— = not written yet).

### Must — type it cold once — `ref/must/`

| # | File | Source | Difficulty | Core DS / algorithm | What it tests | `prep/` |
|---|---|---|---|---|---|---|
| 01 | `01-hangman-next-letter.py` | Moveworks ×6 (statement reconstructed from the summaries) | Medium | filter fitting words + letter counts | what "fits" means; tie: more words, then more occurrences, then smaller letter | — |
| 02 | `02-lowest-proportion-strings.py` | Moveworks ×4, three parts | Medium | counting; compute all → min → collect ties | tied-max total, ties kept, "other string" = count 1, `sim` = multiset Jaccard | — |
| 03 | `03-shortest-uncommon-substring.py` | Moveworks new grad + phone screens 2026 (LC 3076) | Medium | try substrings shortest first, `sub in` the others | (1) shortest then smallest; (2) `<u></u>`, case-insensitive, earliest start; count map as the follow-up | — |
| 04 | `04-text-justification.py` | Moveworks ×4 (LC 68) | Hard | greedy packing, then spread the spaces | extra spaces go to the left gaps; last line and one-word lines left-justified | — |
| 05 | `05-citation-intervals.py` | Moveworks ×2, 2025-07 | Medium | `str.find` parse + merge intervals | no regex, invalid citations skipped, touching ranges merge | — |
| 06 | `06-word-break-ii.py` | Moveworks onsite 2026-09; guides (LC 140) | Hard | backtracking, then memo | `dfs(n) = [""]`; the output can be exponential | — |
| 07 | `07-lru-cache.py` | ServiceNow intern 2026 onsite, phone 2024 (LC 146) | Medium | dict + doubly linked list with sentinels | O(1) get / put; `OrderedDict` as the follow-up | — |

### Recommended — read: say the approach and T/S, read the code once — `ref/recommended/`

| # | File | Source | Difficulty | Core DS / algorithm | What it tests | `prep/` |
|---|---|---|---|---|---|---|
| 08 | `08-word-break.py` | Moveworks onsite 2026-09 (LC 139) | Medium | memo on the start idx, or prefix DP | same cut-the-front shape as 06 | — |
| 09 | `09-synonymous-sentences.py` | Moveworks new grad + query rewrite (LC 1258) | Medium | union-find + backtracking | transitive groups, sorted output; phrase synonyms as follow-up | — |
| 10 | `10-min-remove-valid-parens.py` | ServiceNow intern 2026 phone (LC 1249) | Medium | stack of open idxs | O(n); drop an unmatched `)` at once, leftover `(` at the end | — |
| 11 | `11-min-change-palindrome.py` | ServiceNow phone 2025-12 (LC 2697 + count) | Easy | two pointers | min of the pair is also lexicographically smallest | — |
| 12 | `12-distinct-islands.py` | ServiceNow 2025 (asked iterative) + LC tag (LC 694) | Medium | BFS + set of sorted offsets | no recursion, translation-only shape key | — |
| 13 | `13-stock-price-fluctuation.py` | Cisco Meraki phone (LC 2034) | Medium | dict + min / max heaps, lazy pop | a corrected time leaves a stale heap entry | — |
| 14 | `14-k-closest-elements.py` | pasted batch, company unknown (LC 658) | Medium | binary search on the window start | signed compare, duplicates | — |
| 15 | `15-lfu-cache.py` | pasted batch (LC 460) | Hard | freq -> `OrderedDict` of keys + `min_freq` | a tie evicts the least recent; `min_freq` = 1 after an insert | — |
| 16 | `16-longest-unique-substring.py` | pasted batch (LC 3, return the substring) | Medium | sliding window + last idx per char | a stale idx left of the window is ignored | — |
| 17 | `17-search-rotated-array.py` | ServiceNow phone 2024 (LC 33) | Medium | binary search, one half is sorted | `<=` when lo == mid | — |
| 18 | `18-merge-k-sorted-lists.py` | ServiceNow phone 2024 (LC 23) | Hard | heap of (val, list idx), or divide and conquer | the idx breaks ties (nodes can't compare) | — |
| 19 | `19-subarray-sum-k.py` | ServiceNow, experienced hires (LC 560) | Medium | prefix sum + cnt dict | negatives rule out a window; add after the lookup | — |
| 20 | `20-group-anagrams.py` | ServiceNow, experienced hires (LC 49) | Medium | dict keyed by the sorted letters | empty strs group together | — |
| 21 | `21-meeting-rooms-ii.py` | ServiceNow, experienced hires (LC 253) | Medium | heap of end times, or a sweep | a room freed at t takes a start at t | — |
| 22 | `22-set-matrix-zeroes.py` | ServiceNow, experienced hires (LC 73) | Medium | row / column flags, then row 0 / column 0 as the flags | O(1) space; save row 0 / column 0 first | — |
| 23 | `23-max-product-subarray.py` | ServiceNow NLP intern 2024 (LC 152) | Medium | max and min product ending here | a negative swaps them | — |
| 24 | `24-unique-paths-ii.py` | ServiceNow phone 2024 (LC 63) | Medium | rolling 1D dp | an obstacle zeroes its cell | — |
| 25 | `25-next-right-pointers.py` | ServiceNow phone 2024 (LC 116) | Medium | walk a wired level, wire the one below | O(1) space; the link across two parents | — |

### Optional — `ref/optional/`

| # | File | Source | Difficulty | Core DS / algorithm | `prep/` |
|---|---|---|---|---|---|
| 26 | `26-balanced-strings-count.py` | Moveworks OA 2024 (intern + full time) | Medium | DP over the last letter, mod | — |
| 27 | `27-min-replacements-to-sort.py` | Moveworks OA 2024 (LC 2366) | Hard | greedy right to left | — |

26's summary gives a second example (n = 3, d = 3 → 224) that does not fit its own rule;
the file says so in `# clarify:`.

## Reported, idea only

| Problem | Source | Idea |
|---|---|---|
| Guess the secret word with match feedback (LC 843) | prep sites | guess the word whose largest group of same-match-count candidates is smallest; keep the candidates whose match with the guess equals the feedback |
| Replace Words (LC 648) | Moveworks | trie of roots (`select/15-trie/01-implement-trie.cpp`); for each word walk the trie and stop at the first root end |
| Trapped zones: SCCs, then SCCs with an edge coming in and none going out | ServiceNow IC2 2026-11, Hard | Kosaraju (DFS finish order, DFS on the reversed graph) or Tarjan; condense, count in / out edges per component |
| Count valid words: length > 3, a vowel and a consonant, letters and digits only | ServiceNow intern panel 2023 | split on spaces with a char loop, check each token; O(1) space version walks two pointers over the original string |
| Binary tree boundary clockwise (LC 545); generate parentheses, iterative | ServiceNow 2025 | 545: left boundary, leaves, right boundary reversed; parens: a stack of (string, open, close) |
| Inorder traversal is a palindrome | ServiceNow 2024 | two iterators: a forward inorder stack and a reverse inorder stack, compare until they cross |
| Regions cut by slashes (LC 959) | ServiceNow 2024 | split each cell into 4 triangles, union-find, count components |
| Max subarray sum with one deletion (LC 1186) | ServiceNow IC3 2026-11 | two DPs: best ending at i with no deletion, with one deletion |
| BFS shortest path through an interface (`isDestination()`, `getAvailableNextSteps()`) | ServiceNow 2021 | plain BFS with a visited set; the graph is only reachable through the calls |
| Shop1 / shop2: choose K indexes, maximize min of the two sums | ServiceNow OA 2024 | statement too short to pin down; skip |

## LeetCode company tags (more than six months)

Pasted 2026-11-20, in LeetCode's order. One new file came out of them
(`ref/recommended/12`); 2291, 1963 and 2116 became follow-up lines in `select/`. `ref/`
paths are inside this folder, the rest from the repo root.

### Moveworks

| LC | Problem | Where it lives |
|---|---|---|
| 1258 | Synonymous Sentences | `ref/recommended/09-synonymous-sentences.py` |
| 140 | Word Break II | `ref/must/06-word-break-ii.py` |
| 3076 | Shortest Uncommon Substring in an Array | `ref/must/03-shortest-uncommon-substring.py` |
| 56 | Merge Intervals | `select/11-intervals-greedy/01-merge-intervals.cpp` (citation variant: `ref/must/05`) |
| 2366 | Minimum Replacements to Sort the Array | `ref/optional/27-min-replacements-to-sort.py` |

### ServiceNow

| LC | Problem | Where it lives |
|---|---|---|
| 1 | Two Sum | `select/01-arrays-hashing/01-two-sum.cpp` |
| 3 | Longest Substring Without Repeating Characters | `ref/recommended/16-longest-unique-substring.py` |
| 5 | Longest Palindromic Substring | `select/optional/13-dp/09-longest-palindromic-substring.cpp` |
| 11 | Container With Most Water | `select/02-two-pointers/04-container-most-water.cpp` |
| 20 | Valid Parentheses | `select/05-stack/01-valid-parentheses.cpp` |
| 22 | Generate Parentheses | `topic/stack/22_generate-parentheses.cpp` (his own drill; iterative version: idea table) |
| 42 | Trapping Rain Water | `select/optional/02-two-pointers/03-trapping-rain-water.cpp` |
| 53 | Maximum Subarray | `select/13-dp/16-maximum-subarray.cpp` |
| 56 | Merge Intervals | `select/11-intervals-greedy/01-merge-intervals.cpp` |
| 146 | LRU Cache | `ref/must/07-lru-cache.py` |
| 179 | Largest Number | `topic/hard/ref/179_largest-number.cpp` |
| 200 | Number of Islands | `select/08-graphs/01-number-of-islands.cpp` |
| 206 | Reverse Linked List | `select/06-linked-list/01-reverse-list.cpp` |
| 1944 | Number of Visible People in a Queue | `topic/monotonic-sweep/ref/1944_people-seen-queue.cpp` |
| 417 | Pacific Atlantic Water Flow | `topic/graphs/ref/417_pacific-atlantic.cpp` |
| 735 | Asteroid Collision | `topic/stack/ref/735_asteroid-collision.cpp` |
| 443 | String Compression | `topic/parser/ref/443_string-compression.cpp` |
| 41 | First Missing Positive | `select/optional/01-arrays-hashing/07-first-missing-positive.cpp` |
| 199 | Binary Tree Right Side View | `topic/trees/ref/199_right-side-view.cpp` |
| 239 | Sliding Window Maximum | `select/03-sliding-window/04-sliding-window-max.cpp` |
| 694 | Number of Distinct Islands | `ref/recommended/12-distinct-islands.py` |
| 31 | Next Permutation | `topic/two-pointers/ref/31_next-permutation.cpp` |
| 322 | Coin Change | `select/13-dp/02-coin-change.cpp` |
| 2291 | Maximum Profit From Trading Stocks | follow-up line in `select/13-dp/07-partition-equal-subset-sum.cpp` |
| 1963 | Minimum Number of Swaps to Make the String Balanced | follow-up line in `select/optional/11-intervals-greedy/08-valid-parenthesis-string.cpp` |
| 2116 | Check if a Parentheses String Can Be Valid | follow-up line in `select/optional/11-intervals-greedy/08-valid-parenthesis-string.cpp` |
| 347 | Top K Frequent Elements | `select/10-heap/02-top-k-frequent.cpp` |
| 424 | Longest Repeating Character Replacement | `select/03-sliding-window/03-longest-repeating-replacement.cpp` |
| 213 | House Robber II | `topic/dp/ref/213_house-robber-ii.cpp` |
| 45 | Jump Game II | `topic/greedy-intervals/ref/45_jump-game-ii.cpp` |
| 162 | Find Peak Element | `select/04-binary-search/07-find-peak-element.cpp` |
| 992 | Subarrays with K Different Integers | `topic/hard/ref/992_subarrays-k-different-integers.cpp` |
| 151 | Reverse Words in a String | `topic/parser/ref/151_reverse-words.cpp` |
| 540 | Single Element in a Sorted Array | `topic/binary-search/ref/540_single-element-sorted.cpp` |
| 994 | Rotting Oranges | `select/08-graphs/03-rotting-oranges.cpp` |
| 21 | Merge Two Sorted Lists | `select/06-linked-list/02-merge-two-lists.cpp` |
| 122 | Best Time to Buy and Sell Stock II | `topic/dp/ref/122_stock-ii.cpp` |
| 560 | Subarray Sum Equals K | `ref/recommended/19-subarray-sum-k.py` |
| 17 | Letter Combinations of a Phone Number | `topic/backtracking/ref/17_phone-letters.cpp` |
| 1143 | Longest Common Subsequence | `select/13-dp/04-longest-common-subsequence.cpp` |
| 503 | Next Greater Element II | `topic/stack/ref/503_next-greater-ii.cpp` |
| 127 | Word Ladder | `select/08-graphs/04-word-ladder.cpp` |
| 71 | Simplify Path | `topic/conditional/ref/71_simplify-path.cpp` |
| 904 | Fruit Into Baskets | `topic/sliding-window/ref/904_fruit-into-baskets.cpp` |
| 39 | Combination Sum | `select/12-backtracking/03-combination-sum.cpp` |
| 1192 | Critical Connections in a Network | `topic/optional/ref/1192_critical-connections.cpp` |
| 75 | Sort Colors | `select/02-two-pointers/07-sort-colors.cpp` |
| 238 | Product of Array Except Self | `select/01-arrays-hashing/05-product-except-self.cpp` |

## Doing order

Just follow the numbers: `ref/must/` typed cold, then `ref/recommended/` read in order.

1. `must/01` Hangman, `02` (all three parts), `03` (both parts).
2. `must/04` LC 68 (Moveworks ×4).
3. `must/05` citation + merge, `must/06` Word Break II (read `recommended/08` Word Break
   with it), `must/07` LRU.
4. `recommended/09`–`16`.
5. OOD (elevator, deck of cards) and system design in the round notes.
6. `recommended/17`–`25` (ServiceNow experienced-hire reports, read once), the idea table,
   then the LeetCode tag tables (already-solved problems only need a re-read); `optional/`
   only if time is left.

Sources: his 1point3acres / fastprep.io summary (above); smartrecruiters / zapply.jobs
posting (2026-10-26); prachub.com Moveworks pages; interviewquery.com and dataford.io
Moveworks guides; interviewsolver.com LeetCode tag; Glassdoor Moveworks SWE review
(Apr 2026).
