#include "dusk/texture.h"

#include <SOIL/SOIL.h>

bool dusk_texture_init(dusk_texture_t * this, const char * filename)
{
  this->_texture = SOIL_load_OGL_texture(filename,
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

  if( 0 == this->_texture )
  {
  	DEBUG_ERROR( "SOIL loading error: '%s'", SOIL_last_result() );
    return false;
  }

  return true;
}

void dusk_texture_term(dusk_texture_t * this)
{
}
