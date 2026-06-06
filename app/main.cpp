// app/main.cpp
// Externals
#include <GLFW/glfw3.h>
#include <dans/types.hpp>
// StdLib
#include <atomic>
#include <cassert>
#include <limits>
#include <memory>
#include <print>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
//

namespace dans::platform
{

struct WindowDeleter
{
    auto operator()(GLFWwindow* window) const noexcept -> void
    {
        glfwDestroyWindow(window);
    }
};
using WindowPtr = std::unique_ptr<GLFWwindow, WindowDeleter>;

class WindowHandle
{
  public:
    WindowHandle() = default;
    explicit WindowHandle(u32 handle) : handle_(handle)
    {
    }

    static constexpr u32 k_invalid_handle{std::numeric_limits<u32>::max()};

    [[nodiscard]] auto get() const noexcept -> u32
    {
        return handle_;
    }

  private:
    u32 handle_{k_invalid_handle};
};

// A Window owns exactly one GLFW window: constructing it creates the GLFW
// window, destroying it destroys the GLFW window. The object is pinned (no
// copy, no move) because its address is handed to GLFW as the window user
// pointer and read back in callbacks via from_glfw.
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
        glfwSetWindowPosCallback(window, &on_pos_changed);
    }

    Window(const Window&) = delete;
    auto operator=(const Window&) -> Window& = delete;
    Window(Window&&) = delete;
    auto operator=(Window&&) -> Window& = delete;
    ~Window() = default;

    [[nodiscard]] auto get() const noexcept -> GLFWwindow*
    {
        return glfw_window_.get();
    }
    [[nodiscard]] auto handle() const noexcept -> u32
    {
        return handle_.get();
    }
    [[nodiscard]] auto title() const noexcept -> const std::string&
    {
        return title_;
    }
    [[nodiscard]] auto is_active() const noexcept -> bool
    {
        return is_active_;
    }

    auto check_should_close() -> void
    {
        if (glfwWindowShouldClose(glfw_window_.get())) is_active_ = false;
    }
    auto set_size(usize new_width, usize new_height) -> void
    {
        glfwSetWindowSize(
            glfw_window_.get(), static_cast<int>(new_width), static_cast<int>(new_height)
        );
    }

    struct FramebufferSize
    {
        usize width{};
        usize height{};
    };
    [[nodiscard]] auto get_framebuffer_size() const -> FramebufferSize
    {
        int fb_width{};
        int fb_height{};
        glfwGetFramebufferSize(glfw_window_.get(), &fb_width, &fb_height);
        return {.width = static_cast<usize>(fb_width), .height = static_cast<usize>(fb_height)};
    }

    static auto from_glfw(GLFWwindow* window) -> Window&
    {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        assert(self != nullptr);
        return *self;
    }

  private:
    static auto on_pos_changed(GLFWwindow* window, int xpos, int ypos) -> void
    {
        const auto& self = from_glfw(window);
        std::println("[{}] moved to ({}, {})", self.handle(), xpos, ypos);
    }

    static inline std::atomic<u32> next_handle_{0};

    WindowPtr glfw_window_{};
    WindowHandle handle_;
    std::string title_;
    bool is_active_{true};
};

// Owns the GLFW library: the Platform initialises GLFW on construction and
// terminates it on destruction. Single-instance because GLFW init/terminate
// is global library state, not per-object.
class Platform
{
  public:
    Platform()
    {
        if (exists_) throw std::runtime_error("Platform already exists");
        if (not glfwInit()) throw std::runtime_error("Failed to initialise GLFW");
        exists_ = true;
    }
    ~Platform()
    {
        windows_.clear();  // destroy every GLFW window before terminating GLFW
        glfwTerminate();
        exists_ = false;
    }

    Platform(const Platform&) = delete;
    auto operator=(const Platform&) -> Platform& = delete;
    Platform(Platform&&) = delete;
    auto operator=(Platform&&) -> Platform& = delete;

    auto create_window(usize width, usize height, const std::string& name) -> Window&
    {
        return *windows_.emplace_back(std::make_unique<Window>(width, height, name));
    }

    auto iteration() -> bool
    {
        glfwPollEvents();
        auto any_active = false;
        for (auto& window : windows_)
        {
            window->check_should_close();
            if (not window->is_active()) continue;
            any_active = true;
            glfwMakeContextCurrent(window->get());
        }
        return any_active;
    }
    auto run() -> void
    {
        while (iteration())
            ;
    }

  private:
    static inline bool exists_{false};

    std::vector<std::unique_ptr<Window>> windows_{};
};
}  // namespace dans::platform

auto main() -> int
{
    try
    {
        std::println("Hello, dans-platform!");
        std::println("GLFW {}", glfwGetVersionString());

        dans::platform::Platform platform{};
        platform.create_window(600zu, 400zu, "my_window");
        platform.create_window(600zu, 400zu, "my_window2");
        platform.run();
    }
    catch (...)
    {
        return 1;
    }
    return 0;
}
