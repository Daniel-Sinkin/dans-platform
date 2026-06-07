// src/dans/platform/Platform.hpp
#pragma once
// Internals
#include <dans/platform/Metadata.hpp>
#include <dans/platform/Window.hpp>
// Externals
#include <GLFW/glfw3.h>
#include <dans/development_markers.hpp>
#include <dans/strings.hpp>
#include <dans/types.hpp>
// StdLib
#include <format>
#include <memory>
#include <print>
#include <stdexcept>
#include <string>
#include <vector>
//

namespace
{
using namespace dans;
using namespace dans::platform;

[[nodiscard]] auto get_gflw_metadata() -> PlatformMetadataGLFW
{
    PlatformMetadataGLFW out{};
    int major{};
    int minor{};
    int rev{};
    glfwGetVersion(&major, &minor, &rev);
    out.version = {
        .major = static_cast<u16>(major),
        .minor = static_cast<u16>(minor),
        .rev = static_cast<u16>(rev),
    };
    const auto* version_string_raw = glfwGetVersionString();
    if (!version_string_raw) throw std::runtime_error("Failed to get GLFW version string");
    const std::string version_string{version_string_raw};
    const auto splits = dans::str::split(version_string, " ");
    if (splits.size() < 3)
    {
        throw std::runtime_error(
            std::format("Got {} in the version string split but expected at least 3", splits.size())
        );
    }
    out.window_system_api = std::string{splits[1]};
    out.context_creation_api = std::string{splits[2]};
    out.additional_options_or_apis.reserve(splits.size() - 3zu);
    for (auto i = 3zu; i < splits.size(); ++i)
    {
        out.additional_options_or_apis.emplace_back(splits[i]);
    }
    return out;
}
}  // namespace

namespace dans::platform
{
class Platform
{
  public:
    Platform()
    {
        if (exists_) throw std::runtime_error("Platform already exists");
        if (not glfwInit()) throw std::runtime_error("Failed to initialise GLFW");
        exists_ = true;
        metadata_.glfw = get_gflw_metadata();
    }
    ~Platform()
    {
        windows_.clear();
        glfwTerminate();
        exists_ = false;
    }

    Platform(const Platform&) = delete;
    def operator=(const Platform&)->Platform& = delete;
    Platform(Platform&&) = delete;
    def operator=(Platform&&)->Platform& = delete;

    [[nodiscard]] def get_metadata() const noexcept -> const PlatformMetadata&
    {
        return metadata_;
    }
    def print_metadata() const -> void
    {
        std::println("{}", metadata_);
    }

    def create_window(usize width, usize height, const std::string& name) -> Window&
    {
        return *windows_.emplace_back(std::make_unique<Window>(width, height, name));
    }

    def iteration() -> bool
    {
        glfwPollEvents();
        const auto glfw_err_code = glfwGetError(nullptr);
        if (glfw_err_code != GLFW_NO_ERROR)
        {
        }

        // Handle window closing
        std::erase_if(windows_, [](const auto& window) { return window->should_close(); });

        auto any_active = false;
        for (auto& window : windows_)
        {
            any_active = true;
            window->make_active();
        }
        return any_active;
    }
    def run() -> void
    {
        while (iteration())
            ;
    }

  private:
    static inline bool exists_{false};

    PlatformMetadata metadata_{};

    std::vector<std::unique_ptr<Window>> windows_{};
};
}  // namespace dans::platform
