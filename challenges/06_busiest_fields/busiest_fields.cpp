#include "busiest_fields.h"

#include <cstddef>
#include <queue>
#include <unordered_map>

namespace algolab {
namespace {

struct Entry {
  int field;
  int count;
};

// Ranking rule: more loads is better; on a tie the smaller field id is better.
// A strict, total order makes the answer unique, so tests are deterministic.
bool RanksHigher(const Entry& a, const Entry& b) {
  if (a.count != b.count) return a.count > b.count;
  return a.field < b.field;
}

}  // namespace

std::vector<int> BusiestFields(const std::vector<int>& reports, int k) {
  if (k <= 0 || reports.empty()) return {};

  // Step 1: count the loads per field. unordered_map: average O(1) per update.
  std::unordered_map<int, int> loads;
  for (const int field : reports) ++loads[field];

  // Step 2: keep only the best k in a heap that holds at most k entries.
  //
  // std::priority_queue keeps the LARGEST element (according to the
  // comparator) on top. We pass RanksHigher as the comparator, so "smaller"
  // means "ranks higher", and the "largest" is the entry that ranks LOWEST.
  // So the top of this heap is always the WORST of our current k candidates,
  // exactly the one to throw out when a better candidate arrives.
  std::priority_queue<Entry, std::vector<Entry>, decltype(&RanksHigher)> heap(&RanksHigher);

  for (const auto& [field, count] : loads) {
    heap.push({field, count});
    // Size k+1 -> evict the worst. The heap never holds more than k+1
    // entries, which is what makes the loop O(m log k), not O(m log m).
    // If the newcomer is worse than everything inside, it becomes the top
    // and is evicted right away: the "bouncer sends them away" case.
    if (heap.size() > static_cast<std::size_t>(k)) heap.pop();
  }

  // Step 3: the heap pops worst-first, so fill the result from the back.
  // (`i-- > 0` is the idiom for counting down an unsigned index to 0.)
  std::vector<int> result(heap.size());
  for (std::size_t i = result.size(); i-- > 0;) {
    result[i] = heap.top().field;
    heap.pop();
  }
  return result;
}

}  // namespace algolab
