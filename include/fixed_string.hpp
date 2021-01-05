#ifndef GUARD_DPSG_FIXED_STRING_HPP
#define GUARD_DPSG_FIXED_STRING_HPP

#include <cstddef>

namespace dpsg {
template <std::size_t Capacity>
struct fixed_string {
  constexpr static inline auto capacity = Capacity;
  constexpr static inline auto size = capacity - 1;
  constexpr fixed_string(const char (&value)[capacity]) noexcept {
    for (std::size_t i = 0; i < capacity; ++i) {
      this->value[i] = value[i];
    }
  }

  constexpr operator const char*() const {
    return static_cast<const char*>(value);
  }

  char value[capacity];
};

template <std::size_t S>
fixed_string(const char (&)[S]) -> fixed_string<S>;

}  // namespace dpsg

#endif  // GUARD_DPSG_FIXED_STRING_HPP