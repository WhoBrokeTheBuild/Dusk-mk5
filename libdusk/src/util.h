#ifndef DUSK_UTIL_H
#define DUSK_UTIL_H

#ifdef HAVE_LIBGEN_H
#include <libgen.h>
#else

/* Copyright (c) 2009 Pierre Pronchery <khorben@defora.org> */
static inline char * basename(char * path)
{
  int i;

  if (path == NULL || path[0] == '\0')
    return "";
  for (i = strlen(path) - 1; i >= 0 && path[i] == '/'; i--)
    ;
  if (i == -1)
    return "/";
  for (path[i + 1] = '\0'; i >= 0 && path[i] != '/'; i--)
    ;
  return &path[i + 1];
}

static inline char * dirname(char * path)
{
  int i;

  if (path == NULL || path[0] == '\0')
    return ".";
  for (i = strlen(path) - 1; i >= 0 && path[i] == '/'; i--)
    ;
  if (i == -1)
    return "/";
  for (i--; i >= 0 && path[i] != '/'; i--)
    ;
  if (i == -1)
    return ".";
  path[i] = '\0';
  for (i--; i >= 0 && path[i] == '/'; i--)
    ;
  if (i == -1)
    return "/";
  path[i + 1] = '\0';
  return path;
}

#endif // HAVE_LIBGEN_H

#endif // DUSK_UTIL_H
