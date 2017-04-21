#include "dusk/mesh.h"

#include "debug.h"

void dusk_mesh_init(dusk_mesh_t * this,
                    unsigned int  count,
                    const float * verts,
                    const float * norms,
                    const float * txcds)
{
  this->count = count;

  glGenVertexArrays(1, &this->vao);
  glBindVertexArray(this->vao);

  GLuint vbos[3];
  glGenBuffers(3, vbos);

  glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->count * 3, verts,
               GL_STATIC_DRAW);
  glVertexAttribPointer(DUSK_ATTRID_VERTS, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(DUSK_ATTRID_VERTS);

  if (norms)
  {
    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->count * 3, norms,
                 GL_STATIC_DRAW);
    glVertexAttribPointer(DUSK_ATTRID_NORMS, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(DUSK_ATTRID_NORMS);
  }

  if (txcds)
  {
    glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->count * 2, txcds,
                 GL_STATIC_DRAW);
    glVertexAttribPointer(DUSK_ATTRID_TXCDS, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(DUSK_ATTRID_TXCDS);
  }

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void dusk_mesh_term(dusk_mesh_t * this)
{
  glDeleteVertexArrays(1, &this->vao);
}

void dusk_mesh_render(dusk_mesh_t * this)
{
  glBindVertexArray(this->vao);
  glDrawArrays(GL_TRIANGLES, 0, this->count);
}
