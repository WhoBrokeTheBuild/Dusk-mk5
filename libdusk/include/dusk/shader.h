#ifndef DUSK_SHADER_H
#define DUSK_SHADER_H

#include <dusk/config.h>
#include <stdbool.h>

typedef struct dusk_shader
{
  GLuint program;
  GLuint ubo;
  size_t size;
} dusk_shader_t;

typedef struct dusk_shader_info
{
  GLenum       type;
  const char * filename;
} dusk_shader_info_t;

bool dusk_shader_init(dusk_shader_t * this,
                      void *                     data,
                      size_t                     data_size,
                      const char *               name,
                      const dusk_shader_info_t * shaders);

void dusk_shader_term(dusk_shader_t * this);

void dusk_shader_set_data(dusk_shader_t * this, void * data);

void dusk_shader_bind(dusk_shader_t * this);

#endif // DUSK_SHADERS_H
