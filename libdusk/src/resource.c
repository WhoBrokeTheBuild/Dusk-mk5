#include "dusk/resource.h"

#include <stdlib.h>
#include <dusk/camera.h>
#include <dusk/material.h>
#include <dusk/mesh.h>
#include <dusk/model.h>
#include <dusk/shader.h>

typedef struct dusk_resource
{
  dusk_resource_type_t   type;
  struct dusk_resource * next;

  union {
    void *            ptr_data;
    dusk_mesh_t *     ptr_mesh;
    dusk_model_t *    ptr_model;
    dusk_material_t * ptr_material;
    dusk_camera_t *   ptr_camera;
    dusk_shader_t *   ptr_shader;
  };

} dusk_resource_t;

dusk_resource_t   root_resource;
dusk_resource_t * last_resource = &root_resource;

void dusk_resource_free_data(dusk_resource_t * this)
{
  switch (this->type)
  {
  case DUSK_RSC_DATA: free(this->ptr_data); break;
  case DUSK_RSC_MESH:
    dusk_mesh_term(this->ptr_mesh);
    free(this->ptr_mesh);
    break;
  case DUSK_RSC_MODEL:
    dusk_model_term(this->ptr_model);
    free(this->ptr_model);
    break;
  case DUSK_RSC_MATERIAL:
    dusk_material_term(this->ptr_material);
    free(this->ptr_material);
    break;
  case DUSK_RSC_CAMERA: free(this->ptr_camera); break;
  case DUSK_RSC_SHADER:
    dusk_shader_term(this->ptr_shader);
    free(this->ptr_shader);
    break;
  default:;
  }

  this->ptr_data = NULL;
}

void dusk_track_resource(dusk_resource_type_t type, void * data)
{
  dusk_resource_t * new_item = malloc(sizeof(dusk_resource_t));
  new_item->type             = type;
  new_item->next             = NULL;
  new_item->ptr_data         = data;

  last_resource->next = new_item;
  last_resource       = new_item;
}

void dusk_free_resource(void * data)
{
  dusk_resource_t * ptr  = root_resource.next;
  dusk_resource_t * last = NULL;
  while (NULL != ptr)
  {
    if (ptr->ptr_data == data)
    {
      last->next = ptr->next;

      dusk_resource_free_data(ptr);
      free(ptr);
      break;
    }
    last = ptr;
    ptr  = ptr->next;
  }
}

void dusk_free_all_resources()
{
  dusk_resource_t * ptr = root_resource.next;
  dusk_resource_t * tmp;
  while (NULL != ptr)
  {
    tmp = ptr;
    ptr = ptr->next;
    dusk_resource_free_data(ptr);
    free(ptr);
  }
}
