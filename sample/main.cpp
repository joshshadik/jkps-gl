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

#include "resourceManager.h"

GLFWwindow* window;
static App app;

ResourceManager resourceManager;
Controls controls;

glm::ivec2 size = glm::ivec2(1280, 720);

double lastTime;


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
    double currTime = glfwGetTime();

    /* Poll for and process events */
    glfwPollEvents();

    app.update(currTime - lastTime);
    app.render();

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    controls.updateStates();

    lastTime = currTime;
}

int main(void)
{
    //App app;

    printf("app size: %d bytes \n", sizeof(App));

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

    lastTime = glfwGetTime();

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