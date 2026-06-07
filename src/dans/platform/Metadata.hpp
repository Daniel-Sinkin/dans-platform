// src/dans/platform/Metadata.hpp
#pragma once
// Externals
#include <dans/development_markers.hpp>
#include <dans/format.hpp>
#include <dans/strings.hpp>
#include <dans/types.hpp>
// StdLib
#include <format>
#include <string>
#include <vector>
//

namespace dans::platform
{
inline constexpr u16 k_version_major{0};
inline constexpr u16 k_version_minor{1};

struct PlatformGLFWVersion
{
    u16 major{};
    u16 minor{};
    u16 rev{};
    u16 _pad{};
};
[[nodiscard]] inline def to_string(const PlatformGLFWVersion& md) -> std::string;

struct PlatformMetadataGLFW
{
    PlatformGLFWVersion version{};
    std::string window_system_api{};
    std::string context_creation_api{};
    std::vector<std::string> additional_options_or_apis{};
};
[[nodiscard]] inline def to_string(const PlatformMetadataGLFW& md) -> std::string;

struct PlatformMetadata
{
    PlatformMetadataGLFW glfw{};

    u16 version_major{k_version_major};
    u16 version_minor{k_version_minor};
};
[[nodiscard]] inline def to_string(const PlatformMetadata& md) -> std::string;
}  // namespace dans::platform

DANS_FORMAT_WITH_TO_STRING(dans::platform::PlatformGLFWVersion)
DANS_FORMAT_WITH_TO_STRING(dans::platform::PlatformMetadataGLFW)
DANS_FORMAT_WITH_TO_STRING(dans::platform::PlatformMetadata)

// Defined after the formatter specializations so each body can format nested
// types through `{}` instead of calling to_string on them by hand.
namespace dans::platform
{
[[nodiscard]] inline def to_string(const PlatformGLFWVersion& md) -> std::string
{
    return std::format("{}.{}.{}", md.major, md.minor, md.rev);
}
[[nodiscard]] inline def to_string(const PlatformMetadataGLFW& md) -> std::string
{
    return std::format(
        "version={},window_system_api={},context_creation_api={},additional_options_or_apis=[{}]",
        md.version,
        md.window_system_api,
        md.context_creation_api,
        dans::str::join(md.additional_options_or_apis, ", ")
    );
}
[[nodiscard]] inline def to_string(const PlatformMetadata& md) -> std::string
{
    return std::format("glfw={},version={}.{}", md.glfw, md.version_major, md.version_minor);
}
}  // namespace dans::platform
