#include "dusk/dusk.h"

#include "debug.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

dusk_model_t * dusk_models[DUSK_MAX_MODELS] = {NULL};

const char * dusk_version()
{
  return DUSK_VERSION_STRING;
}

void _dusk_render_cb()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
}

void _dusk_special_key_cb(int key, int x, int y)
{
}

void _dusk_key_cb(unsigned char key, int x, int y)
{
}

bool dusk_init(
    int argc, char ** argv, const char * title, int width, int height)
{
  srand(time(0));

  glutInit(&argc, argv);
  glutInitContextVersion(3, 3);
  glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(width, height);
  glutCreateWindow(title);

  glewExperimental = GL_TRUE;
  GLenum err       = glewInit();
  if (err != GLEW_OK)
  {
    DEBUG_ERROR("%s", glewGetErrorString(err));
    return false;
  }

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
}
