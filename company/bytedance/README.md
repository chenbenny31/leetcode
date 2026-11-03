# ByteDance (TikTok) — NG coding prep

Sources: hack2hire ByteDance/TikTok NG reports and jobright.ai pastes (tag `ByteDance (hack2hire)`;
2026-11-01 additions are tagged `ByteDance (jobright)`) and the LeetCode
ByteDance company list (tag `ByteDance / LC`); those files exclude OA-only questions. Every
file compiles with `g++ -std=c++17 -Wall -Wextra` and prints `label PASS` per test.
Follow-ups are separate functions/classes without tests. Problems shared with Apple reuse the Apple version.
Rows tagged `FastPrep #N` (its ByteDance SWE list: NG / intern / full-time, pulled
2026-10-30) or `1p3a` + thread id (1point3acres SDE reports, 2026-08 to 10) were added on
2026-10-30 in the current conventions (blank line after the includes, ternary PASS/FAIL,
C++17 structured bindings). A newer variant of an older problem has its own file.

Numbers are priority order, continuous across ref/ folders: ref/must/ 01-17, ref/recommended/ 18-110,
ref/optional/ 111-143. Tiers follow the Amazon rules: must = reported in phone / onsite rounds 3+ times
(fastprep, 1p3a, hack2hire / jobright pages combined) or fastprep ×2+; recommended = any other
phone / onsite report; optional = the LeetCode company list only. OA-only problems live in
`oa/` (01-07), outside ref/. A new problem gets a number by priority, and the files after it
are renumbered (update README, STUDY_ORDER.md and cross-references).

## ref/must/ — must do: reported in phone / onsite rounds 3+ times (or fastprep ×2+)
| File | Source | Problem |
|---|---|---|
| `01-lru-cache.cpp` | ByteDance (hack2hire) / LC 146 | lru cache |
| `02-course-schedule.cpp` | ByteDance (hack2hire) / LC 207 | course schedule |
| `03-multi-head-attention.cpp` | FastPrep #1 (4 sightings, phone/onsite) | multi-head attention forward pass |
| `04-kth-largest-quickselect.cpp` | ByteDance (hack2hire) / LC 215 | kth largest quickselect |
| `05-merge-intervals.cpp` | ByteDance (hack2hire) / LC 56 | merge intervals |
| `06-design-hashmap.cpp` | ByteDance (hack2hire) / LC 706 | design hashmap |
| `07-max-path-sum.cpp` | ByteDance (hack2hire) / LC 124 | max path sum |
| `08-meeting-rooms.cpp` | FastPrep #6-7 / LC 252, 253 (NG HM round) | meeting rooms I + II |
| `09-alien-dictionary-smallest.cpp` | FastPrep #4 / LC 269 variant | alien dictionary, lexicographically smallest order |
| `10-search-rotated-array.cpp` | ByteDance (hack2hire) / LC 33 | search rotated array |
| `11-path-sum-ii.cpp` | ByteDance (hack2hire) / LC 113 | path sum ii |
| `12-longest-substring-no-repeat.cpp` | ByteDance (hack2hire) / LC 3 | longest substring no repeat |
| `13-all-paths-directed.cpp` | ByteDance (hack2hire) | all paths directed |
| `14-all-cycles-directed.cpp` | ByteDance (hack2hire) | all cycles directed |
| `15-basic-calculator-ii.cpp` | ByteDance (hack2hire) / LC 227 | basic calculator ii |
| `16-dynamic-median-deletions.cpp` | FastPrep #24 (FT onsite, 2 sightings) | median with add / delete (two heaps, lazy delete) |
| `17-non-overlapping-intervals.cpp` | FastPrep #26 / LC 435 | min removals for non-overlapping intervals (+ proof) |

## ref/recommended/ — other phone / onsite reports
Order: reported twice first; then concurrency (32-36, the recruiter prep doc says Rounds 1-2
test concurrency); then reported once (NG / full-time SWE); then intern, infra and MLE-role
coding questions.

| File | Source | Problem |
|---|---|---|
| `18-word-ladder.cpp` | ByteDance / LC 127 | word ladder |
| `19-tic-tac-toe.cpp` | FastPrep #11, #35 / LC 348 | tic-tac-toe game over + O(1) move class |
| `20-implement-queue.cpp` | ByteDance (hack2hire) | implement queue |
| `21-longest-repeating-char-replacement.cpp` | ByteDance (hack2hire) / LC 424 | longest repeating char replacement |
| `22-number-of-islands.cpp` | ByteDance (hack2hire) / LC 200 | number of islands |
| `23-reverse-words.cpp` | ByteDance (hack2hire) / LC 151 | reverse words |
| `24-restore-ip-addresses.cpp` | ByteDance / LC 93 | restore ip addresses |
| `25-restore-addresses-k-segments.cpp` | FastPrep #25 / LC 93 follow-up | restore addresses with k segments |
| `26-smallest-topological-order.cpp` | FastPrep #30 | lexicographically smallest topological order |
| `27-min-window-substring.cpp` | FastPrep #44 / LC 76 | minimum covering substring |
| `28-substructure-of-tree.cpp` | ByteDance (hack2hire) / Jianzhi Offer 26 | substructure of tree |
| `29-kth-smallest-two-sorted.cpp` | ByteDance (jobright, onsite) + LC 4 follow-up (MLE phone) | kth smallest of two sorted arrays (+ median of two sorted arrays) |
| `30-house-robber-iii.cpp` | ByteDance (hack2hire) / LC 337 | house robber iii |
| `31-construct-pre-post.cpp` | ByteDance (hack2hire) / LC 889 | construct pre post |
| `32-bounded-blocking-queue.cpp` | prep doc (concurrency) / LC 1188 | bounded blocking queue: mutex + two condition variables |
| `33-foobar-alternately.cpp` | prep doc (concurrency) / LC 1115 | two threads print foo / bar alternately (turn flag + cv) |
| `34-thread-safe-lru.cpp` | prep doc (concurrency, memory) | LRU behind one mutex (+ sharding follow-up) |
| `35-thread-pool.cpp` | prep doc (concurrency) | fixed worker pool: job queue, cv, graceful shutdown |
| `36-thread-safe-rate-limiter.cpp` | prep doc (concurrency correctness) | sliding-window limiter, check + record under one lock |
| `37-sjf-process-scheduling.cpp` | FastPrep #8 (NG team-lead round) | shortest-job-first scheduling with a heap |
| `38-heap-priority-queue.cpp` | FastPrep #10 | hand-written min-heap priority queue |
| `39-basic-calculator.cpp` | ByteDance / LC 224 | basic calculator |
| `40-seller-round-robin-scheduler.cpp` | FastPrep #42 (FT onsite) | weighted round-robin seller task scheduler |
| `41-kv-store-codec.cpp` | FastPrep #28 | serialize / deserialize a string KV store |
| `42-possible-bipartition.cpp` | FastPrep #29 / LC 886 | possible bipartition |
| `43-sliding-window-max.cpp` | ByteDance / LC 239 | sliding window max |
| `44-time-based-kv.cpp` | 1p3a 1188918 / LC 981 | time based key-value store |
| `45-task-scheduler-remove.cpp` | 1p3a 1186877 | task scheduler with addTask / removeTask / popTask |
| `46-vertical-order-traversal.cpp` | 1p3a 1191916 / LC 314 | binary tree vertical order traversal |
| `47-sparse-matrix-multiply.cpp` | FastPrep #27 / LC 311 | sparse matrix multiplication |
| `48-combination-sum-signed.cpp` | FastPrep #39 / LC 40 variant | combination sum II with negative values |
| `49-two-distinct-substring.cpp` | FastPrep #40 / LC 159 | longest substring with at most two distinct chars |
| `50-interval-list-intersection.cpp` | FastPrep #43 / LC 986 | intersect two sorted interval lists |
| `51-flat-comments-tree.cpp` | FastPrep #47 | flat comments to nested JSON tree |
| `52-add-strings-char-arrays.cpp` | ByteDance (hack2hire) / LC 415 variant | add strings char arrays |
| `53-add-signed-char-arrays.cpp` | FastPrep #48 | add signed char arrays (62 handles only non-negative) |
| `54-longest-palindromic-substring.cpp` | ByteDance (hack2hire) / LC 5 | longest palindromic substring |
| `55-longest-palindrome-manacher.cpp` | FastPrep #49 / LC 5 | longest palindrome with Manacher (64 has no Manacher) |
| `56-reverse-k-group.cpp` | FastPrep #51 / LC 25 | reverse nodes in k-group |
| `57-largest-number-below-n.cpp` | FastPrep #53 | largest number below n from reusable digits |
| `58-delete-k-balance-index-sums.cpp` | FastPrep #37 | delete k values to balance even/odd index sums |
| `59-csv-record-parser.cpp` | FastPrep #32 | parse one CSV record |
| `60-maximize-fish.cpp` | FastPrep #36 | maximize fish over limited hours (level aggregation) |
| `61-subarray-sum-k.cpp` | ByteDance / LC 560 | subarray sum k |
| `62-valid-parentheses.cpp` | ByteDance (hack2hire) / LC 20 | valid parentheses |
| `63-subsets.cpp` | ByteDance (hack2hire) / LC 78 | subsets |
| `64-kth-node-from-end.cpp` | ByteDance (hack2hire) | kth node from end |
| `65-max-width-binary-tree.cpp` | ByteDance (hack2hire) / LC 662 | max width binary tree |
| `66-group-anagrams.cpp` | ByteDance (hack2hire) / LC 49 | group anagrams |
| `67-combination-sum.cpp` | ByteDance (hack2hire) / LC 39 | combination sum |
| `68-top-k-kols.cpp` | ByteDance (hack2hire) | top k kols |
| `69-add-search-word.cpp` | ByteDance (hack2hire) / LC 211 | add search word |
| `70-sqrt-precision.cpp` | ByteDance (hack2hire) | sqrt precision |
| `71-permutation-in-string.cpp` | ByteDance (hack2hire) / LC 567 | permutation in string |
| `72-lfu-cache.cpp` | ByteDance (hack2hire) / LC 460 | lfu cache |
| `73-regex-matching.cpp` | ByteDance (hack2hire) / LC 10 | regex matching |
| `74-longest-increasing-subsequence.cpp` | ByteDance (hack2hire) / LC 300 | longest increasing subsequence |
| `75-validate-evaluate-expression.cpp` | ByteDance (jobright, phone) / LC 227 variant | validate and evaluate an expression (recursive descent, div-by-zero invalid) |
| `76-making-large-island.cpp` | ByteDance (jobright, phone) / LC 827 | largest island after flipping one 0 |
| `77-split-string-equal-ones.cpp` | ByteDance (jobright, phone) / LC 1573 | ways to split a binary string into 3 parts with equal ones |
| `78-min-stickers.cpp` | FastPrep #45 / LC 691 | minimum stickers to form a target |
| `79-pythagorean-triple-multiplicity.cpp` | FastPrep #46 | pythagorean triple with array multiplicity |
| `80-leftmost-binary-search.cpp` | FastPrep #50 | leftmost binary search match |
| `81-path-exists-graph.cpp` | FastPrep #38 / LC 1971 | path existence in an undirected graph |
| `82-most-frequent-tiebreak.cpp` | FastPrep #56 | most frequent integer, smaller tie-break |
| `83-order-validator-dag.cpp` | FastPrep #52 (format FastPrep-authored) | order validator rule DAG |
| `84-k-length-substrings-no-repeat.cpp` | ByteDance (hack2hire) / LC 1100 | k length substrings no repeat |
| `85-remove-duplicates-ii.cpp` | ByteDance (hack2hire) / LC 80 | remove duplicates ii |
| `86-missing-ranges.cpp` | ByteDance (hack2hire) / LC 163 | missing ranges |
| `87-swim-rising-water.cpp` | ByteDance (hack2hire) / LC 778 | swim rising water |
| `88-combinations.cpp` | ByteDance (hack2hire) / LC 77 | combinations |
| `89-photo-groups.cpp` | ByteDance (hack2hire) / LC 547 | photo groups |
| `90-valid-parenthesis-string.cpp` | ByteDance (hack2hire) / LC 678 | valid parenthesis string |
| `91-search-insert-position.cpp` | ByteDance (hack2hire) / LC 35 | search insert position |
| `92-circular-deque-index.cpp` | ByteDance (hack2hire) | circular deque index |
| `93-daily-login-counts.cpp` | ByteDance (hack2hire) | daily login counts |
| `94-min-cost-steps.cpp` | ByteDance (hack2hire) | min cost steps |
| `95-populate-next-right.cpp` | ByteDance (hack2hire) / LC 116 | populate next right |
| `96-largest-rectangle-histogram.cpp` | ByteDance (hack2hire) / LC 84 | largest rectangle histogram |
| `97-kth-smallest-subarray-sum.cpp` | ByteDance (hack2hire) / LC 1918 | kth smallest subarray sum |
| `98-spiral-matrix.cpp` | FastPrep #22 / LC 54 (intern) | spiral matrix, O(1) extra space |
| `99-rotated-dup-leftmost.cpp` | FastPrep #23 / LC 81 variant (intern) | rotated array with duplicates, leftmost index |
| `100-burst-balloons.cpp` | 1p3a 1194630 / LC 312 (infra) | burst balloons |
| `101-all-paths-to-destination.cpp` | 1p3a 1194630 / LC 1059 (infra) | all paths from source lead to destination |
| `102-shared-ptr.cpp` | 1p3a 1189695 (intern infra) | implement shared_ptr |
| `103-house-robber.cpp` | ByteDance (hack2hire) / LC 198 | house robber |
| `104-max-subarray.cpp` | ByteDance (hack2hire) / LC 53 | max subarray |
| `105-construct-pre-in.cpp` | ByteDance (hack2hire) / LC 105 | construct pre in |
| `106-n-queens.cpp` | ByteDance (hack2hire) / LC 51 | n queens |
| `107-max-candies.cpp` | ByteDance (hack2hire) / LC 2226 | max candies |
| `108-min-jumps-home.cpp` | ByteDance (hack2hire) / LC 1654 | min jumps home |
| `109-camel-case-filter.cpp` | ByteDance (hack2hire) | camel case filter |
| `110-maze-shortest-path.cpp` | ByteDance (jobright, intern phone) | BFS shortest path in a 0/1 maze |

## ref/optional/ — LeetCode ByteDance list only (no interview report), lowest fidelity last
| File | Source | Problem |
|---|---|---|
| `111-shortest-bridge.cpp` | ByteDance / LC 934 | shortest bridge |
| `112-two-sum.cpp` | ByteDance / LC 1 | two sum |
| `113-add-two-numbers.cpp` | ByteDance / LC 2 | add two numbers |
| `114-multiply-strings.cpp` | ByteDance / LC 43 | multiply strings |
| `115-best-time-stock.cpp` | ByteDance / LC 121 | best time stock |
| `116-maximal-square.cpp` | ByteDance / LC 221 | maximal square |
| `117-top-k-frequent.cpp` | ByteDance / LC 347 | top k frequent |
| `118-rotting-oranges.cpp` | ByteDance / LC 994 | rotting oranges |
| `119-longest-common-subsequence.cpp` | ByteDance / LC 1143 | longest common subsequence |
| `120-three-sum-closest.cpp` | ByteDance / LC 16 | three sum closest |
| `121-merge-k-sorted-lists.cpp` | ByteDance / LC 23 | merge k sorted lists |
| `122-trapping-rain-water.cpp` | ByteDance / LC 42 | trapping rain water |
| `123-jump-game.cpp` | ByteDance / LC 55 | jump game |
| `124-reverse-linked-list-ii.cpp` | ByteDance / LC 92 | reverse linked list ii |
| `125-zigzag-level-order.cpp` | ByteDance / LC 103 | zigzag level order |
| `126-clone-graph.cpp` | ByteDance / LC 133 | clone graph |
| `127-sort-list.cpp` | ByteDance / LC 148 | sort list |
| `128-find-peak-element.cpp` | ByteDance / LC 162 | find peak element |
| `129-decode-string.cpp` | ByteDance / LC 394 | decode string |
| `130-remove-k-digits.cpp` | ByteDance / LC 402 | remove k digits |
| `131-delete-node-bst.cpp` | ByteDance / LC 450 | delete node bst |
| `132-single-element-sorted.cpp` | ByteDance / LC 540 | single element sorted |
| `133-accounts-merge.cpp` | ByteDance / LC 721 | accounts merge |
| `134-infected-tree-time.cpp` | ByteDance / LC 2385 | infected tree time |
| `135-longest-common-prefix-length.cpp` | ByteDance / LC 3043 | longest common prefix length |
| `136-design-memory-allocator.cpp` | ByteDance / LC 2502 | design memory allocator |
| `137-product-last-k.cpp` | ByteDance / LC 1352 | product last k |
| `138-text-justification.cpp` | ByteDance / LC 68 | text justification |
| `139-remove-duplicate-letters.cpp` | ByteDance / LC 316 | remove duplicate letters |
| `140-shortest-distance-buildings.cpp` | ByteDance / LC 317 | shortest distance buildings |
| `141-min-area-rectangle.cpp` | ByteDance / LC 939 | min area rectangle |
| `142-remove-zero-sum-nodes.cpp` | ByteDance / LC 1171 | remove zero sum nodes |
| `143-affine-transforms.cpp` | FastPrep #31 (~20% source match) | apply affine array transformations |

## oa/ — online assessments only (do after everything in ref/)
| File | Source | Problem |
|---|---|---|
| `01-cyclic-shift-abs-diff-sums.cpp` | FastPrep #5 (OA) | sorted abs-difference sums across cyclic shifts |
| `02-even-digit-count.cpp` | FastPrep #15 (OA) | count numbers with an even digit count |
| `03-house-segments-destruction.cpp` | FastPrep #16 (OA) | house segments after sequential destruction |
| `04-sort-matrix-borders.cpp` | FastPrep #17 (OA) | sort concentric matrix borders clockwise |
| `05-snake-to-camel-backticks.cpp` | 1p3a 1189035 (intern OA) | snake_case to camelCase inside backticks |
| `06-cyclic-shift-pairs.cpp` | 1p3a 1189035 (intern OA) | pairs that are cyclic digit rotations |
| `07-digit-group-sum.cpp` | 1p3a 1189035 (intern OA) / LC 2243 | digit sum of string in groups of k |

## Also reported in FastPrep / 1p3a — already covered (review, don't redo)
| Reported problem | Existing file |
|---|---|
| LRU cache (FastPrep #2, 4 sightings; 1p3a 1185609, 1188610, intern 1191838) | `ref/must/01-lru-cache.cpp` |
| Course Schedule (FastPrep #3; 1p3a NG 1192024, 1185892, USDS 1191111) | `ref/must/02-course-schedule.cpp` |
| Kth largest without heap / QuickSelect kth smallest (FastPrep #9, #19) | `ref/must/04-kth-largest-quickselect.cpp` (kth smallest = (n-k+1)-th largest) |
| Word Ladder (1p3a NG 1194453) | `ref/recommended/18-word-ladder.cpp` |
| Search in Rotated Sorted Array (FastPrep #20; 1p3a 1186988) | `ref/must/10-search-rotated-array.cpp` |
| Basic calculator with parens follow-up (1p3a 1187474) | `ref/must/15-basic-calculator-ii.cpp`, `ref/recommended/39-basic-calculator.cpp` |
| Longest Repeating Character Replacement (FastPrep #18) | `ref/recommended/21-longest-repeating-char-replacement.cpp` |
| Longest Substring Without Repeating (FastPrep #54) | `ref/must/12-longest-substring-no-repeat.cpp` |
| FIFO queue (FastPrep #21, intern) | `ref/recommended/20-implement-queue.cpp` |
| Binary tree target-sum paths (FastPrep #12; 1p3a 1189120) | `ref/must/11-path-sum-ii.cpp` |
| Enumerate directed paths and cycles (FastPrep #13; 1p3a 1188883) | `ref/must/13-all-paths-directed.cpp` + `ref/must/14-all-cycles-directed.cpp` (FastPrep wants sorted comma strings) |
| Design HashMap (FastPrep #14; 1p3a 1188610) | `ref/must/06-design-hashmap.cpp` |
| Merge intervals (FastPrep #34; 1p3a 1192940) | `ref/must/05-merge-intervals.cpp` |
| Restore IP addresses (1p3a 1188464, 1187531) | `ref/recommended/24-restore-ip-addresses.cpp` (k segments: `ref/recommended/25-restore-addresses-k-segments.cpp`) |
| Sliding Window Maximum (1p3a 1194556) | `ref/recommended/43-sliding-window-max.cpp` |
| Count subarrays with sum k (FastPrep #55) | `ref/recommended/61-subarray-sum-k.cpp` |
| Valid Parentheses (FastPrep #41) | `ref/recommended/62-valid-parentheses.cpp` |
| Binary Tree Maximum Path Sum (FastPrep #33; 1p3a 1192940 variant) | `ref/must/07-max-path-sum.cpp` |
| House Robber III (1p3a infra 1194630, LC 337) | `ref/recommended/30-house-robber-iii.cpp` |
| Construct from preorder + postorder (1p3a NG 1187092, LC 889) | `ref/recommended/31-construct-pre-post.cpp` |
| Reverse linked list (1p3a 1188918, LC 206) | `topic/linked-list/ref/206_reverse-list.cpp` |
| Max area of island variant (1p3a 1185560, LC 695) | `topic/graphs/ref/695_max-area-island.cpp` |
| Create binary tree from descriptions (1p3a 1188044, LC 2196) | `company/amazon/ref/recommended/16-tree-from-descriptions.cpp` |
| Subsets II (jobright phone, LC 90) | `topic/backtracking/ref/90_subsets-ii.cpp` |
| Median of Two Sorted Arrays (jobright MLE phone, LC 4) | `ref/recommended/29-kth-smallest-two-sorted.cpp` (follow-up) |
| Gold bar brain teaser (jobright intern phone): pay 1/7 per day for 7 days with 2 cuts | no code: cut into 1, 2, 4 sevenths and make change each day |

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

## System design reported

`fp ×n` = fastprep.io sightings of the same prompt across companies (ByteDance among them);
fastprep scale numbers are mostly practice assumptions, not interview facts. `1p3a` rows
are what the posts disclosed. Rows are in priority order.

| Topic | Sources | Core requirements | Hard parts / follow-ups to prepare |
|---|---|---|---|
| Personalized recommendation | fp ×32 (phone · onsite; Netflix, Oracle, Google, ByteDance +6) | consented impression / click / save / completion events with stable IDs; multi-strategy retrieval -> ranking with pinned model and feature versions -> paginated list with explanation; popularity fallback | one calibrated probability head (e.g. P(complete \| impression)) checked on held-out real exposures by position / source slice; position bias, train-serve skew, feedback loops + a small exploration slice; write the optimizer update (e.g. Adam vs SGD) with every symbol defined; short video: short clicks vs completion; news feed: 2+ non-content signals with point-in-time features; real-time serving: cache + nearline features vs first-page latency; LLM: one bounded role (text embeddings for retrieval) vs a non-LLM baseline; merchant posts: comments as time-bounded, deletable features (not all positive); two-tower: 50k new videos/day retrievable in 1 h -> embed new items continuously, update the ANN index, retrain towers less often, version query and index vectors together |
| Ticketing (抢票) | fp ×19 (Amazon, Airwallex, Apple, ByteDance +11); 1p3a 1192024 (NG round) | catalog (city -> theatre -> hall -> stable seats; movie <-> artists; show = movie + hall + time) separate from per-show inventory; availability + versioned price quote; 5-min hold; payment -> booking keeps the quoted price; never sell a seat twice | virtual waiting room: per-show FIFO queue, reconnect keeps the position, at most 1,000 active 2-min admission leases for 100k waiters (a lease is not a seat hold); sold-out waitlist: a returned ticket makes one expiring offer at a time, FIFO; hall layout revision = grid + sparse cells (seat / aisle / gap) pinned per show, availability overlaid by seat ID; 10k seat requests/s; 1p3a adds LB, rate limiting, release on payment timeout |
| Metrics monitoring and alerting | fp ×12 (Zscaler, Amazon, Walmart, ByteDance +6); 1p3a 1189821 (latency focus, AI-enabled SD) | samples carry a stable sample ID, service, region, metric, timestamp, value; health dashboards + historical queries; raw kept 30 days, 5-min and hourly rollups 13 months | replayed samples never double count; missing / stale data shows as unknown, not healthy; alert states normal -> pending -> firing -> resolved, one notification per transition via (transition ID, destination) dedup, a timeout is uncertain and retried with the same key; a region keeps ingesting and alerting alone during a partition, global views marked partial; rule changes take effect at an explicit boundary with a policy version; bursts of 3M samples/s, 100M series, 50 regions; TSDB layout: series-ID index, in-memory head block + WAL, compaction, downsampling, retention by dropping whole time blocks; tell processing delay vs missing data vs a bad rule apart |
| Cloud file storage (Drive) | fp ×10 (Amazon, Meta, ByteDance +5); 1p3a 1189821 (AI-enabled SD) | chunked resumable upload / download (files up to 10 GB), folders, rename / move, share as viewer / editor + revoke, versions + restore, multi-device sync within 5 s | a file version = ordered chunk list -> block IDs in an object store; publish metadata only after every chunk is durable, so a partial upload never appears; follow-up: audit "26 servers by the first letter of the user name" (skew, hot users, case and non-A-Z names, server failure) -> place by stable ID or an indirection table, migrate by copy, verify, switch the mapping; estimate metadata DB cost separately from blob storage and transfer |
| Authenticated shopping platform | fp ×8 (onsite; Citadel, ByteDance, Microsoft +4); 1p3a 1191916, 1187413 | register / sign in (salted adaptive password hash), 15-min signed access token + 30-day rotating refresh session, revoke one or all; search 10M products; versioned persistent cart; order through an existing inventory API | ownership comes from the token, never a client-supplied user ID; cart writes carry an expected version (stale -> conflict) + idempotency key; order: durable pending order + outbox command before calling inventory; an uncertain result stays pending and is queryable by idempotency key; tokens never logged or cached; 1M users/day, 10k orders/h |
| Payment hold / charge / settlement | fp ×6 (OpenAI, ByteDance); 1p3a 1192940, 1191111 | hold request -> Kafka -> return an operation ID (queued, not approved); a worker calls the payment network -> approved / declined / uncertain; capture after tip, never above the hold; an unused hold is eventually released | dedup at a durable boundary (idempotency-key table) across retries and restarts; uncertain is not failed; nightly 10 p.m. cutoff per network: immutable charge entries prepared all day in retryable chunks, sealed at cutoff, one wire per network; 10k TPS; correctness over availability; audit history and reconciliation |
| Content moderation | fp ×4 (ByteDance, Google); 1p3a 1189385, 1185936 | already-uploaded image / video -> pending state -> 20-30 text / vision models each emit a 0-1 score + model version -> versioned policy engine -> allow / restrict / remove / needs review -> notify user, act on video or account | at-least-once workers converge to one decision and one notification per (asset, policy version); idempotent actions; low confidence goes to human review within 5 min; reprocess under a newer policy without overwriting history; 10M assets/day with 10x bursts, 100k model tasks/s, 95% decided within 2 min |
| Event booking + analytics | fp ×2 (ByteDance, Groww) | concert / movie search (slightly stale is fine), 5-min seat hold, confirm after payment authorization, retries converge on one outcome | publish catalog / search / hold / booking / payment events to a warehouse on AWS, GCP or Azure (name the services); the warehouse never decides seats; show freshness and dataset version; mask payment and customer IDs; late data and replay without double counting; 3k holds/s, 20k analytics events/s, 99% queryable within 15 min |
| Advertiser campaign configuration | fp ×1 (onsite) | Advertiser -> Campaign -> AdGroup (audience targeting rules) -> Ad -> Creative; edits in drafts with optimistic versions | every edit audited (actor, request ID, old / new version, field-level diff); publish validates the whole hierarchy and creates one immutable snapshot; rollback creates a new version; 50k writes/s, 500k reads/s, published config reaches readers within 60 s |
| Top 10 songs per country | fp ×1 (onsite); 1p3a 1187870 | listen / click events, ~10 min refresh, eventual consistency | stream aggregation, hot keys, sharding |
| Currency conversion service | fp ×1 (onsite); 1p3a 1193703 | title only | |
| Agent quota calculation | 1p3a 1189821 | title only | |
