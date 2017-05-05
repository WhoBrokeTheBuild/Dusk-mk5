#include "dusk/dusk.h"

#include "debug.h"
#include <dusk/timer.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

dusk_settings_t DUSK_DEFAULT_SETTINGS = {
    .window_width  = 1024,
    .window_height = 768,
    .window_title  = "Dusk",

    .max_fps = 60,

    .limit_fps   = true,
    .display_fps = true,

    .update_func = NULL,
    .render_func = NULL,
};
dusk_model_t *  dusk_models[DUSK_MAX_MODELS] = {NULL};
dusk_camera_t * dusk_camera                  = NULL;

dusk_settings_t   g_settings;

SDL_Window * g_window = NULL;
SDL_GLContext g_context;

void _dusk_resize_cb(GLint width, GLint height)
{
  g_settings.window_width  = width;
  g_settings.window_height = height;
  dusk_camera_set_aspect(dusk_camera, width, height);
}

void _dusk_special_key_cb(int key, int x, int y)
{
}

void _dusk_key_cb(unsigned char key, int x, int y)
{
}

bool dusk_init(int argc, char ** argv, dusk_settings_t * settings)
{
  srand(time(0));

  g_settings = DUSK_DEFAULT_SETTINGS;

  if (NULL != settings)
  {
    memcpy(&g_settings, settings, sizeof(dusk_settings_t));
  }
  g_settings.window_title = strdup(settings->window_title);

  if (0 > SDL_Init(SDL_INIT_VIDEO))
  {
      DEBUG_ERROR("SDL_Init failed: %s", SDL_GetError());
      return false;
  }

  g_window = SDL_CreateWindow(g_settings.window_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              g_settings.window_width, g_settings.window_height, SDL_WINDOW_OPENGL);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  g_context = SDL_GL_CreateContext(g_window);

  glewExperimental = GL_TRUE;
  GLenum err       = glewInit();
  if (err != GLEW_OK)
  {
    DEBUG_ERROR("%s", glewGetErrorString(err));
    return false;
  }

  dusk_camera = malloc(sizeof(dusk_camera_t));
  dusk_track_resource(DUSK_RSC_CAMERA, dusk_camera);

  dusk_camera_init(dusk_camera);
  dusk_camera_set_aspect(dusk_camera, g_settings.window_width, g_settings.window_height);
  dusk_camera_set_clip(dusk_camera, 0.001f, 10000.0f);
  dusk_camera_set_fov(dusk_camera, GLMM_RAD(45.0f));

  dusk_camera_set_up(dusk_camera, (vec3f_t){{0.0f, 1.0f, 0.0f}});
  dusk_camera_set_pos(dusk_camera, (vec3f_t){{5.0f, 5.0f, 5.0f}});
  dusk_camera_set_look_at(dusk_camera, (vec3f_t){{0.0f, 0.0f, 0.0f}});

  dusk_camera_update(dusk_camera);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  return true;
}

void dusk_run()
{
  double       frame_delay = 1.0;
  double       fps_delay   = 250.0;
  double       frame_elap  = 0.0;
  double       fps_elap    = 0.0;
  char         title_buffer[300];
  dusk_timer_t timer;
  dusk_frame_info_t frame_info;
  double              diff_ms;
  long frames = 0;

  frame_info.fps          = 0.0f;
  frame_info.delta        = 0.0f;
  frame_info.elapsed_time = 0.0;
  frame_info.total_time   = 0.0;

  SDL_Event ev;
  while (true)
  {
    diff_ms = dusk_timer_get_ms(&timer);
    dusk_timer_start(&timer);

    SDL_PollEvent(&ev);
    if (SDL_QUIT == ev.type)
        break;

    // Update
    dusk_camera_update(dusk_camera);
    if (NULL != g_settings.update_func)
    {
      g_settings.update_func(&frame_info);
    }

    // Render
    if (!g_settings.limit_fps || frame_delay <= frame_elap)
    {
      frame_elap = 0.0;
      ++frames;

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      if (NULL != g_settings.render_func)
      {
        g_settings.render_func();
      }

      for (int i = 0; i < DUSK_MAX_MODELS; ++i)
      {
        if (dusk_models[i])
        {
          dusk_model_render(dusk_models[i]);
        }
      }

      SDL_GL_SwapWindow(g_window);
    }

    fps_elap += frame_info.elapsed_time;
    if (fps_delay <= fps_elap)
    {
      frame_info.fps = (float)((frames / fps_elap) * 1000.0f);

      frames = 0;
      fps_elap = 0.0;

      if (g_settings.display_fps)
      {
        snprintf(title_buffer, sizeof(title_buffer), "%s - %.2f", g_settings.window_title,
                 frame_info.fps);
        SDL_SetWindowTitle(g_window, title_buffer);
      }
    }

    frame_info.elapsed_time = diff_ms;
    frame_info.total_time += frame_info.elapsed_time;
    frame_info.delta = (float)(frame_info.elapsed_time / frame_delay);

    frame_elap += frame_info.elapsed_time;
    frame_delay = 1000.0 / g_settings.max_fps;
  }

}

void dusk_term()
{
  free(g_settings.window_title);

  dusk_free_all_resources();
}
