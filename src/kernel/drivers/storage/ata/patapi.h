#ifndef PATAPI_H
#define PATAPI_H

#include "ata.h"

ata_error_t PATAPI_identify_sync(uint8_t drive, void* buffer);

#endif