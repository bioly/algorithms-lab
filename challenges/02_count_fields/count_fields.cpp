#include "count_fields.h"

#include <utility>

namespace algolab {

int CountFields(const std::vector<std::vector<int>>& grid) {
  if (grid.empty() || grid.front().empty()) return 0;  // nothing to count

  // int (not size_t) for indices: neighbor math like r - 1 can go to -1,
  // and with an unsigned type that would wrap around to a huge number.
  const int rows = static_cast<int>(grid.size());
  const int cols = static_cast<int>(grid.front().size());

  // "Have I already counted this cell as part of some field?"
  // Separate matrix so we don't modify the caller's input (const&).
  // vector<bool> packs 1 bit per cell, so it is memory-friendly.
  std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));

  // The 4 allowed moves: up, down, left, right. No diagonals!
  // Looping over two small tables avoids copy-pasting 4 near-identical blocks.
  constexpr int kDr[4] = {-1, 1, 0, 0};
  constexpr int kDc[4] = {0, 0, -1, 1};

  // Explicit stack instead of recursion: a big all-ones grid would recurse
  // millions of levels deep and crash with a stack overflow.
  // std::vector works well as a stack (push_back / back / pop_back),
  // and its memory lives on the heap, which can grow far larger.
  // It is declared outside the loops so its memory is reused for every field.
  std::vector<std::pair<int, int>> stack;

  int fields = 0;

  // Scan every cell once, left to right, top to bottom.
  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < cols; ++c) {
      // Skip water, and skip land we already counted in an earlier field.
      // This line is what prevents counting the same field twice.
      if (grid[r][c] != 1 || visited[r][c]) continue;

      // Brand-new field: count it ONCE, then "flood" the whole field
      // so none of its other cells can trigger another count.
      ++fields;
      visited[r][c] = true;     // mark when pushing (not when popping), so a
      stack.push_back({r, c});  // cell never sits on the stack twice

      while (!stack.empty()) {
        const auto [cr, cc] = stack.back();  // take the newest cell
        stack.pop_back();

        for (int d = 0; d < 4; ++d) {
          const int nr = cr + kDr[d];
          const int nc = cc + kDc[d];

          // Stay inside the map, only walk on good land, and only
          // on land we haven't seen yet.
          if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;
          if (grid[nr][nc] != 1 || visited[nr][nc]) continue;

          visited[nr][nc] = true;
          stack.push_back({nr, nc});
        }
      }
    }
  }
  return fields;
}

}  // namespace algolab
