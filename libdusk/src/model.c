#include "dusk/model.h"

#include "debug.h"
#include <dusk/dusk.h>
#include <libgen.h>
#include <string.h>

void _dusk_model_update_mat(dusk_model_t * this);
void _dusk_model_update_shader_data(dusk_model_t * this);

void dusk_model_init(dusk_model_t * this,
                     unsigned int    mesh_count,
                     dusk_mesh_t **  meshes,
                     dusk_shader_t * shader)
{
  this->_mesh_count = mesh_count;
  this->_shader     = shader;
  this->_meshes     = (dusk_mesh_t **)malloc(sizeof(dusk_mesh_t *) * this->_mesh_count);
  memcpy(this->_meshes, meshes, sizeof(dusk_mesh_t *) * this->_mesh_count);

  vec3f_init(this->_pos, 0.0f);
  vec3f_init(this->_rot, 0.0f);
  vec3f_init(this->_scale, 1.0f);

  glUseProgram(shader->program);
  glUniform1i(glGetUniformLocation(shader->program, "ambient_map"), DUSK_MATERIAL_AMBIENT_TEXID);
  glUniform1i(glGetUniformLocation(shader->program, "diffuse_map"), DUSK_MATERIAL_DIFFUSE_TEXID);
  glUniform1i(glGetUniformLocation(shader->program, "specular_map"), DUSK_MATERIAL_SPECULAR_TEXID);
  glUniform1i(glGetUniformLocation(shader->program, "bump_map"), DUSK_MATERIAL_BUMP_TEXID);

  _dusk_model_update_mat(this);
  _dusk_model_update_shader_data(this);
  this->_shader_data_id = dusk_shader_add_data(this->_shader, "ModelData", &this->_shader_data,
                                               sizeof(dusk_model_data_t));
}

void dusk_model_term(dusk_model_t * this)
{
  for (unsigned int i = 0; i < this->_mesh_count; ++i)
  {
    dusk_free_resource(this->_meshes[i]);
  }
  free(this->_meshes);
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
  dusk_shader_set_data(this->_shader, this->_shader_data_id, &this->_shader_data);

  for (unsigned int i = 0; i < this->_mesh_count; ++i)
  {
    dusk_mesh_render(this->_meshes[i]);
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

  this->_shader_data_invalid = false;
}

dusk_model_t * dusk_load_model_from_file(const char * filename, dusk_shader_t * shader)
{
  dusk_model_t * model = malloc(sizeof(dusk_model_t));

  char * root_path = NULL;

  dmf_Model_table_t dmf;
  unsigned char *   buffer = NULL;

  unsigned int count = 0;

  float * verts = NULL;
  float * norms = NULL;
  float * txcds = NULL;

  dmf_Shape_vec_t shapes;
  unsigned int    mesh_count = 0;
  dusk_mesh_t **  meshes     = NULL;

  root_path = dirname(strdup(filename));
  dmf       = dusk_load_dmf(filename, &buffer);

  shapes     = dmf_Model_shapes(dmf);
  mesh_count = dmf_Shape_vec_len(shapes);
  meshes     = malloc(sizeof(dusk_mesh_t *) * mesh_count);

  for (unsigned int i = 0; i < dmf_Shape_vec_len(shapes); ++i)
  {
    dusk_mesh_t *     mesh     = malloc(sizeof(dusk_mesh_t));
    dusk_material_t * material = NULL;

    dmf_Shape_table_t shape = dmf_Shape_vec_at(shapes, i);
    DEBUG_INFO("Loading Shape: %s", dmf_Shape_name(shape));

    dmf_Vec3_vec_t dmf_verts = dmf_Shape_vertices(shape);
    dmf_Vec3_vec_t dmf_norms = dmf_Shape_normals(shape);
    dmf_Vec2_vec_t dmf_txcds = dmf_Shape_texcoords(shape);

    if (dmf_Shape_material_is_present(shape))
    {
      dmf_Material_table_t mat = dmf_Shape_material(shape);
      DEBUG_INFO("Loading Material: %s", dmf_Material_name(mat));

      dmf_Color_struct_t dmf_ambient      = dmf_Material_ambient(mat);
      dmf_Color_struct_t dmf_diffuse      = dmf_Material_diffuse(mat);
      dmf_Color_struct_t dmf_specular     = dmf_Material_specular(mat);
      float              shininess        = dmf_Material_shininess(mat);
      float              dissolve         = dmf_Material_dissolve(mat);
      const char *       dmf_ambient_map  = dmf_Material_ambient_map(mat);
      const char *       dmf_diffuse_map  = dmf_Material_diffuse_map(mat);
      const char *       dmf_specular_map = dmf_Material_specular_map(mat);
      const char *       dmf_bump_map     = dmf_Material_bump_map(mat);

      vec4f_t ambient  = {0.0f, 0.0f, 0.0f, 1.0f};
      vec4f_t diffuse  = {0.0f, 0.0f, 0.0f, 1.0f};
      vec4f_t specular = {0.0f, 0.0f, 0.0f, 1.0f};
      vec3f_copy(ambient, (float *)dmf_ambient);
      vec3f_copy(diffuse, (float *)dmf_diffuse);
      vec3f_copy(specular, (float *)dmf_specular);

      size_t map_filename_len;

      char * ambient_map  = NULL;
      char * diffuse_map  = NULL;
      char * specular_map = NULL;
      char * bump_map     = NULL;

      if ('\0' != dmf_ambient_map[0])
      {
        map_filename_len = strlen(root_path) + 1 + strlen(dmf_ambient_map) + 1;
        ambient_map      = malloc(map_filename_len);
        snprintf(ambient_map, map_filename_len, "%s/%s", root_path, dmf_ambient_map);
      }

      if ('\0' != dmf_diffuse_map[0])
      {
        map_filename_len = strlen(root_path) + 1 + strlen(dmf_diffuse_map) + 1;
        diffuse_map      = malloc(map_filename_len);
        snprintf(diffuse_map, map_filename_len, "%s/%s", root_path, dmf_diffuse_map);
      }

      if ('\0' != dmf_specular_map[0])
      {
        map_filename_len = strlen(root_path) + 1 + strlen(dmf_specular_map) + 1;
        specular_map     = malloc(map_filename_len);
        snprintf(specular_map, map_filename_len, "%s/%s", root_path, dmf_specular_map);
      }

      if ('\0' != dmf_bump_map[0])
      {
        map_filename_len = strlen(root_path) + 1 + strlen(dmf_bump_map) + 1;
        bump_map         = malloc(map_filename_len);
        snprintf(bump_map, map_filename_len, "%s/%s", root_path, dmf_bump_map);
      }

      material = malloc(sizeof(dusk_material_t));
      dusk_track_resource(DUSK_RSC_MATERIAL, material);
      dusk_material_init(material, ambient, diffuse, specular, shininess, dissolve, ambient_map,
                         diffuse_map, specular_map, bump_map);
    }

    count = dmf_Vec3_vec_len(dmf_verts);
    verts = (float *)dmf_verts;

    if (dmf_Vec3_vec_len(dmf_norms) > 0)
    {
      norms = (float *)dmf_norms;
    }
    if (dmf_Vec2_vec_len(dmf_txcds) > 0)
    {
      txcds = (float *)dmf_txcds;
    }

    dusk_mesh_init(mesh, material, count, verts, norms, txcds, shader);
    meshes[i] = mesh;
  }

  free(buffer);
  free(root_path);

  dusk_model_init(model, mesh_count, meshes, shader);

  free(meshes);

  return model;
}
