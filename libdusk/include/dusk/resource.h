#ifndef DUSK_RESOURCE_H
#define DUSK_RESOURCE_H

#include <dusk/config.h>

typedef enum dusk_resource_type {
  DUSK_RSC_INVALID = -1,

  DUSK_RSC_DATA,
  DUSK_RSC_MESH,
  DUSK_RSC_MODEL,
  DUSK_RSC_MATERIAL,
  DUSK_RSC_CAMERA,
  DUSK_RSC_SHADER,

} dusk_resource_type_t;

void dusk_track_resource(dusk_resource_type_t type, void * data);

void dusk_free_resource(void * data);

void dusk_free_all_resources();

#endif // DUSK_RESOURCE_H
