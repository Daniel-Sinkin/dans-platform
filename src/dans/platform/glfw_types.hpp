#pragma once
#include "dans/logic.hpp"
#include "dans/types.hpp"
//
#include <GLFW/glfw3.h>
//
#include <array>
#include <string>

namespace dans::platform
{
enum class ErrorGLFWCode : i32
{
    no_error = GLFW_NO_ERROR,
    not_initialized = GLFW_NOT_INITIALIZED,
    no_current_context = GLFW_NO_CURRENT_CONTEXT,
    invalid_enum = GLFW_INVALID_ENUM,
    invalid_value = GLFW_INVALID_VALUE,
    out_of_memory = GLFW_OUT_OF_MEMORY,
    api_unavailable = GLFW_API_UNAVAILABLE,
    version_unavailable = GLFW_VERSION_UNAVAILABLE,
    platform_error = GLFW_PLATFORM_ERROR,
    format_unavailable = GLFW_FORMAT_UNAVAILABLE,
    no_window_context = GLFW_NO_WINDOW_CONTEXT,
    cursor_unavailable = GLFW_CURSOR_UNAVAILABLE,
    feature_unavailable = GLFW_FEATURE_UNAVAILABLE,
    feature_unimplemented = GLFW_FEATURE_UNIMPLEMENTED,
    platform_unavailable = GLFW_PLATFORM_UNAVAILABLE,
};
inline constexpr std::array k_glfw_errors{
    ErrorGLFWCode::no_error,
    ErrorGLFWCode::not_initialized,
    ErrorGLFWCode::no_current_context,
    ErrorGLFWCode::invalid_enum,
    ErrorGLFWCode::invalid_value,
    ErrorGLFWCode::out_of_memory,
    ErrorGLFWCode::api_unavailable,
    ErrorGLFWCode::version_unavailable,
    ErrorGLFWCode::platform_error,
    ErrorGLFWCode::format_unavailable,
    ErrorGLFWCode::no_window_context,
    ErrorGLFWCode::cursor_unavailable,
    ErrorGLFWCode::feature_unavailable,
    ErrorGLFWCode::feature_unimplemented,
    ErrorGLFWCode::platform_unavailable,
};
inline constexpr usize k_glfw_error_count{k_glfw_errors.size()};

[[nodiscard]] auto to_string(ErrorGLFWCode err) -> std::string_view
{
    using enum ErrorGLFWCode;
    // clang-format off
    switch(err) {
        case no_error:              return "no_error";
        case not_initialized:       return "not_initialized";
        case no_current_context:    return "no_current_context";
        case invalid_enum:          return "invalid_enum";
        case invalid_value:         return "invalid_value";
        case out_of_memory:         return "out_of_memory";
        case api_unavailable:       return "api_unavailable";
        case version_unavailable:   return "version_unavailable";
        case platform_error:        return "platform_error";
        case format_unavailable:    return "format_unavailable";
        case no_window_context:     return "no_window_context";
        case cursor_unavailable:    return "cursor_unavailable";
        case feature_unavailable:   return "feature_unavailable";
        case feature_unimplemented: return "feature_unimplemented";
        case platform_unavailable:  return "platform_unavailable";
    }
    // clang-format on
    std::unreachable();
}

struct ErrorGLFW
{
    ErrorGLFWCode code{ErrorGLFWCode::no_error};
    std::string descr{};
};
[[nodiscard]] to_string(const ErrorGLFW& err)->std::string
{
    dans::logic()
}
}  // namespace dans::platform
