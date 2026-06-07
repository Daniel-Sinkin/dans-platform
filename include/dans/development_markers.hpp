// include/dans/development_markers.hpp
// https://github.com/Daniel-Sinkin/dans-core v0.1

#ifndef DANS_CORE_INCLUDE_DANS_DEVELOPMENT_MARKERS_HPP
#define DANS_CORE_INCLUDE_DANS_DEVELOPMENT_MARKERS_HPP

#pragma once

#include <type_traits>  // IWYU pragma: keep

#if defined(def)
#    error "def marker macro is already defined"
#endif

// Only use this on the auto for functions when using trailing return
#define def auto

namespace dans
{
template <typename T>
[[nodiscard]] constexpr def copy(const T& value) -> std::remove_cvref_t<T>
{
    return value;
}
}  // namespace dans

#endif  // DANS_CORE_INCLUDE_DANS_DEVELOPMENT_MARKERS_HPP
