#include "min_harvest_speed.h"

#include <algorithm>
#include <cstdint>

namespace algolab {
namespace {

// Can the harvester at `speed` kg/hour finish every field within `hours`?
// In one hour it works on ONE field only; leftover capacity in that hour is
// wasted. So a field of p kg costs ceil(p / speed) hours.
//
// Anonymous namespace: this helper is private to this file and is not part of
// the interface declared in the header.
bool CanFinish(const std::vector<int>& fields, int speed, std::int64_t hours) {
  // int64_t: with speed = 1 and a million fields of 1e9 kg the total is 1e15,
  // far beyond int. A wide type removes the overflow question entirely.
  std::int64_t needed = 0;
  for (const int kg : fields) {
    needed += (static_cast<std::int64_t>(kg) + speed - 1) / speed;  // integer ceil(kg / speed)
    if (needed > hours) return false;                               // early exit: already too slow
  }
  return true;
}

}  // namespace

int MinHarvestSpeed(const std::vector<int>& fields, int hours) {
  // Key insight: the answer is MONOTONIC. If speed k is fast enough, every
  // speed above k is too. So feasibility looks like  NNNN YYYYYYY  and we can
  // binary search for the first Y instead of trying every speed.
  int lo = 1;  // slowest sensible speed
  int hi = *std::max_element(fields.begin(), fields.end());
  // hi is always enough: at max-pile speed each field takes exactly 1 hour,
  // so we need only fields.size() <= hours. Faster than that gains nothing.

  // Invariant: the answer is always inside [lo, hi].
  while (lo < hi) {
    // lo + (hi - lo) / 2 instead of (lo + hi) / 2: same value, but cannot
    // overflow int when lo and hi are both huge.
    const int mid = lo + (hi - lo) / 2;

    if (CanFinish(fields, mid, hours)) {
      hi = mid;  // mid works: it MIGHT be the answer, so keep it (no -1)
    } else {
      lo = mid + 1;  // mid is too slow, and so is everything below it
    }
  }
  return lo;  // lo == hi: the first speed that works
}

}  // namespace algolab
