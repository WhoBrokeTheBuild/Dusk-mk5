#include "dusk/model.h"

#include "debug.h"
#include <dusk/dusk.h>
#include <string.h>

void _dusk_model_update_mat(dusk_model_t * this);
void _dusk_model_update_shader_data(dusk_model_t * this);

void dusk_model_init(dusk_model_t * this,
                     dusk_material_t * material,
                     unsigned int      mesh_count,
                     dusk_mesh_t *     meshes,
                     dusk_shader_t *   shader)
{
  this->_material   = material;
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
  dusk_model_data_t * data = &this->_shader_data;

  mat4x4_copy(data->model, this->_mat);
  mat4x4_copy(data->view, dusk_camera->view);
  mat4x4_copy(data->proj, dusk_camera->proj);

  mat4x4_xmul(data->mvp, data->proj, data->view);
  mat4x4_mul(data->mvp, data->model);

  if (NULL == this->_material)
  {
    vec4f_copy(data->ambient, (vec4f_t){0.2, 0.2, 0.2, 1.0});
    vec4f_copy(data->diffuse, (vec4f_t){0.0, 1.0, 0.0, 1.0});
    vec4f_copy(data->specular, (vec4f_t){0.0, 0.0, 0.0, 1.0});

    data->shininess = 32;

    data->has_ambient_map  = false;
    data->has_diffuse_map  = false;
    data->has_specular_map = false;
    data->has_bump_map     = false;
  }
  else
  {
    dusk_material_t * mat = this->_material;

    vec4f_copy(data->ambient, mat->ambient);
    vec4f_copy(data->diffuse, mat->diffuse);
    vec4f_copy(data->specular, mat->specular);

    if (mat->_diffuse_map != 0)
    {
      data->has_diffuse_map = true;
    }
  }

  this->_shader_data_invalid = false;
}
