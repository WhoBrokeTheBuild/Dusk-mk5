#ifndef DUSK_MODEL_H
#define DUSK_MODEL_H

#include <dusk/config.h>
#include <dusk/material.h>
#include <dusk/mesh.h>
#include <dusk/shader.h>
#include <glmm/glmm.h>

#ifndef alignas
#include <stdalign.h>
#endif

typedef struct dusk_model_data
{
  _Alignas(64) mat4x4_t model;
  _Alignas(64) mat4x4_t view;
  _Alignas(64) mat4x4_t proj;
  _Alignas(64) mat4x4_t mvp;

} dusk_model_data_t;

typedef struct dusk_model
{
  dusk_shader_t * _shader;
  unsigned int    _mesh_count;
  dusk_mesh_t **  _meshes;

  mat4x4_t _mat;
  bool     _mat_invalid;

  dusk_model_data_t _shader_data;

  int  _shader_data_id;
  bool _shader_data_invalid;

  vec3f_t _pos;
  vec3f_t _rot;
  vec3f_t _scale;

} dusk_model_t;

void dusk_model_init(dusk_model_t * this,
                     unsigned int    mesh_count,
                     dusk_mesh_t **  meshes,
                     dusk_shader_t * shader);

void dusk_model_term(dusk_model_t * this);

void dusk_model_render(dusk_model_t * this);

void dusk_model_get_pos(dusk_model_t * this, vec3f_t out_pos);
void dusk_model_set_pos(dusk_model_t * this, vec3f_t pos);

void dusk_model_get_rot(dusk_model_t * this, vec3f_t out_rot);
void dusk_model_set_rot(dusk_model_t * this, vec3f_t rot);

void dusk_model_get_scale(dusk_model_t * this, vec3f_t out_scale);
void dusk_model_set_scale(dusk_model_t * this, vec3f_t scale);

dusk_model_t * dusk_load_model_from_file(const char * filename, dusk_shader_t * shader);

#endif // DUSK_MODEL_H
