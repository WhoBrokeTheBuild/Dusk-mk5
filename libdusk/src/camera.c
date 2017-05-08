#include "dusk/camera.h"

#include "debug.h"

dusk_camera_t * _current_camera                     = NULL;
dusk_camera_t * _all_cameras[DUSK_MAX_CAMERA_COUNT] = {NULL};
unsigned int    _camera_count                       = 0;

bool dusk_add_camera(dusk_camera_t * camera)
{
    if (_camera_count >= DUSK_MAX_CAMERA_COUNT - 1)
    {
        DEBUG_INFO("Reached maximum number of cameras, %d", DUSK_MAX_CAMERA_COUNT);
        return false;
    }

    for (unsigned int i = 0; i < DUSK_MAX_CAMERA_COUNT; ++i)
    {
        if (NULL == _all_cameras[i])
        {
            ++_camera_count;
            _all_cameras[i] = camera;
            break;
        }
    }

    if (NULL == _current_camera)
    {
        dusk_set_current_camera(camera);
    }

    return true;
}

bool dusk_remove_camera(dusk_camera_t * camera)
{
    bool found = false;

    for (unsigned int i = 0; i < DUSK_MAX_CAMERA_COUNT; ++i)
    {
        if (camera == _all_cameras[i])
        {
            --_camera_count;
            _all_cameras[i] = NULL;
            found           = true;
            break;
        }
    }

    return found;
}

void dusk_set_current_camera(dusk_camera_t * camera)
{
    _current_camera = camera;
}

dusk_camera_t * dusk_get_current_camera()
{
    return _current_camera;
}

dusk_camera_t ** dusk_get_all_cameras(unsigned int * count)
{
    if (NULL != count)
    {
        *count = _camera_count;
    }
    return _all_cameras;
}

void dusk_camera_init(dusk_camera_t * this)
{
    mat4x4_init(&this->proj, 1.0f);
    mat4x4_init(&this->view, 1.0f);

    vec3f_init(&this->_pos, 0.0f);
    vec3f_init(&this->_dir, 0.0f);
    vec3f_init(&this->_up, 0.0f);
    this->_up.z = 1.0f;

    vec3f_init(&this->_pos_delta, 0.0f);

    this->_pitch = 0.0f;
    this->_yaw   = 0.0f;

    this->_aspect = 0.0f;
    this->_vnear  = 1000.0f;
    this->_vfar   = 0.1f;
    this->_fov    = 45.0f;

    this->_invalid_proj = true;
    this->_invalid_view = true;
}

void dusk_camera_term(dusk_camera_t * this)
{
}

void dusk_camera_print(dusk_camera_t * this)
{
    printf("Proj:\n");
    mat4x4_print(&this->proj);

    printf("View:\n");
    mat4x4_print(&this->view);

    printf("Pos:\n");
    vec3f_print(&this->_pos);

    printf("Dir:\n");
    vec3f_print(&this->_dir);

    printf("Up:\n");
    vec3f_print(&this->_up);

    printf("Look At:\n");
    vec3f_print(&this->_look_at);
}

void dusk_camera_set_aspect(dusk_camera_t * this, float width, float height)
{
    this->_aspect       = width / height;
    this->_invalid_proj = true;
}

void dusk_camera_set_clip(dusk_camera_t * this, float vnear, float vfar)
{
    this->_vnear        = vnear;
    this->_vfar         = vfar;
    this->_invalid_proj = true;
}

void dusk_camera_set_fov(dusk_camera_t * this, float fov)
{
    this->_fov          = fov;
    this->_invalid_proj = true;
}

void dusk_camera_set_pos(dusk_camera_t * this, const vec3f_t pos)
{
    glmm_vec3f_copy(&this->_pos, &pos);
    this->_invalid_view = true;
}

vec3f_t dusk_camera_get_pos(dusk_camera_t * this)
{
    return this->_pos;
}

void dusk_camera_set_up(dusk_camera_t * this, const vec3f_t up)
{
    glmm_vec3f_copy(&this->_up, &up);
    this->_invalid_view = true;
}

vec3f_t dusk_camera_get_up(dusk_camera_t * this)
{
    vec3f_t tmp = this->_up;
    return tmp;
}

void dusk_camera_look_at(dusk_camera_t * this, const vec3f_t look_at)
{
    glmm_vec3f_copy(&this->_look_at, &look_at);
    this->_invalid_view = true;
}

void dusk_camera_move(dusk_camera_t * this, vec3f_t move)
{
    dusk_camera_update(this);

    vec4f_t tmp = glmm_mat4x4_mulv(&this->view, &(vec4f_t){{move.x, move.y, move.z, 1.0f}});
    vec3f_t new_move = {{tmp.x, tmp.y, tmp.z}};
    vec3f_add(&this->_pos, &new_move);
    vec3f_add(&this->_look_at, &new_move);

    this->_invalid_view = true;
}

void dusk_camera_change_pitch(dusk_camera_t * this, float angle)
{
    this->_pitch += angle;

    if (this->_pitch > GLMM_TWO_PI)
    {
        this->_pitch -= GLMM_TWO_PI;
    }

    this->_invalid_view = true;
}

void dusk_camera_change_yaw(dusk_camera_t * this, float angle)
{
    this->_yaw += angle;

    if ((this->_pitch > GLMM_PI * 0.5f && this->_pitch < GLMM_PI * 1.5f) ||
        (this->_pitch < -GLMM_PI * 0.5f && this->_pitch > -GLMM_PI * 1.5f))
    {
        this->_yaw -= angle;
    }
    else
    {
        this->_yaw += angle;
    }

    if (this->_yaw > GLMM_TWO_PI)
    {
        this->_yaw -= GLMM_TWO_PI;
    }

    this->_invalid_view = true;
}

void dusk_camera_update(dusk_camera_t * this)
{
    if (this->_invalid_view)
    {
        vec3f_t pitch_axis;
        quat_t  pitch_quat, yaw_quat, tmp;

        vec3f_xsub(&this->_dir, &this->_look_at, &this->_pos);
        vec3f_norm(&this->_dir);

        vec3f_xcross(&pitch_axis, &this->_dir, &this->_up);
        angle_axis(&pitch_quat, this->_pitch, &pitch_axis);
        angle_axis(&yaw_quat, this->_yaw, &this->_up);

        quat_xcross(&tmp, &pitch_quat, &yaw_quat);
        quat_norm(&tmp);

        rotate_vec3f_quat(&this->_dir, &this->_dir, &tmp);

        vec3f_add(&this->_pos, &this->_pos_delta);
        vec3f_xadd(&this->_look_at, &this->_pos, &this->_dir);

        glmm_look_at(&this->view, &this->_pos, &this->_look_at, &this->_up);

        this->_yaw *= 0.5f;
        this->_pitch *= 0.5f;
        vec3f_init(&this->_pos_delta, 0.0f);

        this->_invalid_view = false;
    }

    if (this->_invalid_proj)
    {
        glmm_perspective(&this->proj, this->_aspect, this->_vnear, this->_vfar, this->_fov);
        this->_invalid_proj = false;
    }
}
