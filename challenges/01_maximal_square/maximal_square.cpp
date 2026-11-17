#include "maximal_square.h"

#include <algorithm>  // std::min, std::max
#include <cstddef>

namespace algolab {

int LargestSquareArea(const std::vector<std::vector<int>>& grid) {
  // `const &` -> we only read the grid, so we avoid copying it (could be huge)
  // and promise not to change it.

  // Edge case: nothing to farm. Checking up front avoids crashing on grid.front().
  if (grid.empty() || grid.front().empty()) return 0;

  const std::size_t rows = grid.size();
  const std::size_t cols = grid.front().size();
  // size_t is the "natural" type for sizes and indices of a vector.
  // It's unsigned, so it also avoids signed/unsigned comparison warnings.

  // Why std::vector?
  //  - The grid size is only known at runtime, so a plain array won't work.
  //  - It manages its own memory (no new/delete, no leaks).
  //  - Its elements sit next to each other in memory, which is fast.
  //
  // dp[c + 1] = side of the biggest square whose bottom-right corner is at
  //             column c of the row we're processing.
  // We use cols + 1 slots: slot 0 is a permanent "zero" that stands in for
  // "left of the first column". That way we never need special if-checks
  // for the border.
  std::vector<int> dp(cols + 1, 0);

  int best_side = 0;  // the biggest square side found so far

  for (std::size_t r = 0; r < rows; ++r) {
    // The diagonal neighbor (up-left). At the start of each row there is
    // nothing to the left, so it's 0.
    int diagonal = 0;

    for (std::size_t c = 0; c < cols; ++c) {
      // Save the "above" value BEFORE we overwrite it. dp[c + 1] still
      // holds the previous row's result. It's also the diagonal value
      // for the next column, as the last line of this loop shows.
      const int above = dp[c + 1];

      if (grid[r][c] == 1) {
        // dp[c] is "left": it was already updated in this row.
        // Smallest neighbor decides, then +1 for the current cell.
        dp[c + 1] = 1 + std::min({dp[c], above, diagonal});
        best_side = std::max(best_side, dp[c + 1]);
      } else {
        dp[c + 1] = 0;  // bad land: no square can end here
      }

      // Moving one column to the right: today's "above" becomes
      // tomorrow's "diagonal".
      diagonal = above;
    }
  }

  // The question asks for the area, and area = side * side.
  return best_side * best_side;
}

}  // namespace algolab
