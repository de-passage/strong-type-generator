#ifndef GUARD_DPSG_DATA_DESCRIPTION_HPP
#define GUARD_DPSG_DATA_DESCRIPTION_HPP

#include "./name_for.hpp"
#include "./pseudo_string.hpp"

#include <ostream>
#include <tuple>
#include <type_traits>

namespace dpsg::data_description {

template <typename T>
struct named_t {
  constexpr explicit named_t(const char* name) noexcept : name{name} {}
  const char* name;

  using type = T;
  constexpr static inline name_for_t<T> type_name{name_for_v<T>};
};

template <typename T>
struct variable_t : named_t<T> {
  constexpr explicit variable_t(const char* name) noexcept : named_t<T>{name} {}
};

template <typename T>
struct value_t : variable_t<T> {
  constexpr value_t() noexcept : variable_t<T>{"value"} {}
};

template <typename T>
constexpr static inline auto value = value_t<T>{};

template <class... Args1>
struct struct_t {
  template <
      class... Args2,
      std::enable_if_t<std::conjunction_v<std::is_convertible<Args2, Args1>...>,
                       int> = 0>
  constexpr struct_t(const char* name, Args2&&... args) noexcept
      : name{name}, members{std::forward<Args2>(args)...} {}

  const char* name;
  std::tuple<Args1...> members;
};

template <class... Args>
struct_t(const char* str, Args&&...)
    -> struct_t<std::remove_cv_t<std::remove_reference_t<Args>>...>;

template <class T>
struct typedef_t : named_t<T> {
  constexpr explicit typedef_t(const char* name) noexcept : named_t<T>{name} {}
};
}  // namespace dpsg::data_description

#endif  // GUARD_DPSG_DATA_DESCRIPTION_HPP