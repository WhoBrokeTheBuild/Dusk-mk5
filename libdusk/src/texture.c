#include "dusk/texture.h"

#include "debug.h"
#include <IL/il.h>
#include <IL/ilu.h>

GLuint dusk_texture_load(const char * filename)
{
  DEBUG_INFO("Loading image %s", filename);

  ILuint image   = 0;
  GLuint texture = 0;

  ilGenImages(1, &image);
  ilBindImage(image);

  if (!ilLoadImage(filename))
  {
    DEBUG_ERROR("Failed to load %s: %s", filename, iluErrorString(ilGetError()));
    goto error;
  }

  ILinfo ImageInfo;
  iluGetImageInfo(&ImageInfo);
  if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
  {
    iluFlipImage();
  }

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_WIDTH),
               ilGetInteger(IL_IMAGE_HEIGHT), 0,
               ilGetInteger(IL_IMAGE_FORMAT), // Format of image pixel data
               GL_UNSIGNED_BYTE, ilGetData());

  glGenerateMipmap(GL_TEXTURE_2D);

error:

  ilDeleteImages(1, &image);

  glBindTexture(GL_TEXTURE_2D, 0);

  return texture;
}
