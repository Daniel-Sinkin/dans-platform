// src/dans/strings.hpp
#pragma once
// Externals
#include <dans/development_markers.hpp>
// StdLib
#include <concepts>
#include <format>
#include <functional>
#include <ranges>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>
//
namespace dans::str
{
using std::ranges::input_range;

template <typename T>
using ValueOf = std::ranges::range_value_t<T>;
template <typename T>
using RefOf = std::ranges::range_reference_t<T>;

template <typename V>
concept StringLike = std::convertible_to<V, std::string_view>;
template <typename R>
concept StringRange = input_range<R> and StringLike<ValueOf<R>>;

template <typename V>
concept CharLike = std::convertible_to<V, char>;
template <typename R>
concept FormattableRange = input_range<R> and not StringRange<R> and CharLike<ValueOf<R>>;

template <typename Proj, typename R>
concept StringProjection =
    std::invocable<Proj&, RefOf<R>>
    and std::convertible_to<std::invoke_result_t<Proj&, RefOf<R>>, std::string_view>;

[[nodiscard]] inline def split(std::string_view text, std::string_view delim)
    -> std::vector<std::string_view>
{
    std::vector<std::string_view> out{};
    if (delim.empty())
    {
        out.push_back(text);
        return out;
    }
    auto left = 0zu;
    while (true)
    {
        const auto pos = text.find(delim, left);
        if (pos == std::string_view::npos) break;
        out.push_back(text.substr(left, pos - left));
        left = pos + delim.size();
    }
    out.push_back(text.substr(left));
    return out;
}

static constexpr std::string_view k_default_sep{", "};

template <StringRange R>
[[nodiscard]] def join(R&& range, std::string_view sep = k_default_sep) -> std::string
{
    std::string out{};
    auto first = true;
    for (const std::string_view part : range)
    {
        if (not first) out += sep;
        out += part;
        first = false;
    }
    return out;
}

template <FormattableRange R>
[[nodiscard]] def join(R&& range, std::string_view sep = k_default_sep) -> std::string
{
    std::string out{};
    auto first = true;
    for (auto&& elem : range)
    {
        if (not first) out += sep;
        out += std::format("{}", elem);
        first = false;
    }
    return out;
}

template <input_range R, StringProjection<R> Proj>
[[nodiscard]] def join(R&& range, Proj proj, std::string_view sep = k_default_sep) -> std::string
{
    std::string out{};
    auto first = true;
    for (auto&& elem : range)
    {
        if (not first) out += sep;
        out += std::invoke(proj, elem);
        first = false;
    }
    return out;
}

namespace
{  // These are tests and examples on how to use them
static_assert(StringRange<std::vector<std::string>>);
static_assert(StringRange<std::vector<std::string_view>>);
static_assert(StringRange<std::vector<const char*>>);

static_assert(not StringRange<std::vector<int>>);
static_assert(FormattableRange<std::vector<int>>);

using StringProj = decltype([](int) { return std::string{}; });
using StringViewProj = decltype([](int) { return std::string_view{}; });
static_assert(StringProjection<StringProj, std::vector<int>>);
static_assert(StringProjection<StringViewProj, std::vector<int>>);
// clang-format on
}  // namespace
}  // namespace dans::str
