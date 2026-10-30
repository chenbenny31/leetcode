# Stripe — prep guide for agents

Read this before helping Siyue prep for Stripe. It is the only md in this company folder;
don't add others (no per-problem notes).

## Folders

- `oa/prep/` — his own hand-written OA solutions. Never create, edit or reformat files here.
- `oa/ref/` — AI reference solutions in **Python**, one flat folder, `NN-slug.py`, numbered
  in the tier order below. A new AI solution goes here with the next free number and gets a
  row in the table. No subfolders.

## What we know about the OA

- NG OA problems are long multi-part "build a small system" tasks (billing, payments,
  routing, ledgers) on HackerRank; later parts extend earlier ones, so one function has to
  handle every part. Python is the working language for this OA.
- Statements come from FastPrep reconstructions; several parts are "authored practice", so
  follow the statement text and its examples, and say in the header comment when a rule was
  assumed.
- Order below = FastPrep sighting counts (2026 dates; WebSocket count not shown), a prep
  judgment, not a measured frequency.

## How to run a session

1. Pick the next problem in order. Skip rows that already have a `prep/` file unless he asks.
2. One problem at a time, part by part: what Part 1 needs and which state to keep for the
   later parts → T/S → the code for that part, then the next part's diff.
3. "check" = run his file in `oa/prep/` with `python3 file.py` and report only errors
   (exception vs test failure). No praise, no summaries. Never edit his file — show the fix in
   chat and he types it.

## Code conventions (`oa/ref/` files and any code shown in chat)

Write what a person could type in a live interview: plain statements, one step per line.
Rules follow PEP 8 and the Google Python Style Guide ("okay for simple cases", "use loops
instead when things get more complicated", "avoid power features").

Structure
- Every multi-part problem is a stack: header recap of the whole statement split by part
  (input/output formats and every rule, so the original isn't needed), short general
  "Stacking notes", then `class Part1` (complete, runnable) and `class PartN(PartN-1)`
  overriding only what that part changes. The judged entry function (HackerRank
  snake_case name: `process_events`, `process_ledger`, ...) runs the last part. In the
  real OA it is one class edited in place; the subclasses only show the diff per part.
  Single-part problems keep one class.
- Each Part class starts with a block comment: what this part adds and the idea behind
  the change. Non-obvious steps get a comment above them (intent, not a restatement of
  the code). Comment what each key dict / set / heap stores, and T/S on main methods.
- Data members are set at the top of `__init__`; class constants at the top of the class.
- `01-request-routing.py` and `06-async-event-processing.py` are the templates.

Statements
- One assignment per line. Tuple assignment only to unpack one sequence
  (`user, sub, cost = line.split(",")`, `for dist, name in ranked`) or to swap — never
  `a, b = f(x), g(y)`.
- Dispatch commands with a plain `if` / `elif` on the command name. No tables of bound
  methods, no `getattr`, no `f(*parts[1:])`.
- Conditional expressions only for trivial values (`"PASS" if ok else "FAIL"`); never as
  a call argument or with a non-trivial branch — write the full `if`.
- Comprehensions only when simple: one `for`, at most one short filter, fits on one line.
  Otherwise a loop. No generator passed into `sorted(...)` across lines.
- No lambda except a one-line sort key; no `map`, no `{**d}` merges, no `x = a or b`
  value tricks, no walrus, no boolean-equality tricks like `(p is None) == (...)`.
- Blank lines separate logical sections inside a function; in an `if` / `elif` chain
  whose branches are longer than one line, put a blank line between branches.
- Lines ≤ 90 columns.

Data and tests
- Money in integer cents (parse decimals by hand; `Fraction` for exact rates); no floats
  for money. Round half up explicitly — Python's `round()` rounds half to even.
- Tests: `test(label, part, input..., want)` runs that part's class; labels `p1-...`,
  `p2-...`, short, words joined with `-`, few, hand-made, never the statement's examples,
  no comments on test cases. Print only `label PASS` / `label FAIL`:
  `print(label, "PASS" if ok else "FAIL")`.
- Minimal: implement only what each part requires.

## Problems

Run a ref file with `python3 oa/ref/NN-slug.py`. `prep/` column: — = not written yet.

| # | `oa/ref/` file | Sightings | Last seen | Problem | `prep/` |
|---|---|---|---|---|---|
| 01 | `01-request-routing` | 21 | Oct 21 | datacenter REGISTER / SET_HEALTHY / DISTANCE / ROUTE | — |
| 02 | `02-subscription-billing` | 17 | Oct 27 | prorated subscriptions + chained changes + tiered usage | — |
| 03 | `03-deployment-windows` | 16 | Oct 6 | allowed minus freeze minutes, time zones, lead, min length | — |
| 04 | `04-incident-monitor` | 15 | Oct 26 | 30 s window alerts: >= 5 failures and > 1% of successes | — |
| 05 | `05-account-ledger` | 11 | Oct 18 | WIRE / ACH / STABLECOIN settlement days + ACH limits | — |
| 06 | `06-async-event-processing` | 10 | Oct 27 | payment state machine, dedup, refunds, blocked merchants | — |
| 07 | `07-split-tender-basic` | 9 | Oct 14 | gift card first, then credit card (decimal money) | — |
| 08 | `08-fraud-risk-scoring` | 5 | Sep 23 | three passes: amount multiply, repeat customer, same hour | — |
| 09 | `09-split-tender-full` | 3 | Oct 19 | 07 + expiration order, zip surcharge, refunds to cards | — |
| 10 | `10-payment-approval` | 1 | Oct 4 | CREATE / APPROVE / CAPTURE / CANCEL state machine | — |
| 11 | `11-websocket-load-balancer` | — | Sep 20 | least-load + object affinity + capacity + shutdown reroute | — |
