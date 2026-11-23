# 06 - The K Busiest Fields

| | |
|---|---|
| **Pattern** | Heap (priority queue) of size K |
| **Difficulty** | Medium |
| **Solution** | [`busiest_fields.cpp`](busiest_fields.cpp) |
| **Tests** | [`busiest_fields_test.cpp`](busiest_fields_test.cpp) |
| **Trace demo** | [`busiest_fields_trace.cpp`](busiest_fields_trace.cpp) |

## The problem

Every time a truck leaves a field loaded with crops, the farm office writes down the field's id. At the end of the week the office has a long list of ids such as `3 7 3 5 3 7 9 ...`. The farmer wants to know **which K fields sent out the most loads**.

```
reports = [3, 7, 3, 5, 3, 7, 9, 3, 7, 5, 3, 2, 7, 3],  k = 2
-> [3, 7]       (field 3 had 6 loads, field 7 had 4 loads)
```

The tie rule is our choice: the field with the smaller id wins. The problem says nothing about ties, so in an interview you would ask.

## Clarifying questions

- **Ties.** Which field wins when two have the same count? (Smaller id.)
- **Order.** Must the result be sorted busiest first? (Yes, in this version.)
- **K too big.** What if `k` is larger than the number of distinct fields? (Return them all.)
- **Edge values.** What if `k <= 0`, the list is empty, or ids can be negative? (Empty result, empty result, negative ids are fine.)
- **Size.** How long is the list and how many distinct fields can there be? This decides whether sorting everything is acceptable.

## Brute force

Count the loads per field, put all fields in a list, **sort** it by count and take the first K. With `m` distinct fields that costs O(m log m). It is fine for small data, and you should say so. The weakness is that it **sorts everything just to keep K items**. With `m` = 10 million and K = 3 that is far too much work.

## Quick intro: what is a heap?

A **heap** is a container that is very fast at one thing: always handing you its **most extreme element** while you add and remove items.

- Looking at the extreme element: O(1).
- Adding an element or removing the extreme one: O(log n).

It is **not** fully sorted inside. It only promises that the extreme one is on top. In C++ this container is `std::priority_queue`. By default it puts the **largest** element on top, and this detail causes most of the confusion in this problem.

## The key idea: a bouncer with K seats

Imagine a club with only **K seats**. People arrive one by one, each with a "busyness score".

- While there are free seats, everyone gets in.
- When the club is full and a newcomer arrives, the bouncer compares them with the **weakest person currently inside**. If the newcomer is better, the weakest is thrown out and the newcomer takes the seat. Otherwise the newcomer is sent away.

The bouncer must find the **weakest person inside quickly, every time**. A heap gives exactly that, if it is arranged so the **worst** candidate is on top. After everyone has arrived, the K people inside are the K best.

The plan:

1. **Count** the loads per field with a hash map.
2. Feed the `(field, count)` pairs into a heap that **never holds more than K** entries, and **evict the worst** whenever it grows to K+1.
3. Empty the heap into the result.

**Why it is faster than sorting.** Each heap operation costs O(log K), not O(log m), because the heap never holds more than K+1 items.

### The comparator trick (the confusing bit)

We want the **worst** candidate on top, but `std::priority_queue` shows the "largest" element according to the comparator. So we define "smaller" to mean "ranks higher". Then the "largest" one is whoever ranks **lowest**, which is the worst.

```cpp
// "a is smaller than b"  <=>  "a ranks higher than b"
bool RanksHigher(const Entry& a, const Entry& b) {
    if (a.count != b.count) return a.count > b.count;   // more loads: ranks higher
    return a.field < b.field;                           // tie: smaller id ranks higher
}
```

## Watch it run

Real output from the trace demo. `F3x6` means field 3 with 6 loads. The heap is shown best to worst, and the rightmost entry is the heap's top, the one evicted next.

**Step 1: count the loads.**
```
reports: 3 7 3 5 3 7 9 3 7 5 3 2 7 3     k = 2

   field 2: # (1)
   field 3: ###### (6)
   field 5: ## (2)
   field 7: #### (4)
   field 9: # (1)
```

**Step 2: feed the heap, which holds at most k = 2 entries.**
```
next: F2 with 1 load      push -> size 1
   heap: F2x1                      top (worst) = F2

next: F3 with 6 loads     push -> size 2
   heap: F3x6  F2x1                top (worst) = F2

next: F5 with 2 loads     push -> size 3  -> too big, evict the worst: F2x1
   heap: F3x6  F5x2                top (worst) = F5     (newcomer F5 beats F2)

next: F7 with 4 loads     push -> size 3  -> evict the worst: F5x2
   heap: F3x6  F7x4                top (worst) = F7     (newcomer F7 beats F5)

next: F9 with 1 load      push -> size 3  -> evict the worst: F9x1
   heap: F3x6  F7x4                top (worst) = F7     (the newcomer itself loses)
```

The last step is worth studying. We push first and then check the size. When the newcomer is worse than everything inside, it becomes the top and is evicted immediately. That is the "bouncer sends them away" case, handled by the same two lines of code with no extra `if`.

**Step 3: empty the heap.** It pops worst first, so the result is filled from the back:
```
pop F7 -> result[1]
pop F3 -> result[0]

RESULT (busiest first): F3 F7
```

## Complexity

Let `n` be the number of reports, `m` the number of distinct fields and `k` how many we want.

- **Time: O(n + m log k).** Counting is O(n). Each of the `m` heap pushes and pops costs O(log k), because the heap never exceeds k+1 entries. Emptying the heap at the end is O(k log k).
- **Space: O(m + k).** The hash map holds `m` entries and the heap at most `k+1`.

The sort approach is O(n + m log m), so the heap wins when `k` is much smaller than `m`.

## Tests

| Input | Expected | What it checks |
|---|---|---|
| `[]`, k=3 | `[]` | empty list |
| `[1,2,3]`, k=0 and k=-1 | `[]` | k is zero or negative |
| `[1,1,1,2,2,3]`, k=2 | `[1,2]` | the simple case |
| `[4,4,4,4]`, k=1 | `[4]` | a single field |
| `[1,2,3]`, k=5 | `[1,2,3]` | k larger than the distinct fields |
| `[5,9,5,9,7]`, k=2 and k=1 | `[5,9]` and `[5]` | tie on count, smaller id first |
| `[-3,-3,8,8,8,0]`, k=2 | `[8,-3]` | negative ids |
| the worked example above | `[3,7]` | the README example |
| `[1,2,2,2,3,3]`, k=3 | `[2,3,1]` | the result is ordered busiest first |
| 1,000,001 reports | `[42]` | a large input |
| 2000 random inputs | same as brute force | fast agrees with slow, with many ties |

## Interview talking points

- **How to recognize it.** "Top K", "K most frequent", "K largest", "K closest". The first idea should be a heap of size K.
- **The direction trap.** To keep the **K largest**, the **smallest** of them must sit on top, so you can kick it out. People often use the wrong kind of heap. Say out loud which element is on top and why.
- **Always mention the sort alternative.** Show that you know it is simpler and O(m log m), then explain why the heap is better when K is small. Stating the trade-off is what interviewers listen for.
- **Pairs and tie-breaking.** A default `priority_queue<std::pair<int,int>>` orders by the first member, then the second, and shows the **largest** on top. That is the wrong direction by default, so use `std::greater` or a custom comparator.
- **Push then pop.** Pushing first and then evicting is shorter and also correct. An optimization is to compare with the top before pushing, which avoids one push when the newcomer is worse.
- **Two other solutions worth naming.**
  - *Bucket sort, O(n):* an array of buckets indexed by count (a count can't exceed `n`). Walk from the highest bucket down. Fast, but needs O(n) extra memory.
  - *Quickselect, O(m) on average:* partition until the K best are at the front. Fast on average but harder to write, and the output isn't sorted.
- **Streaming data.** If the list is too large for memory but the counts fit, this solution still works. If even the counts don't fit, the follow-up is an approximate structure such as Count-Min Sketch.

## Follow-ups

1. **Sum of kilograms instead of load counts.** The input becomes `(field, kg)` pairs. Step 1 sums instead of counting, and the rest is identical.
2. **The K *least* busy fields.** Flip the ranking, so the *busiest* candidate sits on top.
3. **Merge K sorted logs.** The heap holds one element per log, and each pop is the next smallest value overall.
4. **A running top K while reports keep arriving.** Keep the counts up to date and recompute with the heap on demand.

## How to run

From the repository root (after `cmake --preset debug`):

```bash
cmake --build --preset debug --target busiest_fields_test busiest_fields_trace
ctest --preset debug -L busiest_fields                      # run this challenge's tests
./build/debug/challenges/06_busiest_fields/busiest_fields_trace   # watch it step by step
```
