#include "longest_unique_run.h"

#include <algorithm>
#include <array>

namespace algolab {

// `std::string_view` accepts a std::string, a string literal or a const char*
// WITHOUT copying. It is the right type for a read-only text parameter.
int LongestUniqueRun(std::string_view row) {
  // last_seen[ch] = index where crop `ch` appeared most recently, or -1.
  // Why std::array<int, 256> and not unordered_map?
  //  - A char has only 256 possible values, so a fixed table is enough.
  //  - Lookup is a plain array index: no hashing, no heap allocation.
  //  - std::array knows its size and is a drop-in for a C array.
  // (If the alphabet were huge, e.g. Unicode, a hash map would make sense.)
  std::array<int, 256> last_seen;
  last_seen.fill(-1);

  int left = 0;  // window = row[left .. right], always free of repeats
  int best = 0;

  for (int right = 0; right < static_cast<int>(row.size()); ++right) {
    // Cast through unsigned char: plain `char` may be negative, which would
    // index before the start of the array.
    const auto ch = static_cast<unsigned char>(row[right]);

    // Repeat INSIDE the current window? Only then must the window shrink.
    // `>= left` matters: an old sighting before `left` is already outside
    // the window and must be ignored. Without it, `left` could move backwards.
    if (last_seen[ch] >= left) {
      left = last_seen[ch] + 1;  // jump just past the earlier copy
    }

    last_seen[ch] = right;                    // remember the newest position
    best = std::max(best, right - left + 1);  // window length
  }
  return best;
}

}  // namespace algolab
