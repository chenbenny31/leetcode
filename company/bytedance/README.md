# ByteDance (TikTok) — NG coding prep

Sources: hack2hire ByteDance/TikTok NG reports (tag `ByteDance (hack2hire)`) and the LeetCode
ByteDance company list (tag `ByteDance / LC`); those files exclude OA-only questions. Every
file compiles with `g++ -std=c++17 -Wall -Wextra` and prints `label PASS` per test.
Follow-ups are separate functions/classes without tests. Problems shared with Apple reuse the Apple version.
Rows tagged `FastPrep #N` (its ByteDance SWE list: NG / intern / full-time, pulled
2026-10-30) or `1p3a` + thread id (1point3acres SDE reports, 2026-08 to 10) were added on
2026-10-30 in the current conventions (blank line after the includes, ternary PASS/FAIL,
C++17 structured bindings). A newer variant of an older problem has its own file.

Numbers are priority order, continuous across folders: ng/ 01-36, important/ 37-100,
optional/ 101-140. Within a folder, problems with more / more recent sightings and
problems named in recent reports come first. A new problem gets a number by priority, and
the files after it are renumbered (update README, STUDY_ORDER.md and cross-references).

## ng/ — must do (top of the LC frequency list + most-reported phone/onsite questions)
| File | Source | Problem |
|---|---|---|
| `01-lru-cache.cpp` | ByteDance (hack2hire) / LC 146 | lru cache |
| `02-course-schedule.cpp` | ByteDance (hack2hire) / LC 207 | course schedule |
| `03-multi-head-attention.cpp` | FastPrep #1 (4 sightings, phone/onsite) | multi-head attention forward pass |
| `04-alien-dictionary-smallest.cpp` | FastPrep #4 / LC 269 variant | alien dictionary, lexicographically smallest order |
| `05-meeting-rooms.cpp` | FastPrep #6-7 / LC 252, 253 (NG HM round) | meeting rooms I + II |
| `06-sjf-process-scheduling.cpp` | FastPrep #8 (NG team-lead round) | shortest-job-first scheduling with a heap |
| `07-kth-largest-quickselect.cpp` | ByteDance (hack2hire) / LC 215 | kth largest quickselect |
| `08-merge-intervals.cpp` | ByteDance (hack2hire) / LC 56 | merge intervals |
| `09-word-ladder.cpp` | ByteDance / LC 127 | word ladder |
| `10-tic-tac-toe.cpp` | FastPrep #11, #35 / LC 348 | tic-tac-toe game over + O(1) move class |
| `11-heap-priority-queue.cpp` | FastPrep #10 | hand-written min-heap priority queue |
| `12-search-rotated-array.cpp` | ByteDance (hack2hire) / LC 33 | search rotated array |
| `13-path-sum-ii.cpp` | ByteDance (hack2hire) / LC 113 | path sum ii |
| `14-design-hashmap.cpp` | ByteDance (hack2hire) / LC 706 | design hashmap |
| `15-implement-queue.cpp` | ByteDance (hack2hire) | implement queue |
| `16-longest-repeating-char-replacement.cpp` | ByteDance (hack2hire) / LC 424 | longest repeating char replacement |
| `17-longest-substring-no-repeat.cpp` | ByteDance (hack2hire) / LC 3 | longest substring no repeat |
| `18-all-paths-directed.cpp` | ByteDance (hack2hire) | all paths directed |
| `19-all-cycles-directed.cpp` | ByteDance (hack2hire) | all cycles directed |
| `20-number-of-islands.cpp` | ByteDance (hack2hire) / LC 200 | number of islands |
| `21-shortest-bridge.cpp` | ByteDance / LC 934 | shortest bridge |
| `22-house-robber.cpp` | ByteDance (hack2hire) / LC 198 | house robber |
| `23-two-sum.cpp` | ByteDance / LC 1 | two sum |
| `24-add-two-numbers.cpp` | ByteDance / LC 2 | add two numbers |
| `25-combination-sum.cpp` | ByteDance (hack2hire) / LC 39 | combination sum |
| `26-multiply-strings.cpp` | ByteDance / LC 43 | multiply strings |
| `27-max-subarray.cpp` | ByteDance (hack2hire) / LC 53 | max subarray |
| `28-best-time-stock.cpp` | ByteDance / LC 121 | best time stock |
| `29-maximal-square.cpp` | ByteDance / LC 221 | maximal square |
| `30-basic-calculator-ii.cpp` | ByteDance (hack2hire) / LC 227 | basic calculator ii |
| `31-basic-calculator.cpp` | ByteDance / LC 224 | basic calculator |
| `32-top-k-frequent.cpp` | ByteDance / LC 347 | top k frequent |
| `33-max-width-binary-tree.cpp` | ByteDance (hack2hire) / LC 662 | max width binary tree |
| `34-rotting-oranges.cpp` | ByteDance / LC 994 | rotting oranges |
| `35-longest-common-subsequence.cpp` | ByteDance / LC 1143 | longest common subsequence |
| `36-reverse-words.cpp` | ByteDance (hack2hire) / LC 151 | reverse words |

## important/ — rest of the LC list and the solid hack2hire reports
| File | Source | Problem |
|---|---|---|
| `37-dynamic-median-deletions.cpp` | FastPrep #24 (FT onsite, 2 sightings) | median with add / delete (two heaps, lazy delete) |
| `38-restore-addresses-k-segments.cpp` | FastPrep #25 / LC 93 follow-up | restore addresses with k segments |
| `39-restore-ip-addresses.cpp` | ByteDance / LC 93 | restore ip addresses |
| `40-non-overlapping-intervals.cpp` | FastPrep #26 / LC 435 | min removals for non-overlapping intervals (+ proof) |
| `41-seller-round-robin-scheduler.cpp` | FastPrep #42 (FT onsite) | weighted round-robin seller task scheduler |
| `42-kv-store-codec.cpp` | FastPrep #28 | serialize / deserialize a string KV store |
| `43-possible-bipartition.cpp` | FastPrep #29 / LC 886 | possible bipartition |
| `44-smallest-topological-order.cpp` | FastPrep #30 | lexicographically smallest topological order |
| `45-sliding-window-max.cpp` | ByteDance / LC 239 | sliding window max |
| `46-max-path-sum.cpp` | ByteDance (hack2hire) / LC 124 | max path sum |
| `47-time-based-kv.cpp` | 1p3a 1188918 / LC 981 | time based key-value store |
| `48-task-scheduler-remove.cpp` | 1p3a 1186877 | task scheduler with addTask / removeTask / popTask |
| `49-sparse-matrix-multiply.cpp` | FastPrep #27 / LC 311 | sparse matrix multiplication |
| `50-spiral-matrix.cpp` | FastPrep #22 / LC 54 (intern) | spiral matrix, O(1) extra space |
| `51-rotated-dup-leftmost.cpp` | FastPrep #23 / LC 81 variant (intern) | rotated array with duplicates, leftmost index |
| `52-burst-balloons.cpp` | 1p3a 1194630 / LC 312 (infra) | burst balloons |
| `53-all-paths-to-destination.cpp` | 1p3a 1194630 / LC 1059 (infra) | all paths from source lead to destination |
| `54-vertical-order-traversal.cpp` | 1p3a 1191916 / LC 314 | binary tree vertical order traversal |
| `55-shared-ptr.cpp` | 1p3a 1189695 (intern infra) | implement shared_ptr |
| `56-combination-sum-signed.cpp` | FastPrep #39 / LC 40 variant | combination sum II with negative values |
| `57-two-distinct-substring.cpp` | FastPrep #40 / LC 159 | longest substring with at most two distinct chars |
| `58-interval-list-intersection.cpp` | FastPrep #43 / LC 986 | intersect two sorted interval lists |
| `59-min-window-substring.cpp` | FastPrep #44 / LC 76 | minimum covering substring |
| `60-flat-comments-tree.cpp` | FastPrep #47 | flat comments to nested JSON tree |
| `61-add-signed-char-arrays.cpp` | FastPrep #48 | add signed char arrays (62 handles only non-negative) |
| `62-add-strings-char-arrays.cpp` | ByteDance (hack2hire) / LC 415 variant | add strings char arrays |
| `63-longest-palindrome-manacher.cpp` | FastPrep #49 / LC 5 | longest palindrome with Manacher (64 has no Manacher) |
| `64-longest-palindromic-substring.cpp` | ByteDance (hack2hire) / LC 5 | longest palindromic substring |
| `65-reverse-k-group.cpp` | FastPrep #51 / LC 25 | reverse nodes in k-group |
| `66-largest-number-below-n.cpp` | FastPrep #53 | largest number below n from reusable digits |
| `67-delete-k-balance-index-sums.cpp` | FastPrep #37 | delete k values to balance even/odd index sums |
| `68-csv-record-parser.cpp` | FastPrep #32 | parse one CSV record |
| `69-maximize-fish.cpp` | FastPrep #36 | maximize fish over limited hours (level aggregation) |
| `70-subarray-sum-k.cpp` | ByteDance / LC 560 | subarray sum k |
| `71-valid-parentheses.cpp` | ByteDance (hack2hire) / LC 20 | valid parentheses |
| `72-subsets.cpp` | ByteDance (hack2hire) / LC 78 | subsets |
| `73-substructure-of-tree.cpp` | ByteDance (hack2hire) / Jianzhi Offer 26 | substructure of tree |
| `74-kth-node-from-end.cpp` | ByteDance (hack2hire) | kth node from end |
| `75-three-sum-closest.cpp` | ByteDance / LC 16 | three sum closest |
| `76-merge-k-sorted-lists.cpp` | ByteDance / LC 23 | merge k sorted lists |
| `77-trapping-rain-water.cpp` | ByteDance / LC 42 | trapping rain water |
| `78-jump-game.cpp` | ByteDance / LC 55 | jump game |
| `79-reverse-linked-list-ii.cpp` | ByteDance / LC 92 | reverse linked list ii |
| `80-zigzag-level-order.cpp` | ByteDance / LC 103 | zigzag level order |
| `81-clone-graph.cpp` | ByteDance / LC 133 | clone graph |
| `82-sort-list.cpp` | ByteDance / LC 148 | sort list |
| `83-find-peak-element.cpp` | ByteDance / LC 162 | find peak element |
| `84-decode-string.cpp` | ByteDance / LC 394 | decode string |
| `85-remove-k-digits.cpp` | ByteDance / LC 402 | remove k digits |
| `86-delete-node-bst.cpp` | ByteDance / LC 450 | delete node bst |
| `87-single-element-sorted.cpp` | ByteDance / LC 540 | single element sorted |
| `88-accounts-merge.cpp` | ByteDance / LC 721 | accounts merge |
| `89-infected-tree-time.cpp` | ByteDance / LC 2385 | infected tree time |
| `90-longest-common-prefix-length.cpp` | ByteDance / LC 3043 | longest common prefix length |
| `91-design-memory-allocator.cpp` | ByteDance / LC 2502 | design memory allocator |
| `92-product-last-k.cpp` | ByteDance / LC 1352 | product last k |
| `93-top-k-kols.cpp` | ByteDance (hack2hire) | top k kols |
| `94-add-search-word.cpp` | ByteDance (hack2hire) / LC 211 | add search word |
| `95-sqrt-precision.cpp` | ByteDance (hack2hire) | sqrt precision |
| `96-permutation-in-string.cpp` | ByteDance (hack2hire) / LC 567 | permutation in string |
| `97-lfu-cache.cpp` | ByteDance (hack2hire) / LC 460 | lfu cache |
| `98-group-anagrams.cpp` | ByteDance (hack2hire) / LC 49 | group anagrams |
| `99-regex-matching.cpp` | ByteDance (hack2hire) / LC 10 | regex matching |
| `100-longest-increasing-subsequence.cpp` | ByteDance (hack2hire) / LC 300 | longest increasing subsequence |

## optional/ — hards, niche, or intern / non-SWE-role reports
| File | Source | Problem |
|---|---|---|
| `101-house-robber-iii.cpp` | ByteDance (hack2hire) / LC 337 | house robber iii |
| `102-construct-pre-post.cpp` | ByteDance (hack2hire) / LC 889 | construct pre post |
| `103-cyclic-shift-abs-diff-sums.cpp` | FastPrep #5 (OA) | sorted abs-difference sums across cyclic shifts |
| `104-even-digit-count.cpp` | FastPrep #15 (OA) | count numbers with an even digit count |
| `105-house-segments-destruction.cpp` | FastPrep #16 (OA) | house segments after sequential destruction |
| `106-sort-matrix-borders.cpp` | FastPrep #17 (OA) | sort concentric matrix borders clockwise |
| `107-snake-to-camel-backticks.cpp` | 1p3a 1189035 (intern OA) | snake_case to camelCase inside backticks |
| `108-cyclic-shift-pairs.cpp` | 1p3a 1189035 (intern OA) | pairs that are cyclic digit rotations |
| `109-digit-group-sum.cpp` | 1p3a 1189035 (intern OA) / LC 2243 | digit sum of string in groups of k |
| `110-min-stickers.cpp` | FastPrep #45 / LC 691 | minimum stickers to form a target |
| `111-pythagorean-triple-multiplicity.cpp` | FastPrep #46 | pythagorean triple with array multiplicity |
| `112-leftmost-binary-search.cpp` | FastPrep #50 | leftmost binary search match |
| `113-path-exists-graph.cpp` | FastPrep #38 / LC 1971 | path existence in an undirected graph |
| `114-most-frequent-tiebreak.cpp` | FastPrep #56 | most frequent integer, smaller tie-break |
| `115-order-validator-dag.cpp` | FastPrep #52 (format FastPrep-authored) | order validator rule DAG |
| `116-affine-transforms.cpp` | FastPrep #31 (~20% source match) | apply affine array transformations |
| `117-text-justification.cpp` | ByteDance / LC 68 | text justification |
| `118-remove-duplicate-letters.cpp` | ByteDance / LC 316 | remove duplicate letters |
| `119-shortest-distance-buildings.cpp` | ByteDance / LC 317 | shortest distance buildings |
| `120-min-area-rectangle.cpp` | ByteDance / LC 939 | min area rectangle |
| `121-remove-zero-sum-nodes.cpp` | ByteDance / LC 1171 | remove zero sum nodes |
| `122-k-length-substrings-no-repeat.cpp` | ByteDance (hack2hire) / LC 1100 | k length substrings no repeat |
| `123-remove-duplicates-ii.cpp` | ByteDance (hack2hire) / LC 80 | remove duplicates ii |
| `124-missing-ranges.cpp` | ByteDance (hack2hire) / LC 163 | missing ranges |
| `125-swim-rising-water.cpp` | ByteDance (hack2hire) / LC 778 | swim rising water |
| `126-combinations.cpp` | ByteDance (hack2hire) / LC 77 | combinations |
| `127-photo-groups.cpp` | ByteDance (hack2hire) / LC 547 | photo groups |
| `128-valid-parenthesis-string.cpp` | ByteDance (hack2hire) / LC 678 | valid parenthesis string |
| `129-search-insert-position.cpp` | ByteDance (hack2hire) / LC 35 | search insert position |
| `130-circular-deque-index.cpp` | ByteDance (hack2hire) | circular deque index |
| `131-daily-login-counts.cpp` | ByteDance (hack2hire) | daily login counts |
| `132-min-cost-steps.cpp` | ByteDance (hack2hire) | min cost steps |
| `133-construct-pre-in.cpp` | ByteDance (hack2hire) / LC 105 | construct pre in |
| `134-populate-next-right.cpp` | ByteDance (hack2hire) / LC 116 | populate next right |
| `135-n-queens.cpp` | ByteDance (hack2hire) / LC 51 | n queens |
| `136-largest-rectangle-histogram.cpp` | ByteDance (hack2hire) / LC 84 | largest rectangle histogram |
| `137-kth-smallest-subarray-sum.cpp` | ByteDance (hack2hire) / LC 1918 | kth smallest subarray sum |
| `138-camel-case-filter.cpp` | ByteDance (hack2hire) | camel case filter |
| `139-max-candies.cpp` | ByteDance (hack2hire) / LC 2226 | max candies |
| `140-min-jumps-home.cpp` | ByteDance (hack2hire) / LC 1654 | min jumps home |

## Also reported in FastPrep / 1p3a — already covered (review, don't redo)
| Reported problem | Existing file |
|---|---|
| LRU cache (FastPrep #2, 4 sightings; 1p3a 1185609, 1188610, intern 1191838) | `ng/01-lru-cache.cpp` |
| Course Schedule (FastPrep #3; 1p3a NG 1192024, 1185892, USDS 1191111) | `ng/02-course-schedule.cpp` |
| Kth largest without heap / QuickSelect kth smallest (FastPrep #9, #19) | `ng/07-kth-largest-quickselect.cpp` (kth smallest = (n-k+1)-th largest) |
| Word Ladder (1p3a NG 1194453) | `ng/09-word-ladder.cpp` |
| Search in Rotated Sorted Array (FastPrep #20; 1p3a 1186988) | `ng/12-search-rotated-array.cpp` |
| Basic calculator with parens follow-up (1p3a 1187474) | `ng/30-basic-calculator-ii.cpp`, `ng/31-basic-calculator.cpp` |
| Longest Repeating Character Replacement (FastPrep #18) | `ng/16-longest-repeating-char-replacement.cpp` |
| Longest Substring Without Repeating (FastPrep #54) | `ng/17-longest-substring-no-repeat.cpp` |
| FIFO queue (FastPrep #21, intern) | `ng/15-implement-queue.cpp` |
| Binary tree target-sum paths (FastPrep #12; 1p3a 1189120) | `ng/13-path-sum-ii.cpp` |
| Enumerate directed paths and cycles (FastPrep #13; 1p3a 1188883) | `ng/18-all-paths-directed.cpp` + `ng/19-all-cycles-directed.cpp` (FastPrep wants sorted comma strings) |
| Design HashMap (FastPrep #14; 1p3a 1188610) | `ng/14-design-hashmap.cpp` |
| Merge intervals (FastPrep #34; 1p3a 1192940) | `ng/08-merge-intervals.cpp` |
| Restore IP addresses (1p3a 1188464, 1187531) | `important/39-restore-ip-addresses.cpp` (k segments: `important/38-restore-addresses-k-segments.cpp`) |
| Sliding Window Maximum (1p3a 1194556) | `important/45-sliding-window-max.cpp` |
| Count subarrays with sum k (FastPrep #55) | `important/70-subarray-sum-k.cpp` |
| Valid Parentheses (FastPrep #41) | `important/71-valid-parentheses.cpp` |
| Binary Tree Maximum Path Sum (FastPrep #33; 1p3a 1192940 variant) | `important/46-max-path-sum.cpp` |
| House Robber III (1p3a infra 1194630, LC 337) | `optional/101-house-robber-iii.cpp` |
| Construct from preorder + postorder (1p3a NG 1187092, LC 889) | `optional/102-construct-pre-post.cpp` |
| Reverse linked list (1p3a 1188918, LC 206) | `topic/linked-list/ref/206_reverse-list.cpp` |
| Max area of island variant (1p3a 1185560, LC 695) | `topic/graphs/ref/695_max-area-island.cpp` |
| Create binary tree from descriptions (1p3a 1188044, LC 2196) | `company/amazon/ref/27-tree-from-descriptions.cpp` |

## What the 1p3a reports say about the loop (45 SDE posts, 2026-08-07 to 10-29)
- Resume deep dives often take 20-50 min of a round: design reasons, internals, alternatives,
  limits and how it was measured — not just what the project did.
- NG loops can include system design (27NG 1192024 got a ticketing SD although the invite
  said coding). Prepare SD even for NG.
- Some rounds require compiling and writing your own tests (HackerRank), others only core
  logic; ask first. Interviewers asked for dry runs and explanation before code changes
  (Alien Dictionary: dedup edges before adding in-degree).
- Infra / backend rounds also ask fundamentals: MySQL composite / covering index, cache-aside
  consistency, cache avalanche / breakdown, Kafka, K8s Service / ClusterIP / NodePort, DNS
  TTL, HTTP/1-2-3, gRPC vs HTTP, TCP handshake, HTTPS/TLS, virtual memory, compile / link
  steps, C++ basics.
- AI use depends on the round: one SD loop (1189821) required prompting AI live; others
  were traditional.

## System design reported (disclosed requirements only)
| Topic | Disclosed requirements / follow-ups | Source |
|---|---|---|
| Ticketing (抢票) | burst traffic, LB, waiting room / queue, rate limit, inventory deduction, hold / reservation, release on payment timeout, no overselling (NG round) | 1p3a 1192024 |
| Payment | real-time hold, batch settlement, retries without double charge, consistency, failure cases; another post only high level | 1p3a 1192940, 1191111 |
| E-commerce platform | title only | 1p3a 1191916, 1187413 |
| Content moderation | async ingestion -> several model scores -> rule engine -> downstream action; model / rule versioning, audit records, rule config, feedback loop; ML as a black box | 1p3a 1189385, 1185936 |
| Top 10 songs per country | listen / click events, ~10 min refresh, eventual consistency; stream aggregation, hot keys, sharding | 1p3a 1187870 |
| File upload / download | AI-enabled SD, scale not disclosed | 1p3a 1189821 |
| Monitoring dashboard | latency focus, AI-enabled SD | 1p3a 1189821 |
| Agent quota calculation | title only | 1p3a 1189821 |
| Exchange-rate calculation | title only | 1p3a 1193703 |
