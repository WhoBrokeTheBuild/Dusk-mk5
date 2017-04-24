#include "dusk/model.h"

#include "debug.h"
#include <dusk/dusk.h>
#include <string.h>

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

  mat4x4_init(this->_mat, 1.0f);
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
  dusk_shader_bind(this->_shader);

  _dusk_model_update_shader_data(this);
  dusk_shader_set_data(
      this->_shader, this->_shader_data_id, &this->_shader_data);

  for (unsigned int i = 0; i < this->_mesh_count; ++i)
  {
    dusk_mesh_render(&this->_meshes[i]);
  }
}

void _dusk_model_update_shader_data(dusk_model_t * this)
{
  mat4x4_copy(this->_shader_data.model, this->_mat);
  mat4x4_copy(this->_shader_data.view, dusk_camera->view);
  mat4x4_copy(this->_shader_data.proj, dusk_camera->proj);

  mat4x4_xmul(
      this->_shader_data.mvp, this->_shader_data.proj, this->_shader_data.view);
  mat4x4_mul(this->_shader_data.mvp, this->_shader_data.model);
}
