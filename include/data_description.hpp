namespace data_description {
template <typename T>
struct type_t {};
template <typename T>
constexpr static inline auto type = type_t<T>{};
}  // namespace data_description