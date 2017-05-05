#include <dusk/dusk.h>
#include <stdio.h>

typedef struct
{
  dusk_Alignas(16) vec3f_t light_pos;
  dusk_Alignas(16) vec3f_t camera_pos;
  dusk_Alignas(4) float time;
  dusk_Alignas(4) float height;
  dusk_Alignas(4) float width;
} wave_data_t;

int           wave_data_index;
wave_data_t   wave_data;
dusk_shader_t wave_shader;

//int           floor_data_index;
//wave_data_t   floor_data;
dusk_shader_t floor_shader;

dusk_model_t * plane_model;
dusk_model_t * floor_model;

void update(dusk_frame_info_t * finfo)
{
  wave_data.time = fmod(wave_data.time + finfo->elapsed_time / 300.0f, M_PI * 2.0f);

  dusk_shader_set_data(&wave_shader, wave_data_index, (void *)&wave_data);
}

int main(int argc, char ** argv)
{
  dusk_settings_t settings = DUSK_DEFAULT_SETTINGS;
  settings.window_width    = 1024;
  settings.window_height   = 768;
  settings.window_title    = "Wave";
  settings.update_func     = &update;

  dusk_init(argc, argv, &settings);

  SDL_version sdlver;
  SDL_GetVersion(&sdlver);

  printf("Dusk Version: %s\n", DUSK_VERSION);
  printf("GLMM Version: %s\n", GLMM_VER_STRING);
  printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
  printf("SDL Version: %d.%d.%d\n", sdlver.major, sdlver.minor, sdlver.patch);
  printf("GLEW Version: %d.%d.%d\n", GLEW_VERSION_MAJOR, GLEW_VERSION_MINOR, GLEW_VERSION_MICRO);

  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  dusk_camera_set_up(dusk_camera, (vec3f_t){{0.0f, 1.0f, 0.0f}});
  dusk_camera_set_pos(dusk_camera, (vec3f_t){{100.0f, 30.0f, 100.0f}});
  dusk_camera_set_look_at(dusk_camera, (vec3f_t){{50.0f, -10.0f, 50.0f}});

  dusk_camera_update(dusk_camera);

  dusk_shader_file_t wave_files[] = {
      {GL_VERTEX_SHADER, "assets/wave.vs.glsl"},
      {GL_FRAGMENT_SHADER, "assets/wave.fs.glsl"},
      {0, NULL},
  };

  dusk_shader_file_t floor_files[] = {
      {GL_VERTEX_SHADER, "assets/floor.vs.glsl"},
      {GL_FRAGMENT_SHADER, "assets/floor.fs.glsl"},
      {0, NULL},
  };

  vec3f_t cam_pos = dusk_camera_get_pos(dusk_camera);
  vec3f_copy(&wave_data.light_pos, &cam_pos);
  vec3f_copy(&wave_data.camera_pos, &cam_pos);
  wave_data.time = 0;
  wave_data.height = 1.3f;
  wave_data.width = 0.4f;

  dusk_shader_init(&wave_shader, wave_files);
  wave_data_index =
      dusk_shader_add_data(&wave_shader, "WaveData", (void *)&wave_data, sizeof(wave_data_t));

  dusk_shader_init(&floor_shader, floor_files);
  //floor_data_index =
  //    dusk_shader_add_data(&floor_shader, "WaveData", (void *)&floor_data, sizeof(wave_data_t));

  dusk_material_t * floor_mat = (dusk_material_t *)malloc(sizeof(dusk_material_t));
  dusk_track_resource(DUSK_RSC_MATERIAL, (void *)floor_mat);
  dusk_material_init(floor_mat,
    (vec4f_t){{0.0f, 0.0f, 0.0f, 1.0f}},
    (vec4f_t){{0.0f, 0.0f, 0.0f, 1.0f}},
    (vec4f_t){{0.0f, 0.0f, 0.0f, 1.0f}},
    2, 0.0f,
    NULL, "assets/floor_diff.jpg", NULL, NULL);

  dusk_mesh_t * floor_mesh = (dusk_mesh_t *)malloc(sizeof(dusk_mesh_t));
  dusk_track_resource(DUSK_RSC_MESH, (void *)floor_mesh);
  dusk_mesh_create_plane(floor_mesh, floor_mat, &floor_shader, 10, 10, 100.0f, 100.0f);

  dusk_mesh_t * plane_mesh = (dusk_mesh_t *)malloc(sizeof(dusk_mesh_t));
  dusk_track_resource(DUSK_RSC_MESH, (void *)plane_mesh);
  dusk_mesh_create_plane(plane_mesh, NULL, &wave_shader, 100, 100, 100.0f, 100.0f);

  floor_model = (dusk_model_t *)malloc(sizeof(dusk_model_t));
  plane_model = (dusk_model_t *)malloc(sizeof(dusk_model_t));
  dusk_track_resource(DUSK_RSC_MODEL, (void *)floor_model);
  dusk_track_resource(DUSK_RSC_MODEL, (void *)plane_model);

  dusk_model_init(floor_model, 1, &floor_mesh, &floor_shader);
  dusk_model_init(plane_model, 1, &plane_mesh, &wave_shader);

  dusk_model_set_pos(floor_model, (vec3f_t){{0.0f, -10.0f, 0.0f}});

  dusk_models[0] = floor_model;
  dusk_models[1] = plane_model;

  dusk_run();

  dusk_term();
  dusk_shader_term(&wave_shader);
  dusk_shader_term(&floor_shader);

  return 0;
}
