# Notes for AI assistants

C++17 LeetCode / interview prep repo. Read these before helping:

1. `STUDY_ORDER.md`, section **Now**: the next interview, the current plan (`company/google/ref/`
   plus some `select/` files, in the Plan's frequency order) and the progress log. When the user says what they
   finished, append one line to the log (date, rows covered, problems to revisit). Keep the
   plan in sync if the user changes it. Part A ranks the `select/` core problems by how often
   Google reports hit their pattern.
2. `select/README.md`: the main problem bank — one mother problem per file in 16 topic
   folders, comments as terse one-line notes (`// clarify:` one point per line,
   `// core idea:` one line, `// T: .., S: ..`, follow-ups as one-line ideas, no code for
   variants) and harder ones in `select/optional/`.
3. `company/<company>/README.md`: that company's session workflow, code conventions and tier
   tables (current companies: `google`, `cloudflare`).
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
  company `ref/` list at its priority number (renumber what follows); niche ones go in the
  company README's Optional reports table. Scope is SWE new grad / intern: a report from an
  L5+, MLE / AI-ML or research loop goes to the last table of the company README
  ("Optional — not now") instead.
- "check" = compile with `g++ -std=c++17 -Wall -Wextra`, run the file's own tests, and report
  only errors.
- Commit only when asked.
