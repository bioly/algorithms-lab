# 02 - Count the Farmer's Fields

| | |
|---|---|
| **Pattern** | Flood fill (DFS/BFS on a grid), connected components |
| **Difficulty** | Medium |
| **Solution** | [`count_fields.cpp`](count_fields.cpp) |
| **Tests** | [`count_fields_test.cpp`](count_fields_test.cpp) |
| **Trace demo** | [`count_fields_trace.cpp`](count_fields_trace.cpp) |

## The problem

The map uses `1` for good land and `0` for bad land. A **field** is a group of `1` cells connected **up, down, left or right**. Diagonal cells do **not** count as connected. How many separate fields are there?

```
1 1 0 0 0
1 1 0 0 1        -> 3 fields
0 0 1 0 1
0 0 0 1 1
```

The three fields are the 2x2 block at the top left, the group on the right (rows 1 to 3), and the single cell at row 2, column 2. That last cell touches the right group only diagonally, so it is its own field.

## Clarifying questions

- May I modify the input? (We assume not, so we use a separate `visited` matrix.)
- How big can the grid be? (This matters for recursion depth.)
- What does "connected" mean? (4 directions, not diagonals.)

## The key idea: a surveyor with a paint bucket

1. Walk over the map cell by cell.
2. When you meet **unpainted land**, you have found a **new field**. Add 1 to your count.
3. Immediately **paint the whole field** by spreading to every connected land cell. This is called a flood fill.
4. Keep walking. When you later meet land that is already painted, it belongs to a field you counted, so skip it.

The paint is the `visited` matrix. It is what prevents counting a field twice.

## Watch it run

Legend: `.` water, `#` land not painted yet, a letter is a painted field, `[x]` is the cell being processed. Run the trace demo for the full output.

```
MAP
  #  #  .  .  .
  #  #  .  .  #
  .  .  #  .  #
  .  .  .  #  #
```

**Scan hits (0,0): new land, so field A, count = 1.** Flood it:

```
pop (0,0): pushes (1,0) and (0,1)      pop (0,1): pushes (1,1)
 [A] A  .  .  .                          A [A] .  .  .
  A  #  .  .  #                          A  A  .  .  #
  stack: (1,0) (0,1)                     stack: (1,0) (1,1)

pop (1,1): nothing new                  pop (1,0): nothing new, stack empty
  A  A  .  .  .                          A  A  .  .  .
  A [A] .  .  #                         [A] A  .  .  #
  stack: (1,0)                           stack: (empty -> field A finished)
```

The scan then meets (0,1), (1,0) and (1,1) and prints, for each one:

```
SCAN (0,1): land, but already part of field A -> skip, NOT counted again
```

This is the key moment. The cells are land, but they are already painted, so the count stays at 1.

**Scan hits (1,4): new land, so field B, count = 2.** The flood follows a path down and around the corner:

```
pop (1,4) -> push (2,4)    pop (2,4) -> push (3,4)    pop (3,4) -> push (3,3)    pop (3,3) -> done
 A  A  .  . [B]             A  A  .  .  B              A  A  .  .  B             A  A  .  .  B
 A  A  .  .  B             .  .  #  . [B]              .  .  #  .  B             .  .  #  .  B
 .  .  #  .  B             .  .  .  B  B               .  .  .  B [B]            .  .  . [B] B
```

**Scan hits (2,2): new land, so field C, count = 3.** It has no land neighbors up, down, left or right (diagonals don't count), so it finishes immediately:

```
 A  A  .  .  .
 A  A  .  .  B
 .  . [C] .  B      stack: (empty -> field C finished)
 .  .  .  B  B
```

The remaining cells (2,4), (3,3) and (3,4) are skipped because they are already painted B. The result is **3 fields**.

## Complexity

- **Time: O(rows x cols).** The scan visits each cell once. The flood visits each land cell once, because every cell is marked before it is pushed, so it can only be pushed once. Each cell checks 4 neighbors, which is a constant.
- **Space: O(rows x cols).** The `visited` matrix plus the stack. In the worst case the stack holds a large part of the grid.

## Tests

Empty grid, `{{}}`, all water, a single `1`, a 2x2 block (1 field), a diagonal pair `{{1,0},{0,1}}` (2 fields), a checkerboard (5 fields), the example above (3 fields), a snake-shaped single field, an input-not-modified check and a 2000x2000 all-ones grid.

## Interview talking points

- **Marking on push.** If you mark when popping, the same cell can be pushed several times by different neighbors, which wastes memory.
- **Explicit stack vs recursion.** Recursion is shorter and fine for small grids. On a big all-ones grid it recurses millions of levels deep and overflows the call stack. Say that this is why the explicit stack was chosen.
- **DFS vs BFS.** Swap the stack for a `std::queue` and you get BFS. The count is identical, because we only care which cells are connected, not the visiting order.
- **Mutating the input.** Writing `0` into `grid` as you visit removes the `visited` matrix. It is a valid trade-off, but ask first.
- **A changing map.** If land is added over time, the standard follow-up is Union-Find (Disjoint Set Union).

## How to run

From the repository root (after `cmake --preset debug`):

```bash
cmake --build --preset debug --target count_fields_test count_fields_trace
ctest --preset debug -L count_fields                      # run this challenge's tests
./build/debug/challenges/02_count_fields/count_fields_trace   # watch the algorithm step by step
```
