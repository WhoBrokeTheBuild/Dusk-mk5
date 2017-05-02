#include "dusk/dmf.h"

#include "debug.h"
#include <stdio.h>
#include <zlib.h>

dmf_Model_table_t dusk_load_dmf(const char * filename, unsigned char ** buffer)
{
  enum
  {
    CHUNK_SIZE  = 131072, // 128K
    FILEID_SIZE = 4,
  };

  uint64_t size;
  FILE *   file = NULL;
  char     fileid_buf[FILEID_SIZE + 1];
  fileid_buf[FILEID_SIZE] = '\0';
  const char fileid[] = "DMF-", zfileid[] = "DMFZ";

  int             zerr, flush;
  z_stream        zs;
  unsigned int    have;
  unsigned char   in[CHUNK_SIZE];
  unsigned char * out = NULL;

  file = fopen(filename, "rb");
  if (NULL == file)
  {
    DEBUG_ERROR("Failed to open %s", filename);
    goto error;
  }

  fread(fileid_buf, 1, FILEID_SIZE, file);

  if (0 == strcmp(fileid, fileid_buf))
  {
    fseek(file, 0, SEEK_END);
    size = (size_t)ftell(file) - FILEID_SIZE;
    fseek(file, FILEID_SIZE, SEEK_SET);

    out = (unsigned char *)malloc(size);
    fread((char *)out, 1, size, file);
  }
  else if (0 == strcmp(zfileid, fileid_buf))
  {
    fread((char *)&size, 1, sizeof(size), file);

    out = (unsigned char *)malloc(size);

    memset(&zs, 0, sizeof(zs));
    zs.zalloc = Z_NULL;
    zs.zfree  = Z_NULL;
    zs.opaque = Z_NULL;
    zerr      = inflateInit(&zs);
    if (Z_OK != zerr)
    {
      DEBUG_ERROR("Failed to initialize zlib: %d", zerr);
      goto error;
    }

    zs.avail_out = size;
    zs.next_out  = out;

    do
    {
      zs.avail_in = fread((char *)in, 1, CHUNK_SIZE, file);
      zs.next_in  = in;

      zerr = inflate(&zs, Z_NO_FLUSH);
      if (Z_STREAM_ERROR == zerr)
      {
        DEBUG_ERROR("Failed to decompress file: %d", zerr);
        goto error;
      }
    } while (Z_STREAM_END != zerr);

    inflateEnd(&zs);
  }
  else
  {
    DEBUG_ERROR("Unrecognized file format");
    goto error;
  }

  fclose(file);

  DEBUG_INFO("Read %s", filename);

  *buffer = out;

  if (0 != (zerr = dmf_Model_verify_as_root(*buffer, size)))
  {
    DEBUG_ERROR("Failed to verify buffer: %s", flatcc_verify_error_string(zerr));
    goto error;
  }
  return dmf_Model_as_root(*buffer);

error:
  free(out);
  fclose(file);
  inflateEnd(&zs);

  return NULL;
}
