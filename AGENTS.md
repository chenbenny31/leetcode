# Notes for AI assistants

C++17 LeetCode / interview prep repo. Read these before helping:

1. `STUDY_ORDER.md`, section **Now**: the next interview, the current plan and the progress
   log. When the user says what they finished, append one line to the log (date, rows
   covered, problems to revisit). Keep the plan table in sync if the user changes it.
2. `company/<company>/README.md`: that company's session workflow, code conventions and tier
   tables (Amazon: `company/amazon/README.md`).
3. `README.md`: the topic list and the `topic/` layout.

Rules that hold across the repo:

- `prep/` folders and the drills in `topic/<category>/` (outside `ref/`) are the user's own
  code. Don't create, edit or reformat them; show fixes in chat.
- AI-written reference solutions go in a `ref/` folder. Problems judged unlikely in a
  45-minute round go in `topic/optional/ref/` and the Optional section of `README.md`.
- "check" = compile with `g++ -std=c++17 -Wall -Wextra`, run the file's own tests, and report
  only errors.
- Commit only when asked.
