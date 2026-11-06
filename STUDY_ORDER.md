# Study order

Amazon and Google new grad come first, grouped by topic (Part A). Then come the OA-style problems
(Part B), then the ByteDance, Apple, specialty and Snowflake sets (Part C). Each problem appears
once, at its highest-priority spot. Paths are relative to the repo root.
`topic/<category>/ref/` holds the AI references in the live-interview style;
`topic/<category>/<lc>_<name>.cpp` is your own drill; `company/` and `specialty/` hold the
company sets. A `—` file means the repo has no solution yet.

How to work each item: timer on (25 min for a medium), write it cold in one file with the
`test` function, run it, then diff against the repo file and note what you missed. In reading
mode: read the header recap, name the approach and T/S, then read the body.

## Now — plan and progress (agents: read first, update the log)

- **Next interview:** Amazon SDE new grad loop, Mon 2026-11-09, four rounds. Each usually has
  15–20 min of Leadership Principles; one may be OOD. Google new grad: no date yet.
- **Mode until the loop:** reading code only, no writing.
- **Plan:**

  | When | Read |
  |---|---|
  | Fri 11-06 afternoon | Amazon must 01–09 (`company/amazon/ref/must/`), then skim the other tier 1 rows: LeetCode problems with recent Amazon sightings (fp ×2–12) |
  | Sat 11-07 | Amazon recommended 10–66 (`company/amazon/ref/recommended/`, 57 files) in Part A topic order; 1 h LP stories |
  | Sun 11-08 morning | Amazon optional 67–75 (hack2hire, round not stated); skim the "Also reported (fastprep / h2h)" table in `company/amazon/README.md` (16) |
  | Sun 11-08 afternoon | LP stories and OOD (`~/Code/interview/AMAZON_NG_PREP.md`) |
  | Sun 11-08 evening | Re-read must 01–09 and every file logged as "revisit" |

  Why: the user's recent interviews never asked a LeetCode problem from a frequency list, so
  reported company questions come first. Until the loop, skip the frequency-list-only
  rows (`G-30d`, `A-list`, Amazon optional 76–100). Also skip the README table "Already in the
  repo (LC 30-day list and older reports)", Part B and Part C. After the loop, go topic by
  topic through Part A; inside a topic the rows are already in tier order.
- **Done before this plan:**
  - Written: Amazon must 01–09 (`company/amazon/prep/`); own drills for Arrays & Hashing,
    Two Pointers, Sliding Window and Stack (`topic/<category>/*.cpp` outside `ref/`).
- **Log** (newest last; one line per session: date, rows covered, problems to revisit):
  - 2026-11-06: plan set, then narrowed to reported Amazon questions (frequency-list
    LeetCode dropped until after the loop); nothing read yet.

## Tiers and tags

| Tier | Rows |
|---|---|
| 1 | Amazon must: onsite / phone reports with 3+ sightings |
| 2 | Amazon recommended (other onsite / phone reports) and Google interview reports |
| 3 | Google LeetCode 30-day list |
| 4 | Amazon optional, and the Amazon LeetCode list (3-month frequency) positions 1–70 |
| 5 | Amazon LeetCode list, positions 71 and later |

Source tags: `A-must` / `A-rec` / `A-opt` = tier in `company/amazon/README.md`;
`G-report` = `company/google/README.md`; `G-30d #n` = rank on the Google 30-day list;
`A-list #n` = position on the Amazon LeetCode list. The file listed first is the AI reference
to diff against; "also" names another reference or your own drill for the same problem.

## Part A — Amazon + Google new grad, by topic (351)

| Topic | Problems | Tier 1 | Tier 2 |
|---|---|---|---|
| Arrays & Hashing | 40 | 3 | 20 |
| Two Pointers | 23 | 2 | 5 |
| Sliding Window | 27 | 3 | 6 |
| Stack & Monotonic Stack | 20 | 1 | 8 |
| Binary Search | 19 | 0 | 7 |
| Linked List | 17 | 2 | 8 |
| Trees | 29 | 1 | 11 |
| Trie & Heap | 13 | 4 | 3 |
| Backtracking | 12 | 0 | 4 |
| Graphs | 30 | 4 | 15 |
| Advanced Graphs | 9 | 1 | 3 |
| DP | 31 | 1 | 12 |
| Knapsack & Sequences | 3 | 1 | 1 |
| Greedy & Intervals | 21 | 3 | 5 |
| Bit / Math / Matrix | 20 | 0 | 3 |
| String Matching | 2 | 0 | 0 |
| Design | 12 | 2 | 8 |
| Parser / Simulation / String Engineering | 16 | 0 | 8 |
| Scheduling | 1 | 0 | 0 |
| Monotonic Structures / Sweep / Range Queries | 5 | 0 | 0 |
| Hard block | 1 | 0 | 0 |

### A1. Arrays & Hashing (40)

| # | Tier | File | LC | Problem | Source |
|---|---|---|---|---|---|
| 1 | 1 | `company/apple/ref/must/13-group-anagrams.cpp` · also `topic/arrays-hashing/49_group-anagrams.cpp` | 49 | Group Anagrams | A-must · G-30d #14 · A-list #6 |
| 2 | 1 | `company/apple/ref/must/14-top-k-frequent.cpp` · also `topic/arrays-hashing/347_top-k-frequent.cpp` | 347 | Top K Frequent Elements | A-must · G-30d #23 · A-list #13 |
| 3 | 1 | `company/apple/ref/must/07-two-sum.cpp` · also `topic/arrays-hashing/1_two-sum.cpp` | 1 | Two Sum | A-must · G-30d #1 · A-list #1 |
| 4 | 2 | `company/amazon/ref/recommended/28-website-visit-pattern.cpp` | 1152 | Analyze User Website Visit Pattern | A-rec |
| 5 | 2 | `company/amazon/ref/recommended/37-domain-score.cpp` | — | Leaf domain score = own + ancestors | A-rec |
| 6 | 2 | `company/amazon/ref/recommended/38-consecutive-like-k.cpp` | — | Longest chain with gaps < k (LC 128 general) | A-rec |
| 7 | 2 | `company/amazon/ref/recommended/40-merge-dicts.cpp` | — | Merge two / k dictionaries | A-rec |
| 8 | 2 | `company/amazon/ref/recommended/41-dedup-events.cpp` | — | Latest per id / per-id history | A-rec |
| 9 | 2 | `company/amazon/ref/recommended/43-aggregate-log-counts.cpp` | — | Actions per user, sorted | A-rec |
| 10 | 2 | `company/amazon/ref/recommended/44-weighted-lottery.cpp` | — | K weighted winners without replacement | A-rec |
| 11 | 2 | `company/amazon/ref/recommended/48-morse-code-words.cpp` | 804 | Unique Morse Code Words | A-rec |
| 12 | 2 | `company/amazon/ref/recommended/49-first-unique-char.cpp` | 387 | First Unique Character in a String | A-rec · A-list #165 |
| 13 | 2 | `company/amazon/ref/recommended/50-one-package-all-products.cpp` | — | One package that fits every product (per-dimension max) | A-rec |
| 14 | 2 | `company/amazon/ref/recommended/54-loyal-customers.cpp` | — | Seen both days with ≥ 2 distinct products (sets) | A-rec |
| 15 | 2 | `company/amazon/ref/recommended/65-last-seen-anagram.cpp` | — | Newest earlier anagram per word (letter-count key) | A-rec |
| 16 | 2 | `topic/arrays-hashing/242_valid-anagram.cpp` | 242 | Valid Anagram | A-rec · G-30d #21 · A-list #41 |
| 17 | 2 | `company/apple/ref/recommended/18-valid-sudoku.cpp` · also `topic/arrays-hashing/36_valid-sudoku.cpp` | 36 | Valid Sudoku | A-rec · A-list #29 |
| 18 | 2 | `company/apple/ref/recommended/41-longest-consecutive.cpp` · also `topic/arrays-hashing/128_longest-consecutive.cpp` | 128 | Longest Consecutive Sequence | A-rec · G-30d #5 · A-list #30 |
| 19 | 2 | `specialty/238_product-except-self.cpp` · also `topic/arrays-hashing/238_product-except-self.cpp` | 238 | Product of Array Except Self | A-rec · A-list #27 |
| 20 | 2 | `specialty/169_majority-element.cpp` · also `topic/arrays-hashing/169_majority-element.cpp` | 169 | Majority Element | A-rec · G-30d #18 · A-list #34 |
| 21 | 2 | `topic/arrays-hashing/303_range-sum-query.cpp` | 303 | Range Sum Query - Immutable | A-rec · A-list #100 |
| 22 | 2 | `company/apple/ref/recommended/16-longest-common-prefix.cpp` | 14 | Longest Common Prefix | A-rec · G-30d #32 · A-list #19 |
| 23 | 2 | `topic/arrays-hashing/217_contains-duplicate.cpp` | 217 | Contains Duplicate | A-rec · G-30d #78 · A-list #80 |
| 24 | 3 | — | 1929 | Concatenation of Array | G-30d #11 · A-list #58 |
| 25 | 3 | `specialty/287_find-duplicate.cpp` · also `topic/arrays-hashing/287_find-duplicate.cpp` | 287 | Find the Duplicate Number | G-30d #46 · A-list #123 |
| 26 | 3 | — | 383 | Ransom Note | G-30d #47 · A-list #233 |
| 27 | 3 | — | 2089 | Find Target Indices After Sorting Array | G-30d #55 |
| 28 | 3 | — | 2913 | Subarrays Distinct Element Sum of Squares I | G-30d #58 |
| 29 | 3 | `company/amazon/ref/optional/95-intersection-arrays.cpp` | 349 | Intersection of Two Arrays | G-30d #86 · A-opt · A-list #164 |
| 30 | 4 | `company/amazon/ref/optional/96-jewels-stones.cpp` | 771 | Jewels and Stones | A-opt |
| 31 | 4 | `company/amazon/ref/optional/100-students-lunch.cpp` | 1700 | Number of Students Unable to Eat Lunch | A-opt |
| 32 | 5 | `company/apple/ref/recommended/27-insert-delete-getrandom.cpp` · also `topic/arrays-hashing/380_insert-delete-getrandom.cpp` | 380 | Insert Delete GetRandom O(1) | A-list #101 |
| 33 | 5 | — | 525 | Contiguous Array | A-list #103 |
| 34 | 5 | — | 205 | Isomorphic Strings | A-list #119 |
| 35 | 5 | — | 724 | Find Pivot Index | A-list #130 |
| 36 | 5 | `specialty/41_first-missing-positive.cpp` · also `topic/arrays-hashing/41_first-missing-positive.cpp` | 41 | First Missing Positive | A-list #141 |
| 37 | 5 | — | 229 | Majority Element II | A-list #160 |
| 38 | 5 | `specialty/528_random-pick-weight.cpp` · also `topic/arrays-hashing/528_random-pick-weight.cpp` | 528 | Random Pick with Weight | A-list #179 |
| 39 | 5 | `topic/arrays-hashing/645_set-mismatch.cpp` | 645 | Set Mismatch | A-list #184 |
| 40 | 5 | — | 1752 | Check if Array Is Sorted and Rotated | A-list #197 |

### A2. Two Pointers (23)

| # | Tier | File | LC | Problem | Source |
|---|---|---|---|---|---|
| 41 | 1 | `company/amazon/ref/must/06-merge-sorted-arrays-variants.cpp` · also `topic/two-pointers/88_merge-sorted-array.cpp` | 88 | Merge Sorted Array | A-must · G-30d #68 · A-list #51 |
| 42 | 1 | `company/snowflake/ref/recommended/56-trapping-rain-water.cpp` · also `topic/two-pointers/42_trapping-rain-water.cpp` | 42 | Trapping Rain Water | A-must · G-30d #64 · A-list #4 |
| 43 | 2 | `company/amazon/ref/recommended/39-in-place-sort.cpp` | — | Insertion sort vs quicksort | A-rec |
| 44 | 2 | `specialty/11_container-with-most-water.cpp` · also `topic/two-pointers/11_container-with-most-water.cpp` | 11 | Container With Most Water | A-rec · G-30d #31 · A-list #10 |
| 45 | 2 | `company/apple/ref/recommended/30-three-sum.cpp` · also `topic/two-pointers/15_three-sum.cpp` | 15 | 3Sum | A-rec · G-30d #8 · A-list #16 |
| 46 | 2 | `topic/two-pointers/18_four-sum.cpp` | 18 | 4Sum | A-rec · A-list #70 |
| 47 | 2 | `company/apple/ref/recommended/37-move-zeroes.cpp` | 283 | Move Zeroes | A-rec · G-30d #85 · A-list #12 |
| 48 | 3 | — | 2337 | Move Pieces to Obtain a String | G-30d #27 |
| 49 | 3 | `topic/two-pointers/ref/912_sort-an-array.cpp` | 912 | Sort an Array | G-30d #99 |
| 50 | 3 | — | 1768 | Merge Strings Alternately | G-30d #109 |
| 51 | 3 | — | 1793 | Maximum Score of a Good Subarray | G-30d #110 |
| 52 | 4 | — | 26 | Remove Duplicates from Sorted Array | A-list #38 |
| 53 | 4 | `specialty/31_next-permutation.cpp` · also `topic/two-pointers/31_next-permutation.cpp` | 31 | Next Permutation | A-list #45 |
| 54 | 4 | `company/amazon/oa/05-min-adjacent-swaps-binary.cpp` | 2938 | Separate Black and White Balls | A-list #59 |
| 55 | 4 | `company/amazon/ref/optional/94-reverse-string.cpp` | 344 | Reverse String | A-opt · A-list #125 |
| 56 | 4 | `company/amazon/ref/optional/97-sort-parity-ii.cpp` | 922 | Sort Array By Parity II | A-opt |
| 57 | 5 | `topic/two-pointers/167_two-sum-ii.cpp` | 167 | Two Sum II - Input Array Is Sorted | A-list #97 |
| 58 | 5 | — | 27 | Remove Element | A-list #109 |
| 59 | 5 | `company/apple/ref/must/15-valid-palindrome.cpp` · also `topic/two-pointers/125_valid-palindrome.cpp` | 125 | Valid Palindrome | A-list #114 |
| 60 | 5 | `company/apple/ref/optional/42-rotate-array.cpp` | 189 | Rotate Array | A-list #118 |
| 61 | 5 | `topic/two-pointers/16_three-sum-closest.cpp` | 16 | 3Sum Closest | A-list #140 |
| 62 | 5 | `specialty/75_sort-colors.cpp` · also `topic/two-pointers/75_sort-colors.cpp` | 75 | Sort Colors | A-list #144 |
| 63 | 5 | `topic/two-pointers/80_remove-duplicates-ii.cpp` | 80 | Remove Duplicates from Sorted Array II | A-list #146 |

### A3. Sliding Window (27)

| # | Tier | File | LC | Problem | Source |
|---|---|---|---|---|---|
| 64 | 1 | `company/amazon/ref/must/01-max-coins-k-bags.cpp` | 3413 | Maximum Coins From K Consecutive Bags | A-must · A-list #2 |
| 65 | 1 | `company/amazon/ref/must/05-sliding-window-max-ds.cpp` · also `topic/sliding-window/239_sliding-window-max.cpp` | 239 | Sliding Window Maximum | A-must · G-30d #44 · A-list #47 |
| 66 | 1 | `company/bytedance/ref/must/12-longest-substring-no-repeat.cpp` · also `topic/sliding-window/3_longest-substring-no-repeat.cpp` | 3 | Longest Substring Without Repeating Characters | A-must · A-rec · G-30d #12 · A-list #3 |
| 67 | 2 | `company/amazon/ref/recommended/18-array-continuous-min-ops.cpp` | 2009 | Minimum Number of Operations to Make Array Continuous | A-rec |
| 68 | 2 | `company/amazon/ref/recommended/57-longest-subarray-k-distinct.cpp` | 340 | Longest Substring with At Most K Distinct Characters | A-rec |
| 69 | 2 | `topic/sliding-window/ref/1004_max-consecutive-ones-iii.cpp` | 1004 | Max Consecutive Ones III | A-rec · A-list #57 |
| 70 | 2 | `topic/sliding-window/219_contains-duplicate-ii.cpp` | 219 | Contains Duplicate II | A-rec · G-30d #79 · A-list #158 |
| 71 | 2 | `company/bytedance/ref/recommended/21-longest-repeating-char-replacement.cpp` · also `topic/sliding-window/424_longest-repeating-char-replace.cpp` | 424 | Longest Repeating Character Replacement | A-rec |
| 72 | 2 | `topic/sliding-window/ref/904_fruit-into-baskets.cpp` | 904 | Fruit Into Baskets | A-rec · G-30d #53 · A-list #43 |
| 73 | 3 | `company/apple/ref/recommended/19-best-time-stock.cpp` · also `topic/sliding-window/121_best-time-stock.cpp` | 121 | Best Time to Buy and Sell Stock | G-30d #17 · A-list #7 · A-opt |
| 74 | 3 | — | 485 | Max Consecutive Ones | G-30d #49 |
| 75 | 3 | `company/apple/ref/recommended/32-subarray-sum-k.cpp` · also `topic/sliding-window/560_subarray-sum-k.cpp` | 560 | Subarray Sum Equals K | G-30d #50 · A-list #18 · A-opt |
| 76 | 3 | `topic/sliding-window/ref/220_contains-duplicate-iii.cpp` | 220 | Contains Duplicate III | G-30d #80 |
| 77 | 4 | `topic/sliding-window/76_min-window-substring.cpp` | 76 | Minimum Window Substring | A-list #62 |
| 78 | 4 | — | 643 | Maximum Average Subarray I | A-list #67 |
| 79 | 4 | `company/amazon/ref/optional/68-max-consecutive-on-servers.cpp` | — | ≤ k range flips, longest run of 1s | A-opt |
| 80 | 4 | `company/amazon/ref/optional/78-max-white-tiles.cpp` | 2271 | Maximum White Tiles Covered by a Carpet | A-opt · A-list #105 |
| 81 | 5 | `topic/sliding-window/567_permutation-in-string.cpp` | 567 | Permutation in String | A-list #128 |
| 82 | 5 | — | 1248 | Count Number of Nice Subarrays | A-list #136 |
| 83 | 5 | `company/snowflake/ref/optional/31-find-anagrams.cpp` · also `topic/sliding-window/438_find-anagrams.cpp` | 438 | Find All Anagrams in a String | A-list #170 |
| 84 | 5 | `topic/sliding-window/523_continuous-subarray-sum.cpp` | 523 | Continuous Subarray Sum | A-list #178 |
| 85 | 5 | `company/apple/ref/recommended/26-subarray-product-less-k.cpp` | 713 | Subarray Product Less Than K | A-list #185 |
| 86 | 5 | — | 1423 | Maximum Points You Can Obtain from Cards | A-list #193 |
| 87 | 5 | — | 1838 | Frequency of the Most Frequent Element | A-list #199 |
| 88 | 5 | — | 2134 | Minimum Swaps to Group All 1's Together II | A-list #201 |
| 89 | 5 | — | 187 | Repeated DNA Sequences | A-list #223 |
| 90 | 5 | `topic/sliding-window/209_min-size-subarray-sum.cpp` | 209 | Minimum Size Subarray Sum | A-list #225 |

### A4. Stack & Monotonic Stack (20)

| # | Tier | File | LC | Problem | Source |
|---|---|---|---|---|---|
| 91 | 1 | `company/snowflake/ref/optional/30-valid-parentheses.cpp` · also `topic/stack/20_valid-parentheses.cpp` | 20 | Valid Parentheses | A-must · G-30d #60 · A-list #24 |
| 92 | 2 | `company/amazon/ref/recommended/58-stock-span.cpp` | 901 | Online Stock Span | A-rec |
| 93 | 2 | `company/amazon/ref/recommended/59-max-chunks-sorted-ii.cpp` | 768 | Max Chunks To Make Sorted II | A-rec |
| 94 | 2 | `topic/stack/22_generate-parentheses.cpp` | 22 | Generate Parentheses | A-rec · G-30d #61 · A-list #25 |
| 95 | 2 | `specialty/32_longest-valid-parentheses.cpp` | 32 | Longest Valid Parentheses | A-rec · A-list #73 |
| 96 | 2 | `specialty/316_remove-duplicate-letters.cpp` · also `topic/stack/316_remove-duplicate-letters.cpp` | 316 | Remove Duplicate Letters | A-rec · A-list #163 |
| 97 | 2 | `topic/stack/ref/402_remove-k-digits.cpp` | 402 | Remove K Digits | A-rec · G-30d #24 · A-list #56 |
| 98 | 2 | `topic/stack/ref/394_decode-string.cpp` | 394 | Decode String | A-rec |
| 99 | 2 | `topic/stack/ref/735_asteroid-collision.cpp` | 735 | Asteroid Collision | A-rec · A-list #42 |
| 100 | 3 | `specialty/84_largest-rectangle.cpp` · also `topic/stack/84_largest-rectangle.cpp` | 84 | Largest Rectangle in Histogram | G-30d #40 · A-list #92 |
| 101 | 3 | `topic/stack/ref/496_next-greater-i.cpp` | 496 | Next Greater Element I | G-30d #89 · A-list #127 |
| 102 | 3 | `topic/stack/ref/739_daily-temperatures.cpp` | 739 | Daily Temperatures | G-30d #93 · A-list #69 |
| 103 | 3 | — | 1541 | Minimum Insertions to Balance a Parentheses String | G-30d #108 |
| 104 | 4 | `company/amazon/ref/optional/98-remove-outer-parens.cpp` | 1021 | Remove Outermost Parentheses | A-opt · A-list #190 |
| 105 | 5 | `topic/stack/ref/503_next-greater-ii.cpp` | 503 | Next Greater Element II | A-list #82 |
| 106 | 5 | — | 1111 | Maximum Nesting Depth of Two Valid Parentheses Strings | A-list #83 |
| 107 | 5 | `topic/stack/ref/155_min-stack.cpp` | 155 | Min Stack | A-list #116 |
| 108 | 5 | `topic/stack/ref/456_pattern-132.cpp` | 456 | 132 Pattern | A-list #174 |
| 109 | 5 | `topic/stack/85_maximal-rectangle.cpp` | 85 | Maximal Rectangle | A-list #212 |
| 110 | 5 | `topic/stack/150_eval-rpn.cpp` | 150 | Evaluate Reverse Polish Notation | A-list #220 |

### A5. Binary Search (19)

| # | Tier | File | LC | Problem | Source |
|---|---|---|---|---|---|
| 111 | 2 | `topic/binary-search/ref/4_median-two-sorted-arrays.cpp` | 4 | Median of Two Sorted Arrays | A-rec · G-30d #3 · A-list #8 |
| 112 | 2 | `topic/binary-search/ref/704_binary-search.cpp` | 704 | Binary Search | A-rec · A-list #68 |
| 113 | 2 | `topic/binary-search/ref/875_koko-bananas.cpp` | 875 | Koko Eating Bananas | A-rec · G-30d #52 · A-list #14 |
| 114 | 2 | `topic/binary-search/ref/540_single-element-sorted.cpp` | 540 | Single Element in a Sorted Array | A-rec · A-list #49 |
| 115 | 2 | `topic/binary-search/ref/33_search-rotated-array.cpp` | 33 | Search in Rotated Sorted Array | A-rec · G-30d #62 · A-list #32 |
| 116 | 2 | `topic/binary-search/ref/74_search-2d-matrix.cpp` | 74 | Search a 2D Matrix | A-rec |
| 117 | 2 | `topic/binary-search/ref/410_split-array-largest-sum.cpp` | 410 | Split Array Largest Sum | A-rec · G-30d #25 · A-list #48 |
| 118 | 3 | `company/amazon/ref/optional/91-search-insert.cpp` | 35 | Search Insert Position | G-30d #34 · A-opt · A-list #74 |
| 119 | 3 | `company/amazon/ref/optional/93-sqrt-x.cpp` | 69 | Sqrt(x) | G-30d #38 · A-list #33 · A-opt |
| 120 | 3 | — | 278 | First Bad Version | G-30d #45 |
| 121 | 3 | `topic/binary-search/ref/34_first-last-position.cpp` | 34 | Find First and Last Position of Element in Sorted Array | G-30d #63 · A-list #61 |
| 122 | 3 | `topic/binary-search/ref/162_find-peak.cpp` | 162 | Find Peak Element | G-30d #72 |
| 123 | 4 | — | 1011 | Capacity To Ship Packages Within D Days | A-list #36 |
| 124 | 4 | `company/amazon/ref/optional/79-peak-element-ii.cpp` | 1901 | Find a Peak Element II | A-opt |
| 125 | 4 | `company/amazon/ref/optional/81-search-2d-matrix-ii.cpp` | 240 | Search a 2D Matrix II | A-opt · A-list #81 |
| 126 | 4 | `company/amazon/ref/optional/99-kth-missing-positive.cpp` | 1539 | Kth Missing Positive Number | A-opt |
| 127 | 5 | `topic/binary-search/ref/153_rotated-array-min.cpp` | 153 | Find Minimum in Rotated Sorted Array | A-list #96 |
| 128 | 5 | — | 81 | Search in Rotated Sorted Array II | A-list #147 |
| 129 | 5 | — | 1552 | Magnetic Force Between Two Balls | A-list #195 |

### A6. Linked List (17)

| # | Tier | File | LC | Problem | Source |
|---|---|---|---|---|---|
| 130 | 1 | `topic/linked-list/ref/146_lru-cache.cpp` | 146 | LRU Cache | A-must · A-list #5 |
| 131 | 1 | `topic/linked-list/ref/23_merge-k-lists.cpp` | 23 | Merge k Sorted Lists | A-must · A-rec · A-list #37 |
| 132 | 2 | `company/amazon/ref/recommended/12-linked-list-queue.cpp` | — | Linked-list queue (+ delete, dedupe) | A-rec |
| 133 | 2 | `topic/linked-list/ref/138_copy-random-list.cpp` | 138 | Copy List with Random Pointer | A-rec · A-list #39 |
| 134 | 2 | `topic/linked-list/ref/142_list-cycle-ii.cpp` | 142 | Linked List Cycle II | A-rec · A-list #218 |
| 135 | 2 | `topic/linked-list/ref/2_add-two-numbers.cpp` | 2 | Add Two Numbers | A-rec · G-30d #2 · A-list #15 |
| 136 | 2 | `topic/linked-list/ref/141_list-cycle.cpp` | 141 | Linked List Cycle | A-rec · A-list #40 |
| 137 | 2 | `topic/linked-list/ref/19_remove-nth-from-end.cpp` | 19 | Remove Nth Node From End of List | A-rec · A-list #206 |
| 138 | 2 | `topic/linked-list/ref/21_merge-two-lists.cpp` | 21 | Merge Two Sorted Lists | A-rec · G-30d #33 · A-list #71 |
| 139 | 2 | `topic/linked-list/ref/206_reverse-list.cpp` | 206 | Reverse Linked List | A-rec · A-list #120 |
| 140 | 3 | — | 876 | Middle of the Linked List | G-30d #98 · A-list #134 |
| 141 | 5 | `topic/linked-list/ref/25_reverse-k-group.cpp` | 25 | Reverse Nodes in k-Group | A-list #87 |
| 142 | 5 | `topic/linked-list/ref/61_rotate-list.cpp` | 61 | Rotate List | A-list #112 |
| 143 | 5 | `topic/linked-list/ref/143_reorder-list.cpp` | 143 | Reorder List | A-list #115 |
| 144 | 5 | `topic/linked-list/ref/328_odd-even-list.cpp` | 328 | Odd Even Linked List | A-list #124 |
| 145 | 5 | `topic/linked-list/ref/92_reverse-list-ii.cpp` | 92 | Reverse Linked List II | A-list #148 |
| 146 | 5 | `topic/linked-list/ref/234_palindrome-list.cpp` | 234 | Palindrome Linked List | A-list #161 |

### A7. Trees (29)

| # | Tier | File | LC | Problem | Source |
|---|---|---|---|---|---|
| 147 | 1 | `company/amazon/ref/must/09-node-relationship.cpp` | — | Siblings / cousins / others | A-must |
| 148 | 2 | `company/amazon/ref/recommended/14-hierarchical-comments.cpp` | — | Print a comment tree with indentation | A-rec |
| 149 | 2 | `company/amazon/ref/recommended/16-tree-from-descriptions.cpp` | 2196 | Create Binary Tree From Descriptions | A-rec |
| 150 | 2 | `company/amazon/ref/recommended/26-lca-stack.cpp` · also `topic/trees/ref/236_lca-binary-tree.cpp` | 236 | Lowest Common Ancestor of a Binary Tree | A-rec · A-list #21 |
| 151 | 2 | `company/amazon/ref/recommended/27-cousins-in-binary-tree-ii.cpp` | 2641 | Cousins in Binary Tree II | A-rec |
| 152 | 2 | `company/amazon/ref/recommended/29-lca-inserted-bst.cpp` · also `topic/trees/ref/235_lca-bst.cpp` | 235 | Lowest Common Ancestor of a BST | A-rec |
| 153 | 2 | `company/amazon/ref/recommended/60-nary-tree-max-depth.cpp` | 559 | Maximum Depth of N-ary Tree | A-rec |
| 154 | 2 | `company/amazon/ref/recommended/62-lca-parent-pointers.cpp` | 1650 | Lowest Common Ancestor of a Binary Tree III | A-rec |
| 155 | 2 | `company/amazon/ref/recommended/63-vertical-order-traversal.cpp` | 987 | Vertical Order Traversal of a Binary Tree | A-rec |
| 156 | 2 | `company/bytedance/ref/optional/115-infected-tree-time.cpp` | 2385 | Amount of Time for Binary Tree to Be Infected | A-rec |
| 157 | 2 | `company/bytedance/ref/must/11-path-sum-ii.cpp` | 113 | Path Sum II | A-rec · A-list #215 |
| 158 | 2 | `topic/trees/ref/863_nodes-distance-k.cpp` | 863 | All Nodes Distance K in Binary Tree | A-rec · A-list #133 |
| 159 | 3 | — | 100 | Same Tree | G-30d #41 · A-list #149 |
| 160 | 3 | — | 112 | Path Sum | G-30d #69 |
| 161 | 3 | `topic/trees/ref/114_flatten-tree-to-list.cpp` | 114 | Flatten Binary Tree to Linked List | G-30d #70 |
| 162 | 3 | `topic/trees/ref/199_right-side-view.cpp` | 199 | Binary Tree Right Side View | G-30d #74 · A-list #155 |
| 163 | 3 | — | 655 | Print Binary Tree | G-30d #91 |
| 164 | 4 | `topic/trees/ref/124_max-path-sum.cpp` | 124 | Binary Tree Maximum Path Sum | A-list #52 |
| 165 | 5 | `topic/trees/ref/98_validate-bst.cpp` | 98 | Validate Binary Search Tree | A-list #76 |
| 166 | 5 | `topic/trees/ref/102_level-order.cpp` | 102 | Binary Tree Level Order Traversal | A-list #150 |
| 167 | 5 | `topic/trees/ref/108_sorted-array-to-bst.cpp` | 108 | Convert Sorted Array to Binary Search Tree | A-list #151 |
| 168 | 5 | — | 110 | Balanced Binary Tree | A-list #152 |
| 169 | 5 | `topic/trees/ref/450_delete-node-bst.cpp` | 450 | Delete Node in a BST | A-list #172 |
| 170 | 5 | `topic/trees/ref/543_diameter-of-tree.cpp` | 543 | Diameter of Binary Tree | A-list #180 |
| 171 | 5 | — | 101 | Symmetric Tree | A-list #213 |
| 172 | 5 | `topic/trees/ref/104_max-depth.cpp` | 104 | Maximum Depth of Binary Tree | A-list #214 |
| 173 | 5 | — | 144 | Binary Tree Preorder Traversal | A-list #219 |
| 174 | 5 | `topic/trees/ref/297_serialize-tree.cpp` | 297 | Serialize and Deserialize Binary Tree | A-list #229 |
| 175 | 5 | `topic/trees/ref/437_path-sum-iii.cpp` | 437 | Path Sum III | A-list #235 |

### A8. Trie & Heap (13)

| # | Tier | File | LC | Problem | Source |
|---|---|---|---|---|---|
| 176 | 1 | `company/amazon/ref/must/07-streaming-top-k.cpp` | — | Online add(v) / top() class, ties by smaller value (ordered set) | A-must |
| 177 | 1 | `topic/trie-heap/ref/767_reorganize-string.cpp` | 767 | Reorganize String | A-must · A-list #22 |
| 178 | 1 | `topic/trie-heap/ref/295_median-stream.cpp` | 295 | Find Median from Data Stream | A-must · A-list #55 |
| 179 | 1 | `topic/trie-heap/ref/212_word-search-ii.cpp` | 212 | Word Search II | A-must · A-list #157 |
| 180 | 2 | `company/amazon/ref/recommended/30-top-k-slowest-logs.cpp` | — | Top-k durations, stable ties (heap) | A-rec |
| 181 | 2 | `company/amazon/ref/recommended/53-assign-partitions-least-loaded.cpp` | — | Each partition to the least-loaded server (min-heap) | A-rec |
| 182 | 2 | `topic/trie-heap/ref/621_task-scheduler.cpp` | 621 | Task Scheduler | A-rec · A-list #35 |
| 183 | 3 | `topic/trie-heap/ref/215_kth-largest.cpp` | 215 | Kth Largest Element in an Array | G-30d #77 · A-list #65 |
| 184 | 3 | `topic/trie-heap/ref/1046_last-stone-weight.cpp` | 1046 | Last Stone Weight | G-30d #101 |
| 185 | 3 | — | 1405 | Longest Happy String | G-30d #106 |
| 186 | 4 | `company/amazon/ref/optional/71-detect-abusive-words.cpp` | — | Obfuscated word matching (trie) | A-opt |
| 187 | 5 | `topic/trie-heap/ref/973_k-closest-points.cpp` | 973 | K Closest Points to Origin | A-list #104 |
| 188 | 5 | — | 451 | Sort Characters By Frequency | A-list #173 |

### A9. Backtracking (12)

| # | Tier | File | LC | Problem | Source |
|---|---|---|---|---|---|
| 189 | 2 | `company/amazon/ref/recommended/21-permutations-circular.cpp` | — | All line orders (+ circular, (n-1)!) | A-rec |
| 190 | 2 | `topic/backtracking/ref/46_permutations.cpp` | 46 | Permutations | A-rec · A-list #89 |
| 191 | 2 | `topic/backtracking/ref/51_n-queens.cpp` | 51 | N-Queens | A-rec · G-30d #65 · A-list #90 |
| 192 | 2 | `company/google/ref/must/02-perfect-wave-number.cpp` | — | Perfect wave numbers: zigzag digits, count in [1, N] | G-report |
| 193 | 3 | `topic/backtracking/ref/78_subsets.cpp` | 78 | Subsets | G-30d #39 · A-list #50 |
| 194 | 4 | `company/amazon/ref/optional/73-block-string-assembly.cpp` | — | Spell target from distinct blocks (matching) | A-opt |
| 195 | 4 | `company/amazon/ref/optional/75-string-reorganization-all.cpp` | — | All rearrangements with no equal neighbors | A-opt |
| 196 | 5 | `topic/backtracking/ref/17_phone-letters.cpp` | 17 | Letter Combinations of a Phone Number | A-list #108 |
| 197 | 5 | — | 77 | Combinations | A-list #145 |
| 198 | 5 | `topic/backtracking/ref/131_palindrome-partition.cpp` | 131 | Palindrome Partitioning | A-list #153 |
| 199 | 5 | `topic/backtracking/ref/79_word-search.cpp` | 79 | Word Search | A-list #211 |
| 200 | 5 | — | 216 | Combination Sum III | A-list #226 |

### A10. Graphs (30)

| # | Tier | File | LC | Problem | Source |
|---|---|---|---|---|---|
| 201 | 1 | `company/amazon/ref/must/08-package-dependency-order.cpp` | — | Install order for one target, [] on a cycle (post-order DFS) | A-must |
| 202 | 1 | `topic/graphs/ref/994_rotting-oranges.cpp` | 994 | Rotting Oranges | A-must · A-list #23 |
| 203 | 1 | `topic/graphs/ref/200_number-of-islands.cpp` | 200 | Number of Islands | A-must · G-30d #20 · A-list #20 |
| 204 | 1 | `topic/graphs/ref/695_max-area-island.cpp` | 695 | Max Area of Island | A-must |
| 205 | 2 | `company/amazon/ref/recommended/17-complete-components.cpp` | 2685 | Count the Number of Complete Components | A-rec |
| 206 | 2 | `company/amazon/ref/recommended/24-page-referrer-reachability.cpp` | — | Target reachable from start via referrer edges (BFS) | A-rec |
| 207 | 2 | `company/amazon/ref/recommended/25-dag-completion-time.cpp` · also `topic/graphs/ref/2050_parallel-courses-iii.cpp` | 2050 | Parallel Courses III | A-rec |
| 208 | 2 | `company/amazon/ref/recommended/32-service-shutdown-impact.cpp` | — | Affected services in a dependency DAG (BFS) | A-rec |
| 209 | 2 | `company/amazon/ref/recommended/45-package-installer.cpp` | — | Install with dependencies, cycle check, payload | A-rec |
| 210 | 2 | `company/amazon/ref/recommended/52-active-graph-connectivity.cpp` | — | Start's component + edges that connect the rest | A-rec |
| 211 | 2 | `company/amazon/ref/recommended/55-largest-stepping-number.cpp` | — | Largest number ≤ limit with adjacent digits ±1 (BFS) | A-rec |
| 212 | 2 | `company/amazon/ref/recommended/56-open-the-lock.cpp` | 752 | Open the Lock | A-rec |
| 213 | 2 | `topic/graphs/ref/127_word-ladder.cpp` | 127 | Word Ladder | A-rec · A-list #64 |
| 214 | 2 | `topic/conditional/ref/277_find-celebrity.cpp` | 277 | Find the Celebrity | A-rec |
| 215 | 2 | `topic/graphs/ref/210_course-schedule-ii.cpp` | 210 | Course Schedule II | A-rec · G-30d #43 · A-list #79 |
| 216 | 2 | `topic/graphs/ref/207_course-schedule.cpp` | 207 | Course Schedule | A-rec · G-30d #76 · A-list #26 |
| 217 | 2 | `topic/graphs/ref/133_clone-graph.cpp` | 133 | Clone Graph | A-rec |
| 218 | 2 | `topic/graphs/ref/323_connected-components.cpp` | 323 | Number of Connected Components in an Undirected Graph | A-rec |
| 219 | 2 | `topic/graphs/ref/417_pacific-atlantic.cpp` | 417 | Pacific Atlantic Water Flow | A-rec · A-list #168 |
| 220 | 3 | `topic/graphs/ref/261_graph-valid-tree.cpp` | 261 | Graph Valid Tree | G-30d #84 |
| 221 | 3 | — | 419 | Battleships in a Board | G-30d #88 |
| 222 | 3 | `topic/graphs/ref/765_couples-holding-hands.cpp` | 765 | Couples Holding Hands | G-30d #95 |
| 223 | 4 | `company/amazon/ref/optional/72-connect-points-2d.cpp` | — | Points connected by row/column moves (BFS) | A-opt |
| 224 | 4 | `company/amazon/ref/optional/80-closed-islands.cpp` | 1254 | Number of Closed Islands | A-opt |
| 225 | 5 | `topic/conditional/ref/680_valid-palindrome-ii.cpp` | 680 | Valid Palindrome II | A-list #129 |
| 226 | 5 | `topic/graphs/ref/815_bus-routes.cpp` | 815 | Bus Routes | A-list #132 |
| 227 | 5 | `topic/graphs/ref/547_provinces.cpp` | 547 | Number of Provinces | A-list #181 |
| 228 | 5 | — | 733 | Flood Fill | A-list #186 |
| 229 | 5 | `topic/graphs/ref/1094_car-pooling.cpp` | 1094 | Car Pooling | A-list #191 |
| 230 | 5 | `topic/graphs/ref/269_alien-dictionary.cpp` | 269 | Alien Dictionary | A-list #228 |

### A11. Advanced Graphs (9)

| # | Tier | File | LC | Problem | Source |
|---|---|---|---|---|---|
| 231 | 1 | `company/amazon/ref/must/02-currency-conversion.cpp` | — | Multi-hop exchange rate with reverse edges, 2 decimals (BFS) | A-must |
| 232 | 2 | `company/amazon/ref/recommended/22-islands-streaming.cpp` | — | Island count with rows streamed (union-find) | A-rec |
| 233 | 2 | `company/amazon/ref/recommended/46-delivery-route-planning.cpp` | — | Pickups before drop-offs, capacity (BFS + DP) | A-rec |
| 234 | 2 | `topic/advanced-graphs/ref/399_evaluate-division.cpp` | 399 | Evaluate Division | A-rec · A-list #102 |
| 235 | 3 | — | 778 | Swim in Rising Water | G-30d #97 |
| 236 | 3 | — | 1514 | Path with Maximum Probability | G-30d #107 |
| 237 | 3 | — | 2812 | Find the Safest Path in a Grid | G-30d #115 |
| 238 | 5 | `topic/advanced-graphs/ref/787_cheapest-flights-k-stops.cpp` | 787 | Cheapest Flights Within K Stops | A-list #131 |
| 239 | 5 | `topic/advanced-graphs/ref/990_equality-equations.cpp` | 990 | Satisfiability of Equality Equations | A-list #189 |

### A12. DP (31)

| # | Tier | File | LC | Problem | Source |
|---|---|---|---|---|---|
| 240 | 1 | `topic/dp/ref/139_word-break.cpp` | 139 | Word Break | A-must · A-rec · A-list #78 |
| 241 | 2 | `company/amazon/ref/recommended/31-stock-variants.cpp` | — | Buy/sell stock: single, unlimited, fee in one function | A-rec |
| 242 | 2 | `company/amazon/ref/recommended/51-max-move-score.cpp` | — | Split into moves / 1-point whiffs, max score (DP) | A-rec |
| 243 | 2 | `company/amazon/ref/recommended/61-pascal-triangle.cpp` | 118 | Pascal's Triangle | A-rec · G-30d #16 · A-list #77 |
| 244 | 2 | `topic/dp/ref/72_edit-distance.cpp` | 72 | Edit Distance | A-rec · G-30d #67 |
| 245 | 2 | `topic/dp/ref/198_house-robber.cpp` | 198 | House Robber | A-rec · A-list #53 |
| 246 | 2 | `topic/dp/ref/64_min-path-sum.cpp` | 64 | Minimum Path Sum | A-rec |
| 247 | 2 | `topic/dp/ref/62_unique-paths.cpp` | 62 | Unique Paths | A-rec · A-list #210 |
| 248 | 2 | `topic/dp/ref/5_longest-palindromic-substring.cpp` | 5 | Longest Palindromic Substring | A-rec · G-30d #7 · A-list #9 |
| 249 | 2 | `topic/dp/ref/140_word-break-ii.cpp` | 140 | Word Break II | A-rec · A-list #217 |
| 250 | 2 | `topic/dp/ref/152_max-product-subarray.cpp` | 152 | Maximum Product Subarray | A-rec · A-list #95 |
| 251 | 2 | `topic/dp/ref/70_climbing-stairs.cpp` | 70 | Climbing Stairs | A-rec · G-30d #66 · A-list #46 |
| 252 | 2 | `topic/dp/ref/122_stock-ii.cpp` | 122 | Best Time to Buy and Sell Stock II | A-rec · A-list #113 |
| 253 | 3 | `topic/dp/ref/416_partition-equal-subset-sum.cpp` | 416 | Partition Equal Subset Sum | G-30d #48 · A-list #167 |
| 254 | 3 | `topic/dp/ref/221_maximal-square.cpp` | 221 | Maximal Square | G-30d #81 |
| 255 | 3 | — | 509 | Fibonacci Number | G-30d #90 |
| 256 | 3 | `topic/dp/ref/1048_longest-string-chain.cpp` | 1048 | Longest String Chain | G-30d #102 |
| 257 | 3 | `topic/dp/ref/1143_longest-common-subseq.cpp` | 1143 | Longest Common Subsequence | G-30d #104 |
| 258 | 3 | — | 1218 | Longest Arithmetic Subsequence of Given Difference | G-30d #105 |
| 259 | 3 | `company/amazon/oa/12-max-alternating-subseq.cpp` | 1911 | Maximum Alternating Subsequence Sum | G-30d #111 |
| 260 | 4 | `company/amazon/ref/optional/82-interleaving-string.cpp` | 97 | Interleaving String | A-list #63 · A-opt |
| 261 | 4 | `topic/dp/ref/322_coin-change.cpp` | 322 | Coin Change | A-list #66 |
| 262 | 4 | `company/amazon/ref/optional/101-bucket-batching.cpp` | — | Partition into G batches, min padding | A-opt |
| 263 | 5 | — | 486 | Predict the Winner | A-list #126 |
| 264 | 5 | — | 1140 | Stone Game II | A-list #192 |
| 265 | 5 | `topic/dp/ref/10_regex-matching.cpp` | 10 | Regular Expression Matching | A-list #205 |
| 266 | 5 | — | 44 | Wildcard Matching | A-list #207 |
| 267 | 5 | — | 115 | Distinct Subsequences | A-list #216 |
| 268 | 5 | `topic/dp/ref/300_longest-increasing-subseq.cpp` | 300 | Longest Increasing Subsequence | A-list #230 |
| 269 | 5 | `topic/dp/ref/312_burst-balloons.cpp` | 312 | Burst Balloons | A-list #231 |
| 270 | 5 | `topic/dp/ref/337_house-robber-iii.cpp` | 337 | House Robber III | A-list #232 |

### A13. Knapsack & Sequences (3)

| # | Tier | File | LC | Problem | Source |
|---|---|---|---|---|---|
| 271 | 1 | `topic/knapsack/ref/474_ones-and-zeroes.cpp` | 474 | Ones and Zeroes | A-must |
| 272 | 2 | `company/amazon/ref/recommended/15-point-payment-min.cpp` | — | Bounded knapsack, min points for exact payment | A-rec |
| 273 | 3 | `topic/knapsack/ref/354_russian-doll-envelopes.cpp` | 354 | Russian Doll Envelopes | G-30d #87 |

### A14. Greedy & Intervals (21)

| # | Tier | File | LC | Problem | Source |
|---|---|---|---|---|---|
| 274 | 1 | `topic/greedy-intervals/ref/56_merge-intervals.cpp` | 56 | Merge Intervals | A-must · G-30d #37 · A-list #11 |
| 275 | 1 | `topic/greedy-intervals/ref/252_meeting-rooms.cpp` | 252 | Meeting Rooms | A-must · A-list #162 |
| 276 | 1 | `topic/greedy-intervals/ref/253_meeting-rooms-ii.cpp` | 253 | Meeting Rooms II | A-must · G-30d #22 · A-list #54 |
| 277 | 2 | `company/amazon/ref/recommended/19-make-change.cpp` | — | Greedy change in cents (+ limited drawer DP) | A-rec |
| 278 | 2 | `company/amazon/ref/recommended/36-package-weights-adjacent-sum.cpp` | — | Order digits so neighbors sum < 10 (+ O(n)) | A-rec |
| 279 | 2 | `topic/greedy-intervals/ref/53_max-subarray.cpp` | 53 | Maximum Subarray | A-rec · G-30d #9 · A-list #17 |
| 280 | 2 | `topic/greedy-intervals/ref/135_candy.cpp` | 135 | Candy | A-rec · A-list #93 |
| 281 | 2 | `company/google/ref/must/01-max-ends-score.cpp` | — | Max ends score: first + last minus the middle (Kadane variant) | G-report |
| 282 | 3 | `topic/greedy-intervals/ref/55_jump-game.cpp` | 55 | Jump Game | G-30d #15 · A-list #143 |
| 283 | 3 | `topic/hard/ref/179_largest-number.cpp` | 179 | Largest Number | G-30d #19 · A-list #117 |
| 284 | 3 | `topic/greedy-intervals/ref/2402_meeting-rooms-iii.cpp` | 2402 | Meeting Rooms III | G-30d #57 |
| 285 | 3 | `topic/greedy-intervals/ref/2333_min-sum-squared-difference.cpp` | 2333 | Minimum Sum of Squared Difference | G-30d #113 |
| 286 | 4 | `company/amazon/ref/optional/67-maximum-interval-overlap.cpp` | — | Max closed intervals sharing a point | A-opt |
| 287 | 4 | `company/amazon/ref/optional/70-find-smallest-hub.cpp` | — | Smallest-capacity free hub | A-opt |
| 288 | 4 | `company/amazon/ref/optional/74-smallest-lex-palindrome.cpp` | — | Smallest palindrome from all characters | A-opt |
| 289 | 4 | `company/amazon/ref/optional/77-min-cost-equal-chars.cpp` | 2712 | Minimum Cost to Make All Characters Equal | A-opt |
| 290 | 5 | `topic/greedy-intervals/ref/45_jump-game-ii.cpp` | 45 | Jump Game II | A-list #110 |
| 291 | 5 | `topic/greedy-intervals/ref/134_gas-station.cpp` | 134 | Gas Station | A-list #154 |
| 292 | 5 | `topic/greedy-intervals/ref/435_non-overlapping-intervals.cpp` | 435 | Non-overlapping Intervals | A-list #169 |
| 293 | 5 | — | 517 | Super Washing Machines | A-list #177 |
| 294 | 5 | `topic/greedy-intervals/ref/57_insert-interval.cpp` | 57 | Insert Interval | A-list #209 |

### A15. Bit / Math / Matrix (20)

| # | Tier | File | LC | Problem | Source |
|---|---|---|---|---|---|
| 295 | 2 | `topic/bit-math-matrix/ref/54_spiral-matrix.cpp` | 54 | Spiral Matrix | A-rec · G-30d #36 · A-list #111 |
| 296 | 2 | `topic/bit-math-matrix/ref/29_divide-two-integers.cpp` | 29 | Divide Two Integers | A-rec · A-list #88 |
| 297 | 2 | `topic/bit-math-matrix/ref/136_single-number.cpp` | 136 | Single Number | A-rec · G-30d #71 · A-list #94 |
| 298 | 3 | — | 9 | Palindrome Number | G-30d #4 · A-list #28 |
| 299 | 3 | `topic/bit-math-matrix/ref/462_min-moves-equal-array-ii.cpp` | 462 | Minimum Moves to Equal Array Elements II | G-30d #10 |
| 300 | 3 | `company/amazon/ref/optional/89-reverse-integer.cpp` | 7 | Reverse Integer | G-30d #29 · A-list #31 · A-opt |
| 301 | 3 | `topic/bit-math-matrix/ref/50_pow.cpp` | 50 | Pow(x, n) | G-30d #35 · A-list #208 |
| 302 | 3 | `company/snowflake/ref/recommended/59-happy-number.cpp` | 202 | Happy Number | G-30d #75 · A-list #156 |
| 303 | 3 | — | 1071 | Greatest Common Divisor of Strings | G-30d #103 |
| 304 | 3 | — | 1922 | Count Good Numbers | G-30d #112 |
| 305 | 5 | `topic/bit-math-matrix/ref/48_rotate-image.cpp` | 48 | Rotate Image | A-list #75 |
| 306 | 5 | — | 2448 | Minimum Cost to Make Array Equal | A-list #138 |
| 307 | 5 | `topic/bit-math-matrix/ref/43_multiply-strings.cpp` | 43 | Multiply Strings | A-list #142 |
| 308 | 5 | — | 412 | Fizz Buzz | A-list #166 |
| 309 | 5 | — | 628 | Maximum Product of Three Numbers | A-list #183 |
| 310 | 5 | — | 836 | Rectangle Overlap | A-list #187 |
| 311 | 5 | — | 2029 | Stone Game IX | A-list #200 |
| 312 | 5 | `topic/bit-math-matrix/ref/204_count-primes.cpp` | 204 | Count Primes | A-list #224 |
| 313 | 5 | — | 231 | Power of Two | A-list #227 |
| 314 | 5 | — | 389 | Find the Difference | A-list #234 |

### A16. String Matching (2)

| # | Tier | File | LC | Problem | Source |
|---|---|---|---|---|---|
| 315 | 3 | `topic/string-matching/ref/1044_longest-duplicate-substring.cpp` | 1044 | Longest Duplicate Substring | G-30d #100 |
| 316 | 5 | `topic/string-matching/ref/28_find-first-occurrence.cpp` | 28 | Find the Index of the First Occurrence in a String | A-list #72 |

### A17. Design (12)

| # | Tier | File | LC | Problem | Source |
|---|---|---|---|---|---|
| 317 | 1 | `company/amazon/ref/must/03-oldest-one-time-visitor.cpp` | 1429 | First Unique Number | A-must · A-list #194 |
| 318 | 1 | `company/amazon/ref/must/04-longest-waiting-unique-task.cpp` | — | First id seen exactly once (+ offline time queries); same family as 03 | A-must |
| 319 | 2 | `company/amazon/ref/recommended/10-freq-stack.cpp` | 895 | Maximum Frequency Stack | A-rec |
| 320 | 2 | `company/amazon/ref/recommended/11-file-search-api.cpp` | — | `find` with composable filters | A-rec |
| 321 | 2 | `company/amazon/ref/recommended/13-robot-charging-monitor.cpp` | — | Assign / release charging stations each tick | A-rec |
| 322 | 2 | `company/amazon/ref/recommended/23-sliding-window-rate-limiter.cpp` | — | 100 requests per user per 60 s (deque per user) | A-rec |
| 323 | 2 | `company/amazon/ref/recommended/33-music-player.cpp` | — | Most frequent unplayed song with cycle reset | A-rec |
| 324 | 2 | `company/amazon/ref/recommended/34-sliding-median-reviews.cpp` | — | Median of the last N ratings | A-rec |
| 325 | 2 | `company/amazon/ref/recommended/66-expiring-key-set.cpp` | — | Add / contains / remove with TTL (map + min-heap) | A-rec |
| 326 | 2 | `topic/design/ref/232_queue-using-stacks.cpp` | 232 | Implement Queue using Stacks | A-rec · G-30d #83 |
| 327 | 3 | `topic/design/ref/715_range-module.cpp` | 715 | Range Module | G-30d #92 |
| 328 | 5 | `topic/design/ref/460_lfu-cache.cpp` | 460 | LFU Cache | A-list #175 |

### A18. Parser / Simulation / String Engineering (16)

| # | Tier | File | LC | Problem | Source |
|---|---|---|---|---|---|
| 329 | 2 | `company/amazon/ref/recommended/20-caesar-cipher.cpp` | — | Caesar shift (+ lookup table) | A-rec |
| 330 | 2 | `company/amazon/ref/recommended/35-log-parser.cpp` | — | Parse logs, filter / count / top-k | A-rec |
| 331 | 2 | `company/amazon/ref/recommended/42-log-field-groups.cpp` | — | Parse `ts\ | A-rec |
| 332 | 2 | `company/amazon/ref/recommended/47-nested-get-path.cpp` | — | `get(obj, "a[0].b")` on nested maps / lists | A-rec |
| 333 | 2 | `company/amazon/ref/recommended/64-csv-ascii-table.cpp` | — | CSV rows as a padded pipe table | A-rec |
| 334 | 2 | `topic/parser/ref/8_atoi.cpp` | 8 | String to Integer (atoi) | A-rec · G-30d #30 · A-list #85 |
| 335 | 2 | `topic/parser/ref/273_integer-to-english.cpp` | 273 | Integer to English Words | A-rec · A-list #122 |
| 336 | 2 | `topic/parser/ref/151_reverse-words.cpp` | 151 | Reverse Words in a String | A-rec |
| 337 | 3 | — | 13 | Roman to Integer | G-30d #13 · A-list #44 |
| 338 | 3 | `topic/hard/ref/6_zigzag-conversion.cpp` | 6 | Zigzag Conversion | G-30d #28 · A-list #107 |
| 339 | 3 | `topic/parser/ref/224_basic-calculator.cpp` | 224 | Basic Calculator | G-30d #82 · A-list #121 |
| 340 | 3 | `topic/parser/ref/772_basic-calculator-iii.cpp` | 772 | Basic Calculator III | G-30d #96 |
| 341 | 4 | `company/amazon/ref/optional/88-integer-to-roman.cpp` | 12 | Integer to Roman | A-opt · A-list #86 |
| 342 | 4 | `company/amazon/ref/optional/92-length-last-word.cpp` | 58 | Length of Last Word | A-opt · A-list #91 |
| 343 | 5 | `topic/parser/ref/227_basic-calculator-ii.cpp` | 227 | Basic Calculator II | A-list #159 |
| 344 | 5 | `topic/parser/ref/443_string-compression.cpp` | 443 | String Compression | A-list #171 |

### A19. Scheduling (1)

| # | Tier | File | LC | Problem | Source |
|---|---|---|---|---|---|
| 345 | 5 | — | 1834 | Single-Threaded CPU | A-list #198 |

### A20. Monotonic Structures / Sweep / Range Queries (5)

| # | Tier | File | LC | Problem | Source |
|---|---|---|---|---|---|
| 346 | 3 | `topic/monotonic-sweep/ref/1944_people-seen-queue.cpp` | 1944 | Number of Visible People in a Queue | G-30d #54 |
| 347 | 3 | `topic/monotonic-sweep/ref/759_employee-free-time.cpp` | 759 | Employee Free Time | G-30d #94 |
| 348 | 3 | `company/amazon/ref/optional/90-distinct-squares-sum.cpp` | 2916 | Subarrays Distinct Element Sum of Squares II | G-30d #116 · A-opt · A-list #139 |
| 349 | 4 | `company/amazon/ref/optional/69-organize-books-to-shelves.cpp` | — | Least-loaded shelf in a prefix (segment tree) | A-opt |
| 350 | 5 | `topic/monotonic-sweep/ref/862_shortest-subarray-sum-k.cpp` | 862 | Shortest Subarray with Sum at Least K | A-list #188 |

### A21. Hard block (1)

| # | Tier | File | LC | Problem | Source |
|---|---|---|---|---|---|
| 351 | 5 | — | 493 | Reverse Pairs | A-list #176 |

## Part B — OA-style (only if an OA is scheduled)

- `company/amazon/oa/` 01–24: the Amazon OA-only reports, in the order of the `oa/` table in
  `company/amazon/README.md`.
- New LeetCode problems (LC 3000+) that appear only on the frequency lists. They are
  contest problems and most likely OA material.

| File | LC | Problem | Source |
|---|---|---|---|
| `company/amazon/ref/optional/76-max-k-sort-permutation.cpp` | 3644 | Maximum K to Sort a Permutation | G-30d #118 · A-opt |
| `company/amazon/ref/optional/87-max-and-increments.cpp` | 3806 | Maximum Bitwise AND After Increment Operations | A-opt · A-list #84 |
| `company/amazon/ref/optional/86-sortable-integers.cpp` | 3886 | Sum of Sortable Integers | G-30d #123 · A-opt |
| `company/amazon/ref/optional/83-longest-subarray-negation.cpp` | 4063 | Divisible by k with one negation | A-opt |
| `company/amazon/ref/optional/84-equal-adjacent-pairs.cpp` | 4066 | One value replacement, max equal neighbors | A-opt |
| `company/amazon/ref/optional/85-meeting-earnings-gaps.cpp` | 4068 | Weighted interval scheduling + gap bonus | A-opt |
| — | 3026 | Maximum Good Subarray Sum | G-30d #117 |
| — | 3414 | Maximum Score of Non-overlapping Intervals | A-list #203 |
| — | 3720 | Lexicographically Smallest Permutation Greater Than Target | A-list #106 |
| — | 3727 | Maximum Alternating Sum of Squares | G-30d #119 |
| — | 3736 | Minimum Moves to Equal Array Elements III | G-30d #120 |
| — | 3749 | Evaluate Valid Expressions | G-30d #121 |
| — | 3863 | Minimum Operations to Sort a String | G-30d #122 |
| — | 3914 | Minimum Operations to Make Array Non Decreasing | A-list #60 |
| — | 3970 | Shortest Path With At Most K Consecutive Identical Characters | G-30d #59 |
| — | 4007 | Widest Possible Fence | A-list #204 |

Left out: SQL 175, 176, 177, 181, 182, 184, 197, 577, 584, 1141, 1148, 1158, 1581, 1757; JavaScript 2620; one-liners 2235, 2413.

## Part C — other companies

Each list keeps its own priority order; problems already in Part A or B, or earlier in
Part C, are left out.

### C1. ByteDance — must (reported 3+ times) (6)

Order and tiers from `company/bytedance/README.md`. Its optional 125–143 (impractical live,
concurrency concepts) and `oa/` are not listed here; skim them last from that README.

| # | File | LC | Problem |
|---|---|---|---|
| 1 | `company/bytedance/ref/must/03-multi-head-attention.cpp` | — | Multi-head attention forward pass |
| 2 | `company/bytedance/ref/must/06-design-hashmap.cpp` | 706 | Design hashmap |
| 3 | `company/bytedance/ref/must/09-alien-dictionary-smallest.cpp` | — | Alien dictionary, lexicographically smallest order |
| 4 | `company/bytedance/ref/must/13-all-paths-directed.cpp` | — | All paths directed |
| 5 | `company/bytedance/ref/must/14-all-cycles-directed.cpp` | — | All cycles directed |
| 6 | `company/bytedance/ref/must/16-dynamic-median-deletions.cpp` | — | Median with add / delete (two heaps, lazy delete) |

### C2. ByteDance — recommended (other phone / onsite reports) (45)

| # | File | LC | Problem |
|---|---|---|---|
| 1 | `company/bytedance/ref/recommended/19-tic-tac-toe.cpp` | 348 | Tic-tac-toe game over + O(1) move class |
| 2 | `company/bytedance/ref/recommended/20-implement-queue.cpp` | — | Implement queue |
| 3 | `company/bytedance/ref/recommended/24-restore-ip-addresses.cpp` | 93 | Restore IP Addresses |
| 4 | `company/bytedance/ref/recommended/25-restore-addresses-k-segments.cpp` | — | Restore addresses with k segments |
| 5 | `company/bytedance/ref/recommended/26-smallest-topological-order.cpp` | — | Lexicographically smallest topological order |
| 6 | `company/bytedance/ref/recommended/28-substructure-of-tree.cpp` | — | Substructure of tree |
| 7 | `company/bytedance/ref/recommended/29-kth-smallest-two-sorted.cpp` | — | Kth smallest of two sorted arrays (+ median of two sorted arrays) |
| 8 | `company/bytedance/ref/recommended/31-construct-pre-post.cpp` | 889 | Construct pre post |
| 9 | `company/bytedance/ref/recommended/32-rate-limiter.cpp` | — | Sliding-window rate limiter; thread-safe = mutex + lock_guard follow-up |
| 10 | `company/bytedance/ref/recommended/33-sjf-process-scheduling.cpp` | — | Shortest-job-first scheduling with a heap |
| 11 | `company/bytedance/ref/recommended/34-heap-priority-queue.cpp` | — | Hand-written min-heap priority queue |
| 12 | `company/bytedance/ref/recommended/36-possible-bipartition.cpp` | 886 | Possible bipartition |
| 13 | `company/bytedance/ref/recommended/38-time-based-kv.cpp` | 981 | Time based key-value store |
| 14 | `company/bytedance/ref/recommended/39-task-scheduler-remove.cpp` | — | Task scheduler with addTask / removeTask / popTask |
| 15 | `company/bytedance/ref/recommended/40-vertical-order-traversal.cpp` | 314 | Binary tree vertical order traversal |
| 16 | `company/bytedance/ref/recommended/41-sparse-matrix-multiply.cpp` | 311 | Sparse matrix multiplication |
| 17 | `company/bytedance/ref/recommended/42-two-distinct-substring.cpp` | 159 | Longest substring with at most two distinct chars |
| 18 | `company/bytedance/ref/recommended/43-interval-list-intersection.cpp` | 986 | Intersect two sorted interval lists |
| 19 | `company/bytedance/ref/recommended/44-add-strings-char-arrays.cpp` | — | Add strings char arrays |
| 20 | `company/bytedance/ref/recommended/50-kth-node-from-end.cpp` | — | Kth node from end |
| 21 | `company/bytedance/ref/recommended/51-max-width-binary-tree.cpp` | 662 | Maximum Width of Binary Tree |
| 22 | `company/bytedance/ref/recommended/53-combination-sum.cpp` | 39 | Combination Sum |
| 23 | `company/bytedance/ref/recommended/54-top-k-kols.cpp` | — | Top k kols |
| 24 | `company/bytedance/ref/recommended/55-add-search-word.cpp` | 211 | Add search word |
| 25 | `company/bytedance/ref/recommended/56-sqrt-precision.cpp` | — | Sqrt precision |
| 26 | `company/bytedance/ref/recommended/61-making-large-island.cpp` | 827 | Largest island after flipping one 0 |
| 27 | `company/bytedance/ref/recommended/62-split-string-equal-ones.cpp` | 1573 | Ways to split a binary string into 3 parts with equal ones |
| 28 | `company/bytedance/ref/recommended/63-leftmost-binary-search.cpp` | — | Leftmost binary search match |
| 29 | `company/bytedance/ref/recommended/64-path-exists-graph.cpp` | 1971 | Path existence in an undirected graph |
| 30 | `company/bytedance/ref/recommended/65-most-frequent-tiebreak.cpp` | — | Most frequent integer, smaller tie-break |
| 31 | `company/bytedance/ref/recommended/66-k-length-substrings-no-repeat.cpp` | 1100 | K length substrings no repeat |
| 32 | `company/bytedance/ref/recommended/68-missing-ranges.cpp` | 163 | Missing ranges |
| 33 | `company/bytedance/ref/recommended/72-valid-parenthesis-string.cpp` | 678 | Valid parenthesis string |
| 34 | `company/bytedance/ref/recommended/74-circular-deque-index.cpp` | — | Circular deque index |
| 35 | `company/bytedance/ref/recommended/75-min-cost-steps.cpp` | — | Min cost steps |
| 36 | `company/bytedance/ref/recommended/76-populate-next-right.cpp` | 116 | Populate next right |
| 37 | `company/bytedance/ref/recommended/78-kth-smallest-subarray-sum.cpp` | 1918 | Kth smallest subarray sum |
| 38 | `company/bytedance/ref/recommended/80-rotated-dup-leftmost.cpp` | — | Rotated array with duplicates, leftmost index |
| 39 | `company/bytedance/ref/recommended/82-all-paths-to-destination.cpp` | 1059 | All paths from source lead to destination |
| 40 | `company/bytedance/ref/recommended/83-shared-ptr.cpp` | — | Implement shared_ptr |
| 41 | `company/bytedance/ref/recommended/86-construct-pre-in.cpp` | 105 | Construct pre in |
| 42 | `company/bytedance/ref/recommended/88-max-candies.cpp` | 2226 | Max candies |
| 43 | `company/bytedance/ref/recommended/89-min-jumps-home.cpp` | 1654 | Min jumps home |
| 44 | `company/bytedance/ref/recommended/90-camel-case-filter.cpp` | — | Camel case filter |
| 45 | `company/bytedance/ref/recommended/91-maze-shortest-path.cpp` | — | BFS shortest path in a 0/1 maze |

### C3. ByteDance — optional 92–124 (LeetCode ByteDance list only) (12)

| # | File | LC | Problem |
|---|---|---|---|
| 1 | `company/bytedance/ref/optional/92-shortest-bridge.cpp` | 934 | Shortest Bridge |
| 2 | `company/bytedance/ref/optional/106-zigzag-level-order.cpp` | 103 | Binary Tree Zigzag Level Order Traversal |
| 3 | `company/bytedance/ref/optional/108-sort-list.cpp` | 148 | Sort List |
| 4 | `company/bytedance/ref/optional/114-accounts-merge.cpp` | 721 | Accounts Merge |
| 5 | `company/bytedance/ref/optional/116-longest-common-prefix-length.cpp` | 3043 | Find the Length of the Longest Common Prefix |
| 6 | `company/bytedance/ref/optional/117-design-memory-allocator.cpp` | 2502 | Design Memory Allocator |
| 7 | `company/bytedance/ref/optional/118-product-last-k.cpp` | 1352 | Product of the Last K Numbers |
| 8 | `company/bytedance/ref/optional/119-text-justification.cpp` | 68 | Text Justification |
| 9 | `company/bytedance/ref/optional/121-shortest-distance-buildings.cpp` | 317 | Shortest Distance from All Buildings |
| 10 | `company/bytedance/ref/optional/122-min-area-rectangle.cpp` | 939 | Min area rectangle |
| 11 | `company/bytedance/ref/optional/123-remove-zero-sum-nodes.cpp` | 1171 | Remove Zero Sum Consecutive Nodes from Linked List |
| 12 | `company/bytedance/ref/optional/124-affine-transforms.cpp` | — | Apply affine array transformations |

### C4. Apple must (NG) (6)

Bit ops first (Silicon Validation role), then the LC list top by frequency, then the hack2hire Apple questions.

| # | File | LC | Problem |
|---|---|---|---|
| 1 | `company/apple/ref/must/02-bit-ops.cpp` | — | Bit ops |
| 2 | `company/apple/ref/must/01-palindrome-score.cpp` | — | Palindrome score |
| 3 | `company/apple/ref/must/03-bipartite.cpp` | 785 | Bipartite |
| 4 | `company/apple/ref/must/04-subset-sum-dp.cpp` | — | Subset sum dp |
| 5 | `company/apple/ref/must/05-subsets-with-sum.cpp` | — | Subsets with sum |
| 6 | `company/apple/ref/must/06-highway-toll.cpp` | — | Highway toll |

### C5. Apple important (2)

Rest of the Apple LC list, already ordered by frequency.

| # | File | LC | Problem |
|---|---|---|---|
| 1 | `company/apple/ref/recommended/22-design-hit-counter.cpp` | 362 | Design hit counter |
| 2 | `company/apple/ref/recommended/28-snapshot-array.cpp` | 1146 | Snapshot array |

### C6. Specialty tricks (12)

One-trick problems; read the `// Trick:` line first, then type from memory. Bit tricks and index tricks matter most for a low-level C/C++ role.

| # | File | LC | Problem |
|---|---|---|---|
| 1 | `specialty/448_find-disappeared-numbers.cpp` | 448 | 448_find disappeared numbers |
| 2 | `specialty/160_intersection-of-lists.cpp` | 160 | 160_intersection of lists |
| 3 | `specialty/974_subarray-sums-div-k.cpp` | 974 | 974_subarray sums div k |
| 4 | `specialty/382_random-list-node.cpp` | 382 | 382_random list node |
| 5 | `specialty/384_shuffle-array.cpp` | 384 | 384_shuffle array |
| 6 | `specialty/470_rand10-using-rand7.cpp` | 470 | 470_rand10 using rand7 |
| 7 | `specialty/137_single-number-ii.cpp` | 137 | 137_single number ii |
| 8 | `specialty/338_counting-bits.cpp` | 338 | 338_counting bits |
| 9 | `specialty/763_partition-labels.cpp` | 763 | 763_partition labels |
| 10 | `specialty/snowcal.cpp` | — | Snowcal |
| 11 | `specialty/271_encode-decode-strings.cpp` | 271 | 271_encode decode strings |
| 12 | `specialty/332_reconstruct-itinerary.cpp` | 332 | 332_reconstruct itinerary |

### C7. Apple optional (12)

Lower-frequency / harder Apple LC problems. Skip `66-sv-constraints.sv` (Design Verification role only).

| # | File | LC | Problem |
|---|---|---|---|
| 1 | `company/apple/ref/optional/46-maximum-swap.cpp` | 670 | Maximum swap |
| 2 | `company/apple/ref/optional/49-lonely-numbers.cpp` | 2150 | Lonely numbers |
| 3 | `company/apple/ref/optional/50-total-hamming-distance.cpp` | 477 | Total hamming distance |
| 4 | `company/apple/ref/optional/52-bst-greater-sum-tree.cpp` | 1038 | Bst greater sum tree |
| 5 | `company/apple/ref/optional/55-last-stone-weight-ii.cpp` | 1049 | Last stone weight ii |
| 6 | `company/apple/ref/optional/56-knight-probability.cpp` | 688 | Knight probability |
| 7 | `company/apple/ref/optional/57-flatten-nested-iterator.cpp` | 341 | Flatten nested iterator |
| 8 | `company/apple/ref/optional/59-design-task-manager.cpp` | 3408 | Design task manager |
| 9 | `company/apple/ref/optional/60-unit-conversion-ii.cpp` | 3535 | Unit conversion ii |
| 10 | `company/apple/ref/optional/61-exam-room.cpp` | 855 | Exam room |
| 11 | `company/apple/ref/optional/63-min-cost-hire-k-workers.cpp` | 857 | Min cost hire k workers |
| 12 | `company/apple/ref/optional/65-parse-lisp-expression.cpp` | 736 | Parse lisp expression |

### C8. Snowflake (only if it moves to another round)

`company/snowflake/prep/` is your own work; the AI references are `company/snowflake/ref/`. Many overlap with the phases above (LRU, merge intervals, course schedule, top-k, calculator, median, task manager).

`company/snowflake/ref/` is numbered `01`–`59` in tier order; its README is the order of attack. `company/snowflake/prep/word-pair-within-k.cpp` and `command-interpreter.cpp` are the two questions actually asked.
