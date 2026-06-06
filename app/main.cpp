// app/main.cpp
// Externals
#include <GLFW/glfw3.h>
// StdLib
#include <print>
//

int main()
{
    std::println("Hello, dans-platform!");
    std::println("GLFW {}", glfwGetVersionString());
    return 0;
}
