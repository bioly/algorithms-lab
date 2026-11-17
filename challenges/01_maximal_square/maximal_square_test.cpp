#include "maximal_square.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <random>
#include <vector>

namespace algolab {
namespace {

using Grid = std::vector<std::vector<int>>;

TEST(MaximalSquare, EmptyGridHasNoSquare) {
  EXPECT_EQ(LargestSquareArea({}), 0);
  EXPECT_EQ(LargestSquareArea({{}}), 0);
}

TEST(MaximalSquare, AllZerosHasNoSquare) { EXPECT_EQ(LargestSquareArea({{0, 0}, {0, 0}}), 0); }

TEST(MaximalSquare, SingleOneIsAreaOne) { EXPECT_EQ(LargestSquareArea({{1}}), 1); }

TEST(MaximalSquare, RowOfOnesIsNotASquare) {
  // The trap in the problem: a rectangle is not a square.
  EXPECT_EQ(LargestSquareArea({{0, 1, 1, 1, 0}}), 1);
}

TEST(MaximalSquare, AllOnesIsLimitedByShorterSide) {
  EXPECT_EQ(LargestSquareArea(Grid(3, std::vector<int>(5, 1))), 9);
}

TEST(MaximalSquare, WorkedExampleFromReadme) {
  const Grid grid = {
      {0, 1, 1, 1, 0},
      {1, 1, 1, 1, 1},
      {0, 1, 1, 1, 0},
  };
  EXPECT_EQ(LargestSquareArea(grid), 9);
}

TEST(MaximalSquare, SingleZeroInTheCornerShrinksTheAnswer) {
  const Grid grid = {
      {1, 1, 1},
      {1, 1, 1},
      {1, 1, 0},
  };
  EXPECT_EQ(LargestSquareArea(grid), 4);
}

TEST(MaximalSquare, ZeroInTheMiddleOfBigBlock) {
  const Grid grid = {
      {1, 1, 1, 1},
      {1, 0, 1, 1},
      {1, 1, 1, 1},
      {1, 1, 1, 1},
  };
  EXPECT_EQ(LargestSquareArea(grid), 4);
}

// Brute force: try every top-left corner and every size. Slow but obviously
// correct, so it can referee the fast version.
int BruteForceArea(const Grid& g) {
  const int rows = static_cast<int>(g.size());
  const int cols = rows ? static_cast<int>(g[0].size()) : 0;
  int best = 0;
  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < cols; ++c) {
      for (int side = 1; r + side <= rows && c + side <= cols; ++side) {
        bool all_ones = true;
        for (int i = r; i < r + side && all_ones; ++i)
          for (int j = c; j < c + side; ++j)
            if (g[i][j] != 1) {
              all_ones = false;
              break;
            }
        if (all_ones) best = std::max(best, side * side);
      }
    }
  }
  return best;
}

TEST(MaximalSquare, MatchesBruteForceOnRandomGrids) {
  std::mt19937 rng(12345);  // fixed seed: a failure is reproducible
  for (int round = 0; round < 500; ++round) {
    const int rows = 1 + static_cast<int>(rng() % 7);
    const int cols = 1 + static_cast<int>(rng() % 7);
    Grid g(rows, std::vector<int>(cols));
    for (auto& row : g)
      for (int& cell : row) cell = static_cast<int>(rng() % 4 != 0);  // ~75% ones
    ASSERT_EQ(LargestSquareArea(g), BruteForceArea(g)) << "round " << round;
  }
}

}  // namespace
}  // namespace algolab
