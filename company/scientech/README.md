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
- Notes on the rounds (format, how to play the OA, code review and incremental design) are
  not in this repo: `~/Code/interview/coding/scientech_qd_round_notes.md`. Fundamentals
  reading order: the Scientech section of `~/Code/interview/bagu/README.md`.

## Folders

- `prep/` — his own hand-written solutions. Never create, edit or reformat files here.
- `oa/` — OA-only problems.
- `ref/` — interview problems, `NN-slug.cpp`, numbered in priority order and split into
  `must/`, `recommended/` and `optional/` (see Tiers).

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
`// follow-up:` block). One exception: `ref/must/02` is a class template, because
`shared_ptr` is one.

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

### Recommended

| # | `ref/recommended/` file | Source | Problem | `prep/` |
|---|---|---|---|---|
| 03 | `03-tower-of-hanoi.cpp` | SWE 2022 round 2 (thread 939108), "Hanoi-like", no statement | Tower of Hanoi: recursion, then the same with an explicit stack; 2^n - 1 moves; variants as ideas | — |

### Optional

| # | `ref/optional/` file | Source | Problem | `prep/` |
|---|---|---|---|---|
| 04 | `04-range-linear-regression.cpp` | MLE phone screen 2026 (thread 1194156), one line | least-squares fit on every contiguous range in O(n^2) total: four prefix sums, O(1) per range. Kept because it is plain prefix sums and the firm is a quant shop | — |

### Optional — not now (Quant Researcher / MLE loops)

| Source | Problem | Idea |
|---|---|---|
| Quant take-home 2024 (thread 1094210); Glassdoor 2026 | 24-hour take-home: one day of quotes and trades, predict the 10-second log return (2026 version: one dataset, 8 questions, pandas + regression) | as-of join trades to quotes, mid-price label, features from data at or before t, time-ordered train / test split |
| MLE phone screen 2026 (thread 1194156) | n seats in a circle, a people of type A and b of type B seated at random: expected number of same-type / different-type neighbor pairs | linearity of expectation over the n adjacent pairs: E[diff] = 2ab / (n - 1), E[same] = (a(a - 1) + b(b - 1)) / (n - 1) |

Sources: 1point3acres threads 1194156, 1094210, 988026, 939173, 939108, 938918 (company
not named), 1078068 (2024 referral post); Glassdoor interview page for Scientech Research
Capital. Collected by Siyue on 2026-11-13.
