#include <cstdint>
#include <iostream>
#include <optional>
#include <string>
#include <type_traits>

template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
std::optional<T> plus_safe(T a, T b)
{
    using namespace std;
    bool overflow = (b > 0 && a > numeric_limits<T>::max() - b) || (b < 0 && a < numeric_limits<T>::min() - b);
    return overflow ? nullopt : make_optional(a + b);
}

int main()
{
    int64_t a;
    int64_t b;
    std::cin >> a >> b;
    const auto result = plus_safe(a, b);
    std::cout << (result.has_value() ? std::to_string(*result) : "Overflow!") << '\n';
    return 0;
}
