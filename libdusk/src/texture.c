#include "dusk/texture.h"

#include "debug.h"

GLuint dusk_texture_load(const char * filename)
{
    DEBUG_INFO("Loading image %s", filename);

    GLuint        texture = 0;
    int           mode;
    SDL_Surface * surf = NULL;

    surf = IMG_Load(filename);
    if (NULL == surf)
    {
        DEBUG_ERROR("SDL_LoadIMG failed: %s", SDL_GetError());
        goto error;
    }

    mode = GL_RGB;
    if (4 == surf->format->BytesPerPixel)
    {
        mode = GL_RGBA;
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, mode, surf->w, surf->h, 0, mode, GL_UNSIGNED_BYTE, surf->pixels);

    glGenerateMipmap(GL_TEXTURE_2D);

error:

    SDL_FreeSurface(surf);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}
