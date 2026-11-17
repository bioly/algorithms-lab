#pragma once

#include <vector>

namespace algolab {

// Challenge 01 - Maximal Square.
//
// `grid` holds 1 (good land) and 0 (bad land). Returns the AREA of the biggest
// square made only of 1s. Returns 0 for an empty grid or a grid with no 1s.
//
// Time O(rows * cols), extra space O(cols).
int LargestSquareArea(const std::vector<std::vector<int>>& grid);

}  // namespace algolab
