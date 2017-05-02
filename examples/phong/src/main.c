#include <dusk/dusk.h>
#include <stdio.h>

typedef struct
{
  vec3f_t light_pos;
  vec3f_t camera_pos;

} phong_data_t;

int           phong_data_index;
phong_data_t  phong_data;
dusk_shader_t phong_shader;

dusk_model_t * monkey;

void update(dusk_frame_info_t * finfo)
{
  vec3f_t rot = dusk_model_get_rot(monkey);
  rot.y += GLMM_RAD(2.0f * finfo->delta);
  if (rot.y > (2.0f * GLMM_PI))
    rot.y = 0.0f;
  dusk_model_set_rot(monkey, rot);
}

int main(int argc, char ** argv)
{
  dusk_settings_t settings = DUSK_DEFAULT_SETTINGS;
  settings.window_width    = 640;
  settings.window_height   = 480;
  settings.window_title    = "Phong";
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

  vec3f_t cam_pos = dusk_camera_get_pos(dusk_camera);
  vec3f_copy(&phong_data.light_pos, &cam_pos);
  vec3f_copy(&phong_data.camera_pos, &cam_pos);

  dusk_shader_file_t phong_files[] = {
      {GL_VERTEX_SHADER, "assets/phong.vs.glsl"}, {GL_FRAGMENT_SHADER, "assets/phong.fs.glsl"}, {0, NULL},
  };

  dusk_shader_init(&phong_shader, phong_files);
  phong_data_index =
      dusk_shader_add_data(&phong_shader, "PhongData", (void *)&phong_data, sizeof(phong_data_t));

  monkey         = dusk_load_model_from_file("assets/monkey.dmfz", &phong_shader);
  dusk_models[0] = monkey;

  dusk_run();

  dusk_shader_term(&phong_shader);

  dusk_term();

  return 0;
}
