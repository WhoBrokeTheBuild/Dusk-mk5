#include "dusk/model.h"

#include "debug.h"
#include <dusk/dusk.h>
#include <string.h>

void _dusk_model_update_mat(dusk_model_t * this);
void _dusk_model_update_shader_data(dusk_model_t * this);

void dusk_model_init(dusk_model_t * this,
                     unsigned int    mesh_count,
                     dusk_mesh_t *   meshes,
                     dusk_shader_t * shader)
{
  this->_mesh_count = mesh_count;
  this->_shader     = shader;
  this->_meshes =
      (dusk_mesh_t *)malloc(sizeof(dusk_mesh_t) * this->_mesh_count);
  memcpy(this->_meshes, meshes, sizeof(dusk_mesh_t) * this->_mesh_count);

  vec3f_init(this->_pos, 0.0f);
  vec3f_init(this->_rot, 0.0f);
  vec3f_init(this->_scale, 1.0f);

  _dusk_model_update_mat(this);
  _dusk_model_update_shader_data(this);
  this->_shader_data_id = dusk_shader_add_data(this->_shader,
                                               "ModelData",
                                               &this->_shader_data,
                                               sizeof(dusk_model_data_t));
}

void dusk_model_term(dusk_model_t * this)
{
}

void dusk_model_render(dusk_model_t * this)
{
  if (this->_mat_invalid)
  {
    _dusk_model_update_mat(this);
  }
  if (this->_shader_data_invalid)
  {
    _dusk_model_update_shader_data(this);
  }

  dusk_shader_bind(this->_shader);
  dusk_shader_set_data(
      this->_shader, this->_shader_data_id, &this->_shader_data);

  for (unsigned int i = 0; i < this->_mesh_count; ++i)
  {
    dusk_mesh_render(&this->_meshes[i]);
  }
}

void dusk_model_get_pos(dusk_model_t * this, vec3f_t out_pos)
{
  vec3f_copy(out_pos, this->_pos);
  this->_mat_invalid = true;
}

void dusk_model_set_pos(dusk_model_t * this, vec3f_t pos)
{
  vec3f_copy(this->_pos, pos);
  this->_mat_invalid = true;
}

void dusk_model_get_rot(dusk_model_t * this, vec3f_t out_rot)
{
  vec3f_copy(out_rot, this->_rot);
  this->_mat_invalid = true;
}

void dusk_model_set_rot(dusk_model_t * this, vec3f_t rot)
{
  vec3f_copy(this->_rot, rot);
  this->_mat_invalid = true;
}

void dusk_model_get_scale(dusk_model_t * this, vec3f_t out_scale)
{
  vec3f_copy(out_scale, this->_scale);
  this->_mat_invalid = true;
}

void dusk_model_set_scale(dusk_model_t * this, vec3f_t scale)
{
  vec3f_copy(this->_scale, scale);
  this->_mat_invalid = true;
}

void _dusk_model_update_mat(dusk_model_t * this)
{
  mat4x4_init(this->_mat, 1.0f);

  mat4x4_scale(this->_mat, this->_scale);

  mat4x4_rotate(this->_mat, this->_rot[0], (vec3f_t){1.0f, 0.0f, 0.0f});
  mat4x4_rotate(this->_mat, this->_rot[1], (vec3f_t){0.0f, 1.0f, 0.0f});
  mat4x4_rotate(this->_mat, this->_rot[2], (vec3f_t){0.0f, 0.0f, 1.0f});

  mat4x4_translate(this->_mat, this->_pos);

  this->_mat_invalid         = false;
  this->_shader_data_invalid = true;
}

void _dusk_model_update_shader_data(dusk_model_t * this)
{
  mat4x4_copy(this->_shader_data.model, this->_mat);
  mat4x4_copy(this->_shader_data.view, dusk_camera->view);
  mat4x4_copy(this->_shader_data.proj, dusk_camera->proj);

  mat4x4_xmul(
      this->_shader_data.mvp, this->_shader_data.proj, this->_shader_data.view);
  mat4x4_mul(this->_shader_data.mvp, this->_shader_data.model);

  this->_shader_data_invalid = false;
}
