#pragma once

#include <vector>

namespace algolab {

// One planting schedule: the field is in use from day `start` to day `end`
// (both inclusive).
struct Interval {
  int start;
  int end;

  bool operator==(const Interval&) const = default;  // C++20: free ==
};

// Challenge 04 - Merging the Planting Schedules (merge intervals).
//
// Merges schedules that overlap or touch (they share at least one day, so
// [1,3] and [3,5] become [1,5]). Schedules on adjacent but different days,
// like [1,3] and [4,5], stay separate. Returns the merged schedules sorted by
// start day. Assumes start <= end for every input interval.
//
// Time O(n log n), space O(n).
std::vector<Interval> MergeSchedules(std::vector<Interval> schedules);

}  // namespace algolab
