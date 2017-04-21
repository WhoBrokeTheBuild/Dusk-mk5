#ifndef DUSK_MESH_H
#define DUSK_MESH_H

#include <dusk/config.h>

#define DUSK_ATTRID_VERTS 0
#define DUSK_ATTRID_NORMS 1
#define DUSK_ATTRID_TXCDS 2

typedef struct dusk_mesh
{
  unsigned int count;
  GLuint       vao;
} dusk_mesh_t;

void dusk_mesh_init(dusk_mesh_t * this,
                    unsigned int  count,
                    const float * verts,
                    const float * norms,
                    const float * txcds);

void dusk_mesh_term(dusk_mesh_t * this);

void dusk_mesh_render(dusk_mesh_t * this);

#endif // DUSK_MESH_H
