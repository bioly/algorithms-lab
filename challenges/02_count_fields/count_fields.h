#pragma once

#include <vector>

namespace algolab {

// Challenge 02 - Count the Farmer's Separate Fields (number of islands).
//
// `grid` holds 1 (good land) and 0 (bad land). A field is a group of 1s
// connected up, down, left or right (NOT diagonally). Returns how many
// separate fields there are. The input is not modified.
//
// Time O(rows * cols), space O(rows * cols).
int CountFields(const std::vector<std::vector<int>>& grid);

}  // namespace algolab
