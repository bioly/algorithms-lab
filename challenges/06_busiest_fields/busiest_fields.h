#pragma once

#include <vector>

namespace algolab {

// Challenge 06 - The K Busiest Fields (top K frequent elements).
//
// `reports` lists the id of the field for every truck load that left a field.
// Returns the ids of the `k` fields with the most loads, busiest first. Ties
// are broken by the smaller field id, so the answer is always unique.
//
// If there are fewer than `k` distinct fields, all of them are returned.
// `k <= 0` or an empty `reports` returns an empty vector.
//
// Time O(n + m log k), space O(m + k), where n = reports.size() and
// m = number of distinct fields.
std::vector<int> BusiestFields(const std::vector<int>& reports, int k);

}  // namespace algolab
