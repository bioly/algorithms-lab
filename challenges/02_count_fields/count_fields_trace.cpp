// Step-by-step demo for Challenge 02: the "paint bucket" flood fill.
//
// Same algorithm as count_fields.cpp, but it paints each field with a letter
// (A, B, C...) instead of using a bool matrix, so the output is readable.
#include <iostream>
#include <utility>
#include <vector>

using Grid = std::vector<std::vector<int>>;

// '.' water, '#' land not painted yet, letter = field label, [x] = cell being processed
void Show(const Grid& g, const std::vector<std::vector<char>>& label, int hr, int hc) {
  for (int r = 0; r < static_cast<int>(g.size()); ++r) {
    std::cout << "      ";
    for (int c = 0; c < static_cast<int>(g[0].size()); ++c) {
      const char ch = g[r][c] == 0 ? '.' : (label[r][c] ? label[r][c] : '#');
      if (r == hr && c == hc)
        std::cout << "[" << ch << "]";
      else
        std::cout << " " << ch << " ";
    }
    std::cout << "\n";
  }
}

int main() {
  const Grid g = {{1, 1, 0, 0, 0}, {1, 1, 0, 0, 1}, {0, 0, 1, 0, 1}, {0, 0, 0, 1, 1}};
  const int rows = 4, cols = 5;
  std::vector<std::vector<char>> label(rows, std::vector<char>(cols, 0));
  const int dr[4] = {-1, 1, 0, 0}, dc[4] = {0, 0, -1, 1};
  const char* dir_name[4] = {"up", "down", "left", "right"};
  std::vector<std::pair<int, int>> stack;
  int fields = 0;

  std::cout << "MAP   (. = water, # = land not painted yet)\n";
  Show(g, label, -1, -1);

  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < cols; ++c) {
      if (g[r][c] != 1) continue;  // water: stay silent
      if (label[r][c]) {
        std::cout << "\nSCAN (" << r << "," << c << "): land, but already part of field "
                  << label[r][c] << " -> skip, NOT counted again\n";
        continue;
      }
      const char name = static_cast<char>('A' + fields++);
      std::cout << "\n##### SCAN (" << r << "," << c << "): NEW land -> field " << name
                << "  (count = " << fields << ") #####\n";
      label[r][c] = name;
      stack.push_back({r, c});
      while (!stack.empty()) {
        const auto [cr, cc] = stack.back();
        stack.pop_back();
        std::cout << "\n  pop (" << cr << "," << cc << ")\n";
        for (int d = 0; d < 4; ++d) {
          const int nr = cr + dr[d], nc = cc + dc[d];
          if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;
          if (g[nr][nc] != 1 || label[nr][nc]) continue;
          label[nr][nc] = name;
          stack.push_back({nr, nc});
          std::cout << "    " << dir_name[d] << " (" << nr << "," << nc << ") is new land -> label "
                    << name << ", push\n";
        }
        Show(g, label, cr, cc);
        std::cout << "      stack: ";
        if (stack.empty()) std::cout << "(empty -> field " << name << " finished)";
        for (const auto& p : stack) std::cout << "(" << p.first << "," << p.second << ") ";
        std::cout << "\n";
      }
    }
  }
  std::cout << "\nRESULT: " << fields << " fields\n";
}
