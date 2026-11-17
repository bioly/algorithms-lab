#include "min_harvest_speed.h"

#include <gtest/gtest.h>

#include <cstdint>
#include <random>
#include <vector>

namespace algolab {
namespace {

TEST(MinHarvestSpeed, MainExample) { EXPECT_EQ(MinHarvestSpeed({3, 6, 7, 11}, 8), 4); }

TEST(MinHarvestSpeed, HoursEqualFieldsMeansMaxPile) {
  EXPECT_EQ(MinHarvestSpeed({30, 11, 23, 4, 20}, 5), 30);
}

TEST(MinHarvestSpeed, OneSpareHour) { EXPECT_EQ(MinHarvestSpeed({30, 11, 23, 4, 20}, 6), 23); }

TEST(MinHarvestSpeed, OneFieldPlentyOfTime) { EXPECT_EQ(MinHarvestSpeed({7}, 7), 1); }

TEST(MinHarvestSpeed, OneFieldOneHour) { EXPECT_EQ(MinHarvestSpeed({7}, 1), 7); }

TEST(MinHarvestSpeed, LowerBoundIsOne) { EXPECT_EQ(MinHarvestSpeed({1, 1, 1, 1}, 100), 1); }

TEST(MinHarvestSpeed, LargeValuesDoNotOverflow) {
  EXPECT_EQ(MinHarvestSpeed({1'000'000'000, 1'000'000'000}, 2), 1'000'000'000);
}

TEST(MinHarvestSpeed, ManyHugeFieldsDoNotOverflowTheHourCounter) {
  // 100,000 fields of 1e9 kg with a 1e9 hour deadline.
  // Each field may take at most 1e9 / 1e5 = 1e4 hours, so speed >= 1e9 / 1e4 = 1e5.
  const std::vector<int> fields(100'000, 1'000'000'000);
  EXPECT_EQ(MinHarvestSpeed(fields, 1'000'000'000), 100'000);
}

// Brute force: try speeds 1, 2, 3... until one works. Slow but obviously correct.
int BruteForce(const std::vector<int>& fields, int hours) {
  for (int k = 1;; ++k) {
    std::int64_t needed = 0;
    for (int p : fields) needed += (p + k - 1) / k;
    if (needed <= hours) return k;
  }
}

TEST(MinHarvestSpeed, MatchesBruteForceOnRandomInput) {
  std::mt19937 rng(5);  // fixed seed: a failure is reproducible
  for (int round = 0; round < 1500; ++round) {
    const int n = 1 + static_cast<int>(rng() % 6);
    std::vector<int> fields(n);
    for (int& p : fields) p = 1 + static_cast<int>(rng() % 30);
    const int hours = n + static_cast<int>(rng() % 40);
    ASSERT_EQ(MinHarvestSpeed(fields, hours), BruteForce(fields, hours)) << "round " << round;
  }
}

}  // namespace
}  // namespace algolab
