#include "count_fields.h"

#include <gtest/gtest.h>

#include <vector>

namespace algolab {
namespace {

using Grid = std::vector<std::vector<int>>;

TEST(CountFields, EmptyGrid) {
  EXPECT_EQ(CountFields({}), 0);
  EXPECT_EQ(CountFields({{}}), 0);
}

TEST(CountFields, AllWater) { EXPECT_EQ(CountFields({{0, 0}, {0, 0}}), 0); }

TEST(CountFields, SingleCell) { EXPECT_EQ(CountFields({{1}}), 1); }

TEST(CountFields, SolidBlockIsOneField) { EXPECT_EQ(CountFields({{1, 1}, {1, 1}}), 1); }

TEST(CountFields, DiagonalCellsAreNotConnected) { EXPECT_EQ(CountFields({{1, 0}, {0, 1}}), 2); }

TEST(CountFields, CheckerboardGivesFiveFields) {
  EXPECT_EQ(CountFields({{1, 0, 1}, {0, 1, 0}, {1, 0, 1}}), 5);
}

TEST(CountFields, WorkedExampleFromReadme) {
  const Grid grid = {
      {1, 1, 0, 0, 0},
      {1, 1, 0, 0, 1},
      {0, 0, 1, 0, 1},
      {0, 0, 0, 1, 1},
  };
  EXPECT_EQ(CountFields(grid), 3);
}

TEST(CountFields, SnakeShapedFieldIsOneField) {
  // A long winding field: the classic stress shape for recursive solutions.
  const Grid grid = {
      {1, 1, 1}, {0, 0, 1}, {1, 1, 1}, {1, 0, 0}, {1, 1, 1},
  };
  EXPECT_EQ(CountFields(grid), 1);
}

TEST(CountFields, DoesNotModifyInput) {
  const Grid grid = {{1, 0}, {0, 1}};
  const Grid copy = grid;
  CountFields(grid);
  EXPECT_EQ(grid, copy);
}

TEST(CountFields, HugeSolidGridDoesNotOverflowTheStack) {
  // 4 million connected cells. Naive recursion would crash here.
  EXPECT_EQ(CountFields(Grid(2000, std::vector<int>(2000, 1))), 1);
}

}  // namespace
}  // namespace algolab
