#ifndef DUSK_TEXTURE_H
#define DUSK_TEXTURE_H

#include <dusk/config.h>

typedef struct dusk_texture
{
  GLuint _texture;
  unsigned int width;
  unsigned int height;

} dusk_texture_t;

bool dusk_texture_init(dusk_texture_t * this, const char * filename);

void dusk_texture_term(dusk_texture_t * this);

#endif // DUSK_TEXTURE_H
