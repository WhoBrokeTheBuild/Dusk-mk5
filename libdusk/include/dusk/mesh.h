#ifndef DUSK_MESH_H
#define DUSK_MESH_H

#include <dusk/config.h>
#include <dusk/material.h>
#include <dusk/shader.h>

#define DUSK_ATTRID_VERTS 0
#define DUSK_ATTRID_NORMS 1
#define DUSK_ATTRID_TXCDS 2

typedef struct dusk_mesh_data
{
  dusk_Alignas(16) vec4f_t ambient;
  dusk_Alignas(16) vec4f_t diffuse;
  dusk_Alignas(16) vec4f_t specular;

  dusk_Alignas(4) float shininess;
  dusk_Alignas(4) float dissolve;

  dusk_Alignas(4) bool has_ambient_map;
  dusk_Alignas(4) bool has_diffuse_map;
  dusk_Alignas(4) bool has_specular_map;
  dusk_Alignas(4) bool has_bump_map;

} dusk_mesh_data_t;

typedef struct dusk_mesh
{
  unsigned int      count;
  GLuint            vao;
  dusk_material_t * _material;
  dusk_shader_t *   _shader;

  dusk_mesh_data_t _shader_data;

  int  _shader_data_id;
  bool _shader_data_invalid;

} dusk_mesh_t;

void dusk_mesh_init(dusk_mesh_t * this,
                    dusk_material_t * material,
                    unsigned int      count,
                    const float *     verts,
                    const float *     norms,
                    const float *     txcds,
                    dusk_shader_t *   shader);

void dusk_mesh_term(dusk_mesh_t * this);

void dusk_mesh_render(dusk_mesh_t * this);

#endif // DUSK_MESH_H
