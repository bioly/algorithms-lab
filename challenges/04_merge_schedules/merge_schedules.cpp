#include "merge_schedules.h"

#include <algorithm>
#include <cstddef>

namespace algolab {

std::vector<Interval> MergeSchedules(std::vector<Interval> schedules) {
  // Taken BY VALUE on purpose: we need to sort, and sorting a copy leaves the
  // caller's data untouched. (std::move at the call site avoids the copy.)
  if (schedules.empty()) return {};

  // Key insight: after sorting by start day, any schedule that can overlap
  // the current block comes immediately after it, so we only ever compare
  // against the LAST merged block. Without sorting we'd have to compare
  // every pair: O(n^2).
  std::sort(schedules.begin(), schedules.end(),
            [](const Interval& a, const Interval& b) { return a.start < b.start; });

  std::vector<Interval> merged;
  merged.reserve(schedules.size());  // worst case: nothing merges -> no reallocation
  merged.push_back(schedules.front());

  for (std::size_t i = 1; i < schedules.size(); ++i) {
    const Interval& cur = schedules[i];
    Interval& last = merged.back();

    // Overlap or touch? `<=` (not `<`) because sharing a single day, like
    // [1,3] and [3,5], counts as connected. Ask the interviewer which they
    // want; that is a one-character difference.
    if (cur.start <= last.end) {
      // std::max matters: cur may sit fully INSIDE last (e.g. [1,10]
      // then [2,3]). Blindly writing last.end = cur.end would shrink it.
      last.end = std::max(last.end, cur.end);
    } else {
      merged.push_back(cur);  // a gap: start a new block
    }
  }
  return merged;
}

}  // namespace algolab
