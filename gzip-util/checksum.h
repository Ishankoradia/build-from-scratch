#ifndef CHECKSUM_H
#define CHECKSUM_H

#include <stddef.h>

void precompute_crc_table();
unsigned int compute_crc32(size_t bytes_read, unsigned char *input_bytes);

#endif