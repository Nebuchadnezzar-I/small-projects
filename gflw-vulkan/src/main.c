// 24:11 / 1:05:54 YTB: https://www.youtube.com/watch?v=wSpwuX3c27Y&list=PLlKj-4rp1Gz0eBLIcq2wzd8uigFrJduJ-

#include <stdio.h>
#include <signal.h>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdbool.h>
#include <vulkan/vulkan_core.h>

#define PANIC(ERROR, FORMAT, ...) {if(ERROR) { fprintf(stderr, "%s -> %s -> %i -> Error(%i):\n\t" FORMAT "\n", __FILE_NAME__, __FUNCTION__, __LINE__, ERROR, ##__VA_ARGS__); raise(SIGABRT);}}

void glfwErrorCallback(int error_code, const char *desc) {
    PANIC(error_code, "GLFW: %s", desc);
}

void exitCallback() {
    glfwTerminate();
}

void setupErrorHandling() {
    glfwSetErrorCallback(glfwErrorCallback);
    atexit(exitCallback);
}

typedef struct {
    char *window_title;
    int window_width, window_height;
    bool window_resizeable;
    bool window_fullscreen;

    uint32_t api_version;

    GLFWmonitor *monitor;
    GLFWwindow *window;

    VkAllocationCallbacks *allocator;
    VkInstance instance;
} State;

void createWindow(State *state) {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, state->window_resizeable);

    if (state->window_fullscreen) {
        state->monitor = glfwGetPrimaryMonitor();
    }

    state->window = glfwCreateWindow(
        state->window_width,
        state->window_height,
        state->window_title,
        state->monitor,
        NULL
    );
}

void createInstance(State *state) {
    uint32_t reqExtensionsCount;

    const char **req_extensions =
        glfwGetRequiredInstanceExtensions(&reqExtensionsCount);

    VkApplicationInfo applicationInfo = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .apiVersion = state->api_version,
    };

    VkInstanceCreateInfo createInfo =  {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &applicationInfo,
        .enabledExtensionCount = reqExtensionsCount,
        .ppEnabledExtensionNames = req_extensions
    };

    PANIC(vkCreateInstance(
        &createInfo,
        state->allocator,
        &state->instance
    ), "Could not creatr instance");
}

void logInfo() {
    uint32_t instanceApiVersion;
    vkEnumerateInstanceVersion(&instanceApiVersion);

    uint32_t apiVersionVariant = VK_API_VERSION_VARIANT(instanceApiVersion);
    uint32_t apiVersionMajor = VK_API_VERSION_MAJOR(instanceApiVersion);
    uint32_t apiVersionMinor = VK_API_VERSION_MINOR(instanceApiVersion);
    uint32_t apiVersionPatch = VK_API_VERSION_PATCH(instanceApiVersion);

    printf(
        "Vulkan API %i.%i.%i.%i\n",
        apiVersionVariant, apiVersionMajor, apiVersionMinor, apiVersionPatch
    );

    printf("GLFW %s\n", glfwGetVersionString());
}

void init(State *state) {
    setupErrorHandling();
    logInfo();
    createWindow(state);
    createInstance(state);
}

void loop(State *state) {
    while(!glfwWindowShouldClose(state->window)) {
        glfwPollEvents();
    }
}

void cleanup(State *state) {
    glfwDestroyWindow(state->window);
    vkDestroyInstance(state->instance, state->allocator);
    state->window = NULL;
}

int main(void) {
    State state = {
        .window_title = "AdmiralCapo",
        .window_width = 720,
        .window_height = 480,
        .window_resizeable = false,
        .window_fullscreen = false,
        .api_version = VK_API_VERSION_1_0,
    };

    init(&state);
    loop(&state);
    cleanup(&state);

    return EXIT_SUCCESS;
}
