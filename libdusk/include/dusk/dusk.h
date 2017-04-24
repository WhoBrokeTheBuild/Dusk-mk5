#ifndef DUSK_H
#define DUSK_H

#include <dusk/camera.h>
#include <dusk/config.h>
#include <dusk/dmf.h>
#include <dusk/mesh.h>
#include <dusk/model.h>
#include <dusk/shader.h>

typedef struct dusk_time
{
  float  fps;
  float  delta;
  double elapsed;
  double total;

} dusk_time_t;

typedef struct dusk_settings
{
  unsigned int window_width;
  unsigned int window_height;
  char *       window_title;

  void (*update_func)(dusk_time_t *);
  void (*render_func)();

} dusk_settings_t;

const char * dusk_version();

bool dusk_init(int argc, char ** argv, dusk_settings_t * settings);

void dusk_run();

void dusk_term();

#define DUSK_MAX_MODELS 20
extern dusk_settings_t DUSK_DEFAULT_SETTINGS;
extern dusk_model_t *  dusk_models[DUSK_MAX_MODELS];
extern dusk_camera_t * dusk_camera;

#endif // DUSK_H
