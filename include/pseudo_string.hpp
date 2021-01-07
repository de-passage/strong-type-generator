#ifndef GUARD_DPSG_PSEUDO_STRING_HPP
#define GUARD_DPSG_PSEUDO_STRING_HPP

#include <cstddef>
#include <iostream>
#include <ostream>

namespace dpsg {

namespace detail {
struct pseudo_string_char_wrapper {
  inline constexpr pseudo_string_char_wrapper(const char* str) noexcept
      : value{str} {}

  inline constexpr char operator[](std::size_t idx) const {
    return idx < size() ? value[idx] : throw;
  }

  inline constexpr char operator[](std::ptrdiff_t idx) const {
    return idx >= 0 && static_cast<std::size_t>(idx) < size() ? value[idx]
                                                              : throw;
  }

  const char* value;

 private:
  constexpr static inline std::size_t strlen(const char* str) noexcept {
    std::size_t s = 0;
    while (*str++) {
      ++s;
    }
    return s;
  }

 public:
  const std::size_t str_size = strlen(value);

  inline constexpr std::size_t size() const noexcept { return str_size; }
};

template <class E, class T>
std::basic_ostream<E, T>& operator<<(std::basic_ostream<E, T>& out,
                                     const pseudo_string_char_wrapper& w) {
  return out << w.value;
}

template <class T, class = void>
struct pseudo_string_wrapper {
  using type = T;
};

template <class T>
struct remove_pointer_const {
  using type = T;
};
template <class T>
struct remove_pointer_const<const T*> {
  using type = T*;
};

template <class T>
using wither_t = typename remove_pointer_const<std::decay_t<T>>::type;

template <class T>
struct pseudo_string_wrapper<
    T,
    std::enable_if_t<std::is_same_v<wither_t<T>, char*>>> {
  using type = pseudo_string_char_wrapper;
};

template <class T>
using pseudo_string_wrapper_t = typename pseudo_string_wrapper<T>::type;
};  // namespace detail

template <class... Args>
struct pseudo_string;
template <class T, class... R>
struct pseudo_string<T, R...> {
  constexpr explicit pseudo_string(T pref, R... rest) noexcept
      : prefix{pref}, suffix{rest...} {}
  constexpr explicit pseudo_string(T pref, pseudo_string<R...> rest) noexcept
      : prefix{pref}, suffix{rest} {}
  detail::pseudo_string_wrapper_t<T> prefix;
  pseudo_string<R...> suffix;

  const std::size_t str_size = std::size(prefix) + std::size(suffix);
  constexpr std::size_t size() const noexcept { return str_size; }
};

template <class E, class T, class A1, class... Args>
std::basic_ostream<E, T>& operator<<(std::basic_ostream<E, T>& out,
                                     const pseudo_string<A1, Args...>& str) {
  return out << str.prefix << str.suffix;
}

template <class E, class T>
std::basic_ostream<E, T>& operator<<(
    std::basic_ostream<E, T>& out,
    [[maybe_unused]] pseudo_string<> unused) noexcept {
  return out;
}

template <>
struct pseudo_string<> {
  constexpr std::size_t size() const noexcept { return 0; }
};

}  // namespace dpsg

#endif  // GUARD_DPSG_PSEUDO_STRING_HPP