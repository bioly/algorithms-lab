#include "merge_schedules.h"

#include <gtest/gtest.h>

#include <random>
#include <vector>

namespace algolab {
namespace {

using V = std::vector<Interval>;

TEST(MergeSchedules, EmptyInput) { EXPECT_TRUE(MergeSchedules({}).empty()); }

TEST(MergeSchedules, SingleSchedule) { EXPECT_EQ(MergeSchedules({{1, 4}}), (V{{1, 4}})); }

TEST(MergeSchedules, ClassicExample) {
  EXPECT_EQ(MergeSchedules({{1, 3}, {2, 6}, {8, 10}, {15, 18}}), (V{{1, 6}, {8, 10}, {15, 18}}));
}

TEST(MergeSchedules, TouchingSchedulesMerge) {
  EXPECT_EQ(MergeSchedules({{1, 3}, {3, 5}}), (V{{1, 5}}));
}

TEST(MergeSchedules, AdjacentDaysStaySeparate) {
  EXPECT_EQ(MergeSchedules({{1, 2}, {3, 4}}), (V{{1, 2}, {3, 4}}));
}

TEST(MergeSchedules, NestedScheduleDoesNotShrinkTheBlock) {
  // The trap: [2,3] sits inside [1,10]. Using `end = cur.end` would break this.
  EXPECT_EQ(MergeSchedules({{1, 10}, {2, 3}, {4, 5}}), (V{{1, 10}}));
}

TEST(MergeSchedules, UnsortedInputIsHandled) {
  EXPECT_EQ(MergeSchedules({{5, 6}, {1, 2}, {3, 4}}), (V{{1, 2}, {3, 4}, {5, 6}}));
}

TEST(MergeSchedules, DuplicatesCollapse) {
  EXPECT_EQ(MergeSchedules({{2, 3}, {2, 3}, {2, 3}}), (V{{2, 3}}));
}

TEST(MergeSchedules, WorkedExampleFromReadme) {
  EXPECT_EQ(MergeSchedules({{8, 10}, {1, 3}, {2, 6}, {3, 4}, {6, 7}, {15, 18}, {17, 20}}),
            (V{{1, 7}, {8, 10}, {15, 20}}));
}

TEST(MergeSchedules, DoesNotModifyCallersVector) {
  const V input = {{5, 6}, {1, 2}};
  const V copy = input;
  MergeSchedules(input);  // passed by value -> a copy is sorted, not ours
  EXPECT_EQ(input, copy);
}

// Brute force on a small day range: mark every covered point on a doubled
// number line (so [1,2] and [3,4] stay distinct from [1,3] + [3,4]), then
// read off the maximal covered runs.
V BruteForce(const V& v) {
  std::vector<bool> covered(2 * 30 + 3, false);
  for (const auto& i : v)
    for (int p = 2 * i.start; p <= 2 * i.end; ++p) covered[p] = true;
  V out;
  for (int p = 0; p < static_cast<int>(covered.size()); ++p) {
    if (!covered[p]) continue;
    int q = p;
    while (q + 1 < static_cast<int>(covered.size()) && covered[q + 1]) ++q;
    out.push_back({p / 2, q / 2});
    p = q;
  }
  return out;
}

TEST(MergeSchedules, MatchesBruteForceOnRandomInput) {
  std::mt19937 rng(11);  // fixed seed: a failure is reproducible
  for (int round = 0; round < 1500; ++round) {
    V v;
    const int n = static_cast<int>(rng() % 8);
    for (int i = 0; i < n; ++i) {
      const int s = static_cast<int>(rng() % 25);
      v.push_back({s, s + static_cast<int>(rng() % 5)});
    }
    ASSERT_EQ(MergeSchedules(v), BruteForce(v)) << "round " << round;
  }
}

}  // namespace
}  // namespace algolab
