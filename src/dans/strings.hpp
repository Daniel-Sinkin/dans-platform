// src/dans/strings.hpp
#pragma once
// Externals
#include <dans/development_markers.hpp>
// StdLib
#include <concepts>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>
//

namespace dans::str
{
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

template <std::ranges::input_range R>
    requires std::convertible_to<std::ranges::range_value_t<R>, std::string_view>
[[nodiscard]] def join(R&& range, std::string_view sep = ", ") -> std::string
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
}  // namespace dans::str
