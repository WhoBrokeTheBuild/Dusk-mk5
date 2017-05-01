#include <dusk/dusk.h>
#include <stdio.h>
#include <time.h>

typedef struct
{
  vec3f_t light_pos;
  vec3f_t camera_pos;

} textured_data_t;

int             textured_data_index;
textured_data_t textured_data;
dusk_shader_t   textured_shader;

dusk_model_t * crate;

void update(dusk_frame_info_t * finfo)
{
  vec3f_t rot = dusk_model_get_rot(crate);
  rot.y += GLMM_RAD(0.3f * finfo->delta);
  if (rot.y > (2.0f * GLMM_PI))
    rot.y = 0.0f;
  dusk_model_set_rot(crate, rot);

#ifndef _MSC_VER
  struct timespec ts;
  ts.tv_sec  = 0;
  ts.tv_nsec = 10000;
  nanosleep(&ts, NULL);
#endif 
}

int main(int argc, char ** argv)
{
  dusk_settings_t settings = DUSK_DEFAULT_SETTINGS;
  settings.window_width    = 1024;
  settings.window_height   = 768;
  settings.window_title    = "Textured";
  settings.update_func     = &update;

  dusk_init(argc, argv, &settings);

  int glut_ver = glutGet(GLUT_VERSION);
  int glut_pat = glut_ver % 100;
  int glut_min = ((glut_ver - glut_pat) % 10000) / 100;
  int glut_maj = glut_ver / 10000;

  printf("Dusk Version: %s\n", dusk_version());
  printf("GLMM Version: %s\n", GLMM_VER_STRING);
  printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
  printf("(Free)GLUT Version: %d.%d.%d\n", glut_maj, glut_min, glut_pat);
  printf("GLEW Version: %d.%d.%d\n", GLEW_VERSION_MAJOR, GLEW_VERSION_MINOR, GLEW_VERSION_MICRO);

  dusk_camera_set_up(dusk_camera, (vec3f_t){0.0f, 1.0f, 0.0f});
  dusk_camera_set_pos(dusk_camera, (vec3f_t){2.0f, 2.0f, 2.0f});
  dusk_camera_set_look_at(dusk_camera, (vec3f_t){0.0f, 0.0f, 0.0f});

  dusk_camera_update(dusk_camera);

  vec3f_t cam_pos = dusk_camera_get_pos(dusk_camera);
  vec3f_copy(&textured_data.light_pos, &(vec3f_t){5.0f, 5.0f, 5.0f});
  vec3f_copy(&textured_data.camera_pos, &cam_pos);

  dusk_shader_file_t textured_files[] = {
      {GL_VERTEX_SHADER, "assets/textured.vs"},
      {GL_FRAGMENT_SHADER, "assets/textured.fs"},
      {0, NULL},
  };

  dusk_shader_init(&textured_shader, textured_files);
  textured_data_index = dusk_shader_add_data(&textured_shader, "LightData", (void *)&textured_data,
                                             sizeof(textured_data_t));

  crate          = dusk_load_model_from_file("assets/globe.dmfz", &textured_shader);
  dusk_models[0] = crate;

  dusk_run();

  dusk_shader_term(&textured_shader);

  dusk_term();

  return 0;
}
