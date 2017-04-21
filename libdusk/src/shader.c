#include "dusk/shader.h"

#include "debug.h"

#define _MAX_SHADER_COUNT 100
#define _MAX_SHADER_SIZE 10000
#define _MAX_SHADER_LOG_SIZE 10000

GLuint _dusk_shader_load(const char * filename, GLenum shader_type);

bool _dusk_shader_program_print_log(GLuint program);
bool _dusk_shader_print_log(GLuint shader);

bool dusk_shader_init(dusk_shader_t * this,
                      void *                     data,
                      size_t                     data_size,
                      const char *               name,
                      const dusk_shader_info_t * shaders)
{
  assert(NULL != this);

  GLint        program_linked = GL_FALSE;
  GLuint       shader_ids[_MAX_SHADER_COUNT];
  unsigned int shader_count = 0;

  this->program = 0;
  this->ubo     = 0;
  this->size    = data_size;

  if (NULL == shaders)
    return false;

  this->program = glCreateProgram();
  if (0 == this->program)
  {
    DEBUG_ERROR("Failed to create shader program object");
    goto error;
  }

  for (int i = 0; shaders[i].filename; ++i)
  {
    if (i > _MAX_SHADER_COUNT)
    {
      DEBUG_ERROR("Exceeded maximum number of shaders allowed %d",
                  _MAX_SHADER_COUNT);
      goto error;
    }

    shader_ids[i] = _dusk_shader_load(shaders[i].filename, shaders[i].type);
    if (0 == shader_ids[i])
    {
      DEBUG_ERROR("Failed to load shader program");
      goto error;
    }

    glAttachShader(this->program, shader_ids[i]);
    ++shader_count;
  }

  glLinkProgram(this->program);

  glGetProgramiv(this->program, GL_LINK_STATUS, &program_linked);
  if (!program_linked)
  {
    DEBUG_ERROR("Failed to link shader program");
    goto error;
  }

  for (unsigned int i = 0; i < shader_count; ++i)
  {
    glDetachShader(this->program, shader_ids[i]);
    glDeleteShader(shader_ids[i]);
  }

  if (NULL != data)
  {
    glGenBuffers(1, &this->ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, this->ubo);
    glBufferData(GL_UNIFORM_BUFFER, this->size, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    GLuint data_index = glGetUniformBlockIndex(this->program, name);
    glUniformBlockBinding(this->program, data_index, 2);
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, this->ubo);
  }

  return true;

error:

  if (this->program)
  {
    _dusk_shader_program_print_log(this->program);
  }

  for (int i = 0; i < _MAX_SHADER_COUNT; ++i)
  {
    glDeleteShader(shader_ids[i]);
  }
  glDeleteProgram(this->program);
  this->program = 0;

  return false;
}

void dusk_shader_term(dusk_shader_t * this)
{
  glDeleteProgram(this->program);
}

void dusk_shader_set_data(dusk_shader_t * this, void * data)
{
  glBindBuffer(GL_UNIFORM_BUFFER, this->ubo);
  GLvoid * ptr = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
  memcpy(ptr, data, this->size);
  glUnmapBuffer(GL_UNIFORM_BUFFER);
}

void dusk_shader_bind(dusk_shader_t * this)
{
  glUseProgram(this->program);
}

bool _dusk_shader_program_print_log(GLuint program)
{
  enum
  {
    _MAX_LOG_SIZE = 100000
  };

  char * program_log = NULL;
  GLint  log_size    = 0;
  GLint  ret_size    = 0;

  if (!glIsProgram(program))
  {
    DEBUG_ERROR("Cannot print program log, %d is not a program", program);
    goto error;
  }

  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_size);
  if (log_size > _MAX_LOG_SIZE)
  {
    DEBUG_ERROR("Program log for %d exceeds maximum size allowed (%d > %d)",
                program, log_size, _MAX_LOG_SIZE);
    goto error;
  }

  program_log = malloc((log_size + 1) * sizeof(char));
  if (!program_log)
  {
    DEBUG_ERROR("Out of memory");
    goto error;
  }

  glGetProgramInfoLog(program, log_size, &ret_size, program_log);
  if (ret_size <= 0)
  {
    DEBUG_ERROR("Failed to get program log for %d", program);
    goto error;
  }
  program_log[log_size] = '\0';

  DEBUG_INFO("Log for program %d:\n%s", program, program_log);

  free(program_log);
  return true;

error:

  free(program_log);
  return false;
}

bool _dusk_shader_print_log(GLuint shader)
{
  enum
  {
    _MAX_LOG_SIZE = 100000
  };

  char * shader_log = NULL;
  GLint  log_size   = 0;
  GLint  ret_size   = 0;

  if (!glIsShader(shader))
  {
    DEBUG_ERROR("Cannot print shader log, %d is not a shader", shader);
    goto error;
  }

  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_size);
  if (log_size > _MAX_LOG_SIZE)
  {
    DEBUG_ERROR("Shader log for %d exceeds maximum size allowed (%d > %d)",
                shader, log_size, _MAX_LOG_SIZE);
    goto error;
  }

  shader_log = malloc((log_size + 1) * sizeof(char));
  if (!shader_log)
  {
    DEBUG_ERROR("Out of memory");
    goto error;
  }

  glGetShaderInfoLog(shader, log_size, &ret_size, shader_log);
  if (ret_size <= 0)
  {
    DEBUG_ERROR("Failed to get shader log for %d", shader);
    goto error;
  }
  shader_log[log_size] = '\0';

  DEBUG_INFO("Log for shader %d:\n%s", shader, shader_log);

  free(shader_log);
  return true;

error:

  free(shader_log);
  return false;
}

GLuint _dusk_shader_load(const char * filename, GLenum shader_type)
{
  GLuint shader     = 0;
  char * shader_str = NULL;
  FILE * fp         = NULL;
  size_t bytes_read = 0;
  size_t file_size  = 0;

  DEBUG_INFO("Loading Shader from '%s'", filename);

  fp = fopen(filename, "rb");
  if (NULL == fp)
  {
    DEBUG_ERROR("Failed to open shader '%s'", filename);
    goto error;
  }

  fseek(fp, 0, SEEK_END);
  file_size = ftell(fp);
  rewind(fp);

  if (file_size > _MAX_SHADER_SIZE)
  {
    DEBUG_ERROR("Shader exceeds maximum size allowed (%ld > %d)", file_size,
                _MAX_SHADER_SIZE);
    goto error;
  }

  shader_str = malloc((file_size + 1) * sizeof(char));
  if (!shader_str)
  {
    DEBUG_ERROR("Out of memory");
    goto error;
  }

  bytes_read = fread(shader_str, 1, file_size, fp);
  if (bytes_read != file_size)
  {
    DEBUG_ERROR("Failed to read whole shader file '%s' read %zu/%ld", filename,
                bytes_read, file_size);
    goto error;
  }
  shader_str[file_size] = '\0';

  shader = glCreateShader(shader_type);
  if (0 == shader)
  {
    DEBUG_ERROR("Failed to create shader object");
    goto error;
  }
  if (GL_INVALID_ENUM == shader)
  {
    DEBUG_ERROR("Invalid shader type %d", shader_type);
    goto error;
  }

  glShaderSource(shader, 1, (const GLchar **)&shader_str, NULL);
  glCompileShader(shader);

  GLint shader_compiled = GL_FALSE;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_compiled);
  if (!shader_compiled)
  {
    DEBUG_ERROR("Failed to compile shader '%s'", filename);
    goto error;
  }

  free(shader_str);
  fclose(fp);

  return shader;

error:

  if (shader)
  {
    _dusk_shader_print_log(shader);
  }

  free(shader_str);
  fclose(fp);

  glDeleteShader(shader);

  return -1;
}
