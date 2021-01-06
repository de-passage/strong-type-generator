#ifndef GUARD_DPSG_CPP_GENERATOR_HPP
#define GUARD_DPSG_CPP_GENERATOR_HPP

#include "../data_description.hpp"
#include "../meta.hpp"

#include <tuple>

namespace dpsg::generators {

struct cpp17_t {
 private:
  template <class V, template <class...> class T>
  constexpr static inline bool is = is_template_instance_v<std::decay_t<V>, T>;

  template <class V, template <class...> class T>
  constexpr static inline bool is_similar = std::is_base_of_v<feed_t<V, T>, V>;

 public:
  template <class Stream, class Value>
  void operator()(Value&& value, Stream& out, int indent = 0) const noexcept {
    namespace dd = data_description;
    using value_type = std::decay_t<Value>;
    const auto println = [&out, indent](auto&&... vals) {
      auto i = indent;
      while (i-- > 0) {
        out.put(' ');
      }
      ((out << std::forward<decltype(vals)>(vals)), ...);
      out.put('\n');
    };

    if constexpr (is<value_type, dd::struct_t>) {
      println("struct ", value.name, '{');
      visit(*this, value.members, out, indent + 1);
      println("};");
    }
    else if constexpr (is_similar<value_type, dd::variable_t>) {
      println(value_type::type_name, " ", value.name, ";");
    }
    else {
      static_assert("Unsupported type");
    }
  }

} constexpr static inline cpp17;

}  // namespace dpsg::generators

#endif  // GUARD_DPSG_CPP_GENERATOR_HPP
