#include <dusk/dusk.h>
#include <stdio.h>

int main(int argc, char ** argv)
{
  dusk_settings_t settings = DUSK_DEFAULT_SETTINGS;
  settings.window_width    = 640;
  settings.window_height   = 480;
  settings.window_title    = "Triangle";

  dusk_init(argc, argv, &settings);

  SDL_version sdlver;
  SDL_GetVersion(&sdlver);

  printf("Dusk Version: %s\n", DUSK_VERSION);
  printf("GLMM Version: %s\n", GLMM_VER_STRING);
  printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
  printf("SDL Version: %d.%d.%d\n", sdlver.major, sdlver.minor, sdlver.patch);
  printf("GLEW Version: %d.%d.%d\n", GLEW_VERSION_MAJOR, GLEW_VERSION_MINOR, GLEW_VERSION_MICRO);

  typedef struct
  {
    float color[4];
  } triangle_data_t;

  dusk_shader_t   triangle_shader;
  triangle_data_t data = {
      .color = {0.0f, 1.0f, 0.0f, 1.0f},
  };

  dusk_shader_file_t triangle_files[] = {
      {GL_VERTEX_SHADER, "assets/triangle.vs.glsl"},
      {GL_FRAGMENT_SHADER, "assets/triangle.fs.glsl"},
      {0, NULL},
  };

  dusk_shader_init(&triangle_shader, triangle_files);
  dusk_shader_add_data(&triangle_shader, "TriangleData", (void *)&data, sizeof(triangle_data_t));

  // clang-format off

  static const GLfloat verts[] = {
      -1.0f, -1.0f, 0.0f,
       1.0f, -1.0f, 0.0f,
       0.0f,  1.0f, 0.0f,
  };

  // clang-format on

  dusk_mesh_t triangle_mesh;
  dusk_mesh_init(&triangle_mesh, NULL, &triangle_shader, 3, verts, NULL, NULL);

  dusk_model_t  triangle;
  dusk_mesh_t * meshes = {&triangle_mesh};
  dusk_model_init(&triangle, 1, &meshes, &triangle_shader);
  dusk_models[0] = &triangle;

  dusk_run();
  dusk_term();

  dusk_model_term(&triangle);
  dusk_mesh_term(&triangle_mesh);
  dusk_shader_term(&triangle_shader);

  return 0;
}
