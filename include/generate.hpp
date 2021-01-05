#ifndef GUARD_DPSG_GENERATE
#define GUARD_DPSG_GENERATE

#include "./generators/concept.hpp"

#include <iostream>

namespace dpsg {

template <class T, generator G, class... Args>
void generate(T&& something,
              [[maybe_unused]] G generator,
              std::basic_ostream<Args...>& out) {}

}  // namespace dpsg

#endif  // GUARD_DPSG_GENERATE