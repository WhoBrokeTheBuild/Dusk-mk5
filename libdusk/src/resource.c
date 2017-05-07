#include "dusk/resource.h"

#include "debug.h"

typedef struct dusk_resource
{
    dusk_resource_type_t type;

    bool dynamic;
    int  refcount;

    union {
        void *            ptr_data;
        dusk_mesh_t *     ptr_mesh;
        dusk_model_t *    ptr_model;
        dusk_material_t * ptr_material;
        dusk_camera_t *   ptr_camera;
        dusk_shader_t *   ptr_shader;
    };

} dusk_resource_t;

unsigned int      _resource_count    = 0;
unsigned int      _resource_capacity = 0;
dusk_resource_t * _resource_list     = NULL;

dusk_resource_t * _dusk_find_next_free_resource();

void dusk_resource_free_data(dusk_resource_t * this)
{
    if (NULL == this->ptr_data)
    {
        return;
    }

    switch (this->type)
    {
    case DUSK_RSC_DATA: free(this->ptr_data); break;
    case DUSK_RSC_MESH: dusk_mesh_term(this->ptr_mesh); break;
    case DUSK_RSC_MODEL: dusk_model_term(this->ptr_model); break;
    case DUSK_RSC_MATERIAL: dusk_material_term(this->ptr_material); break;
    case DUSK_RSC_CAMERA: dusk_camera_term(this->ptr_camera); break;
    case DUSK_RSC_SHADER: dusk_shader_term(this->ptr_shader); break;
    default:;
    }

    if (this->dynamic)
    {
        free(this->ptr_data);
    }
    this->ptr_data = NULL;
}

bool dusk_track_static_resource(dusk_resource_type_t type, void * data)
{
    dusk_resource_t * ptr = _dusk_find_next_free_resource();

    if (NULL == ptr)
    {
        DEBUG_ERROR("Failed to track resource");
        return false;
    }

    ptr->type     = type;
    ptr->dynamic  = false;
    ptr->ptr_data = data;
    ptr->refcount = 1;

    return true;
}

bool dusk_track_dynamic_resource(dusk_resource_type_t type, void * data)
{
    dusk_resource_t * ptr = _dusk_find_next_free_resource();

    if (NULL == ptr)
    {
        DEBUG_ERROR("Failed to track resource");
        return false;
    }

    ptr->type     = type;
    ptr->dynamic  = true;
    ptr->ptr_data = data;
    ptr->refcount = 1;

    return true;
}

void dusk_bind_resource(void * data)
{
    dusk_resource_t * ptr;
    for (unsigned int i = 0; i < _resource_capacity; ++i)
    {
        ptr = &_resource_list[i];

        if (ptr->ptr_data == data)
        {
            ++ptr->refcount;
            break;
        }
    }
}

void dusk_free_resource(void * data)
{
    if (NULL == data)
    {
        return;
    }

    dusk_resource_t * ptr;
    for (unsigned int i = 0; i < _resource_capacity; ++i)
    {
        ptr = &_resource_list[i];

        if (ptr->ptr_data == data)
        {
            --ptr->refcount;
            if (ptr->refcount <= 0)
            {
                --_resource_count;

                dusk_resource_free_data(ptr);
                ptr->ptr_data = NULL;
            }
        }
    }
}

unsigned int dusk_get_resource_count()
{
    return _resource_count;
}

void dusk_free_all_resources()
{
    dusk_resource_t * ptr;
    for (unsigned int i = 0; i < _resource_capacity; ++i)
    {
        ptr = &_resource_list[i];

        if (NULL != ptr->ptr_data)
        {
            --_resource_count;

            dusk_resource_free_data(ptr);
            ptr->ptr_data = NULL;
        }
    }

    if (_resource_count != 0)
    {
        // Problem?
    }

    _resource_capacity = 0;
    _resource_count    = 0;

    free(_resource_list);
    _resource_list = NULL;
}

dusk_resource_t * _dusk_find_next_free_resource()
{
    dusk_resource_t * ptr;
    unsigned int      previous_capacity = _resource_capacity;

    ++_resource_count;
    if (_resource_count > _resource_capacity)
    {
        if (NULL == _resource_list)
        {
            _resource_capacity = 10;
        }
        else
        {
            _resource_capacity *= 2;
        }

        ptr = (dusk_resource_t *)realloc(_resource_list,
                                         sizeof(dusk_resource_t) * _resource_capacity);
        if (NULL == ptr)
        {
            DEBUG_ERROR("Out of memory");
            return NULL;
        }
        _resource_list = ptr;

        memset(_resource_list + previous_capacity,
               0,
               sizeof(dusk_resource_t) * (_resource_capacity - previous_capacity));
    }

    ptr = NULL;
    for (unsigned int i = 0; i < _resource_capacity; ++i)
    {
        if (NULL == _resource_list[i].ptr_data)
        {
            ptr = &_resource_list[i];
        }
    }
    return ptr;
}
