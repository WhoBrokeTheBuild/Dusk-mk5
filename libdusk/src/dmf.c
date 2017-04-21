#include "dusk/dmf.h"

#include <stdio.h>
#include <zlib.h>

dmf_Model_table_t dusk_load_dmf(const char * filename, unsigned char ** buffer)
{
    const size_t CHUNK_SIZE = 131072; // 128K
    const int FILEID_SIZE = 4;

    uint64_t size;
    FILE * file = NULL;
    char fileid_buf[FILEID_SIZE + 1];
    fileid_buf[FILEID_SIZE] = '\0';
    const char fileid[] = "DMF-",
               zfileid[] = "DMFZ";

    int zerr,
        flush;
    z_stream zs;
    unsigned int have;
    unsigned char in[CHUNK_SIZE];
    unsigned char * out = NULL;

    file = fopen(filename, "rb");
    if (NULL == file)
    {
        fprintf(stderr, "Failed to open %s\n", filename);
        goto error;
    }

    fread(fileid_buf, 1, FILEID_SIZE, file);

    if (0 == strcmp(fileid, fileid_buf))
    {
        fseek(file, 0, SEEK_END);
        size = (size_t)ftell(file) - FILEID_SIZE;
        rewind(file);

        printf("%zu\n", size);

        out = (unsigned char *)malloc(size);
        fread((char *)out, 1, size, file);
    }
    else if (0 == strcmp(zfileid, fileid_buf))
    {
        fread((char *)&size, 1, sizeof(size), file);

        out = (unsigned char *)malloc(size);

        memset(&zs, 0, sizeof(zs));
        zs.zalloc = Z_NULL;
        zs.zfree = Z_NULL;
        zs.opaque = Z_NULL;
        zerr = inflateInit(&zs);
        if (Z_OK != zerr)
        {
            fprintf(stderr, "Failed to initialize zlib: %d\n", zerr);
            goto error;
        }

        zs.avail_out = size;
        zs.next_out = out;

        do
        {
            zs.avail_in = fread((char *)in, 1, CHUNK_SIZE, file);
            zs.next_in = in;

            zerr = inflate(&zs, Z_NO_FLUSH);
            if (Z_STREAM_ERROR == zerr)
            {
                fprintf(stderr, "Failed to decompress file: %d\n", zerr);
                goto error;
            }
        }
        while (Z_STREAM_END != zerr);

        inflateEnd(&zs);

    }
    else
    {
        fprintf(stderr, "Unrecognized file format\n");
        goto error;
    }

    fclose(file);

    printf("Read %s\n", filename);

    *buffer = out;

    if (0 != (zerr = dmf_Model_verify_as_root(*buffer, size)))
    {
        fprintf(stderr, "Failed to verify buffer: %s\n", flatcc_verify_error_string(zerr));
        goto error;
    }
    return dmf_Model_as_root(*buffer);

error:
    free(out);
    fclose(file);
    inflateEnd(&zs);

    return NULL;
}
