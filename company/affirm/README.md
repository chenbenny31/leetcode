# Affirm — prep guide for agents

Read this before helping Siyue prep for Affirm. It is the only md in this company folder;
don't add others (no per-problem notes).

## What we know

- Full JD and current status: `~/Code/interview/jd/affirm.md`.
- Role: Software Engineer I, Backend (Streaming Infra), Remote US (remote-first; in-person
  onboarding). Recorded 2026-11-21 from the posting he pasted.
- Status (2026-11-21): he has a referral and expects an interview is likely. No date, no
  format yet. Nothing researched yet: no interview reports, no problem list.
- Team: Streaming, which owns the data side: event-driven architecture, stream
  processing, data exploration, discovery and validation. Built on Kafka, Flink and Spark,
  plus in-house tools. The work: find new uses of the data infrastructure, design and
  scale the existing one, and help other teams use data well.
- What they look for:
  - work or internship experience designing, building and launching backend systems at
    scale, in Python or Kotlin;
  - distributed-systems building blocks; AWS, MySQL, Kubernetes;
  - turning a business scenario into a solution across several components, with clear,
    tested, extensible code;
  - navigating a large codebase, debugging others' code, giving code review feedback;
  - owning one's growth, seeking feedback; strong written and verbal communication;
  - plus: building and owning large-scale stream processing (Kafka, SNS);
  - a Bachelor's in a related field or equivalent experience.
- Pay (from the posting): base grade J, equity grade 5; base $130k–$180k in CA, WA, NY,
  NJ, CT, $115k–$165k elsewhere in the US; new hires usually start at the bottom of the
  range.
- Language: the posting names Python or Kotlin. He codes interviews in C++; ask the
  recruiter whether C++ is accepted before deciding.

## Folders

- `prep/` — his own hand-written solutions. Never create, edit or reformat files here.
- `ref/` — AI reference solutions, `NN-slug.cpp` (empty until the format is known).

## Next steps once an interview is scheduled

1. Ask the recruiter for the round format and language.
2. Search 1point3acres, Glassdoor, LeetCode discuss and prep sites for Affirm SWE I /
   new-grad reports, rank the problems by how often they appear, and fill `ref/` and a
   Tiers section here the same way as `company/cisco/README.md`.
3. Fundamentals to expect from the JD: Kafka (partitions, consumer groups, offsets,
   delivery semantics), stream processing (windows, state, exactly-once), and
   distributed-systems basics: `~/Code/interview/bagu/extension/` and
   `~/Code/interview/bagu/foundation/distributed_systems.md`.
