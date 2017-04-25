#ifndef DUSK_MATERIAL_H
#define DUSK_MATERIAL_H

#include <dusk/config.h>
#include <glmm/glmm.h>

typedef struct dusk_material
{
  vec3f_t ambient;
  vec3f_t diffuse;
  vec3f_t specular;

  float shininess;
  float dissolve;

  GLuint _ambient_map;
  GLuint _diffuse_map;
  GLuint _specular_map;
  GLuint _bump_map;

} dusk_material_t;

void dusk_material_init(dusk_material_t * this,
                        const char * ambient_map,
                        const char * diffuse_map,
                        const char * specular_map,
                        const char * bump_map);

void dusk_material_term(dusk_material_t * this);

#endif // DUSK_MATERIAL_H
