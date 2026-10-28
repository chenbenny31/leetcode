# Snowflake — prep guide for agents

Read this before helping Siyue prep for Snowflake. It is the only md in this company folder;
don't add others (no per-problem notes).

## Folders

- `prep/` — his own hand-written solutions (followups as `*-followup.cpp`). Never create, edit
  or reformat files here.
- `ref/` — AI reference solutions, one flat folder, `NN-slug.cpp`, `01`–`59`, numbered in the
  tier order below. A new AI solution goes here with the next free number and gets a row in
  the matching tier table. No subfolders except `ref/archive/` (superseded older AI versions;
  don't use unless asked).

## What we know about the interview

- NG / IC1 / IC2 SWE. Screens: two back-to-back online technical rounds, LeetCode medium–hard,
  C++ on CoderPad (C++17), no DP expected, no AI. At least 2 min of clarifying questions, explain
  the approach before coding, then write and run his own tests.
- Asked in his own rounds: word pairs within k (passed) and the SnowCal command interpreter —
  only his `prep/` versions exist.
- Tier order is a prep judgment from the collected reports, not a measured frequency.

## How to run a session

1. Pick the next problem in tier order (high-frequency → recommended → optional). Skip rows
   that already have a `prep/` file unless he asks to redo one.
2. One problem at a time: a very concise derivation (which data structure, why it is
   efficient) → T/S for the crucial functions → the full code.
3. "check" = review his file in `prep/`: compile with `g++ -std=c++17 -Wall -Wextra`, run its
   own tests, and report only errors (say whether it is a compile failure or a test failure).
   No praise, no summaries. Never edit his file — show the fix in chat and he types it.
4. Don't run tests for solutions you only show in chat; he tests them himself.

## Code conventions (`ref/` files and any code shown in chat)

- One runnable file: header comment stating the problem, includes, `using std::x;` for every
  type and function used (no `std::` in the body), solution, `test(label, input..., want)`, `main`.
- Main function = the most straightforward passable solution. Add a second function only for a
  substantially better one, and test both in the same `test`.
- Comment above each function: approach + `T: O(...), S: O(...)` (graphs in V+E). Comment what
  each key data structure stores. Adjacency list is `graph`; BFS/DFS use `cur` / `nxt`; DFS
  helper is `dfs`; matrix size is `m` / `n`; class data members go at the top.
- Tests: few, hand-made, high coverage, never LeetCode's examples. Labels short, words joined
  with `-`. No comments on test cases. Print only `label PASS` / `label FAIL`:
  `cout << label << (ok ? " PASS\n" : " FAIL\n");`.
- No mutation inside a condition (`if (--indeg[v] == 0)`, `if (s.erase(x))`); the only
  exception is `insert(...).second`. No `<sstream>`. Keep lines short (move trailing comments
  above the line).
- Minimal: implement only what the statement requires. Ignore hack2hire "insider tips".
- Grids: separate `dr[]` and `dc[]` arrays indexed by `d` in `0..3`; no 5-element direction trick.

## Tiers

The `prep/` column names his file(s) for that problem (names differ from `ref/`; — = not
written yet). Run a ref file with
`g++ -std=c++17 -Wall -Wextra -g -fsanitize=address,undefined ref/NN-slug.cpp -o a && ./a`.

### High-frequency — must do

His versions only:

| Problem | `prep/` |
|---|---|
| Word pairs within k | `word-pair-within-k.cpp` |
| SnowCal command interpreter | `command-interpreter.cpp` |

| # | `ref/` 文件 | 题族 | 证据 | LC | `prep/` |
|---|---|---|---|---|---|
| 01 | `01-grid-shortest-path` | A07 障碍网格最短路（+ 返回路径） | 明确 NG 原帖 + NG 标签 | 1091 原型 | `shortest-path`（+ followup） |
| 02 | `02-nearest-two-distance` | A08 每个 1 到最近 2（+ 全局最近一对） | 明确 NG 原帖 + NG 标签 | 821 / 243 | `nearest-two`, `array-nearest-distance`（+ followup） |
| 03 | `03-forest-delete-reindex` | A02 parent 数组删除并重新编号 | NG 标签 | 1110 弱原型 | `delete-node`, `forest-delete-node` |
| 04 | `04-dag-privilege` | A09 DAG 权限并集（+ 有序合并） | NG 标签 | 210 / 2192 | `priviledge-inherit`, `dag-privilege`（+ followup） |
| 05 | `05-course-schedule-ii` | A13 Course Schedule II | NG 标签 | 210 | `course-schedule` |
| 06 | `06-tree-boundary` | A14 二叉树逆时针边界（+ 求和） | NG 标签 | 545 | `tree-boundary` |
| 07 | `07-tree-height-after-deletes` | B01 删除后 N 叉树高度 | 职级未确认，两轮 coding | 559 原型 | `delete-nodes`, `tree-height-deleted-nodes`（+ followup） |
| 08 | `08-connect-four` | B03 当前落子是否四连（+ 重力落子） | 职级未确认，两轮 coding | 348 原型 | `winning-move` |
| 09 | `09-kv-store-ttl` | B04 KV CRUD → TTL → 按时间列出有效项 | 职级未确认，两轮店面 | 706 / 1797 | `ttl-kv`, `kv-store-ttl` |
| 10 | `10-encode-decode-strings` | B05 Encode / Decode Strings | 职级未确认，phone screen | 271 | `encode-string`, `encode-decode-strings` |
| 11 | `11-in-memory-file-system` | B07 内存文件系统 | 职级未确认，phone screen | 588 | `file-system` |
| 12 | `12-word-search-ii` | B16 Word Search II | 职级未确认，两轮 phone screen | 212 | `word-search` |

### Recommended

| # | `ref/` 文件 | 题族 | 证据 | LC | `prep/` |
|---|---|---|---|---|---|
| 13 | `13-parallel-courses` | A05 最少学期（+ 异时长） | NG 标签 | 1136 / 2050 | `parallel-courses`（+ followup） |
| 14 | `14-throne-inheritance` | A11 王位继承 | NG 标签 + IC1 二手变体 | 1600 | `throne-inheritance` |
| 15 | `15-top-k-unique-searches` | A12 按独立用户数 Top-K 搜索词 | NG 标签 | 347 / 692 原型 | — |
| 16 | `16-task-executor-dedup` | A19 优先级任务执行器，重复 ID 只执行一次 | 作者评论 IC2 | 3408 部分原型 | `task-executor-dedup` |
| 17 | `17-dag-reachability` | A15 DAG 可达集合 | IC2 标签 | 2192 反向 | `ancestor-list` |
| 18 | `18-subtree-sum-tree` | A06 子树和构建新树 | NG 标签 | 1339 原型 | `subtree-sum-tree` |
| 19 | `19-binary-search-queries` | A04 二分查找 + 重复查询 | NG 标签 | 704 / 34 | `binary-search-queries`（+ followup） |
| 20 | `20-web-crawler` | A01 Web Crawler 最少点击（+ 路径） | IC1 | 1236 / 127 原型 | `web-crawler`（+ followup） |
| 44 | `44-hit-counter` | E01 Hit Counter | IC1 / IC2 | 362 | `hit-count`, `hit-counter-followup` |
| 45 | `45-log-storage` | E01 按粒度检索日志 | IC1 / IC2 | 635 | `log-storage`（+ followup） |
| 46 | `46-subtree-delete-depth` | 最少删除使 N 叉树深度达标 | J12962 | 559 原型 | `subtree-delete-depth` |
| 47 | `47-bipartition` | 二分图分组 | handout | 886 | `bipartition` |
| 48 | `48-basic-calculator` | 带括号和一元负号的计算器 | handout | 224 | `basic-calculator`（+ followup） |
| 49 | `49-tree-directions` | 二叉树两节点间的方向串 | LC tag | 2096 | `tree-directions` |
| 50 | `50-task-manager` | Task Manager | handout | 3408 | `task-manager` |
| 51 | `51-median-stream` | 数据流中位数 | handout | 295 | `median-stream` |
| 52 | `52-top-k-frequent` | Top-K 高频元素 | LC tag | 347 | `top-k-frequent`（+ followup） |
| 53 | `53-min-stack` | Min Stack | LC tag | 155 | `min-stack` |
| 54 | `54-most-frequent-ids` | 每步后的最高频 ID | LC tag | 3092 | `most-frequent-ids` |
| 55 | `55-simplify-path` | Unix 路径规范化 | LC tag | 71 | `simplify-path` |
| 56 | `56-trapping-rain-water` | 接雨水 | LC tag | 42 | `trapping-rain-water` |
| 57 | `57-quad-tree` | 构建四叉树 | LC tag | 427 | `quad-tree` |
| 58 | `58-vowel-substring` | 含全部五个元音的子串计数 | LC tag | 2062 | `vowel-substring` |
| 59 | `59-happy-number` | 快乐数 | J10271 | 202 | `happy-number` |

### Optional

| # | `ref/` 文件 | 题族 | 证据 | LC | `prep/` |
|---|---|---|---|---|---|
| 21 | `21-acl-allow-deny` | A18 多重继承 ACL allow / deny | IC1 二手摘编 | 210 / 2192 原型 | — |
| 22 | `22-lru-cache-resize` | B10 可 resize 的 LRU | 职级未确认（Hack2Hire） | 146 | `lru-cache-resize` |
| 23 | `23-top-k-book-sales` | B12 累计销量 Top-K | 职级未确认（Hack2Hire） | 347 / 692 原型 | `top-k-book-sales` |
| 24 | `24-rate-limiter` | B15 滑动窗口限流（+ 多规则） | 职级未确认 | 359 / 362 原型 | — |
| 25 | `25-meeting-rooms-bus` | I01 Meeting Rooms II（+ 巴士复用） | Intern 标签 | 253 / 252 | `meeting-rooms`（+ followup） |
| 26 | `26-copy-random-list` | I05 复制带随机指针的链表 | Intern 标签 | 138 | — |
| 27 | `27-time-map-out-of-order` | B24 TimeMap 乱序时间戳 | 职级未确认 P0 | 981 | `time-map`（+ followup） |
| 28 | `28-event-store` | B06 事件流保留最近 m 条 | 职级未确认 P0 | 635 / 347 部分原型 | `event-store`（+ followup） |
| 29 | `29-merge-intervals` | B08 合并区间 | 职级未确认 P0 | 56 | — |
| 30 | `30-valid-parentheses` | B09 括号匹配 | 职级未确认 P0 | 20 | — |
| 31 | `31-find-anagrams` | B17 找所有字母异位词 | 职级未确认 P0 | 438 | — |
| 32 | `32-rotated-array-min` | B18 旋转数组最小值（+ 有重复） | 职级未确认 P0 | 153 / 154 | `rotated-array-min` |
| 33 | `33-reverse-alnum-runs` | B20 反转连续字母数字段 | 职级未确认 P0 | 344 原型 | `reverse-alnum-runs` |
| 34 | `34-tax-brackets` | B22 分段税档 | 职级未确认 P0 | 2303 | `tax-brackets` |
| 35 | `35-course-retry` | A17 课程发现与失败重试 | P1（IC2） | 210 原型 | — |
| 36 | `36-overpay-coins` | A10 允许多付找零的最少硬币 | P1（NG） | 322 仅原型 | — |
| 37 | `37-max-throughput` | B11 预算内最大化最小吞吐 | P1 | 875 / 2064 原型 | `max-throughput` |
| 38 | `38-query-profit` | B14 只选一种 query 的最大利润 | P1 | 无强映射 | `query-profit` |
| 39 | `39-work-schedule` | B13 补齐七天工时 | P1 | 39 / 216 原型 | `work-scheduler` |
| 40 | `40-blacklist-stream` | B19 动态黑名单过滤 | P1 | 706 基础 | `blacklist-stream` |
| 41 | `41-integer-ops-path` | B21 +2 / ×2 / ÷2 任意路径 | P1 | 397 弱原型 | `integer-ops-path` |
| 42 | `42-subtree-average` | B23 子树平均值的平均 | P1 | 2265 原型 | `subtree-average`（+ followup） |
| 43 | `43-transaction-kv` | I02 事务 KV（嵌套 begin / commit / rollback） | P1 | 706 / 1146 部分原型 | `transaction-kv`（+ followup） |

## Contract choices to clarify first

The source reports leave these open; the `ref/` code picks one. Ask before coding:

- **09 TTL KV**：`update` 不续期，`ttl <= 0` 表示永不过期，`now < expireAt` 才算有效。
- **16 任务执行器**：优先级大的先执行，同优先级时间戳早的先，再按 ID 小的先。
- **21 ACL**：子节点只继承父节点的最终权限集合，自己的 allow 可以重新授予。
- **28 事件流**：时间戳相同时先到的记录先被淘汰。
- **34 税档**：超过最高档的收入是否征税，由构造参数切换。
- **42 子树平均**：除法向零截断；向下取整版本是 follow-up。
