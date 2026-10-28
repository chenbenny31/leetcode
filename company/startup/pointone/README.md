# PointOne prep

Guess based on the product (lawyers bill in 6-minute units; month-end bill review),
not on interview reports. Likely themes: interval merge / split, per-client / matter
roll-ups, activity log -> time entries, billing-rule validation; system design around a
timekeeping service and a RAG pipeline.

- `ref/` = reference solutions (AI-written). Put your own attempts in this folder.
- Each ref file: straightforward solution first; an improved one beside it when it is
  substantially better; both checked by the same small hand-made tests.

## Tier 1 — most on-theme
| File | Source | Scenario | Technique |
|---|---|---|---|
| `ref/00-task-log.cpp` | PointOne | start / end log -> closed time in a window, per id | clip intervals; binary search |
| `ref/01-exclusive-time.cpp` | LC 636 | activity log -> time per task | stack + last timestamp |
| `ref/02-insert-interval.cpp` | LC 57 | add / edit one entry in a sorted timeline | sort+merge / linear 3-phase |
| `ref/03-meeting-rooms.cpp` | LC 252, 253 | double-billing check, max concurrency | sort; min-heap of ends |
| `ref/04-interval-intersections.cpp` | LC 986 | activity log ∩ billable windows | all pairs / two pointers |
| `ref/05-invalid-transactions.cpp` | LC 1169 | billing-rule validation, report all | parse + pairwise rules |
| `ref/06-log-storage.cpp` | LC 635 | query records by granularity | prefix compare / sorted map |
| `ref/07-six-minute-billing.cpp` | practical | 6-minute rounding, per-client totals | ceil in int math + map |
| `ref/08-split-by-period.cpp` | practical | split entries at midnight / period end | walk boundaries |

Already in the repo (review, don't redo):
- LC 56 Merge Intervals — `company/apple/ref/09-merge-intervals.cpp`
- LC 981 Time Based Key-Value Store (rate at time t) — `company/apple/ref/25-time-map.cpp`
- LC 1146 Snapshot Array (bill revisions) — `company/apple/ref/28-snapshot-array.cpp`
- LC 362 Design Hit Counter — `company/apple/ref/22-design-hit-counter.cpp`

## Tier 2 — same family, if time allows
LC 435 Non-overlapping Intervals (min removals to fix overlaps) · LC 759 Employee Free
Time (unlogged gaps) · LC 1604 Key-Card Alert (log + time-window rule) · LC 715 Range
Module (add / remove / query ranges) · LC 692 Top K Frequent Words (roll-up + tie-break)
· LC 721 Accounts Merge (dedupe client identities)

Suggested order: 00 -> 01 -> 02 -> 03 -> 04 -> 05 -> 06, then 07 / 08, then a quick pass over
the already-done ones above.
