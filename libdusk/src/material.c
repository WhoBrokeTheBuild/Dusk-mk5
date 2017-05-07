#include "dusk/material.h"

#include <dusk/texture.h>

void dusk_material_init(dusk_material_t * this,
                        vec4f_t      ambient,
                        vec4f_t      diffuse,
                        vec4f_t      specular,
                        float        shininess,
                        float        dissolve,
                        const char * ambient_map,
                        const char * diffuse_map,
                        const char * specular_map,
                        const char * bump_map)
{
    vec4f_copy(&this->ambient, &ambient);
    vec4f_copy(&this->diffuse, &diffuse);
    vec4f_copy(&this->specular, &specular);

    this->shininess = shininess;
    this->dissolve  = dissolve;

    this->_ambient_map  = 0;
    this->_diffuse_map  = 0;
    this->_specular_map = 0;
    this->_bump_map     = 0;

    if (NULL != ambient_map)
    {
        this->_ambient_map = dusk_texture_load(ambient_map);
    }

    if (NULL != diffuse_map)
    {
        this->_diffuse_map = dusk_texture_load(diffuse_map);
    }

    if (NULL != specular_map)
    {
        this->_specular_map = dusk_texture_load(specular_map);
    }

    if (NULL != bump_map)
    {
        this->_bump_map = dusk_texture_load(bump_map);
    }
}

void dusk_material_term(dusk_material_t * this)
{
}

void dusk_material_bind(dusk_material_t * this)
{
    if (0 != this->_ambient_map)
    {
        glActiveTexture(GL_TEXTURE0 + DUSK_MATERIAL_AMBIENT_TEXID);
        glBindTexture(GL_TEXTURE_2D, this->_ambient_map);
    }

    if (0 != this->_diffuse_map)
    {
        glActiveTexture(GL_TEXTURE0 + DUSK_MATERIAL_DIFFUSE_TEXID);
        glBindTexture(GL_TEXTURE_2D, this->_diffuse_map);
    }

    if (0 != this->_specular_map)
    {
        glActiveTexture(GL_TEXTURE0 + DUSK_MATERIAL_SPECULAR_TEXID);
        glBindTexture(GL_TEXTURE_2D, this->_specular_map);
    }

    if (0 != this->_bump_map)
    {
        glActiveTexture(GL_TEXTURE0 + DUSK_MATERIAL_BUMP_TEXID);
        glBindTexture(GL_TEXTURE_2D, this->_bump_map);
    }
}
