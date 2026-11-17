# 04 - Merging the Planting Schedules

| | |
|---|---|
| **Pattern** | Intervals: sort, then sweep |
| **Difficulty** | Medium |
| **Solution** | [`merge_schedules.cpp`](merge_schedules.cpp) |
| **Tests** | [`merge_schedules_test.cpp`](merge_schedules_test.cpp) |
| **Trace demo** | [`merge_schedules_trace.cpp`](merge_schedules_trace.cpp) |

## The problem

The farmer has a list of planting schedules. Each one says "this field is in use from day `start` to day `end`". Many overlap, because different crews booked the same stretch of time. Produce **one clean list of non-overlapping blocks** that covers exactly the same days.

```
[8,10] [1,3] [2,6] [3,4] [6,7] [15,18] [17,20]
                 |
                 v
        [1,7]  [8,10]  [15,20]
```

Another example: `[1,3] [3,5]` becomes `[1,5]`, because they share day 3.

## Clarifying questions

- Do schedules that only **touch** (`[1,3]` and `[3,5]`) count as overlapping? (We assume yes.)
- Do **adjacent** days (`[1,3]` and `[4,5]`) merge? (We assume no. They share no day.)
- Is the input sorted? Can it be empty? Is `start <= end` guaranteed?
- May I modify the input, or should I return a new list?

## Brute force

Compare each schedule with every other one, merge if they overlap, and repeat until nothing changes. That is O(n^2) or worse and fiddly to get right.

## The key idea: sort first

Sort the schedules by **start day**. After sorting, the schedules that can overlap a block are always the ones **right after it**. So you only ever compare the next schedule with the **last block you built**:

- **It starts before or on the last block's end?** They overlap or touch, so stretch the last block's end.
- **It starts after the last block's end?** There is a gap, so it begins a new block.

Sorting turns "compare everything with everything" into "walk once from left to right".

## Watch it run

Input schedules on a day line (`|===|` is one schedule). Run the trace demo for the full output.

```
  [8,10]               |===|
  [1,3]    |===|
  [2,6]      |=======|
  [3,4]        |=|
  [6,7]              |=|
  [15,18]                          |=====|
  [17,20]                              |=====|
```

Sorted by start day: `[1,3] [2,6] [3,4] [6,7] [8,10] [15,18] [17,20]`. The first block is `[1,3]`.

**Step 1: next = `[2,6]`, last block = `[1,3]`.**
```
  2 <= 3  -> overlap -> new end = max(3,6) = 6          blocks: [1,6]
```

**Step 2: next = `[3,4]`, last block = `[1,6]`. This is the trap.**
```
  last   |=========|
  next       |=|
  3 <= 6  -> overlap -> new end = max(6,4) = 6          blocks: [1,6]
```
`[3,4]` sits fully inside the block. If the code wrote `last.end = cur.end`, the block would shrink from 6 to 4. This is why the code uses `max`.

**Step 3: next = `[6,7]`, last block = `[1,6]`.**
```
  6 <= 6  -> touch -> new end = max(6,7) = 7            blocks: [1,7]
```
They share day 6, so they merge. This is the `<=` (not `<`) decision.

**Step 4: next = `[8,10]`, last block = `[1,7]`.**
```
  8 > 7   -> GAP -> start a new block                   blocks: [1,7] [8,10]
```

**Step 5: next = `[15,18]`, last block = `[8,10]`.**
```
  15 > 10 -> GAP -> start a new block                   blocks: [1,7] [8,10] [15,18]
```

**Step 6: next = `[17,20]`, last block = `[15,18]`.**
```
  17 <= 18 -> overlap -> new end = max(18,20) = 20      blocks: [1,7] [8,10] [15,20]
```

Final result:

```
  [1,7]    |===========|
  [8,10]                 |===|
  [15,20]                              |=========|
```

## Complexity

- **Time: O(n log n).** Sorting dominates. The merge pass is O(n).
- **Space: O(n).** One output vector. In place, you can reach O(1) extra space by writing merged blocks back into the sorted input, if the interviewer allows modifying it.

## Tests

| Input | Expected | What it checks |
|---|---|---|
| empty | empty | no crash |
| `[1,4]` | `[1,4]` | a single schedule |
| `[1,3] [2,6] [8,10] [15,18]` | `[1,6] [8,10] [15,18]` | the classic example |
| `[1,3] [3,5]` | `[1,5]` | touching counts as overlap |
| `[1,2] [3,4]` | unchanged | adjacent days stay separate |
| `[1,10] [2,3] [4,5]` | `[1,10]` | nested, so `max` is required |
| `[5,6] [1,2] [3,4]` | `[1,2] [3,4] [5,6]` | unsorted input |
| `[2,3]` three times | `[2,3]` | duplicates |
| 1500 random inputs | same as brute force | fast agrees with slow |

## Interview talking points

- **Recognizing the pattern.** Words like "overlapping", "meetings", "schedules" or "ranges" mean: sort by start, then sweep.
- **The `<=` vs `<` decision.** Say it out loud. If adjacent integer days should also merge (`[1,3]` and `[4,5]`), use `cur.start <= last.end + 1`.
- **Common bugs.** Forgetting to sort, overwriting `end` instead of using `max`, and holding a reference that a reallocation invalidates. Here `reserve` prevents the reallocation, and `push_back` only happens after `last` is no longer used.
- **Pass by value.** The function takes the vector by value so it can sort its own copy. Callers can `std::move` to avoid the copy.
- **Follow-ups.** "Insert a new interval into an already merged list" is O(n) with no sort. "Minimum number of rooms for all meetings" uses the same sort plus a min-heap. "Can one person attend all meetings?" is a sort and a check for any overlap.

## How to run

From the repository root (after `cmake --preset debug`):

```bash
cmake --build --preset debug --target merge_schedules_test merge_schedules_trace
ctest --preset debug -L merge_schedules                      # run this challenge's tests
./build/debug/challenges/04_merge_schedules/merge_schedules_trace   # watch the algorithm step by step
```
