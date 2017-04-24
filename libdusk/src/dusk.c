#include "dusk/dusk.h"

#include "debug.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DUSK_MAX_FPS 60

dusk_settings_t DUSK_DEFAULT_SETTINGS = {
    .window_width  = 1024,
    .window_height = 768,
    .window_title  = "Dusk",

    .update_func = NULL,
    .render_func = NULL,
};
dusk_model_t *  dusk_models[DUSK_MAX_MODELS] = {NULL};
dusk_camera_t * dusk_camera                  = NULL;

dusk_frame_info_t g_frame_info;
dusk_settings_t   g_settings;

const char * dusk_version()
{
  return DUSK_VERSION_STRING;
}

void _dusk_render_cb()
{
  static double frame_delay = 1.0;
  static double fps_delay   = 250.0;
  static double frame_elap  = 0.0;
  static double fps_elap    = 0.0;
  static long   frames      = 0;
  clock_t       start, diff;

  start = clock();

  dusk_camera_update(dusk_camera);

  if (NULL != g_settings.update_func)
  {
    g_settings.update_func(&g_frame_info);
  }

  if (frame_delay <= frame_elap)
  {
    ++frames;
    frame_elap = 0.0;

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

    glutSwapBuffers();
  }

  fps_elap += g_frame_info.elapsed_time;
  if (fps_delay <= fps_elap)
  {
    g_frame_info.fps = (float)((frames / fps_elap) * 1000.0f);

    frames   = 0;
    fps_elap = 0.0;
  }

  diff = clock() - start;

  g_frame_info.elapsed_time = (diff * 1000.0) / CLOCKS_PER_SEC;
  g_frame_info.total_time += g_frame_info.elapsed_time;
  g_frame_info.delta = (float)(g_frame_info.elapsed_time / frame_delay);

  frame_elap += g_frame_info.elapsed_time;
  frame_delay = 1000.0 / DUSK_MAX_FPS;
}

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

  glutInit(&argc, argv);
  glutInitContextVersion(3, 3);
  glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(g_settings.window_width, g_settings.window_height);
  glutCreateWindow(g_settings.window_title);

  glewExperimental = GL_TRUE;
  GLenum err       = glewInit();
  if (err != GLEW_OK)
  {
    DEBUG_ERROR("%s", glewGetErrorString(err));
    return false;
  }

  dusk_camera = malloc(sizeof(dusk_camera_t));
  dusk_camera_init(dusk_camera);
  dusk_camera_set_aspect(dusk_camera, g_settings.window_width,
                         g_settings.window_height);
  dusk_camera_set_clip(dusk_camera, 0.001f, 10000.0f);
  dusk_camera_set_fov(dusk_camera, GLMM_RAD(45.0f));

  dusk_camera_set_pos(dusk_camera, (vec3f_t){5.0f, 5.0f, 5.0f});
  dusk_camera_set_look_at(dusk_camera, (vec3f_t){0.0f, 0.0f, 0.0f});
  dusk_camera_set_up(dusk_camera, (vec3f_t){0.0f, 1.0f, 0.0f});

  dusk_camera_update(dusk_camera);

  // glEnable(GL_DEPTH_TEST);
  // glDepthFunc(GL_LESS);
  // glEnable(GL_BLEND);
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  glutDisplayFunc(&_dusk_render_cb);
  glutIdleFunc(&_dusk_render_cb);
  glutReshapeFunc(&_dusk_resize_cb);
  glutSpecialFunc(&_dusk_special_key_cb);
  glutKeyboardFunc(&_dusk_key_cb);

  return true;
}

void dusk_run()
{
  g_frame_info.fps          = 0.0f;
  g_frame_info.delta        = 0.0f;
  g_frame_info.elapsed_time = 0.0;
  g_frame_info.total_time   = 0.0;
  glutMainLoop();
}

void dusk_term()
{
  free(dusk_camera);
}
