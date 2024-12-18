cmake_minimum_required(VERSION 3.15)
project(MyVulkanProject C)

# Set C standard
set(CMAKE_C_STANDARD 11)

# Enable CMake's FetchContent module to download dependencies
include(FetchContent)

# Specify GLFW as a dependency
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG latest
)
FetchContent_MakeAvailable(glfw)

# Find Vulkan
find_package(Vulkan REQUIRED)

# Include directories for Vulkan and GLFW
include_directories(${Vulkan_INCLUDE_DIRS})

# Add the source files
add_executable(${PROJECT_NAME} src/main.c)

# Link Vulkan and GLFW
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan glfw)
