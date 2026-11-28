# Notes for AI assistants

C++17 LeetCode / interview prep repo. Read these before helping:

1. `STUDY_ORDER.md`, section **Now**: the next interview, the current plan (`company/google/plan/`:
   copies of the solutions numbered 001… in doing order, table in its README) and the progress log. When the user says what they
   finished, append one line to the log (date, rows covered, problems to revisit). Keep the
   plan in sync if the user changes it. Part A ranks the `select/` core problems by how often
   Google reports hit their pattern.
2. `select/README.md`: the main problem bank — one mother problem per file in 16 topic
   folders, comments as terse one-line notes (`// clarify:` one point per line,
   `// core idea:` one line, `// T: .., S: ..`, follow-ups as one-line ideas, no code for
   variants) and harder ones in `select/optional/`. Its **How to use** section sets the
   company-neutral order: Tier 1 (30 files, typed cold), Tier 2 (rest of core, read).
3. `company/<company>/README.md`: that company's session workflow, code conventions and tier
   tables (current companies: `cisco`, `moveworks`; finished ones are under
   `company/archive/`: `google`, `cloudflare`, `scientech`).
4. `README.md`: the older 300-problem bank in `topic/` (reference only).

Rules that hold across the repo:

- `prep/` folders and the drills in `topic/<category>/` (outside `ref/`) are the user's own
  code. Don't create, edit or reformat them; show fixes in chat.
- AI-written reference solutions go in a `ref/` folder (company sets) or in `select/`.
  `select/` holds mother problems only: a second solution only when the mother problem
  itself has several standard named techniques (heap / buckets / quickselect, Kahn /
  three-color DFS); every variant is a one-line idea in the closing follow-up block. A new
  reported problem first maps onto a `select/` core file: add a follow-up idea line there
  and raise the weight in `STUDY_ORDER.md`; if the variant needs code, the code goes in the
  lean copy in `company/<company>/ref/`. A reported problem that is a new pattern gets its
  own mother file in `select/`; one that combines two techniques gets a `.cpp` in the
  company `ref/` list with the next free number (numbers are stable ids, never renumber;
  the Plan in `STUDY_ORDER.md` sets the order); niche ones go in the
  company README's Optional reports table. Scope is SWE new grad / intern: a report from an
  L5+, MLE / AI-ML or research loop goes to the last table of the company README
  ("Optional — not now") instead.
- `company/google/plan/` is a numbered copy of `ref/` and `select/` files in doing order.
  The sources stay canonical: after changing a source that has a copy, apply the same
  change to the copy; plan numbers follow the order and may change, `ref/` numbers never
  do (details at the end of `company/google/plan/README.md`).
- Follow-ups are numbered one-liners, `// N. <variant> -> <what changes>` (the data
  structure, formula or index that changes, nothing more); new and edited files use this,
  older unnumbered blocks are converted when a file is touched.
- Multi-part files: one section per part, in part order. Each section starts with a
  divider `// ==================== (k) ====================`, then a 1–2 line recap of
  that part's question, then its helpers, then its core solution. A helper shared by
  several parts sits in the first part that uses it. Test code gets its own
  `// ==================== tests ====================` section.
- Comment wording: write short words in full (`with`, `without`, never `w/`, `w/o`);
  abbreviate long common words (`num`, `str`, `init`, `idx`, `len`, `prev`, `cur`,
  `max`, `min`). Problem titles keep their official wording.
- "check" = compile with `g++ -std=c++17 -Wall -Wextra`, run the file's own tests, and report
  only errors.
- Exception: `company/moveworks/ref/` is Python (`NN-slug.py`, python-interview-style
  skill); he codes Moveworks rounds in Python. There "check" = `python3 file.py`.
- `company/cisco/ref/`, `company/archive/scientech/ref/` and `company/moveworks/ref/` are numbered
  in priority order (01 first, across the tier folders) instead of stable ids: `must/` =
  type it cold once, then `recommended/` and `optional/` by importance; renumber when the
  order changes (2026-11-22 request). Every reported problem of the company has a file
  there; one already solved elsewhere is copied in with a `// copy of <path>` header line,
  so copy it again when the source changes.
- Difficulty cap for every company `ref/` (2026-11-27 request, after the Scientech round
  asked plain graph reachability): no problem above NeetCode level (NeetCode 150 /
  LeetCode easy-medium patterns, typed in 15 minutes). Anything harder or more exotic
  (lock-free rings, matching engines, allocators, hard LC) is not added to `ref/`; if a
  company really reported one, it goes in `optional/` with a one-line reason. Spend the
  time on timed drills of basics instead.
- `select/` follows the same cap (2026-11-27 request: lower intensity, company-neutral,
  bounded). Core holds LeetCode Easy / Medium only (LC 23 and LC 295 are the two
  exceptions); Hard and company-specific problems go in `select/optional/` and keep their
  number. Tier 1 has at most 30 files and core at most 90: to add one, move a colder one to
  `optional/`. When a file moves between core and `optional/`, update `select/README.md`
  and every `select/...` path that names it.
- Commit only when asked.
