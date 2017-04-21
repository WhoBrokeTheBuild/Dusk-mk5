#ifndef DUSK_MODEL_H
#define DUSK_MODEL_H

#include <dusk/config.h>
#include <dusk/mesh.h>
#include <dusk/shader.h>
#include <glmm/glmm.h>

typedef struct dusk_model_data
{
  mat4x4_t model;
  mat4x4_t view;
  mat4x4_t proj;
  mat4x4_t mvp;

} dusk_model_data_t;

typedef struct dusk_model
{
  dusk_shader_t * _shader;
  mat4x4_t        _mat;
  unsigned int    _mesh_count;
  dusk_mesh_t *   _meshes;

  int               _shader_data_id;
  dusk_model_data_t _shader_data;

} dusk_model_t;

void dusk_model_init(dusk_model_t * this,
                     unsigned int    mesh_count,
                     dusk_mesh_t *   meshes,
                     dusk_shader_t * shader);

void dusk_model_term(dusk_model_t * this);

void dusk_model_render(dusk_model_t * this);

#endif // DUSK_MODEL_H
