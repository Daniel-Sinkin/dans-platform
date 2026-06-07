// app/main.cpp
// Internals
#include <dans/chrono.hpp>
#include <dans/platform/Platform.hpp>
// Externals
#include <GLFW/glfw3.h>
#include <dans/development_markers.hpp>
// StdLib
//
/*

glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

struct CreateWindowConfig
{
    u32 width{};
    u32 height{};
    std::string name{};
    bool is_opengl{false};
};
   */

def main() -> int
{
    DANS_SCOPE_TIMER("main");
    dans::platform::Platform platform{};
    platform.create_window(
        {.width = 600, .height = 400, .name = "my_opengl_window", .is_opengl = true}
    );

    platform.run();
}
