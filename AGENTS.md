# Notes for AI assistants

C++17 LeetCode / interview prep repo. Read these before helping:

1. `STUDY_ORDER.md`, section **Now**: the next interview, the current plan and the progress
   log. When the user says what they finished, append one line to the log (date, rows
   covered, problems to revisit). Keep the plan in sync if the user changes it. Part A ranks
   the `select/` core problems by how often Google reports hit their pattern.
2. `select/README.md`: the main problem bank — core problems in 16 topic folders (the basic
   algorithm plus its important follow-ups as code) and harder ones in `select/optional/`.
3. `company/<company>/README.md`: that company's session workflow, code conventions and tier
   tables (current companies: `google`, `cloudflare`).
4. `README.md`: the older 300-problem bank in `topic/` (reference only).

Rules that hold across the repo:

- `prep/` folders and the drills in `topic/<category>/` (outside `ref/`) are the user's own
  code. Don't create, edit or reformat them; show fixes in chat.
- AI-written reference solutions go in a `ref/` folder (company sets) or in `select/`. A new
  reported problem first maps onto a `select/` core file: add its variant there as a follow-up
  function and raise the weight in `STUDY_ORDER.md`. A reported problem that combines two
  techniques gets a company `ref/recommended/` file; niche ones go in the company README's
  Optional table.
- "check" = compile with `g++ -std=c++17 -Wall -Wextra`, run the file's own tests, and report
  only errors.
- Commit only when asked.
