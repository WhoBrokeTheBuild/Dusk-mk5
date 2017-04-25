#include <dusk/dusk.h>
#include <stdio.h>

typedef struct
{
  vec3f_t textured_pos;
  vec3f_t camera_pos;

} textured_data_t;

int             textured_data_index;
textured_data_t textured_data;
dusk_shader_t   textured_shader;

dusk_model_t crate;

void update(dusk_frame_info_t * finfo)
{
  vec3f_t rot;

  dusk_model_get_rot(&crate, rot);
  rot[1] += GLMM_RAD(2.0f * finfo->delta);
  if (rot[1] > (2.0f * GLMM_PI))
    rot[1] = 0.0f;
  dusk_model_set_rot(&crate, rot);
}

int main(int argc, char ** argv)
{
  dusk_settings_t settings = DUSK_DEFAULT_SETTINGS;
  settings.window_width    = 640;
  settings.window_height   = 480;
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
  printf("GLEW Version: %d.%d.%d\n", GLEW_VERSION_MAJOR, GLEW_VERSION_MINOR,
         GLEW_VERSION_MICRO);

  vec3f_t cam_pos;
  dusk_camera_get_pos(dusk_camera, cam_pos);
  vec3f_copy(textured_data.textured_pos, cam_pos);
  vec3f_copy(textured_data.camera_pos, cam_pos);

  dusk_shader_file_t textured_files[] = {
      {GL_VERTEX_SHADER, "assets/textured.vs"},
      {GL_FRAGMENT_SHADER, "assets/textured.fs"},
      {0, NULL},
  };

  dusk_shader_init(&textured_shader, textured_files);
  textured_data_index =
      dusk_shader_add_data(&textured_shader, "LightData",
                           (void *)&textured_data, sizeof(textured_data_t));

  dmf_Model_table_t dmf;
  unsigned char *   buffer = NULL;

  dmf = dusk_load_dmf("assets/crate.dmfz", &buffer);

  unsigned int count = 0;

  float * verts = NULL;
  float * norms = NULL;
  float * txcds = NULL;

  dmf_Shape_vec_t   shapes     = dmf_Model_shapes(dmf);
  unsigned int      mesh_count = dmf_Shape_vec_len(shapes);
  dusk_mesh_t *     meshes     = malloc(sizeof(dusk_mesh_t) * mesh_count);
  unsigned int      mat_count  = 0;
  dusk_material_t * materials  = NULL;

  dusk_material_t * dusk_mat = NULL;

  for (unsigned int i = 0; i < dmf_Shape_vec_len(shapes); ++i)
  {
    dmf_Shape_table_t shape     = dmf_Shape_vec_at(shapes, i);
    dmf_Vec3_vec_t    dmf_verts = dmf_Shape_vertices(shape);
    dmf_Vec3_vec_t    dmf_norms = dmf_Shape_normals(shape);
    dmf_Vec2_vec_t    dmf_txcds = dmf_Shape_texcoords(shape);

    if (dmf_Shape_material_is_present(shape))
    {
      dmf_Material_table_t mat          = dmf_Shape_material(shape);
      const char *         ambient_map  = dmf_Material_ambient_map(mat);
      const char *         diffuse_map  = dmf_Material_diffuse_map(mat);
      const char *         specular_map = dmf_Material_specular_map(mat);
      const char *         bump_map     = dmf_Material_bump_map(mat);

      ++mat_count;
      materials = realloc(materials, sizeof(dusk_material_t) * mat_count);
      dusk_mat  = &materials[mat_count - 1];
      dusk_material_init(&materials[mat_count - 1], ambient_map, diffuse_map,
                         specular_map, bump_map);
    }

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

    dusk_mesh_init(&meshes[i], count, verts, norms, txcds);
  }

  free(buffer);

  dusk_model_init(&crate, dusk_mat, mesh_count, meshes, &textured_shader);
  dusk_models[0] = &crate;

  dusk_run();
  dusk_term();

  dusk_model_term(&crate);

  for (unsigned int i = 0; i < mat_count; ++i)
  {
    dusk_material_term(&materials[i]);
  }
  free(materials);

  for (unsigned int i = 0; i < mesh_count; ++i)
  {
    dusk_mesh_term(&meshes[i]);
  }
  free(meshes);

  dusk_shader_term(&textured_shader);

  return 0;
}
