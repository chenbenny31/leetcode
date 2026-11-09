# Cloudflare — prep guide for agents

Read this before helping Siyue prep for Cloudflare. It is the only md in this company folder.

## What we know

- Role: Software Engineer, Network Performance & Reliability (Argo), Austin (hybrid). Argo
  makes connectivity more reliable than the Internet itself for CDN, Spectrum, Magic Transit,
  Workers, R2, WARP. Stack: Rust, Go, Linux networking, ClickHouse, PostgreSQL, Grafana, K8s.
  Run-what-you-build on-call. AI tools are expected in the workflow.
- Reported coding (Interview Query, LeetCode discuss, Glassdoor): LRU cache, a live ring
  buffer, extending a rate-limit method, flattening a nested array, parsing-heavy
  HackerRank-style problems, pair programming on a feature.
- Problem shape to expect: a networking / edge story that reduces to one data structure
  (LRU with bytes, ring buffer, token bucket, trie for prefixes, ordered map of intervals,
  consistent hashing, Dijkstra for routing).
- Fundamentals next to coding: L3/L4 (IP, routing, BGP and anycast basics, TCP handshake,
  congestion control, retransmission, UDP), HTTP/1.1 vs 2 vs 3 (QUIC), TLS handshake, CDN
  caching and cache keys, DNS, latency vs throughput, percentiles, monitoring and alerting.

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
| 01 | `01-ring-buffer-rtt.cpp` | reported | Easy | circular array + running sum | wraparound overwrite, O(1) average via running sum | — |
| 02 | `02-edge-lru-bytes.cpp` | reported | Medium | list + hash map LRU, byte budget | multi-evict by bytes, update resize, oversized skip | — |
| 03 | `03-token-bucket-limiter.cpp` | reported | Medium | hash map client -> tokens, lazy refill | refill math, burst cap, per-client isolation | — |
| 04 | `04-argo-lowest-latency-route.cpp` | JD | Medium | Dijkstra min-heap + parent array | weighted shortest path plus path reconstruction | — |
| 05 | `05-longest-prefix-match.cpp` | JD | Medium | binary trie on IPv4 bits | CIDR parsing, bit walk, longest match fallback | — |
| 06 | `06-tcp-reassembly.cpp` | JD | Medium | ordered map interval merge | out-of-order, overlap, dup segments, contiguous pointer | — |
| 07 | `07-origin-health-hysteresis.cpp` | JD | Easy | streak counters + ordered set | hysteresis streak resets, lowest healthy via set | — |
| 08 | `08-consistent-hash-ring.cpp` | JD | Medium | ordered map + lower_bound, virtual nodes | ring lookup, wraparound, add/remove virtual nodes | — |
| 09 | `09-sliding-latency-percentile.cpp` | JD | Medium | queue + multiset sliding window | nearest-rank math, expiry, erase one duplicate | — |
| 10 | `10-multi-field-rate-limiter.cpp` | reported | Medium | deque per rule, sliding-window log | AND-match on field subsets, inclusive window, check all then record all | — |
| 11 | `11-stack-with-increment.cpp` | LC tag (1381) | Medium | vector stack; lazy inc[] passed down on pop | bottom-k loop first, O(1) lazy add as the follow-up | — |
| 12 | `12-reaching-points.cpp` | LC tag (780) | Hard | work backward, subtraction -> modulo | unique parent backward, gcd-style %, multiple check at the start | — |

## LeetCode company tag (Cloudflare, more than six months)

Only 1381 and 780 needed new files (rows 11-12); the rest were already covered:

| LC | Problem | Where it lives |
|---|---|---|
| 15 | 3Sum | `select/02-two-pointers/01-three-sum.cpp` |
| 18 | 4Sum | `topic/two-pointers/18_four-sum.cpp` |
| 1381 | Design a Stack With Increment Operation | `ref/11-stack-with-increment.cpp` |
| 362 | Design Hit Counter | `select/14-design/03-hit-counter.cpp` |
| 622 | Design Circular Queue | `select/14-design/02-circular-queue.cpp` |
| 146 | LRU Cache | `select/14-design/01-lru-cache.cpp` (byte-budget variant: `ref/02`) |
| 200 | Number of Islands | `select/08-graphs/01-number-of-islands.cpp` |
| 780 | Reaching Points | `ref/12-reaching-points.cpp` |
| 359 | Logger Rate Limiter | `topic/design/ref/359_logger-rate-limiter.cpp` |
| 23 | Merge k Sorted Lists | `select/10-heap/03-merge-k-sorted-lists.cpp` |
| 207 | Course Schedule | `select/08-graphs/02-course-schedule.cpp` |

Rate-limiter family in priority order: `ref/03` token bucket -> `ref/10` multi-field rules
-> LC 359 / 362 as warm-ups. Distributed rate limiting theory:
`~/Code/interview/bagu/foundation/distributed_systems.md` (Rate limiting and token bucket).

Related generic patterns in `select/`: 14-design/01 lru-cache, 14-design/02 circular-queue,
14-design/03 hit-counter, 09-advanced-graphs/01 network-delay, 15-trie/01 implement-trie,
11-intervals-greedy/01 merge-intervals, 14-design/10 flatten-nested-iterator (the reported
"flatten a nested array").

Sources: Interview Query Cloudflare SWE guide; LeetCode discuss post 7396827; Glassdoor
Cloudflare interview reviews; LeetCode Cloudflare company tag (more than six months).
