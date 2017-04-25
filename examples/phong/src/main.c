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

dusk_model_t monkey;

void update(dusk_frame_info_t * finfo)
{
  vec3f_t rot;

  dusk_model_get_rot(&monkey, rot);
  rot[1] += GLMM_RAD(2.0f * finfo->delta);
  if (rot[1] > (2.0f * GLMM_PI))
    rot[1] = 0.0f;
  dusk_model_set_rot(&monkey, rot);
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
  printf("GLEW Version: %d.%d.%d\n", GLEW_VERSION_MAJOR, GLEW_VERSION_MINOR,
         GLEW_VERSION_MICRO);

  vec3f_t cam_pos;
  dusk_camera_get_pos(dusk_camera, cam_pos);
  vec3f_copy(phong_data.light_pos, cam_pos);
  vec3f_copy(phong_data.camera_pos, cam_pos);

  dusk_shader_file_t phong_files[] = {
      {GL_VERTEX_SHADER, "assets/phong.vs"},
      {GL_FRAGMENT_SHADER, "assets/phong.fs"},
      {0, NULL},
  };

  dusk_shader_init(&phong_shader, phong_files);
  phong_data_index = dusk_shader_add_data(
      &phong_shader, "PhongData", (void *)&phong_data, sizeof(phong_data_t));

  dmf_Model_table_t dmf;
  unsigned char *   buffer = NULL;

  dmf = dusk_load_dmf("assets/monkey.dmfz", &buffer);

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

  dusk_mesh_t monkey_mesh;
  dusk_mesh_init(&monkey_mesh, count, verts, norms, txcds);

  free(buffer);

  dusk_model_init(&monkey, NULL, 1, &monkey_mesh, &phong_shader);
  dusk_models[0] = &monkey;

  dusk_run();
  dusk_term();

  dusk_model_term(&monkey);
  dusk_mesh_term(&monkey_mesh);
  dusk_shader_term(&phong_shader);

  return 0;
}
