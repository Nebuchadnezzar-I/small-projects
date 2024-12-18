// 24:11 / 1:05:54 YTB: https://www.youtube.com/watch?v=wSpwuX3c27Y&list=PLlKj-4rp1Gz0eBLIcq2wzd8uigFrJduJ-

#include <stdio.h>
#include <signal.h>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>

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
    GLFWwindow *window;
} State;

void createWindow(State *state) {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}

void init(State *state) {
    setupErrorHandling();
    createWindow(state);
}

void loop(State *state) {

}

void cleanup(State *state) {

}

int main(void) {
    State state = {

    };

    init(&state);
    loop(&state);
    cleanup(&state);

    return EXIT_SUCCESS;
}
