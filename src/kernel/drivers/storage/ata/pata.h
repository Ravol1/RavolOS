#ifndef PATA_H
#define PATA_H

#include <stdint.h>
#include <stdbool.h>
#include "pata_generic.h"


ata_error_t PATA_identify_sync(uint8_t drive, void* buffer);

#endif