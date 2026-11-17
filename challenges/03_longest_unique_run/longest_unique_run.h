#pragma once

#include <string_view>

namespace algolab {

// Challenge 03 - The Farmer's Longest Clean Row (longest substring without
// repeating characters).
//
// Each character of `row` is a crop type. Returns the length of the longest
// continuous stretch in which no crop type appears twice. Returns 0 for "".
//
// Time O(n), space O(1) (a fixed 256-entry table).
int LongestUniqueRun(std::string_view row);

}  // namespace algolab
