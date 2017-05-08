#ifndef DUSK_CAMERA_H
#define DUSK_CAMERA_H

#include <dusk/config.h>
#include <dusk/dusk.h>

#define DUSK_MAX_CAMERA_COUNT 100

typedef struct dusk_camera
{
    mat4x4_t view;
    mat4x4_t proj;

    vec3f_t _pos;
    vec3f_t _dir;
    vec3f_t _up;
    vec3f_t _look_at;
    vec3f_t _pos_delta;

    float _pitch;
    float _yaw;

    float _aspect;
    float _vnear;
    float _vfar;
    float _fov;

    bool _invalid_proj;
    bool _invalid_view;

} dusk_camera_t;

bool dusk_add_camera(dusk_camera_t * camera);
bool dusk_remove_camera(dusk_camera_t * camera);
void dusk_set_current_camera(dusk_camera_t * camera);
dusk_camera_t *  dusk_get_current_camera();
dusk_camera_t ** dusk_get_all_cameras(unsigned int * count);

void dusk_camera_init(dusk_camera_t * this);
void dusk_camera_term(dusk_camera_t * this);

void dusk_camera_print(dusk_camera_t * this);

void dusk_camera_set_aspect(dusk_camera_t * this, float width, float height);
void dusk_camera_set_clip(dusk_camera_t * this, float vnear, float vfar);
void dusk_camera_set_fov(dusk_camera_t * this, float fov);

void dusk_camera_set_pos(dusk_camera_t * this, const vec3f_t pos);
vec3f_t dusk_camera_get_pos(dusk_camera_t * this);

void dusk_camera_set_up(dusk_camera_t * this, const vec3f_t up);
vec3f_t dusk_camera_get_up(dusk_camera_t * this);

void dusk_camera_look_at(dusk_camera_t * this, const vec3f_t look_at);

void dusk_camera_move(dusk_camera_t * this, vec3f_t move);

void dusk_camera_change_pitch(dusk_camera_t * this, float angle);
void dusk_camera_change_yaw(dusk_camera_t * this, float angle);

void dusk_camera_update(dusk_camera_t * this);

#endif // DUSK_CAMERA_H
