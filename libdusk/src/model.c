#include "dusk/model.h"

#include "debug.h"
#include <string.h>

void dusk_model_init(dusk_model_t * this,
                     unsigned int    count,
                     dusk_mesh_t *   meshes,
                     dusk_shader_t * shader)
{
  this->count  = count;
  this->meshes = (dusk_mesh_t *)malloc(sizeof(dusk_mesh_t) * this->count);
  memcpy(this->meshes, meshes, sizeof(dusk_mesh_t) * this->count);
  memcpy(&this->shader, shader, sizeof(dusk_shader_t));
}

void dusk_model_term(dusk_model_t * this)
{
}

void dusk_model_render(dusk_model_t * this)
{
  dusk_shader_bind(&this->shader);

  for (unsigned int i = 0; i < this->count; ++i)
  {
    dusk_mesh_render(&this->meshes[i]);
  }
}
