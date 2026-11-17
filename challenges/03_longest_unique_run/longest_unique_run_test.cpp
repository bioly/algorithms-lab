#include "longest_unique_run.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <random>
#include <string>

namespace algolab {
namespace {

TEST(LongestUniqueRun, EmptyRow) { EXPECT_EQ(LongestUniqueRun(""), 0); }

TEST(LongestUniqueRun, SingleCrop) { EXPECT_EQ(LongestUniqueRun("a"), 1); }

TEST(LongestUniqueRun, AllSameCrop) { EXPECT_EQ(LongestUniqueRun("aaaa"), 1); }

TEST(LongestUniqueRun, NoRepeatsMeansWholeRow) { EXPECT_EQ(LongestUniqueRun("abcdef"), 6); }

TEST(LongestUniqueRun, ClassicExample) { EXPECT_EQ(LongestUniqueRun("abcabcbb"), 3); }

TEST(LongestUniqueRun, AnswerInTheMiddle) { EXPECT_EQ(LongestUniqueRun("pwwkew"), 3); }

TEST(LongestUniqueRun, StaleSightingOutsideWindowIsIgnored) {
  // The trap from the README: the last 'a' was seen at index 0, but the
  // window already starts at 2, so it is not a repeat.
  EXPECT_EQ(LongestUniqueRun("abcbdea"), 5);
}

TEST(LongestUniqueRun, LeftEdgeNeverMovesBackwards) {
  // After "abb", left == 2. The final 'a' was last seen at 0 (< left).
  // Without the `>= left` check the answer would be wrong.
  EXPECT_EQ(LongestUniqueRun("abba"), 2);
}

TEST(LongestUniqueRun, HandlesNonAsciiBytes) {
  // Bytes >= 128 are negative as plain `char`; the unsigned cast keeps the
  // table index valid.
  EXPECT_EQ(LongestUniqueRun("\xC3\xA9\xC3\xA8"),
            3);  // bytes C3 A9 C3 A8: C3 repeats, best window is A9 C3 A8
}

TEST(LongestUniqueRun, LongRowStaysFast) {
  EXPECT_EQ(LongestUniqueRun(std::string(1'000'000, 'x')), 1);
}

// Brute force: check every start/end pair. Slow but obviously correct.
int BruteForce(const std::string& s) {
  int best = 0;
  for (size_t i = 0; i < s.size(); ++i) {
    for (size_t j = i; j < s.size(); ++j) {
      bool unique = true;
      for (size_t a = i; a <= j && unique; ++a)
        for (size_t b = a + 1; b <= j; ++b)
          if (s[a] == s[b]) {
            unique = false;
            break;
          }
      if (unique) best = std::max(best, static_cast<int>(j - i + 1));
    }
  }
  return best;
}

TEST(LongestUniqueRun, MatchesBruteForceOnRandomRows) {
  std::mt19937 rng(7);  // fixed seed: a failure is reproducible
  for (int round = 0; round < 1000; ++round) {
    std::string s;
    const int n = static_cast<int>(rng() % 14);
    for (int i = 0; i < n; ++i) s += static_cast<char>('a' + rng() % 4);
    ASSERT_EQ(LongestUniqueRun(s), BruteForce(s)) << "row: " << s;
  }
}

}  // namespace
}  // namespace algolab
