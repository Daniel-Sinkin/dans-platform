#pragma once
#include "dans/development_markers.hpp"
#include "dans/format.hpp"
#include "dans/logic.hpp"  // IWYU pragma: keep
#include "dans/types.hpp"
//
#include <GLFW/glfw3.h>
//
#include <array>
#include <cassert>
#include <format>
#include <string>

namespace dans::platform
{
enum class ErrorGLFWCodeE : u32
{
    no_error = static_cast<u32>(GLFW_NO_ERROR),
    not_initialized = static_cast<u32>(GLFW_NOT_INITIALIZED),
    no_current_context = static_cast<u32>(GLFW_NO_CURRENT_CONTEXT),
    invalid_enum = static_cast<u32>(GLFW_INVALID_ENUM),
    invalid_value = static_cast<u32>(GLFW_INVALID_VALUE),
    out_of_memory = static_cast<u32>(GLFW_OUT_OF_MEMORY),
    api_unavailable = static_cast<u32>(GLFW_API_UNAVAILABLE),
    version_unavailable = static_cast<u32>(GLFW_VERSION_UNAVAILABLE),
    platform_error = static_cast<u32>(GLFW_PLATFORM_ERROR),
    format_unavailable = static_cast<u32>(GLFW_FORMAT_UNAVAILABLE),
    no_window_context = static_cast<u32>(GLFW_NO_WINDOW_CONTEXT),
    cursor_unavailable = static_cast<u32>(GLFW_CURSOR_UNAVAILABLE),
    feature_unavailable = static_cast<u32>(GLFW_FEATURE_UNAVAILABLE),
    feature_unimplemented = static_cast<u32>(GLFW_FEATURE_UNIMPLEMENTED),
    platform_unavailable = static_cast<u32>(GLFW_PLATFORM_UNAVAILABLE),
};
inline constexpr std::array k_glfw_errors{
    ErrorGLFWCodeE::no_error,
    ErrorGLFWCodeE::not_initialized,
    ErrorGLFWCodeE::no_current_context,
    ErrorGLFWCodeE::invalid_enum,
    ErrorGLFWCodeE::invalid_value,
    ErrorGLFWCodeE::out_of_memory,
    ErrorGLFWCodeE::api_unavailable,
    ErrorGLFWCodeE::version_unavailable,
    ErrorGLFWCodeE::platform_error,
    ErrorGLFWCodeE::format_unavailable,
    ErrorGLFWCodeE::no_window_context,
    ErrorGLFWCodeE::cursor_unavailable,
    ErrorGLFWCodeE::feature_unavailable,
    ErrorGLFWCodeE::feature_unimplemented,
    ErrorGLFWCodeE::platform_unavailable,
};
inline constexpr usize k_glfw_error_count{k_glfw_errors.size()};

[[nodiscard]] auto to_string(ErrorGLFWCodeE err) -> std::string_view
{
    using enum ErrorGLFWCodeE;
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

struct ErrorGLFWCode
{
    ErrorGLFWCodeE code{ErrorGLFWCodeE::no_error};

    [[nodiscard]] explicit operator bool() const noexcept
    {
        return not is_error();
    }
    [[nodiscard]] auto is_error() const noexcept -> bool
    {
        return code != ErrorGLFWCodeE::no_error;
    }
};
[[nodiscard]] inline def to_string(const ErrorGLFWCode&) -> std::string;

struct ErrorGLFW
{
    ErrorGLFWCode code{};
    std::string descr{};
};
[[nodiscard]] inline def to_string(const ErrorGLFW&) -> std::string;

[[nodiscard]] inline def get_glfw_error() -> ErrorGLFW
{
    const char* descr{};
    const auto err_code = glfwGetError(&descr);
    return ErrorGLFW{
        .code = ErrorGLFWCode{ErrorGLFWCodeE(static_cast<u32>(err_code))},
        .descr = (descr) ? std::string{descr} : std::string{}
    };
}
}  // namespace dans::platform

DANS_FORMAT_WITH_TO_STRING(dans::platform::ErrorGLFWCode)
DANS_FORMAT_WITH_TO_STRING(dans::platform::ErrorGLFW)

namespace dans::platform
{
[[nodiscard]] inline def to_string(const ErrorGLFWCode& err) -> std::string
{
    return std::string{to_string(err.code)};
}

[[nodiscard]] inline def to_string(const ErrorGLFW& err) -> std::string
{
    {  // Expects
        assert(dans::logic::implies(not err.descr.empty(), err.code.is_error()));
    }
    return std::format(
        "ErrorGLFW({},{})", to_string(err.code), (err.descr.empty()) ? "\"\"" : err.descr
    );
}
}  // namespace dans::platform
