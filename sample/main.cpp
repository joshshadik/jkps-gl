#ifdef USE_WASM
#include <GLES3/gl3.h>
#include <GLES3/gl3platform.h>
#define GLFW_INCLUDE_ES3
#include <emscripten.h>
#include <emscripten/html5.h>
#else
#include <GL/glew.h>
#include <GL/GL.h>
#endif

#include <GLFW/glfw3.h>

#include "app.h"
#include "controls.h"

#include <chrono>

GLFWwindow* window;
static App app;

Controls controls;

glm::ivec2 size = glm::ivec2(1280, 720);
std::chrono::high_resolution_clock::time_point lastTimePoint;


static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    controls.setMouseButton(button, action, mods);
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    controls.setCursorPosition(xpos / size.x, ypos / size.y);
}

#ifdef USE_WASM
EM_BOOL on_pointerlockchange(int eventType, const EmscriptenPointerlockChangeEvent *pointerlockChangeEvent, void *userData) {
    printf("pointerlockchange, isActive=%d\n", pointerlockChangeEvent->isActive);
    // This is the application-level workaround to sync HTML5 Pointer Lock with glfw cursor state.
    if (!pointerlockChangeEvent->isActive) {
        printf("pointerlockchange deactivated, so enabling cursor\n");
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    
    return 0;
}
#endif

void main_loop()
{
    using namespace std::chrono;

    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    duration<double> time_span = duration_cast<duration<double>>(t1 - lastTimePoint);

    /* Poll for and process events */
    glfwPollEvents();


    app.update(time_span.count());
    app.render();

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    controls.updateStates();


    lastTimePoint = t1;
}

int main(void)
{
    //App app;


    /* Initialize the library */
    if (!glfwInit())
        return -1;

    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef USE_WASM
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#else
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
#endif

    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(size.x, size.y, "Hello World", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
#ifdef USE_WASM

#else
    glewExperimental = GL_TRUE;
    glewInit();
#endif

    app.init();
    app.resize(size);

    app.setControls(&controls);

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    lastTimePoint = std::chrono::high_resolution_clock::now();
#ifdef USE_WASM
    emscripten_set_pointerlockchange_callback(NULL, NULL, 0, on_pointerlockchange);
    emscripten_set_main_loop(main_loop, 0, 1);
#else
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        //app.render();

        main_loop();
    }
    
#endif

    glfwTerminate();
    return 0;
}