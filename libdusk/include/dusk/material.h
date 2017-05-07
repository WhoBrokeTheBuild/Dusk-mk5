#ifndef DUSK_MATERIAL_H
#define DUSK_MATERIAL_H

#include <dusk/config.h>
#include <glmm/glmm.h>

#define DUSK_MATERIAL_AMBIENT_TEXID (0)
#define DUSK_MATERIAL_DIFFUSE_TEXID (1)
#define DUSK_MATERIAL_SPECULAR_TEXID (2)
#define DUSK_MATERIAL_BUMP_TEXID (3)

typedef struct dusk_material
{
    vec4f_t ambient;
    vec4f_t diffuse;
    vec4f_t specular;

    float shininess;
    float dissolve;

    GLuint _ambient_map;
    GLuint _diffuse_map;
    GLuint _specular_map;
    GLuint _bump_map;

} dusk_material_t;

void dusk_material_init(dusk_material_t * this,
                        vec4f_t      ambient,
                        vec4f_t      diffuse,
                        vec4f_t      specular,
                        float        shininess,
                        float        dissolve,
                        const char * ambient_map,
                        const char * diffuse_map,
                        const char * specular_map,
                        const char * bump_map);

void dusk_material_term(dusk_material_t * this);

static inline bool dusk_material_has_ambient_map(dusk_material_t * this)
{
    return 0 != this->_ambient_map;
}

static inline bool dusk_material_has_diffuse_map(dusk_material_t * this)
{
    return 0 != this->_diffuse_map;
}

static inline bool dusk_material_has_specular_map(dusk_material_t * this)
{
    return 0 != this->_specular_map;
}

static inline bool dusk_material_has_bump_map(dusk_material_t * this)
{
    return 0 != this->_bump_map;
}

void dusk_material_bind(dusk_material_t * this);

#endif // DUSK_MATERIAL_H
