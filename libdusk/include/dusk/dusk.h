#ifndef DUSK_H
#define DUSK_H

#include <dusk/config.h>

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <glmm/glmm.h>

#include <dusk/camera.h>
#include <dusk/dmf.h>
#include <dusk/material.h>
#include <dusk/mesh.h>
#include <dusk/model.h>
#include <dusk/resource.h>
#include <dusk/shader.h>
#include <dusk/texture.h>
#include <dusk/timer.h>

typedef struct dusk_frame_info
{
    float  fps;
    float  delta;
    double elapsed_time;
    double total_time;

} dusk_frame_info_t;

typedef struct dusk_settings
{
    vec2u_t window_size;
    char *  window_title;

    float max_fps;
    bool  limit_fps;
    bool  display_fps;

} dusk_settings_t;

typedef struct dusk_callbacks
{
    void (*update)(dusk_frame_info_t *, SDL_Event *);
    void (*render)();

} dusk_callbacks_t;

static inline void dusk_print_versions()
{
    SDL_version sdlver;
    SDL_GetVersion(&sdlver);

    printf("Dusk Version: %s\n", DUSK_VERSION);
    printf("GLMM Version: %s\n", GLMM_VER_STRING);
    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("SDL Version: %d.%d.%d\n", sdlver.major, sdlver.minor, sdlver.patch);
    printf("GLEW Version: %d.%d.%d\n", GLEW_VERSION_MAJOR, GLEW_VERSION_MINOR, GLEW_VERSION_MICRO);
}

bool dusk_init(int argc, char ** argv, dusk_settings_t * settings, dusk_callbacks_t * callbacks);
void dusk_term();

void dusk_run();
void dusk_stop();

void dusk_update_settings(dusk_settings_t * settings);
void dusk_update_callbacks(dusk_callbacks_t * callbacks);

extern dusk_settings_t DUSK_DEFAULT_SETTINGS;

#endif // DUSK_H
