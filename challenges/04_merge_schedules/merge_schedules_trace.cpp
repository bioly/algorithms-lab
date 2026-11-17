// Step-by-step demo for Challenge 04: sort, then sweep once.
// Bars are drawn on a day line, two characters per day.
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct Interval {
  int start, end;
};

constexpr int kMaxDay = 20;

std::string Bar(int s, int e) {
  std::string line(2 * (kMaxDay + 1), ' ');
  for (int p = 2 * s; p <= 2 * e; ++p) line[p] = '=';
  line[2 * s] = '|';
  line[2 * e] = '|';
  return line;
}

void Axis() {
  std::cout << "      ";
  for (int d = 0; d <= kMaxDay; ++d)
    std::cout << (d % 2 == 0 ? (d < 10 ? " " + std::to_string(d) : std::to_string(d)) : "  ");
  std::cout << "\n";
}

std::string Label(const Interval& i) {
  std::string s = "[" + std::to_string(i.start) + "," + std::to_string(i.end) + "]";
  s.resize(7, ' ');  // pad so the bars line up
  return s;
}

int main() {
  std::vector<Interval> v = {{8, 10}, {1, 3}, {2, 6}, {3, 4}, {6, 7}, {15, 18}, {17, 20}};

  std::cout << "INPUT (unsorted)\n";
  Axis();
  for (const auto& i : v) std::cout << "  " << Label(i) << Bar(i.start, i.end) << "\n";

  std::sort(v.begin(), v.end(),
            [](const Interval& a, const Interval& b) { return a.start < b.start; });
  std::cout << "\nSORTED by start day:  ";
  for (const auto& i : v) std::cout << Label(i);
  std::cout << "\n";

  std::vector<Interval> merged = {v[0]};
  std::cout << "\nstep 0: take " << Label(v[0]) << " as the first block\n";
  for (size_t k = 1; k < v.size(); ++k) {
    const Interval cur = v[k];
    Interval& last = merged.back();
    std::cout << "\n--- step " << k << ": next = " << Label(cur) << ", last block = " << Label(last)
              << " ---\n";
    Axis();
    std::cout << "  last   " << Bar(last.start, last.end) << "\n";
    std::cout << "  next   " << Bar(cur.start, cur.end) << "\n";
    if (cur.start <= last.end) {
      const int new_end = std::max(last.end, cur.end);
      std::cout << "   " << cur.start << " <= " << last.end
                << "  -> overlap/touch -> merge, new end = max(" << last.end << "," << cur.end
                << ") = " << new_end << "\n";
      last.end = new_end;
    } else {
      std::cout << "   " << cur.start << " >  " << last.end << "  -> GAP -> start a new block\n";
      merged.push_back(cur);
    }
    std::cout << "   merged so far: ";
    for (const auto& m : merged) std::cout << Label(m);
    std::cout << "\n";
  }
  std::cout << "\nFINAL\n";
  Axis();
  for (const auto& m : merged) std::cout << "  " << Label(m) << Bar(m.start, m.end) << "\n";
}
