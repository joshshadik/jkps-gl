
#ifdef USE_WASM
#include <GLES3/gl3.h>
#include <GLES3/gl3platform.h>
#define GLFW_INCLUDE_ES3
#include <emscripten.h>
#include <emscripten/html5.h>
#include <emscripten/vr.h>
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
static ResourceManager resourceManager;
static Controls controls;

static int vrDisplay = -1;
static bool inVR = false;

static glm::mat4 vrProjectionMatrices[2];
static glm::mat4 vrViewMatrices[2];


static glm::ivec2 windowSize = glm::ivec2(1280, 720);
static glm::ivec2 size = windowSize;

double lastTime;


static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    controls.setMouseButton(button, action, mods);
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    controls.setCursorPosition(xpos / windowSize.x, ypos / windowSize.y);
}


static void window_resized(GLFWwindow* window, int width, int height)
{
    size = glm::ivec2(width, height);
    app.resize(size);
}

#ifdef USE_WASM

void report_result(int result) {
    emscripten_cancel_main_loop();
    if (result == 0) {
        printf("Test successful!\n");
    }
    else {
        printf("Test failed!\n");
    }
#ifdef REPORT_RESULT
    REPORT_RESULT(result);
#endif
}

EM_BOOL on_pointerlockchange(int eventType, const EmscriptenPointerlockChangeEvent *pointerlockChangeEvent, void *userData) {
    printf("pointerlockchange, isActive=%d\n", pointerlockChangeEvent->isActive);
    // This is the application-level workaround to sync HTML5 Pointer Lock with glfw cursor state.
    if (!pointerlockChangeEvent->isActive) {
        printf("pointerlockchange deactivated, so enabling cursor\n");
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    return 0;
}

void vr_init_callback(void* userData)
{
    printf("Browser supports WebVR\n");
    if (vrDisplay == -1) {
        int numDisplays = emscripten_vr_count_displays();
        if (numDisplays == 0) {
            printf("No VR displays found!\n");
            report_result(0);
            return;
        }

        printf("%d VR displays found\n", numDisplays);

        int id = -1;
        char *devName;
        for (int i = 0; i < numDisplays; ++i) {
            VRDisplayHandle handle = emscripten_vr_get_display_handle(i);
            if (vrDisplay == -1) {
                /* Save first found display for more testing */
                vrDisplay = handle;
                devName = const_cast<char*>(emscripten_vr_get_display_name(handle));
                printf("Using VRDisplay '%s' (displayId '%d')\n", devName, vrDisplay);

                //VRDisplayCapabilities caps;
                //if (!emscripten_vr_get_display_capabilities(handle, &caps)) {
                //    printf("Error: failed to get display capabilities.\n");
                //    report_result(1);
                //    return;
                //}
                //if (!emscripten_vr_display_connected(vrDisplay)) {
                //    printf("Error: expected display to be connected.\n");
                //    report_result(1);
                //    return;
                //}
                //printf("Display Capabilities:\n"
                //    "{hasPosition: %d, hasExternalDisplay: %d, canPresent: %d, maxLayers: %lu}\n",
                //    caps.hasPosition, caps.hasExternalDisplay, caps.canPresent, caps.maxLayers);
            }
        }

        if (vrDisplay == -1) {
            printf("Couln't find a VR display even though at least one was found.\n");
            report_result(1);
            return;
        }
    }
}


#endif



void main_loop(bool vr = false)
{
    double currTime = glfwGetTime();

    /* Poll for and process events */
    glfwPollEvents();

    app.update(currTime - lastTime);
    if (vr)
    {

        app.overrideViewProjection(vrViewMatrices[0], vrProjectionMatrices[0]);
        app.render(glm::ivec4(0, 0, size.x / 2, size.y));

        app.overrideViewProjection(vrViewMatrices[1], vrProjectionMatrices[1]);
        app.render(glm::ivec4(size.x / 2, 0, size.x / 2, size.y));
    }
    else
    {
        app.render(glm::ivec4(0, 0, size.x, size.y));
    }
    

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    controls.updateStates();

    lastTime = currTime;
}


#ifdef USE_WASM

static void printMatrix(float* m) {
    printf("{%f, %f, %f, %f,\n"
        " %f, %f, %f, %f,\n"
        " %f, %f, %f, %f,\n"
        " %f, %f, %f, %f}\n",
        m[0], m[1], m[2], m[3],
        m[4], m[5], m[6], m[7],
        m[8], m[9], m[10], m[11],
        m[12], m[13], m[14], m[15]);
}


/* Render loop without argument, set in `mainLoop()` */
static void renderLoop() {

    if (!inVR)
    {
        return;
    }

    VRFrameData data;
    if (!emscripten_vr_get_frame_data(vrDisplay, &data)) {
        printf("Could not get frame data.\n");
        report_result(1);
    }

    memcpy(&vrViewMatrices[0][0], data.leftViewMatrix, 16 * sizeof(float));
    memcpy(&vrViewMatrices[1][0], data.rightViewMatrix, 16 * sizeof(float));

    memcpy(&vrProjectionMatrices[0][0], data.leftProjectionMatrix, 16 * sizeof(float));
    memcpy(&vrProjectionMatrices[1][0], data.rightProjectionMatrix, 16 * sizeof(float));


    main_loop(true);

    if (!emscripten_vr_submit_frame(vrDisplay)) {
        printf("Error: Failed to submit frame to VR display %d (second iteration)\n", vrDisplay);
        report_result(1);
    }



}


static void requestPresentCallback(void* userData) {
    if (!emscripten_vr_set_display_render_loop(vrDisplay, renderLoop)) {
        printf("Error: Failed to dereference handle while settings display render loop of device %d\n", vrDisplay);
        report_result(1);
    }

    VREyeParameters eyeParams;
    emscripten_vr_get_eye_parameters(vrDisplay, (VREye)0, &eyeParams);

    glfwSetWindowSize(window, eyeParams.renderWidth * 2, eyeParams.renderHeight);

    inVR = true;
}

static EM_BOOL presentVRButtonEvent(int eventType, const EmscriptenMouseEvent* mouseEvent, void* userData)
{
    if (mouseEvent->button == 0 && vrDisplay != -1)
    {
        if (!inVR)
        {
            VRLayerInit init = {
                "canvas",
                VR_LAYER_DEFAULT_LEFT_BOUNDS,
                VR_LAYER_DEFAULT_RIGHT_BOUNDS
            };

            if (!emscripten_vr_request_present(vrDisplay, &init, 1, requestPresentCallback, NULL)) {
                printf("Request present with default canvas failed.\n");
                report_result(1);
                return 0;
            }


        }
        else if (inVR)
        {        
            emscripten_vr_cancel_display_render_loop(vrDisplay);
            emscripten_vr_exit_present(vrDisplay);
            inVR = false;

            glfwSetWindowSize(window, windowSize.x, windowSize.y);
        }
    }

    return 1;
}


void main_loop_wasm()
{
    if (inVR)
    {

    }
    else
    {
        main_loop();
    }
}

#endif


int main(void)
{
#ifdef USE_WASM
    if (!emscripten_vr_init(vr_init_callback, NULL)) {
        printf("Browser does not support WebVR\n");
    }
    else
    {
        emscripten_set_click_callback("enterVR", nullptr, 1, presentVRButtonEvent);
    }
#endif

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

    glfwWindowHint(GLFW_SAMPLES, 0);


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(windowSize.x, windowSize.y, "Hello World", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(window, window_resized);

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
    emscripten_set_main_loop(main_loop_wasm, 0, 1);
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