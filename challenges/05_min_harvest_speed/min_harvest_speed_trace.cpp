// Step-by-step demo for Challenge 05: binary search on the answer.
// L = lo, H = hi, M = mid, "--" marks speeds still in play.
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

long long HoursAt(const std::vector<int>& fields, int speed) {
  long long total = 0;
  for (int p : fields) total += (p + speed - 1) / speed;
  return total;
}

void Ruler(int lo, int hi, int mid, int max_speed) {
  std::cout << "      speed:";
  for (int k = 1; k <= max_speed; ++k) std::cout << std::setw(3) << k;
  std::cout << "\n      range:";
  for (int k = 1; k <= max_speed; ++k) std::cout << " " << (k >= lo && k <= hi ? "--" : "  ");
  std::cout << "\n      mark :";
  for (int k = 1; k <= max_speed; ++k) {
    std::string m = "   ";
    if (k == mid)
      m = "  M";
    else if (k == lo)
      m = "  L";
    else if (k == hi)
      m = "  H";
    std::cout << m;
  }
  std::cout << "\n";
}

int main() {
  const std::vector<int> fields = {3, 6, 7, 11};
  const int hours = 8, max_speed = 11;

  std::cout << "fields (kg): 3 6 7 11     deadline: " << hours << " hours\n\n";
  std::cout << "WHY BINARY SEARCH WORKS - hours needed at every speed:\n";
  std::cout << "      speed:";
  for (int k = 1; k <= max_speed; ++k) std::cout << std::setw(3) << k;
  std::cout << "\n      hours:";
  for (int k = 1; k <= max_speed; ++k) std::cout << std::setw(3) << HoursAt(fields, k);
  std::cout << "\n      ok?  :";
  for (int k = 1; k <= max_speed; ++k)
    std::cout << "  " << (HoursAt(fields, k) <= hours ? "Y" : "N");
  std::cout << "\n      -> pattern N N N Y Y Y Y Y Y Y Y : we want the FIRST Y\n";

  int lo = 1, hi = max_speed, iteration = 0;
  while (lo < hi) {
    const int mid = lo + (hi - lo) / 2;
    std::cout << "\n--- iteration " << ++iteration << ": lo=" << lo << " hi=" << hi
              << " mid=" << mid << " ---\n";
    Ruler(lo, hi, mid, max_speed);
    std::cout << "   hours at speed " << mid << ": ";
    long long total = 0;
    for (size_t i = 0; i < fields.size(); ++i) {
      const long long c = (fields[i] + mid - 1) / mid;
      total += c;
      std::cout << "ceil(" << fields[i] << "/" << mid << ")=" << c
                << (i + 1 < fields.size() ? " + " : "");
    }
    std::cout << " = " << total << "\n";
    if (total <= hours) {
      std::cout << "   " << total << " <= " << hours << "  -> speed " << mid
                << " WORKS -> hi = " << mid << "   (keep mid: it may be the answer)\n";
      hi = mid;
    } else {
      std::cout << "   " << total << " >  " << hours << "  -> speed " << mid
                << " too slow -> lo = " << mid + 1 << "   (mid and everything below is out)\n";
      lo = mid + 1;
    }
  }
  std::cout << "\nlo == hi == " << lo << "  -> RESULT: minimum speed = " << lo << " kg/hour\n";
}
