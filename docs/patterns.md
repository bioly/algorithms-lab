# Patterns

Challenges grouped by technique. When you meet a new problem, scan the **"Smells like..."** column and try the matching pattern first.

Keep this file up to date when you add a challenge (see [AGENTS.md](../AGENTS.md)). Put each challenge under one primary pattern. If it also uses a second one, mention that in the note.

| Pattern | Smells like... | One-line idea |
|---|---|---|
| [Dynamic programming](#dynamic-programming) | "largest/smallest/number of ways", answers built from smaller answers | Store the answer for each subproblem and reuse it |
| [Graph and grid traversal](#graph-and-grid-traversal) | "connected", "islands", "reachable", "shortest path" | Visit every node once, marking where you have been |
| [Sliding window](#sliding-window) | "longest/shortest contiguous stretch with a property" | Two edges that only move forward |
| [Intervals](#intervals) | "overlapping", "meetings", "ranges", "schedules" | Sort by start, then sweep once |
| [Binary search](#binary-search) | "minimum X such that...", sorted data, a yes/no test that flips once | Halve the search space with each check |

## Dynamic programming

| # | Challenge | Note |
|---|---|---|
| 01 | [Maximal Square](../challenges/01_maximal_square/README.md) | `dp` = side of the biggest square ending at this cell; one-row memory trick |

**How to recognize it:** the answer for a big input is built from answers for smaller inputs, and a brute force would recompute the same things again and again.
**Method:** define exactly what `dp[i]` means in one sentence, write the recurrence, handle the borders, then see whether you only need the last row or two.

## Graph and grid traversal

| # | Challenge | Note |
|---|---|---|
| 02 | [Count the Farmer's Fields](../challenges/02_count_fields/README.md) | Flood fill with an explicit stack; marks on push |

**How to recognize it:** a grid or a network where "neighbors" are defined, and you care about groups, reachability or distance.
**Method:** a `visited` set, a stack (DFS) or queue (BFS), and a loop over all start points. Prefer an explicit stack when depth can be large.

## Sliding window

| # | Challenge | Note |
|---|---|---|
| 03 | [The Longest Clean Row](../challenges/03_longest_unique_run/README.md) | `last_seen` table lets `left` jump; the `>= left` trap |

**How to recognize it:** a contiguous stretch of an array or string, with a property you can maintain as the edges move.
**Method:** grow the right edge, shrink the left edge only when the rule breaks, and record the best size after each step. Both edges only move forward, so the cost is O(n).

## Intervals

| # | Challenge | Note |
|---|---|---|
| 04 | [Merging the Planting Schedules](../challenges/04_merge_schedules/README.md) | Sort by start, compare with the last block; `max` for nested intervals |

**How to recognize it:** ranges of time or position that may overlap.
**Method:** sort by start, then a single sweep that compares each item with the last result. Decide early whether touching counts as overlapping.

## Binary search

| # | Challenge | Note |
|---|---|---|
| 05 | [The Harvester Speed](../challenges/05_min_harvest_speed/README.md) | Search on the answer; `hi = mid` / `lo = mid + 1` template |

**How to recognize it:** a sorted array, or a yes/no question about a guess that goes `NNNN YYYY` (feasibility is monotonic).
**Method:** write `Works(mid)`, pick `lo` and `hi` so the answer is always inside, and keep the invariant on every step.

## Patterns still to come

Heaps and priority queues, tries, stacks and monotonic stacks, backtracking, union-find, topological sort, two pointers on sorted data, prefix sums.
