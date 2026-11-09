# PlusAI — prep guide for agents

Read this before helping Siyue prep for PlusAI. It is the only md in this company folder.

## What we know

- Role: Software Engineer (SE / Sr SE), Data Infrastructure, Santa Clara (hybrid). Owns the
  vehicle-data path: high-throughput C++ recording on resource-constrained vehicle computers,
  Python data-plane services (transfer, ingestion, validation, format conversion, metadata,
  cataloging, storage lifecycle), versioned data formats (indexing, serialization,
  compression, schema evolution), columnar / lakehouse access. Modern C++ + Python.
- Reported rounds (Glassdoor): coding = connected regions on an N x M grid with a per-region
  status such as cell count, follow-up on optimal space (Jun 2025); C++ basics on lambdas and
  rvalue references plus self-driving scenario questions (Feb 2024); one C++ coding question
  (Oct 2025).
- Problem shape to expect: a vehicle-data story that reduces to one data structure (ring
  buffer, heap merge, binary search seek, interval merge, bit-level encoding).
- Fundamentals next to coding: C++ move semantics / rvalue references / lambdas and captures
  / RAII / smart pointers; Linux concurrency, memory, filesystem and storage I/O (page cache,
  fsync, mmap), networking basics, profiling (perf); protobuf wire format, compression,
  schema evolution; Parquet / columnar layout.

## Folders

- `prep/` — his own hand-written solutions. Never create, edit or reformat files here.
- `ref/` — AI reference solutions, `NN-slug.cpp`, numbered in priority order (reported first).

## How to run a session

1. One problem at a time, in table order. Start with what to clarify, written in English
   (input order and ranges, ties, edge cases, what to return), each with the answer or the
   assumption to state.
2. Say the reduction out loud first ("this is a k-way merge with a min-heap"), then a very
   concise derivation, T/S, and the full code.
3. "check" = compile his `prep/` file with `g++ -std=c++17 -Wall -Wextra
   -fsanitize=address,undefined`, run its tests, report only errors; never edit his file.

## Code conventions

Same as `select/README.md`: one runnable C++17 file, `using std::x;` (no `std::` in the
body), the textbook solution you can type live, comments only on key data structures and
easy-to-get-wrong lines, `test(label, ..., want)` printing `label PASS` / `label FAIL`,
3-4 hand-made tests, a `// follow-up:` block at the end. Generic patterns live in `select/`.

## Problems

| # | `ref/` file | Source | Difficulty | Core DS / algorithm | What it tests | `prep/` |
|---|---|---|---|---|---|---|
| 01 | `01-sensor-blob-regions.cpp` | reported Jun 2025 | Medium | BFS flood fill, sizes sorted descending | grid BFS, in-place visited, 4-connectivity, region sizes | — |
| 02 | `02-pre-trigger-recorder.cpp` | JD | Easy | fixed-size circular array (head + count) | ring overwrite of oldest, time-window query over buffer | — |
| 03 | `03-merge-sensor-streams.cpp` | JD | Medium | k-way merge, min-heap of tuples | heap merge with deterministic timestamp tie-break by stream | — |
| 04 | `04-recording-seek.cpp` | JD | Medium | hand-written lower_bound / upper_bound | binary search template with duplicates, inclusive range count | — |
| 05 | `05-coverage-gaps.cpp` | JD | Medium | sort + merge intervals, emit gaps | half-open interval merging, clipping to a window | — |
| 06 | `06-disk-lifecycle-eviction.cpp` | JD | Medium | min-heap (priority, arrival) + running total | priority eviction, oldest tie-break, oversized-item handling | — |
| 07 | `07-zigzag-varint-codec.cpp` | JD | Medium | delta + zigzag + LEB128 bit manipulation | unsigned shifts, varint byte layout, exact encode/decode round trip | — |
| 08 | `08-tag-catalog-query.cpp` | JD | Medium | inverted index + two-pointer sorted intersection | hash map of postings, AND query, shortest list first | — |
| 09 | `09-schema-compat-check.cpp` | JD | Medium | hash map keyed by field number | protobuf-style compatibility rules, renames vs number reuse | — |

Related generic patterns in `select/`: 08-graphs/01 number-of-islands, 14-design/02
circular-queue, 10-heap/03 merge-k-sorted-lists, 04-binary-search/01 first-last-position,
11-intervals-greedy/01 merge-intervals.

Sources: Glassdoor PlusAI interview reviews (Sr Software Engineer questions; review
RVW84895188).
