#include "dusk/material.h"

#include <SOIL/SOIL.h>

void dusk_material_init(dusk_material_t * this,
                        const char * ambient_map,
                        const char * diffuse_map,
                        const char * specular_map,
                        const char * bump_map)
{
  vec3f_init(this->ambient, 0.0f);
  vec3f_init(this->diffuse, 0.0f);
  vec3f_init(this->specular, 0.0f);

  this->shininess = 0.0f;
  this->dissolve  = 0.0f;

  this->_ambient_map  = 0;
  this->_diffuse_map  = 0;
  this->_specular_map = 0;
  this->_bump_map     = 0;

  if (NULL != ambient_map)
  {
    this->_ambient_map = SOIL_load_OGL_texture(
        ambient_map, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB |
            SOIL_FLAG_COMPRESS_TO_DXT);
  }

  if (NULL != diffuse_map)
  {
    this->_diffuse_map = SOIL_load_OGL_texture(
        diffuse_map, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB |
            SOIL_FLAG_COMPRESS_TO_DXT);
  }

  if (NULL != specular_map)
  {
    this->_specular_map = SOIL_load_OGL_texture(
        specular_map, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB |
            SOIL_FLAG_COMPRESS_TO_DXT);
  }

  if (NULL != bump_map)
  {
    this->_bump_map = SOIL_load_OGL_texture(
        bump_map, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB |
            SOIL_FLAG_COMPRESS_TO_DXT);
  }
}

void dusk_material_term(dusk_material_t * this)
{
}
