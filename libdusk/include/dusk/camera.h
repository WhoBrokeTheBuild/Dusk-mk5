#ifndef DUSK_CAMERA_H
#define DUSK_CAMERA_H

#include <dusk/config.h>
#include <glmm/glmm.h>

typedef enum dusk_camera_dir {
  CAM_DIR_UP,
  CAM_DIR_DOWN,
  CAM_DIR_LEFT,
  CAM_DIR_RIGHT,
  CAM_DIR_FORWARD,
  CAM_DIR_BACK

} dusk_camera_dir_t;

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

void dusk_camera_init(dusk_camera_t * this);
void dusk_camera_print(dusk_camera_t * this);

void dusk_camera_set_aspect(dusk_camera_t * this, float width, float height);
void dusk_camera_set_clip(dusk_camera_t * this, float vnear, float vfar);
void dusk_camera_set_fov(dusk_camera_t * this, float fov);

void dusk_camera_set_pos(dusk_camera_t * this, vec3f_t pos);
void dusk_camera_get_pos(dusk_camera_t * this, vec3f_t out_pos);

void dusk_camera_set_dir(dusk_camera_t * this, vec3f_t dir);
void dusk_camera_get_dir(dusk_camera_t * this, vec3f_t out_dir);

void dusk_camera_set_up(dusk_camera_t * this, vec3f_t up);
void dusk_camera_get_up(dusk_camera_t * this, vec3f_t out_up);

void dusk_camera_set_look_at(dusk_camera_t * this, vec3f_t look_at);
void dusk_camera_get_look_at(dusk_camera_t * this, vec3f_t out_look_at);

void dusk_camera_move(dusk_camera_t * this,
                      dusk_camera_dir_t dir,
                      float             amount);

void dusk_camera_change_pitch(dusk_camera_t * this, float angle);
void dusk_camera_change_yaw(dusk_camera_t * this, float angle);

void dusk_camera_update(dusk_camera_t * this);

#endif // DUSK_CAMERA_H
