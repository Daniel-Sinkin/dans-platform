// src/dans/format.hpp
#pragma once

#include <concepts>
#include <format>
#include <string>

namespace dans
{
// `to_string(const T&)` reachable by ADL, yielding something a std::string is
// constructible from.
template <typename T>
concept has_free_to_string = requires(const T& value) {
    { to_string(value) } -> std::convertible_to<std::string>;
};

// `value.to_string()` callable on a const T (so it is usable from a formatter,
// which holds the value by const ref), same result contract.
template <typename T>
concept has_member_to_string = requires(const T& value) {
    { value.to_string() } -> std::convertible_to<std::string>;
};

// Either spelling is enough; the free function wins when both exist.
template <typename T>
concept has_to_string = has_free_to_string<T> or has_member_to_string<T>;

namespace detail
{
// Stringify T, preferring the free `to_string(T)` over the member
// `T.to_string()`. The has_to_string constraint guarantees one branch is viable,
// and `if constexpr` discards the other so only the chosen call is instantiated.
template <has_to_string T>
[[nodiscard]] auto to_string_any(const T& value) -> std::string
{
    if constexpr (has_free_to_string<T>)
    {
        return to_string(value);
    }
    else
    {
        return value.to_string();
    }
}
}  // namespace detail
}  // namespace dans

/// # DANS_FORMAT_WITH_TO_STRING
/// Generate a `std::formatter<T>` that renders `T` through `to_string` -- a free
/// `to_string(const T&)` (ADL, preferred) or a member `T.to_string() const`
/// (fallback) -- so `std::format("{}", value)`, `std::print` and `std::println`
/// "just work" on `T` without writing the formatter machinery by hand.
///
/// - Invoke at GLOBAL namespace scope: an explicit specialization of
///   `std::formatter` must live in a namespace enclosing std. Put it right after
///   the type's `}  // namespace ...`, in the same header as the type.
/// - The free `to_string` is found by ADL, so it only needs to sit in T's own
///   namespace; the member form needs to be const-callable. Both must be
///   declared before this macro. When both exist, the free function wins.
/// - Inheriting `std::formatter<std::string>` reuses its `parse()`, so the
///   format spec still applies to the produced string: `{:>20}`, `{:^10}`, ... .
/// - The `static_assert` turns "neither spelling exists" into a one-line error
///   that names the type, instead of a deep error inside `std::format`.
///
/// ```cpp
/// struct Meta { ... };
/// [[nodiscard]] def to_string(const Meta&) -> std::string;
///
/// DANS_FORMAT_WITH_TO_STRING(dans::platform::Meta)   // global scope
/// std::format("meta = {}", meta);
/// ... std::format("{}", meta);                         // now valid
/// ```
#define DANS_FORMAT_WITH_TO_STRING(T)                                                              \
    static_assert(                                                                                 \
        ::dans::has_to_string<T>,                                                                  \
        "DANS_FORMAT_WITH_TO_STRING(" #T "): need a free `to_string(const " #T                     \
        "&)` or a const member `" #T "::to_string()`, returning something "                        \
        "convertible to std::string"                                                               \
    );                                                                                             \
    template <>                                                                                    \
    struct std::formatter<T> : std::formatter<std::string>                                         \
    {                                                                                              \
        auto format(const T& value, std::format_context& ctx) const                                \
            -> std::format_context::iterator                                                       \
        {                                                                                          \
            return std::formatter<std::string>::format(::dans::detail::to_string_any(value), ctx); \
        }                                                                                          \
    };
