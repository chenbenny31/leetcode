# Scientech Research — prep guide for agents

Read this before helping Siyue prep for Scientech. It is the only md in this company folder;
don't add others (no per-problem notes).

## What we know

- Full JD and current status: `~/Code/interview/jd/scientech.md`.
- The posting he applied to (Ashby, Junior Quantitative Developer, New Jersey, on-site):
  build and run the in-house research data platform (price / volume, fundamental,
  alternative data): ingestion, processing, quality checks, anomaly detection,
  archiving, research tools; C++ and Python required. He says the interview is
  100% C++ (2026-11-21).
- Role: Quant Developer (new grad, applied 2026-11). Scientech Research Capital is a small
  quant firm (founded 2019; Jersey City / New York and Shanghai; about 20+ people, mostly
  research). Main languages are Python and C++; a 2024 referral post says 5 days in office.
- Seven public reports exist (1point3acres ×5, Glassdoor ×2). Three are on the developer
  side and drive this folder: Quant Dev intern 2022 (OA), Quant Dev intern 2023 (OA +
  technical interview), SWE new grad 2022 (several technical rounds). The other four are
  Quant Researcher / MLE loops (take-home data modeling, probability): see the last table.
- Process for Quant Dev: HR emails the OA at an agreed time, 2 hours, send the code back →
  technical interview about a week later (remote) → later rounds are not reported. The SWE
  2022 report had two problems in round 1, one in round 2, and "several" onsite rounds.
- The OA was the same problem in 2022 and 2023: the VLQ codec in `oa/01`. Both posters
  were rejected; one finished only part 1 and described part 2 in words. Target: both
  parts typed, tested and commented inside 2 hours.
- Interview coding so far: LC 68 Text Justification, Dutch national flag (reported twice),
  and "a Hanoi-like problem" (no statement). The candidate writes the tests.
- Fundamentals are asked in detail: C++ first (smart pointers were asked in the intern
  interview), then OS, networking and design patterns; questions follow the resume.
- Unverified: thread 938918 does not name the company; the SWE 2022 poster guessed its
  author is an interviewer there. It describes an onsite with a code review / debug round
  and an incremental design round (requirements added one at a time; do you hack or
  redesign), and C++ asked through real code: smart pointers, constructors, writing a
  class, with a hand-written `shared_ptr` as the example. `ref/must/04` covers that.
- Three reports say HR stops replying; follow up after one to two weeks.
- 2026-11-17: he was invited to interview. The codec follow-ups and the quant-dev core
  were added that day; none of them is a reported Scientech problem. The quant-dev core
  he asked for was chosen from problems that recur across public write-ups of quant
  developer interviews (order book, matching engine, lock-free rings, thread pool,
  hand-written vector, memory pool, rolling stats, conflation, feed parsing, PnL).
  `recommended/14`, `15`, `21`, `22` and `optional/24` are what is left of the OA codec
  family after he asked to keep only the key variants (UTF-8, hex, base64 and a bit packer were deleted: don't
  add them back). A search that day found no new Scientech reports.
- Notes on the rounds (format, how to play the OA, code review and incremental design) are
  not in this repo: `~/Code/interview/coding/scientech_qd_round_notes.md`. Walkthroughs
  (clarify, derivation, key code, pitfalls, follow-ups per problem):
  `~/Code/interview/coding/scientech_quant_core.md` for the quant core (its section 16
  lists the common quant live-coding problems found on the web and where each one is
  covered) and
  `~/Code/interview/coding/scientech_codec_family.md` for the OA codec family. Fundamentals
  reading order: the Scientech section of `~/Code/interview/bagu/README.md`.

## Junior research-data track

- The JD centers on onboarding datasets, ingestion, data quality, anomaly detection,
  archiving, and research tools. Keep the reported coding questions at the top and type
  the parser, sequencer, and rolling VWAP as role-specific practice.
- Walk through one dataset onboarding. Pin down the schema, timestamp convention, units,
  and row key. Check malformed rows, missing values, duplicate keys, late records, and
  price/volume outliers. Keep raw inputs and record validation failures so the same data
  can be replayed. Explain how a researcher would check coverage and point-in-time
  availability before using the dataset.
- For ingestion concurrency, start with the bounded blocking queue and explain
  backpressure and shutdown. For a roster project follow-up, explain SPSC index
  ownership and SPMC delivery semantics, publication, and slow readers.
- Type the reported VLQ exercise separately when preparing for the OA. Use its codec
  variants as follow-ups in a live round.

## Folders

- `prep/` — his own hand-written solutions. Never create, edit or reformat files here.
- `oa/` — OA-only problems.
- `ref/` — interview problems, `NN-slug.cpp`, split into `must/`, `recommended/` and
  `optional/` (see Tiers). The number is the priority order (01 first, across the three
  folders). Type `must/` cold once. Read or explain `recommended/` aloud in order.
  Use `optional/` for deeper follow-ups.

## How to run a session

1. One problem at a time, in tier order. Start with what to clarify with the interviewer,
   written in English, as a plain list.
2. A very concise derivation (which idea, why it is efficient) → T/S for the crucial
   functions → the full code.
3. For the OA: recap each part's statement, then build part 2 on top of part 1 without
   changing part 1's functions.
4. "check" = compile his `prep/` file with `g++ -std=c++17 -Wall -Wextra
   -fsanitize=address,undefined`, run its tests, report only errors; never edit his file.

## Code conventions

Every file in `ref/` follows the `cpp-live-interview-style` skill (restyled 2026-11-26):
the smallest version that meets the statement, the way a new grad would type it in the
round. What a junior is unlikely to be asked for (extra query methods, range checks the
statement does not ask for, `int64_t` everywhere) is a one-line follow-up, not code.

For live coding, use the corresponding hand-written `prep/` attempt as a guide to the
approach, variable names, and tests. Prefer familiar C++17 that meets the stated bounds.
Use owning strings for parsed fields and ordinary map operations for deduplication.
Discuss views or fewer hash lookups when a follow-up asks about copying or throughput.

- line 1 is `// NN · title — source`, then a 1–3 line statement and a `// clarify:`
  block, one `question -> answer` per line;
- `// core idea:` and `// T: .., S: ..` above each solution; in a class the data members
  come first, each with a short note on what it stores, then private helpers, then
  `public:` (no `private:` label);
- free functions for an algorithm, a class only for a design problem; `int` and
  `long long` instead of `int64_t`, except in the codec and lock-free files;
- tests print `label PASS` / `label FAIL`: `test(label, input..., want)` for a function,
  `check(ok, label)` after each step of a short call sequence for a class; no comments
  on test lines;
- a closing numbered `// follow-up:` block, `// N. <variant> -> <what changes>`;
- multi-part files use the `// ==================== (k) ====================` dividers;
- lines stay within 90 columns and the body never spells `std::`.

Exceptions:

- `oa/01-vlq-codec.cpp` is his own file and keeps its own layout. The codec follow-ups
  (`recommended/15`, `optional/24`) keep its contract: `encode(val, buf, cap)` /
  `decode(buf, len, val&)` return the number of bytes, `-1` on failure. `optional/24`
  carries his two codec classes unchanged.
- Two files are class templates because the real thing is one: `must/04` (`shared_ptr`)
  and `optional/30` (`vector`).
- Threaded files (`recommended/18`, `23`, `optional/25`, `26`, `34`) compile with
  `-pthread`, use thread functions (or `bind`) instead of lambdas, and must stay clean
  under `-fsanitize=thread`. The 2026-11-26 restyle was checked with
  `-fsanitize=address,undefined` only: `libtsan` is not installed on this machine
  (`sudo dnf install libtsan`).
- A file with `// copy of <path>` on line 2 keeps the wording of its source.

## Tiers

Run a file with `g++ -std=c++17 -Wall -Wextra -g -fsanitize=address,undefined <file> -o a
&& ./a`. The `prep/` column names his file (— = not written yet).

### OA

| # | `oa/` file | Source | Problem | `prep/` |
|---|---|---|---|---|
| 01 | `01-vlq-codec.cpp` | Quant Dev OA 2022, 2023 (threads 939173, 988026) | VLQ codec: (1) unsigned encode into the caller's buffer + decode that finds its own end; (2) negatives via zigzag on top of (1). This file is his own hand-written version (`UnsignedCodec` / `SignedCodec`): never restyle or rewrite it | — |

Tiers were cut again on 2026-11-26 by new-grad difficulty, for a junior first round:
`must/` = every reported problem plus the JD practice a junior can finish in 25 minutes;
`recommended/` = new-grad level and plausible in round 1; `optional/` = beyond a junior
first round (lock-free code, allocators, trading-desk logic) or off the role. Level is
how hard it is for a new grad: easy (10 minutes), medium (25 minutes), hard (35 minutes
or a trick to know), beyond (explain it, don't type it).

Moved that day: Hanoi `recommended` → `must/03` (reported, easy); `shared_ptr`
`recommended` → `must/04` (smart pointers are the C++ topic the reports name); bit ops
`optional` → `recommended/13` (easy); varint series `recommended` → `optional/24` (long
OA follow-up). Inside `recommended/` the LeetCode-style strings and arrays now come before
the codec variants, concurrency and the quant designs.

### Must — type it cold once

| # | File | Source | Level | Practice | `prep/` |
|---|---|---|---|---|---|
| 01 | `01-text-justification.cpp` | Quant Dev interview 2023 (thread 988026), LC 68 | hard | Greedy line packing, distribute extra spaces from the left, pad the last and single-word lines. | `text-justify.cpp` |
| 02 | `02-sort-colors.cpp` | Quant Dev 2023 + SWE 2022, LC 75 | medium | Dutch national flag, one pass with three regions. Dry-run a 2 swapped in from the right. | `sort-colors.cpp` |
| 03 | `03-tower-of-hanoi.cpp` | SWE 2022 round 2 (thread 939108), "Hanoi-like", no statement | easy | Type part 1, the recursion, and say why it is 2^n - 1 moves. Part 2, the explicit stack, is read only. | — |
| 04 | `04-shared-ptr.cpp` | thread 938918 (unverified) + smart pointer questions in thread 988026 | medium | Two pointers per owner: the object and a heap count. Type ctor, dtor and copy ctor first, then copy assign, then the two moves. Explain RAII, copy vs move, and cycles. | `shared-ptr.cpp` |
| 05 | `05-feed-parser.cpp` | JD practice (ingestion and data quality) | medium | Split on commas, check the field count per message type, digits-only ints with an overflow guard, price to ticks by padding the decimals. | — |
| 06 | `06-feed-sequencer.cpp` | JD practice (duplicates and ordering) | easy | Track the next sequence with a hash map for early messages. Deduplicate and drain the contiguous run. | — |
| 07 | `07-rolling-vwap.cpp` | JD practice (research data and analysis) | medium | Type part 1 first. Use a deque and running weighted sum and volume. Part 2 adds a monotonic deque for the max. | — |

### Recommended — read or explain aloud

| # | File | Source | Level | Practice | `prep/` |
|---|---|---|---|---|---|
| 08 | `08-lru-cache.cpp` | quant core (not reported), LC 146 | medium | Hash map and a doubly linked list give O(1) lookups and eviction. | — |
| 09 | `09-string-to-integer-atoi.cpp` | codec family, LC 8 | medium | Scan chars, handle signs and clamp overflow. Same overflow guard as the feed parser. | — |
| 10 | `10-string-compression.cpp` | codec family, LC 443 | medium | Write run lengths in place with read and write indices. | — |
| 11 | `11-encode-decode-strings.cpp` | codec family, LC 271 | medium | Frame each string with its length so delimiters can appear in the payload. | — |
| 12 | `12-ring-buffer-rtt.cpp` | Quant core, Cloudflare copy | easy | Fixed-size ring with a running sum. A simple circular buffer exercise. | — |
| 13 | `13-bit-ops.cpp` | codec family: bit basics | easy | Flip, clear and count bits with masks. | — |
| 14 | `14-endian-fixed-width.cpp` | codec family; the OA statement mentions endianness | easy | Read and write a uint32 with shifts. Explain byte order and byte swapping. | — |
| 15 | `15-leb128-codec.cpp` | OA follow-up (not reported) | medium | Write the low chunk first. Bound the read to ten bytes for a 64-bit value. | — |
| 16 | `16-sliding-window-max.cpp` | quant core, LC 239 | hard | Monotonic deque of indices. Expire old indices and drop dominated values. | — |
| 17 | `17-median-stream.cpp` | quant core, LC 295 | hard | Two heaps keep the running median. Their sizes differ by at most one. | — |
| 18 | `18-blocking-queue.cpp` | quant core (not reported) | medium | Bounded ingestion queue with a mutex and two condition variables. Explain backpressure and drain on close. | — |
| 19 | `19-order-book.cpp` | quant core (not reported) | medium | Build a depth snapshot with an order ID map and sorted price-level totals. Add, cancel and reduce. | `order-book.cpp` |
| 20 | `20-conflating-queue.cpp` | quant core (not reported); reported at Citadel Securities | medium | Keep the newest unread price per symbol and pop symbols in waiting order. Late quotes are a follow-up. | — |
| 21 | `21-memmove.cpp` | standard C++ hand-written task | medium | Choose copy direction from pointer order when the ranges overlap. | — |
| 22 | `22-bitset.cpp` | standard implementation class | easy | Pack flags into 64-bit words. Set, clear, test and count. | — |
| 23 | `23-spsc-ring.cpp` | Resume drill, quant core | beyond | Resume follow-up. Explain who owns each index, when a slot is published, and when it may be reused. | `spsc-ring.cpp` |

### Optional — beyond a junior first round

| # | File | Source | Level | Practice | `prep/` |
|---|---|---|---|---|---|
| 24 | `24-varint-series.cpp` | OA follow-up (not reported) | medium, long | Length-prefix a batch, then delta-encode a price series. | — |
| 25 | `25-spmc-ring.cpp` | Roster project drill | beyond | Resume follow-up. Explain one consumer per item versus broadcast, and how a slow reader handles overwrite. | — |
| 26 | `26-thread-pool.cpp` | quant core (not reported) | hard | Workers take jobs under one lock and run them outside it. Shutdown drains then joins. | — |
| 27 | `27-token-bucket-limiter.cpp` | Quant core, Cloudflare copy | medium, off the role | Refill tokens by elapsed time. | — |
| 28 | `28-position-pnl.cpp` | quant core (not reported) | medium, trading desk | Average cost and FIFO lots for realized and unrealized PnL. | — |
| 29 | `29-range-linear-regression.cpp` | Prefix-sum practice | medium, MLE loop | Compute least-squares over every range with four prefix sums. | — |
| 30 | `30-vector.cpp` | quant core (not reported); reported at Jump | hard | Raw storage, placement new, growth and copy/move ownership. | — |
| 31 | `31-block-pool.cpp` | quant core (not reported) | beyond | Fixed-size blocks with a free list inside unused blocks. | — |
| 32 | `32-matching-engine.cpp` | quant core (not reported) | beyond | Price-time priority with a queue per price and an ID index for cancellation. | — |
| 33 | `33-currency-arbitrage.cpp` | quant core (not reported); reported at Citadel Securities | hard | Use negative log rates and Bellman-Ford to detect a profitable cycle. | — |
| 34 | `34-spinlock.cpp` | quant core (not reported) | beyond | Atomic exchange to acquire, plain loads while spinning. | — |

### Optional — not now (Quant Researcher / MLE loops)

| Source | Problem | Idea |
|---|---|---|
| Quant take-home 2024 (thread 1094210); Glassdoor 2026 | 24-hour take-home: one day of quotes and trades, predict the 10-second log return (2026 version: one dataset, 8 questions, pandas + regression) | as-of join trades to quotes, mid-price label, features from data at or before t, time-ordered train / test split |
| MLE phone screen 2026 (thread 1194156) | Least-squares fit over every contiguous range | Prefix sums of x, y, x² and xy |
| MLE phone screen 2026 (thread 1194156) | n seats in a circle, a people of type A and b of type B seated at random: expected number of same-type / different-type neighbor pairs | linearity of expectation over the n adjacent pairs: E[diff] = 2ab / (n - 1), E[same] = (a(a - 1) + b(b - 1)) / (n - 1) |

Files whose header says "copy of" mirror a `select/`, `topic/` or other company file;
when the source changes, copy it again.

Sources: 1point3acres threads 1194156, 1094210, 988026, 939173, 939108, 938918 (company
not named), 1078068 (2024 referral post); Glassdoor interview page for Scientech Research
Capital. Collected by Siyue on 2026-11-13.
