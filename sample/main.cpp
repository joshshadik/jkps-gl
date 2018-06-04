#ifdef USE_WASM
#include <GLES3/gl3.h>
#include <GLES3/gl3platform.h>
#define GLFW_INCLUDE_ES3
#include <emscripten.h>
#else
#include <GL/glew.h>
#include <GL/GL.h>
#endif

#include <GLFW/glfw3.h>

#include "app.h"

GLFWwindow* window;
static App app;

void main_loop()
{
    //glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT);

    app.render();

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
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

    glm::ivec2 size = glm::ivec2(1280, 720);
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

#ifdef USE_WASM
    emscripten_set_main_loop(main_loop, 0, 1);
#else
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        //app.render();

        main_loop();
    }

    glfwTerminate();
#endif


    return 0;
}