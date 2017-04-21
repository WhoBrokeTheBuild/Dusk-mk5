#ifndef DUSK_DEBUG_H
#define DUSK_DEBUG_H

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

// clang-format off

#if defined(NDEBUG)
#  define DEBUG_INFO(M, ...) do { } while(false)
#  define DEBUG_WARN(M, ...) do { } while(false)
#  define DEBUG_ERROR(M, ...) do { } while(false)
#else

#  define DEBUG_INFO(M, ...) \
          do { fprintf(stdout, "[INFO](%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__); } while (false)

#  define DEBUG_WARN(M, ...) \
          do { fprintf(stderr, "[WARN](%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__); } while (false)

#  define DEBUG_ERROR(M, ...) \
          do { fprintf(stderr, "[ERROR](%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__); } while (false)

#endif

// clang-format on

#endif // DUSK_DEBUG_H
