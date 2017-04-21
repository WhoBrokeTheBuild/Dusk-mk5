#ifndef DUSK_MODEL_H
#define DUSK_MODEL_H

#include <dusk/config.h>
#include <dusk/mesh.h>
#include <dusk/shader.h>

typedef struct dusk_model
{
  unsigned int  count;
  dusk_shader_t shader;
  dusk_mesh_t * meshes;
} dusk_model_t;

void dusk_model_init(dusk_model_t * this,
                     unsigned int    count,
                     dusk_mesh_t *   meshes,
                     dusk_shader_t * shader);

void dusk_model_term(dusk_model_t * this);

void dusk_model_render(dusk_model_t * this);

#endif // DUSK_MODEL_H
