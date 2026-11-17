# 05 - The Harvester Speed

| | |
|---|---|
| **Pattern** | Binary search on the answer |
| **Difficulty** | Medium |
| **Solution** | [`min_harvest_speed.cpp`](min_harvest_speed.cpp) |
| **Tests** | [`min_harvest_speed_test.cpp`](min_harvest_speed_test.cpp) |
| **Trace demo** | [`min_harvest_speed_trace.cpp`](min_harvest_speed_trace.cpp) |

## The problem

The farmer has several fields, and field `i` holds `fields[i]` kg of crops. His harvester works at a fixed speed of `k` kg per hour, and he picks `k` once. In one hour it works on **one field only**. If the field has less than `k` kg left, the rest of that hour is wasted. He has `h` hours before the frost. What is the **smallest whole-number speed** that finishes everything in time?

```
fields = [3, 6, 7, 11], h = 8     ->   4
```

At speed 4 the fields take 1 + 2 + 2 + 3 = 8 hours, which just fits. At speed 3 they take 1 + 2 + 3 + 4 = 10 hours, which is too slow.

## Clarifying questions

- Is `h >= number of fields` guaranteed? Each field needs at least 1 hour, so otherwise it is impossible. (We assume yes.)
- Is the speed a whole number? Can a field be empty? (Whole numbers, each field at least 1 kg.)
- How big can the numbers be? With up to 10^9 kg per field and many fields, the total hours can overflow `int`, so we use a 64-bit counter.

## Brute force

Try speed 1, 2, 3, ... and for each speed add up the hours. That is O(max pile x n), far too slow when piles are around 10^9.

## The key idea: binary search on the answer

There is no sorted array here. But look at the hours needed for every possible speed (real output from the trace demo):

```
speed:  1  2  3  4  5  6  7  8  9 10 11
hours: 27 15 10  8  8  6  5  5  5  5  4
ok?  :  N  N  N  Y  Y  Y  Y  Y  Y  Y  Y      (ok = hours <= 8)
```

If a speed is fast enough, **every faster speed is too**. So the pattern is always a block of `N` followed by a block of `Y`, and we want the **first Y**.

Whenever you have a yes/no test that flips exactly once, you can binary search for the flip point, even though no array exists. Each check "does speed `mid` work?" is one O(n) pass over the fields, and binary search needs only about log2(max pile) checks.

The range runs from `lo = 1` to `hi = max pile`. At the max-pile speed every field takes exactly 1 hour, so it works, and going faster gains nothing.

## Watch it run

`L` is lo, `H` is hi, `M` is mid, `--` marks the range still in play.

**Iteration 1: lo=1, hi=11, mid=6.**
```
speed:  1  2  3  4  5  6  7  8  9 10 11
range: -- -- -- -- -- -- -- -- -- -- --
mark :  L              M              H
hours at 6: 1 + 1 + 2 + 2 = 6      6 <= 8 -> WORKS -> hi = 6
```
6 works, so the answer is 6 **or something smaller**. Keep 6 in the range and throw away everything above it.

**Iteration 2: lo=1, hi=6, mid=3.**
```
range: -- -- -- -- -- --
mark :  L     M        H
hours at 3: 1 + 2 + 3 + 4 = 10     10 > 8 -> too slow -> lo = 4
```
3 fails, and so does everything slower. Throw away speeds 1 to 3.

**Iteration 3: lo=4, hi=6, mid=5.**
```
range:          -- -- --
mark :           L  M  H
hours at 5: 1 + 2 + 2 + 3 = 8      8 <= 8 -> WORKS -> hi = 5
```

**Iteration 4: lo=4, hi=5, mid=4.**
```
range:          -- --
mark :           M  H
hours at 4: 1 + 2 + 2 + 3 = 8      8 <= 8 -> WORKS -> hi = 4
```

Now `lo == hi == 4`, so the answer is **4**. The search took 4 checks instead of up to 11.

## The template: memorize this shape

Almost every "find the smallest X such that ..." problem fits this skeleton:

```cpp
while (lo < hi) {
    int mid = lo + (hi - lo) / 2;
    if (Works(mid)) hi = mid;      // keep mid, it could be the answer
    else            lo = mid + 1;  // discard mid and everything below it
}
return lo;
```

The two lines `hi = mid` and `lo = mid + 1` are the part people get wrong. They must not both move past `mid`, or the loop can run forever or skip the answer.

For the "largest X such that ..." variant, flip it: use `mid = lo + (hi - lo + 1) / 2`, then `if (Works(mid)) lo = mid; else hi = mid - 1;`. The `+ 1` rounds the midpoint up, so `lo = mid` always makes progress.

## Complexity

- **Time: O(n log M)** where `M` is the largest pile. There are about log2(M) iterations, and each is an O(n) pass.
- **Space: O(1).**

## Tests

| Input | Expected | What it checks |
|---|---|---|
| `[3,6,7,11], h=8` | 4 | the main example |
| `[30,11,23,4,20], h=5` | 30 | hours equal fields, so the max pile |
| `[30,11,23,4,20], h=6` | 23 | one spare hour |
| `[7], h=7` | 1 | one field, plenty of time |
| `[7], h=1` | 7 | one field, one hour |
| `[1,1,1,1], h=100` | 1 | the lower bound |
| two fields of 10^9, `h=2` | 10^9 | large values |
| 100,000 fields of 10^9, `h=10^9` | 100,000 | overflow stress |
| 1500 random inputs | same as brute force | fast agrees with slow |

For the overflow row: each field may take at most 10^9 / 10^5 = 10^4 hours, so the speed must be at least 10^9 / 10^4 = 100,000.

## Interview talking points

- **How to spot it.** Look for "minimum X such that ...", "maximum possible minimum ...", or "fewest days/capacity/speed". Then ask: "If I guess a value, can I cheaply check it? And does feasibility only ever flip once?"
- **Prove monotonicity out loud.** Say "a faster harvester can never do worse". Interviewers want to hear that reasoning.
- **Pitfalls.** Off-by-one between `hi = mid` and `lo = mid + 1`, overflow in `(lo + hi) / 2`, integer ceiling division, and too small a type for the hour total.
- **Same pattern, other problems.** "Ship packages within D days" (capacity), "split an array into k parts with the smallest largest sum", and "place cows/routers as far apart as possible". Only `Works()` changes.
- **The classic version.** Searching a sorted array is the same loop. `std::lower_bound` finds the first element `>= x`, which is the first `Y` in a sorted `N...NY...Y` pattern.

## How to run

From the repository root (after `cmake --preset debug`):

```bash
cmake --build --preset debug --target min_harvest_speed_test min_harvest_speed_trace
ctest --preset debug -L min_harvest_speed                      # run this challenge's tests
./build/debug/challenges/05_min_harvest_speed/min_harvest_speed_trace   # watch the algorithm step by step
```
