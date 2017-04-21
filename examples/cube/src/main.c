#include <dusk/dusk.h>
#include <stdio.h>

int main(int argc, char **argv)
{
  dusk_init(argc, argv, "Cube", 640, 480);

  int glut_ver = glutGet(GLUT_VERSION);
  int glut_pat = glut_ver % 100;
  int glut_min = ((glut_ver - glut_pat) % 10000) / 100;
  int glut_maj = glut_ver / 10000;

  printf("Dusk Version: %s\n", dusk_version());
  printf("GLMM Version: %s\n", GLMM_VER_STRING);
  printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
  printf("(Free)GLUT Version: %d.%d.%d\n", glut_maj, glut_min, glut_pat);
  printf("GLEW Version: %d.%d.%d\n", GLEW_VERSION_MAJOR, GLEW_VERSION_MINOR, GLEW_VERSION_MICRO);

  typedef struct {
    float color[4];
    float rotation;
  } flat_data_t;

  dusk_shader_t flat_shader;
  flat_data_t data = {
    .color = { 0.0f, 0.0f, 1.0f, 1.0f },
    .rotation = 0.0f,
  };

  dusk_shader_info_t flat_files[] = {
    { GL_VERTEX_SHADER,   "assets/flat.vs" },
    { GL_FRAGMENT_SHADER, "assets/flat.fs" },
    { 0, NULL },
  };

  dusk_shader_init(&flat_shader, (void *)&data, sizeof(flat_data_t), "flat_data", flat_files);
  dusk_shader_set_data(&flat_shader, (void *)&data);

  dmf_Model_table_t dmf;
  unsigned char * buffer = NULL;

  dmf = dusk_load_dmf("assets/cube.dmfz", &buffer);

  unsigned int count = 0;
  float * verts = NULL;
  float * norms = NULL;
  float * txcds = NULL;

  dmf_Shape_vec_t shapes = dmf_Model_shapes(dmf);

  for (unsigned int i = 0; i < dmf_Shape_vec_len(shapes); ++i)
  {
    dmf_Shape_table_t shape = dmf_Shape_vec_at(shapes, i);
    dmf_Vec3_vec_t dmf_verts = dmf_Shape_vertices(shape);
    dmf_Vec3_vec_t dmf_norms = dmf_Shape_normals(shape);
    dmf_Vec2_vec_t dmf_txcds = dmf_Shape_texcoords(shape);

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

  dusk_model_t cube;
  dusk_model_init(&cube, 1, &cube_mesh, &flat_shader);
  dusk_models[0] = &cube;

  dusk_run();
  dusk_term();

  dusk_model_term(&cube);
  dusk_mesh_term(&cube_mesh);
  dusk_shader_term(&flat_shader);

  return 0;
}
