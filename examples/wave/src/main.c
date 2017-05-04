#include <dusk/dusk.h>
#include <stdio.h>

typedef struct
{
  float time;
} wave_data_t;

int           wave_data_index;
wave_data_t   wave_data;
dusk_shader_t wave_shader;

dusk_model_t * plane;

void update(dusk_frame_info_t * finfo)
{
  wave_data.time += finfo->elapsed_time;
}

int main(int argc, char ** argv)
{
  dusk_settings_t settings = DUSK_DEFAULT_SETTINGS;
  settings.window_width    = 1024;
  settings.window_height   = 768;
  settings.window_title    = "Wave";
  settings.update_func     = &update;

  dusk_init(argc, argv, &settings);

  int glut_ver = glutGet(GLUT_VERSION);
  int glut_pat = glut_ver % 100;
  int glut_min = ((glut_ver - glut_pat) % 10000) / 100;
  int glut_maj = glut_ver / 10000;

  printf("Dusk Version: %s\n", DUSK_VERSION);
  printf("GLMM Version: %s\n", GLMM_VER_STRING);
  printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
  printf("(Free)GLUT Version: %d.%d.%d\n", glut_maj, glut_min, glut_pat);
  printf("GLEW Version: %d.%d.%d\n", GLEW_VERSION_MAJOR, GLEW_VERSION_MINOR, GLEW_VERSION_MICRO);

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  dusk_camera_set_up(dusk_camera, (vec3f_t){{0.0f, 1.0f, 0.0f}});
  dusk_camera_set_pos(dusk_camera, (vec3f_t){{15.0f, 15.0f, 15.0f}});
  dusk_camera_set_look_at(dusk_camera, (vec3f_t){{5.0f, 0.0f, 5.0f}});

  dusk_shader_file_t wave_files[] = {
      {GL_VERTEX_SHADER, "assets/wave.vs.glsl"},
      {GL_FRAGMENT_SHADER, "assets/wave.fs.glsl"},
      {0, NULL},
  };

  dusk_shader_init(&wave_shader, wave_files);
  wave_data_index =
      dusk_shader_add_data(&wave_shader, "WaveData", (void *)&wave_data, sizeof(wave_data_t));

  dusk_material_t * plane_material = (dusk_material_t *)malloc(sizeof(dusk_material_t));
  dusk_track_resource(DUSK_RSC_MATERIAL, (void *)plane_material);
  dusk_material_init(plane_material, (vec4f_t){{0.0f, 0.0f, 0.0f, 1.0f}},
                     (vec4f_t){{0.0f, 1.0f, 0.0f, 1.0f}}, (vec4f_t){{0.0f, 0.0f, 0.0f, 1.0f}}, 0.0f,
                     0.0f, NULL, NULL, NULL, NULL);

  dusk_mesh_t * plane_mesh = (dusk_mesh_t *)malloc(sizeof(dusk_mesh_t));
  dusk_track_resource(DUSK_RSC_MESH, (void *)plane_mesh);
  dusk_mesh_create_plane(plane_mesh, plane_material, &wave_shader, 100, 100, 10.0f, 10.0f);

  plane = (dusk_model_t *)malloc(sizeof(dusk_model_t));
  dusk_model_init(plane, 1, &plane_mesh, &wave_shader);
  dusk_track_resource(DUSK_RSC_MODEL, (void *)plane);
  dusk_models[0] = plane;

  dusk_run();

  dusk_term();
  dusk_shader_term(&wave_shader);

  return 0;
}
