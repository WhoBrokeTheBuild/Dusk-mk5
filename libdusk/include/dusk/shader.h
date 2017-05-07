#ifndef DUSK_SHADER_H
#define DUSK_SHADER_H

#include <dusk/config.h>
#include <stdbool.h>

typedef struct dusk_shader_file
{
    GLenum       type;
    const char * filename;

} dusk_shader_file_t;

typedef struct dusk_shader_data
{
    char * _name;
    GLuint _ubo;
    size_t _size;

    struct dusk_shader_data * _next;

} dusk_shader_data_t;

typedef struct dusk_shader
{
    GLuint program;
} dusk_shader_t;

bool dusk_shader_init(dusk_shader_t * this, const dusk_shader_file_t * shaders);

void dusk_shader_term(dusk_shader_t * this);

int dusk_shader_add_data(dusk_shader_t * this, const char * name, void * data, size_t size);

void dusk_shader_set_data(dusk_shader_t * this, int index, void * data);

void dusk_shader_bind(dusk_shader_t * this);

#endif // DUSK_SHADERS_H
