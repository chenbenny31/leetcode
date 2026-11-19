# Scientech Research — prep guide for agents

Read this before helping Siyue prep for Scientech. It is the only md in this company folder;
don't add others (no per-problem notes).

## What we know

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
  class, with a hand-written `shared_ptr` as the example. `ref/must/02` covers that.
- Three reports say HR stops replying; follow up after one to two weeks.
- 2026-11-17: he was invited to interview. `ref/` 05–28 were added that day; none of them
  is a reported Scientech problem. 14–28 are the quant-dev core he asked for, chosen from
  problems that recur across public write-ups of quant developer interviews (order book,
  matching engine, lock-free rings, thread pool, hand-written vector, memory pool, rolling
  stats, conflation, feed parsing, PnL): these come first. 05, 06, 07, 09, 11 are what is
  left of the OA codec family after he asked to keep only the key variants (UTF-8, hex,
  base64 and a bit packer were deleted: don't add them back). A search that day found no
  new Scientech reports.
- Notes on the rounds (format, how to play the OA, code review and incremental design) are
  not in this repo: `~/Code/interview/coding/scientech_qd_round_notes.md`. Walkthroughs
  (clarify, derivation, key code, pitfalls, follow-ups per problem):
  `~/Code/interview/coding/scientech_quant_core.md` for 14–28 (its section 16 lists the
  common quant live-coding problems found on the web and where each one is covered) and
  `~/Code/interview/coding/scientech_codec_family.md` for the OA codec and 05–11. Fundamentals
  reading order: the Scientech section of `~/Code/interview/bagu/README.md`.

## Folders

- `prep/` — his own hand-written solutions. Never create, edit or reformat files here.
- `oa/` — OA-only problems.
- `ref/` — interview problems, `NN-slug.cpp`, split into `must/`, `recommended/` and
  `optional/` (see Tiers). The number is only an id (01–04 reported, 05–11 codec family,
  14–28 quant core; 08, 10, 12, 13 were deleted); the tier gives the priority.

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

Same as the Code conventions section of `company/google/README.md` (terse one-line
comments, `// clarify:` one point per line, `// core idea:` and `// T: .., S: ..` above
each solution, `test(label, input..., want)` printing `label PASS` / `label FAIL`, a closing
`// follow-up:` block). Two files are class templates because the real thing is one:
`ref/must/02` (`shared_ptr`) and `ref/recommended/24` (`vector`).

Files 05–28 follow the layout of his own `oa/01-vlq-codec.cpp` instead, and any new file
in this folder should too:

- top of file: a 2–3 line statement, then `// usage:` (compile line + which class for
  what), `// clarify / assumptions`, then the API contract and `// bit-ops:` cheat lines
  (codec files) or `// data:` and T / S (14–28);
- a class with `private:` constants first, then `public:` static `encode` / `decode` that
  take the caller's buffer (`val, buf, cap` / `buf, len, val&`) and return the number of
  bytes, `-1` on failure;
- above each function one line for the idea and one for the return value and T / S;
  short trailing comments on the key lines only;
- tests: `testXxx(label, val, want)` checking bytes, `nread == nwrite` and the round
  trip, plus `check(label, ok)`; `main` grouped as valid cases / following bytes /
  invalid cases;
- sequence-style problems (14–28) test with `check(label, ok)` after each step;
- threaded files (16, 17, 18, 23, 28) compile with `-pthread`, use thread functions (or
  `bind`) instead of lambdas, and must stay clean under `-fsanitize=thread`;
- no follow-up block in the file: follow-ups live in the walkthrough md.

## Tiers

Run a file with `g++ -std=c++17 -Wall -Wextra -g -fsanitize=address,undefined <file> -o a
&& ./a`. The `prep/` column names his file (— = not written yet).

### OA

| # | `oa/` file | Source | Problem | `prep/` |
|---|---|---|---|---|
| 01 | `01-vlq-codec.cpp` | Quant Dev OA 2022, 2023 (threads 939173, 988026) | VLQ codec: (1) unsigned encode into the caller's buffer + decode that finds its own end; (2) negatives via zigzag on top of (1). This file is his own hand-written version (`UnsignedCodec` / `SignedCodec`): never restyle or rewrite it | — |

### Must

| # | `ref/must/` file | Source | Problem | `prep/` |
|---|---|---|---|---|
| 01 | `01-text-justification.cpp` | Quant Dev interview 2023 (thread 988026), LC 68 | greedy line packing, then each / extra space split; last and one-word lines left-justified. The poster needed about 35 minutes: type it until it runs first time | — |
| — | `select/02-two-pointers/07-sort-colors.cpp` | Quant Dev 2023 + SWE 2022, LC 75 | Dutch national flag, one pass with three regions. Already in `select/`, no copy here; write his own tests (empty, one color, sorted, reversed) | — |
| 02 | `02-shared-ptr.cpp` | thread 938918 (unverified) + smart pointer questions in thread 988026 | hand-written `shared_ptr`: count on the heap, rule of five, self-assignment, `noexcept` moves; `unique_ptr`, `weak_ptr`, `make_shared` as follow-up ideas | — |
| 05 | `05-leb128-codec.cpp` | OA follow-up (not reported) | the OA codec with the low chunk first (protobuf varint): two lines differ in encode / decode; the decoder needs a 10-byte guard because its shift grows | — |
| 06 | `06-varint-series.cpp` | OA follow-up (not reported) | many values in one buffer on top of the OA classes: (1) count prefix + `buf + pos, cap - pos`; (2) a price series stored as signed diffs to the previous value | — |
| 14 | `14-order-book.cpp` | quant core (not reported) | book building from a feed: add / cancel / reduce; `map` price -> total qty per side + id -> order; best bid / ask, qty at a price | — |
| 15 | `15-matching-engine.cpp` | quant core (not reported) | limit orders with price-time priority: `map` price -> `list` of orders, id -> list iterator; submit returns trades at the maker's price, the rest rests; cancel; modify (less qty at the same price keeps its place, anything else is cancel + submit) | — |
| 16 | `16-spsc-ring.cpp` | quant core (not reported) | lock-free SPSC ring: two counters with one writer each, relaxed / acquire / release | — |
| 17 | `17-spmc-ring.cpp` | quant core (not reported); his roster project has an SPMC ring | (1) work queue: consumers claim a msg with CAS on head; (2) broadcast: seqlock per slot, a slow consumer detects being lapped | — |

### Recommended

| # | `ref/recommended/` file | Source | Problem | `prep/` |
|---|---|---|---|---|
| 03 | `03-tower-of-hanoi.cpp` | SWE 2022 round 2 (thread 939108), "Hanoi-like", no statement | Tower of Hanoi: recursion, then the same with an explicit stack; 2^n - 1 moves; variants as ideas | — |
| 07 | `07-endian-fixed-width.cpp` | codec family; the OA statement mentions endianness | uint32 as 4 bytes, big- and little-endian, with shifts (portable); host byte order check; byte swap | — |
| 09 | `09-memmove.cpp` | standard C++ hand-written task | `memcpy` and `memmove`: copy forward when dst is before src, backward when after | — |
| 11 | `11-bitset.cpp` | standard implementation class | n bits in 64-bit words: `set` / `clear` / `test` / `count` (`word &= word - 1`) | — |
| 18 | `18-blocking-queue.cpp` | quant core (not reported) | bounded queue with mutex + two condition variables; `close()` for shutdown | — |
| 19 | `19-block-pool.cpp` | quant core (not reported) | fixed-size memory pool: one allocation, free list stored inside the free blocks, placement new | — |
| 20 | `20-rolling-vwap.cpp` | quant core (not reported) | VWAP over a time window: deque + running sums; max price with a monotonic deque | — |
| 21 | `21-feed-sequencer.cpp` | quant core (not reported) | hand on msgs in seq order: `nextSeq` + map of early msgs; drops old and duplicate seq (A / B feeds) | — |
| 22 | `22-position-pnl.cpp` | quant core (not reported) | (1) average cost: signed position, average entry price, realized / unrealized PnL, a fill may cross through flat; (2) FIFO lots in a deque | — |
| 23 | `23-thread-pool.cpp` | quant core (not reported) | fixed workers on one task queue: take a task under the lock, run it outside; `shutdown()` drains, then joins | — |
| 24 | `24-vector.cpp` | quant core (not reported); reported at Jump | hand-written `vector`: raw memory + placement new, doubling growth, rule of five, push_back of its own element | — |
| 25 | `25-conflating-queue.cpp` | quant core (not reported); reported at Citadel Securities | keep only the newest unread quote per symbol; symbols are handed out in waiting order; late quotes dropped | — |
| 26 | `26-currency-arbitrage.cpp` | quant core (not reported); reported at Citadel Securities | edge weight `-log(rate)`, Bellman-Ford finds a negative cycle and returns it | — |
| 27 | `27-feed-parser.cpp` | quant core (not reported) | one text line of an order feed -> msg with a char loop; price to integer ticks without double; rejects malformed lines | — |

### Optional

| # | `ref/optional/` file | Source | Problem | `prep/` |
|---|---|---|---|---|
| 04 | `04-range-linear-regression.cpp` | MLE phone screen 2026 (thread 1194156), one line | least-squares fit on every contiguous range in O(n^2) total: four prefix sums, O(1) per range. Kept because it is plain prefix sums and the firm is a quant shop | — |
| 28 | `28-spinlock.cpp` | quant core (not reported) | `atomic<bool>` spinlock: exchange to take it, plain loads while waiting; `tryLock` | — |

### Optional — not now (Quant Researcher / MLE loops)

| Source | Problem | Idea |
|---|---|---|
| Quant take-home 2024 (thread 1094210); Glassdoor 2026 | 24-hour take-home: one day of quotes and trades, predict the 10-second log return (2026 version: one dataset, 8 questions, pandas + regression) | as-of join trades to quotes, mid-price label, features from data at or before t, time-ordered train / test split |
| MLE phone screen 2026 (thread 1194156) | n seats in a circle, a people of type A and b of type B seated at random: expected number of same-type / different-type neighbor pairs | linearity of expectation over the n adjacent pairs: E[diff] = 2ab / (n - 1), E[same] = (a(a - 1) + b(b - 1)) / (n - 1) |

Already in the repo, no copy here. Quant core: LRU cache
(`select/14-design/01-lru-cache.cpp`), sliding window max
(`topic/sliding-window/ref/239_sliding-window-max.cpp`), median from a stream
(`topic/trie-heap/ref/295_median-stream.cpp`), rate limiters (`company/cloudflare/ref/`).
Codec family: LC 271 Encode and Decode Strings
(`topic/arrays-hashing/ref/271_encode-decode-strings.cpp`, length-prefix framing in text),
LC 443 String Compression (`topic/parser/ref/443_string-compression.cpp`, run-length in
place), LC 8 atoi (`select/optional/16-bit-math-matrix/07-string-to-integer-atoi.cpp`), bit
basics (`select/16-bit-math-matrix/04-bit-ops.cpp`), ring buffer
(`company/cloudflare/ref/01-ring-buffer-rtt.cpp`).

Sources: 1point3acres threads 1194156, 1094210, 988026, 939173, 939108, 938918 (company
not named), 1078068 (2024 referral post); Glassdoor interview page for Scientech Research
Capital. Collected by Siyue on 2026-11-13.
