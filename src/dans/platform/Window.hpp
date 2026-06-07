// src/dans/platform/Window.hpp
#pragma once
// Externals
#include <GLFW/glfw3.h>
#include <dans/development_markers.hpp>
#include <dans/types.hpp>
// StdLib
#include <atomic>
#include <cassert>
#include <limits>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
//

namespace dans::platform
{
class WindowHandle
{
  public:
    WindowHandle() = default;
    explicit WindowHandle(u32 handle) : handle_(handle)
    {
    }

    static constexpr u32 k_invalid_handle{std::numeric_limits<u32>::max()};

    [[nodiscard]] def get() const noexcept -> u32
    {
        return handle_;
    }

  private:
    u32 handle_{k_invalid_handle};
};

struct WindowPos
{
    i32 x{};
    i32 y{};
};
struct WindowSize
{
    u32 width{};
    u32 height{};
};

class Window
{
  public:
    Window(usize width, usize height, std::string title)
        : handle_{next_handle_++}, title_{std::move(title)}
    {
        // GLFW copies the title internally, so it need only live for this call.
        auto* window = glfwCreateWindow(
            static_cast<int>(width), static_cast<int>(height), title_.c_str(), nullptr, nullptr
        );
        if (not window) throw std::runtime_error("Failed to create GLFW window");
        glfw_window_.reset(window);
        glfwSetWindowUserPointer(window, this);
        glfwSetWindowPosCallback(window, &on_window_pos);
        glfwSetWindowSizeCallback(window, &on_window_size);
    }

    Window(const Window&) = delete;
    def operator=(const Window&)->Window& = delete;
    Window(Window&&) = delete;
    def operator=(Window&&)->Window& = delete;
    ~Window() = default;

    [[nodiscard]] def handle() const noexcept -> u32
    {
        return handle_.get();
    }
    [[nodiscard]] def title() const noexcept -> const std::string&
    {
        return title_;
    }
    def close() noexcept -> void
    {
        glfwSetWindowShouldClose(glfw_window_.get(), true);
    }
    [[nodiscard]] def should_close() const noexcept -> bool
    {
        return glfwWindowShouldClose(glfw_window_.get());
    }
    def set_title(std::string new_title)
    {
        title_ = std::move(new_title);
        glfwSetWindowTitle(glfw_window_.get(), title_.c_str());
    }
    def set_size(usize new_width, usize new_height) -> void
    {
        glfwSetWindowSize(
            glfw_window_.get(), static_cast<int>(new_width), static_cast<int>(new_height)
        );
    }
    def make_active()
    {
        glfwMakeContextCurrent(glfw_window_.get());
    }

    [[nodiscard]] def get_framebuffer_size() const -> std::optional<WindowSize>
    {
        int fb_width{};
        int fb_height{};
        glfwGetFramebufferSize(glfw_window_.get(), &fb_width, &fb_height);
        return WindowSize{
            .width = static_cast<u32>(fb_width), .height = static_cast<u32>(fb_height)
        };
    }
    [[nodiscard]] def position_cache_valid() const noexcept -> bool
    {
        if (auto res = from_glfw_get_window_pos(); res.has_value())
        {
            const auto x_correct = res->x == pos_.x;
            const auto y_correct = res->y == pos_.y;
            return x_correct and y_correct;
        }
        return false;
    }
    [[nodiscard]] def get_position() const noexcept -> WindowPos
    {
        assert(position_cache_valid());
        return pos_;
    }
    [[nodiscard]] def get_height() const noexcept -> usize
    {
        return size_.height;
    }
    [[nodiscard]] def get_width() const noexcept -> usize
    {
        return size_.width;
    }
    [[nodiscard]] def get_size() const noexcept -> WindowSize
    {
        return size_;
    }

    static def from_glfw(GLFWwindow* window) -> Window&
    {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        assert(self != nullptr);
        return *self;
    }

  private:
    static def on_window_pos(GLFWwindow* window, int xpos, int ypos) -> void
    {
        auto& self = from_glfw(window);
        self.pos_.x = static_cast<i32>(xpos);
        self.pos_.y = static_cast<i32>(ypos);
    }
    static def on_window_size(GLFWwindow* window, int width, int height) -> void
    {
        assert(width >= 0 and height >= 0);
        auto& self = from_glfw(window);
        self.size_.width = static_cast<u32>(width);
        self.size_.height = static_cast<u32>(height);
    }

    struct WindowPosGLFW
    {
        i32 x{};
        i32 y{};
    };
    [[nodiscard]] def from_glfw_get_window_pos() const -> std::optional<WindowPosGLFW>
    {
        int xpos{};
        int ypos{};
        glfwGetWindowPos(glfw_window_.get(), &xpos, &ypos);
        return WindowPosGLFW{.x = static_cast<i32>(xpos), .y = static_cast<i32>(ypos)};
    }

    static inline std::atomic<u32> next_handle_{0};

    WindowPos pos_{};
    WindowSize size_{};

    struct WindowDeleter
    {
        auto operator()(GLFWwindow* window) const noexcept -> void
        {
            glfwDestroyWindow(window);
        }
    };
    std::unique_ptr<GLFWwindow, WindowDeleter> glfw_window_{};
    WindowHandle handle_;
    std::string title_;
};
}  // namespace dans::platform
