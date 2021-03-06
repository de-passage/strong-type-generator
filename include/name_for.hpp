#ifndef GUARD_DPSG_NAME_FOR_HPP
#define GUARD_DPSG_NAME_FOR_HPP

#include "./pseudo_string.hpp"

#include <type_traits>

namespace dpsg {
template <class T, class = void>
struct name_for;

#define MAKE_NAME_STRUCT(name_)                          \
  template <>                                            \
  struct name_for<name_, void> {                         \
    constexpr static inline const char value[] = #name_; \
  };

#define LIST_TYPES(f) f(float) f(double) f(int) f(char) f(long) f(long long)

LIST_TYPES(MAKE_NAME_STRUCT)

#undef LIST_TYPES
#undef MAKE_NAME_STRUCT

template <class T>
using name_for_t =
    detail::pseudo_string_wrapper_t<decltype(name_for<T>::value)>;

template <class T>
constexpr static inline auto name_for_v = name_for<T>::value;

namespace detail {
template <class T>
using string_type_for = pseudo_string<const char*, name_for_t<T>>;
template <class T>
using string_type_for_r = pseudo_string<name_for_t<T>, const char*>;
}  // namespace detail

template <class T>
struct name_for<T, std::enable_if_t<std::is_const_v<T>>> {
  constexpr static inline detail::string_type_for<std::remove_const_t<T>> value{
      "const ",
      name_for_v<std::remove_const_t<T>>};
};

template <class T>
struct name_for<
    T,
    std::enable_if_t<std::is_volatile_v<T> && !std::is_const_v<T>>> {
  constexpr static inline detail::string_type_for<std::remove_volatile_t<T>>
      value{"volatile ", name_for_v<std::remove_volatile_t<T>>};
};

template <class T>
struct name_for<
    T,
    std::enable_if_t<std::is_unsigned_v<T> &&
                     !(std::is_const_v<T> || std::is_volatile_v<T>)>> {
  constexpr static inline detail::string_type_for<std::make_signed_t<T>> value{
      "unsigned ",
      name_for_v<std::make_signed_t<T>>};
};

template <class T>
struct name_for<
    T,
    std::enable_if_t<std::is_reference_v<T> &&
                     !(std::is_const_v<T> || std::is_volatile_v<T> ||
                       std::is_unsigned_v<T>)>> {
  constexpr static inline detail::string_type_for_r<std::remove_reference_t<T>>
      value{name_for_v<std::remove_reference_t<T>>, "&"};
};

template <class T>
struct name_for<
    T,
    std::enable_if_t<std::is_pointer_v<T> &&
                     !(std::is_const_v<T> || std::is_volatile_v<T> ||
                       std::is_unsigned_v<T> || std::is_reference_v<T>)>> {
  constexpr static inline detail::string_type_for_r<std::remove_pointer_t<T>>
      value{name_for_v<std::remove_pointer_t<T>>, "*"};
};

}  // namespace dpsg

#endif  // GUARD_DPSG_NAME_FOR_HPP