#ifndef A6_UTILS_H
#define A6_UTILS_H
#include <algorithm>

namespace s21 {
template <typename T>
auto AvgOfTwo(T &a, T &b) -> T {
  return (a + b) / static_cast<T>(2);
}

auto AvgOfTwo(int a, int b) -> int { return (a + b) >> 1; }
auto AvgOfTwo(size_t a, size_t b) -> size_t { return (a + b) >> 1; }
}  // namespace s21

#endif  // A6_UTILS_H
