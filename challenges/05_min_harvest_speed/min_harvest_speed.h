#pragma once

#include <vector>

namespace algolab {

// Challenge 05 - The Harvester Speed (binary search on the answer).
//
// `fields[i]` is the amount of crop (kg) in field i. A harvester with speed k
// (kg/hour, whole number) works on ONE field per hour; if the field has less
// than k kg left, the rest of that hour is wasted. Returns the smallest k that
// finishes every field within `hours`.
//
// Preconditions: fields is non-empty, every pile >= 1, hours >= fields.size()
// (otherwise it is impossible, since each field needs at least one hour).
//
// Time O(n log M) where M is the largest pile, space O(1).
int MinHarvestSpeed(const std::vector<int>& fields, int hours);

}  // namespace algolab
