#ifndef GUARD_DPSG_DATA_DESCRIPTION_HPP
#define GUARD_DPSG_DATA_DESCRIPTION_HPP

#include <tuple>

#include <type_traits>

namespace dpsg::data_description {
template <typename T>
struct value_t {
  using type = T;
};

template <typename T>
constexpr static inline auto value = value_t<T>{};

template <class... Args1>
struct struct_t {
  template <class... Args2>
  constexpr struct_t(const char* name, Args2&&... args) requires(
      std::conjunction_v<std::is_convertible<Args2, Args1>...>)
      : name{name}, values{std::forward<Args2>(args)...} {}

  std::tuple<Args1...> values;
  const char* name;
};

template <class... Args>
struct_t(const char* str, Args&&...)
    -> struct_t<std::remove_cvref_t<Args&&>...>;

}  // namespace dpsg::data_description

#endif  // GUARD_DPSG_DATA_DESCRIPTION_HPP