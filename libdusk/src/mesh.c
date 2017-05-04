#include "dusk/mesh.h"

#include "debug.h"
#include <dusk/resource.h>

void _dusk_mesh_update_shader_data(dusk_mesh_t * this);

void dusk_mesh_init(dusk_mesh_t * this,
                    dusk_material_t * material,
                    dusk_shader_t *   shader,
                    unsigned int      count,
                    const float *     verts,
                    const float *     norms,
                    const float *     txcds)
{
  this->count      = count;
  this->_material  = material;
  this->_shader    = shader;
  this->_draw_mode = GL_TRIANGLES;

  glGenVertexArrays(1, &this->_vao);
  glBindVertexArray(this->_vao);

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
  glDeleteVertexArrays(1, &this->_vao);
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

  glBindVertexArray(this->_vao);
  glDrawArrays(this->_draw_mode, 0, this->count);
}

void dusk_mesh_create_plane(dusk_mesh_t * this,
                            dusk_material_t * material,
                            dusk_shader_t *   shader,
                            int               rows,
                            int               cols,
                            float             width,
                            float             height)
{
  this->_material  = material;
  this->_shader    = shader;
  this->_draw_mode = GL_TRIANGLE_STRIP;
  this->count      = (rows * cols) + (rows - 1) * (cols - 2);

  float square_width  = width / (float)cols;
  float square_height = height / (float)rows;

  vec3f_t * verts = malloc(sizeof(float) * this->count);
  vec3f_t * norms = malloc(sizeof(float) * this->count);
  vec2f_t * txcds = malloc(sizeof(float) * this->count);

  unsigned int ind = 0;
  for (int row = 0; row < rows - 1; ++row)
  {
    if ((row & 1) == 0)
    {
      for (int col = 0; col < cols; ++col)
      {
        verts[ind] = (vec3f_t){{row * square_width, 0.0f, col * square_height}};
        norms[ind] = (vec3f_t){{0.0f, 1.0f, 0.0f}};
        txcds[ind] = (vec2f_t){{(float)row / (float)rows, (float)col / (float)cols}};
      }
    }
    else
    {
      for (int col = cols - 1; col > 0; --col)
      {
        verts[ind] = (vec3f_t){{row * square_width, 0.0f, col * square_height}};
        norms[ind] = (vec3f_t){{0.0f, 1.0f, 0.0f}};
        txcds[ind] = (vec2f_t){{(float)row / (float)rows, (float)col / (float)cols}};
      }
    }
  }

  glGenVertexArrays(1, &this->_vao);
  glBindVertexArray(this->_vao);

  GLuint vbos[3];
  glGenBuffers(3, vbos);

  glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->count * 3, verts, GL_STATIC_DRAW);
  glVertexAttribPointer(DUSK_ATTRID_VERTS, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(DUSK_ATTRID_VERTS);

  glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->count * 3, norms, GL_STATIC_DRAW);
  glVertexAttribPointer(DUSK_ATTRID_NORMS, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(DUSK_ATTRID_NORMS);

  glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->count * 2, txcds, GL_STATIC_DRAW);
  glVertexAttribPointer(DUSK_ATTRID_TXCDS, 2, GL_FLOAT, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(DUSK_ATTRID_TXCDS);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  free(verts);
  free(norms);
  free(txcds);

  _dusk_mesh_update_shader_data(this);
  this->_shader_data_id = dusk_shader_add_data(this->_shader, "MeshData", &this->_shader_data,
                                               sizeof(dusk_mesh_data_t));
}

void _dusk_mesh_update_shader_data(dusk_mesh_t * this)
{
  dusk_mesh_data_t * data = &this->_shader_data;

  if (NULL == this->_material)
  {
    vec4f_copy(&data->ambient, &(vec4f_t){{0.2, 0.2, 0.2, 1.0}});
    vec4f_copy(&data->diffuse, &(vec4f_t){{0.2, 0.2, 0.2, 1.0}});
    vec4f_copy(&data->specular, &(vec4f_t){{0.0, 0.0, 0.0, 1.0}});

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
