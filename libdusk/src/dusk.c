#include "dusk/dusk.h"

#include "debug.h"

vec2u_t DUSK_OPENGL_VERSION = {{3, 2}};

dusk_settings_t DUSK_DEFAULT_SETTINGS = {
    .window_size  = {{1024, 768}},
    .window_title = "Dusk",

    .max_fps     = 60,
    .limit_fps   = true,
    .display_fps = true,
};

bool _running = false;

dusk_settings_t  _settings;
dusk_callbacks_t _callbacks;

SDL_Window *  _sdl_window  = NULL;
SDL_GLContext _sdl_context = NULL;

bool _dusk_create_window();

bool dusk_init(int argc, char ** argv, dusk_settings_t * settings, dusk_callbacks_t * callbacks)
{
    srand(time(0));

    _settings = DUSK_DEFAULT_SETTINGS;
    dusk_update_settings(settings);
    dusk_update_callbacks(callbacks);

    if (!_dusk_create_window())
    {
        DEBUG_INFO("Failed to create window");
        return false;
    }

    dusk_camera_t * default_camera = malloc(sizeof(dusk_camera_t));
    dusk_track_dynamic_resource(DUSK_RSC_CAMERA, default_camera);

    dusk_add_camera(default_camera);
    dusk_set_current_camera(default_camera);

    dusk_camera_init(default_camera);
    dusk_camera_set_aspect(default_camera, _settings.window_size.x, _settings.window_size.y);
    dusk_camera_set_clip(default_camera, 0.001f, 10000.0f);
    dusk_camera_set_fov(default_camera, GLMM_RAD(45.0f));

    dusk_camera_set_up(default_camera, (vec3f_t){{0.0f, 1.0f, 0.0f}});
    dusk_camera_set_pos(default_camera, (vec3f_t){{5.0f, 5.0f, 5.0f}});
    dusk_camera_look_at(default_camera, (vec3f_t){{0.0f, 0.0f, 0.0f}});

    dusk_camera_update(default_camera);

    return true;
}

void dusk_term()
{
    if (NULL != _settings.window_title &&
        DUSK_DEFAULT_SETTINGS.window_title != _settings.window_title)
    {
        free(_settings.window_title);
        _settings.window_title = NULL;
    }

    DEBUG_INFO("Freeing %d resources", dusk_get_resource_count());
    dusk_free_all_resources();

    SDL_DestroyWindow(_sdl_window);
    _sdl_window = NULL;

    SDL_GL_DeleteContext(_sdl_context);
    _sdl_context = NULL;

    SDL_Quit();
}

void dusk_run()
{
    double frame_delay = 1.0;
    double frame_elap  = 0.0;
    double fps_delay   = 250.0;
    double fps_elap    = 0.0;
    double diff_ms;

    long frames = 0;

    char title_buffer[300];

    dusk_frame_info_t frame_info;
    memset(&frame_info, 0, sizeof(dusk_frame_info_t));

    dusk_camera_t ** cameras;
    unsigned int     camera_count;

    dusk_timer_t timer;
    dusk_timer_start(&timer);

    SDL_Event event;
    SDL_Event * ev;

    _running = true;
    while (_running)
    {
        diff_ms = dusk_timer_get_ms(&timer);
        dusk_timer_start(&timer);

        ev = NULL;
        if (SDL_PollEvent(&event))
        {
            ev = &event;
            if (SDL_QUIT == ev->type)
            {
                dusk_stop();
            }
        }

        // Update
        cameras = dusk_get_all_cameras(&camera_count);
        for (unsigned int i = 0; i < camera_count; ++i)
        {
            dusk_camera_update(cameras[i]);
        }

        if (NULL != _callbacks.update)
        {
            _callbacks.update(&frame_info, ev);
        }

        // Render
        if (!_settings.limit_fps || frame_delay <= frame_elap)
        {
            frame_elap = 0.0;
            ++frames;

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            if (NULL != _callbacks.render)
            {
                _callbacks.render();
            }

            SDL_GL_SwapWindow(_sdl_window);
        }

        fps_elap += frame_info.elapsed_time;
        if (fps_delay <= fps_elap)
        {
            frame_info.fps = (float)((frames / fps_elap) * 1000.0f);

            frames   = 0;
            fps_elap = 0.0;

            if (_settings.display_fps)
            {
                snprintf(title_buffer, sizeof(title_buffer), "%s - %.2f", _settings.window_title,
                         frame_info.fps);
                SDL_SetWindowTitle(_sdl_window, title_buffer);
            }
        }

        frame_info.elapsed_time = diff_ms;
        frame_info.total_time += frame_info.elapsed_time;
        frame_info.delta = (float)(frame_info.elapsed_time / frame_delay);

        frame_elap += frame_info.elapsed_time;
        frame_delay = 1000.0 / _settings.max_fps;
    }
}

void dusk_stop()
{
    _running = false;
}

void dusk_update_settings(dusk_settings_t * settings)
{
    if (NULL != _sdl_window)
    {
        if (_settings.window_size.x != settings->window_size.x ||
            _settings.window_size.y != settings->window_size.y)
        {
            SDL_SetWindowSize(_sdl_window, settings->window_size.x, settings->window_size.y);
        }

        if (strcmp(_settings.window_title, settings->window_title) != 0)
        {
            SDL_SetWindowTitle(_sdl_window, settings->window_title);
        }
    }

    if (NULL != _settings.window_title &&
        DUSK_DEFAULT_SETTINGS.window_title != _settings.window_title)
    {
        free(_settings.window_title);
        _settings.window_title = NULL;
    }

    if (NULL != settings)
    {
        memcpy(&_settings, settings, sizeof(dusk_settings_t));
    }
    _settings.window_title = strdup(settings->window_title);
}

void dusk_update_callbacks(dusk_callbacks_t * callbacks)
{
    if (NULL != callbacks)
    {
        memcpy(&_callbacks, callbacks, sizeof(dusk_callbacks_t));
    }
}

bool _dusk_create_window()
{
    if (0 > SDL_Init(SDL_INIT_VIDEO))
    {
        DEBUG_ERROR("SDL_Init failed: %s", SDL_GetError());
        goto error_sdl;
    }

    _sdl_window =
        SDL_CreateWindow(_settings.window_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         _settings.window_size.x, _settings.window_size.y, SDL_WINDOW_OPENGL);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, DUSK_OPENGL_VERSION.data[0]);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, DUSK_OPENGL_VERSION.data[1]);

    _sdl_context = SDL_GL_CreateContext(_sdl_window);

    glewExperimental = GL_TRUE;
    GLenum err       = glewInit();
    if (err != GLEW_OK)
    {
        DEBUG_ERROR("glewInit failed: %s", glewGetErrorString(err));
        goto error_glew;
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);
    glEnable(GL_MULTISAMPLE);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    return true;

error_glew:

error_sdl:

    SDL_DestroyWindow(_sdl_window);
    _sdl_window = NULL;

    SDL_GL_DeleteContext(_sdl_context);
    _sdl_context = NULL;

    SDL_Quit();

    return false;
}
