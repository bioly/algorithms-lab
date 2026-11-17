// Step-by-step demo for Challenge 01. Run:
// ./build/debug/challenges/01_maximal_square/maximal_square_trace
//
// This is the same algorithm as maximal_square.cpp with print statements
// added, plus a second table (`shown`) that exists only so we can draw the
// whole dp table. The real solution keeps just one row.
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

// Prints the dp table "so far": filled cells show their value, the current
// cell is wrapped in [ ], cells not processed yet show '.'
void PrintTable(const std::vector<std::vector<int>>& shown, size_t cur_r, size_t cur_c, size_t rows,
                size_t cols) {
  for (size_t r = 0; r < rows; ++r) {
    std::cout << "      ";
    for (size_t c = 0; c < cols; ++c) {
      const bool done = (r < cur_r) || (r == cur_r && c <= cur_c);
      const std::string v = done ? std::to_string(shown[r][c]) : ".";
      if (r == cur_r && c == cur_c)
        std::cout << "[" << v << "]";
      else
        std::cout << " " << v << " ";
    }
    std::cout << "\n";
  }
}

int main() {
  const std::vector<std::vector<int>> grid = {
      {0, 1, 1, 1, 0},
      {1, 1, 1, 1, 1},
      {0, 1, 1, 1, 0},
  };
  const size_t rows = grid.size(), cols = grid[0].size();

  std::cout << "INPUT GRID (1 = good land, 0 = bad land)\n";
  for (const auto& row : grid) {
    std::cout << "      ";
    for (int v : row) std::cout << " " << v << " ";
    std::cout << "\n";
  }
  std::cout << "\n";

  std::vector<int> dp(cols + 1, 0);                                      // the real one-row memory
  std::vector<std::vector<int>> shown(rows, std::vector<int>(cols, 0));  // display only
  int best_side = 0, step = 0;

  for (size_t r = 0; r < rows; ++r) {
    int diagonal = 0;
    std::cout << "=========== ROW " << r << " ===========\n";
    for (size_t c = 0; c < cols; ++c) {
      const int above = dp[c + 1];
      const int left = dp[c];
      std::cout << "\nStep " << ++step << ": cell (" << r << "," << c << ") grid=" << grid[r][c]
                << "\n";
      if (grid[r][c] == 1) {
        const int m = std::min({left, above, diagonal});
        dp[c + 1] = 1 + m;
        std::cout << "   diag=" << diagonal << "  above=" << above << "  left=" << left
                  << "   -> min=" << m << " -> dp = " << m << " + 1 = " << dp[c + 1] << "\n";
        if (dp[c + 1] > best_side) {
          best_side = dp[c + 1];
          std::cout << "   *** new best side = " << best_side << " ***\n";
        }
      } else {
        dp[c + 1] = 0;
        std::cout << "   bad land -> dp = 0\n";
      }
      shown[r][c] = dp[c + 1];
      diagonal = above;

      PrintTable(shown, r, c, rows, cols);
      std::cout << "      1-row memory: [ ";
      for (size_t i = 1; i <= cols; ++i) std::cout << dp[i] << " ";
      std::cout << "]  (saved diagonal for next cell = " << diagonal << ")\n";
    }
    std::cout << "\n";
  }
  std::cout << "RESULT: best side = " << best_side << ", area = " << best_side * best_side << "\n";
}
