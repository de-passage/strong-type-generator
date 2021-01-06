#ifndef GUARD_DPSG_META_HPP
#define GUARD_DPSG_META_HPP

#include <tuple>
#include <type_traits>

namespace dpsg {
template <class T, template <typename...> class C>
struct is_template_instance : std::false_type {};
template <template <typename...> class C, typename... Args>
struct is_template_instance<C<Args...>, C> : std::true_type {};

template <class T, template <typename...> class C>
constexpr static inline bool is_template_instance_v =
    is_template_instance<T, C>::value;

namespace detail {
template <class F, class T, class... Args>
struct is_invocable : std::false_type {};

template <class F, class... Args, class... Args2>
struct is_invocable<F, std::tuple<Args...>, Args2...>
    : std::bool_constant<
          std::conjunction_v<std::is_invocable<F, Args, Args2...>...>> {};

template <class F, class T, class... Args>
constexpr static inline bool is_invocable_v =
    is_invocable<F, T, Args...>::value;

template <class T>
struct apply_to_each;
template <class... Args1>
struct apply_to_each<std::tuple<Args1...>> {
  template <
      class F,
      class T,
      class... Args2,
      std::enable_if_t<std::is_same_v<std::tuple<Args1...>, std::decay_t<T>>,
                       int> = 0>
  static void do_it(F&& f, T&& t, Args2&&... args) noexcept {
    (std::forward<F>(f)(std::get<Args1>(std::forward<T>(t)),
                        std::forward<Args2>(args)...),
     ...);
  }
};

}  // namespace detail

template <
    class F,
    class T,
    class... Args,
    std::enable_if_t<is_template_instance_v<std::decay_t<T>, std::tuple> &&
                         detail::is_invocable_v<F, std::decay_t<T>, Args...>,
                     int> = 0>
void visit(F&& f, T&& tuple, Args&&... args) noexcept {
  detail::apply_to_each<std::decay_t<T>>::do_it(
      std::forward<F>(f), std::forward<T>(tuple), std::forward<Args>(args)...);
}

template <class T, template <class...> class C>
struct feed;
template <template <class...> class T,
          template <class...>
          class C,
          class... Args>
struct feed<T<Args...>, C> {
  using type = C<Args...>;
};
template <class T, template <class...> class C>
using feed_t = typename feed<T, C>::type;

}  // namespace dpsg

#endif  // GUARD_DPSG_META_HPP