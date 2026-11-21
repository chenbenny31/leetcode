# Moveworks (ServiceNow) — prep guide for agents

Read this before helping Siyue prep for Moveworks. It is the only md in this company
folder; don't add others (no per-problem notes). ServiceNow (the parent company since
2025) reports live here too.

## What we know

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
  design patterns. He interviews in C++, so language questions are prepped in C++
  terms (Java-version trivia is out of scope). IC2 2026-11: SCCs ("trapped zones"). Phone screen 2025-12:
  LC 2697 with the count.
- He codes in C++ (the JD lists Python, C++ and Go).

## Folders

- `prep/` — his own hand-written solutions. Never create, edit or reformat files here.
- `ref/` — AI reference solutions, `NN-slug.cpp`, split into `must/`, `recommended/` and
  `optional/`. The number is a stable id (never renumber); the tier gives the priority.

## How to run a session

Same as `company/cisco/README.md`: one problem at a time in "Doing order", clarify list in
English first, then the reduction, a short derivation, T/S and the full code; "check" =
compile his `prep/` file with `g++ -std=c++17 -Wall -Wextra -fsanitize=address,undefined`,
run its tests, report only errors, never edit his file.

## Code conventions

Same as `select/README.md` (one runnable C++17 file, `using std::x;`, `// clarify:`,
`// core idea:`, `// T: .., S: ..`, `test(label, ..., want)` or `check(ok, label)`, a
closing `// follow-up:` block, lines within 90 columns).

## Tiers

The `prep/` column names his file (— = not written yet).

### Must

| # | `ref/must/` file | Source | Difficulty | Core DS / algorithm | What it tests | `prep/` |
|---|---|---|---|---|---|---|
| 03 | `03-hangman-next-letter.cpp` | Moveworks ×6 (statement reconstructed from the summaries) | Medium | filter fitting words + letter counts | what "fits" means; tie: more words, then more occurrences, then smaller letter | — |
| 02 | `02-lowest-proportion-strings.cpp` | Moveworks ×4, three parts | Medium | counting + exact fraction compare | tied-max total, cross multiplication, ties kept, `sim` = multiset Jaccard | — |
| 01 | `01-shortest-uncommon-substring.cpp` | Moveworks new grad + phone screens 2026 (LC 3076) | Medium | substring → owner-count map | (1) shortest then smallest; (2) `<u></u>`, case-insensitive, earliest start | — |
| 06 | `06-min-change-palindrome.cpp` | ServiceNow phone 2025-12 (LC 2697 + count) | Easy | two pointers | min of the pair is also lexicographically smallest | — |

Already in the repo (paths relative to the repo root):

| Problem | Source | Where it lives |
|---|---|---|
| Minimum Remove to Make Valid Parentheses (LC 1249) | ServiceNow intern 2026 phone | `topic/conditional/ref/1249_min-remove-valid-parens.cpp` |
| LRU cache (LC 146) | ServiceNow intern 2026 onsite | `select/14-design/01-lru-cache.cpp` |
| Text Justification (LC 68) | Moveworks ×4 | `company/scientech/ref/must/01-text-justification.cpp` |

### Recommended

| # | `ref/recommended/` file | Source | Difficulty | Core DS / algorithm | What it tests | `prep/` |
|---|---|---|---|---|---|---|
| 07 | `07-citation-intervals.cpp` | Moveworks ×2, 2025-07 | Medium | char-loop parse + merge intervals | no regex, invalid citations skipped, touching ranges merge | — |
| 08 | `08-synonymous-sentences.cpp` | Moveworks new grad + query rewrite (LC 1258) | Medium | union-find + backtracking | transitive groups, sorted output; phrase synonyms as follow-up | — |
| 04 | `04-stock-price-fluctuation.cpp` | Cisco Meraki phone (LC 2034) | Medium | hash map + multiset | correction erases one copy, not all | — |
| 05 | `05-k-closest-elements.cpp` | pasted batch, company unknown (LC 658) | Medium | binary search on the window start | signed compare, duplicates | — |

Already in the repo:

| Problem | Source | Where it lives |
|---|---|---|
| Word Break (LC 139), Word Break II (LC 140) | Moveworks onsite 2026-09; guides | `select/13-dp/06-word-break.cpp`, `company/google/ref/optional/132-word-break-ii.cpp` |
| LFU cache (LC 460) | pasted batch | `select/optional/14-design/08-lfu-cache.cpp` |
| Longest substring without repeat, return it | pasted batch | `company/cisco/ref/must/17-longest-unique-substring.cpp` |
| ServiceNow phone screens 2024: LC 33, LC 116, LC 63, LC 23, LC 146 | experienced hires | `select/04-binary-search/02-search-rotated-array.cpp`, `topic/trees/ref/116_next-right-pointers.cpp`, `topic/dp/ref/63_unique-paths-ii.cpp`, `select/10-heap/03-merge-k-sorted-lists.cpp`, `select/14-design/01-lru-cache.cpp` |
| ServiceNow: set matrix zeroes in O(1) space, subarray sum K (LC 560), group anagrams + meeting rooms | experienced hires | `topic/bit-math-matrix/ref/73_set-matrix-zeroes.cpp`, `select/01-arrays-hashing/02-subarray-sum-k.cpp`, `select/01-arrays-hashing/03-group-anagrams.cpp`, `select/11-intervals-greedy/02-meeting-rooms-ii.cpp` |
| ServiceNow NLP intern: an LC 152-style O(n) array problem | intern 2024 | `topic/dp/ref/152_max-product-subarray.cpp` |

### Optional

| # | `ref/optional/` file | Source | Difficulty | Core DS / algorithm | `prep/` |
|---|---|---|---|---|---|
| 09 | `09-balanced-strings-count.cpp` | Moveworks OA 2024 (intern + full time) | Medium | DP over the last letter, mod | — |
| 10 | `10-min-replacements-to-sort.cpp` | Moveworks OA 2024 (LC 2366) | Hard | greedy right to left | — |

09's summary gives a second example (n = 3, d = 3 → 224) that does not fit its own rule;
the file says so in `// clarify:`.

## Reported, idea only

| Problem | Source | Idea |
|---|---|---|
| Guess the secret word with match feedback (LC 843) | prep sites | guess the word whose largest group of same-match-count candidates is smallest; keep the candidates whose match with the guess equals the feedback |
| Replace Words (LC 648) | Moveworks | trie of roots (`select/15-trie/01-implement-trie.cpp`); for each word walk the trie and stop at the first root end |
| Trapped zones: SCCs, then SCCs with an edge coming in and none going out | ServiceNow IC2 2026-11, Hard | Kosaraju (DFS finish order, DFS on the reversed graph) or Tarjan; condense, count in / out edges per component |
| Count valid words: length > 3, a vowel and a consonant, letters and digits only | ServiceNow intern panel 2023 | split on spaces with a char loop, check each token; O(1) space version walks two pointers over the original string |
| Binary tree boundary clockwise (LC 545); distinct islands, iterative (LC 694); generate parentheses, iterative | ServiceNow 2025 | 545: left boundary, leaves, right boundary reversed; 694: BFS with a shape string of relative offsets; parens: a stack of (string, open, close) |
| Inorder traversal is a palindrome | ServiceNow 2024 | two iterators: a forward inorder stack and a reverse inorder stack, compare until they cross |
| Regions cut by slashes (LC 959) | ServiceNow 2024 | split each cell into 4 triangles, union-find, count components |
| Max subarray sum with one deletion (LC 1186) | ServiceNow IC3 2026-11 | two DPs: best ending at i with no deletion, with one deletion |
| BFS shortest path through an interface (`isDestination()`, `getAvailableNextSteps()`) | ServiceNow 2021 | plain BFS with a visited set; the graph is only reachable through the calls |
| Shop1 / shop2: choose K indexes, maximize min of the two sums | ServiceNow OA 2024 | statement too short to pin down; skip |

## Doing order

1. `ref/must/03` (Hangman), `ref/must/02` (all three parts), `ref/must/01` (both parts).
2. LC 1249, LRU, `ref/must/06` (the ServiceNow new-grad / intern set).
3. LC 68 (`company/scientech/ref/must/01`).
4. `ref/recommended/07`, `08`, then Word Break and Word Break II.
5. `ref/recommended/04`, `05`, LFU.
6. OOD (elevator, deck of cards) and system design in the round notes.
7. The idea table, read once; `ref/optional/` only if time is left.

Sources: his 1point3acres / fastprep.io summary (above); smartrecruiters / zapply.jobs
posting (2026-10-26); prachub.com Moveworks pages; interviewquery.com and dataford.io
Moveworks guides; interviewsolver.com LeetCode tag; Glassdoor Moveworks SWE review
(Apr 2026).
