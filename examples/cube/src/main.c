#include <dusk/dusk.h>
#include <stdio.h>

#define MAX_COLOR_CHANGE_DELAY 300
vec4f_t COLORS[] = {
    {190.0f / 255.0f, 25.0f / 255.0f, 49.0f / 255.0f, 1.0f},
    {253.0f / 255.0f, 203.0f / 255.0f, 88.0f / 255.0f, 1.0f},
    {120.0f / 255.0f, 177.0f / 255.0f, 89.0f / 255.0f, 1.0f},
    {93.0f / 255.0f, 173.0f / 255.0f, 236.0f / 255.0f, 1.0f},
    {170.0f / 255.0f, 143.0f / 255.0f, 214.0f / 255.0f, 1.0f},
};
#define NUM_COLORS (int)(sizeof(COLORS) / sizeof(vec4f_t))

int    color_index        = 0;
double color_change_delay = MAX_COLOR_CHANGE_DELAY;

typedef struct
{
  vec4f_t color;
} flat_data_t;

int           flat_data_index;
flat_data_t   flat_data;
dusk_shader_t flat_shader;

dusk_model_t cube;

void update(dusk_frame_info_t * finfo)
{
  vec3f_t rot;

  dusk_model_get_rot(&cube, rot);
  rot[1] += GLMM_RAD(5.0f * finfo->delta);
  if (rot[1] > GLMM_PI)
    rot[1] = 0.0f;
  dusk_model_set_rot(&cube, rot);

  color_change_delay -= finfo->elapsed_time;
  if (color_change_delay <= 0)
  {
    color_change_delay = MAX_COLOR_CHANGE_DELAY;
    ++color_index;
    if (color_index >= NUM_COLORS)
    {
      color_index = 0;
    }

    vec4f_copy(flat_data.color, COLORS[color_index]);
    dusk_shader_set_data(&flat_shader, flat_data_index, (void *)&flat_data);
  }
}

int main(int argc, char ** argv)
{
  dusk_settings_t settings = DUSK_DEFAULT_SETTINGS;
  settings.window_width    = 640;
  settings.window_height   = 480;
  settings.window_title    = "Cube";
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
  printf("GLEW Version: %d.%d.%d\n", GLEW_VERSION_MAJOR, GLEW_VERSION_MINOR,
         GLEW_VERSION_MICRO);

  color_index        = 0;
  color_change_delay = MAX_COLOR_CHANGE_DELAY;
  vec4f_copy(flat_data.color, COLORS[color_index]);

  dusk_shader_file_t flat_files[] = {
      {GL_VERTEX_SHADER, "assets/flat.vs"},
      {GL_FRAGMENT_SHADER, "assets/flat.fs"},
      {0, NULL},
  };

  dusk_shader_init(&flat_shader, flat_files);
  flat_data_index = dusk_shader_add_data(
      &flat_shader, "FlatData", (void *)&flat_data, sizeof(flat_data_t));

  dmf_Model_table_t dmf;
  unsigned char *   buffer = NULL;

  dmf = dusk_load_dmf("assets/cube.dmfz", &buffer);

  unsigned int count = 0;

  float * verts = NULL;
  float * norms = NULL;
  float * txcds = NULL;

  dmf_Shape_vec_t shapes = dmf_Model_shapes(dmf);

  for (unsigned int i = 0; i < dmf_Shape_vec_len(shapes); ++i)
  {
    dmf_Shape_table_t shape     = dmf_Shape_vec_at(shapes, i);
    dmf_Vec3_vec_t    dmf_verts = dmf_Shape_vertices(shape);
    dmf_Vec3_vec_t    dmf_norms = dmf_Shape_normals(shape);
    dmf_Vec2_vec_t    dmf_txcds = dmf_Shape_texcoords(shape);

    count = dmf_Vec3_vec_len(dmf_verts);
    verts = (float *)dmf_verts;

    if (dmf_Vec3_vec_len(dmf_norms) > 0)
    {
      norms = (float *)dmf_norms;
    }
    if (dmf_Vec2_vec_len(dmf_txcds) > 0)
    {
      txcds = (float *)dmf_txcds;
    }
  }

  dusk_mesh_t cube_mesh;
  dusk_mesh_init(&cube_mesh, count, verts, norms, txcds);

  free(buffer);

  dusk_model_init(&cube, 1, &cube_mesh, &flat_shader);
  dusk_models[0] = &cube;

  dusk_run();
  dusk_term();

  dusk_model_term(&cube);
  dusk_mesh_term(&cube_mesh);
  dusk_shader_term(&flat_shader);

  return 0;
}
