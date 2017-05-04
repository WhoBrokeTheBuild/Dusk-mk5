#ifndef DUSK_DMF_H
#define DUSK_DMF_H

#include <dusk/config.h>
#include <dusk/dmf_reader.h>
#include <dusk/dmf_verifier.h>
#include <string.h>

dmf_Model_table_t dusk_load_dmf(const char * filename, unsigned char ** buffer);

#endif // DUSK_DMF_H
