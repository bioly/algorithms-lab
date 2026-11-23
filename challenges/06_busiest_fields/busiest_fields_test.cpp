#include "busiest_fields.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <random>
#include <unordered_map>
#include <vector>

namespace algolab {
namespace {

using V = std::vector<int>;

TEST(BusiestFields, EmptyReports) { EXPECT_TRUE(BusiestFields({}, 3).empty()); }

TEST(BusiestFields, ZeroOrNegativeKGivesNothing) {
  EXPECT_TRUE(BusiestFields({1, 2, 3}, 0).empty());
  EXPECT_TRUE(BusiestFields({1, 2, 3}, -1).empty());
}

TEST(BusiestFields, SimpleCase) { EXPECT_EQ(BusiestFields({1, 1, 1, 2, 2, 3}, 2), (V{1, 2})); }

TEST(BusiestFields, SingleField) { EXPECT_EQ(BusiestFields({4, 4, 4, 4}, 1), (V{4})); }

TEST(BusiestFields, KLargerThanDistinctFieldsReturnsAll) {
  EXPECT_EQ(BusiestFields({1, 2, 3}, 5), (V{1, 2, 3}));
}

TEST(BusiestFields, TieOnCountPrefersSmallerFieldId) {
  // Fields 5 and 9 both have 2 loads, field 7 has 1.
  EXPECT_EQ(BusiestFields({5, 9, 5, 9, 7}, 2), (V{5, 9}));
  EXPECT_EQ(BusiestFields({5, 9, 5, 9, 7}, 1), (V{5}));
}

TEST(BusiestFields, NegativeIdsAreFine) {
  EXPECT_EQ(BusiestFields({-3, -3, 8, 8, 8, 0}, 2), (V{8, -3}));
}

TEST(BusiestFields, WorkedExampleFromReadme) {
  const V reports = {3, 7, 3, 5, 3, 7, 9, 3, 7, 5, 3, 2, 7, 3};
  EXPECT_EQ(BusiestFields(reports, 2), (V{3, 7}));
}

TEST(BusiestFields, ResultIsOrderedBusiestFirst) {
  // Loads: field 1 -> 1, field 2 -> 3, field 3 -> 2.
  EXPECT_EQ(BusiestFields({1, 2, 2, 2, 3, 3}, 3), (V{2, 3, 1}));
}

TEST(BusiestFields, LargeInput) {
  V reports(1'000'000);
  for (int i = 0; i < 1'000'000; ++i) reports[i] = i % 1000;  // every field: 1000 loads
  reports.push_back(42);                                      // field 42 gets one extra
  EXPECT_EQ(BusiestFields(reports, 1), (V{42}));
}

// Brute force: count, sort ALL distinct fields by the same rule, take the first k.
// Slow when there are many fields, but obviously correct.
V BruteForce(const V& reports, int k) {
  std::unordered_map<int, int> loads;
  for (int field : reports) ++loads[field];
  std::vector<std::pair<int, int>> all(loads.begin(), loads.end());  // (field, count)
  std::sort(all.begin(), all.end(), [](const auto& a, const auto& b) {
    if (a.second != b.second) return a.second > b.second;
    return a.first < b.first;
  });
  V out;
  for (int i = 0; i < k && i < static_cast<int>(all.size()); ++i) out.push_back(all[i].first);
  return out;
}

TEST(BusiestFields, MatchesBruteForceOnRandomInput) {
  std::mt19937 rng(3);  // fixed seed: a failure is reproducible
  for (int round = 0; round < 2000; ++round) {
    V reports(rng() % 30);
    for (int& id : reports) id = static_cast<int>(rng() % 8) - 2;  // small range -> many ties
    const int k = static_cast<int>(rng() % 10);
    ASSERT_EQ(BusiestFields(reports, k), BruteForce(reports, k)) << "round " << round;
  }
}

}  // namespace
}  // namespace algolab
