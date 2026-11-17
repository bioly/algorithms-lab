# 03 - The Farmer's Longest Clean Row

| | |
|---|---|
| **Pattern** | Sliding window (two pointers) |
| **Difficulty** | Medium |
| **Solution** | [`longest_unique_run.cpp`](longest_unique_run.cpp) |
| **Tests** | [`longest_unique_run_test.cpp`](longest_unique_run_test.cpp) |
| **Trace demo** | [`longest_unique_run_trace.cpp`](longest_unique_run_trace.cpp) |

## The problem

The farmer plants a single row of crops. Each position holds one crop, written as a letter. If the same crop appears twice in a stretch, its pests spread between the two plants. Find the **longest continuous stretch with no crop repeated** and return its length.

```
row = "abcbdea"   ->   5   (the stretch "cbdea")
```

More examples: `"abcabcbb"` gives 3, and `"aaaa"` gives 1.

## Clarifying questions

- Which characters can appear? Lowercase only, ASCII or Unicode? (This decides the lookup structure. We assume plain `char`.)
- Can the row be empty? (Return 0.)
- Return the length or the stretch itself? (The length.)
- How long can the row be? (This rules out O(n^2) for rows over about 10^5.)

## Brute force

Try every start and end and check each stretch for repeats. That is O(n^3) naively, or O(n^2) with a set. Too slow for long rows.

## The key idea: a sliding window

Imagine a frame sliding along the row, with a left edge and a right edge. The rule is:

> **The frame always contains no repeats.**

1. Move the right edge one step at a time and look at the new crop.
2. If it **does not repeat** inside the frame, the frame grows.
3. If it **repeats** something inside the frame, move the left edge to just **past the earlier copy**.
4. After each step, the frame length is a candidate for the answer.

The left edge **only moves right** and never goes back, so the whole row is processed in one pass. To jump the left edge in one move, we remember the **last position where each crop was seen** (`last_seen`).

## Watch it run on `"abcbdea"`

`R` is the right edge, `^` marks the other cells inside the frame. Run the trace demo for the full output.

**Steps 1 to 3: the frame grows.**
```
 a  b  c  b  d  e  a          a  b  c  b  d  e  a          a  b  c  b  d  e  a
 R                             ^  R                         ^  ^  R
 "a"  len=1                    "ab"  len=2                  "abc"  len=3   best=3
```

**Step 4: a repeat appears inside the frame.**
```
'b' last seen at 1 >= left(0) -> repeat INSIDE window -> left = 2

 a  b  c  b  d  e  a
       ^  R              window = "cb"  len=2   best=3
```

**Steps 5 and 6: the frame grows again.**
```
 a  b  c  b  d  e  a          a  b  c  b  d  e  a
       ^  ^  R                       ^  ^  ^  R
 "cbd"  len=3   best=3         "cbde"  len=4   best=4
```

**Step 7: the trap.**
```
'a' last seen at 0 < left(2) -> old sighting is OUTSIDE window -> ignore, left stays 2

 a  b  c  b  d  e  a
       ^  ^  ^  ^  R     window = "cbdea"  len=5   best=5
```

There **is** an earlier `a`, at index 0, but it is **outside** the frame because the left edge is already at 2. So there is no real repeat. This is why the code checks `last_seen[ch] >= left` and not just "have I seen it before?".

The result is **5**.

## Complexity

- **Time: O(n).** `right` moves once across the row and `left` only ever moves forward, so there are at most 2n moves.
- **Space: O(1).** A fixed 256-entry table. In general it is O(alphabet size).

## Tests

| Input | Expected | What it checks |
|---|---|---|
| `""` | 0 | empty row |
| `"a"` | 1 | smallest non-empty |
| `"aaaa"` | 1 | the window never grows |
| `"abcdef"` | 6 | no repeats, so the whole row |
| `"abcabcbb"` | 3 | the classic example |
| `"pwwkew"` | 3 | the answer is in the middle |
| `"abcbdea"` | 5 | the stale-entry trap from step 7 |
| `"abba"` | 2 | `left` must never move backwards |
| bytes >= 128 | 3 | the `unsigned char` cast |
| 1,000,000 x `'x'` | 1 | a long row, still fast |
| 1000 random rows | same as brute force | fast agrees with slow |

`"abba"` is a good interview test. After `abb`, `left` is 2. The final `a` was last seen at index 0, **before** `left`. Without the `>= left` check the code moves `left` back to 1 and reports a wrong answer.

## Interview talking points

- **Recognizing the pattern.** "Longest or shortest contiguous stretch with some property" means: try a sliding window first.
- **Proof of O(n).** Both pointers only move forward, so there are at most 2n moves in total.
- **Alternative with a set.** Keep a `std::unordered_set` of the window and shrink with a `while` loop. It is easier to explain but needs hashing. The `last_seen` table jumps `left` in one step.
- **`std::string_view`.** It accepts a `std::string`, a literal or a `const char*` without copying.
- **Follow-ups.** "At most K distinct crops" uses the same window with a counter map. "Return the stretch itself" needs you to store `left` whenever `best` improves.

## How to run

From the repository root (after `cmake --preset debug`):

```bash
cmake --build --preset debug --target longest_unique_run_test longest_unique_run_trace
ctest --preset debug -L longest_unique_run                      # run this challenge's tests
./build/debug/challenges/03_longest_unique_run/longest_unique_run_trace   # watch the algorithm step by step
```
