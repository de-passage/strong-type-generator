#ifndef GUARD_DPSG_DATA_DESCRIPTION_HPP
#define GUARD_DPSG_DATA_DESCRIPTION_HPP

#include <ostream>
#include <tuple>

#include <iostream>
#include <type_traits>

namespace dpsg::data_description {
template <class T, class = void>
struct name_for;

template <class T>
struct name_for<
    T,
    std::void_t<decltype(std::enable_if_t<std::is_unsigned_v<T>, int>{})>> {
  struct pseudo_string {
    constexpr static inline const char prefix[] = "unsigned ";
    constexpr static inline const char* type_name =
        name_for<std::make_signed_t<T>>::value;

    template <class... Args>
    friend std::basic_ostream<Args...>& operator<<(
        std::basic_ostream<Args...>& out,
        [[maybe_unused]] pseudo_string marker) noexcept {
      return out << prefix << type_name;
    }
  } constexpr static inline value;
};

template <typename T, typename C = decltype(name_for<T>::value)>
struct named_t {
  constexpr explicit named_t(const char* name) noexcept : name{name} {}
  const char* name;

  using type = T;
  constexpr static inline C type_name = name_for<T>::value;
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

#define MAKE_NAME_STRUCT(name_)                         \
  template <>                                           \
  struct name_for<name_, void> {                        \
    constexpr static inline const char* value = #name_; \
  };

#define LIST_TYPES(f) f(float) f(double) f(int) f(char) f(long) f(long long)

LIST_TYPES(MAKE_NAME_STRUCT)

#undef LIST_TYPES
#undef MAKE_NAME_STRUCT

}  // namespace dpsg::data_description

#endif  // GUARD_DPSG_DATA_DESCRIPTION_HPP