# 01 - Maximal Square

| | |
|---|---|
| **Pattern** | Dynamic programming on a grid |
| **Difficulty** | Medium |
| **Solution** | [`maximal_square.cpp`](maximal_square.cpp) |
| **Tests** | [`maximal_square_test.cpp`](maximal_square_test.cpp) |
| **Trace demo** | [`maximal_square_trace.cpp`](maximal_square_trace.cpp) |

## The problem

The farmer wants to farm the biggest **square** of good land. The land is a matrix where `1` is good land and `0` is bad land. Return the **area** of the biggest square made only of `1`s.

```
0 1 1 1 0
1 1 1 1 1        -> biggest square is 3x3 -> answer 9
0 1 1 1 0
```

Rows of ones don't count, because they are rectangles. A square needs equal width and height.

## Clarifying questions (ask these first)

- Is the input rectangular, and can it be empty? (Assume rectangular and handle empty.)
- Return the area or the side length? (The area.)
- How big can the matrix be? (This decides whether O(rows x cols) is needed.)
- Do we need the position of the square too? (Not here. See the follow-ups.)

## Brute force (mention it, don't code it)

Treat every cell as a top-left corner and grow the square while everything inside is `1`. Each growth step re-checks cells, so the total is roughly O(rows x cols x min(rows, cols)^2). It re-checks the same cells again and again.

## The key idea

Instead of asking where a square **starts**, ask where it **ends** (its bottom-right corner). For each cell, remember one number:

> `dp` = the side of the biggest all-ones square whose bottom-right corner is this cell.

- If the cell is `0`, no square can end here, so `dp = 0`.
- If the cell is `1`, look at three neighbors, **above**, **left** and **diagonal up-left**. Take the **smallest** of their numbers and add 1.

```
 diagonal | above
 ---------+------
  left    | THIS CELL
```

**Why the smallest?** A 3x3 square ending here needs all three neighbors to already hold a square of side at least 2. If even one neighbor only manages side 1, the whole thing is limited to 2. The weakest neighbor decides.

### Saving memory

To fill row `r` we only need row `r-1`, so we keep **one row** and overwrite it. The catch is that overwriting a cell loses its old value, and the next cell to the right needs that old value as its "diagonal". So we save it in a variable first (`above`, which then becomes `diagonal`).

## Watch it run

The grid is the example above. `[x]` is the cell being processed. Run the trace demo for the full output with every step and the one-row memory.

```
Row 0 (nothing above, every 1 becomes 1)
[0] .  .  .  .       0 [1] .  .  .       0  1 [1] .  .       0  1  1 [1] .       0  1  1  1 [0]

Row 1 (squares start to grow)
 0  1  1  1  0        0  1  1  1  0       0  1  1  1  0       0  1  1  1  0       0  1  1  1  0
[1] .  .  .  .        1 [1] .  .  .       1  1 [2] .  .       1  1  2 [2] .       1  1  2  2 [1]

Row 2 (the 3x3 square appears)
 0  1  1  1  0        0  1  1  1  0       0  1  1  1  0       0  1  1  1  0       0  1  1  1  0
 1  1  2  2  1        1  1  2  2  1       1  1  2  2  1       1  1  2  2  1       1  1  2  2  1
[0] .  .  .  .        0 [1] .  .  .       0  1 [2] .  .       0  1  2 [3] .       0  1  2  3 [0]
```

Three steps worth understanding:

```
Step 7, cell (1,1): the cell is 1, but the answer is only 1
  diag=0  above=1  left=1   -> min=0 -> dp = 0 + 1 = 1
  The diagonal is 0. "The weakest neighbor decides."

Step 8, cell (1,2): the first square of side 2
  diag=1  above=1  left=1   -> min=1 -> dp = 1 + 1 = 2

Step 14, cell (2,3): the winner
  diag=2  above=2  left=2   -> min=2 -> dp = 2 + 1 = 3     *** new best side = 3 ***
```

The one-row memory evolves like this:

| After | `dp[1..5]` | Meaning |
|---|---|---|
| row 0 | `0 1 1 1 0` | the first row of the table |
| row 1 | `1 1 2 2 1` | the second row replaced the first |
| row 2 | `0 1 2 3 0` | the third row replaced the second |

The biggest number is 3, so the area is 3 x 3 = **9**.

## Complexity

- **Time: O(rows x cols).** One pass, each cell visited once.
- **Space: O(cols).** One row. A full table would be O(rows x cols), which is fine to write first and then optimize.

## Tests

| Input | Expected | What it checks |
|---|---|---|
| empty grid | 0 | no crash |
| all zeros | 0 | nothing to farm |
| single `1` | 1 | smallest answer |
| `0 1 1 1 0` (one row) | 1 | a row is not a square |
| 3x5 all ones | 9 | limited by the shorter side |
| 500 random grids | same as brute force | the fast version agrees with a slow, obviously correct one |

## Interview talking points

- `const&` for the input and `size_t` for indices.
- The leading sentinel slot removes all `i-1 < 0` branches.
- If the input is `vector<vector<char>>` with `'0'` and `'1'`, compare with `'1'`. Ask which one it is.
- Overflow: `side^2` fits in `int` unless a side is above about 46,000. For huge grids return `int64_t`.

## Follow-ups interviewers like

1. **Return the coordinates.** Track `(r, c)` whenever `best_side` improves. The top-left corner is `(r - side + 1, c - side + 1)`.
2. **Largest rectangle of 1s.** A harder problem, solved with per-row histograms and a monotonic stack in O(rows x cols).
3. **Without extra space.** Mutating the input gives O(1) extra space, but ask before changing the caller's data.
4. **Matrix too large for memory.** The row-by-row version already works as a stream.

## How to run

From the repository root (after `cmake --preset debug`):

```bash
cmake --build --preset debug --target maximal_square_test maximal_square_trace
ctest --preset debug -L maximal_square                      # run this challenge's tests
./build/debug/challenges/01_maximal_square/maximal_square_trace   # watch the algorithm step by step
```
