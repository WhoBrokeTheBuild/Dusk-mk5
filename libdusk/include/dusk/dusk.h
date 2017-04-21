#ifndef DUSK_H
#define DUSK_H

#include <dusk/camera.h>
#include <dusk/config.h>
#include <dusk/dmf.h>
#include <dusk/mesh.h>
#include <dusk/model.h>
#include <dusk/shader.h>

const char * dusk_version();

bool dusk_init(
    int argc, char ** argv, const char * title, int width, int height);

void dusk_run();
void dusk_term();

#define DUSK_MAX_MODELS 20
extern dusk_model_t *  dusk_models[DUSK_MAX_MODELS];
extern dusk_camera_t * dusk_camera;

#endif // DUSK_H
