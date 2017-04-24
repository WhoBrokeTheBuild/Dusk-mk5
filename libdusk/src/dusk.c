#include "dusk/dusk.h"

#include "debug.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

dusk_settings_t DUSK_DEFAULT_SETTINGS = {
    .window_width  = 1024,
    .window_height = 768,
    .window_title  = "Dusk",

    .update_func = NULL,
    .render_func = NULL,
};

dusk_settings_t dusk_settings;
dusk_model_t *  dusk_models[DUSK_MAX_MODELS] = {NULL};
dusk_camera_t * dusk_camera                  = NULL;

const char * dusk_version()
{
  return DUSK_VERSION_STRING;
}

void _dusk_render_cb()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  dusk_camera_update(dusk_camera);

  if (NULL != dusk_settings.update_func)
  {
    dusk_settings.update_func(NULL);
  }

  if (NULL != dusk_settings.render_func)
  {
    dusk_settings.render_func();
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

void _dusk_resize_cb(GLint width, GLint height)
{
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

  dusk_settings = DUSK_DEFAULT_SETTINGS;

  if (NULL != settings)
  {
    memcpy(&dusk_settings, settings, sizeof(dusk_settings_t));
  }
  dusk_settings.window_title = strdup(settings->window_title);

  glutInit(&argc, argv);
  glutInitContextVersion(3, 3);
  glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(dusk_settings.window_width, dusk_settings.window_height);
  glutCreateWindow(dusk_settings.window_title);

  glewExperimental = GL_TRUE;
  GLenum err       = glewInit();
  if (err != GLEW_OK)
  {
    DEBUG_ERROR("%s", glewGetErrorString(err));
    return false;
  }

  dusk_camera = malloc(sizeof(dusk_camera_t));
  dusk_camera_init(dusk_camera);
  dusk_camera_set_aspect(dusk_camera, dusk_settings.window_width, dusk_settings.window_height);
  dusk_camera_set_clip(dusk_camera, 0.001f, 10000.0f);
  dusk_camera_set_fov(dusk_camera, GLMM_RAD(45.0f));

  dusk_camera_set_pos(dusk_camera, (vec3f_t){5.0f, 5.0f, 5.0f});
  dusk_camera_set_look_at(dusk_camera, (vec3f_t){0.0f, 0.0f, 0.0f});
  dusk_camera_set_up(dusk_camera, (vec3f_t){0.0f, 1.0f, 0.0f});

  dusk_camera_update(dusk_camera);
  dusk_camera_print(dusk_camera);

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
  glutMainLoop();
}

void dusk_term()
{
  free(dusk_camera);
}
