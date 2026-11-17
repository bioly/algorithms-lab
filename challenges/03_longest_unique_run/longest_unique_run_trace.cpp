// Step-by-step demo for Challenge 03: the sliding window.
// R marks the right edge, ^ marks the other cells inside the window.
#include <algorithm>
#include <array>
#include <iostream>
#include <string>

int main() {
  const std::string row = "abcbdea";
  std::array<int, 256> last_seen;
  last_seen.fill(-1);
  int left = 0, best = 0;

  std::cout << "ROW:  ";
  for (char c : row) std::cout << " " << c << " ";
  std::cout << "\nIDX:  ";
  for (size_t i = 0; i < row.size(); ++i) std::cout << " " << i << " ";
  std::cout << "\n";

  for (int right = 0; right < static_cast<int>(row.size()); ++right) {
    const auto ch = static_cast<unsigned char>(row[right]);
    std::cout << "\n--- step " << right + 1 << ": right=" << right << " crop='" << row[right]
              << "' ---\n";
    if (last_seen[ch] >= left) {
      std::cout << "   '" << row[right] << "' last seen at " << last_seen[ch] << " >= left(" << left
                << ")  -> repeat INSIDE window -> left = " << last_seen[ch] + 1 << "\n";
      left = last_seen[ch] + 1;
    } else if (last_seen[ch] >= 0) {
      std::cout << "   '" << row[right] << "' last seen at " << last_seen[ch] << " < left(" << left
                << ")  -> old sighting is OUTSIDE window -> ignore, left stays " << left << "\n";
    } else {
      std::cout << "   '" << row[right] << "' never seen -> window just grows\n";
    }
    last_seen[ch] = right;
    best = std::max(best, right - left + 1);

    std::cout << "ROW:  ";
    for (char c : row) std::cout << " " << c << " ";
    std::cout << "\nWIN:  ";
    for (int i = 0; i < static_cast<int>(row.size()); ++i)
      std::cout << (i == right ? " R " : (i >= left && i < right ? " ^ " : "   "));
    std::cout << "\n   window = \"" << row.substr(left, right - left + 1)
              << "\"  length = " << right - left + 1 << "   best = " << best << "\n";
    std::cout << "   last_seen: ";
    for (int c = 'a'; c <= 'z'; ++c)
      if (last_seen[c] >= 0) std::cout << static_cast<char>(c) << "@" << last_seen[c] << " ";
    std::cout << "\n";
  }
  std::cout << "\nRESULT: longest run = " << best << "\n";
}
