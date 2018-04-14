//SDL2 flashing random color example
//Should work on iOS/Android/Mac/Windows/Linux

#include "app.h"

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>


#include <stdlib.h> //rand()



static bool quitting = false;
static float r = 0.0f;
static SDL_Window *window = NULL;
static SDL_GLContext gl_context;

static App app;

void render() {

    SDL_GL_MakeCurrent(window, gl_context);

    app.render();

    SDL_GL_SwapWindow(window);

} //render


int SDLCALL watch(void *userdata, SDL_Event* event) {

    if (event->type == SDL_APP_WILLENTERBACKGROUND) {
        quitting = true;
    }

    return 1;
}

int main(int argc, char *argv[]) {


    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);


    window = SDL_CreateWindow("jiggles", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 1024, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    gl_context = SDL_GL_CreateContext(window);

    int mv;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &mv);

    //Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK)
    {
        printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
    }

    SDL_AddEventWatch(watch, NULL);


    app.init();
    while (!quitting) {

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quitting = true;
            }
        }

        render();

        SDL_Delay(2);
    }

    SDL_DelEventWatch(watch, NULL);
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    exit(0);

} //main