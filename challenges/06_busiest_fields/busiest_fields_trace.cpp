// Step-by-step demo for Challenge 06: a heap that keeps only the best k.
//
// Same logic as busiest_fields.cpp, with print statements. "F3x6" means
// field 3 with 6 loads. The heap is shown best -> worst; the worst one is the
// heap's top, the next to be evicted.
#include <cstddef>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

struct Entry {
  int field;
  int count;
};

bool RanksHigher(const Entry& a, const Entry& b) {
  if (a.count != b.count) return a.count > b.count;
  return a.field < b.field;
}

using Heap = std::priority_queue<Entry, std::vector<Entry>, decltype(&RanksHigher)>;

// Takes the heap BY VALUE on purpose: we pop from the copy to look inside it,
// and the caller's heap stays untouched.
void ShowHeap(Heap copy) {
  std::vector<Entry> worst_first;
  while (!copy.empty()) {
    worst_first.push_back(copy.top());
    copy.pop();
  }
  std::cout << "      heap (best -> worst): ";
  for (std::size_t i = worst_first.size(); i-- > 0;) {
    std::cout << "F" << worst_first[i].field << "x" << worst_first[i].count << "  ";
  }
  if (!worst_first.empty()) std::cout << "   <- top (worst) = F" << worst_first[0].field;
  std::cout << "\n";
}

int main() {
  const std::vector<int> reports = {3, 7, 3, 5, 3, 7, 9, 3, 7, 5, 3, 2, 7, 3};
  const int k = 2;

  std::cout << "reports: ";
  for (const int r : reports) std::cout << r << " ";
  std::cout << "\nk = " << k << "\n\n=== STEP 1: count loads per field ===\n";

  // An ordered map ONLY so the demo prints in a fixed order. The real
  // solution uses unordered_map; its result does not depend on the order.
  std::map<int, int> loads;
  for (const int r : reports) ++loads[r];
  for (const auto& [field, count] : loads) {
    std::cout << "   field " << field << ": " << std::string(static_cast<std::size_t>(count), '#')
              << " (" << count << ")\n";
  }

  std::cout << "\n=== STEP 2: heap that holds at most k = " << k << " entries ===\n";
  Heap heap(&RanksHigher);
  for (const auto& [field, count] : loads) {
    std::cout << "\n  next: F" << field << " with " << count << " loads\n";
    heap.push({field, count});
    std::cout << "    push -> size " << heap.size() << "\n";
    if (heap.size() > static_cast<std::size_t>(k)) {
      const Entry evicted = heap.top();
      heap.pop();
      std::cout << "    size > k -> evict the worst: F" << evicted.field << "x" << evicted.count;
      if (evicted.field == field) {
        std::cout << "  (the newcomer itself: not good enough)\n";
      } else {
        std::cout << "  (newcomer F" << field << " beats it)\n";
      }
    }
    ShowHeap(heap);
  }

  std::cout << "\n=== STEP 3: pop worst-first, fill result from the back ===\n";
  std::vector<int> result(heap.size());
  for (std::size_t i = result.size(); i-- > 0;) {
    std::cout << "   pop F" << heap.top().field << " -> result[" << i << "]\n";
    result[i] = heap.top().field;
    heap.pop();
  }
  std::cout << "\nRESULT (busiest first): ";
  for (const int f : result) std::cout << "F" << f << " ";
  std::cout << "\n";
}
