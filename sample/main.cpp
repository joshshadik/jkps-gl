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

static int renderLoopCalled = 0;
static bool renderLoopArgCalled = false;
static void* renderLoopArgArg = NULL;

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

void vr_init_callback(void* userData)
{
    printf("Browser supports WebVR\n");
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

/* Render loop with an argument, set in `renderLoop()` */
static void renderLoopArg(void* arg) {
    emscripten_vr_cancel_display_render_loop(vrDisplay);

    renderLoopArgCalled = true;
    renderLoopArgArg = arg;

    emscripten_vr_exit_present(vrDisplay);

    printf("Render loop with argument was called.\n");

    return;
}

static void requestPresentCallback(void* userData) {
    // Will likely never happen as the callback is not called
    // from a user-gesture in this test.
    printf("Request present callback called.\n");
}

/* Render loop without argument, set in `mainLoop()` */
static void renderLoop() {
    renderLoopCalled++;
    if (renderLoopCalled == 1) {
        /* First iteration */
        VRLayerInit init = {
            "#canvas",
            VR_LAYER_DEFAULT_LEFT_BOUNDS,
            VR_LAYER_DEFAULT_RIGHT_BOUNDS
        };
        if (!emscripten_vr_request_present(vrDisplay, &init, 1, requestPresentCallback, NULL)) {
            printf("Request present with default canvas failed.\n");
            report_result(1);
            return;
        }

        if (emscripten_vr_display_presenting(vrDisplay)) {
            /* request present needs to be called from a user gesture callback and
            * should have failed to make the display present. */
            printf("Error: Expected display not to be presenting.\n");
            report_result(1);
            return;
        }

        VRFrameData data;
        if (!emscripten_vr_get_frame_data(vrDisplay, &data)) {
            printf("Could not get frame data. (first iteration)\n");
            report_result(1);
            return;
        }
        if (!emscripten_vr_submit_frame(vrDisplay)) {
            printf("Error: Failed to submit frame to VR display %d (first iteration)\n", vrDisplay);
            report_result(1);
            return;
        }
        return;
    }
    else if (renderLoopCalled > 2) {
        printf("Error: Unexpected render loop iteration\n");
        report_result(1);
        return; // only two iterations should run code
    }

    /* Second iteration */

    emscripten_vr_cancel_display_render_loop(vrDisplay);

    VRFrameData data;
    if (!emscripten_vr_get_frame_data(vrDisplay, &data)) {
        printf("Could not get frame data.\n");
        report_result(1);
    }

    ///* Print list of properties which are reported as valid */
    //printf("The following properties are valid:\n[");
    //if (data.pose.poseFlags & VR_POSE_POSITION) {
    //    printf("position, ");
    //}
    //if (data.pose.poseFlags & VR_POSE_LINEAR_VELOCITY) {
    //    printf("linear vel, ");
    //}
    //if (data.pose.poseFlags & VR_POSE_LINEAR_ACCELERATION) {
    //    printf("linear accel, ");
    //}
    //if (data.pose.poseFlags & VR_POSE_ORIENTATION) {
    //    printf("orientation, ");
    //}
    //if (data.pose.poseFlags & VR_POSE_ANGULAR_VELOCITY) {
    //    printf("angular vel, ");
    //}
    //if (data.pose.poseFlags & VR_POSE_ANGULAR_ACCELERATION) {
    //    printf("angular accel");
    //}
    //printf("]\n");

    ///* Print all values (independent of validity) */
    //printf("Position: \t\t%f %f %f\n",
    //    data.pose.position.x,
    //    data.pose.position.y,
    //    data.pose.position.z);

    //printf("Linear Velocity: \t%f %f %f\n",
    //    data.pose.linearVelocity.x,
    //    data.pose.linearVelocity.y,
    //    data.pose.linearVelocity.z);

    //printf("Linear Acceleration: \t%f %f %f\n",
    //    data.pose.linearAcceleration.x,
    //    data.pose.linearAcceleration.y,
    //    data.pose.linearAcceleration.z);

    //printf("Orientation: \t\t%f %f %f %f\n",
    //    data.pose.orientation.x,
    //    data.pose.orientation.y,
    //    data.pose.orientation.z,
    //    data.pose.orientation.w);

    //printf("Angular Velocity: \t%f %f %f\n",
    //    data.pose.angularVelocity.x,
    //    data.pose.angularVelocity.y,
    //    data.pose.angularVelocity.z);

    //printf("Angular Acceleration: \t%f %f %f\n",
    //    data.pose.angularAcceleration.x,
    //    data.pose.angularAcceleration.y,
    //    data.pose.angularAcceleration.z);

    //printf("Left Projection Matrix:\n");
    //printMatrix(data.leftProjectionMatrix);

    //printf("Right Projection Matrix:\n");
    //printMatrix(data.rightProjectionMatrix);

    //printf("Left View Matrix:\n");
    //printMatrix(data.leftViewMatrix);

    //printf("Right View Matrix:\n");
    //printMatrix(data.rightViewMatrix);

    main_loop();

    if (!emscripten_vr_submit_frame(vrDisplay)) {
        printf("Error: Failed to submit frame to VR display %d (second iteration)\n", vrDisplay);
        report_result(1);
    }

    printf("Submitted frame.\n");

    

    if (!emscripten_vr_set_display_render_loop_arg(vrDisplay, renderLoopArg, (void*)42)) {
        printf("Error: Failed to dereference handle while settings display render loop of device %d\n", vrDisplay);
        report_result(1);
    }

    printf("Set main loop with argument to be called next.\n");
}

void main_loop_vr()
{
    static int loopcount = 0;

    if (!emscripten_vr_ready()) {
        printf("VR not ready\n");
        return;
    }

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

                VRDisplayCapabilities caps;
                if (!emscripten_vr_get_display_capabilities(handle, &caps)) {
                    printf("Error: failed to get display capabilities.\n");
                    report_result(1);
                    return;
                }
                if (!emscripten_vr_display_connected(vrDisplay)) {
                    printf("Error: expected display to be connected.\n");
                    report_result(1);
                    return;
                }
                printf("Display Capabilities:\n"
                    "{hasPosition: %d, hasExternalDisplay: %d, canPresent: %d, maxLayers: %lu}\n",
                    caps.hasPosition, caps.hasExternalDisplay, caps.canPresent, caps.maxLayers);
            }
        }

        if (vrDisplay == -1) {
            printf("Couln't find a VR display even though at least one was found.\n");
            report_result(1);
            return;
        }

        VREyeParameters leftParams, rightParams;
        emscripten_vr_get_eye_parameters(vrDisplay, VREyeLeft, &leftParams);
        emscripten_vr_get_eye_parameters(vrDisplay, VREyeRight, &rightParams);

        VREyeParameters* p = &leftParams;
        printf("Left eye params: {offset: [%f, %f, %f], renderWidth: %lu, renderHeight: %lu}\n", p->offset.x, p->offset.y, p->offset.z, p->renderWidth, p->renderHeight);

        p = &rightParams;
        printf("Right eye params: {offset: [%f, %f, %f], renderWidth: %lu, renderHeight: %lu}\n", p->offset.x, p->offset.y, p->offset.z, p->renderWidth, p->renderHeight);

        if (!emscripten_vr_set_display_render_loop(vrDisplay, renderLoop)) {
            printf("Error: Failed to dereference handle while settings display render loop of device %d\n", vrDisplay);
            report_result(1);
        }
}
}

#endif


int main(void)
{
#ifdef USE_WASM
    if (!emscripten_vr_init(vr_init_callback, NULL)) {
        printf("Browser does not support WebVR\n");
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
    emscripten_set_main_loop(main_loop_vr, 2, 0);
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