// app/main.cpp
// Internals
#include <dans/platform/Platform.hpp>
// Externals
#include <GLFW/glfw3.h>
#include <dans/development_markers.hpp>
// StdLib
#include <print>
//

def main() -> int
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
