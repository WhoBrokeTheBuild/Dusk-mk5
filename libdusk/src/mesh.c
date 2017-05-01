#include "dusk/mesh.h"

#include "debug.h"
#include <dusk/resource.h>

void _dusk_mesh_update_shader_data(dusk_mesh_t * this);

void dusk_mesh_init(dusk_mesh_t * this,
                    dusk_material_t * material,
                    unsigned int      count,
                    const float *     verts,
                    const float *     norms,
                    const float *     txcds,
                    dusk_shader_t *   shader)
{
  this->_material = material;
  this->_shader   = shader;
  this->count     = count;

  glGenVertexArrays(1, &this->vao);
  glBindVertexArray(this->vao);

  GLuint vbos[3];
  glGenBuffers(3, vbos);

  glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->count * 3, verts, GL_STATIC_DRAW);
  glVertexAttribPointer(DUSK_ATTRID_VERTS, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(DUSK_ATTRID_VERTS);

  if (norms)
  {
    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->count * 3, norms, GL_STATIC_DRAW);
    glVertexAttribPointer(DUSK_ATTRID_NORMS, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(DUSK_ATTRID_NORMS);
  }

  if (txcds)
  {
    glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->count * 2, txcds, GL_STATIC_DRAW);
    glVertexAttribPointer(DUSK_ATTRID_TXCDS, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(DUSK_ATTRID_TXCDS);
  }

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  _dusk_mesh_update_shader_data(this);
  this->_shader_data_id = dusk_shader_add_data(this->_shader, "MeshData", &this->_shader_data,
                                               sizeof(dusk_mesh_data_t));
}

void dusk_mesh_term(dusk_mesh_t * this)
{
  glDeleteVertexArrays(1, &this->vao);
  dusk_free_resource(this->_material);
}

void dusk_mesh_render(dusk_mesh_t * this)
{
  if (this->_shader_data_invalid)
  {
    _dusk_mesh_update_shader_data(this);
  }

  dusk_shader_bind(this->_shader);
  dusk_shader_set_data(this->_shader, this->_shader_data_id, &this->_shader_data);

  if (NULL != this->_material)
  {
    dusk_material_bind(this->_material);
  }

  glBindVertexArray(this->vao);
  glDrawArrays(GL_TRIANGLES, 0, this->count);
}

void _dusk_mesh_update_shader_data(dusk_mesh_t * this)
{
  dusk_mesh_data_t * data = &this->_shader_data;

  if (NULL == this->_material)
  {
    vec4f_copy(&data->ambient, &(vec4f_t){0.2, 0.2, 0.2, 1.0});
    vec4f_copy(&data->diffuse, &(vec4f_t){0.2, 0.2, 0.2, 1.0});
    vec4f_copy(&data->specular, &(vec4f_t){0.0, 0.0, 0.0, 1.0});

    data->shininess = 32;
    data->dissolve  = 0;

    data->has_ambient_map  = false;
    data->has_diffuse_map  = false;
    data->has_specular_map = false;
    data->has_bump_map     = false;
  }
  else
  {
    dusk_material_t * mat = this->_material;

    vec4f_copy(&data->ambient, &mat->ambient);
    vec4f_copy(&data->diffuse, &mat->diffuse);
    vec4f_copy(&data->specular, &mat->specular);

    data->shininess = mat->shininess;
    data->dissolve  = mat->dissolve;

    data->has_ambient_map  = dusk_material_has_ambient_map(mat);
    data->has_diffuse_map  = dusk_material_has_diffuse_map(mat);
    data->has_specular_map = dusk_material_has_specular_map(mat);
    data->has_bump_map     = dusk_material_has_bump_map(mat);
  }

  this->_shader_data_invalid = false;
}
