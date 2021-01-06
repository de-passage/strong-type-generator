#ifndef GUARD_DPSG_DATA_DESCRIPTION_HPP
#define GUARD_DPSG_DATA_DESCRIPTION_HPP

#include <tuple>

#include <type_traits>

namespace dpsg::data_description {
template <class T>
struct name_for;

template <typename T>
struct variable_t {
  constexpr explicit variable_t(const char* name) noexcept : name{name} {}
  const char* name;

  using type = T;
  constexpr static inline const char* type_name = name_for<T>::value;
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

#define MAKE_NAME_STRUCT(name_)                         \
  template <>                                           \
  struct name_for<name_> {                              \
    constexpr static inline const char* value = #name_; \
  };

#define LIST_TYPES(f) f(float) f(double) f(int) f(char) f(long) f(long long)

LIST_TYPES(MAKE_NAME_STRUCT)

#undef LIST_TYPES
#undef MAKE_NAME_STRUCT

}  // namespace dpsg::data_description

#endif  // GUARD_DPSG_DATA_DESCRIPTION_HPP