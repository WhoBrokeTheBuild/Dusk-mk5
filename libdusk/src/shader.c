#include "dusk/shader.h"

#include "debug.h"
#include <stdlib.h>
#include <dusk/camera.h>

#define _MAX_SHADER_COUNT 100
#define _MAX_SHADER_SIZE 10000
#define _MAX_SHADER_LOG_SIZE 10000

dusk_shader_data_t * _data_root   = NULL;
int                  _data_max_id = 0;

GLuint _dusk_shader_load(const char * filename, GLenum shader_type);

bool _dusk_shader_program_print_log(GLuint program);
bool _dusk_shader_print_log(GLuint shader);

bool dusk_shader_init(dusk_shader_t * this, const dusk_shader_file_t * shaders)
{
  assert(NULL != this);

  GLint        program_linked = GL_FALSE;
  GLuint       shader_ids[_MAX_SHADER_COUNT];
  unsigned int shader_count = 0;

  this->program = 0;

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
      DEBUG_ERROR("Exceeded maximum number of shaders allowed %d", _MAX_SHADER_COUNT);
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
  dusk_shader_data_t * ptr;
  for (ptr = _data_root; NULL != ptr; ptr = ptr->_next)
  {
    dusk_shader_data_t * tmp = ptr->_next;
    free(ptr->_name);
    free(ptr);
    ptr = tmp;
  }

  glDeleteProgram(this->program);
}

int dusk_shader_add_data(dusk_shader_t * this, const char * name, void * data, size_t size)
{
  assert(NULL != this);

  int                   index = 0;
  dusk_shader_data_t *  ptr   = NULL;
  dusk_shader_data_t ** next  = &_data_root;
  for (ptr = _data_root; NULL != ptr; ptr = ptr->_next)
  {
    if (0 == strcmp(ptr->_name, name))
    {
      break;
    }

    next = &ptr->_next;
    ++index;
  }

  if (NULL == ptr)
  {
    ptr = malloc(sizeof(dusk_shader_data_t));
    memset(ptr, 0, sizeof(dusk_shader_data_t));

    if (*next == NULL)
    {
      *next = ptr;
    }
    else
    {
      (*next)->_next = ptr;
    }

    ptr->_name = strdup(name);
    ptr->_ubo  = 0;
    ptr->_size = size;

    glUseProgram(this->program);

    glGenBuffers(1, &ptr->_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, ptr->_ubo);
    glBufferData(GL_UNIFORM_BUFFER, ptr->_size, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    GLuint data_index = glGetUniformBlockIndex(this->program, name);
    glUniformBlockBinding(this->program, data_index, _data_max_id);
    glBindBufferBase(GL_UNIFORM_BUFFER, _data_max_id, ptr->_ubo);

    DEBUG_INFO("Added Shader Data %s at index %d", name, _data_max_id);

    ++_data_max_id;
  }
  else
  {
    GLuint data_index = glGetUniformBlockIndex(this->program, name);
    glUniformBlockBinding(this->program, data_index, index);
    glBindBufferBase(GL_UNIFORM_BUFFER, _data_max_id, ptr->_ubo);

    dusk_shader_set_data(this, index, data);
    DEBUG_INFO("Updated Shader Data %s at index %d", ptr->_name, index);
  }

  return index;
}

void dusk_shader_set_data(dusk_shader_t * this, int index, void * data)
{
  assert(NULL != this);

  dusk_shader_data_t * ptr = _data_root;
  for (int i = 0; i < index; ++i)
  {
    if (NULL == ptr)
      break;
    ptr = ptr->_next;
  }

  if (NULL == ptr)
    return;

  glUseProgram(this->program);

  glBindBuffer(GL_UNIFORM_BUFFER, ptr->_ubo);
  GLvoid * data_ptr = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
  memcpy(data_ptr, data, ptr->_size);
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
    DEBUG_ERROR("Program log for %d exceeds maximum size allowed (%d > %d)", program, log_size,
                _MAX_LOG_SIZE);
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
    DEBUG_ERROR("Shader log for %d exceeds maximum size allowed (%d > %d)", shader, log_size,
                _MAX_LOG_SIZE);
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
    DEBUG_ERROR("Shader exceeds maximum size allowed (%ld > %d)", file_size, _MAX_SHADER_SIZE);
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
    DEBUG_ERROR("Failed to read whole shader file '%s' read %zu/%ld", filename, bytes_read,
                file_size);
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
